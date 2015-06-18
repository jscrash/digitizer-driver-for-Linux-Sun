/* DEC/CMS REPLACEMENT HISTORY, Element HO_FTOCSTR.C*/
/* *3    17-AUG-1990 21:51:37 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:49 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:58:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_FTOCSTR.C*/
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
#include "esi_glue.h"

/* Function Description -----------------------------------------------------
Description:
    Function converts a Fortran string (array of characters) to a C string.

Prototype:
    See below.
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    See below.
    
Return Value/Status:
    Function returns 0 if ok or -1 if string too long.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES

#else

#endif

#ifdef primos
INT ftocstr (fstr, cstr, longlen)
CHAR *fstr;                         /* Fortran string to convert */
CHAR *cstr;                         /* space for C string        */
INT longlen;                        /* length of fortran string incl. blank pad */
    {
    INT len, status;
    CHAR *cp;
    
    status = 0;
    len = longlen & 65535;          /* discard high 2 bytes */
    if (len > MAXLEN)
        {
        status = -1;                /* string too long */
        len = MAXLEN;               /* process only the first MAXLEN chars */
        }
    hoblockmove (fstr, cstr, len);
    *(cstr + len) = '\0';           /* null terminate C string */
    
    /* remove trailing blanks  */
    for (cp = cstr + len - 1; strlen (cstr) && (*cp == ' '); cp--)
        *cp = '\0';
    
    return status;
    }


#endif

#if UNIX 
INT ftocstr (fstr, cstr, len)
CHAR *fstr;                         /* Fortran string to convert */
CHAR *cstr;                         /* space for C string        */
INT len;                            /* length of fortran string incl. blank pad */
    {
    INT status = SUCCESS;
    CHAR *cp;
    
    if (len > MAXLEN)
        {
        status = FAIL;              /* string too long */
        len = MAXLEN;               /* process only the first MAXLEN chars */
        }
    hoblockmove (fstr, cstr, len);
    *(cstr + len) = '\0';           /* null terminate C string */
    
    /* remove trailing blanks  */
    for (cp = cstr + len - 1; strlen (cstr) && (*cp == ' '); cp--)
        *cp = '\0';
    
    return status;
    }


#endif
/* END:     */
