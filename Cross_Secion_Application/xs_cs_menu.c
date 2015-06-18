/* DEC/CMS REPLACEMENT HISTORY, Element XS_CS_MENU.C*/
/* *3    17-AUG-1990 22:38:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-JUN-1989 11:06:05 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:43:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_CS_MENU.C*/
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

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_CONF_WS_H

#include "esi_conf_ws.h"

#endif

#ifndef XS_MENUS_RH

#include "xs_menus.rh"

#endif

/* Function Description -----------------------------------------------------
Description:
    To manage the availablity of the menu items in the (layout)   
    Cross Section menu.  These items are activated based on the   
    current state of the data structure.     DJC 9/1/1987         

Prototype:
    extern INT xs_cs_menu_mgr(XS_STRUCT *pxs);

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
extern INT xs_cs_menu_mgr (XS_STRUCT *pxs)
#else
extern INT xs_cs_menu_mgr (pxs)
XS_STRUCT *pxs;
#endif
    {
    CONFIGURATION_WORKSPACE *pconfig;
    INT status;
    PROGNAME ("xs_cs_menu_mgr");
    
    am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *) & pconfig);
    
    if (pconfig->maconly)           /*  No graphics screen available!  */
        {
        wienmni (pxs->layout_menu_id, DRAW_MENU_ITEM, OFF);
        wienmni (pxs->layout_menu_id, REDRAW_MENU_ITEM, OFF);
        if (pxs->valid_well_list)   /*  We have a well list!  */
            {
            wienmni (pxs->layout_menu_id, PLOT_MENU_ITEM, ON);
            }
        else                        /*  We have no well list! */
            {
            wienmni (pxs->layout_menu_id, PLOT_MENU_ITEM, OFF);
            }
        }
    else                            /*  A graphics screen is available */
        {
        if (pxs->valid_well_list)   /*  We have a well list!  */
            {
            wienmni (pxs->layout_menu_id, DRAW_MENU_ITEM, ON);
            wienmni (pxs->layout_menu_id, PLOT_MENU_ITEM, ON);
            if (pxs->changed_since_display) /* The data have changed! */
                {
                wienmni (pxs->layout_menu_id, REDRAW_MENU_ITEM, OFF);
                }
            else                    /* No changes in the data */
                {
                wienmni (pxs->layout_menu_id, REDRAW_MENU_ITEM, ON);
                }
            }
        else                        /* No valid well list!    */
            {
            wienmni (pxs->layout_menu_id, DRAW_MENU_ITEM, OFF);
            wienmni (pxs->layout_menu_id, REDRAW_MENU_ITEM, OFF);
            wienmni (pxs->layout_menu_id, PLOT_MENU_ITEM, OFF);
            }
        }
    return SUCCESS;
    }
/* END:     */
