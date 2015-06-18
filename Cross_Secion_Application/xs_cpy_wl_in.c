/* DEC/CMS REPLACEMENT HISTORY, Element XS_CPY_WL_IN.C*/
/* *2    17-AUG-1990 22:37:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_CPY_WL_IN.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_XS_ERR_H

#include "esi_xs_err.h"

#endif

#ifndef ESI_XS_MSG_H

#include "esi_xs_msg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To supervise the transcription of changes to the master   
    layout into the individual well layouts.                   

Prototype:
    extern INT xs_copy_well_info(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) The pointer to the cross section structure.          

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xs_copy_well_info (XS_STRUCT *pxs)
#else
extern INT xs_copy_well_info (pxs)
XS_STRUCT *pxs;
#endif
    {
    LAYOUT_STRUCT *p_layout;
    TRACK_STRUCT *ptrk;
    TRACK_STRUCT *pmstr_trk;
    LP_TRACE_STRUCT *ptrace;
    LP_TRACE_STRUCT *pmstr_trace;
    static BOOL track_copied;
    static INT i;
    static INT j;
    static INT k;
    static INT l;
    static INT m;
    static INT num_wells;
    static INT num_tracks;
    static INT num_traces;
    static INT total_tracks;
    static INT total_traces;
    static INT status;
    static CHAR message[120];
    
    PROGNAME ("xs_copy_well_info");
    
    num_wells = llcount (&pxs->drawing.log_plots);
    total_tracks = llcount (&(pxs->plo->tracks));
    total_traces = llcount (&(pxs->plo->traces));
    
    /* Verify that tracks are defined in master */
    /* There should always be at least one track*/
    /* because the track_def_cdlg will not allow*/
    /* the user to delete the last track.       */
    if (total_tracks <= 0)
        {
        status = am_message (AM_ALERT, mg_message (XS_NO_TRACKS));
        return FAIL;
        }
    /* Verify that traces are defined in master */
    /* This is no longer a hard requirement, but*/
    /* the user should be advised anyway.       */
    if (total_traces <= 0)
        {
        status = am_message (AM_STATUS, mg_message (XS_NO_TRACES_REQUESTED));
        }
    for (i = 0; i < num_wells; i++)
        {
        /* Get a logplot from the linked list.  */
        
        status = (i == 0) ? llfirst (&pxs->drawing.log_plots,
                                     &p_layout) : llnext (&pxs->drawing.log_plots,
                                                          &p_layout);
        
        /* Copy any attribute changes into it.  */
        
        status = xsz_copy_layout (pxs->plo, p_layout);
        
        
        num_tracks = llcount (&p_layout->tracks);
        
        for (j = 0; j < total_tracks; j++)
            {
            /* Get a track from the master layout.  */
            
            status = (j == 0) ? llfirst (&pxs->plo->tracks,
                                         &pmstr_trk) : llnext (&pxs->plo->tracks,
                                                               &pmstr_trk);
            
            /* Find matching track in well's layout */
            /* and copy info from master layout.    */
            
            track_copied = FALSE;
            for (k = 0; k < num_tracks; k++)
                {
                status = (k == 0) ? llfirst (&p_layout->tracks,
                                             &ptrk) : llnext (&p_layout->tracks,
                                                              &ptrk);
                
                if (pmstr_trk->number == ptrk->number)
                    {
                    status = xsz_copy_track_info (pmstr_trk, ptrk);
                    track_copied = TRUE;
                    if (pxs->track_width_scale_units == XS_CENTIMETER_UNITS)
                        {
                        ptrk->xorigin = ptrk->xorigin * 0.3937;
                        ptrk->width = ptrk->width * 0.3937;
                        }
                    }
                }
            /* A track needs to be added.  */
            if (track_copied != TRUE)
                {
                ptrk = (TRACK_STRUCT *)tc_alloc (sizeof(TRACK_STRUCT));
                status = xsz_copy_track_info (pmstr_trk, ptrk);
                ptrk->number = pmstr_trk->number;
                status = llappnd (&p_layout->tracks, &ptrk);
                if (pxs->track_width_scale_units == XS_CENTIMETER_UNITS)
                    {
                    ptrk->xorigin = ptrk->xorigin * 0.3937;
                    ptrk->width = ptrk->width * 0.3937;
                    }
                }
            }
        num_traces = llcount (&p_layout->traces);
        
        for (l = 0; l < num_traces; l++)
            {
            /* Get a trace from the logplot layout. */
            
            status = (l == 0) ? llfirst (&p_layout->traces,
                                         &ptrace) : llnext (&p_layout->traces,
                                                            &ptrace);
            
            /* Find the matching trace in the master*/
            /* and copy the info into the well.     */
            
            for (m = 0; m < total_traces; m++)
                {
                status = (m == 0) ? llfirst (&pxs->plo->traces,
                                             &pmstr_trace) : llnext (&pxs->plo->traces,
                                                                     &pmstr_trace);
                
                if (pmstr_trace->number == ptrace->number)
                    status = xsz_copy_trace_info (pmstr_trace, ptrace);
                }
            }
        }
    return SUCCESS;
    }
/* END:     */
