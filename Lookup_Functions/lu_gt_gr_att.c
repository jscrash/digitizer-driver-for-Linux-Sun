/* DEC/CMS REPLACEMENT HISTORY, Element LU_GT_GR_ATT.C */
/* *6    16-FEB-1990 15:05:43 GORDON "(SPR 0) use the right tcp (snames) on cancel" */
/* *5    26-JAN-1990 08:56:20 GILLESPIE "(SPR 1) change out_thick_str to out_style_str - fixes minor logic error" */
/* *4    28-DEC-1989 19:51:46 SCD "(SPR 100) Fix thickness attribute bugs." */
/* *3    20-SEP-1989 14:55:37 GORDON "(SPR 200) Get in sync with Gulf" */
/* *2    14-SEP-1989 18:21:11 GORDON "(SPR -1) checkin from gulf" */
/* *1    14-SEP-1989 17:34:09 GORDON "a" */
/* DEC/CMS REPLACEMENT HISTORY, Element LU_GT_GR_ATT.C */

/******************************************************************************

		Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF 

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work) 


******************************************************************************/

#include "esi_lu.h"
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef LU_GET_GRAPHIC_ATTR_CDLG_RH
#include "lu_get_graphic_attr_cdlg.rh"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

/****************************************************************************/
/*                                                                          */
/*  Routine:      LUP_GET_GRAPHIC_ATTRIBUTES                                */
/*                                                                          */
/*  Copyright:    November 1987                                             */
/*                Exploration Systems, Inc.                                 */
/*                San Francisco, CA  94105                                  */
/*                                                                          */
/*  Author:       Douglas J. Canfield                                       */
/*                                                                          */
/*  Purpose:      This subroutine displays current selections for graphics  */
/*                attributes of an object and allows the user to click on   */
/*                entries in three selector boxes to change line style,     */
/*                thickness and color.                                      */
/*                                                                          */
/*  Parameters:   prompt       (input) The prompt string to be displayed.   */
/*                in_thick     (input) The current integer thickness code.  */
/*                in_style     (input) The current integer line style code. */
/*                in_color     (input) The current integer color code.      */
/*                out_thick   (output) The returned thickness code.         */
/*                out_style   (output) The returned line style code.        */
/*                out_color   (output) The returned color code.             */
/*                out_thick_str (output) The returned thickness text string.*/
/*                out_style_str (output) The returned line style text str.  */
/*                out_color_str (output) The returned color text string.    */
/*                                                                          */
/*  Return Codes: SUCCESS if attributes were taken from the selector boxes. */
/*                        This will occur if the user clicks Accept Changes */
/*                        (required if any of the input codes are invalid). */
/*                                                                          */
/*                CANCEL  if user clicks Ignore Changes and the input codes */
/*                        are all valid codes (found in the n-lists).       */
/*                                                                          */
/*                FAIL    if any of the attributes n-list have less than    */
/*                        one point.  This is most likely due to a problem  */
/*                        with the EXPRESSION_CODES table.  (An entry will  */
/*                        be made in the error log.)                        */
/*                                                                          */
/*    8/28/89 Gordon Shannon - rewrite.                                     */
/****************************************************************************/

publicdef INT lup_get_graphic_attributes( prompt,in_thick,in_style,in_color,out_thick, 
              out_style,out_color,out_thick_str,out_style_str,out_color_str )
CHAR *prompt;
INT   in_thick;
INT   in_style;
INT   in_color;
INT  *out_thick;
INT  *out_style;
INT  *out_color;
CHAR *out_thick_str;
CHAR *out_style_str;
CHAR *out_color_str;
{
    static CHAR **cnames;
    static CHAR **snames;
    static CHAR **tnames;
    static BOOL firstTime = TRUE;
    static INT ncolors;          /* Number of entries in color table.   */
    static INT nthicks;          /* Number of entries in thickness tbl. */
    static INT nstyles;          /* Number of entries in line style tbl.*/

    INT status;
    INT ii;
    INT rc;
    INT nselected;
    INT curr_code;                   /* The current code while searching.   */
    INT color_num;                   /* The position of in_color in table.  */
    INT thick_num;                   /* The position of in_thick in table.  */
    INT style_num;                   /* THe position of in_style in table.  */
    INT selected[1];
    INT class;
    INT id;                          /* The rs id which generated an event. */
    INT cdlgid;                      /* The dialog id.                      */
    INT timeout = 0;
    INT item;
    BOOL done;                       /* Signal a correct event recieved.     */
    BOOL found;                      /* Signal if table search succeeded.    */
    BOOL bad_in_flag;                /* TRUE if original codes not in table. */


                                /*  Build static TCPs for selector boxes.*/
    if (firstTime)
        {
        lu_get_tcp( "COLOR", &cnames, &ncolors );
        lu_get_tcp( "THICKNESS", &tnames, &nthicks );
        lu_get_tcp( "LINE_STYLE", &snames, &nstyles );
    
        if ( ncolors < 1 OR nthicks < 1 OR nstyles < 1 )
            {
            return FAIL;
            }
        firstTime = FALSE;
        }

    fi_push();	                      /* disable the rest of the world    */

                                      /* Define the dialog box.           */

    am_define_resource(AM_DIALOG, "lu_get_graphic_attr_cdlg",
                   LU_SELECT_GRAPHIC_ATTR, NULL_FUNCTION_PTR, &cdlgid, NULL);

    wiscdt(cdlgid, LU_GR_ATTR_PROMPT, prompt);   /* set the dialog prompt */

                                      /* Load the three selector boxes.   */

    wi_set_selections(cdlgid,LU_GR_ATTR_COLOR_SELECTOR,LU_GR_ATTR_COLOR_SCROLL,
                                  ncolors, 1, cnames);
    wi_set_selections(cdlgid,LU_GR_ATTR_THICK_SELECTOR,LU_GR_ATTR_THICK_SCROLL,
                                  nthicks, 1, tnames);
    wi_set_selections(cdlgid,LU_GR_ATTR_STYLE_SELECTOR,LU_GR_ATTR_STYLE_SCROLL,
                                  nstyles, 1, snames);

                                /* Initialize to search tables (nl's)     */
    bad_in_flag = FALSE;

                                /* Find the number of the item in the     */
                                /* selector list which corresponds with   */
                                /* the current color (in_color).          */

    for (ii=0,found=FALSE; cnames[ii]!=NULL && !found; ++ii)
        {
        status = lu_text_to_index( "COLOR", &curr_code, cnames[ii] );
        if (status == SUCCESS)
            { 
            if ( curr_code == in_color )
                {
                wi_set_default_sel( cdlgid, LU_GR_ATTR_COLOR_SELECTOR, ii+1 );
                wiscdr( cdlgid, LU_GR_ATTR_COLOR_SCROLL, 0, ncolors - 8 );
                wiscdv( cdlgid, LU_GR_ATTR_COLOR_SCROLL, ii+1 );
                found = TRUE;
                color_num = ii;   /* Set this in case of Ignore */
                }
            }
        }
        
    if ( found != TRUE)  
        bad_in_flag = TRUE;

                                /* Find the number of the item in the     */
                                /* selector list which corresponds with   */
                                /* the current line style. (in_style)     */

    for (ii=0,found=FALSE; snames[ii]!=NULL && !found; ++ii)
        {
        status = lu_text_to_index( "LINE_STYLE", &curr_code, snames[ii] );
        if (status == SUCCESS)
            { 
            if ( curr_code == in_style )
                {
                wi_set_default_sel( cdlgid, LU_GR_ATTR_STYLE_SELECTOR, ii+1 );
                found = TRUE;
                style_num = ii;   /* Set this in case of Ignore */
                }
            }
        }
        
    if ( found != TRUE)  
        bad_in_flag = TRUE;

                                /* Find the number of the item in the     */
                                /* selector list which corresponds with   */
                                /* the current thickness (in_thick).      */

    for (ii=0,found=FALSE; tnames[ii]!=NULL && !found; ++ii)
        {
        status = lu_text_to_index( "THICKNESS", &curr_code, tnames[ii] );
        if (status == SUCCESS)
            { 
            if ( curr_code == in_thick )
                {
                wi_set_default_sel ( cdlgid, LU_GR_ATTR_THICK_SELECTOR, ii+1 );
                found = TRUE;
                thick_num = ii;   /* Set this in case of Ignore */
                }
            }
        }
        
    if ( found != TRUE)  
        bad_in_flag = TRUE;
 

                                 /* if the input codes were bad, force a   */
                                 /* table look up of the displayed attr.   */
    if ( bad_in_flag == TRUE )
       {
       wiencdi(cdlgid, LU_GR_ATTR_IGNORE, OFF);
       }
    else
       {
       wiencdi(cdlgid, LU_GR_ATTR_IGNORE, ON);
       }

    wi_open_dialog(cdlgid);

					    /* process event loop */
    done = FALSE;

    while (NOT done)
	{
	wi_request_event(timeout, &class, &id, &item);
        if ( id == cdlgid )
           {
           switch (item) {
	      case LU_GR_ATTR_IGNORE:
                   done   = TRUE;
                   rc     = CANCEL;
                   break;
              case LU_GR_ATTR_ACCEPT:
                   done = TRUE;
                   rc   = SUCCESS;
                   break;
              default:
                   break;
              }
	   }
	}



    if ( rc == SUCCESS )
	{
                                    /* Look up the style code and string. */

	wi_query_selections(cdlgid, LU_GR_ATTR_STYLE_SELECTOR,
                                    &nselected, selected);
        strcpy( out_style_str, snames[selected[0] - 1] );
        status = lu_text_to_index( "LINE_STYLE", out_style, out_style_str );
        
                                    /* Look up the thick code and string. */

	wi_query_selections(cdlgid, LU_GR_ATTR_THICK_SELECTOR,
                                    &nselected, selected);
        strcpy( out_thick_str, tnames[selected[0] - 1] );
        status = lu_text_to_index( "THICKNESS", out_thick, out_thick_str );
        
                                    /* Look up the color code and string. */

	wi_query_selections(cdlgid, LU_GR_ATTR_COLOR_SELECTOR,
                                    &nselected, selected);
	strcpy( out_color_str, cnames[selected[0] - 1] );
	status = lu_text_to_index( "COLOR", out_color, out_color_str );
	}

    else
        {                                /* Return the original codes.      */
        strcpy( out_style_str, snames[style_num] );
        status = lu_text_to_index( "LINE_STYLE", out_style, out_style_str );
        strcpy( out_thick_str, tnames[thick_num] ); 
        status = lu_text_to_index( "THICKNESS", out_thick, out_thick_str );
	strcpy( out_color_str, cnames[color_num] );
	status = lu_text_to_index( "COLOR", out_color, out_color_str );
	}    
   am_release_resource(cdlgid);
   fi_pop();
   return rc;
}
