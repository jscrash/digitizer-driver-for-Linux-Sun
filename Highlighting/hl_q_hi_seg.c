/* DEC/CMS REPLACEMENT HISTORY, Element HL_Q_HI_SEG.C*/
/* *3    17-AUG-1990 21:49:44 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    11-AUG-1989 10:24:49 JULIAN "(SPR -1) statement not reached"*/
/* *1    19-JUN-1989 12:55:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_Q_HI_SEG.C*/
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

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#define HL_DUMP_WM_TREE  FALSE

/* wm classes */
#define MAP_WINDOW_CLASS     WM_CLASS_MAP
#define WELL_SEGMENT_CLASS   WM_CLASS_WELL_SYMBOL
#define SEIS_SEGMENT_CLASS   WM_CLASS_SEISMIC
static INT gbWindowId = 0;
static INT gbGroupClass = 0;
static INT gbSegmentClass = 0;
static INT gbLowestSegment = 0;

static INT appSave, winSave, tnrSave, groSave;

/* Function Description -----------------------------------------------------
Description:
    This routine searches for the lowest segment id
    in the current map window and stores it in the
    given select list.

Prototype:
    publicdef INT hl_query_hi_seg_id(SELECT_STRUCT *pslist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pslist          -(SELECT_STRUCT *) Select list pointer.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hl_query_hi_seg_id (SELECT_STRUCT *pslist)
#else
publicdef INT hl_query_hi_seg_id (pslist)
SELECT_STRUCT *pslist;
#endif
    {
    INT i, ii,                      /* Loop control */
        group,                      /* WM group id */
        length,                     /* length of QS strings */
        segment,                    /* WM segment id */
        ifExists,                   /* TRUE when Map window attached to */
        numGroups,                  /* total WM groups to loop on & check */
        numSegments,                /* total WM segments per group */
        totalSegments,              /* total running count of ALL segments */
        well_group_class,           /* Class id from DB */
        status,                     /* general status flag */
        seismic_group_class,        /* Class id from DB */
        wm_status,                  /* WM status return */
        ll_status;                  /* linked list status return */
    
    LISTHEAD groupList,             /* linked list of WM groups */
        segmentList;                /* linked list of WM segments */
    
    CHAR *dummy_pchar;
    MAP_STRUCTURE *map;
    
    /* Start code...*/
    
    /* for debugging, have the entire WM tree printed out if requested */
#if HL_DUMP_WM_TREE == TRUE
    wmoshowall ();
#endif
    
    /* get out WM Classes from the DB */
    
    switch (pslist->data_type)
        {
    case WELLS_DATA_TYPE:
        gbGroupClass = WM_CLASS_WELL_HILITE;
        gbSegmentClass = WELL_SEGMENT_CLASS;
        break;
        
    case SEISMIC_DATA_TYPE:
        gbGroupClass = WM_CLASS_SEISMIC_HILITE;
        gbSegmentClass = SEIS_SEGMENT_CLASS;
        break;
        
    default:
        return FAIL;
        }
    /* clear global stuff */
    
    gbWindowId = 0;
    gbLowestSegment = 0;
    
    /* initialize local lists */
    
    ll_status = llinit (&groupList, 0, sizeof(INT), 0);
    ll_status = llinit (&segmentList, 0, sizeof(INT), 0);
    
    /* save wm state and close out to application */
    
    wm_status = wmowhere (&appSave, &winSave, &tnrSave, &groSave, &wm_status);
    
    /* query system state variables */
    
    if (mp_get_current_map (&map) != SUCCESS)
        {
        return FAIL;
        }
    /* map window id */
    
    gbWindowId = pslist->window_id;
    
    wm_status = wmoexists (appSave, gbWindowId, 0, 0, &ifExists);
    if (!(ifExists))
        {
        return FAIL;
        }
    /* attach to top level */
    
    wm_status = wmoattach (appSave, 0, 0, 0, &wm_status);
    if (wm_status != SUCCESS)
        {
        return FAIL;
        }
    /* select the lowest segment id. */
    
    totalSegments = 0;
    
    wm_status = wmwopen (gbWindowId);
    wm_status = wmginqlist (gbGroupClass, &groupList);
    numGroups = llcount (&groupList);
    
    if (numGroups > 0)
        {
        for (i = 0; i < numGroups; i++)
            {
            ll_status = (i == 0) ? llfirst (&groupList,
                                            &group) : llnext (&groupList, &group);
            
            /* make sure the linked list is valid */
            if (ll_status < 0 OR (ll_status == FAIL))
                {
                return FAIL;
                }
            /* Open this group and get it's segment list */
            wm_status = wmgopen (group);
            wm_status = wmsinqlist (gbSegmentClass, &segmentList);
            
            numSegments = llcount (&segmentList);
            if (numSegments < 0)
                {
                return FAIL;
                }
            if (numSegments > 0)
                {
                totalSegments += numSegments;
                
                for (ii = 0; ii < numSegments; ii++)
                    {
                    ll_status = (ii == 0) ? llfirst (&segmentList,
                                                     &segment) :
                                llnext (&segmentList, &segment);
                    
                    if (ll_status < 0 OR (ll_status == FAIL))
                        {
                        return FAIL;
                        }
                    /* if this is the lowest seg. so far, save it as thus */
                    if (gbLowestSegment)
                        {
                        gbLowestSegment = MIN (gbLowestSegment, segment);
                        }
                    else
                        {
                        gbLowestSegment = segment;
                        }
                    }
                }
            /* done with this group.  Free up our local segment tree */
            ll_status = llfree (&segmentList);
            wm_status = wmgclose (group);
            pslist->segment_id = gbLowestSegment;
            }
        }
    /* done with everything. Free up the local work tree */
    ll_status = llfree (&groupList);
    
    wm_status = wmwclose (gbWindowId);
    
    if (totalSegments <= 0)
        {
        return FAIL;
        }
    status = SUCCESS;
    return status;
    }
/* END:     */
