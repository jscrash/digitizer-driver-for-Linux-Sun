/* DEC/CMS REPLACEMENT HISTORY, Element XS_TO_PLOTTER.C*/
/* *2    17-AUG-1990 22:45:41 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_TO_PLOTTER.C*/
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
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_xs_msg.h"
#include "esi_mg.h"
#include "xs_menus.rh"
#include "xs_cdlgs.rh"

#if USE_PROTOTYPES
static INT xs_replot_cross_section (XS_STRUCT *pxs);
#else
static INT xs_replot_cross_section ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Plotter routines.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_send_to_plotter (XS_STRUCT *pxs);

Private_Functions:
    static INT xs_replot_cross_section (XS_STRUCT *pxs);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Determine if the structure is already built or needs to be built       
    before dumping to file for plotting.  This is done by checking the     
    changed since displayed item in the data structure.  If the layout     
    has changed since displayed, it is freed and re-built before calling   
    the data dump routine and submitting the batch plot job.               

Prototype:
    publicdef INT xs_send_to_plotter (XS_STRUCT *pxs);

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
publicdef INT xs_send_to_plotter (XS_STRUCT *pxs)
#else
publicdef INT xs_send_to_plotter (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    CHAR *message;
    PROGNAME ("xs_send_to_plotter");
    
    if (pxs->changed_since_display)
        {
        status = xs_free_struct (&(pxs->drawing.log_plots));
        
        message = mg_message (XS_RETV_DATA);
        status = am_message (AM_STATUS, message);
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE3, "");
        status = xs_create_drawing (pxs);
        
        switch (status)
            {
        case SUCCESS:
            status = xs_replot_cross_section (pxs);
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
        }
    else
        {
        status = xs_replot_cross_section (pxs);
        }
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
    static INT xs_replot_cross_section (XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_replot_cross_section (XS_STRUCT *pxs)
#else
static INT xs_replot_cross_section (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    CHAR *message;
    CHAR msg[80];
    
    PROGNAME ("xs_replot_cross_section");
    
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
        status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, "");
        
        status = xs_gr_plot_section (pxs);
        if (status >= SUCCESS)
            {
            pxs->changed_since_display = FALSE;
            }
        }
    message = mg_message (XS_PICK_MENU_ITEM);
    status = wiscdt (pxs->msg_cdlg_id, XS_MESSAGE1, message);
    
    return SUCCESS;
    }
/* END:     */
