/* DEC/CMS REPLACEMENT HISTORY, Element HO_CTOFSTR.C*/
/* *3    17-AUG-1990 21:50:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:20 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:57:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_CTOFSTR.C*/
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

#include "esi_gl_defs.h"

/* Function Description -----------------------------------------------------
Description:
    Convert a C string to a Fortran string. 

Prototype:
    See below.
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    See below.
    
Return Value/Status:
    See below.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#ifdef primos
INT ctofstr (fstr, cstr, longlen)
CHAR *fstr;                         /* space for fortran string */
CHAR *cstr;                         /* C string to convert      */
INT longlen;                        /* length of fortran data space */
    {
    INT len, tlen;
    
    if (am_check_pointer (fstr, "w") != SUCCESS)
        return FAIL;
    len = longlen & 65535;          /* discard high 2 bytes */
    tlen = strlen (cstr);
    hoblockmove (cstr, fstr, len);
    if (tlen <= len)
        {                           /* blankpad */
        while (len > tlen)
            *(fstr + tlen++) = ' ';
        }
    return SUCCESS;
    }


#endif

#if UNIX 
publicdef ctofstr (fstr, cstr, len)
CHAR *fstr;
CHAR *cstr;
INT len;
    {
    INT tlen = strlen (cstr);
    
    /*    if (am_check_pointer(fstr,"w") != SUCCESS)
            return FAIL;
    */
    hoblockmove (cstr, fstr, len);
    if (tlen <= len)
        {                           /* blankpad */
        while (len > tlen)
            *(fstr + tlen++) = ' ';
        }
    return SUCCESS;
    }


#endif
/* END:     */
