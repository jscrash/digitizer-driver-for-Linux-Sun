/* DEC/CMS REPLACEMENT HISTORY, Element HOZ_GETJPI.C*/
/* *3    17-AUG-1990 21:50:10 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-AUG-1990 13:30:27 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:56:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HOZ_GETJPI.C*/
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

#ifdef vms

#include "libdef.h"
#include "descrip.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    HOZ_GETJPI returns jpi infomation, caller should include jpidef and
    reference the option symbolically. This is a VAX/VMS
    dependent function.

Prototype:
    publicdef INT hoz_getjpi (INT option, CHAR *buffer, INT size, INT *returned_size);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) option          -(INT) GETJPI option code - VMS specific.
    (O) buffer          -(CHAR *) Address of output buffer tp receive string.
    (I) size            -(INT) Size of output buffer (remember the null byte).
    (O) returned_size   -(INT *) Ptr to integer which will contain actual length
                            returned by GETJPI.

Return Value/Status:
    Returns the number of characters written to buffer.
    Returns a negative number if the call failed.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoz_getjpi (INT option, CHAR *buffer, INT size, INT *returned_size) 
#else
publicdef INT hoz_getjpi (option, buffer, size, returned_size)
INT option;
CHAR *buffer;
INT size;
INT *returned_size;
#endif
    {                               /* start of HOZ_GETJPI */
    INT status = SUCCESS;

#if UNIX
    return FAIL;
#else

#ifdef primos
    return FAIL;
#endif

#ifdef vms
    SHORT rlen;
    struct dsc$descriptor desc;
    
    desc.dsc$w_length = size;
    desc.dsc$b_class = DSC$K_CLASS_S;
    desc.dsc$b_dtype = DSC$K_DTYPE_T;
    desc.dsc$a_pointer = buffer;
    
    status = VAX_ROUTINE (LIB$GETJPI (&option, 0, 0, 0, &desc, &rlen));
    
    /* null terminate the string, if possible      */
    if (rlen < size)
        {
        buffer[rlen] = 0;
        }
    /* report the returned length, if caller       */
    /* provided a non-zero address                 */
    if (returned_size != (INT *)0)
        {
        *returned_size = rlen;
        }
    return status;
#endif      /* VMS */
#endif      /* UNIX */
    }                               /* end of HOZ_GETJPI */
/* END:     */
