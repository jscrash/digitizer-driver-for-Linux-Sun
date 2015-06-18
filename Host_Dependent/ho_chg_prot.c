/* DEC/CMS REPLACEMENT HISTORY, Element HO_CHG_PROT.C*/
/* *2    17-AUG-1990 21:50:28 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:56:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_CHG_PROT.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ho_change_protection(FILENAME file_spec, INT mode);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    file_spec       -(FILENAME)
    mode            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_change_protection (FILENAME file_spec, INT mode) 
#else
publicdef INT ho_change_protection (file_spec, mode)
FILENAME file_spec;
INT mode;
#endif
    {
    INT status = SUCCESS;

#ifdef vms
    status = chmod (file_spec, mode);
#endif
    return status;
    }
/* END:     */
