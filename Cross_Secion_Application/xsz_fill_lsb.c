/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_LSB.C*/
/* *5    17-AUG-1990 22:35:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:51:57 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:14:32 JULIAN "(SPR -1) parmptr may be used before set"*/
/* *2    27-JUL-1989 20:22:57 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_LSB.C*/
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
    value into XS structure with the verb name "LOGPLOT SCALE BLOCK".

Prototype:
    publicdef INT xsz_fill_lsb(MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keyptr      -(MD_KEY_STRUCT *) A pointer points to the MD_KEY_STRUCT.
    (O) xsptr       -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_lsb (MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr)
#else
publicdef INT xsz_fill_lsb (keyptr, xsptr)
MD_KEY_STRUCT *keyptr;
XS_STRUCT *xsptr;
#endif
    {
    
    INT status = SUCCESS;
    MD_PARM_STRUCT *parmptr = (MD_PARM_STRUCT *)0;
    CHAR *keystr;
    INT ithick = 0;
    /* ********************************************************************** */
    
    keystr = keyptr->keywrd;
    
    xsptr->plo->scale_block.visible = TRUE;
    if (ARE_SAME (keystr, "NONE"))  /* if invisible */
        xsptr->plo->scale_block.visible = FALSE;
    
    if (ARE_SAME (keystr, "BORDER THICKNESS"))  /* put thickness */
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR) & ithick, &status);
        xsptr->plo->scale_block.line.width = (FLOAT)ithick;
        
        return status;
        }
    if (ARE_SAME (keystr, "COLOR")) /* put color */
        {
        xsz_get_parms (keyptr, parmptr, 1, (VOIDPTR) & (xsptr->plo->scale_block.line.color),
                       &status);
        return status;
        }
    return status;
    }
/* END:     */
