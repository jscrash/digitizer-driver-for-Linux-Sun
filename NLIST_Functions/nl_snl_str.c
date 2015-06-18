/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_STR.C */
/* *5    25-OCT-1990 12:27:07 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:06:05 VINCE "(SPR 5644) Header Standardization" */
/* *3    12-MAR-1990 19:09:46 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:50:57 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:40 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SNL_STR.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STNL_STR.C*/
/* *1    19-JUN-1989 13:21:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STNL_STR.C*/
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

#include "esi_nl_int.h"
#include "esi_c_lib.h"

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#include "esi_tc.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Set the character string status variable for the N_list.   

Prototype:
    publicdef INT nl_set_nlist_string(NLIST_HEADER nlist, INT opcode, CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    string          -(CHAR *)
    opcode          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_nlist_string (NLIST_HEADER nlist, INT opcode, CHAR *string)
#else
publicdef INT nl_set_nlist_string (nlist, opcode, string)
NLIST_HEADER nlist;
CHAR *string;
INT opcode;
#endif
    {
    INT status = SUCCESS;
    FILENAME temp_filename;
    
    /* error checking.  */
    INIT_TEST (nlist);
    if (string EQUALS NULL)
        return NL_INVALID_POINTER;
    
    switch (opcode)
        {
    case NL_NLIST_NAME:
        if (nlist->name IS_NOT_EQUAL_TO NULL)
            {
            tc_free ((VOIDPTR)nlist->name);
            }
        nlist->name = STRING_ALLOCATE (string);
        strcpy ((char *)nlist->name, (char *)string);
        ts_sto_upper (nlist->name);
        break;
        
    case NL_NLIST_FILENAME:
        if (nlist->data_filename IS_NOT_EQUAL_TO NULL)
            {
            tc_free ((VOIDPTR)nlist->data_filename);
            }
        ho_translate_filename (string, temp_filename);
        nlist->data_filename = STRING_ALLOCATE (temp_filename);
        strcpy ((char *)nlist->data_filename, (char *)temp_filename);
        break;
        
    case NL_NLIST_NEXT:
        if (nlist->next_nlist IS_NOT_EQUAL_TO NULL)
            {
            tc_free ((VOIDPTR)nlist->next_nlist);
            }
        nlist->next_nlist = STRING_ALLOCATE (string);
        strcpy ((char *)nlist->next_nlist, (char *)string);
        break;
        
    case NL_NLIST_PREVIOUS:
        if (nlist->prev_nlist IS_NOT_EQUAL_TO NULL)
            {
            tc_free ((VOIDPTR)nlist->prev_nlist);
            }
        nlist->prev_nlist = STRING_ALLOCATE (string);
        strcpy ((char *)nlist->prev_nlist, (char *)string);
        break;
        
    default:
        status = NL_INVALID_CODE;
        }
    return status;
    
    }
/* END:     */
