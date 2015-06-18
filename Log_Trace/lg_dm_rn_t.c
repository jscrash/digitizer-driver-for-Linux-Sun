/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_RN_T.C*/
/* *3    19-NOV-1990 11:18:41 MING "(SPR 6192) fix rename and delete options in logDB"*/
/* *2    23-AUG-1990 17:50:03 JESSIE "(SPR 1) fix include files"*/
/* *1    19-JUN-1989 13:04:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_RN_T.C*/
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
#include "esi_tc.h"
#include "esi_ho.h"
#include "esi_lg_defs.h"
#include "esi_lg_ldefs.h"
#include "esi_lg_err.h"

/* Function Description -----------------------------------------------------
Description:
    Immediately renames a trace in the database and trace file.  Traces
    owned by other areas cannot be renamed.

Prototype:
    publicdef INT lg_ltdm_rename_trace(UWI uwi, CHAR oldName[], CHAR newName[]);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) uwi         -(UWI) %P Unique well identifier.        
    (I) oldName     -(CHAR []) %P the old trace name.         
    (M) newName     -(CHAR []) %P the new trace name.      

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT lg_ltdm_rename_trace (UWI uwi, CHAR oldName[], CHAR newName[]) 
#else
publicdef INT lg_ltdm_rename_trace (uwi, oldName, newName)
UWI uwi;
CHAR oldName[];
CHAR newName[];
#endif
    {
    LOG_TRACE_STRUCT traceOld;      /*   workspace of LOG_TRACE_STRUCT.          */
    LOG_TRACE_STRUCT traceNew;      /*   workspace of LOG_TRACE_STRUCT.          */
    INT status = SUCCESS;           /*   the returned status.                    */
    /*---------------------------------------*/
    /* to start a new version after the last */
    /* version.                              */
    /*---------------------------------------*/
    INT newVersion = TRUE;
    
    /*  ************************************************************************** */
    
    /*   error checking.                         */
    
    if (uwi EQUALS NULL OR oldName EQUALS NULL OR newName EQUALS NULL)
        return LG_INVALID_POINTER;
    
    if (lgz_check_exist (uwi)OR lgz_check_exist (oldName)OR lgz_check_exist (newName))
        return LG_INVALID_POINTER;
    
    /*   parsing both names.  If one of names has*/
    /*   no version number or has invalid trace  */
    /*   composite name, returns error status.   */
    
    tc_zeroes (&traceNew, sizeof(LOG_TRACE_STRUCT));
    tc_zeroes (&traceOld, sizeof(LOG_TRACE_STRUCT));
    hoblockmove (uwi, traceNew.uwi, sizeof(UWI));
    hoblockmove (uwi, traceOld.uwi, sizeof(UWI));
    strcpy (traceNew.trace_name, newName);
    status = lg_ltdm_parser (&traceNew);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    /*   if there is no version number in the new*/
    /*   trace structure, call LGZ_INQ_LAST_VERSION*/
    /*   to get the last version.                */
    
    if (traceNew.version <= 0)
        {
        status = lgz_inq_last_version (&traceNew, newVersion);
        if (status)
            return status;
        }
    strcpy (traceOld.trace_name, oldName);
    status = lg_ltdm_parser (&traceOld);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    
    /*   if there is no version number in the old*/
    /*   trace structure, call LGZ_INQ_LAST_VERSION*/
    /*   to get the last version.                */
    
    if (traceOld.version <= 0)
        {
        newVersion = FALSE;
        status = lgz_inq_last_version (&traceOld, newVersion);
        if (status)
            return status;
        }

    /*---------------------- */
    /* get trace information */
    /* --------------------- */
    status = lgz_get_trace_names (&traceOld);

    /*   rename the database entry first, if*/
    /*   succeed, rename the id in the      */
    /*   trace file. otherwise returns error*/
    /*   status.                            */
    
    return lgz_rename_db_entry (&traceOld, &traceNew);
    
    }
/* END:     */
