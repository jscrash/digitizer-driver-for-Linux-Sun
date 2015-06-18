/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DR_SCL_BAR.C*/
/*  *9    15-AUG-1991 17:16:19 JTM "(SPR 0) Changed char_param to DOUBLE for function calls."*/
/*  *8    30-OCT-1990 11:53:28 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/*  *7    17-AUG-1990 22:04:32 VINCE "(SPR 5644) Code Cleanup"*/
/*  *6     7-MAR-1990 17:24:42 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/*  *5    18-SEP-1989 12:43:42 JULIAN "Gulf mods under SPR 100"*/
/*  *4    14-SEP-1989 16:18:38 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/*  *3    18-JUL-1989 10:57:58 GORDON "(SPR 100) Merge in UNIX changes"*/
/*  *2    26-JUN-1989 10:54:24 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:08 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DR_SCL_BAR.C*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DR_SCL_BAR.C                            */
/*  *7    17-AUG-1990 22:04:32 VINCE "(SPR 5644) Code Cleanup"                      */
/*  *6     7-MAR-1990 17:24:42 WALTERS "(SPR 0) Remove FLOAT types from system"     */
/*  *5    18-SEP-1989 12:43:42 JULIAN "Gulf mods under SPR 100"                      */
/*  *4    14-SEP-1989 16:18:38 GORDON "(SPR 100) Gulf/Sun/Unix mods"                */
/*  *3    18-JUL-1989 10:57:58 GORDON "(SPR 100) Merge in UNIX changes"             */
/*  *2    26-JUN-1989 10:54:24 GILLESPIE "(SPR 20) Fix castings"                    */
/*  *1    19-JUN-1989 13:11:08 SYSTEM ""                                            */
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DR_SCL_BAR.C                            */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_math.h"
#include "esi_c_lib.h"

#ifndef ESI_GL_PROJECT_H

#include "esi_gl_project.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine to construct and post the requested scale bar.                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mp_draw_scale_bar(DOUBLE wcsxmin,DOUBLE wcsxmax,DOUBLE wcsymin,DOUBLE wcsymax,  */
/*        DOUBLE wcs_to_inches,DOUBLE scale,CHAR *scale_uom,INT type);              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    wcsxmin         -(DOUBLE) The area in which to place the scale bar.           */
/*    wcsxmax         -(DOUBLE) "                                                   */
/*    wcsymin         -(DOUBLE) "                                                   */
/*    wcsymax         -(DOUBLE) "                                                   */
/*    wcs_to_inches   -(DOUBLE) Conversion factor for the window.                   */
/*    scale           -(DOUBLE) Numeric scale value to use.                         */
/*    scale_uom       -(CHAR *) Uom for scale.                                      */
/*    type            -(INT) Type of scale bar to display.                          */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*    The window is assumed allocated and open. The transform is assumed            */
/*    allocated.  Segment is assumed open.                                          */
/*                                                                                  */
/*    This is a general routine and the world coordinate system for the window      */
/*    is not assumed to match the scale bar being displayed.  Likewise,             */
/*    WCS_TO_INCHES is not assumed to match the scale info.                         */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mp_draw_scale_bar (DOUBLE wcsxmin, DOUBLE wcsxmax, DOUBLE wcsymin,
                                 DOUBLE wcsymax, DOUBLE wcs_to_inches, DOUBLE scale,
                                 CHAR *scale_uom, INT type)
#else
publicdef INT mp_draw_scale_bar (wcsxmin, wcsxmax, wcsymin, wcsymax, wcs_to_inches,
                                 scale, scale_uom, type)
DOUBLE wcsxmin;
DOUBLE wcsxmax;
DOUBLE wcsymin;
DOUBLE wcsymax;
DOUBLE wcs_to_inches;
DOUBLE scale;
CHAR *scale_uom;
INT type;
#endif
    {
    FLOAT x[2], y[2];
    FLOAT loc_x, loc_y;
    DOUBLE divider_length;
    DOUBLE char_param;
    UOM_NAME uom1, uom2;
    CHAR dummy[60];
    PROJECT_DEFAULT *defaults;
    DOUBLE mid_y;
    DOUBLE bar_to_inches;
    DOUBLE ndigs, dum1, dum2;
    INT i, j, base;
    INT color;
    
    /********************************************************************************/
    
    char_param = 1.0;
    wm_gschxp (char_param);
    wm_gslwsc (char_param);
    
    wm_gstxp (GRIGHT);
    
    wm_gsln (1);
    
    lu_text_to_index ("COLOR", &color, "WHITE");
    wm_gstxci (color);
    wm_gsplci (color);
    
    wm_gstxfp (SIMPLEX, GSTRKP);
    
    loc_x = 0.0;
    loc_y = 1.0;
    wm_gschup (loc_x, loc_y);
    
    /* * CALCULATE BAR TO INCHES *                                                  */
    /* ---------------------------                                                  */
    
    sprintf (dummy, "%f %s", scale, scale_uom);
    strcpy (uom1, "");
    tu_parse_unit (dummy, &bar_to_inches, uom1, uom2);
    dum1 = 1.0;
    tu_unit_conversion (dum1, uom2, &bar_to_inches, "INCHES");
    bar_to_inches /= scale;
    
    /* * MAKE SURE HAVE A UOM TO DISPLAY *                                          */
    /* -----------------------------------                                          */
    
    if (strlen (uom1) EQUALS 0)
        {
        am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&defaults);
        strcpy (uom1, defaults->unit.horizontal);
        }
    /* * COMPUTE DIVIDER_LENGTH IN WCS AND BASE IN BAR UNITS *                      */
    /* -------------------------------------------------------                      */
    
    dum1 = (wcsxmax - wcsxmin) * wcs_to_inches; /* BOX DELTA X IN INCHES            */
    dum2 = ((dum1 >= 2.5) ? 2 : (0.8 * dum1)) / bar_to_inches;
    
    ndigs = tc_magnitude (dum2);
    i = pow (10.0, ndigs) + 0.5;
    base = dum2 / i;
    base *= i;
    divider_length = ((DOUBLE)base)*bar_to_inches / wcs_to_inches;
    
    /* * SWITCH TO TYPE OF SCALE BAR DESIRED *                                      */
    /* ---------------------------------------                                      */
    
    switch (type)
        {
        
    case 1:
        
        dum1 = (wcsymax - wcsymin) * wcs_to_inches;
        dum2 = ((dum1 <= 1.0) ? (0.1 * dum1) : 0.1) / wcs_to_inches;
        
        char_param = MIN ((2.0 * dum2), (0.25 * divider_length / (ndigs + 1)));
        mid_y = (wcsymin + wcsymax) / 2.0;
        
        /* POSITION BAR BASE                                                        */
        
        x[0] = (wcsxmin + wcsxmax - divider_length) / 2.0;
        x[1] = x[0] + divider_length;
        y[0] = mid_y - dum2;
        y[1] = y[0];
        
        i = 2;
        wm_gpl (i, x, y);
        
        /* POSITION BAR UOM                                                         */
        
        loc_x = (x[0] + x[1]) / 2.0;
        loc_y = y[0] - dum2 - char_param;
        wm_gstxal (GACENT, GAVNOR);
        wm_gschh (char_param);
        wm_gtx (loc_x, loc_y, uom1);
        
        /* PLACE MAJOR VERTICALS                                                    */
        
        divider_length /= 2.0;
        base /= 2;
        y[1] = y[0] + (2.0 * dum2);
        loc_y = y[1] + dum2;
        for (j = 0; j < 3; j++)
            {
            x[1] = x[0];
            wm_gpl (i, x, y);
            
            loc_x = x[0];
            sprintf (dummy, "%d", j * base);
            wm_gtx (loc_x, loc_y, dummy);
            
            x[0] += divider_length;
            }
        /* PLACE MINOR VERTICALS                                                    */
        
        x[0] -= (1.5 * divider_length);
        y[1] = y[0] + dum2;
        for (j = 0; j < 2; j++)
            {
            x[1] = x[0];
            wm_gpl (i, x, y);
            x[0] -= divider_length;
            }
        break;
        }
    return(SUCCESS);
    }
/* END:                                                                             */
