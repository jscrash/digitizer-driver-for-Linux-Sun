/* DEC/CMS REPLACEMENT HISTORY, Element WM_SET_SCR.C*/
/* *5     3-MAY-1991 16:15:26 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *4    23-AUG-1990 16:16:55 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *3    14-AUG-1990 13:44:36 VINCE "(SPR 5644) Header Standardization"*/
/* *2     2-NOV-1989 15:25:21 JULIAN "(SPR 30) move wm_inq_scr, make wm_current_def_sreen static"*/
/* *1    19-JUN-1989 13:41:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_SET_SCR.C*/
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

#include "esi_wm.h"

/* *************************  GLOBAL VARIABLES ***************************** */

static INT wm_current_default_screen = 0;

/* File Description ---------------------------------------------------------
Overview:
    Screen utilities.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT wm_set_current_screen(INT default_screen);
    
    publicdef INT wm_inq_current_screen(INT *default_screen);
Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    wm_set_current_screen is called to set the current default
    workstation screen (e.g., left or right screen).
            
    The companion function wm_inq_current_screen(&default_screen)
    is used to inquiry the current setting, so that wmwallocate
    can initially place the window on the desired screen.
 
    INITIAL CONDITIONS:
    
            1.  finder_config_workstation must have been invoked to establish
                the global workspace CONFIGURATION, containing the number of
                graphics screens available on the current workstation.
    
Prototype:
    publicdef INT wm_set_current_screen(INT default_screen);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) default_screen  -(INT) [1...n], where by convention:
                            screen 1 is taken to be the "left" or only screen, and
                            screen 2 is taken to be the "right" screen.
            
Return Value/Status:
    SUCCESS - If request was honored.
    FAIL - If default_screen is out of range, or 
                CONFIGURATION workspace not defined, or
                the number of graphics_heads is less than one.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wm_set_current_screen (INT default_screen) 
#else
publicdef INT wm_set_current_screen (default_screen)
INT default_screen;
#endif
    {
    INT status = SUCCESS;
    INT graphics_terminal_type, graphics_heads, graphics_planes;
#ifndef SUN_GKS_OR_PRIOR_GKS
    /* Vaxstations don't support this */
    if (ug_if_gpx () == FALSE)
        {
        status = gkz_inq_workstation (&graphics_terminal_type, &graphics_heads,
                                      &graphics_planes);
        
        if (status == SUCCESS)
            {
            if (default_screen < 1 || graphics_heads < 1)
                {
                status = FAIL;
                }
            else if (default_screen <= graphics_heads)
                {
                wm_current_default_screen = default_screen;
                }
            else
                {
                wm_current_default_screen = 1;
                }
            }
        }
#endif
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    wm_inq_current_screen is called to inquiry the current default
    workstation screen (e.g., left or right screen).
            
    The companion function wm_set_current_screen(&default_screen)
    is used to set the current default
    workstation screen, so that wmwallocate
    can initially place the window on the desired screen.
 
    INITIAL CONDITIONS:
    
            1.  finder_config_workstation must have been invoked to establish
                the global workspace CONFIGURATION, containing the number of
                graphics screens available on the current workstation.

Prototype:
    publicdef INT wm_inq_current_screen(INT *default_screen);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) default_screen  -(INT *) [1...n], where by convention:
                            screen 1 is taken to be the "left" or only screen, and
                            screen 2 is taken to be the "right" screen.
                            Ifgkz_set_workstation_screen has not been invoked, the
                            default of screen 1 will be returned.
            

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wm_inq_current_screen (INT *default_screen) 
#else
publicdef INT wm_inq_current_screen (default_screen)
INT *default_screen;
#endif
    {
    
    INT status = SUCCESS;
#ifndef SUN_GKS_OR_PRIOR_GKS
    /* Vaxstations don't support this */
    if (ug_if_gpx () == FALSE)
        {
        if (wm_current_default_screen == 0)
            {
            *default_screen = 1;
            }
        else
            {
            *default_screen = wm_current_default_screen;
            }
        }
    else
        {
        *default_screen = 1;
        }
#endif  
    return status;
    }
/* END:     */
