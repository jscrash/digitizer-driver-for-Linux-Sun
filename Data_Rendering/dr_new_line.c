/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_LINE.C*/
/* *2    27-JUL-1989 15:04:34 GILLESPIE "(SPR 1) Fix minor casing problem"*/
/* *1    19-JUN-1989 12:47:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_LINE.C*/

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

NAME:         dr_new_line 

DESCRIPTION:  Create a new Line 


     Input:   dr_data - The standard data structure

     Output:  dr_data - Updated

     Status:  SUCCESS
     
     TODO:	  1) Consolidate the definitions of FAULTS, etc.
     		  2) Should generation of new line_id be done by dr_append_object?

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define FAULTS "FAULTS"
#define AVERAGE_VELOCITY "AVERAGE_VELOCITY"
#define INTERVAL_VELOCITY "INTERVAL_VELOCITY"

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

publicdef INT dr_new_line(dr_data)
DR_DATA *dr_data;
{
    INT status;
    OBJECT_STRUCTURE *this_object;


    dr_data->current_line_seg = 0;

/*  Create new line structure & append to list of lines  */

    dr_append_object( dr_data, (BYTE *)&dr_data->default_line, DR_LINE_OBJECT, TRUE);
    
    dr_new_project_id(&dr_data->current_line->line_id);


/* Get a name, if necessary   */

    if((strcmp(dr_data->current_object_type,FAULTS) == 0) ||
       (strcmp(dr_data->current_object_type,AVERAGE_VELOCITY) == 0) ||
       (strcmp(dr_data->current_object_type,INTERVAL_VELOCITY) == 0))
    {
        status = dr_set_name(dr_data);
    }
    return SUCCESS;
}
