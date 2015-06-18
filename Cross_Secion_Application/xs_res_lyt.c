/* DEC/CMS REPLACEMENT HISTORY, Element XS_RES_LYT.C*/
/* *2    17-AUG-1990 22:43:03 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RES_LYT.C*/
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

#include "esi_xs_defs.h"

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To resolve the x,y origins of various layout sub-     
    structures, ie. the header block, the scale block,    
    the tracks etc.. An origin at the bottom left corner  
    of the entire layout is assumed ie. the bottom left   
    corner of the text block.  Must have previously cal-  
    led XS_HDR_SIZE, XS_SCALE_BLK_SIZE, XS_CHG_TRACK_INFO 
    and in future XS_RESOLVE_TEXT_BLK.                    

Prototype:
    publicdef INT xs_resolve_layout(LISTHEAD *lplot_head);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) lplot_head  -(LISTHEAD *) Pointer to layout LINKLIST.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_resolve_layout (LISTHEAD *lplot_head)
#else
publicdef INT xs_resolve_layout (lplot_head)
LISTHEAD *lplot_head;
#endif
    {
    LAYOUT_STRUCT *pls;
    TRACK_STRUCT *p_track;
    INT no_layouts, no_tracks;
    DOUBLE xorigin;
    INT i, j;
    INT flag;
    
    no_layouts = llcount (lplot_head);  /* get number of layouts present*/
    
    /* loop on layouts, setting width for each layout according to tracks         */
    
    for (i = 0; i < no_layouts; i++)
        {
        flag = (i == 0) ? llfirst (lplot_head, &pls) : llnext (lplot_head, &pls);
        
        xorigin = pls->xorigin;
        
        /* set text block origins  */
        pls->text_block.xorigin = xorigin;
        
        no_tracks = llcount (&pls->tracks);
        llfirst (&pls->tracks, &p_track);
        for (j = 1; j <= no_tracks; j++)
            {
            /* Convert relative X origin
                                       to absolute X origin.   */
            p_track->xorigin = p_track->xorigin + xorigin;
            llnext (&pls->tracks, &p_track);
            }
        /* end of track loop       */
        
        pls->scale_block.xorigin = xorigin; /* set scale blk origins   */
        
        pls->log_hdr.xorigin = xorigin; /* set log header origins  */
        
        }
    return(SUCCESS);
    }
/* END:     */
