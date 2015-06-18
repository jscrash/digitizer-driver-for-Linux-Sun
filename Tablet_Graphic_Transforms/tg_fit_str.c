/* DEC/CMS REPLACEMENT HISTORY, Element TG_FIT_STR.C */
/* *5    17-AUG-1990 22:26:52 VINCE "(SPR 5644) Code Cleanup" */
/* *4     4-JUL-1990 20:27:19 SCD "(SPR 1) Ensure that character height is always positive." */
/* *3    19-JUN-1990 17:09:31 MING "(SPR 0) add status check under wm_gqtxx " */
/* *2    12-JUN-1990 10:13:32 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit " */
/* *1    12-JUN-1990 10:01:19 SCD "tg_fit_string move from tg_fit_text" */
/* DEC/CMS REPLACEMENT HISTORY, Element TG_FIT_STR.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_tg.h"

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Size text string to fit within a rectangle.

Prototype:
    publicdef INT tg_fit_string (CHAR *string, DOUBLE xmin, DOUBLE ymin, DOUBLE xmax, 
        DOUBLE ymax, DOUBLE chh, DOUBLE *char_height);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    -(I) string         -(CHAR *) Char pointer to allocated string.
    -(I) xmin           -(DOUBLE) -\
    -(I) ymin           -(DOUBLE) Rectangle extents.
    -(I) xmax           -(DOUBLE) 
    -(I) ymax           -(DOUBLE)       
    -(I) chh            -(DOUBLE) Char height to try.
    -(O) char_height    -(DOUBLE *) Returned character height which will
                            ensure that text will fit within the
                            limiting dimension of X or Y.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_fit_string (CHAR *string, DOUBLE xmin, DOUBLE ymin, DOUBLE xmax,
                             DOUBLE ymax, DOUBLE chh, DOUBLE *char_height)
#else
publicdef INT tg_fit_string (string, xmin, ymin, xmax, ymax, chh, char_height)
CHAR *string;
DOUBLE xmin, ymin, xmax, ymax;
DOUBLE chh;
DOUBLE *char_height;
#endif
    {
    
    DOUBLE height;                  /* Computed char height to use              */
    DOUBLE dx;                      /* Ratio of desired height to actual height */
    DOUBLE dy;                      /* Ratio of desired width to actual width   */
    
    GKS_REAL ext_x[5], ext_y[5];    /* the text extents array returned by GKS */
    GKS_REAL cat_x, cat_y;          /* the text concatonation coords from GKS */
    GKS_REAL old_x_up, old_y_up;    /* Current char up vector which we save, restore */
    GKS_REAL old_height;            /* Current char height which we save, restore */
    INT status = SUCCESS;
    INT count;
    
    *char_height = height = chh;
    
    /* ------------------------- */
    /* Validate input parameters */
    /* ------------------------- */
    if (strlen (string) == 0 || chh <= 0.0 || ABS (xmax - xmin) <= 0.0)
        {
        return FAIL;
        }
    /* ----------------------------- */
    /* Save the current text context */
    /* and restore when done         */
    /* ----------------------------- */
    wm_gqchup (&status, &old_x_up, &old_y_up);
    wm_gqchh (&status, &old_height);
    
    /* Set normal character orientation */
    wm_gschup (0.0, 1.0);
    
    /* determine the string's current extents */
    wm_gschh (height);
    wm_gqtxx (MAIN_WKID, xmin, ymin, string, &status, &cat_x, &cat_y, ext_x, ext_y);
    
    /***********************************/
    /* user default in case of failure */
    /* this is needed because for metafile workstation, status return 39 */
    /* saying that workstation is not a OUTPUT or OUTIN */
    /***********************************/
    
    if (status > 0)
        {
        *char_height = chh;
        return FAIL;
        }
    /* ----------------------------------------- */
    /* Extents returned by GSK are in the order: */
    /* LL, LR, UR, UL.  So to find the width, we */
    /* use x[1]-x[0], and for height we use      */
    /* y[2]-y[1]                                 */
    /* ----------------------------------------- */
    dx = ABS (xmax - xmin) / (ext_x[1] - ext_x[0]);
    if (ABS (ymax) > 0.0)
        {
        /* --------------------------------------- */
        /* In normal case of both width and height */
        /* constraints, apply the most restrictive */
        /* --------------------------------------- */
        dy = ABS (ymax - ymin) / (ext_y[2] - ext_y[1]);
        height *= MIN (dx, dy);
        }
    else
        {
        /* ----------------------------------- */
        /* If special case where size is to be */
        /* determined by width only, just      */
        /* increase or decrease by the x ratio */
        /* ----------------------------------- */
        height *= dx;
        }
    /* ------------------------ */
    /* Restore the text context */
    /* ------------------------ */
    wm_gschup (old_x_up, old_y_up);
    wm_gschh (old_height);
    
    *char_height = height;
    return SUCCESS;
    }
/* END:     */
