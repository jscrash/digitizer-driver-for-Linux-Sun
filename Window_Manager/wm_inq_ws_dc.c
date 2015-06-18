/* DEC/CMS REPLACEMENT HISTORY, Element WM_INQ_WS_DC.C */
/* *10    3-MAY-1991 16:15:21 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *9    23-OCT-1990 13:59:57 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *8    26-AUG-1990 14:08:24 GILLESPIE "(SPR 1) Add esi_gks.h in USE_X section" */
/* *7    14-AUG-1990 13:44:14 VINCE "(SPR 5644) Header Standardization" */
/* *6     3-MAY-1990 21:46:26 KEE "(SPR -1) multi window support." */
/* *5     3-MAY-1990 21:40:25 KEE "(SPR -1) multi window stuff." */
/* *4    18-APR-1990 09:35:52 MING "(SPR -1) add comment" */
/* *3    18-APR-1990 09:21:09 MING "(SPR 0) include esi_gl_defs.h for ifndef USE_X" */
/* *2    17-APR-1990 20:40:16 KEE "(SPR -1) Inquire workstation device coordindate" */
/* *1    17-APR-1990 20:16:02 KEE "Return workstation device coordinate" */
/* DEC/CMS REPLACEMENT HISTORY, Element WM_INQ_WS_DC.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */
#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif

#ifdef USE_X

#ifndef ESI_X_H
#include "esi_x.h"
#endif

#include "ansixt.h"

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_inq_ws_dc(INT *xmin, INT *xmax, INT *ymin, INT *ymax);       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    xmin            -(INT *)                                                      */
/*    xmax            -(INT *)                                                      */
/*    ymin            -(INT *)                                                      */
/*    ymax            -(INT *)                                                      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_inq_ws_dc (INT *xmin, INT *xmax, INT *ymin, INT *ymax)
#else
publicdef INT wm_inq_ws_dc (xmin, xmax, ymin, ymax)
INT *xmin, *xmax, *ymin, *ymax;
#endif
    {
#ifdef USE_X
    Arg arglist[4];
    INT i, x, y, width, height;
    WM_CURRENT *wmv_current = wm_static ();
    
    for (i = 0; i < wmv_current->max_ws; i++)
        {
        if (wmv_current->gks_widget_array[i].ws_id == MAIN_WKID)
            break;
        }
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XtSetArg (arglist[0], XmNx, &x);
    XtSetArg (arglist[1], XmNy, &y);
    XtSetArg (arglist[2], XmNwidth, &width);
    XtSetArg (arglist[3], XmNheight, &height);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    XtSetArg (arglist[0], DwtNx, &x);
    XtSetArg (arglist[1], DwtNy, &y);
    XtSetArg (arglist[2], DwtNwidth, &width);
    XtSetArg (arglist[3], DwtNheight, &height);
#endif  /* ---------- END DECW BLOCK ---------- */  

#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
    XtGetValues (wmv_current->gks_widget_array[i].widget, arglist, 4);
#else /* ========== END PRIOR GKS = BEGIN DEC GKS ========== */
    XtGetValues (wmv_current->gks_widget_array[i].out_widget, arglist, 4);
#endif  /* ========== END DEC GKS ========== */

    *xmin = x;
    *ymin = y;
    *xmax = x + width;
    *ymax = y + height;
#endif
    return(SUCCESS);
    }
/* END:                                                                             */












