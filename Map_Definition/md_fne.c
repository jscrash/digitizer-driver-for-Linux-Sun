/* DEC/CMS REPLACEMENT HISTORY, Element MD_FNE.C*/
/* *2    17-AUG-1990 21:57:01 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:06:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_FNE.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_MD_DEFS_H

#include "esi_md_defs.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_MD_MSG_H

#include "esi_md_msg.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to check existence of filename stored in parameter
    structure.

Prototype:
    publicdef INT md_fne(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_FILE_DOES_NOT_EXIST
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_fne (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_fne (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_FNE");
    INT status;                     /* RETURN STATUS */
    CHAR text[STMAX];
    FILENAME filename;
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (strlen (keylist->val_arg) != 0)
        {
        ho_add_path (keylist->val_arg, parmstruc->parmval.cval, sizeof(FILENAME),
                     filename);
        }
    else
        {
        strcpy (filename, parmstruc->parmval.cval);
        }
    if (NOT hoexists (filename))
        {
        sprintf (text, mg_message (MD_FILE_NOT_EXIST), filename);
        am_message (AM_ALERT, text);
        status = MD_FILE_DOES_NOT_EXIST;
        }
    else
        {
        parmstruc->parmtype = 1;
        }
    return status;
    }
/* END:     */
