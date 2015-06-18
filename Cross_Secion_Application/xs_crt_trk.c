/* DEC/CMS REPLACEMENT HISTORY, Element XS_CRT_TRK.C*/
/* *2    17-AUG-1990 22:38:03 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_CRT_TRK.C*/
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
    Copy layout tracks linked list into individual well linked list.

Prototype:
    publicdef INT xs_create_tracks(LAYOUT_STRUCT *sample_layout, LAYOUT_STRUCT *copied_layout);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) sample_layout   -(LAYOUT_STRUCT *) The current layout structure.
    (M) copied_layout   -(LAYOUT_STRUCT *) A pointer to layout structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    XS_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_create_tracks (LAYOUT_STRUCT *sample_layout,
                                LAYOUT_STRUCT *copied_layout)
#else
publicdef INT xs_create_tracks (sample_layout, copied_layout)
LAYOUT_STRUCT *sample_layout;
LAYOUT_STRUCT *copied_layout;
#endif
    {
    INT i, totalItems;
    INT status = SUCCESS;
    TRACK_STRUCT *sample_track;
    TRACK_STRUCT *copied_track;
    
    /* error checking.                  */
    
    if (sample_layout EQUALS (LAYOUT_STRUCT *)
        NULL OR copied_layout EQUALS (LAYOUT_STRUCT *)NULL)
        return XS_INVALID_POINTER;
    
    /* initializes a new linked list.     */
    
    status = llinit (&(copied_layout->tracks), 0, sizeof(TRACK_STRUCT *),
                     LL_LIST_SYSTEM);
    if (status < 0)
        return status;
    
    /* if there is no items in the linked */
    /* list, return.                      */
    
    totalItems = llcount (&(sample_layout->tracks));
    if (totalItems <= 0)
        return totalItems;
    
    /* goes througth all items and makes */
    /* a copy of structure for the linked*/
    /* list in the copied_layout.        */
    
    for (i = 0, llfirst (&(sample_layout->tracks), &sample_track); i < totalItems; i++)
        {
        copied_track = (TRACK_STRUCT *)tc_zalloc (sizeof(TRACK_STRUCT));
        hoblockmove (sample_track, copied_track, sizeof(TRACK_STRUCT));
        llinsa (&(copied_layout->tracks), &copied_track);
        llnext (&(sample_layout->tracks), &sample_track);
        }
    return SUCCESS;
    }
/* END:     */
