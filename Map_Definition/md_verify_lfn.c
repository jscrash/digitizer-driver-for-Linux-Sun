/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_LFN.C*/
/* *2    17-AUG-1990 21:58:32 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_LFN.C*/
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

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to check the legality of filename stored in parameter
    structure.

Prototype:
    publicdef INT md_verify_lfn(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **)        

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns status from ho_lgl_fname.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_lfn (MD_KEY_INFO *key_info,
                             CHAR parm_string[MD_MAX_PARM_STRING],
                             MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_lfn (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    MD_PARM_STRUCT *temp_parm;
    
    /* ********************************************************************** */
    
    status = ho_lgl_fname (parm_string);
    if (status EQUALS SUCCESS)
        {
        temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_PARM_STRUCT));
        temp_parm->parmtype = 1;
        temp_parm->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL,
                                                       strlen (parm_string) + 1);
        strcpy (temp_parm->parmval.cval, parm_string);
        temp_parm->nxtparm = NULL_PARM;
        *parmstruc = temp_parm;
        }
    return status;
    }
/* END:     */
