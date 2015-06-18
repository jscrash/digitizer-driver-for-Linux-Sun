/* DEC/CMS REPLACEMENT HISTORY, Element TO_POST_GROV.C */
/* *3    24-JUL-1991 17:02:53 JULIAN "(SPR 0) added more stuff for prev mods" */
/* *2     7-MAY-1991 10:08:50 JULIAN "(SPR 0) from .pc file" */
/* *1     7-MAY-1991 10:07:29 JULIAN "culture overlay" */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_POST_GROV.C */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_POST_GROV.PC */
/* *11    5-DEC-1990 12:11:17 JULIAN "(SPR 6278) transform nlist before displayin
g" */
/* *10   14-AUG-1990 16:45:50 GILLESPIE "(SPR 2001) Use LINE_GC" */
/* *9    19-JUL-1990 19:22:30 GILLESPIE "(SPR 1) Change esi_to.h to esi_to.x" */
/* *8    18-JUL-1990 18:45:14 JESSIE "(SPR 1) change include files" */
/* *7    29-MAR-1990 14:34:29 JULIAN "(SPR 0) fix text size problem" */
/* *6    26-MAR-1990 23:25:39 JULIAN "(SPR 5095) text bundle mods" */
/* *5    26-MAR-1990 09:48:15 GILLESPIE "(SPR 1) Clean up select logic" */
/* *4    22-MAR-1990 10:37:56 JULIAN "(SPR 5095) change on external variable, add
 township labeling & text clip" */
/* *3    21-MAR-1990 16:28:09 JULIAN "(SPR 0) more changes" */
/* *2    16-MAR-1990 09:11:21 JULIAN "(SPR 5095) culture" */
/* *1    16-MAR-1990 08:50:57 JULIAN "culture overlay" */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_POST_GROV.PC */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_am.h"

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_TO_X
#include "esi_to.x"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_LY_H
#include "esi_ly.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT to_post_grov(TO_STRUCT *values)
#else
publicdef INT to_post_grov(values)
TO_STRUCT *values;
#endif
    {
    INT status;
    INT iline;                /* number of lines to process */
    COORD xmin, xmax, ymin, ymax;
    NLIST_HEADER slist = (NLIST_HEADER)0;   /* temporary select list pointer */
    NLIST_HEADER process_list = (NLIST_HEADER)0;    /* final list to process */
    CHAR *select_phrases;
    CHAR temp_text[256];
    BOOL free_phrase = FALSE;
    INT n_items,len;
    PROJECTION_ID map_projection;
    PROJECTION_ID project_projection;
    PROJECTION_STRUCTURE *project_data;
    PROJECTION_STRUCTURE *map_data;
    MAP_STRUCTURE       *pmap;    
    PROJECT_NAME project_name;
    INT n_lines,i,bundle_id,x;
    size_t n_bytes;
    LINE_GC line;
    TEXT_GC text; 
    BOOL trans_pts = FALSE;
    NLIST_HEADER nlist_out;
    static UINT ln_col_list[2] = { 1,2 };
    static UINT tx_col_list[2] = { 3,4 };


    /*--------------------------------------------------------------------*/
    /* nlist value list                                                   */
    /* The following values are set by the SELECT SLIST SEISMIC SUBSYSTEM */
    /* and must not be dicked with                                        */
    /*--------------------------------------------------------------------*/
    
    /**************************************************************************/
    
			     /*	get map structure for map corners	    */
			     /*	coordinates				    */
    status = mp_get_current_map(&pmap);
    if(status != SUCCESS)
	return status;

	/* get project name for oracle statement */
    qs_inq_c(QS_PROJECT_NAME,project_name,(INT *)0);

    status = SUCCESS;
    slist = (NLIST_HEADER)0;
    
    /* * IF SELECT LIST PRESENT, READ IT * */
    /* ----------------------------------- */
    if (values->select_list != (CHAR *)0)
	{
	status = sl_read_list (LYNX_DATA_TYPE, values->select_list,&slist);
	if(status != SUCCESS)
	    {
	    am_message(AM_ALERT,mg_message (status));
	    return status;
	    }
	} 

/* * INTERSECT WITH MAP WINDOW AND SELECT PHRASE * */ 
/* ----------------------------------------------- */
    xmin = pmap->lower_left_xy.x;
    ymin = pmap->lower_left_xy.y;
    xmax = pmap->upper_right_xy.x;
    ymax = pmap->upper_right_xy.y;
    qs_inq_c (QS_MAP_PROJECTION, map_projection, (INT *)0);
    qs_inq_c (QS_PROJECT_PROJECTION, project_projection, (INT *)0);
    if(ARE_DIFFERENT(project_projection,map_projection)) 
	{	
	ct_get_projection (project_projection, &project_data);
 	ct_get_projection (map_projection, &map_data);
        ct_transform_point (xmin, ymin, map_data, 
			    &xmin, &ymin, project_data);
	ct_transform_point (xmax, ymax, map_data,
			    &xmax, &ymax, project_data);
	trans_pts= TRUE;
        }
    
    /* get new select list */
    sprintf (temp_text, 
       "MAXX > %lf AND MAXY > %lf AND \
MINX < %lf AND MINY < %lf AND DATA_TYPE = '%s'",
       xmin,ymin,xmax,ymax,values->object_type); 
    

    if (IS_STRING (values->select_by_phrase))
        {
			     /*	allocate new space for select by phrase.
			     	When ov_chk_where is called, it will throw
			     	away the old string and create a new one.
			     	Values->select_by_phrase is just a pointer to
			     	one of the structure in map def in which we
			     	do not want to discard the contents of the
			     	string					    */

        len = strlen (values->select_by_phrase);
        select_phrases = (CHAR *)TALLOC (len + strlen (temp_text) + 10);
        strcpy(select_phrases,values->select_by_phrase);
        ov_chk_where(&values->select_by_phrase);
        sprintf (select_phrases, "%s AND (%s)",
                 values->select_by_phrase, temp_text);
        free_phrase = TRUE;
        }
    else
        {
        select_phrases = temp_text;
        }

    status = sl_select (slist, "LYNX_CULTURE", select_phrases, &process_list,
 	&n_items); 

#if NEED_TO_FREE 
    if (free_phrase) 
	{
        tc_free (select_phrases);
        }
#endif
    
    /* * CHECK FOR GOOD SELECT * */
    /* ------------------------- */
    
    if (status != SUCCESS OR n_items == 0)
	goto error;


    /** LOAD SELECT LIST TO TEMP TABLE **/
    sprintf(temp_text,"DRAW_%s",values->object_type);
    status = or_load_lynx(temp_text,process_list);
    if(status != SUCCESS)
      goto error;

    /* * DRAW GRID **/
    status = to_draw_object(values->suppress_lines,values->suppress_text,
			    &n_lines);
    if(status != SUCCESS)
	goto error;

wmsallocate(&x);
if(! values->suppress_lines)
    {
    for(i=0;i<n_lines;i++)
        {
        status=nl_inq_nlist_user_struct(to_line_nlists[i],sizeof(INT),
            (VOIDPTR)&bundle_id,&n_bytes);
        status=ly_get_arc_bundle(NULL,0,bundle_id,&line);
        if ( values->line.color >= 0 )
            line.line.color = values->line.color;
        if ( values->line.style >= 0 )
            line.line.style = values->line.style;
        if ( values->line.width >= 0 )
            line.line.width = values->line.width;
        if(trans_pts)
            {
            status=ct_transform_nlist(to_line_nlists[i],project_data,
                &nlist_out,map_data,ln_col_list,0.0);
            if(status EQUALS SUCCESS)
                {
                nl_free_nlist(to_line_nlists[i]);
                to_line_nlists[i]=nlist_out;
                }
            }
        nl_invoke_nlist_method(to_line_nlists[i],NL_DRAW_METHOD,
            (VOIDPTR)&line);
        }
    }
if(! values->suppress_text)
    {
    text.text_color=values->text_color;
    text.char_height=values->text_height;
    text.char_spacing=values->text_spacing;
    text.text_font=values->text_font;
    if(trans_pts)
        {
        status=ct_transform_nlist(to_text_nlist,project_data,
            &nlist_out,map_data,tx_col_list,0.0);

        if(status EQUALS SUCCESS)
            {
            nl_free_nlist(to_text_nlist);
            to_text_nlist=nlist_out;
            }
        }
    nl_invoke_nlist_method(to_text_nlist,NL_DRAW_METHOD,(VOIDPTR)&text);
    }

wmsclose(x);

error:
if(to_line_nlists != (NLIST_HEADER *)0)
    {
    for (i = 0; i < n_lines; i++)
    {
    nl_free_nlist(to_line_nlists[i]);
    }
    tc_free(to_line_nlists);
    }


    if (slist != (NLIST_HEADER)0)
	{
        nl_free_nlist (slist);
        }

    if (process_list IS_NOT_EQUAL_TO (NLIST_HEADER)0)
	{
        nl_free_nlist (process_list);
        }

    return status;
    }
/* END:     */
