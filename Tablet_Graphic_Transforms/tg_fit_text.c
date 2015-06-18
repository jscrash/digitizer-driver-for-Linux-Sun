/* DEC/CMS REPLACEMENT HISTORY, Element TG_FIT_TEXT.C*/
/* *6    17-AUG-1990 22:27:01 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    12-JUN-1990 10:13:54 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit "*/
/* *4    20-APR-1990 18:30:33 JULIAN "(SPR 0) fix text leaking"*/
/* *3    18-APR-1990 10:06:01 JULIAN "(SPR 1) fix depth tracks bad aspect ratio"*/
/* *2    18-SEP-1989 12:44:11 JULIAN "Gulf mods under SPR 100"*/
/* *1    19-JUN-1989 13:32:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_FIT_TEXT.C*/
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
    Size text strings to fit within a rectangle.
    This module contains two similar routines: tg_fit_text and tg_fit_string.
    The following description is for tg_fit_text, the only difference is
    that tg_fit_string is called with a single string pointer while
    tg_fit_text is called with a linked list of n strings.
    
    The function tg_fit_text returns the GKS expansion factor that best fits
    the text supplied in the linked list within a rectangular
    region described by the coords in the argument list.  The longest string
    in the linked list is fitted to the rectangle and all the rest of the
    strings will cascade inward from there.   
    
    NOTES:  (1) A call to this routine will reset the GKS text alignment
                attribute to 'normal' upright mode and the character
                spacing (in X) will be reset to zero.
    
            (2) The text font should be set by the calling routine before
                calling tg_fit_text or tg_fit_string. Different fonts take
                different amounts of screen real estate (of course!).
    
            (3) A window must be open and the active transformation must
                of course be the same one that the text string(s) will
                eventually be drawn with.
    
            (4) The strings may be either enlarged or shrunk to fit the
                the rectangular region.  Thus, the text/rectangle size
                relationship should at least be reasonable.  If it is not
                you will probably get either a color bar (highly compressed
                text) or individual letters that are incredibly wide!
    
            (5) The height of the text is not adjusted to fit the height
                of the enclosing rectangle.  This routine only cares about
                tailoring the X values.

Prototype:
    publicdef INT tg_fit_text(LISTHEAD *llist,DOUBLE xmin,DOUBLE ymin,DOUBLE xmax,
        DOUBLE ymax,DOUBLE chh,DOUBLE *char_height);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) llist       -(LISTHEAD *) Pointer to a linked list of strings.
    (I) xmin        -(DOUBLE) -\
    (I) ymin        -(DOUBLE) Left most edge of rect. (start loc).
    (I) xmax        -(DOUBLE) -\
    (I) ymax        -(DOUBLE) Right most edge of rect. (limit).
    (I) char_size   -(DOUBLE) Char height pre-calculated by the
                        caller. Char height effects the width
                        therefore must be pre-determined.
    (O) exp_factor  -(DOUBLE *) The returned expansion factor to call
                        GSCHXP with before drawing the text.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_fit_text (LISTHEAD *llist, DOUBLE xmin, DOUBLE ymin, DOUBLE xmax,
                           DOUBLE ymax, DOUBLE chh, DOUBLE *char_height)
#else
publicdef INT tg_fit_text (llist, xmin, ymin, xmax, ymax, chh, char_height)
LISTHEAD *llist;
DOUBLE xmin, xmax, ymax, ymin;
DOUBLE chh;
DOUBLE *char_height;
#endif
    {
    DOUBLE text_x3d, text_y3d,      /* 3d text representation vars.           */
        text_exp,                   /* expansion factor                       */
        text_x, text_y,             /* coords for 'drawing' string            */
        ch_h;                       /* char height                */
    
    /*----------------------------------*/
    /* delta x.  The difference between */
    /* the max x of a string and the    */
    /* max x of the rectangular region  */
    /*----------------------------------*/
    DOUBLE dx;
    
    GKS_REAL ext_x[5], ext_y[5];    /* the text extents array returned by GKS */
    GKS_REAL cat_x, cat_y;          /* the text concatonation coords from GKS */
    
    DOUBLE max_extent_x,            /* string by string maximum extent value  */
        max_extent_y;               /* tracker.                               */
    
    INT nlines,                     /* string count                           */
        i,                          /* loop control                           */
        wkid,                       /* workstation id always is MAIN_WKID     */
        status = SUCCESS;           /* Hopefully never FAIL!                  */
    
    CHAR *str,                      /* general purpose string pointer         */
        *maxstr;                    /* pointer to the longest string found    */
    
    /* Reset text characteristics to initial (normal) state                    */
    text_x3d = 0.0;                 /* normally aligned text                */
    text_y3d = 1.0;
    text_exp = 1.0;                 /* normal ratio for text drawing        */
    
    wm_gschxp (text_exp);           /* set def. exp.  in the GKS state list */
    wm_gschup (text_x3d, text_y3d); /* normalize the 3d text alignment      */
    
    /* a little setting up - dummy vars etc.                                   */
    wkid = MAIN_WKID;               /* seems to always work! taken from ESI_GKS.H      */
    
    text_x = xmin;                  /* start initially at left edge of rect.           */
    text_y = ymin;
    
    max_extent_x = 0.0;             /* reset */
    max_extent_y = 0.0;
    
    /* count the strings and check the validity of */
    /* the list                                    */
    nlines = llcount (llist);
    if (nlines <= 0)                /* negative values indicate a bad list         */
        {                           /* operation a return of 0 elements is also an */
        /* error                                       */
        /*-----------------------------------------*/
        /* if the linked list of strings is either */
        /* empty or screwed up, return FAIL        */
        /*-----------------------------------------*/
        *char_height = chh;
        
        return FAIL;
        }
    /* loop through each string in the linked list */
    /* to determine the longest string, not in     */
    /* chars but by the max extent X               */
    for (i = 0; i < nlines; i++)
        {
        /* fetch characterpointer from list */
        status = (i == 0) ? llfirst (llist,
                                     (BYTE *) & str) : llnext (llist, (BYTE *) & str);
        
        /* be cautious with the linked list (you don't know where it's been!)  */
        if (status == FAIL OR (status < 0))
            {
            /*     if error, return def. expansion factor  */
            *char_height = chh;
            return FAIL;
            }
        /*      clean the white space from the strings */
        /*      in order to get accurate lengths       */
        str = ts_snowhite (str);
        
        if (strlen (str) == 0)      /* 2/6/89 check for empty string */
            continue;
        
        /* determine the extents of this 'vanilla string' (exp = 1, no spaces) */
        wm_gqtxx (wkid, text_x, text_y, str, &status, &cat_x, &cat_y, ext_x, ext_y);
        
        /* make sure GKS was happy */
        if (status > 0)             /* GKS returns pos. integers for errors    */
            {
            *char_height = chh;
            return FAIL;
            }
        /* is this our longest string yet? */
        if (ext_x[2] > max_extent_x)
            {
            max_extent_x = ext_x[2];
            maxstr = str;           /* if longest, hang on to it            */
            }
        }
    status = tg_fit_string (maxstr, xmin, ymin, xmax, ymax, chh, char_height);
    
    return status;
    }
/* END:     */
