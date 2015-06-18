/* DEC/CMS REPLACEMENT HISTORY, Element XS_NEW_LAYOUT.C*/
/* *2    17-AUG-1990 22:41:15 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:46 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_NEW_LAYOUT.C*/
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

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef XS_CDLGS_RH

#include "xs_cdlgs.rh"

#endif

#ifndef XS_MENUS_RH

#include "xs_menus.rh"

#endif

#if USE_PROTOTYPES
static INT xs_newlayout_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
#else
static INT xs_newlayout_cdlg_server ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Allow user to erase current layout with new.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_newlayout_cdlg_init(XS_STRUCT *pxs);

Private_Functions:
    static INT xs_newlayout_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Allow user to erase current layout with new.

Prototype:
    publicdef INT xs_newlayout_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_newlayout_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_newlayout_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_NEWLAYOUT,
                                 xs_newlayout_cdlg_server, &id, pxs);
    
    /*  Deactivate conflicting menu items. */
    
    wienmni (pxs->layout_menu_id, NEW_MENU_ITEM, OFF);
    wienmni (pxs->layout_menu_id, RETRIEVE_MENU_ITEM, OFF);
    wienmni (pxs->layout_menu_id, DELETE_MENU_ITEM, OFF);
    
    status = wiopcd (id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to allow user to load 'new' layout.

Prototype:
    static INT xs_newlayout_cdlg_server(INT id,INT item, XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Event number.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_new_layout.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_newlayout_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_newlayout_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT unused;
    CHAR buffer[81];
    ANALYST_NAME analyst_name;
    PROJECT_NAME project_name;
    
    switch (item)
        {
        
    case XS_NEWLAYOUT_PROCEED:
        /* The NEW_LAYOUT is always owned by DEFAULT_PROJECT. */
        qs_inq_c (QS_PROJECT_NAME, project_name, &unused);
        strcpy (analyst_name, "DEFAULT_PROJECT");
        status = xsz_free_top_table (pxs);
        status = xs_get_layout (pxs, NEW_LAYOUT, project_name, analyst_name);
        if (status == SUCCESS)
            {
            strcpy (pxs->layout_name, NEW_LAYOUT);
            sprintf (buffer, "%s  %s", mg_message (XS_LAYOUT_IS), NEW_LAYOUT);
            wiscdt (pxs->msg_cdlg_id, XS_MESSAGE2, buffer);
            pxs->changed_since_save = FALSE;
            pxs->changed_since_display = TRUE;
            }
        else
            {
            /* (status != SUCCESS) */
            /* warn user new layout not available */
            /* shouldnt happen unless 
          1) new layout not available in oracle
        2) new layout screwed up */
            am_message (AM_ALERT, mg_message (XS_RETV_LAYOUT_FAILED));
            pxs->changed_since_display = TRUE;
            };
        
        status = xs_cs_menu_mgr (pxs);
        
    case XS_NEWLAYOUT_CANCEL:
        
        status = am_release_resource (id);
        break;
        
    default:
        break;
        
        }
    /*  Activate conflicting menu items. */
    
    wienmni (pxs->layout_menu_id, NEW_MENU_ITEM, ON);
    wienmni (pxs->layout_menu_id, RETRIEVE_MENU_ITEM, ON);
    wienmni (pxs->layout_menu_id, DELETE_MENU_ITEM, ON);
    
    return SUCCESS;
    
    }
/* END:     */
