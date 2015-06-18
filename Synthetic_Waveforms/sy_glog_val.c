/* DEC/CMS REPLACEMENT HISTORY, Element SY_GLOG_VAL.C*/
/* *4    17-AUG-1990 22:23:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     7-MAR-1990 18:12:53 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *2    30-JUL-1989 00:51:56 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:31:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SY_GLOG_VAL.C*/
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

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_SY_H
#include "esi_sy.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif
    publicdef INT sy_get_log_values();
    publicdef INT sy_check_log();
    publicdef INT sy_log_min_max();
    publicdef INT sy_get_log_values();
/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sy_get_log_values(SYNTHE_LOG *p_log );
    
    publicdef INT sy_check_log(SYNTHE_LOG *p_log, DOUBLE null);
    
    publicdef INT sy_log_min_max(SYNTHE_LOG *pLog );

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Fill the array p_log->values.

Prototype:
    publicdef INT sy_get_log_values(SYNTHE_LOG *p_log );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_log           -(SYNTHE_LOG *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_get_log_values (SYNTHE_LOG *p_log)
#else
publicdef INT sy_get_log_values (p_log)
SYNTHE_LOG *p_log;
#endif
    {
    
    /*      start declaration       */
    
    INT status;
    INT i, j, k;
    UINT npts, ndim, Col_List[2];
    VOIDPTR Var_List[2];
    
    FLOAT null;                     /* 4 byte float */
    FLOAT *log_arr, *temp, *ref;
    DOUBLE z, d_inc, val_uom;
    UOM_NAME uom_copy;
    CHAR string1[64], string2[256];
    LOG_TRACE_STRUCT *ptr;
    
    /*      end declaration         */
    
    /*      get log header              */
    
    ptr = p_log->log_hdr;
    
    /*      retrieve DESIRED depth/time sampling            */
    
    if (ptr->depth_type EQUALS DEPTH_IN_DISTANCE)
        status = tu_unit_conversion ((DOUBLE)(ptr->dinc), ptr->depth_uom, &d_inc,
                                     "METERS");
    else
        status = tu_unit_conversion ((DOUBLE)(ptr->dinc), ptr->depth_uom, &d_inc,
                                     "SECONDS");
    if (status)
        return status;
    strcpy (uom_copy, ptr->depth_uom);
    
    status = lg_ltdm_get_trace (ptr);
    if (status)
        return status;
    
    status = nl_inq_nlist_int (ptr->ptrace, NL_NLIST_NPOINTS, (INT *) & npts);
    if (status IS_NOT_EQUAL_TO SUCCESS OR npts EQUALS 0)
        {
        nl_free_nlist (ptr->ptrace);
        return status;
        }
    ndim = 1;
    Col_List[0] = 2;
    Var_List[0] = (VOIDPTR)(&null);
    nl_inq_nlist_info (ptr->ptrace, NL_NLIST_NULL_VALUE, ndim, Col_List, Var_List);
    
    log_arr = (FLOAT *)tc_alloc (npts * sizeof(FLOAT));
    Var_List[0] = (VOIDPTR)log_arr;
    nl_set_current_point (ptr->ptrace, 1, 1);
    if (nl_get_points (ptr->ptrace, npts, ndim, Col_List, Var_List,
                       (UINT *) & i) < 0)
        return FAIL;
    
    j = SY_LOG_NPTS (ptr);
    
    if (i IS_NOT npts OR i IS_NOT j)
        if (i < 1)
            return FAIL;
        /*      assume that the missing points are the last ones    */
        else
            ptr->base = ptr->top + (i - 1) * ptr->dinc;
    
    nl_free_nlist (ptr->ptrace);
    
    /*      Now see if we need to under_sample this log         */
    
    if (*ptr->depth_uom IS'\0')
        strcpy (ptr->depth_uom, uom_copy);
    if (ptr->depth_type IS DEPTH_IN_DISTANCE)
        status = tu_unit_conversion ((DOUBLE)(ptr->dinc), ptr->depth_uom, &val_uom,
                                     "METERS");
    else
        status = tu_unit_conversion ((DOUBLE)(ptr->dinc), ptr->depth_uom, &val_uom,
                                     "SECONDS");
    if (status)
        return status;
    
    i = d_inc / val_uom + 0.0001;
    if (i > 1)
        {
        ptr->dinc *= i;
        npts = SY_LOG_NPTS (ptr);
        ptr->base = ptr->top + (npts - 1) * ptr->dinc;
        
        ref = temp = (FLOAT *)tc_alloc (npts * sizeof(FLOAT));
        
        for (j = 0; j < npts; j++, log_arr += i)
            *temp++ = *log_arr;
        
        tc_free (log_arr);
        log_arr = ref;
        }
    p_log->values = log_arr;
    
    /*      check it is OK   and also                       */
    /*      get rid of null values and interpolate them     */
    
    status = sy_check_log (p_log, null);
    if (status)
        tc_free (p_log->values);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Remove null values by interpolation or extrapolation...
    check also for remaining negative values....

Prototype:
    publicdef INT sy_check_log(SYNTHE_LOG *p_log, DOUBLE null);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_log           -(SYNTHE_LOG *)
    null            -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_check_log (SYNTHE_LOG *p_log, DOUBLE null)
#else
publicdef INT sy_check_log (p_log, null)
SYNTHE_LOG *p_log;
DOUBLE null;
#endif
    {
    /*      start declaration       */
    
    INT value_ok, i, j, k, npts;
    DOUBLE null_min, null_max, val, coef;
    FLOAT *log_arr, *last_good;
    LOG_TRACE_STRUCT *ptr;
    
    /*      end declaration         */
    
    ptr = p_log->log_hdr;
    npts = SY_LOG_NPTS (ptr);
    
    log_arr = p_log->values;
    
    /*      we shall be tolerant about null_values and ask reasonable precision */
    
    null_min = 0.0001 * ABS (null);
    null_max = null + null_min;
    null_min = null - null_min;
    
    value_ok = TRUE;
    last_good = (FLOAT *)NULL;
    
    for (i = 0; i < npts; i++, log_arr++)
        {
        val = *log_arr;
        if (value_ok)
            {
            if (val > null_max OR val < null_min)
                last_good = log_arr;
            else
                value_ok = FALSE;
            }
        else if (val > null_max AND val < null_min)
            {
            if (last_good IS (FLOAT *)NULL)
                {
                last_good = p_log->values;
                *last_good = val;
                }
            coef = (*last_good - val);
            k = log_arr - last_good;
            coef /= (FLOAT)(k);
            log_arr = last_good + 1;
            for (j = 1; j < k; j++)
                *log_arr++ = *last_good++ + coef;
            last_good++;
            value_ok = TRUE;
            }
        else if (i IS (npts - 1))
            {
            if (last_good IS_NOT (FLOAT *)NULL)
                {
                val = *last_good++;
                while (last_good <= log_arr)
                    *last_good++ = val;
                }
            }
        }
    if (last_good IS (FLOAT *)NULL)
        return FAIL;
    
    /* depth logs (real logs) should not have negative values       */
    
    sy_log_min_max (p_log);
    if (ptr->depth_type IS DEPTH_IN_DISTANCE AND ptr->min < 0.)
        return FAIL;
    else
        return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_log_min_max(SYNTHE_LOG *pLog );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pLog            -(SYNTHE_LOG *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_log_min_max (SYNTHE_LOG *pLog)
#else
publicdef INT sy_log_min_max (pLog)
SYNTHE_LOG *pLog;
#endif
    {
    INT npts;
    DOUBLE min = HUGE_VAL, max = -HUGE_VAL;
    register FLOAT *log_arr;
    /***************************************************************************/
    
    npts = SY_LOG_NPTS (pLog->log_hdr);
    log_arr = pLog->values;
    
    for (; npts > 0; npts--, log_arr++)
        {
        if (max < *log_arr)
            max = *log_arr;
        if (min > *log_arr)
            min = *log_arr;
        }
    pLog->log_hdr->min = min;
    pLog->log_hdr->max = max;
    return SUCCESS;
    }
/* END:     */
