/* DEC/CMS REPLACEMENT HISTORY, Element DR_LOAD_LINE.C*/
/* *6    24-SEP-1990 16:13:04 GILLESPIE "Beta Deltas"*/
/* *5    19-JUL-1990 17:53:52 GILLESPIE "(SPR 5411) Support for fault traces"*/
/* *4    18-JUL-1990 10:24:02 GILLESPIE "(SPR 5411) Add support for seismic fault trace"*/
/* *3    11-AUG-1989 07:43:39 GORDON "(SPR 105) stop adding the dot to nlist_file, its not necessary"*/
/* *2    27-JUL-1989 15:04:28 GILLESPIE "(SPR 1) Fix minor casing problem"*/
/* *1    19-JUN-1989 12:46:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_LOAD_LINE.C*/
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

NAME:         dr_load_line

DESCRIPTION:  Load a line object from the database


     Input:   item_id - Database key of item to retrieve
              dr_data - Standard data structure

     Output:  dr_data - updated

     Status:  SUCCESS;
              DR_READ_ERROR

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_ct.h"

#if USE_PROTOTYPES
publicdef INT dr_load_line(INT item_id, DR_DATA *dr_data)
#else
publicdef INT dr_load_line(item_id, dr_data)
INT item_id;
DR_DATA *dr_data;
#endif
    {
    INT      line_status;
    LINE     line_buffer;
    FILENAME nlist_file;
    BOOL     isfault;
    
    isfault = (strcmp(dr_data->current_object_type, FAULTS_TYPE) == 0);

    if (! isfault)
        {
        /*  Set up nlist file name as <current_object_type> under the
            ESI$CULTURE directory */

        ho_add_path ("ESI$CULTURE", dr_data->current_object_type,
                     sizeof(FILENAME), nlist_file);
        }

    /*  Get the single line  */
        
    line_status = dr_get_line (&line_buffer, item_id, nlist_file, isfault,
                               dr_data->current_marker);

    if (line_status == SUCCESS)
        {
        /* Create new line structure & append to list of lines */
        dr_append_object (dr_data, (VOIDPTR) &line_buffer,
                          DR_LINE_OBJECT, FALSE);
        }

    return line_status;
    }
