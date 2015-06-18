/* DEC/CMS REPLACEMENT HISTORY, Element SY_DEPTOTIME.C*/
/* *4    17-AUG-1990 22:23:24 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     7-MAR-1990 18:12:45 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *2    26-JUN-1989 11:04:36 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:31:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SY_DEPTOTIME.C*/
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
#include "esi_tc.h"
publicdef INT sy_desintegration ();
publicdef INT sy_get_time_curve ();

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sy_depth_to_time(UWI uwi, FLOAT *depth, FLOAT *time, UOM_NAME time_uom, 
        INT npts);
    
    publicdef INT sy_get_time_curve(UWI uwi, SYNTHE_LOG **time_curve);
    
    publicdef INT sy_desintegration(SYNTHE_LOG *depth_curve, SYNTHE_LOG **time_curve);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sy_depth_to_time(UWI uwi, FLOAT *depth, FLOAT *time, UOM_NAME time_uom, 
        INT npts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uwi             -(UWI)
    depth           -(FLOAT *)
    time            -(FLOAT *)
    time_uom        -(UOM_NAME)
    npts            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_depth_to_time (UWI uwi, FLOAT *depth, FLOAT *time, UOM_NAME time_uom,
                                INT npts)
#else
publicdef INT sy_depth_to_time (uwi, depth, time, time_uom, npts)
UWI uwi;
FLOAT *depth, *time;
UOM_NAME time_uom;
INT npts;
#endif
    {
    /*  start declaration   */
    
    DOUBLE depth_coef, time_coef;
    SYNTHE_LOG *time_curve = (SYNTHE_LOG *)NULL;
    PROJECT_DEFAULT *proj_def;
    INT i, k, status;
    DOUBLE d, t;
    
    /*  end declaration     */
    /**************************************************************************/
    
    status = sy_get_time_curve (uwi, &time_curve);
    if (status)
        return status;
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *) & proj_def);
    
    status = tu_unit_conversion ((DOUBLE)(1.), proj_def->unit.vertical, &depth_coef,
                                 time_curve->log_hdr->depth_uom);
    if (status)
        return status;
    
    status = tu_unit_conversion ((DOUBLE)(1.), time_curve->log_hdr->trace_uom,
                                 &time_coef, time_uom);
    if (status)
        return status;
    
    if (time  EQUALS  (FLOAT *)NULL)
        time = (FLOAT *)tc_zalloc (npts * sizeof(FLOAT));
    
    for (; npts > 0; npts--)
        {
        d = *depth++ * depth_coef;
        
        if (d < time_curve->log_hdr->top OR d > time_curve->log_hdr->base)
            t = -999.;
        
        else
            {
            i = (d - time_curve->log_hdr->top) / time_curve->log_hdr->dinc;
            
            t = (time_curve->values[i + 1] - time_curve->values[i]);
            t /= time_curve->log_hdr->dinc;
            t *= (d - (time_curve->log_hdr->top + i * time_curve->log_hdr->dinc));
            t += time_curve->values[i];
            }
        *time++ = t * time_coef;
        }
    sy_free_log (time_curve);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Retrieve the database information about the available
        - existing logs,
        - and particularly SONICs.

Prototype:
    publicdef INT sy_get_time_curve(UWI uwi, SYNTHE_LOG **time_curve);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uwi             -(UWI)
    time_curve      -(SYNTHE_LOG **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_get_time_curve (UWI uwi, SYNTHE_LOG **time_curve)
#else
publicdef INT sy_get_time_curve (uwi, time_curve)
UWI uwi;
SYNTHE_LOG **time_curve;
#endif
    {
    /*  start declaration   */
    
    INT i, j, status;
    CHAR sql_string[256];
    LOG_TRACE_STRUCT **logs;
    SYNTHE_LOG *depth_curve;
    SYNTHE_LOG *temp;
    
    /*  end declaration     */
    /**************************************************************************/
    
    /*  retrieve log traces info        */
    
    strcpy (sql_string,
            " ( DEPTH_TYPE = 'D' OR DEPTH_TYPE = 'd') AND TRACE = 'TIME' ");
    strcat (sql_string, " ORDER BY LAST_UPDATE DESC ");
    status = lg_ltdm_inq_well (uwi, sql_string, &logs);
    
    if (NOT status)
        {
        *time_curve = temp = (SYNTHE_LOG *)tc_zalloc (sizeof(SYNTHE_LOG));
        temp->log_hdr = *logs;
        status = sy_get_log_values (*time_curve);
        if (NOT status)
            logs++;
        while (*logs IS_NOT_EQUAL_TO (LOG_TRACE_STRUCT *)NULL)
            tc_free (*logs++);
        }
    if (status)
        /*      last chance                 */
        /* Try to retrieve the depth_curve and covert it to time_curve  */
        /*                              */
        {
        strcpy (sql_string,
                " ( DEPTH_TYPE = 'T' OR DEPTH_TYPE = 't') AND TRACE = 'DEPTH' ");
        strcat (sql_string, " ORDER BY LAST_UPDATE DESC ");
        status = lg_ltdm_inq_well (uwi, sql_string, &logs);
        if (status)
            return FAIL;
        
        depth_curve = (SYNTHE_LOG *)tc_zalloc (sizeof(SYNTHE_LOG));
        depth_curve->log_hdr = *logs;
        status = sy_get_log_values (depth_curve);
        
        if (NOT status)
            status = sy_desintegration (depth_curve, time_curve);
        
        if (NOT status)
            logs++;
        
        while (*logs IS_NOT_EQUAL_TO (LOG_TRACE_STRUCT *)NULL)
            tc_free (*logs++);
        }
    sy_free_log (depth_curve);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Computing back from the depth_curve the time_curve.

Prototype:
    publicdef INT sy_desintegration(SYNTHE_LOG *depth_curve, SYNTHE_LOG **time_curve);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    depth_curve     -(SYNTHE_LOG *)
    time_curve      -(SYNTHE_LOG **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sy_desintegration (SYNTHE_LOG *depth_curve, SYNTHE_LOG **time_curve)
#else
publicdef INT sy_desintegration (depth_curve, time_curve)
SYNTHE_LOG *depth_curve, **time_curve;
#endif
    {
    
    /*  start declaration   */
    
    INT status;
    INT i, j, k, n_depth, n_time;
    DOUBLE cur_time, cur_depth, t, d;
    DOUBLE t_inc, d_inc;
    LOG_TRACE_STRUCT *log;
    SYNTHE_LOG *temp;
    FLOAT *time_arr, *depth_arr;
    DOUBLE coef;
    
    /*  end declaration     */
    
    n_depth = SY_LOG_NPTS (depth_curve->log_hdr);
    status = tu_unit_conversion ((DOUBLE)(1.), depth_curve->log_hdr->depth_uom, &coef,
                                 "SECONDS");
    if (status)
        goto no_good;
    d_inc = depth_curve->log_hdr->dinc;
    depth_arr = depth_curve->values;
    
    /*  cut the zero padding at the end of the depth_curve  */
    for (i = 0; i < n_depth; i++)
        {
        if (*depth_arr++ EQUALS 0.0)
            break;
        }
    if (i < n_depth)
        {
        n_depth = i;
        depth_curve->log_hdr->base = depth_curve->log_hdr->top + (n_depth - 1) * d_inc;
        }
    if (n_depth < 2)
        goto no_good;
    
    depth_arr = depth_curve->values;
    
    /*  allocations and initializations         */
    
    *time_curve = temp = (SYNTHE_LOG *)tc_zalloc (sizeof(SYNTHE_LOG));
    temp->log_hdr = (LOG_TRACE_STRUCT *)tc_zalloc (sizeof(LOG_TRACE_STRUCT));
    
    temp->log_hdr->dinc = t_inc = 2.;
    strcpy (temp->log_hdr->depth_uom, "METERS");
    strcpy (temp->log_hdr->trace_uom, "SECONDS");
    
    i = depth_arr[0] / t_inc + 1;
    j = depth_arr[n_depth - 1] / t_inc;
    n_time = j - i + 1;
    
    temp->log_hdr->top = i * t_inc;
    temp->log_hdr->base = j * t_inc;
    temp->values = time_arr = (FLOAT *)tc_zalloc (n_time * sizeof(FLOAT));
    
    cur_depth = temp->log_hdr->top;
    cur_time = depth_curve->log_hdr->top - d_inc;
    k = 0;
    
    /*  tranform depth_curve in time_domain log into    */
    /*  time_curve in depth_domain          */
    
    for (i = 0; i < n_time; i++)
        {
        while (depth_arr[k] < cur_depth)
            {
            cur_time += d_inc;
            k++;
            }
        t = depth_arr[k] - depth_arr[k - 1];
        t = d_inc * (cur_depth - depth_arr[k - 1]) / t;
        time_arr[i] = cur_time + t;
        
        cur_depth += t_inc;
        }
    if (k >= n_depth)
        goto no_good;
    
    status = SUCCESS;
    return status;
    
no_good:
    status = FAIL;
    return status;
    }
/* END:     */
