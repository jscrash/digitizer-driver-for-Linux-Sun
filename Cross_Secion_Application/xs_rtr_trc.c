/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_TRC.C*/
/*  3    27-FEB-1992 09:30:06 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *2    17-AUG-1990 22:43:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_TRC.C*/
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
#include "esi_xs_defs.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Copy layout traces linked list into individual well linked list.

Prototype:
    publicdef INT xs_retrieve_traces(LAYOUT_STRUCT *sample_layout, LAYOUT_STRUCT *copied_layout);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) sample_layout   -(LAYOUT_STRUCT *) The current layout structure.
    (M) copied_layout   -(LAYOUT_STRUCT *) A pointer to layout structure.

Return Value/Status:
    SUCCESS - Successful completion.
    XS_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_retrieve_traces (LAYOUT_STRUCT *sample_layout,
                                  LAYOUT_STRUCT *copied_layout)
#else
publicdef INT xs_retrieve_traces (sample_layout, copied_layout)
LAYOUT_STRUCT *sample_layout;
LAYOUT_STRUCT *copied_layout;
#endif
    {
    INT i, totalItems;
    INT status = SUCCESS;
    CHAR text[STMAX];
    LP_TRACE_STRUCT *sample_trace;
    LP_TRACE_STRUCT *copied_trace;
    
    /* error checking.                  */
    
    if (sample_layout EQUALS (LAYOUT_STRUCT *)
        NULL OR copied_layout EQUALS (LAYOUT_STRUCT *)NULL)
        return XS_INVALID_POINTER;
    
    /* initialize a linked list.          */
    
    llinit (&(copied_layout->traces), 0, sizeof(LP_TRACE_STRUCT *), LL_LIST_SYSTEM);
    
    /* if there is no items in the linked */
    /*   list, return.                    */
    
    totalItems = llcount (&(sample_layout->traces));
    if (totalItems <= 0)
        return SUCCESS;
    
    /* goes througth all items and makes */
    /* a copy of structure for the linked*/
    /* list in the copied_layout.        */
    
    for (i = 0, llfirst (&(sample_layout->traces), &sample_trace); i < totalItems; i++)
        {
        
        copied_trace = (LP_TRACE_STRUCT *)tc_zalloc (sizeof(LP_TRACE_STRUCT));
        hoblockmove (sample_trace, copied_trace, sizeof(LP_TRACE_STRUCT));
        
        /* copy the UWI from the WELL_STRUCT */
        
        strcpy (copied_trace->trace_struct.uwi, copied_layout->pwell->uwi);
        
        /* copy the composite trace name.    */
        
        strcpy (copied_trace->trace_struct.trace_name, copied_trace->name);
        
        /* retrieve the log trace.           */
        /* update the selecting TOP and BASE.*/
        copied_trace->trace_struct.top = copied_layout->interval.ltdm_data_top;
        copied_trace->trace_struct.base = copied_layout->interval.ltdm_data_base;
        copied_trace->trace_struct.non_append_null =
                                                     copied_trace->trace_struct.
                                                     multi_strokes = TRUE;
        if ((copied_trace->trace_struct.top EQUALS copied_trace->trace_struct.
             base) AND copied_trace->trace_struct.top EQUALS 0.0)
            {
            sprintf (text, "Start retrieving entire trace, Uwi = %s, trace = %s.",
                     copied_trace->trace_struct.uwi,
                     copied_trace->trace_struct.trace_name);
            }
        else
            {
            sprintf (text, "Start retrieving Uwi = %s, trace = %s from %f to %f ",
                     copied_trace->trace_struct.uwi,
                     copied_trace->trace_struct.trace_name,
                     copied_trace->trace_struct.top, copied_trace->trace_struct.base);
            }
        am_message (AM_DIALOG, text);
        status = lg_ltdm_get_trace (&(copied_trace->trace_struct));
        if ((copied_trace->trace_struct.top EQUALS copied_trace->trace_struct.
             base) AND copied_trace->trace_struct.top EQUALS 0.0)
            {
            sprintf (text, "Finish retrieving entire trace, Uwi = %s, trace = %s.",
                     copied_trace->trace_struct.uwi,
                     copied_trace->trace_struct.trace_name);
            }
        else
            {
            sprintf (text, "Finish retrieving Uwi = %s, trace = %s from %f to %f ",
                     copied_trace->trace_struct.uwi,
                     copied_trace->trace_struct.trace_name,
                     copied_trace->trace_struct.top, copied_trace->trace_struct.base);
            }
        am_message (AM_DIALOG, text);
        if (status < 0)
            {
            tc_free (copied_trace);
            }
        else
            llinsa (&(copied_layout->traces), &copied_trace);
        
        llnext (&(sample_layout->traces), &sample_trace);
        }
    return SUCCESS;
    }
/* END:     */
