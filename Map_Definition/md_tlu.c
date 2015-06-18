/* DEC/CMS REPLACEMENT HISTORY, Element MD_TLU.C */
/* *4    17-AUG-1990 21:58:21 VINCE "(SPR 5644) Code Cleanup" */
/* *3    19-SEP-1989 13:37:22 GILLESPIE "(SPR 100) GULF MODS" */
/* *2    14-SEP-1989 16:38:33 GORDON "(SPR 100) Gulf mods" */
/* *1    14-SEP-1989 16:37:39 GORDON "Mapping table lookup function" */
/* DEC/CMS REPLACEMENT HISTORY, Element MD_TLU.C */
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
#include "esi_gl_defs.h"
#include "esi_am_defs.h"
#include "esi_md_defs.h"
#include "esi_mg.h"
#include "esi_lu.h"

/* Function Description -----------------------------------------------------
Description:
    Function to do table lookup parameter validation on given
    parameter.

Prototype:
    publicdef INT md_tlu(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

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
publicdef INT md_tlu (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_tlu (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    INT status;
    INT code;
    
    status = lu_text_to_index (keylist->val_arg, &code, parmstruc->parmval.cval);
    if (status != SUCCESS)
        am_message (AM_STATUS, mg_message (status));
    
    if (keylist->tran_flg EQUALS 'Y')
        {
        am_free (parmstruc->parmval.cval);
        parmstruc->parmval.ival = code;
        parmstruc->parmtype = 2;
        }
    else
        {
        parmstruc->parmtype = 1;
        }
    return SUCCESS;
    }
/* END:     */
