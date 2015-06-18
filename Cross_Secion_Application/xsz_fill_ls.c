/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_LS.C*/
/* *5    17-AUG-1990 22:35:03 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:51:52 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:19:12 JULIAN "(SPR -1) parmptr may be used before set, function xsz_fill_ls has return(e) and return;"*/
/* *2    27-JUL-1989 20:22:49 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_LS.C*/
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

/* Function Description -----------------------------------------------------
Description:
    To retrieve a layout information structure from an oracle database
    and make it to be parser language structure; Function to fill the 
    value into XS structure with the verb name "LOGPLOT SCALE".

Prototype:
    publicdef INT xsz_fill_ls(MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keyptr      -(MD_KEY_STRUCT *) A pointer to the MD_KEY_STRUCT.
    (I) xsptr       -(XS_STRUCT *) A pointer to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_ls (MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr)
#else
publicdef INT xsz_fill_ls (keyptr, xsptr)
MD_KEY_STRUCT *keyptr;
XS_STRUCT *xsptr;
#endif
    {
    INT status = SUCCESS;
    MD_PARM_STRUCT *parmptr = (MD_PARM_STRUCT *)0;
    CHAR *keystr;
    /* ********************************************************************** */
    
    keystr = keyptr->keywrd;
    if (ARE_SAME (keystr, "MEASURED"))  /* put measured */
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->depth_scale_str, &status);
        xsptr->depth_display_as = MEASURED_DEPTH;
        return status;
        }
    if (ARE_SAME (keystr, "SUBSEA"))    /* put subsea */
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->depth_scale_str, &status);
        xsptr->depth_display_as = SUBSEA_ELEV;
        return status;
        }
    if (ARE_SAME (keystr, "TIME"))  /* put time */
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->depth_scale_str, &status);
        xsptr->depth_display_as = TIME;
        return status;
        }
    if (ARE_SAME (keystr, "TVD"))   /* put tvd */
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR)xsptr->depth_scale_str, &status);
        xsptr->depth_display_as = TVD;
        return status;
        }
    return status;
    }
/* END:     */
