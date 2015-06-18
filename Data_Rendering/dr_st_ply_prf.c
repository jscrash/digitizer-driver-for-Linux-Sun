/* DEC/CMS REPLACEMENT HISTORY, Element DR_ST_PLY_PRF.C*/
/* *1    19-JUN-1989 12:48:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_ST_PLY_PRF.C*/

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

NAME:            dr_set_poly_pref

DESCRIPTION:     Set polygon attributes


	Input:		dr_data 	- Standard data structure
				poly_pref	- TRUE if lines are to be closed polygons
				fill_pref	- TRUE if closed polygons are to be filled

	Output:     dr_data - Updated

	Status:		SUCCESS
     				

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_gks.h"

publicdef INT dr_set_poly_pref( dr_data, poly_pref, fill_pref )
DR_DATA *dr_data;
BOOL poly_pref, fill_pref;
{
    INT status;

    
/*	Set the dr_data attributes */

    dr_data->line_poly = poly_pref;
    dr_data->line_fill = fill_pref;
    
/*	Set the polygon attributes for the current line, if any */

    if(dr_data->current_line != (LINE *)NULL)
	{
        dr_data->current_line->polygon_flag = poly_pref;
        dr_data->current_line->polygon_fill_style = fill_pref ? SOLID : HOLLOW;
	}
            
/*	Set defaults to reflect current user preferences */

    dr_data->default_line.polygon_flag = poly_pref;
    dr_data->default_line.polygon_fill_style = fill_pref ? SOLID : HOLLOW;

    return SUCCESS;
}

