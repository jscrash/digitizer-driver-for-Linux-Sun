/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_UOMS.C*/
/* *4    13-NOV-1990 10:48:27 JESSIE "(SPR 0) check DMS input format"*/
/* *3    17-AUG-1990 21:59:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     7-MAR-1990 17:24:25 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *1    19-JUN-1989 13:08:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_UOMS.C*/
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

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to validate a UOMS parameter value ( size plus uom).

Prototype:
    publicdef INT md_verify_uoms(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **) Map def parameter structure.       

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns status from tu_parse_unit.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_uoms (MD_KEY_INFO *key_info,
                              CHAR parm_string[MD_MAX_PARM_STRING],
                              MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_uoms (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    MD_PARM_STRUCT *temp_parm;
    UOM_NAME uom1;                  /* FIRST UOM PARAMETER FOR PARSE UOM */
    UOM_NAME uom2;                  /* 2nd UOM PARAMETER FOR PARSE UOM */
    DOUBLE value;                   /* VALUE FOR PARSE UOM */
    
    /* ********************************************************************** */

    if (((status = tu_parse_unit (parm_string, &value, uom1, uom2)) != SUCCESS)
         AND ((status = con_dms2dec(parm_string, &value)) != SUCCESS))
        return status;
    
    temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_PARM_STRUCT));
        
    temp_parm->parmtype = 1;
    temp_parm->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL,
                                                   strlen (parm_string) + 1);
    strcpy (temp_parm->parmval.cval, parm_string);
        
    temp_parm->nxtparm = NULL_PARM;
    *parmstruc = temp_parm;
    return status;
    }
/* END:     */
