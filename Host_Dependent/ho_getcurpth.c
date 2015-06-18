/* DEC/CMS REPLACEMENT HISTORY, Element HO_GETCURPTH.C*/
/* *3    17-AUG-1990 21:51:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:52 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:58:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GETCURPTH.C*/
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

#ifdef primos

#include "esi_ho_files.h"

#endif

#if UNIX
extern CHAR *getwd ();
#endif

/* Function Description -----------------------------------------------------
Description:
    Returns the pathname (directory) of the current attach point.                                            *

Prototype:
    publicdef CHAR *ho_get_current_path (CHAR *buff);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) buff        -(CHAR *) Pathname.

Return Value/Status:
    Function returns pointer to input parameter.                                        *

Scope:
    PUBLIC
    
Limitations/Assumptions:
    NOTE: I do not know what the maximum length is that this routine  
    can return.  It will copy into the provided pointer the           
    name of the current directory.  Programmers must assure that     
    there is sufficient room.  Suggest defining buff as FILENAME.     

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ho_get_current_path (CHAR *buff) 
#else
publicdef CHAR *ho_get_current_path (buff)
CHAR *buff;
#endif
    {
#ifdef primos
    SHORT key, unit, buflen, pathlen, code;
    INT i;
    PATHNAME string;
    fortran gpath$();
    
    key = 2;
    unit = 0;
    buflen = sizeof string;
    pathlen = 0;
    code = 0;
    
    gpath$(key, unit, string, buflen, pathlen, code);
    i = pathlen;
    
    strncpy (buff, string, i);
    buff[i] = '\0';

#endif

#ifdef vms
    strcpy (buff, getenv ("PATH"));
#endif

#if UNIX
    if (getwd (buff) == (CHAR *)0)
        buff = (CHAR *)0;
#endif
    return(buff);
    }
/* END:     */
