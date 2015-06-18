/* DEC/CMS REPLACEMENT HISTORY, Element FM_CR_OS_NAME.C*/
/* *3    14-AUG-1990 13:24:33 VINCE "(SPR 5644) Header Standardization"*/
/* *2     7-NOV-1989 13:23:18 GILLESPIE "(SPR 30) Use esi_fm_int.h include file"*/
/* *1    19-JUN-1989 12:50:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FM_CR_OS_NAME.C*/
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

#include "esi_c_lib.h"
#include "esi_fm_int.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to create and return the OS filename
    the file with the given id.

Prototype:
    publicdef INT fm_create_os_filename(INT file_id,FILENAME os_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    file_id         -(INT)
    os_name         -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fm_create_os_filename (INT file_id, FILENAME os_name)
#else
publicdef INT fm_create_os_filename (file_id, os_name)
INT file_id;
FILENAME os_name;
#endif
    {
    
    /**************************************************************************/
    
    sprintf (os_name, "FM%ld", file_id);
    return SUCCESS;
    }
/* END:     */
