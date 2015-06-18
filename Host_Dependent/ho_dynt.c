/* DEC/CMS REPLACEMENT HISTORY, Element HO_DYNT.C*/
/* *2    17-AUG-1990 21:51:05 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:57:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_DYNT.C*/
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

#include "esi_ts.h"
typedef int (*ProcPtr)();
typedef struct vchar_
    {
    short len;
    char arr[32];
    } VChar;

/* Function Description -----------------------------------------------------
Description:
    Given a function's name, return a "dynamic link" to it. 
    Return the NULL pointer, if it isn't available.        

Prototype:
    ProcPtr ho_dynt (CHAR *name);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)

Return Value/Status:
    Function returns dynamic link.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
ProcPtr ho_dynt (CHAR *name) 
#else
ProcPtr ho_dynt (name)
CHAR *name;
#endif
    {

#ifndef primos
    return(ProcPtr)0;               /* no work to do on other systems. */
#else
    ProcPtr realproc;
    VChar vname;
    short status;
    
    union 
        {
        ProcPtr pproc;
        short *pvchar;
        short pshort;
        } u;
    
    fortran ckdyn$();
    
    /* return a zero pointer if there is any problem */
    realproc = (ProcPtr)0;
    if (name == (char *)0)
        return realproc;            /* no name pointer given */
    if (strlen (name) == 0)
        return realproc;            /* no name given         */
    
    /* clean and copy the routine name to a VCHAR */
    strcpy (vname.arr, ts_snowhite (name));
    vname.len = strlen (name);
    
    /* make sure name is in upper case */
    ts_sto_upper (vname.arr);
    
    /* see if there is a callable routine by the given name */
    status = 0;
    ckdyn$(vname.len, status);
    if (status != 0)
        return realproc;            /* function NOT available */
    
    /* create the dynamic link pointer */
    u.pvchar = &vname.len;          /* copy the VCHAR pointer         */
    u.pshort |= 0x8000;             /* turn on fault bit              */
    u.pshort &= 0x8FFF;             /* turn off ring bits and EPB bit */
    
    /* snap the link */
    realproc = &(*u.pproc);
    
    return realproc;

#endif
    }
/* END:     */
