/* DEC/CMS REPLACEMENT HISTORY, Element SY_FREE.C*/
/* *2    17-AUG-1990 22:23:31 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:31:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SY_FREE.C*/
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

#include "esi_sy.h"
publicdef INT sy_free_traces ();
    publicdef INT sy_free();
    publicdef INT sy_clean();
    publicdef INT sy_free_log();
    publicdef INT sy_free_logs();
    publicdef INT sy_free_trace();
    publicdef INT sy_free_traces();
    publicdef INT sy_free();
/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sy_free(SYNTHE_STRUCT *p_synthe );
    
    publicdef INT sy_clean(SYNTHE_STRUCT *p_synthe );
    
    publicdef INT sy_free_log(SYNTHE_LOG *log );
    
    publicdef INT sy_free_logs(SYNTHE_STRUCT *p_synthe );
    
    publicdef INT sy_free_trace(SYNTHE_TRACE *trace );
    
    publicdef INT sy_free_traces(SYNTHE_STRUCT *p_synthe );

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_free(SYNTHE_STRUCT *p_synthe );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_synthe        -(SYNTHE_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_free (SYNTHE_STRUCT *p_synthe)
#else
publicdef INT sy_free (p_synthe)
SYNTHE_STRUCT *p_synthe;
#endif
    {
    tc_free (p_synthe->proj_def);
    
    if (p_synthe->pWell IS_NOT (WELL_STRUCT *)NULL)
        tc_free (p_synthe->pWell);
    
    sy_free_logs (p_synthe);
    sy_free_traces (p_synthe);
    
    tc_free (p_synthe);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_clean(SYNTHE_STRUCT *p_synthe );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_synthe        -(SYNTHE_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_clean (SYNTHE_STRUCT *p_synthe)
#else
publicdef INT sy_clean (p_synthe)
SYNTHE_STRUCT *p_synthe;
#endif
    {
    PROJECT_DEFAULT *ptr;
    
    if (p_synthe->pWell IS (WELL_STRUCT *)NULL)
        return SUCCESS;
    
    ptr = p_synthe->proj_def;
    
    tc_free (p_synthe->pWell);
    
    sy_free_logs (p_synthe);
    sy_free_traces (p_synthe);
    
    tc_zeroes (p_synthe, sizeof(SYNTHE_STRUCT));
    
    ptr = p_synthe->proj_def;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_free_log(SYNTHE_LOG *log );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    log             -(SYNTHE_LOG *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_free_log (SYNTHE_LOG *log)
#else
publicdef INT sy_free_log (log)
SYNTHE_LOG *log;
#endif
    {
    
    if ((log)IS_NOT (SYNTHE_LOG *)NULL)
        {
        if (log->values IS_NOT (FLOAT *)NULL)
            tc_free (log->values);
        if (log->log_hdr IS_NOT (LOG_TRACE_STRUCT *)NULL)
            tc_free (log->log_hdr);
        tc_free (log);
        return SUCCESS;
        }
    return FAIL;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_free_logs(SYNTHE_STRUCT *p_synthe );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_synthe        -(SYNTHE_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_free_logs (SYNTHE_STRUCT *p_synthe)
#else
publicdef INT sy_free_logs (p_synthe)
SYNTHE_STRUCT *p_synthe;
#endif
    {
    
    SYNTHE_LOG **tab;
    
    tab = p_synthe->logs;
    
    if (tab IS_NOT (SYNTHE_LOG **)NULL)
        while (NOT sy_free_log (*tab++))
            {
            }
    tc_free (p_synthe->logs);
    
    sy_free_log (p_synthe->time_curve);
    sy_free_log (p_synthe->sonic);
    sy_free_log (p_synthe->density);
    sy_free_log (p_synthe->impedance);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_free_trace(SYNTHE_TRACE *trace );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    trace           -(SYNTHE_TRACE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_free_trace (SYNTHE_TRACE *trace)
#else
publicdef INT sy_free_trace (trace)
SYNTHE_TRACE *trace;
#endif
    {
    
    if ((trace)IS_NOT (SYNTHE_TRACE *)NULL)
        {
        if (trace->values IS_NOT (FLOAT *)NULL)
            tc_free (trace->values);
        tc_free (trace);
        return SUCCESS;
        }
    return FAIL;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_free_traces(SYNTHE_STRUCT *p_synthe );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_synthe        -(SYNTHE_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_free_traces (SYNTHE_STRUCT *p_synthe)
#else
publicdef INT sy_free_traces (p_synthe)
SYNTHE_STRUCT *p_synthe;
#endif
    {
    SYNTHE_TRACE **tab;
    
    tab = p_synthe->traces;
    
    if (tab IS_NOT (SYNTHE_TRACE **)NULL)
        while (NOT sy_free_trace (*tab++))
            {
            }
    tc_free (p_synthe->traces);
    
    sy_free_trace (p_synthe->impulse);
    sy_free_trace (p_synthe->multiple);
    sy_free_trace (p_synthe->depth_curve);
    
    return SUCCESS;
    }
/* END:     */
