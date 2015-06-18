/* DEC/CMS REPLACEMENT HISTORY, Element XS_PREFS_4.C*/
/* *2    17-AUG-1990 22:42:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_PREFS_4.C*/
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

#if 0

/* this routine contains references to functions which are not likely to
   be completed at this time, but I dont want to delete from cms yet */

#include "esi_am.h"
#include "esi_wi.h"
#include "esi_xs_defs.h"
#include "xs_cdlgs.rh"
#include "xs_menus.rh"

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_shading_cdlg_init(XS_STRUCT *pxs);
    
    publicdef INT xs_shading_cdlg_server(INT id, INT item, XS_STRUCT *pxs);
    
    publicdef INT xs_shading_cdlg_term(XS_STRUCT *pxs);
    
    publicdef INT xs_annotate_cdlg_init(XS_STRUCT *pxs);
    
    publicdef INT xs_annotate_cdlg_server(INT id, INT item, XS_STRUCT *pxs);
    
    publicdef INT xs_annotate_cdlg_term(XS_STRUCT *pxs);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Display dialog to accept track shading defs.

Prototype:
    publicdef INT xs_shading_cdlg_init(XS_STRUCT *pxs);

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
publicdef INT xs_shading_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_shading_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    PROGNAME ("xs_shading_cdlg_init");
#if 0
    status = am_define_resource........................wienmni (pxs->edit_menu_id,
                                                                CHANGE_ALL_MENU_ITEM,
                                                                OFF);
    wienmni (pxs->edit_menu_id, SHADING_MENU_ITEM, OFF);
    pxs->shading_in_use = ON;
    pxs->shading_cdlg_id = id;
#endif
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to accept track shading defs from user.

Prototype:
    publicdef INT xs_shading_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Resource event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_shading_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
publicdef INT xs_shading_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;

#if 0
    switch (item)
        {
    case XS_SHADING_ACCEPT:
    case XS_SHADING_IGNORE:
    default:
        break;
        }
#endif
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Finish up shading stuff.

Prototype:
    publicdef INT xs_shading_cdlg_term(XS_STRUCT *pxs);

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
publicdef INT xs_shading_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_shading_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
#if 0
    pxs->shading_in_use = OFF;
    am_release_resource (pxs->shading_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_annotate_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, SHADING_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
#endif
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Display dialog to accept track annotations.

Prototype:
    publicdef INT xs_annotate_cdlg_init(XS_STRUCT *pxs);

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
publicdef INT xs_annotate_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_annotate_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
#if 0
    status = am_define_resource (.........................wienmni (pxs->edit_menu_id,
                                                                 CHANGE_ALL_MENU_ITEM,
                                                                   OFF);
                                 wienmni (pxs->edit_menu_id, ANNOTATE_MENU_ITEM, OFF);
                                 pxs->annotation_in_use = ON;
                                                                                       pxs
                                                                                       ->
                                                                                       annotation_cdlg_id
                                                                                        
                                                                                       =
                                                                                        
                                                                                       id
                                                                                       ;
                                 
#endif
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to accept track annotation defs from user.

Prototype:
    publicdef INT xs_annotate_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Resource event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_annotate_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
publicdef INT xs_annotate_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
#if 0
    switch (item)
        {
    case XS_ANNOTATE_ACCEPT:
    case XS_ANNOTATE_IGNORE:
    default:
        break;
        }
#endif
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Take down annotation dialog.

Prototype:
    publicdef INT xs_annotate_cdlg_term(XS_STRUCT *pxs);

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
publicdef INT xs_annotate_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_annotate_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
#if 0
    am_release_resource (pxs->annotation_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_stop_chg_all (pxs);
    
    wienmni (pxs->edit_menu_id, ANNOTATE_MENU_ITEM, ON);
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);

#endif
    return SUCCESS;
    
    }
/* END:     */

#endif
