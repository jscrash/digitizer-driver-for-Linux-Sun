/* DEC/CMS REPLACEMENT HISTORY, Element HO_VAX_ROUT.C*/
/* *2    17-AUG-1990 21:54:18 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_VAX_ROUT.C*/
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

#include "esi_gl_defs.h"

#ifdef vms

#include "esi_er.h"
#include "esi_ho_desc.h"
#include "iodef.h"
#include "ssdef.h"

/* Function Description -----------------------------------------------------
Description:
    Logs a VAX system service error message.

Prototype:
    publicdef INT VAX_ROUTINE(INT message_id);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    message_id  -(INT) Message id number.

Return Value/Status:
    Function returns the message id number.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT VAX_ROUTINE (INT message_id) 
#else
publicdef INT VAX_ROUTINE (message_id)
INT message_id;
#endif
    {
    
    struct dsc$descriptor_s message_text;
    INT status;
    USHORT message_len;
    CHAR text[133];
    
    /*
     *  test for "normal" conditions - if all's well, return
     *  under VMS, odd status codes are success completions
     */
    if (message_id & 1)
        return message_id;
    
    message_text.dsc$w_length = 132;
    message_text.dsc$a_pointer = text;
    status = SYS$GETMSG (message_id, &message_len, &message_text, 15, 0);
    
    if (status EQUALS SS$_MSGNOTFND)
        {
        sprintf (text, "VMS Error - unknown error code %d (hex %x) !!!\n\n", status,
                 status);
        }
    else
        {
        *(text + message_len) = 0;
        }
    er_log (text);
    return message_id;
    }


#endif
/* END:     */
