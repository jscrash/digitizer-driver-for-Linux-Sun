/* DEC/CMS REPLACEMENT HISTORY, Element MD_SLE.C*/
/* *2    17-AUG-1990 21:58:06 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_SLE.C*/

/* DEC/CMS REPLACEMENT HISTORY, Element MD_SLE.C*/
/* *3    17-DEC-1988 14:47:25 HALL "(SPR 4981) Unisource"*/
/* *2     3-NOV-1987 16:45:40 DAVID "(SPR 3332) Remove am_message call"*/
/* *1     7-JUL-1987 11:02:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_SLE.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_md_defs.h"
#include "esi_md_err.h"
#include "esi_am.h"
#include "esi_sl.h"
#include "esi_nl.h"

/* Function Description -----------------------------------------------------
Description:
    Function to check the existence of the select list provided
    by the given parameter.

Prototype:
    publicdef INT md_sle(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_sle (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_sle (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    INT i;                          /* DUMMY INDEX */
    NLIST_HEADER temp;              /* TEMPORARY NLIST FOR ANALYSIS */
    CHAR text[STMAX];
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    for (i = 1; i <= SL_MAX_DATA_TYPES; i++)
        {
        status = sl_init_select_list ((SL_DATA_TYPE)i, &temp);
        if (status != SUCCESS)
            {
            goto finished;
            }
        status = sl_read_list ((SL_DATA_TYPE)i, parmstruc->parmval.cval, &temp);
        nl_free_nlist (temp);
        if (status EQUALS SUCCESS)
            {
            parmstruc->parmtype = 1;
            goto finished;
            }
        }
    /* * RETURN * */
    /* ---------- */
    
finished:
    return status;
    }
/* END:     */
