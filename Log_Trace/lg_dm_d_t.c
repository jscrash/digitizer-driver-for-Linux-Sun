/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_D_T.C*/
/* *5    19-NOV-1990 11:18:37 MING "(SPR 6192) fix rename and delete options in logDB"*/
/* *4    23-AUG-1990 17:41:06 JESSIE "(SPR 1) fix include files"*/
/* *3    26-SEP-1989 09:20:02 GILLESPIE "(SPR 100) Changes due to Gulf schema changes"*/
/* *2    19-JUL-1989 07:59:04 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 13:04:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_D_T.C*/
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

#include "esi_lg_ldefs.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#include "esi_lg_err.h"

/* Function Description -----------------------------------------------------
Description:
    Deletes the requested trace from the database and trace file.  First
    delete the database entry, then deletes the specified trace from trace
    file.

Prototype:
    publicdef INT lg_ltdm_delete_trace(UWI uwi, CHAR traceCname[]);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) uwi         -(UWI) %P Unique Well Identifier.
    (I) traceCname  -(CHAR [])

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT lg_ltdm_delete_trace (UWI uwi, CHAR traceCname[]) 
#else
publicdef INT lg_ltdm_delete_trace (uwi, traceCname)
UWI uwi;
CHAR traceCname[];
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    LOG_TRACE_STRUCT trace;         /*   workspace of LOG_TRACE_STRUCT.        */
    FILENAME filename;
    
    /* ************************************************************************* */
    
    if (uwi EQUALS NULL OR traceCname EQUALS NULL)
        return LG_INVALID_POINTER;
    
    if (lgz_check_exist (uwi)OR lgz_check_exist (traceCname))
        return LG_INVALID_POINTER;
    
    tc_zeroes (&trace, sizeof(LOG_TRACE_STRUCT));
    strcpy (trace.uwi, uwi);
    strcpy (trace.trace_name, traceCname);
    
    status = lg_ltdm_parser (&trace);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    
    /*   If there is no version number, returns  */
    /*   error message.                          */
    
    if (trace.version <= 0)
        return LG_INVALID_VERSION;
    
    /*   check the existance of the trace.       */
    
    status = lg_ltdm_check_write (trace.uwi, trace.trace_name);
    if (!status)
        return LG_TRACE_NOT_FOUND;
    
    /*   delete the entry from the database, if  */
    /*   succeed, then deletes the trace from the*/
    /*   trace file.                             */
    
    if ((status = lgz_get_trace_names (&trace))IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    if ((status = lgz_delete_db_entry (&trace))IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }

    ho_add_path ("ESI$LOGS",trace.filename,sizeof(FILENAME),filename);

    status = nl_delete_nlist (filename, trace.nlist_id);

    if (status != SUCCESS)
      {
      am_message (AM_ALERT,"Failed to delete the nlist");
      }

    return SUCCESS;
    }
/* END:     */
