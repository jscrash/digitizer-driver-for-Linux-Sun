/* DEC/CMS REPLACEMENT HISTORY, Element SO_SSHPT_DEF.C*/
/* *4    26-JUL-1990 17:22:31 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:06:38 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:08:10 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_SSHPT_DEF.C*/
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

#include "esi_so.h"

/* Function Description -----------------------------------------------------
Description:
    Function to set shotpoint label defaults.

Prototype:
    publicdef INT so_set_shotpoint_defaults(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.                   

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_set_shotpoint_defaults (SO_STRUCT *pso)
#else
publicdef INT so_set_shotpoint_defaults (pso)
SO_STRUCT *pso;
#endif
    {
    INT status;
    
    status = SUCCESS;
    
    pso->seismic_generic.shotpoint_label = pso->disp_opt.shot_label_default;
    pso->seismic_generic.zvalue_label = pso->disp_opt.zvalue_default;
    
    return status;
    }
/* END:     */
