/* DEC/CMS REPLACEMENT HISTORY, Element XS_DISP_SEC.C*/
/* *3    17-AUG-1990 22:38:28 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     9-JUL-1990 09:43:22 MING "(SPR -1) produce CGM"*/
/* *1    19-JUN-1989 13:43:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_DISP_SEC.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_XS_MSG_H

#include "esi_xs_msg.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef XS_MENUS_RH

#include "xs_menus.rh"

#endif

#ifndef XS_CDLGS_RH

#include "xs_cdlgs.rh"

#endif

/* External entry points in this module.  */

#if 0
extern INT xs_display_cross_section ();
extern INT xs_redraw_cross_section ();

/* Functions external to this module.     */

extern INT xs_free_struct ();
extern INT xs_create_drawing ();
extern INT xs_chg_drawing ();
extern INT xs_gr_draw_section ();
extern INT xs_put_layout ();
#endif

#ifndef ESI_XS_GR_H

#include "esi_xs_gr.h"

#endif
extern INT xs_redraw_cross_section ();

/* File Description ---------------------------------------------------------
Overview:
    Drawing structrue routines.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    extern INT xs_display_cross_section(XS_STRUCT *pxs);

    extern INT xs_redraw_cross_section(XS_STRUCT *pxs);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Calls the routines to:                                                 
        1)  Free the drawing structure (logplot linked list)                  
        2)  Create a new drawing structure (logplot linked list)              
        3)  Call xs_redraw_cross_section to edit structure & draw it.         

Prototype:
    extern INT xs_display_cross_section(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xs_display_cross_section (XS_STRUCT *pxs)
#else
extern INT xs_display_cross_section (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    CHAR *message;
    
    PROGNAME ("xs_display_cross_section");
    
    /* Disable menus requiring a drawing structure.  */
    
    status = wienmni (pxs->tops_menu_id, DIG_TOPS_MENU_ITEM, 0);
    status = wienmni (pxs->tops_menu_id, SAVE_TOPS_MENU_ITEM, 0);
    
    status = xs_free_struct (&(pxs->drawing.log_plots));
    
    message = mg_message (XS_RETV_DATA);
    status = am_message (AM_STATUS, message);
    status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
    status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE3, "");
    status = xs_create_drawing (pxs);
    
    switch (status)
        {
    case SUCCESS:
        status = xs_redraw_cross_section (pxs);
        break;
        
    case XS_INVALID_POINTER:
        message = mg_message (XS_BUILD_DRAWING_FAILED);
        status = am_message (AM_ALERT, message);
        pxs->changed_since_display = TRUE;
        break;
        
    default:
        message = mg_message (XS_CHECK_DATA_PRIV);
        status = am_message (AM_ALERT, message);
        pxs->changed_since_display = TRUE;
        }
    message = mg_message (XS_PICK_MENU_ITEM);
    status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
    status = xs_cs_menu_mgr (pxs);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Calls the routines to:                                                 
        1)  Change drawing structure (determine x-y locations etc)            
        2)  Draw the cross section.                                           
        3)  Sets changed_since_display flag in the system structure.          
        4)  Enable the redraw menu item.                                      
        5)  Save the layout as LAST_DISPLAYED_LAYOUT  ( disabled )            

Prototype:
    extern INT xs_redraw_cross_section(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xs_redraw_cross_section (XS_STRUCT *pxs)
#else
extern INT xs_redraw_cross_section (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    CHAR *message;
    CHAR msg[80];
    
    PROGNAME ("xs_redraw_cross_section");
    
    message = mg_message (XS_RESOLVE_DRAWING);
    status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
    
    /* Copy attributes from master layout to well layouts. */
    
    status = xs_copy_well_info (pxs);
    
    /* Make all necessary changes to drawing structure.    */
    
    if (status == SUCCESS)
        {
        status = xs_chg_drawing (pxs);
        }
    /* Draw the cross section!                             */
    
    if (status == SUCCESS)
        {
        message = mg_message (XS_DRAWING_SECTION);
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
        
        if (pxs->depth_scale_user_units == XS_CENTIMETER_UNITS)
            {
            message = mg_message (XS_DRAWING_SIZE_CM);
            sprintf (msg, message, pxs->drawing.width * 2.54,
                     pxs->drawing.height * 2.54);
            }
        else
            {
            message = mg_message (XS_DRAWING_SIZE_INCH);
            sprintf (msg, message, pxs->drawing.width, pxs->drawing.height);
            }
        am_message (AM_STATUS, msg);
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE3, msg);
        
        /* add second argument for non-metafile call */
        status = xs_gr_draw_section (pxs, (CHAR *)0);
        if (status == SUCCESS)
            {
            pxs->changed_since_display = FALSE;
            status = wienmni (pxs->tops_menu_id, RETV_TOPS_MENU_ITEM, 1);
            status = wienmni (pxs->tops_menu_id, DISPLAY_TOPS_MENU_ITEM, 1);
            status = wienmni (pxs->tops_menu_id, DIG_TOPS_MENU_ITEM, 1);
            status = wienmni (pxs->tops_menu_id, SAVE_TOPS_MENU_ITEM, 1);
            
            /*                                                           Disabled auto save.
                     status = xs_delete_layout( LAST_DISPLAYED_LAYOUT );
                     status = xs_put_layout( pxs, LAST_DISPLAYED_LAYOUT);
                     if (status != SUCCESS) 
                       {
                       message = mg_message(XS_LAST_DISPLAYED_NOT_SAVED);
                       er_log( message );
                       status = am_message(AM_ALERT, message);
                       }
            */
            }
        }
    message = mg_message (XS_PICK_MENU_ITEM);
    status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
    status = xs_cs_menu_mgr (pxs);
    
    return SUCCESS;
    }
/* END:     */
