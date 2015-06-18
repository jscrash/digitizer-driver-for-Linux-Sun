/* DEC/CMS REPLACEMENT HISTORY, Element SE_FIRST_L_SP.C */
/* *4    17-AUG-1990 22:16:21 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:08:14 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:06:35 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:47:20 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_FIRST_L_SP.C */
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
    publicdef INT se_first_last_sp (FULL_LINE_NAME *seis_line,
        SP *first_sp, SP *last_sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) seis_line   -(FULL_LINE_NAME *) Structure composed of the
                        line name and the survey name.
    (O) first_sp    -(SP *) First_sp is the first sp on the line.
    (O) last_sp     -(SP *) Last_sp is the last sp on the line.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_first_last_sp (FULL_LINE_NAME *seis_line, SP *first_sp, SP *last_sp)
#else
publicdef INT se_first_last_sp (seis_line, first_sp, last_sp)
FULL_LINE_NAME *seis_line;
SP *first_sp, *last_sp;
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
    
    *first_sp = p_hdr->init_sp;
    *last_sp = p_hdr->final_sp;
    
    return SUCCESS;
    }
/* END:     */
