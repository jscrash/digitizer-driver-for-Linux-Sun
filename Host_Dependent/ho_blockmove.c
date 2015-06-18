/* DEC/CMS REPLACEMENT HISTORY, Element HO_BLOCKMOVE.C*/
/* *4    17-AUG-1990 21:50:24 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    13-DEC-1989 11:27:04 GORDON "(SPR 0) For Unix, handle overlapping moves"*/
/* *2     1-NOV-1989 10:12:27 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *1    19-JUN-1989 12:56:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_BLOCKMOVE.C*/
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

#if UNIX

#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    Fast in-memory data mover. 

Prototype:
    publicdef INT hoblockmove(VOIDPTR from,VOIDPTR to,size_t num_bytes);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) from        -(VOIDPTR) Pointer to move memory from.
    (I) to          -(VOIDPTR) Pointer to move memory to.
    (I) num_bytes   -(size_t) Size of memory to move.

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS - If error occurs while host is VMS.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    The target region must point to enough memory to hold the number
    of bytes to be moved; otherwise, information may be overwritten!!

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoblockmove (VOIDPTR from, VOIDPTR to, size_t num_bytes) 
#else
publicdef INT hoblockmove (from, to, num_bytes)
VOIDPTR from;
VOIDPTR to;
size_t num_bytes;
#endif
    {
    INT ii;
    CHAR *src, *dest;
    
    /* memcpy moves starting from the beginning of the regions,
       and doesn't check for overlapping regions. Thus, if the regions
       overlap and the destination is in higher memory than the
       source, you get unintended results.  Here, we check for that
       possibliity and if necessary, do the copy ourselves, beginning
       at the end of the regions. */
    
    if ((CHAR *)to > (CHAR *)from && (CHAR *)to < (CHAR *)from + num_bytes)
        {
        src = (CHAR *)from + num_bytes - 1;
        dest = (CHAR *)to + num_bytes - 1;
        for (ii = 0; ii < num_bytes; ++ii)
            *dest-- = *src--;
        }
    else
        {
        memcpy (to, from, num_bytes);
        }
    return SUCCESS;
    }


#else

/******************************************************************************/

#ifdef primos
publicdef INT hoblockmove (from, to, num_bytes)
VOIDPTR from;
VOIDPTR to;
size_t num_bytes;
    {
    VOID homvit ();                 /* assembly program on PRIME (PMA) */
    
    /* NOTE: We used to do a lot of checking here that we handle segment
       boundaries properly, but we now realize that the only time it would
       try to copy over a segment boundary is in a memory-overwrite bug which
       which also happens to be near the high end of a segment. This is because
       Prime C won't let you allocate a data object greater than a segment size,
       either through a call to malloc, or static allocation.  Therefore, no
       one should ever legally be copying over a segment boundary.  -GS 2/15/89 */
    
    homvit (from, to, (LONG)num_bytes);
    
    return SUCCESS;
    }


#endif

/******************************************************************************/

#ifdef vms

#include "ssdef.h"
#include "esi_ho_err.h"

/* LIB$MOVEC3 moves a maximum of 65535 bytes in one operation.  If more bytes
   need to be moved, then we have to move the blocks in segments
 */
#define MAX_BYTES 65535
publicdef INT hoblockmove (from, to, num_bytes)
VOIDPTR from;
VOIDPTR to;
size_t num_bytes;
    {
    UINT long_nbytes = num_bytes;
    USHORT nbytes;
    INT status;
    
    /* move simple things simply */
    if (num_bytes < MAX_BYTES)
        {
        nbytes = num_bytes;
        status = LIB$MOVC3 (&nbytes, from, to);
        }
    else
        {
        BYTE *fromPtr = (BYTE *)from, *toPtr = (BYTE *)to;
        INT i;
        
        for (i = 0; i < num_bytes;
                                        i += MAX_BYTES, fromPtr += MAX_BYTES,
                                        toPtr += MAX_BYTES)
            {
            long_nbytes = num_bytes - i;
            if (long_nbytes > MAX_BYTES)
                {
                long_nbytes = MAX_BYTES;
                }
            nbytes = long_nbytes;
            status = LIB$MOVC3 (&nbytes, fromPtr, toPtr);
            }
        }
    return(status == SS$_NORMAL) ? SUCCESS : HO_ERR_VMS;
    }


#endif  /* VMS */
#endif  /* UNIX */
/* END:     */
