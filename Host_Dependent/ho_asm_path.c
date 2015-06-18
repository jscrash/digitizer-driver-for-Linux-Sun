/* DEC/CMS REPLACEMENT HISTORY, Element HO_ASM_PATH.C*/
/* *4    17-AUG-1990 21:50:20 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     4-DEC-1989 12:44:33 GORDON "(SPR 0) fix spelling error on non-VMS side"*/
/* *2    24-SEP-1989 23:41:49 GILLESPIE "(SPR 103) Simplify by using Entryseparator (defined in esi_ho_files.h)"*/
/* *1    19-JUN-1989 12:56:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ASM_PATH.C*/
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
#include "esi_ho_files.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to assemble a path and a filename into a full pathname.

Prototype:
    publicdef INT ho_assemble_pathname(PATHNAME pathname, PATHNAME pathpart, 
            FILENAME filepart);
            
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) pathname    -(PATHNAME) The resulting pathname.
    (I) pathpart    -(PATHNAME) The path part of the pathname.
    (I) filepart    -(FILENAME) The file part of the pathname.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_assemble_pathname (PATHNAME pathname, PATHNAME pathpart,
                                    FILENAME filepart) 
#else
publicdef INT ho_assemble_pathname (pathname, pathpart, filepart)
PATHNAME pathname;
PATHNAME pathpart;
FILENAME filepart;
#endif
    {
    
    if (pathname == NULL)
        return FAIL;

#ifdef vms
    sprintf (pathname, "%s%s", pathpart, filepart);

#else
    sprintf (pathname, "%s%c%s", pathpart, ENTRYSEPERATOR, filepart);

#endif
    return SUCCESS;
    }
/* END:     */
