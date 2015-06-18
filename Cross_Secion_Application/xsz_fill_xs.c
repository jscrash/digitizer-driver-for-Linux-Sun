/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_XS.C*/
/* *6    27-MAR-1991 13:29:00 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *5    17-AUG-1990 22:35:45 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:52:22 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:27:58 JULIAN "(SPR -1) parmptr may be used before set"*/
/* *2    27-JUL-1989 20:23:40 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_XS.C*/
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

#include "esi_xs_parse.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To retrieve a layout information structure from an oracle database
    and make it to be parser language structure; Function to fill the 
    value into XS structure with the verb name "CROSS SECTION".

Prototype:
    publicdef INT xsz_fill_xs(MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keyptr      -(MD_KEY_STRUCT *) A pointer to MD_KEY_STRUCT.
    (O) xsptr       -(XS_STRUCT *) A pointer to XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_xs (MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr)
#else
publicdef INT xsz_fill_xs (keyptr, xsptr)
MD_KEY_STRUCT *keyptr;
XS_STRUCT *xsptr;
#endif
    {
    
    INT status = SUCCESS;
    MD_PARM_STRUCT *parmptr = (MD_PARM_STRUCT *)0;
    CHAR *keystr;
    /* ********************************************************************** */
    
    keystr = keyptr->keywrd;
    if (ARE_SAME (keystr, "TYPE"))
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR) & (xsptr->xs_type), &status);
        return status;
        }
    if (ARE_SAME (keystr, "DATUM"))
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->xs_datum_select, &status);
        return status;
        }
    if (ARE_SAME (keystr, "DISTANCE SCALE"))
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->xs_scale_str, &status);
        return status;
        }
    if (ARE_SAME (keystr, "WELL SPACING"))
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR) & (xsptr->xs_spacing), &status);
        return status;
        }
    if (ARE_SAME (keystr, "SUPPRESS VBAR"))
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR) & (xsptr->xs_suppress_vbar), &status);
        return status;
        }
    if (ARE_SAME (keystr, "TITLE"))
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->title.line1, &status);
        xsz_strip_parens (xsptr->title.line1);
        xsz_get_parms (keyptr, parmptr, 2, (VOIDPTR)xsptr->title.line2, &status);
        xsz_strip_parens (xsptr->title.line2);
        xsz_get_parms (keyptr, parmptr, 3, (VOIDPTR)xsptr->title.line3, &status);
        xsz_strip_parens (xsptr->title.line3);
        xsz_get_parms (keyptr, parmptr, 4, (VOIDPTR)xsptr->title.line4, &status);
        xsz_strip_parens (xsptr->title.line4);
        xsz_get_parms (keyptr, parmptr, 5, (VOIDPTR)xsptr->title.line5, &status);
        xsz_strip_parens (xsptr->title.line5);
        xsz_get_parms (keyptr, parmptr, 6, (VOIDPTR)xsptr->title.line6, &status);
        xsz_strip_parens (xsptr->title.line6);
        }
    return status;
    
    }
/* END:     */
