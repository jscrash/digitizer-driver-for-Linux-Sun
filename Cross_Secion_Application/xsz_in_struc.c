/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_IN_STRUC.C*/
/* *4    17-AUG-1990 22:36:06 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    10-MAY-1990 17:30:35 JULIAN "(SPR 5214) fix default value on depth scale interval"*/
/* *2    14-SEP-1989 16:23:36 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:43:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_IN_STRUC.C*/
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
#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifdef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    To initialize the cross section structure to allow the    
    user to fight his/her way through without being forced     
    to retrieve a layout.                                     

    At least one track must be defined in pxs->plo in order   
    for the track definition cdlg to copy track marks when     
    tracks are added.                                          

    At least one trace must be defined in pxs->plo in order    
    for the trace selection cdlg to work.                      

Prototype:
    publicdef INT xsz_init_struct(XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_init_struct (XS_STRUCT *pxs)
#else
publicdef INT xsz_init_struct (pxs)
XS_STRUCT *pxs;
#endif
    {
    TRACK_STRUCT *pts;
    COLOR_INDEX blue;
    INDEX solid, dotted;
    PROJECT_DEFAULT *proj;
    INT status;
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *) & proj);
    if (status != SUCCESS)
        return status;
    
    if (ts_like (proj->unit.horizontal, "*CENTI*", '*') ||
        ts_like (proj->unit.horizontal, "CM", '*') ||
        ts_like (proj->unit.horizontal, "METER*", '*'))
        {
        pxs->depth_interval_scale_units = XS_CENTIMETER_UNITS;
        }
    else
        {
        pxs->depth_interval_scale_units = XS_INCH_UNITS;
        }
    lu_text_to_index ("COLOR", (INDEX *)&blue, "BLUE");
    
    strcpy (pxs->layout_name, "< ----- >");
    pxs->changed_since_display = TRUE;
    pxs->changed_since_save = FALSE;
    pxs->tops_changed_since_save = FALSE;
    pxs->faults_changed_since_save = FALSE;
    pxs->quit_flag = FALSE;
    pxs->review_all_switch = FALSE;
    pxs->xs_type = MEASURED_DEPTH;
    pxs->xs_spacing = XS_CONSTANT_SPACED;
    
    pxs->plo->interval.specify_as = MEASURED_DEPTH;
    pxs->plo->interval.method = SEL_BY_DEPTH;
    pxs->plo->data_thinning = NO_THINNING;
    
    pxs->plo->log_hdr.visible = TRUE;
    pxs->plo->log_hdr.visible_border = TRUE;
    pxs->plo->log_hdr.line.width = 1.0;
    pxs->plo->scale_block.line.color = blue;
    pxs->plo->scale_block.line.width = 1.0;
    
    pxs->plo->text_block.visible = FALSE;
    pxs->plo->text_block.line.color = blue;
    pxs->plo->text_block.line.width = 1.0;
    
    /* One track must be put in pxs->plo so that */
    /* the track definition custom dialog box    */
    /* will have something to copy mark info from*/
    
    pts = (TRACK_STRUCT *)tc_zalloc (sizeof(TRACK_STRUCT));
    pts->number = 9;
    pts->type = DEPTH_TRACK;
    pts->line.color = blue;
    pts->visible = TRUE;
    pts->xorigin = 0.0;
    pts->width = 0.75;
    pts->line.width = 1.0;
    
    lu_text_to_index ("LINE_STYLE", &solid, "SOLID");
    lu_text_to_index ("LINE_STYLE", &dotted, "DOTTED");
    
    pts->divisions.type = 0;
    pts->divisions.number = 0;
    pts->divisions.line.color = blue;
    pts->divisions.line.style = solid;
    pts->divisions.line.width = 1.0;
    
    pts->primary.frequency = 100.0;
    pts->primary.type = 2;          /* lines  */
    pts->primary.line.color = blue;
    pts->primary.line.style = solid;
    pts->primary.line.width = 2.0;   /* medium */
    
    pts->secondary.frequency = 25.0;
    pts->secondary.type = 2;        /* lines  */
    pts->secondary.line.color = blue;
    pts->secondary.line.style = solid;   /* solid  */
    pts->secondary.line.width = 1.0; /* fine   */
    
    pts->tertiary.frequency = 5.0;
    pts->tertiary.type = 2;         /* lines  */
    pts->tertiary.line.color = blue;
    pts->tertiary.line.style = dotted;   /* dotted */
    pts->tertiary.line.width = 1.0;  /* fine   */
    
    llinit (&(pxs->plo->tracks), 0, sizeof(TRACK_STRUCT *), LL_LIST_SYSTEM);
    llappnd (&(pxs->plo->tracks), &pts);
    
    /* The traces linked list must be        */
    /* initialized so that the trace         */
    /* selection cdlg will work.             */
    
    llinit (&(pxs->plo->traces), 0, sizeof(LP_TRACE_STRUCT *), LL_LIST_SYSTEM);
    
    return SUCCESS;
    }
/* END:     */
