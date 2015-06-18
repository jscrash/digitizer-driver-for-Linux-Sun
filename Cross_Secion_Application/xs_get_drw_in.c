/* DEC/CMS REPLACEMENT HISTORY, Element XS_GET_DRW_IN.C*/
/* *6    17-AUG-1990 22:39:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    20-SEP-1989 07:52:53 GORDON "(SPR 100) undo last checkin"*/
/* *4    19-SEP-1989 13:40:21 GILLESPIE "(SPR 100) GULF MODS"*/
/* *3    18-SEP-1989 12:45:04 JULIAN "Gulf mods under SPR 100"*/
/* *2    14-SEP-1989 16:24:08 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:44:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_GET_DRW_IN.C*/
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
#include "esi_xs_gr.h"

/* Function Description -----------------------------------------------------
Description:
    Compile info used during drawing ahead of time.

    Analyze all wells to fillout pd->XX where XX is:
        max_tname_len       The length of the longest trace name.
        max_tname_ptrack    The track in which this trace lives.
        most_traces_ptrack  The track that holds the most traces.
        most_traces_count   The number of traces in that track.
        cs                  The SB Tname annot. text size.
        region_ht           The height of each trace region in SB.

    The information compiled here is used repetitively in the trace
    scale block drawing routine.

Prototype:
    publicdef INT xs_gr_get_drawinfo(XS_STRUCT *pxs,XS_DIS *pd);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    pd              -(XS_DIS *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    The calling routine must have initialized the pd struct.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_gr_get_drawinfo (XS_STRUCT *pxs, XS_DIS *pd)
#else
publicdef INT xs_gr_get_drawinfo (pxs, pd)
XS_STRUCT *pxs;
XS_DIS *pd;
#endif
    {
    LAYOUT_STRUCT *pls;
    TRACK_STRUCT *pt;
    LP_TRACE_STRUCT *ptrace;
    
    INT l, j, jj;
    INT ntraces, ntracks, nlyts, count, len, st;
    
    DOUBLE sb_ht, max_sb_ht, min_sb_ht;
    
    /******************* Start Code ********************/
    
    count = len = 0;
    st = SUCCESS;
    pd->cs = 9999.0;                /* reset cs to too big and work it down */
    min_sb_ht = 9999.0;
    max_sb_ht = 0.0;
    
    llfirst (&pxs->drawing.log_plots, (BYTE *) & pls);
    sb_ht = pls->log_hdr.text_height;   /* default.  May not even be used */
    
    nlyts = llcount (&pxs->drawing.log_plots);
    
    if (nlyts <= 0)
        return FAIL;
    
    /*  Loop through all the wells.   */
    for (l = 0; l < nlyts; l++)
        {
        st = (l) ? llnext (&pxs->drawing.log_plots,
                           (BYTE *) & pls) : llfirst (&pxs->drawing.log_plots,
                                                      (BYTE *) & pls);
        
        min_sb_ht = MIN (min_sb_ht, pls->scale_block.height);
        max_sb_ht = MAX (max_sb_ht, pls->scale_block.height);
        
        ntraces = llcount (&pls->traces);
        ntracks = llcount (&pls->tracks);
        
        for (j = 0; j < ntracks; j++)
            {
            st = (j) ? llnext (&pls->tracks,
                               (BYTE *) & pt) : llfirst (&pls->tracks, (BYTE *) & pt);
            
            pd->max_track_wid = MAX (pd->max_track_wid, pt->width);
            
            for (count = jj = 0; jj < ntraces; jj++)
                {
                st = (jj) ? llnext (&pls->traces,
                                    (BYTE *) & ptrace) : llfirst (&pls->traces,
                                                                  (BYTE *) & ptrace);
                
                if (ptrace->track_num != pt->number)
                    continue;
                
                len = MIN (MAX_TRACE_NAME_LEN,
                           strlen (ptrace->trace_struct.trace_name));
                len += 12;
                
                /* count number of traces on this track */
                count++;
                pd->cs = MIN (pd->cs, (pt->width / (DOUBLE)len));
                
                if (len > pd->max_tname_len)
                    {
                    pd->max_tname_len = len;
                    pd->max_tname_ptrack = pt;
                    }
                }
            /* End traces loop */
            
            /* keep a tally of the most traces */
            /* found in any given track */
            if (count > pd->most_traces_count)
                {
                pd->most_traces_count = count;
                pd->most_traces_ptrack = pt;
                }
            }
        /* End tracks loop */
        }
    /* End layouts loop */
    
    sb_ht = MAX (max_sb_ht, min_sb_ht);
    pd->region_ht = (pd->most_traces_count == 0) ?
                    sb_ht : (sb_ht / (DOUBLE)pd->most_traces_count);
    
    return st;
    }
/* END:     */
