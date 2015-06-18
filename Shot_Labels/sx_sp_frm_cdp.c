/* DEC/CMS REPLACEMENT HISTORY, Element SX_SP_FRM_CDP.C*/
/* *4    17-AUG-1990 22:22:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-SEP-1989 18:38:46 GORDON "(SPR -1) checkin from gulf"*/
/* *2    25-JUL-1989 20:15:05 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SP_FRM_CDP.C*/
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

#include "esi_sx.h"

/* Function Description -----------------------------------------------------
Description:
    Sx shot from cdp.

Prototype:
    publicdef INT sx_sp_from_cdp(SX_STRUCT *psx,CDP cdp,SP *fshot);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    cdp             -(CDP)
    fshot           -(SP *)

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns status from se_sp_from_cdp.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_sp_from_cdp (SX_STRUCT *psx, CDP cdp, SP *fshot)
#else
publicdef INT sx_sp_from_cdp (psx, cdp, fshot)
SX_STRUCT *psx;
CDP cdp;
SP *fshot;
#endif
    {
    
    /* convert to shotpoint */
    
    if (psx->new_line)
        {
        *fshot = (cdp - psx->new_line_intercept) / psx->new_line_slope;
        return SUCCESS;
        }
    else
        {
        return se_sp_from_cdp (&psx->pso->current_line, cdp, fshot);
        }
    }
/* END:     */
