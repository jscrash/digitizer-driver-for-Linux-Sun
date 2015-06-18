/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_G_T_R.C*/
/* *8    17-JUL-1991 16:25:57 JANTINA "(SPR 0) Update top,base,increment after resample"*/
/* *7    23-AUG-1990 17:42:10 JESSIE "(SPR 1) fix include files"*/
/* *6    17-MAR-1990 13:00:27 JULIAN "(SPR 0) undo last mods"*/
/* *5    16-MAR-1990 11:21:18 JULIAN "(SPR 5135) add nl_strip_points"*/
/* *4     9-NOV-1989 14:51:30 GORDON "(SPR 5058) Use new nl_methods for resampling"*/
/* *3    14-SEP-1989 18:20:22 GORDON "(SPR -1) checkin from gulf"*/
/* *2    27-JUL-1989 22:30:05 GILLESPIE "(SPR 1) fix minor casting problem"*/
/* *1    19-JUN-1989 13:04:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_G_T_R.C*/
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

#include "esi_lg_defs.h"

#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#include "esi_nl_meth_hdr.h"
#include "esi_nl_methods.h"

#include "esi_lg_err.h"

#define FILTERLEN 29	     /*	The length of the anti-aliasing filter
			     	which will be appliedif necessary to the
			     	trace before resampling. */

/* Function Description -----------------------------------------------------
Description:
    Retrieves trace and resamples trace by the specified increment in the
    trace Struct.

Prototype:
    publicdef INT lg_ltdm_get_trace_resamp(LOG_TRACE_STRUCT   *traceStruct);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) traceStruct -(LOG_TRACE_STRUCT *) %P a pointer of LOG_TRACE_STRCUT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT lg_ltdm_get_trace_resamp (LOG_TRACE_STRUCT *traceStruct) 
#else
publicdef INT lg_ltdm_get_trace_resamp (traceStruct)
LOG_TRACE_STRUCT *traceStruct;
#endif
    {
    INT status = SUCCESS;           /*   the returned status.                    */
    UINT ncol = 2;
    DOUBLE resampleInc;             /*   the specified increment of resampling. */
    UINT colList[2];
    NLIST_HEADER tempTrace;         /*   worksapce for returned trace.            */
    NLM_RESAMPLE_HEADER parms;
    /*  ************************************************************************** */
    
    if (traceStruct EQUALS (LOG_TRACE_STRUCT *)NULL)
        return LG_INVALID_POINTER;
    
    resampleInc = traceStruct->dinc;
    status = lg_ltdm_get_trace (traceStruct);
    if (status)
        {
        return status;
        }
    /* compare two increments, if they are same,
       return, otherwise resample the trace.    */
    if (resampleInc IS_NOT_EQUAL_TO 0.0 AND 
	resampleInc IS_NOT_EQUAL_TO traceStruct->dinc)
        {
        colList[0] = 1, colList[1] = 2;
        status = nl_set_nlist_method (traceStruct->ptrace, NL_RESAMPLE_METHOD,
                                      "NLM_SPLINE_NLIST", nlm_spline_nlist);
        
        parms.ncol = ncol;
        parms.Col_List = colList;
        parms.resampleInc = resampleInc;
        parms.filterLen = FILTERLEN;
        
        status = nl_invoke_nlist_method (traceStruct->ptrace, NL_RESAMPLE_METHOD,
                                         &parms);
        if (status < SUCCESS)
            return status;
        tempTrace = parms.outNlist;
        status = nl_free_nlist (traceStruct->ptrace);
        if (status < SUCCESS)
            {
            return status;
            }
        traceStruct->ptrace = tempTrace;
	status = lgz_update_ranges(traceStruct);
	traceStruct->non_null_top = traceStruct->top;
	traceStruct->non_null_base = traceStruct->base;
        return status;
        }
    return status;
    }
/* END:     */
