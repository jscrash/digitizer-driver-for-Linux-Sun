/* DEC/CMS REPLACEMENT HISTORY, Element TG_FIT_STRS.C */
/* *3    17-AUG-1990 22:26:57 VINCE "(SPR 5644) Code Cleanup" */
/* *2    12-JUN-1990 10:13:44 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit " */
/* *1    12-JUN-1990 10:03:23 SCD "handle more than one string as tg_fit_string" */
/* DEC/CMS REPLACEMENT HISTORY, Element TG_FIT_STRS.C */
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

#include "esi_c_lib.h"
#include "esi_tg.h"

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Size text strings to fit within a rectangle.
    The function tg_fit_strings returns the character height that best fits
    the text supplied in the linked list within a rectangular
    region described by the coords in the argument list.  The longest string
    in the linked list is fitted to the rectangle and all the rest of the
    strings will cascade inward from there.   
    
    NOTES:  
    
        (1) The text context (font, etc.) should be pre-set by 
            the calling routine. 
    
        (2) A window must be open and the active transformation must
            of course be the same one that the text string(s) will
            eventually be drawn with.

Prototype:
    publicdef INT tg_fit_strings (LISTHEAD *llist, DOUBLE xmin, DOUBLE ymin, DOUBLE xmax, 
        DOUBLE ymax, DOUBLE line_spacing, DOUBLE *char_height);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) llist       -(LISTHEAD *) Pointer to a linked list of strings.
    (I) xmin        -(DOUBLE) -\
    (I) ymin        -(DOUBLE) Left most edge of rect. (start loc).
    (I) xmax        -(DOUBLE)
    (I) ymax        -(DOUBLE) Right most edge of rect. (limit).
    (I) char_size   -(DOUBLE) Char height pre-calculated by the
                        caller. Char height effects the width
                        therefore must be pre-determined.
    (O) exp_fatcor  -(DOUBLE *) The returned expansion factor to call
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
publicdef INT tg_fit_strings (LISTHEAD *llist, DOUBLE xmin, DOUBLE ymin, DOUBLE xmax,
                              DOUBLE ymax, DOUBLE line_spacing, DOUBLE *char_height)
#else
publicdef INT tg_fit_strings (llist, xmin, ymin, xmax, ymax, line_spacing, char_height)
LISTHEAD *llist;
DOUBLE xmin, xmax, ymax, ymin;
DOUBLE line_spacing;
DOUBLE *char_height;
#endif
    {
    DOUBLE max_height;
    DOUBLE height;                  /* height determined for the current string         */
    DOUBLE test_height;             /* the smallest char height req'd for all strings   */
    /*----------------------------------------*/
    /* total height of all strings, given the */
    /* height determined from their widths    */
    /*----------------------------------------*/
    DOUBLE total_height;
    
    INT nlines,                     /* string count                                     */
        i,                          /* loop control                                     */
        status = SUCCESS;           /* returned status                                  */
    
    CHAR *str;                      /* the current string                               */
    
    /* -------------------------------------- */
    /* Set the default return value of height */
    /* -------------------------------------- */
    *char_height = max_height = ABS (ymax - ymin);
    
    /* ------------------------------- */
    /* Init smallest char found so far */
    /* to height of rectangle          */
    /* ------------------------------- */
    test_height = max_height;
    
    /* --------------------------- */
    /* count the strings and check */
    /* the validity of the list    */
    /* --------------------------- */
    nlines = llcount (llist);
    if (nlines <= 0)
        {
        return FAIL;
        }
    /* ------------------------------------ */
    /* If there is only one string, we just */
    /* return results from tg_fit_string    */
    /* ------------------------------------ */
    if (nlines == 1)
        {
        llfirst (llist, (BYTE *) & str);
        status = tg_fit_string (str, xmin, ymin, xmax, ymax, max_height, char_height);
        return status;
        }
    /* ------------------------------------------- */
    /* loop through each string in the linked list */
    /* to determine the smallest character req'd   */
    /* by the width of all strings                 */
    /* ------------------------------------------- */
    for (i = 0; i < nlines; i++)
        {
        /* fetch next string from list */
        status = (i == 0) ? llfirst (llist,
                                     (BYTE *) & str) : llnext (llist, (BYTE *) & str);
        if (status == FAIL || (status < 0))
            {
            return FAIL;
            }
        if (strlen (str) == 0)
            continue;
        
        /* -------------------------------------------------- */
        /* Calc height based only on the width of each string */
        /* (Signalled by the bloody !! ymax == 0.0 flag)      */
        /* -------------------------------------------------- */
        status = tg_fit_string (str, xmin, ymin, xmax, 0.0, max_height, &height);
        
        if (status != SUCCESS)
            {
            return FAIL;
            }
        if (height < test_height)
            {
            test_height = height;
            }
        }
    /* -------------------------------------- */
    /* Now we check the total height, because */
    /* that may be the limiting constraint    */
    /* -------------------------------------- */
    
    total_height = nlines * test_height + (nlines - 1) * test_height *
                   (line_spacing - 1.0);
    if (total_height > max_height)
        {
        test_height *= max_height / total_height;
        }
    *char_height = test_height;
    return status;
    }
/* END:     */
