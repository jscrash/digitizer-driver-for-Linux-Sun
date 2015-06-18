/* DEC/CMS REPLACEMENT HISTORY, Element MD_EXPR.C*/
/* *2    17-AUG-1990 21:56:44 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:06:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_EXPR.C*/
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

#ifndef ESI_MD_MSG_H

#include "esi_md_msg.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Function to check the legality of an expression stored in 
    parameter.

Prototype:
    publicdef INT md_expr(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_BAD_EXPR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_expr (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_expr (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_EXPR");
    INT status;                     /* RETURN STATUS */
    CHAR text[STMAX];
    CHAR *str_out;
    /* ********************************************************************** */
    
    status = ee_parser (parmstruc->parmval.cval, &str_out);
    
    if (status != SUCCESS)
        {
        sprintf (text, mg_message (MD_BAD_EXP), parmstruc->parmval.cval);
        am_message (AM_ALERT, text);
        status = MD_BAD_EXPR;
        }
    return status;
    }
/* END:     */
