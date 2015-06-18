/* DEC/CMS REPLACEMENT HISTORY, Element SE_BLD_NLIDS.C*/
/* *6    20-DEC-1991 15:32:36 CHEW "(SPR 1) Append data_type, data_class to nlist id"*/
/* *5    16-OCT-1990 15:04:14 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  3B2  15-OCT-1990 17:25:43 GILLESPIE "Merge Ernie deltas"*/
/*  3B1  12-OCT-1990 17:52:23 GILLESPIE "Merge Ernie Deltas"*/
/* *4    17-AUG-1990 22:15:39 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    29-JUN-1990 19:03:34 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *2     2-JUN-1990 17:00:42 GILLESPIE "(SPR 1) Add support for fault traces"*/
/* *1    26-JUL-1989 00:15:19 GILLESPIE "New seismic system mods"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SE_BLD_NLIDS.C*/
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

#include "esi_c_lib.h"
#include "esi_se.h"

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#include "esi_se_err.h"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine creating a file name using seismic line name.                         */
/*    Used to create an nlist filename from a linename.                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT se_bld_nlist_ids(FULL_LINE_NAME *line, VOIDPTR context, SEIS_NLIST_DATA_TYPE data_type, */
/*        NLIST_ID nlist_id, FILENAME nlist_file, FILENAME full_filename);          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    line            -(FULL_LINE_NAME *)                                           */
/*    context         -(VOIDPTR)                                                    */
/*    data_type       -(SEIS_NLIST_DATA_TYPE)                                       */
/*    nlist_id        -(NLIST_ID)                                                   */
/*    nlist_file      -(FILENAME) Name of n-List without directory.                 */
/*    full_filename   -(FILENAME) Fully qualified file name (nlist file with path). */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    This function returns the status of ho_add_path.                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT se_bld_nlist_ids (FULL_LINE_NAME *line, VOIDPTR context,
                                SEIS_NLIST_DATA_TYPE data_type, NLIST_ID nlist_id,
                                FILENAME nlist_file, FILENAME full_filename)
#else
publicdef INT se_bld_nlist_ids (line, context, data_type, nlist_id, nlist_file,
                                full_filename)
FULL_LINE_NAME *line;
VOIDPTR context;
SEIS_NLIST_DATA_TYPE data_type;
NLIST_ID nlist_id;
FILENAME nlist_file;                /* name of n-List without directory             */
FILENAME full_filename;             /* fully qualified file name (nlist file with path) */
#endif
    {
    INT id;
    PATHNAME directory;
    SEIS_SURFACE_HDR *psurf;
    SEIS_FAULT_TRACE *pFTrace;
    
    /* If necessary, manufacture the n-List ID                                      */
    
    if (IS_EMPTY_STRING (nlist_id))
        {
        switch (data_type)
            {
        default:
            return FAIL;
        case SEIS_HDR_DATA:
            sprintf (nlist_id, "%s.HDR", line->line_name);
            break;
        case SEIS_SHOT_LABEL_DATA:
            sprintf (nlist_id, "%s.%s", line->line_name, "SHOT_LABELS_FROM_SLE");
            break;
        case SEIS_CDPSP_DATA:
            sprintf (nlist_id, "%s.CDPSP", line->line_name);
            break;
        case SEIS_XY_DATA:
            sprintf (nlist_id, "%s.XY", line->line_name);
            break;
        case SEIS_ALTIM_DATA:
            sprintf (nlist_id, "%s.ALTIM", line->line_name);
            break;
        case SEIS_TRACE_DATA:
            sprintf (nlist_id, "%s", context);
            break;
        case SEIS_LATLONG_DATA:
            sprintf (nlist_id, "%s.LATLONG", line->line_name);
            break;
        case SEIS_OPTIM_XY_DATA:
            sprintf (nlist_id, "%s.OPTIM_XY", line->line_name);
            break;
        case SEIS_SURFACE_DATA:
            psurf = (SEIS_SURFACE_HDR *)context;
            sprintf (nlist_id, "%s.%s.%s.%d.%d;%d", line->survey,
                 line->line_name, psurf->source, psurf->data_class,
                 psurf->data_type, psurf->version);
            break;
        case SEIS_FTRACE_DATA:
            pFTrace = (SEIS_FAULT_TRACE *)context;
            /* sprintf(nlist_id, "%s.%s", pFTrace -> fault_name,                    */
            /*                            pFTrace -> source);                       */
            dr_new_project_id (&id);
            sprintf (nlist_id, "%d", id);
            break;
            }
        }
    /* now do the same sort of thing for the file                                   */
    /* name                                                                         */
    switch (data_type)
        {
    default:
    case SEIS_CDPSP_DATA:
    case SEIS_HDR_DATA:
    case SEIS_OPTIM_XY_DATA:
    case SEIS_LATLONG_DATA:
    case SEIS_XY_DATA:
        /* This data has storage based on the SURVEY                                */
        /* name                                                                     */
        strcpy (directory, "ESI$SEISMIC_HDR");
        
        if (IS_EMPTY_STRING (nlist_file))
            {
            /* If the SURVEY is NULL (which it should never be),                    */
            /* then use the string "NULL"                                           */
            if (line->survey == NULL)
                {
                strcpy (nlist_file, "NULL.HDR");
                }
            else
                {
                sprintf (nlist_file, "%s.hdr", line->survey);
                }
            ts_sto_lower ((CHAR *)nlist_file);
            }
        break;
        
    case SEIS_SHOT_LABEL_DATA:
        /* This data has storage based on the SURVEY                                */
        /* name                                                                     */
        strcpy (directory, "ESI$SEISMIC_PREFS");
        
        if (IS_EMPTY_STRING (nlist_file))
            {
            /* If the SURVEY is NULL (which it should never be),                    */
            /* then use the string "NULL"                                           */
            if (line->survey == NULL)
                {
                strcpy (nlist_file, "NULL.HDR");
                }
            else
                {
                sprintf (nlist_file, "%s.hdr", line->survey);
                }
            ts_sto_lower ((CHAR *)nlist_file);
            }
        break;
        
    case SEIS_SURFACE_DATA:
        /* for surfaces, the context is the SURFACE                                 */
        /* NAME...                                                                  */
        strcpy (directory, "ESI$SEISMIC_SURFACE");
        
        if (IS_EMPTY_STRING (nlist_file))
            {
            psurf = (SEIS_SURFACE_HDR *)context;
            strcpy (nlist_file, psurf->name);
            ts_sto_lower (ts_noblanks ((CHAR *)nlist_file));
            strcat ((char *)nlist_file, ".surf");
            }
        break;
        
    case SEIS_FTRACE_DATA:
        /* for fault traces, the context is the SURFACE                             */
        /* NAME...                                                                  */
        strcpy (directory, "ESI$FAULT_TRACE");
        
        if (IS_EMPTY_STRING (nlist_file))
            {
            pFTrace = (SEIS_FAULT_TRACE *)context;
            strcpy (nlist_file, pFTrace->hor_code);
            ts_sto_lower (ts_noblanks ((CHAR *)nlist_file));
            strcat ((char *)nlist_file, ".flt");
            }
        break;
        
    case SEIS_TRACE_DATA:
        /* file name generated from line name                                       */
        strcpy (directory, "ESI$SEISMIC_SEGY");
        
        if (IS_EMPTY_STRING (nlist_file))
            {
            sprintf (nlist_file, "%s.segy", line->line_name);
            ts_sto_lower ((CHAR *)nlist_file);
            }
        break;
        }
    return ho_add_path (directory, nlist_file, MAXPATHLENGTH, full_filename);
    }
/* END:                                                                             */
