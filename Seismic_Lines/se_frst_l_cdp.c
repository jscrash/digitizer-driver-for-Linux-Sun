/* DEC/CMS REPLACEMENT HISTORY, Element SE_FRST_L_CDP.C */
/* *3    17-AUG-1990 22:16:25 VINCE "(SPR 5644) Code Cleanup" */
/* *2    18-JUN-1990 12:08:21 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *1     5-MAR-1990 16:59:58 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_FRST_L_CDP.C */
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

Prototype:
    publicdef INT se_first_last_cdp (FULL_LINE_NAME *seis_line,
        CDP *first_cdp, CDP *last_cdp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) seis_line   -(FULL_LINE_NAME *) Structure composed of the
                        line name and the survey name.
    (O) first_cdp   -(CDP *) First_cdp is the first cdp on the line.
    (O) last_cdp    -(CDP *) Last_cdp is the last cdp on the line.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_first_last_cdp (FULL_LINE_NAME *seis_line, CDP *first_cdp,
                                 CDP *last_cdp)
#else
publicdef INT se_first_last_cdp (seis_line, first_cdp, last_cdp)
FULL_LINE_NAME *seis_line;
CDP *first_cdp, *last_cdp;
#endif
    {
    LINE_HDR *p_hdr;
    INT status;
    /* look for the pointer to the structure       */
    /* describing the header                       */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* fill values for first and last cdp          */
    
    *first_cdp = p_hdr->init_cdp;
    *last_cdp = p_hdr->final_cdp;
    
    return SUCCESS;
    }
/* END:     */
