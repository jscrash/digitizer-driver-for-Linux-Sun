/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_UPD_CORES.C */
/* *3    17-AUG-1990 22:15:35 VINCE "(SPR 5644) Code Cleanup" */
/* *2     1-NOV-1989 14:03:09 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:41:58 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_UPD_CORES.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This function is called if we have a non
    regular correspondance between cdp and sp
    it updates the seis_line_hdr structure
    corresponding to the line and return the
    cdp corresponding to the sp.

    Seis_line is the structure name of the line
    honor is a flag TRUE if we honor the
    correspondance already existing FALSE if we
    do not honor the correspondance regul is
    the flag saying if the correspondance is
    regular or not last_cdp is the cdp
    corresponding to the previous SP sp is the
    sp number.

Prototype:
    publicdef INT sez_upd_cores_cdp_sp (FULL_LINE_NAME *seis_line, SEIS_SP_ORDER regul, 
        INT honor, CDP last_cdp, SP sp,CDP *cdp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    regul           -(SEIS_SP_ORDER)
    honor           -(INT)
    last_cdp        -(CDP)
    sp              -(SP)
    (O) cdp         -(CDP *) Cdp is the cdp number corresponding to this sp.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sez_upd_cores_cdp_sp (FULL_LINE_NAME *seis_line, SEIS_SP_ORDER regul,
                                    INT honor, CDP last_cdp, SP sp, CDP *cdp)
#else
publicdef INT sez_upd_cores_cdp_sp (seis_line, regul, honor, last_cdp, sp, cdp)
FULL_LINE_NAME *seis_line;
SEIS_SP_ORDER regul;
INT honor;
CDP last_cdp;
SP sp;
CDP *cdp;
#endif
    {
    INT status;
    LINE_HDR *p_hdr;
    
    /* look for the structure corresponding to the */
    /* line                                        */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* compute the new value for the CDP           */
    
    *cdp = last_cdp + p_hdr->cdp_per_sp;
    
    /* update the structure                        */
    
    p_hdr->regular = regul;
    
    /* if honor we try to use the relation already
       in place if it is impossible we 'invent"
       the relation                                */
    
    return SUCCESS;
    }
/* END:     */
