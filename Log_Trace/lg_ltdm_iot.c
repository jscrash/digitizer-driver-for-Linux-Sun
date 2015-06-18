/* DEC/CMS REPLACEMENT HISTORY, Element LG_LTDM_IOT.C*/
/* *7    17-JUL-1991 16:27:20 JANTINA "(SPR 0) Get top,base,increment from nlist"*/
/* *6    26-OCT-1990 15:35:07 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *5    23-AUG-1990 17:57:30 JESSIE "(SPR 1) fix include files"*/
/* *4    29-MAR-1990 16:57:40 MING "(SPR 5173) GULF bug fix"*/
/* *3    14-SEP-1989 16:17:42 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    27-JUL-1989 23:41:04 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:05:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_LTDM_IOT.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* ******************************************************************************   */
/*                                                                                  */
/*    MODULE:   LG_LTDM_IO_TOOLS                                                    */
/*                                                                                  */
/*    DESCRIPTION:                                                                  */
/*                                                                                  */
/*         A module of routines od Log Trace Data Manager.  The function of this    */
/*         module is to perform Input/Output funcion for N-list.                    */
/*                                                                                  */
/*    AUTHOR:   James Chang                                                         */
/*                                                                                  */
/*    ***************************************************************************   */

#include "esi_lg_defs.h"

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif

#ifndef ESI_LG_ERR_H
#include "esi_lg_err.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

/* ******************************************************************************   */
/*                                                                                  */
/*    ROUTINE:  LG_LTDM_GET_TRACE.                                                  */
/*                                                                                  */
/*    DESCRIPTION                                                                   */
/*         Parses the trace name, reads the complete log trace according to the     */
/*         search rules specified in the LOG_TRACE_STRUCT structure and then builds */
/*         an N-list of the exact interval requested.  If the requested interval    */
/*         top and base are NULL, the complete log trace is returned.               */
/*                                                                                  */
/*    RETURNED STATUS                                                               */
/*         SUCCESS --- O.K.                                                         */
/*         LG_TRACE_NOT_FOUND --- The trace is not found or it exists but is not    */
/*                                     flagged as "shared".                         */
/*                                                                                  */
/*         LG_TRACE_OUTSIZE_AREA --- The trace is exists ans is flagged as "shared" */
/*                                      but is outside the current project area.    */
/*                                                                                  */
/*         OTHERS --- returned from lg_ltdm_parser().                               */
/*                                                                                  */
/*    ***************************************************************************   */
publicdef INT lg_ltdm_get_trace (trace) /*****************************************************************************/

LOG_TRACE_STRUCT *trace;            /*%P pointer of LOG_TRACE_STRUCT.               */
    {
    PROJECT_DEFAULT *proj;
    DOUBLE conv;
    INT status = SUCCESS;
    FLOAT inputTop, inputBase;
    FLOAT dummy;
    FILENAME filename;
    /* ***************************************************************************  */
    /*   parsing the composite trace name first,                                    */
    /*   if the returned status equals SUCCESS,                                     */
    /*   then gets trace.                                                           */
    
    status = lg_ltdm_parser (trace);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    
    /*   get the nlist name and the pathname of                                     */
    /*   the trace file.                                                            */
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    inputTop = trace->top;
    inputBase = trace->base;
    
    if (trace->version > 0)
        {
        /* if there is a version number, retrieve                                   */
        /* the data in two steps.                                                   */
        status = lgz_get_trace_names (trace);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        
        status = lgz_get_trace_data (trace);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        }
    else
        {
        /* if there is no version number, for speeding                              */
        /* up, we retrieve all data together.                                       */
        /* GULF FIX TO check quality code                                           */
        status = lgz_get_tr_all_data_q (trace);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            status = lgz_get_trace_all_data (trace);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                return status;
            }
        }
    status = tu_unit_conversion (1.0, trace->depth_uom, &conv, proj->unit.vertical);
    inputTop = inputTop / conv;
    inputBase = inputBase / conv;
    
    /* If retrieve the whole trace, need not to                                     */
    /* set the non_null_top and non_null_base.                                      */
    /*                                                                              */
    /* * The filename in the trace structure does not include the path - add it in  */
    /* * before reading the trace                                                   */
    /*                                                                              */
    
    ho_add_path ("ESI$LOGS", trace->filename, sizeof(FILENAME), filename);

    /* get top, base and dinc values from nlist */

    status = nl_read_nlist(&(trace->ptrace),filename,trace->nlist_id);
    if(status) return status;
    status = lgz_update_ranges(trace);
    
    if (inputTop EQUALS 0.0 AND inputBase EQUALS 0.0)
        status = lgz_get_trace (&(trace->ptrace), filename, trace->nlist_id, 1,
                                trace->top, trace->base,
				&(trace->non_null_top),&(trace->non_null_base),
                                trace->multi_strokes, trace->non_append_null);
    else
        status = lgz_get_trace (&(trace->ptrace), filename, trace->nlist_id, 1,
                                inputTop, inputBase, &(trace->non_null_top),
                                &(trace->non_null_base), trace->multi_strokes,
                                trace->non_append_null);
    
    trace->top = inputTop;
    trace->base = inputBase;
    
    if (status < 0)
        return LG_TRACE_NOT_FOUND;
    
    if (status > 0)
        return LG_TRACE_OUTSIZE_AREA;
    
    return status;
    }
/* END:                                                                             */
