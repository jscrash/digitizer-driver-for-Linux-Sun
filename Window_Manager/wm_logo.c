/* DEC/CMS REPLACEMENT HISTORY, Element WM_LOGO.C*/
/* *8    14-AUG-1990 13:44:24 VINCE "(SPR 5644) Header Standardization"*/
/* *7     3-MAY-1990 21:46:48 KEE "(SPR -1) multi window support."*/
/* *6     3-MAY-1990 21:40:30 KEE "(SPR -1) multi window stuff."*/
/* *5    13-OCT-1989 09:53:07 GORDON "(SPR 0) Update address, phone number"*/
/* *4    14-SEP-1989 16:22:04 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *3    13-AUG-1989 14:02:10 JULIAN "(SPR -1) changed logo from esi to FGS."*/
/* *2    20-JUL-1989 11:07:48 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:41:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_LOGO.C*/
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

#include "esi_gks.h"
#include "esi_wm.h"
#include "esi_lu.h"
#include "esi_wm_cls.h"

/* Function Description -----------------------------------------------------
Description:
    Finder Logo generator.

Prototype:
    publicdef INT wm_logo();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of window.
    
Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wm_logo () 
#else
publicdef INT wm_logo ()
#endif
    {
    static char finder[] = 
        {
        "FINDER"
        };
    static char copyright[] = 
        {
        "Copyright (c) 1986"
        };
    static char fgs[] = 
        {
        "Finder Graphics Systems, Inc."
        };
    static char town[] = 
        {
        "Corte Madera, CA  94925"
        };
    static char phone[] = 
        {
        "(415) 927-0100"
        };
    
    INT window, tnr, segment, ws_type, ws_heads, ws_planes, screen;
    INT color;
    
    /* Test for the type of graphics device in use. */
    if (ug_if_gpx () == FALSE)
        gkz_inq_workstation (&ws_type, &ws_heads, &ws_planes);
    else
        {
        ws_heads = 1;               /* GPX doesn't support multiple screens. JSC 2/1/89 */
        return 0;                   /* Don't draw logo with multi GKS windows */
        }
    for (screen = 1; screen <= ws_heads; screen++)
        {
        wm_set_current_screen (screen);
        lu_text_to_index ("COLOR", &color, "BACKGROUND");
        wmwallocate (&window, 0.7, 1.0, 0.0, 0.1, color);
        
        wmtallocate (&tnr, 0.0, 1.0, 0.0, 1.0, 0.0, 50.0, 0.0, 20.0);
        
        wmsallocate (&segment);
        
        wm_gschup (0.0, 1.0);
        lu_text_to_index ("COLOR", &color, "NORMAL");
        wm_gstxci (color);
        wm_gstxp (GRIGHT);
        wm_gstxal (GAHNOR, GAVNOR);
        wm_gstxfp (SIMPLEX, GSTRKP);
        wm_gschxp (1.0);
        wm_gschsp (0.0);
        
        wm_gschh (3.0);
        wm_gtx (2.0, 14.0, finder);
        
        wm_gschh (1.0);
        wm_gtx (2.0, 12.0, copyright);
        
        wm_gschh (1.5);
        wm_gtx (2.0, 8.0, fgs);
        wm_gtx (2.0, 5.0, town);
        wm_gtx (2.0, 2.0, phone);
        
        wmsclose (segment);
        wmoset (segment, WM_CLASS_SEGMENT, "", "Logo");
        
        wmwclose (window);
        if (ws_heads == 1)
            wmoset (window, WM_CLASS_WINDOW, "", "Logo");
        else if (screen == 1)
            wmoset (window, WM_CLASS_WINDOW, "", "Logo Left Screen");
        else if (screen == 2)
            wmoset (window, WM_CLASS_WINDOW, "", "Logo Right Screen");
        }
    wm_update ();
    wm_set_current_screen (1);
    return(window);
    }
/* END:     */
