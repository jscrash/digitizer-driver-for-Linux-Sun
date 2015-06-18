/* DEC/CMS REPLACEMENT HISTORY, Element MD_UOM.C*/
/* *3    17-AUG-1990 21:58:24 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     7-MAR-1990 17:24:10 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *1    19-JUN-1989 13:07:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_UOM.C*/
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
#include "esi_tu.h"

/* Function Description -----------------------------------------------------
Description:
    Function to validate a UOM parameter value.

    -- Currently does nothing but returns SUCCESS.

Prototype:
    publicdef INT md_uom(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns status from tu_parse_unit.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_uom (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_uom (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_UOM");
    INT status;                     /* RETURN STATUS */
    UOM_NAME uom1;                  /* FIRST UOM PARAMETER FOR PARSE UOM */
    UOM_NAME uom2;                  /* 2nd UOM PARAMETER FOR PARSE UOM */
    DOUBLE value;                   /* VALUE FOR PARSE UOM */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    status = tu_parse_unit (parmstruc->parmval.cval, &value, uom1, uom2);
    if (status EQUALS SUCCESS)
        {
        parmstruc->parmtype = 1;
        }
    return status;
    }
/* END:     */
