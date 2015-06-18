/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_VLD_OVRLY.C*/
/* *3    17-AUG-1990 22:37:23 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     2-NOV-1989 13:03:04 GILLESPIE "(SPR 31) Change for esi_xs_parse.h"*/
/* *1    19-JUN-1989 13:43:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_VLD_OVRLY.C*/
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

#include "esi_xs_parse.h"

/* Function Description -----------------------------------------------------
Description:
    Function to analyze and complete the verb/keyword
    structure for a cross section layout.

Prototype:
    publicdef INT xsz_vld_ovrly(MD_VERB_STRUCT *verbstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    verbstruc       -(MD_VERB_STRUCT *) Map def verb structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_vld_ovrly (MD_VERB_STRUCT *verbstruc)
#else
publicdef INT xsz_vld_ovrly (verbstruc)
MD_VERB_STRUCT *verbstruc;
#endif
    {
    INT status = SUCCESS;           /* RETURN STATUS */
    MD_KEY_STRUCT *keystruc;        /* KEYWORD STRUCTURE */
    
    /* ********************************************************************** */
    
    keystruc = verbstruc->keyptr;
    
    while ((status == SUCCESS) AND (keystruc != NULL_KEY))
        {
        if (NOT keystruc->valid)
            {
            keystruc->realverb = verbstruc->verb;
            status = md_vld_1key (keystruc);
            }
        keystruc = keystruc->nxtkey;
        }
    return status;
    }
/* END:     */
