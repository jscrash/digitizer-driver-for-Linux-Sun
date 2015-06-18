/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_TLU.C*/
/* *4    17-AUG-1990 21:58:55 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    20-OCT-1989 11:32:28 CONROY "(SPR 5030) remove include file esi_md_tlu, make use of lu routines"*/
/* *2    10-OCT-1989 14:18:53 GORDON "(SPR 4999) Mods for new lookup method"*/
/* *1    19-JUN-1989 13:07:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_TLU.C*/
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

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to validate a TLU parameter value.

Prototype:
    publicdef INT md_verify_tlu(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **) Map def parameter structure.       

Return Value/Status:
    Function returns status from lu_text_to_index.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_tlu (MD_KEY_INFO *key_info,
                             CHAR parm_string[MD_MAX_PARM_STRING],
                             MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_tlu (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    MD_PARM_STRUCT *temp_parm;
    INT i;
    INT j;
    INT status;                     /* RETURN STATUS */
    
    /* ********************************************************************** */
    
    if ((status = lu_text_to_index (key_info->val_arg, &i, parm_string)) EQUALS SUCCESS)
        {
        temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_PARM_STRUCT));
        if (key_info->translate)
            {
            temp_parm->parmtype = 2;
            temp_parm->parmval.ival = i;
            }
        else
            {
            temp_parm->parmtype = 1;
            temp_parm->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL,
                                                           strlen (parm_string) + 1);
            strcpy (temp_parm->parmval.cval, parm_string);
            }
        temp_parm->nxtparm = NULL_PARM;
        *parmstruc = temp_parm;
        }
    return status;
    }
/* END:     */
