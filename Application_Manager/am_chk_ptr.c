/* DEC/CMS REPLACEMENT HISTORY, Element AM_CHK_PTR.C*/
/* *3    14-AUG-1990 23:55:57 GILLESPIE "(SPR 5644) Fix last checkin"*/
/* *2    14-AUG-1990 11:39:40 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 11:59:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_CHK_PTR.C*/
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

#include "esi_am.h"

#ifdef primos

#include "setjmp.h"
#include "esi_ctype.h"
fortran mkon$p ();
static VOID pointer_onunit ();
static SHORT seventeen = 17;
static jmp_buf env;

#endif
static INT status;

/* Function Description -----------------------------------------------------
Description:
    This function checks whether or not the caller has access to the
    specified pointer.
    
Prototype:
    publicdef INT am_check_pointer(VOIDPTR pointer, CHAR *access);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pointer     -(VOIDPTR) pointer to check access rights to.
    (I) access      -(CHAR *) type of access requested.

Return Value/Status:
    SUCCESS - Access rights to specified pointer;
    ACCESS_VIOLATION - If no access rights to specified pointer.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_check_pointer (VOIDPTR pointer, CHAR *access) 
#else
publicdef INT am_check_pointer (pointer, access)
VOIDPTR pointer;
CHAR *access;
#endif
    {
    BYTE value;
    
    /* ********************************************************************** */
    
    status = SUCCESS;

#ifdef primos
    mkon$p ("ACCESS_VIOLATION$", &seventeen, pointer_onunit);
    setjmp (env);
    if (status EQUALS SUCCESS)
        {
        value = *pointer;
        if (status EQUALS SUCCESS)
            if ('W'EQUALS _toupper (*access))
                {
                setjmp (env);
                if (status EQUALS SUCCESS)
                    *pointer = value;
                }
        }
#endif
    return status;
    }


#ifdef primos

/* ********************************************************************** */
static VOID pointer_onunit (p)  /* ********************************************************************** */

INT *p;

/* ********************************************************************** */
    {
    status = FAIL;
    longjmp (env, 0);
    }


}
#endif
/* END:     */
