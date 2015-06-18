/* DEC/CMS REPLACEMENT HISTORY, Element HL_QUERY_SEG.C*/
/* *4     7-JUN-1991 13:53:01 MING "(SPR 0) change SEISMIC TIME to SEISMIC DATA"*/
/* *3    17-AUG-1990 21:49:34 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    11-AUG-1989 10:25:27 JULIAN "(SPR -1) statement not reached"*/
/* *1    19-JUN-1989 12:54:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_QUERY_SEG.C*/
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

#include "esi_gl_defs.h"

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_MD_BASIS_X
#include "esi_md_basis.x"
#endif

#ifndef ESI_OV_H
#include "esi_ov.h"
#endif

#define HL_DUMP_WM_TREE  FALSE

/* Function Description -----------------------------------------------------
Description:
    This routine searches for the lowest segment id
    in the current map window and stores it in the
    given select list.

Prototype:
    publicdef INT hl_query_seg_id(SELECT_STRUCT *pslist);

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
publicdef INT hl_query_seg_id (SELECT_STRUCT *pslist)
#else
publicdef INT hl_query_seg_id (pslist)
SELECT_STRUCT *pslist;
#endif
    {
    PROGNAME ("hl_query_seg_id");
    INT i, ii, j;                   /* Loop control */
    INT row;                        /* row pointer into compiled mapdef struct */
    INT nclass;                     /* Number of classes for the data type */
    INT segment;                    /* WM segment id */
    INT ifExists;                   /* TRUE when Map window attached to */
    INT verb_ndx[10];               /* Index into overlay_verbs array */
    INT old_verb_ndx;               /* Verb index for last display line */
    INT numSegments;                /* total WM segments per group */
    INT totalSegments = 0;          /* total running count of ALL segments */
    INT status;                     /* general status flag */
    INT data_class;
    INT LowestSegment = 0;
    INT appSave, winSave, tnrSave, groSave;
    
    LISTHEAD segmentList;           /* linked list of WM segments */
    
    CHAR *dummy_pchar;
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    
    /***************************************************************************/
    
    /*-----------------------------------------------------------------*/
    /* FOR DEBUGGING, HAVE THE ENTIRE WM TREE PRINTED OUT IF REQUESTED */
#if HL_DUMP_WM_TREE == TRUE
    wmoshowall ();
#endif
    /*-----------------------------------------------------------------*/
    
    /* * ESTABLISH STATIC DATA TYPE SPECIFIC PARAMETERS * */
    /* -------------------------------------------------- */
    
    switch (pslist->data_type)
        {
    case WELLS_DATA_TYPE:
        data_class = WM_CLASS_WELL_SYMBOL;
        nclass = 3;
        verb_ndx[0] = MP_WELLS;
        verb_ndx[1] = MP_WELLDPTH;
        verb_ndx[2] = MP_WELLPACH;
        break;
        
    case SEISMIC_DATA_TYPE:
        data_class = WM_CLASS_SEISMIC;
        nclass = 6;
        verb_ndx[0] = MP_SEISMIC;
        verb_ndx[1] = MP_SEISDPTH;
        verb_ndx[2] = MP_SEISCHRN;
        verb_ndx[3] = MP_SEISPACH;
        verb_ndx[4] = MP_MISTIE;
        verb_ndx[5] = MP_SEISDATA;
        break;
        
    default:
        return FAIL;
        }
    /* * CONVERT VERB_CODE NUMBER IN verb_ndx ARRAY TO TRUE INDEX NUMBER * */
    /* ------------------------------------------------------------------- */
    
    for (j = 0; j < nclass; j++)
        {
        for (i = 0; i < MD_MAX_OVERLAY_TYPES; i++)
            {
            if (overlay_verbs[i].verb_code EQUALS verb_ndx[j])
                {
                verb_ndx[j] = i;
                break;
                }
            }
        }
    /* * CHECK EXISTENCE OF MAP * */
    /* -------------------------- */
    
    if ((status = mp_get_current_map (&map)) != SUCCESS)
        {
        return status;
        }
    display = map->display;
    compiled = map->compiled;
    
    /* * SAVE WM STATE AND CLOSE OUT TO APPLICATION * */
    /* ---------------------------------------------- */
    
    wmowhere (&appSave, &winSave, &tnrSave, &groSave, &status);
    status = wmoexists (appSave, pslist->window_id, 0, 0, &ifExists);
    if (!ifExists)
        {
        return status;
        }
    /* * ATTACH TO TOP LEVEL * */
    /* ----------------------- */
    
    if (wmoattach (appSave, 0, 0, 0, &status) != SUCCESS)
        {
        return status;
        }
    wmwopen (pslist->window_id);
    
    /* * LOOP THRU THE DISPLAY STRUCTURE AND FIND LOWEST SEGMENT ID * */
    /* -------------------------------------------------------------- */
    
    old_verb_ndx = -1;
    for (i = 0; i < display->last_row_used; i++)
        {
        
        /* PROCESS ONLY ON FIRST LINE OF OVERLAY DEFINITION */
        
        if (display->verb_ndx[i] != old_verb_ndx)
            {
            old_verb_ndx = display->verb_ndx[i];
            
            /* DONT PROCESS OVERLAY SEPARATOR LINE */
            
            if (old_verb_ndx != -1)
                {
                
                /* CHECK OVERLAY TYPE AGAINST verb_ndx ARRAY */
                
                for (j = 0; j < nclass; j++)
                    {
                    if (display->verb_ndx[i] EQUALS verb_ndx[j])
                        {
                        row = display->compiled_row[i];
                        
                        /* OPEN THIS GROUP AND GET IT'S SEGMENT LIST */
                        
                        wmgopen (compiled->overlay_wm_id[row]);
                        if (compiled->verb_wm_id[row] != 0)
                            {
                            wmgopen (compiled->verb_wm_id[row]);
                            }
                        llinit (&segmentList, 0, sizeof(INT), 0);
                        wmsinqlist (data_class, &segmentList);
                        
                        if ((numSegments = llcount (&segmentList)) < 0)
                            {
                            return FAIL;
                            }
                        /* LOOP THRU AND EXAMINE SEGMENT ID'S */
                        
                        if (numSegments > 0)
                            {
                            totalSegments += numSegments;
                            status = llfirst (&segmentList, &segment);
                            
                            for (ii = 0; ii < numSegments;
                                                                          ii++,
                                                                          status =
                                                                          llnext (
                                                                          &segmentList,
                                                                                  &segment
                                                                                  ))
                                {
                                if (status < 0)
                                    {
                                    return FAIL;
                                    }
                                /* SAVE LOWEST SEGMENT ID */
                                
                                if (LowestSegment)
                                    {
                                    LowestSegment = MIN (LowestSegment, segment);
                                    }
                                else
                                    {
                                    LowestSegment = segment;
                                    }
                                }
                            }
                        /* FREE LOCAL SEGMENT TREE, CLOSE GROUP */
                        
                        llfree (&segmentList);
                        if (compiled->verb_wm_id[row] != 0)
                            {
                            wmgclose (compiled->verb_wm_id[row]);
                            }
                        wmgclose (compiled->overlay_wm_id[row]);
                        break;
                        }
                    }
                }
            }
        }
    wmwclose (pslist->window_id);
    if (totalSegments <= 0)
        {
        return FAIL;
        }
    pslist->segment_id = LowestSegment;
    return SUCCESS;
    }
/* END:     */
