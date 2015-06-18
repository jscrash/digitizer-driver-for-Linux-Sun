/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_BLT_BUF.C*/
/* *2    17-AUG-1990 22:33:31 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:41:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_BLT_BUF.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    A module of routines of Cross Section. This module includes
    all routines which perform function on database. 

    This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS.

    Builds a buffer of character string, if the allocated buffer is big
    enough to store the append string, caternates it and returns.
    Otherwise, allocates a new buffer and moves the content in the old
    buffer to the new allocated buffer then append the new string then
    free the old buffer.
        
    If the length of buffer is longer than maximum, stores the old buffer
    to the database first, then returns the new buffer, which only contains
    new added string.

Prototype:
    publicdef INT xsz_build_buffer(CHAR *layout, CHAR *verb, CHAR *oldBuf, INT oldBufLen, 
        INT availLen, INT incLen, CHAR *addStr, INT addStrLen, CHAR **newBuf, 
        INT *newBufLen, INT *totalAlloc, INT maxBufLen, INT writeOut);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) layout      -(CHAR *) The name of layout.
    (I) verb        -(CHAR *) The name of verb.
    (I) oldBuf      -(CHAR *) The old used buffer.
    (I) oldBufLen   -(INT) The totally used buffer length.
    (I) availLen    -(INT) The totally available space in the old buffer.
    (I) incLen      -(INT) The new increase space.
    (I) addStr      -(CHAR *) The added string.
    (I) addStrLen   -(INT ) The length of added string.
    (I) maxBufLen   -(INT) The maximum length of buffer could be.
    (I) writeOut    -(INT) The flag of writing out immediately.
                        TRUE -- write out the buffer immediately, even the buffer
                                is short than XS_MAX_BUF_LENGTH.
                        FALSE -- not write out yet until the buffer length is 
                                longer then XS_MAX_BUF_LENGTH.
    (O) newBuf      -(CHAR **) The new allocated returned buffer.
    (O) newBufLen   -(INT *) The totally occupied buffer length.
    (O) totalAlloc  -(INT *) The totally allocated space in the new buffer.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_build_buffer (CHAR *layout, CHAR *verb, CHAR *oldBuf, INT oldBufLen,
                                INT availLen, INT incLen, CHAR *addStr, INT addStrLen,
                                CHAR **newBuf, INT *newBufLen, INT *totalAlloc,
                                INT maxBufLen, INT writeOut)
#else
publicdef INT xsz_build_buffer (layout, verb, oldBuf, oldBufLen, availLen, incLen,
                                addStr, addStrLen, newBuf, newBufLen, totalAlloc,
                                maxBufLen, writeOut)
CHAR *layout;
CHAR *verb;
CHAR **newBuf;
CHAR *oldBuf;
CHAR *addStr;
INT addStrLen;
INT *newBufLen;
INT oldBufLen;
INT availLen;
INT incLen;
INT *totalAlloc;
INT maxBufLen;
INT writeOut;
#endif
    {
    INT status = SUCCESS;           /*   result status.                          */
    INT free_old = FALSE;
    INT allocate_bytes = 0;
    CHAR *newPtr, oldPtr;
    static CHAR *carma[1] = 
        {
        ", "
        };
    /* ************************************************************************* */
    /* if the available buffer len is not*/
    /* big enought to store all the data */
    /* reallocate the space and move the */
    /* old data to the new buffer first. */
    
    *newBufLen = (oldBufLen > 0) ? oldBufLen + addStrLen + 2 : addStrLen;
    
    if (*newBufLen >= availLen)
        {
        /* allocates the new buffer.        */
        
        allocate_bytes = availLen + incLen;
        while (allocate_bytes <= *newBufLen)
            allocate_bytes += incLen;
        
        /* if the requested length is greater*/
        /* than the maximum buffer length    */
        /* write out the buffer to the       */
        /* database, frees the oldBuf, then  */
        /* creates a new buffer, whose length*/
        /* is the length of added string.    */
        
        if (allocate_bytes >= maxBufLen)
            {
            status = xsz_add_record (layout, verb, oldBuf);
            tc_free (oldBuf);
            *totalAlloc = allocate_bytes = addStrLen + 1;
            oldBufLen = 0;
            }
        else
            {
            allocate_bytes += 1;
            *totalAlloc = allocate_bytes;
            }
        *newBuf = newPtr = (CHAR *)tc_zalloc (allocate_bytes);
        
        /* moves the old data to new buffer.*/
        
        if (oldBufLen IS_NOT_EQUAL_TO 0)
            {
            
            /* copy the seperate mark.          */
            
            strcpy (newPtr, oldBuf);
            strcat (newPtr, carma[0]);
            /* frees the old buffer.            */
            tc_free (oldBuf);
            /* append the new data.         */
            
            strcat (newPtr, addStr);
            
            }
        else
            strcpy (newPtr, addStr);
        
        /* if it should be written out      */
        /* immediately, ...                 */
        if (writeOut)
            {
            status = xsz_add_record (layout, verb, newPtr);
            tc_free (newPtr);
            *newBufLen = *totalAlloc = 0;
            }
        }
    else
        {
        
        /* if the space is big enough to   */
        /* store the new added string...   */
        
        /* set the new buffer is equals to */
        /* old buffer and increase the used*/
        /* space, then returns.            */
        
        *newBuf = oldBuf;
        if (oldBufLen IS_NOT_EQUAL_TO 0)
            {
            strcat (oldBuf, carma[0]);
            }
        strcat (oldBuf, addStr);
        *totalAlloc = availLen;
        /* if it should be written out      */
        /* immediately, ...                 */
        if (writeOut)
            {
            status = xsz_add_record (layout, verb, oldBuf);
            tc_free (oldBuf);
            oldBuf = (CHAR *)NULL;
            *totalAlloc = *newBufLen = 0;
            }
        }
    return status;
    
    }
/* END:     */
