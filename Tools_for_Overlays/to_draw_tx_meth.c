/* DEC/CMS REPLACEMENT HISTORY, Element TO_DRAW_TX_METH.C */
/* *11    6-NOV-1990 15:31:00 JULIAN "(SPR 6156) township & section labels mods" */
/* *10   18-JUL-1990 18:14:20 JESSIE "(SPR 1) change include files" */
/* *9    28-MAY-1990 09:17:32 SCD "(SPR 1) Add management of text alignment." */
/* *8    29-MAR-1990 14:33:23 JULIAN "(SPR 0) fix text size problem" */
/* *7    26-MAR-1990 23:25:07 JULIAN "(SPR 5095) text bundle mods" */
/* *6    25-MAR-1990 22:45:39 SCD "(SPR 1) First round of cleanup -- much more to do" */
/* *5    23-MAR-1990 16:34:46 JULIAN "(SPR 0) remove debugging analyzer" */
/* *4    23-MAR-1990 15:32:06 JULIAN "(SPR 5095) change method in text clip" */
/* *3    21-MAR-1990 16:27:47 JULIAN "(SPR 0) more changes" */
/* *2    16-MAR-1990 09:13:19 JULIAN "(SPR 5095) culture" */
/* *1    16-MAR-1990 08:53:39 JULIAN "culture overlay" */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_DRAW_TX_METH.C */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_DRAW_TX_METH.C*/
/* *1    13-AUG-1990 17:16:30 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element TO_DRAW_TX_METH.C*/
/******************************************************************************

                Copyright FINDER Graphics Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  && MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            FINDER Graphics Systems, Inc.
            201 Tamal Vista
            Corte Madera, CA 94925
            415/927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/





/******************************************************************************

    FUNCTION :      TO_DRAW_TEXT_METHOD

    DESCRIPTION :   Clip and draw a text nlist object.
                    
                    Text bundle_id = 0 for the nlist is the
                    default bundle.  Overriding of the bundle
                    attributes is accomplished by the param structure.
                    
    INPUT:          NLIST_HEADER    nlist
                    TEXT_GC         *param
                                    
    INITIAL
    CONDITIONS:     The user of this method is responsible for:
    
                    1)  cartographic transform of the nlist when loaded
                    2)  allocation/free of the text nlist
                    3)  closing of nlist file if read exclusive is used
                    4)  ensuring that the text bundle is defined
                    5)  ensuring that the NL_CLIP_METHOD is registered
                        NOTE: If the user of this method wants to disable 
                        clipping, set the function to NUL_FUNCTION_PTR
                    6)  The convention for text sizing in the bundle
                        is PLOTTER INCHES

    AUTHOR :        Purna Tjahjana (Feb 8,1990)

********************************************************************************/

#include "esi_ly.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_NL_METH_HDR_H
#include "esi_nl_meth_hdr.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"                      /* window manager */
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"                      /* mapping */
#endif

#ifndef ESI_MD_DEFS_H
#include "esi_md_defs.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_CT_MSG_H
#include "esi_ct_msg.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_TO_H
#include "esi_to.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

publicdef INT to_draw_text_method (nlist, param)
NLIST_HEADER nlist;
VOIDPTR param;
    {
    DOUBLE node_x, node_y;
    DOUBLE text_up_x, text_up_y;
    DOUBLE min_point[2], max_point[2];
    TEXT_GC *t_gc = (TEXT_GC *)param;
    MAP_STRUCTURE *mapdef;  /* current map structure */
    INT status,n_string;
    CHAR tmp_text_str[256];
    CHAR *tmp_str_end, *tmp_str_begin;
    GKS_REAL delta_x, delta_y,height;
    UINT table_num;
    UINT currentPt;
    INT total_tables;
    INT rowsInTable;
    UINT nrows;
    TEXT_GC text;
    CHAR text_string[84];
    INT bundle_id;
    NLM_CLIP_HEADER clip;
    size_t dummy;
    struct 
        {
        GKS_REAL x, y;
        } box_base;

    UINT ncol = 5;
    static UINT col_list[5] = 
        {
        2, 3, 4, 8, 9
        };
    VOIDPTR value_list[5];

    /* * GET THE MAP * */
    status = mp_get_current_map (&mapdef);
    if (status != SUCCESS)
        {
        return status;
        }


    /* Clip the text n-List if it's not township labels */

    status = nl_inq_nlist_string(nlist,NL_NLIST_NAME,sizeof text_string,
	     (VOIDPTR)text_string,&dummy);
    if(status != SUCCESS OR 
       ARE_DIFFERENT(text_string,"TOWNSHIP_LABELS"))
       {
       clip.Col_List[0] = 3;
       clip.Col_List[1] = 4;
       clip.minList[0] = mapdef->lower_left_xy.x;
       clip.minList[1] = mapdef->lower_left_xy.y;
       clip.maxList[0] = mapdef->upper_right_xy.x;
       clip.maxList[1] = mapdef->upper_right_xy.y;

       status = nl_invoke_nlist_method (nlist, NL_CLIP_METHOD, &clip);
       if (status == SUCCESS)
           {
           nl_free_nlist (nlist);
           nlist = clip.outNlist;
           }
       else
          {
          nl_free_nlist(nlist);
          return SUCCESS;
          }
       }
    /* --------------------------------------------*/
    /* Cartographic transformations are ALWAYS the */
    /* responsibility of the database interface    */
    /* code -- i.e., read the nlist, convert the   */
    /* nlist the instant it comes into memory      */
    /* --------------------------------------------*/
    /* DRAW NLIST */

    value_list[0] = (VOIDPTR)text_string;
    value_list[1] = (VOIDPTR) & node_x;
    value_list[2] = (VOIDPTR) & node_y;
    value_list[3] = (VOIDPTR) & text_up_x;
    value_list[4] = (VOIDPTR) & text_up_y;

    nl_inq_nlist_int (nlist, NL_NLIST_NTABLES, &total_tables);

    for (table_num = 1; table_num <= total_tables; table_num++)
        {
        status = nl_set_current_row (nlist, 1, table_num);
        if (status < 0)
            continue;

	status = nl_inq_table_user_struct (nlist, table_num, sizeof bundle_id,
                                           &bundle_id, &dummy);
	if(status != SUCCESS)
	    bundle_id=0;
	
        status = ly_get_text_bundle (NULL, 0, bundle_id, &text);

        /* ------------------------------ */
        /* Override the object graphic    */
        /* attributes with those supplied */
        /* by the invoker of this method  */
        /* ------------------------------ */

        if (t_gc != (TEXT_GC *)0)
            {
            if (t_gc->text_color >= 0)
                text.text_color = t_gc->text_color;
            if (t_gc->char_height >= 0)
                text.char_height = t_gc->char_height;
            if (t_gc->char_spacing >= 0)
                text.char_spacing = t_gc->char_spacing;
            if (t_gc->text_font >= 0)
                text.text_font = t_gc->text_font;
            if (t_gc->string_v_just >= 0)
                text.string_v_just = t_gc->string_v_just;
            if (t_gc->string_h_just >= 0)
                text.string_h_just = t_gc->string_h_just;
            }


        /* ---------------------------- */
        /* Give GKS the text attributes */
        /* ---------------------------- */
        /*   Text path                  */
        if (text.text_color < 0)
            status = lu_text_to_index ("COLOR", (INT *)&text.text_color, "NORMAL");

        wm_gstxp (text.text_path);

        /*   Text height converted to world coord       */
        wm_gschh (text.char_height);

        /*   Character char spacing         */
        wm_gschsp (text.char_spacing);

        /*   Character expansion factor */
	wm_gschxp (1.0);

        /*   Text Font & Precision              */
        wm_gstxfp (text.text_font, text.text_precision);

        /*   Text Alignment                 */
        wm_gstxal (text.string_h_just, text.string_v_just);

        /*   Text Color                 */
        wm_gstxci (text.text_color);
        status = nl_inq_table_int (nlist, table_num, NL_TABLE_NROWS,
                                   &rowsInTable);
        if (status < 0 OR rowsInTable == 0)
            continue;

        for (currentPt = 1; currentPt <= rowsInTable && status >= 0;
             currentPt++)
	    {
	    status = nl_get_rows (nlist, 1, ncol, col_list, value_list,&nrows);
	    /*   Text Up Vector                 */
	    wm_gschup (text_up_x, text_up_y);

	    /* Test for and remove any carriage return */
	    strcpy(tmp_text_str,text_string);
	    tmp_str_end = strchr (text_string, '\n');

	    if (tmp_str_end != NULL)
		{
		*tmp_str_end++ = '\0';
		}

	    if (tmp_str_end EQUALS NULL)
		{
	        wm_gtx (node_x, node_y, text_string);
  		continue;
		}

	    /* The following code is only reached if the current text
		string contains carriage returns... */

	    /*  NOTE :
		To calculate the location of the text, it is always assumed
		to work for specified text allignment and text up vector, until
		further modification needed to make this algorithm work better
		(purna - 10/17/90) */

	    /*   Text Alignment                 */
	    wm_gstxal (GACENT,GATOP);

	    /*   Text Up Vector                 */
	    wm_gschup (0.0,1.0);

/* this part of the code is temporarily not functioning since we always 
    assume that text_up_x = 0.0 and text_up_y = 1.0 */
#if 0
            /*   Calculate delta x and delta y for multiple lines */
            if (text_up_x == 0.0)
                angle = PI / 2.0;
            else
                angle = atan (text_up_y / text_up_x);

            delta_x = (FLOAT)cos (angle) * text.string_spacing;
            delta_y = (FLOAT)sin (angle) * text.string_spacing;

            delta_x = (delta_x >= 0.0) ? delta_x : delta_x * (-1);
            delta_y = (delta_y >= 0.0) ? delta_y : delta_y * (-1);
#endif
	    /* calculate how many string to calculate height of strings */
	    strcpy(text_string,tmp_text_str);
	    tmp_str_end = text_string;
	    n_string=0;
	    while (tmp_str_end != NULL)
		{
		tmp_str_begin = tmp_str_end;
		tmp_str_end = strchr (tmp_str_end, '\n');
		if (tmp_str_end != NULL)
		    {
		    *tmp_str_end++ = '\0';
		    }
		n_string++;
		}
	    height = ((1.5 * n_string) - 0.5) * text.char_height;

	    /* calculate delta y */
	    delta_y = 1.5 * text.char_height;

	    /*   Copy string to temporary location      */
	    tmp_str_end = tmp_text_str;

	    /* determine where to start displaying text */
	    box_base.x = node_x;
	    box_base.y = (height / 2) + node_y;

	    /*   Loop on every line feed            */
	    while (tmp_str_end != NULL)
		{

		/*   find line feed and NULL it out.        */
		tmp_str_begin = tmp_str_end;
		tmp_str_end = strchr (tmp_str_end, '\n');
		if (tmp_str_end != NULL)
		    {
		    *tmp_str_end++ = '\0';
		    }

		/*   Set new begining               */
		/*   Set position for next text string      */
		box_base.y -= delta_y;

/* this code is temporarily not functioning since we always assume that
   text_up_x = 0.0 and text_up_y = 1.0 */
#if 0
		if (text_up_x >= 0.0 && text_up_y == 0.0)
		    {
		    box_base.x -= delta_x;
		    }
		else if (text_up_x < 0.0 && text_up_y == 0.0)
		    {
		    box_base.x += delta_x;
		    }
		else if (text_up_x == 0.0 && text_up_y >= 0.0)
		    {
		    box_base.y -= delta_y;
		    }
		else if (text_up_x == 0.0 && text_up_y < 0.0)
		    {
		    box_base.y += delta_y;
		    }
		else if (text_up_x > 0.0 && text_up_y > 0.0)
		    {
		    box_base.x -= delta_x;
		    box_base.y -= delta_y;
		    }
		else if (text_up_x < 0.0 && text_up_y < 0.0)
		    {
		    box_base.x += delta_x;
		    box_base.y += delta_y;
		    }
		else if (text_up_x > 0.0 && text_up_y < 0.0)
		    {
		    box_base.x -= delta_x;
		    box_base.y += delta_y;
		    }
		else if (text_up_x < 0.0 && text_up_y > 0.0)
		    {
		    box_base.x += delta_x;
		    box_base.y -= delta_y;
		    }
#endif
		/*   Post the text                  */
		wm_gtx (box_base.x, box_base.y, tmp_str_begin);
		}
	    }
        }

    return status;
    }

