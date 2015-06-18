/* DEC/CMS REPLACEMENT HISTORY, Element MD_LFN.C*/
/* *2    17-AUG-1990 21:57:23 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:06:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_LFN.C*/
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

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_MD_MSG_H

#include "esi_md_msg.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_ho.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Function to check the legality of filename stored in parameter
    structure.

Prototype:
    publicdef INT md_lfn(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_BAD_FILENAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_lfn (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_lfn (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_LFN");
    INT status;                     /* RETURN STATUS */
    CHAR text[STMAX];
    /* ********************************************************************** */
    
    status = ho_lgl_fname (parmstruc->parmval.cval);
    
    if (status != SUCCESS)
        {
        sprintf (text, mg_message (MD_BAD_NAME), parmstruc->parmval.cval);
        am_message (AM_ALERT, text);
        status = MD_BAD_FILENAME;
        }
    else
        {
        parmstruc->parmtype = 1;
        }
    return status;
    }
/* END:     */
