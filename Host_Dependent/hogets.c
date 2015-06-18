/* DEC/CMS REPLACEMENT HISTORY, Element HOGETS.C*/
/* *4    14-AUG-1990 13:30:11 VINCE "(SPR 5644) Header Standardization"*/
/* *3    26-SEP-1989 18:29:12 VINCE "(SPR -1) minor fix"*/
/* *2    26-SEP-1989 18:21:45 VINCE "(SPR -1) added io redirection"*/
/* *1    21-JUL-1989 10:34:33 GILLESPIE "Removed in error"*/
/* DEC/CMS REPLACEMENT HISTORY, Element HOGETS.C*/
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

#include "esi_ho.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#define MAX_BUFF_SIZE (int)256

/* Function Description -----------------------------------------------------
Description:
    Move to host library and get a string from the terminal.

Prototype:
    publicdef INT hogets (VOIDPTR linein);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) linein      -(CHAR *) Line buffer.

Return Value/Status:
    FAIL - If EOF.
    SUCCESS - Otherwise.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hogets (VOIDPTR linein) 
#else
publicdef INT hogets (linein)
VOIDPTR linein;
#endif
    {
    static BOOL first = TRUE;
    static FILE *in;
    
    CHAR mac_device[31];
    CHAR *rtn;
    
    if (first)
        {
        qs_inq_c (QS_WORKSTATION_MAC_DEVICE, mac_device, (INT *)0);
        if (strlen (mac_device) == 0)
            strcpy (mac_device, "TT:");
        in = fopen (mac_device, "r");
        first = FALSE;
        }
    /*  Get string thru FGETS (to preserve the      */
    /*  End-of-Line character               */
    
    rtn = fgets ((char *)linein, MAX_BUFF_SIZE, in);
    return(rtn EQUALS NULL) ? FAIL : SUCCESS;
    }
/* END:     */
