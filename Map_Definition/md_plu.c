/* DEC/CMS REPLACEMENT HISTORY, Element MD_PLU.C*/
/* *2    17-AUG-1990 21:57:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_PLU.C*/
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
#include "esi_md_defs.h"
#include "esi_md_err.h"
#include "esi_md.h"

/* Function Description -----------------------------------------------------
Description:
    Function to do Project table look up validation.

Prototype:
    publicdef INT md_plu(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    MD_UNRECOGNIZED_VALID_CODE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_plu (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_plu (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_PLU");
    INT status;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (strcmp (keylist->val_arg, "LITHOSTRAT_CODES") EQUALS 0)
        {
        status = md_litho (parmstruc);
        }
    else if (strcmp (keylist->val_arg, "SEIS_HOR_CODES") EQUALS 0)
        {
        status = md_seis_hor (parmstruc);
        }
    else if (strcmp (keylist->val_arg, "SEIS_FAULT_CODES") EQUALS 0)
        {
        status = md_seis_fault (parmstruc);
        }
    else
        {
        status = MD_UNRECOGNIZED_VALID_CODE;
        }
    return status;
    }
/* END:     */
