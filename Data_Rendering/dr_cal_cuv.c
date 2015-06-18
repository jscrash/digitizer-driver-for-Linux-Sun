/* DEC/CMS REPLACEMENT HISTORY, Element DR_CAL_CUV.C*/
/* *1    19-JUN-1989 12:46:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_CAL_CUV.C*/

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

NAME:           dr_calc_char_up_vector

DESCRIPTION:    Given deltax, deltay representation of a vector along
				the desired text path, compute the GKS char up vector
				orthogonal to the specified path.

     Input:     dr_data 	- Standard data structure
                deltax 		- Vector base in WCS
                deltay 		- Vector height in WCS

     Output:    dr_data 	- Updated

     Status:

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_math.h"

publicdef INT dr_calc_char_up_vector( dr_data, deltax, deltay )

DR_DATA *dr_data;
DOUBLE deltax;
DOUBLE deltay;
{
    INT status;
    DOUBLE angle;

    angle = atan2( deltay, deltax ) + PI/2.0;
	 
    dr_data->current_text->text_up_x = (FLOAT) cos( angle );
    dr_data->current_text->text_up_y = (FLOAT) sin( angle );

    return SUCCESS;
}
