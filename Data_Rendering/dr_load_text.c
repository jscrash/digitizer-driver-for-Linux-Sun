/* DEC/CMS REPLACEMENT HISTORY, Element DR_LOAD_TEXT.C*/
/* *3    18-JUL-1990 08:22:15 GILLESPIE "(SPR 1) Change call to ct_get_text"*/
/* *2    25-SEP-1989 11:30:33 GORDON "(SPR 201) Add castings"*/
/* *1    19-JUN-1989 12:46:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_LOAD_TEXT.C*/

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

NAME:         dr_load_text

DESCRIPTION:  Load a text object from the database


     Input:   item_id - Database key of item to retrieve
              dr_data - Standard data structure

     Output:  dr_data - updated

     Status:  SUCCESS;
              DR_READ_ERROR

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_ct.h"

#if USE_PROTOTYPES
publicdef INT dr_load_text(INT item_id, DR_DATA *dr_data)
#else
publicdef INT dr_load_text(item_id,dr_data)
INT item_id;
DR_DATA *dr_data;
#endif
{
    INT status;
    TEXT text_buffer;


/*  Get the text  */

    status = ct_get_text(&text_buffer,item_id);

    if(status == SUCCESS)
	{

/*	Create new text structure & append to list of text's  */

	dr_append_object( dr_data, (VOIDPTR)&text_buffer, DR_TEXT_OBJECT, FALSE);

	}
         
    return status;
}
