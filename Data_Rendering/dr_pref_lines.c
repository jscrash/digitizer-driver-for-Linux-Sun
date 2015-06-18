/* DEC/CMS REPLACEMENT HISTORY, Element DR_PREF_LINES.C*/
/* *3    15-OCT-1990 23:10:36 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  1B1  12-OCT-1990 16:32:42 GILLESPIE "Merge Ernie Deltas"*/
/* *2    14-AUG-1990 18:00:48 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *1    19-JUN-1989 12:47:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PREF_LINES.C*/
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

/******************************************************************************

NAME:           dr_pref_lines 

DESCRIPTION:    Get user preferences for line graphic attributes. 


     Input:     dr_data - the standard data structure

     Output:    dr_data - Updated 

     Status:    SUCCESS
     
     TODO:      1)  Make polygon_fill_color independently accessible to user.
                
******************************************************************************/

#include    "esi_dr.h"
#include    "esi_tg.h"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define WM_CANCEL 1

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

publicdef INT dr_pref_lines(dr_data)
DR_DATA *dr_data;
{
    INT status;
    COLOR_INDEX color;
    INDEX style;
    FLOAT width;
    static COLOR_INDEX erase_color = (COLOR_INDEX)-1;

    
    if (dr_data->current_line != (LINE *) NULL)
        { 
	color = dr_data->current_line->line.color;
     	style = dr_data->current_line->line.style;  
      	width = dr_data->current_line->line.width; 
        }
    else
        {
	color = dr_data->default_line.line.color;
	style = dr_data->default_line.line.style;
	width = dr_data->default_line.line.width;
	}
	
    tg_ask_polyline_attributes( &color, &style, &width, &status );

    if(status != WM_CANCEL)
        {
        if(dr_data->current_line != (LINE *)NULL)
            {
            /* --------------------------------- */  
            /* Erase the old line before drawing */
            /* with the new color, width, style  */
            /* --------------------------------- */ 
            if ( erase_color < 0 )
                {     
                lu_text_to_index ("COLOR", &erase_color, "BACKGROUND"); 
                }
            dr_data->current_line->line.color = erase_color;        
            dr_data->current_line->polygon_fill_color = erase_color;
            mm_draw_line ( dr_data->current_line, TRUE );
    
            dr_data->object_changed = TRUE; 
            dr_data->current_line->line.color = color;
            dr_data->current_line->line.style = style;  
            dr_data->current_line->line.width = width;  
            dr_data->current_line->polygon_fill_color = color;
            
            /* ----------------------------------------- */ 
            /* Redraw the line to reflect new attributes */
            /* ----------------------------------------- */ 

            status = dr_redraw_line(dr_data);
            } 

        /* ------------------------------------------------ */ 
        /* Set defaults to reflect current user preferences */
        /* ------------------------------------------------ */ 

		dr_data->default_line.line.color = color;
		dr_data->default_line.line.style = style;
		dr_data->default_line.line.width = width;
		dr_data->default_line.polygon_fill_color = color;  
        }

    return SUCCESS;
}
