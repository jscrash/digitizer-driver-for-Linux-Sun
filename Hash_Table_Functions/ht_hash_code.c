/* DEC/CMS REPLACEMENT HISTORY, Element HT_HASH_CODE.C*/
/* *4    17-AUG-1990 21:55:34 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    21-NOV-1989 10:01:36 CONROY "(SPR 5059) Add new key type HT_BYTE_KEY"*/
/* *2    25-AUG-1989 17:19:01 CONROY "(SPR 9999) check length on char keys"*/
/* *1    19-JUN-1989 13:00:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_HASH_CODE.C*/
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

#include "esi_ht.h"

/* Function Description -----------------------------------------------------
Description:
    Computes a hash code for a given key value.

Prototype:
    publicdef INT ht_hash_code(VOIDPTR key, INT key_type, size_t key_length, UINT table_length, 
        INT *hash_code);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) key             -(VOIDPTR) The key to be hashed.
    (I) key_type        -(INT) The key type.
    (I) key_length      -(size_t) The length of the key in bytes.   
    (I) table_length    -(UINT) The allowable address range.
    (O) hash_code       -(INT *) The computed hash code
                            (0 >= hash_code < table_length).

Return Value/Status:
    SUCCESS - Successful completion.
    HT_INVALID_KEY_TYPE

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_hash_code (VOIDPTR key, INT key_type, size_t key_length,
                            UINT table_length, INT *hash_code)
#else
publicdef INT ht_hash_code (key, key_type, key_length, table_length, hash_code)
VOIDPTR key;
INT key_type;
size_t key_length;
UINT table_length;
INT *hash_code;
#endif
    {
    INT i = 0;
    INT count = key_length;
    BYTE *p = (BYTE *)key;
    
    /*   Process character keys.  Note that only
         the characters actually used are processed
         (previous implementations required blank
         or zero padding).              */
    
    if (key_type EQUALS HT_CHAR_KEY)
        {
        while (*p)
            {
            i = (i << 1) ^ *p++;
            }
        }
    else if (key_type EQUALS HT_BYTE_KEY)
        {
        while (count--)
            {
            i = (i << 1) ^ *p++;
            }
        }
    else
        {
        /*  Process non-character keys.  The maximum
            length of such things is assumed to be the
            sizeof an INT (or so), and since we have
            about 6 bytes MAX to generate a unique
            number, we accelerate the left shift over
            character strings.  Otherwise, the maximum
            number of bits would be about 11 (original
            8 plus 3 shifts of 1 bit each).  This way,
            the maximum number of bits is 20 (original
            8 plus 3 shifts of 4 bits each) */
        
        while (count--)
            {
            i = (i << 4) ^ *p++;
            }
        }
    if (i < 0)
        {
        i = -i;
        }
    return *hash_code = i % table_length;
    }
/* END:     */
