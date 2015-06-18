/* DEC/CMS REPLACEMENT HISTORY, Element DR_PREF_POLY.C*/
/* *4    15-OCT-1990 23:13:14 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  2B1  15-OCT-1990 16:56:05 GILLESPIE "Merge Ernie deltas"*/
/* *3    15-OCT-1990 12:09:11 GILLESPIE "(SPR 1) Change line_color to line.color"*/
/* *2    12-OCT-1990 16:34:28 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *1    19-JUN-1989 12:47:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PREF_POLY.C*/
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

NAME:           dr_pref_polygon

DESCRIPTION:    Set polygon mode
                Allow user to set closed non-filled polygons
                in one step, by toggling the polygon menu item ON or OFF

    Input:      dr_data - Standard data structure

    Output:     dr_data - Updated

    Status:     SUCCESS
                    

******************************************************************************/

#include "esi_dr.h"
#include "esi_lu.h"

#if USE_PROTOTYPES
publicdef INT dr_pref_polygon(DR_DATA *dr_data)
#else
publicdef INT dr_pref_polygon(dr_data)
DR_DATA *dr_data;
#endif
{
    BOOL poly_pref, fill_pref;
    COLOR_INDEX erase_color, old_color;

    dr_data->object_changed = TRUE;
    poly_pref = !dr_data->line_poly;
    fill_pref = FALSE;

    /* --------------------------------------------- */
    /* If we are changing from polygon to line, we   */
    /* erase the old line by redrawing in background */
    /* --------------------------------------------- */

    if ( !poly_pref && dr_data->current_line != (LINE *) NULL )
        {
        old_color = dr_data->current_line->line.color;
        lu_text_to_index ("COLOR", (INDEX *)&erase_color, "BACKGROUND");
        dr_data->current_line->line.color = erase_color;
        dr_data->current_line->polygon_fill_color = erase_color;
        mm_draw_line ( dr_data->current_line, TRUE );       
	dr_data->current_line->line.color = old_color;
        dr_data->current_line->polygon_fill_color = old_color;
        }

    dr_set_poly_pref(dr_data, poly_pref, fill_pref);

    /* -------------------------------------- */
    /* Draw the new line in the current color */
    /* -------------------------------------- */

    if ( dr_data->current_line != (LINE *) NULL )
        {
        dr_redraw_line(dr_data);
        }

    return SUCCESS;
}
