/* DEC/CMS REPLACEMENT HISTORY, Element WM_ZOOM.C*/
/* *4    18-JUL-1990 18:49:39 JESSIE "(SPR 1) change include files"*/
/* *3    22-DEC-1989 06:51:16 GORDON "(SPR 0) delete wm_set_zoom_aspect, use qs variable"*/
/* *2    13-NOV-1989 13:52:49 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *1    19-JUN-1989 13:41:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_ZOOM.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_ly.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#include "esi_wm_int.h"
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif
#ifndef ESI_WM_ERR_H
#include "esi_wm_err.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_LL_H
#include "esi_ll.h"
#endif
#ifndef ESI_BT_H
#include "esi_bt.h"
#endif
#ifndef ESI_PR_H
#include "esi_pr.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#include "esi_wm_msg.h"
#include "esi_qs.h"

#define REFRESH_SCREEN if (ug_if_gpx() == FALSE) wm_grsgwk(MAIN_WKID); \
                else wm_update();
        
#if USE_PROTOTYPES

static INT zzw_zoomin (INT win_id);
static VOID zzw_zoomout (INT win_id);

#else

static INT zzw_zoomin ();
static VOID zzw_zoomout ();

#endif

/* === WMTDOPUCKKEY === */
/*
   This function provides the FINDER standard support for zoom/pan
   operations on a window.

   Following execution of the specified zoom/pan operation, the window
   will be automatically refreshed.  Therefore any primitives not in
   segments will be erased.

   When puck A,B,E,F button interpretation is
   enabled, such functions as wmtlocate(), wmtbandrectangle(), etc.
   invoke this function to service these buttons.

   When button interpretation is disabled, the graphics application
   should use this function to service the A,B,E,F buttons, then perform
   application specific updates to the window (such as redrawing primitives
   which were not stored in display list segments).

   The returned status is

      SUCCESS  Button was interpreted and serviced
      FAIL     Button was not recognized
*/

INT wmtdopuckkey (wx, wy, puckkey)

DOUBLE wx, wy;
INT puckkey;

    {
    PROGNAME ("wmtdopuckkey");

    INT returned_status = SUCCESS, wiStatus, wiTnr, wiButton;
    INT front_window, open_window = FALSE;  /* use to determine window open/close on entry */
    FLOAT nwcX, nwcY;
    INT defmod, regmod;
    INT deferral_changed = FALSE;
    FLOAT factor = 2.0;

    wm_inq_deferral (&defmod, &regmod);
    wm_set_deferral (GASAP, GSUPPD);
    wm_inq_front_window (&front_window);
    switch (puckkey)
        {
    case WM_BUTTON_ZOOMIN:  /* A - ZoomIn */
        wmwzoom (front_window, TRUE);
        REFRESH_SCREEN;
        break;

    case WM_BUTTON_PAN:     /* B - Pan */
        zzt_wcs2nwc (wx, wy, &nwcX, &nwcY);
        wmwpanzoom (front_window, nwcX, nwcY, 1.0, 1.0);
        REFRESH_SCREEN;
        break;

    case WM_BUTTON_RESET:   /* F - Zoom out */
        wmwzoom (front_window, FALSE);
        REFRESH_SCREEN;
        break;

    default:
        returned_status = FAIL;
        }
        
    wm_attach_window_screen (front_window); /* ensure that correct screen
                                            is selected, as GRSGWK may
                                            have executed diff screen seg
                                            in the last window redrawn */
    wm_set_deferral (defmod, regmod);

    return returned_status;

    }



/* === WMWZOOM === */
/*

   Banding zoom with optional constant aspect ratio (as defined by the
   function gb_toggle_aspect_lock activated from the global menu.

   Band a rectangle to define
   the desired zoom; then zoom the current window (or the specified window
   if there is no current window.

   This function may be used with or without an opened window.
*/

publicdef INT wmwzoom (win_id, if_zoomin)
INT win_id, if_zoomin;

    {
    PROGNAME ("wmwzoom_aspect");
    CHAR prompt[STMAX];
    INT class, max_sel, num_sel, win_ids[1], status;

    if (if_zoomin)
        {
        strcpy (prompt, mg_message (WM_SELECT_ZOOMIN_WINDOW));
        }
    else
        {
        strcpy (prompt, mg_message (WM_SELECT_ZOOMOUT_WINDOW));
        }

    if (!(win_id))
        {
        class = WM_CLASS_WINDOW;
        max_sel = 1;
        num_sel = 0;
        win_ids[0] = 0;
        status = 0;
        wmwselector (prompt, class, max_sel, &num_sel, win_ids, &status);
        if ((status == SUCCESS) && (num_sel == 1))
            {
            win_id = win_ids[0];
            }
        }
    if (win_id)
        {
        if (if_zoomin)
            {
            zzw_zoomin (win_id);
            }
        else
            {
            zzw_zoomout (win_id);
            }
        }

    return SUCCESS;
    }

/* === zzw_zoomin === */

static INT zzw_zoomin (win_id)
INT win_id;

    {
    PROGNAME ("zzw_zoomin_aspect");

    INT status = SUCCESS, button;
    INT curr_app, curr_win, curr_tnr, curr_gro, curr_seg, curr_pic;
    INT temp_app, temp_win, temp_tnr, temp_gro, temp_seg, temp_pic;
    INT tnr;
    INT saveInterp;
    BOOL constant_aspect=FALSE;
    DOUBLE fx, fy;
    DOUBLE xo, yo;
    DOUBLE tx, ty;          /* width, height of window transform */
    DOUBLE taspect, faspect;    /* aspect ratio of window, req'd zoom */
    FLOAT wx1, wx2, wy1, wy2;
    FLOAT vpx1, vpx2, vpy1, vpy2;
    FLOAT wdx1, wdx2, wdy1, wdy2;
    FLOAT nwcx, nwcy;
    WM_CURRENT *ptr_wmv_current = 0;

    qs_inq_i( QS_ASPECT_LOCK, &constant_aspect );

    /*----------------------------------------------------------*/
    /* Open the input window if there is no current open window */
    /*----------------------------------------------------------*/
    wmoinqcur (&curr_app, &curr_win, &curr_tnr, &curr_gro, &curr_seg,
               &curr_pic);
    if (!(curr_win))
        {
        wmwopen (win_id);
        }

    /*-------------------------------------------------*/
    /* Retrieve pointer to window manager static area  */
    /* and determine if current transformation is a    */
    /* tablet/registered transformation for the GPX    */
    /* style tablet support.                           */
    /*                                                 */
    /* If it is, and the window is ZOOMed, then we     */
    /* have to UNZOOM the window in order to get valid */
    /* zoom factors (because the tablet driver forces  */
    /* us to accept the zoom                           */
    /* rectangle coordinates in WCS instead of NWC.    */
    /*                                                 */
    /* If we are using mouse input or Raster Tek       */
    /* we only have to select the window normalization */
    /* transform.                                      */
    /*-------------------------------------------------*/

    ptr_wmv_current = wm_static ();
    if (ptr_wmv_current->reg.key_type == WM_PD_TABLET)
        {
        if (ptr_wmv_current->zoomx > 1.0 || ptr_wmv_current->zoomy > 1.0)
            {
            zzw_zoomout (win_id);
            }
        }
    else
        {
        wmtselect (0);      /* select 1st transform for this window */
        }

    /*-------------------------------------------*/
    /* Inquiry id of the selected transformation */
    /*-------------------------------------------*/
    wmoinqcur (&temp_app, &temp_win, &tnr, &temp_gro, &temp_seg, &temp_pic);

    /*-------------------------------------------------------------------*/
    /* Disable puck button trapping while using standard wmt... function */
    /* to support zoom.  Then restore button interpretation established  */
    /* by caller                                                         */
    /*-------------------------------------------------------------------*/

    wmtinqbuttoninterp (&saveInterp);
    wmtbuttoninterp (FALSE);

    /*------------------------------*/
    /* Put up banding prompt dialog */
    /*------------------------------*/
    am_message (AM_DIALOG, "Band a rectangle around the ZoomIn area");

    wmtlocate (&status, &wx1, &wy1, &button);   /* CHECK FOR CANCEL HERE??? */
    wmtbandrectangle (&status, wx1, wy1, &wx2, &wy2, &button);

    am_message (AM_DIALOG, "");
    wmtbuttoninterp (saveInterp);

    /*-------------------------------------------------*/
    /* Guard against degenerate rectangle spec by user */
    /*-------------------------------------------------*/

    if ((ABS (wx2 - wx1) < 0.001)OR (ABS (wy2 - wy1) < 0.001))
        {
        status = WM_ERR_WI_CANCEL;
        }
    if (status == SUCCESS)
        {

        /*---------------------------------------------------------------*/
        /* Get definition of current transform to determine aspect ratio */
        /*---------------------------------------------------------------*/
        wmtinq (tnr, &vpx1, &vpx2, &vpy1, &vpy2, &wdx1, &wdx2, &wdy1, &wdy2);

        tx = ABS (wdx2 - wdx1);
        ty = ABS (wdy2 - wdy1);

        xo = (wx1 + wx2) / 2.0;
        yo = (wy1 + wy2) / 2.0;
        fx = tx / ABS (wx2 - wx1);
        fy = ty / ABS (wy2 - wy1);

        /*-----------------------------*/
        /* Check constant aspect state */
        /*-----------------------------*/
        if (constant_aspect)
            {
            faspect = fy / fx;
            taspect = ty / tx;
            if (faspect < taspect)
                fx = fy * tx / ty;  /* stretch xzoom to match aspect */
            if (faspect > taspect)
                fy = fx * ty / tx;  /* stretch yzoom to match aspect */
            }

        /*-------------------------------------------------------------*/
        /* Transform the desired world coord window center to NWC      */
        /* (Guard against transforms so big that Raster gets overflow) */
        /*-------------------------------------------------------------*/
        if (fx < 1000.0 && fy < 1000.0)
            {
            zzt_wcs2nwc (xo, yo, &nwcx, &nwcy);
            wmwpanzoom (win_id, nwcx, nwcy, fx, fy);
            }
        }

    wmtselect (curr_tnr);
    if (!(curr_win))
        {
        wmwclose (win_id);
        }

    return SUCCESS;
    }

/* === WMWPANZOOM === */

INT wmwpanzoom (win_id, wxo, wyo, wfx, wfy)

INT win_id;
DOUBLE wfx, wfy, wxo, wyo;

    {
    PROGNAME ("wmwpanzoom");

    INT returned_status = SUCCESS;
    INT curr_app, curr_win, curr_tnr, curr_gro, curr_seg, curr_pic;
    INT num_tnrs, i, tnr_id;
    FLOAT llx_nwc, lly_nwc, urx_nwc, ury_nwc;   /* nwc */
    FLOAT llx_wcs, lly_wcs, urx_wcs, ury_wcs;   /* wcs */
    FLOAT wx_min, wx_max, wy_min, wy_max;
    FLOAT vx_min, vx_max, vy_min, vy_max;
    DOUBLE fx, fy, xo, yo;
    FLOAT wcx, wcy, wdx, wdy;
    FLOAT wx1, wx2, wy1, wy2;
    LISTHEAD tnr_list;
    RECTANGLE wcsRect, nwcRect;
    PIPE nwc2wcs;

    wmoinqcur (&curr_app, &curr_win, &curr_tnr, &curr_gro, &curr_seg,
               &curr_pic);
    if (!(curr_win))
        {
        wmwopen (win_id);
        }

    llx_nwc = wxo - (0.5 / wfx);
    lly_nwc = wyo - (0.5 / wfy);
    urx_nwc = wxo + (0.5 / wfx);
    ury_nwc = wyo + (0.5 / wfy);

    llinit (&tnr_list, 0, sizeof(INT), 0);
    wmtinqlist (WM_CLASS_TNR, &tnr_list);
    num_tnrs = llcount (&tnr_list);
    if (num_tnrs > 1)
        {
        llfirst (&tnr_list, &tnr_id);   /* 1st is priv to wm */
        for (i = 1; i < num_tnrs; i++)
            {
            llnext (&tnr_list, &tnr_id);
            wmtinq (tnr_id, &vx_min, &vx_max, &vy_min, &vy_max, &wx_min,
                    &wx_max, &wy_min, &wy_max);
            wmtinqpanzoom (tnr_id, &xo, &yo, &fx, &fy);
            /* tnr wcs center before zoom and pan */
            wcx = wx_min + ((wx_max - wx_min) / 2.0);
            wcy = wy_min + ((wy_max - wy_min) / 2.0);
            /* tnr wcs corners after zoom and pan */
            wx1 = xo + ((wx_min - wcx) / fx);
            wx2 = xo + ((wx_max - wcx) / fx);
            wy1 = yo + ((wy_min - wcy) / fy);
            wy2 = yo + ((wy_max - wcy) / fy);
            /* pipe corners of tnr to corners of window */
            PR_SetRect (&wcsRect, wx1, wy1, wx2, wy2);
            PR_SetRect (&nwcRect, vx_min, vy_min, vx_max, vy_max);
            PR_SetPipe (&nwc2wcs, &nwcRect, &wcsRect);

            PR_SetRect (&nwcRect, 0.0, 0.0, 1.0, 1.0);
            PR_PipeRect (&nwcRect, &nwc2wcs, &wcsRect);
            PR_InqRect (&wcsRect, &wx1, &wy1, &wx2, &wy2);

            /* window sides in wcs after zoom pan */
            wdx = wx2 - wx1;
            wdy = wy2 - wy1;
            /* new window corners in wcs */
            llx_wcs = wx1 + (llx_nwc * wdx);
            lly_wcs = wy1 + (lly_nwc * wdy);
            urx_wcs = wx1 + (urx_nwc * wdx);
            ury_wcs = wy1 + (ury_nwc * wdy);
            /* pipe window corners to tnr corners */
            PR_SetRect (&wcsRect, llx_wcs, lly_wcs, urx_wcs, ury_wcs);
            PR_SetRect (&nwcRect, 0.0, 0.0, 1.0, 1.0);
            PR_SetPipe (&nwc2wcs, &nwcRect, &wcsRect);

            PR_SetRect (&nwcRect, vx_min, vy_min, vx_max, vy_max);
            PR_PipeRect (&nwcRect, &nwc2wcs, &wcsRect);
            PR_InqRect (&wcsRect, &llx_wcs, &lly_wcs, &urx_wcs, &ury_wcs);

            /* new zoom and pan */
            fx = (wx_max - wx_min) / (urx_wcs - llx_wcs);
            fy = (wy_max - wy_min) / (ury_wcs - lly_wcs);
            xo = llx_wcs + ((urx_wcs - llx_wcs) / 2.0);
            yo = lly_wcs + ((ury_wcs - lly_wcs) / 2.0);
            wmtpanzoom (tnr_id, xo, yo, fx, fy);
            }               /* for */
        }

    wmtselect (curr_tnr);
    if (!(curr_win))
        {
        wmwclose (win_id);
        }
    return returned_status;
    }

/* === ZZW_ZOOMOUT === */

static VOID zzw_zoomout (win_id)
INT win_id;

    {
    PROGNAME ("zzw_zoomout");

    INT curr_app, curr_win, curr_tnr, curr_gro, curr_seg, curr_pic;
    INT num_tnrs, i, tnr_id;
    FLOAT wx_min, wx_max, wy_min, wy_max;
    FLOAT vx_min, vx_max, vy_min, vy_max;
    DOUBLE fx, fy, xo, yo;
    LISTHEAD tnr_list;

    wmoinqcur (&curr_app, &curr_win, &curr_tnr, &curr_gro, &curr_seg,
               &curr_pic);
    if (!(curr_win))
        {
        wmwopen (win_id);
        }

    llinit (&tnr_list, 0, sizeof(INT), 0);
    wmtinqlist (WM_CLASS_TNR, &tnr_list);
    num_tnrs = llcount (&tnr_list);
    if (num_tnrs > 1)
        {
        llfirst (&tnr_list, &tnr_id);   /* 1st is priv to wm */
        for (i = 1; i < num_tnrs; i++)
            {
            llnext (&tnr_list, &tnr_id);
            wmtinq (tnr_id, &vx_min, &vx_max, &vy_min, &vy_max, &wx_min,
                    &wx_max, &wy_min, &wy_max);
            xo = wx_min + ((wx_max - wx_min) / 2.0);
            yo = wy_min + ((wy_max - wy_min) / 2.0);
            fx = 1;
            fy = 1;
            wmtpanzoom (tnr_id, xo, yo, fx, fy);
            }
        }

    wmtselect (curr_tnr);
    if (!(curr_win))
        {
        wmwclose (win_id);
        }
    }

