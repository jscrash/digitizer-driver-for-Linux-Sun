/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_FILE.C*/
/* *2    17-AUG-1990 21:52:10 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:58:51 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_FILE.C*/
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

#include "esi_ctype.h"
#include "esi_ho_files.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    This function takes a filename as input and adds "F_" prefix to it         *
    if it begins with a digit                                    *

Prototype:
    publicdef INT ho_get_filename (FILENAME inName, FILENAME outName);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) inName      -(FILENAME) In file name.
    (O) outName     -(FILENAME) Modified file name.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    No-op on VMS.                                                 

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_get_filename (FILENAME inName, FILENAME outName) 
#else
publicdef INT ho_get_filename (inName, outName)
FILENAME inName;
FILENAME outName;
#endif
    {                               /* start of HO_GET_FILENAME */
#if UNIX
    strcpy ((char *)outName, (char *)inName);
#else       /* not UNIX */

#ifdef vms
    strcpy ((char *)outName, (char *)inName);
#endif      /* VMS */

#ifdef primos
    /*  On PRIMOS, the first character of a file
        name cannot be a digit, so prepend F_ to
        the file name                   */
    
    if (isdigit (inName[0]) != 0)
        {
        strcpy ((char *)outName, "F_");
        strcat ((char *)outName, (char *)inName);
        }
    else
        {
        strcpy ((char *)outName, (char *)inName);
        }
#endif      /* PRIMOS */
#endif      /* UNIX */
    ts_convert_char ((CHAR *)outName, ' ', '_');    /*  remove blanks    */
    
    return SUCCESS;
    }                               /* end of HO_GET_FILENAME */
/* END:     */
