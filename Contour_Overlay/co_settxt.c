/* DEC/CMS REPLACEMENT HISTORY, Element CO_SETTXT.C*/
/* *4    27-NOV-1990 16:28:03 JULIAN "(SPR 6198) resize contour labels"*/
/* *3    14-AUG-1990 13:06:21 VINCE "(SPR 5644) Header Standardization"*/
/* *2    18-JUL-1989 14:29:14 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:44:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_SETTXT.C*/
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
#include "esi_math.h"

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    ESI developed routine to override the DGI text sizes.  Thus,
    eventually the text characteristics for the labels may be
    user specified.  This is the reason for the inclusion of
    "contours" in the function parameter list.  Currently,
    the text characteristics are hard-wired in as follows:

        FONT = SIMPLEX
        PRECISION = "STROKE"
        EXPANSION FACTOR = 1.15
        SPACING = 0
        HEIGHT = 0.02
        UP VECTOR = (0,1)
        PATH = RIGHT
        ALIGNMENT = NORMAL

    From these characteristics the width of the characters 
    which make up the labels (currently limited to the digits,
    ".", "-", and "+") can be determined and sent to DGI. 

Prototype:
    publicdef INT co_settxt(CONTOURS *contours, DOUBLE scale);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour information structure.
    (I) scale       -(DOUBLE)  Character scale size.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_settxt (CONTOURS *contours, DOUBLE scale) 
#else
publicdef INT co_settxt (contours, scale)
CONTOURS *contours;
DOUBLE scale;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;                     /* RETURN STATUS */
    MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE */
    FLOAT xrect[4];                 /* X VALUES FOR EXTENT RECTANGLE */
    FLOAT yrect[4];                 /* Y VALUES FOR EXTENT RECTANGLE */
    FLOAT xcncat;                   /* X VALUE FOR CONCATENATION POINT */
    FLOAT ycncat;                   /* Y VALUE FOR CONCATENATION POINT */
    INT errind;                     /* GKS ERROR INDICATOR */
    INT i;
    INT batch;
    CHAR dum_string[2];             /* DUMMY CHARACTER STRING */
    FLOAT charsize[14];             /* CHARACTER SIZE INFO: */
    /*  CHARSIZE[0]  = CHAR HEIGHT */
    /*  CHARSIZE[1]  = WIDTH OF "0" */
    /*  CHARSIZE[2]  = WIDTH OF "1" */
    /*  CHARSIZE[3]  = WIDTH OF "2" */
    /*  CHARSIZE[4]  = WIDTH OF "3" */
    /*  CHARSIZE[5]  = WIDTH OF "4" */
    /*  CHARSIZE[6]  = WIDTH OF "5" */
    /*  CHARSIZE[7]  = WIDTH OF "6" */
    /*  CHARSIZE[8]  = WIDTH OF "7" */
    /*  CHARSIZE[9]  = WIDTH OF "8" */
    /*  CHARSIZE[10] = WIDTH OF "9" */
    /*  CHARSIZE[11] = WIDTH OF "." */
    /*  CHARSIZE[12] = WIDTH OF "-" */
    /*  CHARSIZE[13] = WIDTH OF "+" */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * GET MAP WINDOW AND COMPUTE CHARSIZE[0] * */
    /* ------------------------------------------ */
    
    mp_get_current_map (&map);
    charsize[0] = (map->upper_right_xy.y - map->lower_left_xy.y) * 0.01;
    
    /* * CHECK IF IN BATCH (PLOTTING) * */
    /* -------------------------------- */
    
    qs_inq_i (QS_IF_BATCH, &batch);
    if (batch)
        {
        charsize[0] *= scale;
        for (i = 1; i < 14; i++)
            {
            charsize[i] = charsize[0] * 1.05;
            }
        }
    /* * SET GKS VARIABLES * */
    /* --------------------- */
    
    else
        {
        wm_gschh (charsize[0]);     /* HEIGHT */
        wm_gstxfp (SIMPLEX, GSTRKP);    /* FONT AND PRECISION */
        wm_gschxp (1.15);           /* EXPANSION FACTOR */
        wm_gschsp (0.1);            /* SPACING */
        wm_gschup (0.0, 1.0);       /* UP VECTOR */
        wm_gstxp (GRIGHT);          /* PATH */
        wm_gstxal (GAHNOR, GAVNOR); /* ALIGNMENT */
        
        /* * GET CHARACTER WIDTHS * */
        /* ------------------------ */
        
        /* "0" */
        
        dum_string[0] = '0';
        dum_string[1] = '\0';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[1] = fabs (xrect[1] - xrect[0]);
        
        /* "1" */
        
        dum_string[0] = '1';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[2] = fabs (xrect[1] - xrect[0]);
        
        /* "2" */
        
        dum_string[0] = '2';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[3] = fabs (xrect[1] - xrect[0]);
        
        /* "3" */
        
        dum_string[0] = '3';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[4] = fabs (xrect[1] - xrect[0]);
        
        /* "4" */
        
        dum_string[0] = '4';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[5] = fabs (xrect[1] - xrect[0]);
        
        /* "5" */
        
        dum_string[0] = '5';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[6] = fabs (xrect[1] - xrect[0]);
        
        /* "6" */
        
        dum_string[0] = '6';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[7] = fabs (xrect[1] - xrect[0]);
        
        /* "7" */
        
        dum_string[0] = '7';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[8] = fabs (xrect[1] - xrect[0]);
        
        /* "8" */
        
        dum_string[0] = '8';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[9] = fabs (xrect[1] - xrect[0]);
        
        /* "9" */
        
        dum_string[0] = '9';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[10] = fabs (xrect[1] - xrect[0]);
        
        /* "." */
        
        dum_string[0] = '.';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[11] = fabs (xrect[1] - xrect[0]);
        
        /* "-" */
        
        dum_string[0] = '-';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[12] = fabs (xrect[1] - xrect[0]);
        
        /* "+" */
        
        dum_string[0] = '+';
        wm_gqtxx (MAIN_WKID, 0.0, 0.0, dum_string, &errind, &xcncat, &ycncat, xrect,
                  yrect);
        charsize[13] = fabs (xrect[1] - xrect[0]);
        
        for (i = 0; i < 14; i++)
            {
            charsize[i] *= scale;
            }
        }
    /* * SEND CHARSIZE TO DGI * */
    /* ------------------------ */
    
    co_esichr_gc (charsize, 14);
    
    return status;
    }
/* END:     */
