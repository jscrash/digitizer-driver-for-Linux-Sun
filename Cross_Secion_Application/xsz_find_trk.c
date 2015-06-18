/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FIND_TRK.C*/
/* *2    17-AUG-1990 22:35:50 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:42:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FIND_TRK.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Traverse the Tracks linked list and return a pointer to a requested track. 

Prototype:
    extern INT xsz_find_track(LAYOUT_STRUCT   *p_layout, INT track_number, 
        TRACK_STRUCT **p_track );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_layout        -(LAYOUT_STRUCT *) Pointer to layout structure.
    track_number    -(INT) Requested track number.
    (O) p_track     -(TRACK_STRUCT **) (Returned) Pointer to track. 

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xsz_find_track (LAYOUT_STRUCT *p_layout, INT track_number,
                           TRACK_STRUCT **p_track)
#else
extern INT xsz_find_track (p_layout, track_number, p_track)
LAYOUT_STRUCT *p_layout;
INT track_number;
TRACK_STRUCT **p_track;
#endif
    {
    INT ntrks, i;
    TRACK_STRUCT *ptrk;
    
    ntrks = llcount (&p_layout->tracks);    /* Find the total number of trks.*/
    llfirst (&p_layout->tracks, &ptrk); /* Get the first track.          */
    
    for (i = 1; i <= ntrks; i++)
        {
        if (ptrk->number == track_number)   /* Is this the requested track?  */
            {
            *p_track = ptrk;        /* Return pointer to this track. */
            return SUCCESS;
            }
        else
            llnext (&p_layout->tracks, &ptrk);  /* Get the next track.         */
        }
    return FAIL;                    /* No track match was found.     */
    }
/* END:     */
