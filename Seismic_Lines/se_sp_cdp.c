/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_CDP.C */
/*  3B1  12-OCT-1990 17:54:20 GILLESPIE "Merge Ernie Deltas" */
/* *5    30-AUG-1990 18:09:35 GILLESPIE "(SPR 1) Pass correct data type to se_sp_from_cdp_intern (was INT, now is DOUBLE)" */
/* *4    17-AUG-1990 22:18:04 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:09:04 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:10:26 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:56:07 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_CDP.C */
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

#include "esi_se.h"

/* Function Description -----------------------------------------------------
Description:
    For the line this function gives
    the sp corresponding to the given cdp.

Prototype:
    publicdef INT se_sp_from_cdp (FULL_LINE_NAME *seis_line, CDP cdp, SP *sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    cdp             -(CDP)
    sp              -(SP *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_sp_from_cdp (FULL_LINE_NAME *seis_line, CDP cdp, SP *sp)
#else
publicdef INT se_sp_from_cdp (seis_line, cdp, sp)
FULL_LINE_NAME *seis_line;
CDP cdp;
SP *sp;
#endif
    {
    INT status;
    CDP_FLOAT cdp_float;
    LINE_HDR *p_hdr;

    /* look for the pointer to the structure       */
    /* describing the header                       */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO 0)
        {
        return status;
        }

    cdp_float = cdp;
    return se_sp_from_cdp_intern (p_hdr, cdp_float, sp);
    }

/* END:     */
