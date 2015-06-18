/* DEC/CMS REPLACEMENT HISTORY, Element XS_DSP_TOPS.C*/
/* *4     3-MAY-1991 16:32:09 KEE "(SPR -1) Bug fix when porting MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *3    17-AUG-1990 22:39:22 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     1-JUL-1989 15:42:24 JULIAN "(SPR 5) fix deferral problem."*/
/* *1    19-JUN-1989 13:44:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_DSP_TOPS.C*/
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

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#include "xs_cdlgs.rh"
#include "xs_menus.rh"
typedef struct internal_use
    {
    CHAR **tops;
    XS_STRUCT *pxs;
    } INTERNAL_STRUCT;

/* File Description ---------------------------------------------------------
Overview:
    This module includes initial and server routines for operating
    tops.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_dsp_tops_init(XS_STRUCT *pxs );

Private_Functions:
    static INT xs_dsp_tops_server(INT id, INT item, INTERNAL_STRUCT *parms );

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    The excutive server routine of xs_dsp_tops.

Prototype:
    static INT xs_dsp_tops_server(INT id, INT item, INTERNAL_STRUCT *parms );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) parms       -(INTERNAL_STRUCT *) A pointer of INTERNAL_STRUCT.
    (I) id          -(INT) The id of active CDLG.
    (I) item        -(INT) The item picked in the active CDLG.
    (O) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_dsp_tops.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_dsp_tops_server (INT id, INT item, INTERNAL_STRUCT *parms)
#else
static INT xs_dsp_tops_server (id, item, parms)
INTERNAL_STRUCT *parms;
INT id;
INT item;
#endif
    {
    INT status = SUCCESS;
    INT *selected_list, total_tops = 0;
    XS_STRUCT *pxs;
    CHAR **tops;
    DOUBLE min_char_height;

    /* ************************************************************************* */
    
    /* analyze the picked item.          */
    tops = parms->tops;
    pxs = parms->pxs;
    switch (item)
        {
    case XS_DSP_BTN_DISPLAY:
        /* allocates the selected_list for   */
        /* retruned items.           */
        total_tops = ts_tcp_len (tops);
        selected_list = (INT *)tc_alloc (sizeof(INT) * (total_tops + 1));
        wiqsel (id, XS_DSP_SELECT_LIST, &total_tops, selected_list);
        
        status = xsz_set_tops_vis (pxs, total_tops, tops, selected_list);
        if (total_tops >= 0)
            {
            wmwopen (pxs->drawing.window_id);
	    status = xs_get_hdr_min_height(pxs, &min_char_height);
            status = xsz_display_tops (pxs, min_char_height);
            wmwclose (pxs->drawing.window_id);
            }
        wm_update ();
        tc_free (selected_list);
        
    case XS_DSP_BTN_CANCEL:
        am_release_resource (pxs->display_top_cdlg_id);
        ts_tcp_free (tops);
    default:
        wienmni (pxs->tops_menu_id, DISPLAY_TOPS_MENU_ITEM, ON);
        break;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine to define the display tops dialog box.

Prototype:
    publicdef INT xs_dsp_tops_init(XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_dsp_tops_init (XS_STRUCT *pxs)
#else
publicdef INT xs_dsp_tops_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    INT id, llstat, n, nn, ntops;
    INT num_items = 0;
    static INTERNAL_STRUCT parms;
    TOP_INFO *ptinfo;
    /* ************************************************************************** */
    
    /* defines the dialog resource.            */
    wienmni (pxs->tops_menu_id, DISPLAY_TOPS_MENU_ITEM, OFF);
    parms.pxs = pxs;
    status = xsz_make_tops_tcp (pxs, &(parms.tops));
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_DISPLAY_TOPS,
                                 xs_dsp_tops_server, &id, &parms);
    pxs->display_top_cdlg_id = id;
    
    /* set the texts to the selecting box.     */
    
    num_items = ts_tcp_len (parms.tops);
    if (num_items EQUALS 0)
        {
        am_message (AM_ALERT, mg_message (XS_MUST_HAVE_TOPS));
        wienmni (pxs->tops_menu_id, DISPLAY_TOPS_MENU_ITEM, ON);
        }
    else
        {
        wisselt (id, XS_DSP_SELECT_LIST, XS_DSP_SELECT_BAR, num_items, num_items,
                 parms.tops);
        
        ntops = llcount (&pxs->drawing.top_table);
        
        for (n = 1; n <= ntops; n++)
            {
            llstat = (n == 1) ? llfirst (&pxs->drawing.top_table,
                                         &ptinfo) : llnext (&pxs->drawing.top_table,
                                                            &ptinfo);
            
            if (ptinfo->visible == TRUE)
                {
                for (nn = 0; nn < num_items; nn++)
                    {
                    if (!(strcmp (parms.tops[nn], ptinfo->top)))
                        wi_set_default_sel (id, XS_DSP_SELECT_LIST, nn + 1);
                    }
                }
            }
        status = wiopcd (id);
        }
    return status;
    }
/* END:     */
