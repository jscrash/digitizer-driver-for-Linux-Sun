/* DEC/CMS REPLACEMENT HISTORY, Element HF_START_HELP.C*/
/* *3    17-AUG-1990 21:48:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-JUN-1989 10:46:01 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:54:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HF_START_HELP.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"

#ifndef ESI_HF_INT_X

#include "esi_hf_int.x"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To reset the help workspace.

Prototype:
    publicdef INT hf_start_help (CHAR *help_key);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    help_key        -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hf_start_help (CHAR *help_key)
#else
publicdef INT hf_start_help (help_key)
CHAR *help_key;
#endif
    {
    INT status = SUCCESS;
    status = am_open_workspace ("HELP_WORKSPACE", AM_GLOBAL, (VOIDPTR *) & hf_struct);
    
    /* if there is no opening workspace.                                */
    /* allocates one and initializes all                                */
    /* variables.                                                       */
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        /* if there is no allocation before, do it                      */
        status = am_define_workspace ("HELP_WORKSPACE", AM_GLOBAL, sizeof(HF_STRUCT),
                                      (VOIDPTR *) & hf_struct);
        hf_struct->message_ptr = (CHAR *)am_allocate (AM_SYSTEM, MAX_NUM_CHARS * 46);
        hf_struct->total_used_chars = hf_struct->cdlg_id = hf_struct->total_lines = 0;
        hf_struct->total_allocated_chars = MAX_NUM_CHARS * 46;
        hf_struct->parent_key = (CHAR *)NULL;
        hf_struct->current_key = (CHAR *)am_allocate (AM_SYSTEM,
                                                      strlen (help_key) + 2);
        strcpy (hf_struct->current_key, help_key);
        hf_display_help_init ();
        }
    else
        {
        /* If there is an opening workspace,                            */
        /* cleans the buffer, sets the                                  */
        /* previous current key to be the                               */
        /* parent key and allocates space for                           */
        /* one current key.                                             */
        
        hf_struct->message_ptr = (CHAR *)tc_zeroes (hf_struct->message_ptr,
                                                    hf_struct->total_allocated_chars);
        hf_struct->parent_key = hf_struct->current_key;
        hf_struct->current_key = (CHAR *)am_allocate (AM_SYSTEM,
                                                      strlen (help_key) + 2);
        strcpy (hf_struct->current_key, help_key);
        hf_struct->total_used_chars = 0;
        }
    return SUCCESS;
    }
/* END:     */
