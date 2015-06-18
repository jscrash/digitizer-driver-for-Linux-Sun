/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_LH.C*/
/* *6    17-AUG-1990 22:34:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    29-MAR-1990 16:25:55 MING "(SPR 5173) gulf bug fix"*/
/* *4     2-NOV-1989 12:51:48 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:14:18 JULIAN "(SPR -1) parmptr may be used before set"*/
/* *2    27-JUL-1989 20:22:40 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_LH.C*/
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

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    To retrieve a layout information structure from an oracle database
    and make it to be parser language structure; Function to fill the 
    value into XS structure with the verb name "LOGPLOT HEADER".

Prototype:
    publicdef INT xsz_fill_lh(MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keyptr      -(MD_KEY_STRUCT *) A pointer points to the MD_KEY_STRUCT.
    (I) xsptr       -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_lh (MD_KEY_STRUCT *keyptr, XS_STRUCT *xsptr)
#else
publicdef INT xsz_fill_lh (keyptr, xsptr)
MD_KEY_STRUCT *keyptr;
XS_STRUCT *xsptr;
#endif
    {
    
    INT status = SUCCESS;
    MD_PARM_STRUCT *parmptr = (MD_PARM_STRUCT *)0;
    CHAR *keystr;
    INT dummy;
    INT item_num = 0, item_index;
    INT value = 1;
    INT i;
    INT ithick = 0;
    /* ********************************************************************** */
    
    keystr = keyptr->keywrd;
    if (ARE_SAME (keystr, "ANGLE")) /* put angle */
        xsz_get_parms (keyptr, parmptr, value, (VOIDPTR) & (xsptr->plo->log_hdr.angle),
                       &status);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    
    if (ARE_SAME (keystr, "COLOR")) /* put color */
        {
        xsz_get_parms (keyptr, parmptr, value, (VOIDPTR) & (xsptr->plo->log_hdr.line.color),
                       &status);
        return status;
        }
    if (ARE_SAME (keystr, "BORDER THICKNESS"))  /* put thickness */
        {
        
        xsz_get_parms (keyptr, parmptr, value, (VOIDPTR) & ithick, &status);
        xsptr->plo->log_hdr.line.width = (DOUBLE)ithick;
        return status;
        }
    xsptr->plo->log_hdr.visible_border = TRUE;
    if (ARE_SAME (keystr, "NO BORDER")) /* if border not visible */
        {
        xsptr->plo->log_hdr.visible_border = FALSE;
        return SUCCESS;
        }
    if (ARE_SAME (keystr, "ITEM"))  /* put item */
        {
        if (keyptr->parmptr EQUALS (MD_PARM_STRUCT *)0) /* GULF bug fix */
            {
            item_num = 0;
            }
        else
            {
            do
                {
                item_num++;
                xsz_get_parms (keyptr, parmptr, item_num, (VOIDPTR) & dummy, &status);
                } while (status == SUCCESS);
            item_num--;
            }
        xsptr->plo->log_hdr.total_items = item_num;
        if (item_num > 0)
            {
            xsptr->plo->log_hdr.items = (INT *)tc_zalloc (sizeof(INT) * (item_num));
            for (i = 1; i <= item_num; i++)
                {
                xsz_get_parms (keyptr, parmptr, i,
                               (VOIDPTR) & (xsptr->plo->log_hdr.items[i - 1]),
                               &status);
                }
            }
        return status;
        }
    xsptr->plo->log_hdr.visible = TRUE;
    if (ARE_SAME (keystr, "NONE"))  /* if invisible */
        xsptr->plo->log_hdr.visible = FALSE;
    return status;
    }
/* END:     */
