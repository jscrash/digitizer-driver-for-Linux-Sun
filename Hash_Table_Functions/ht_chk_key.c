/* DEC/CMS REPLACEMENT HISTORY, Element HT_CHK_KEY.C*/
/* *4    25-OCT-1990 12:35:41 GILLESPIE "(SPR 1) Add esi_c_lib.h"*/
/* *3    17-AUG-1990 21:54:50 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    21-NOV-1989 09:59:14 CONROY "(SPR 5059) Add new key type HT_BYTE_KEY"*/
/* *1    19-JUN-1989 13:00:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_CHK_KEY.C*/
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
#include "esi_c_lib.h"
#include "esi_ht_err.h"

#if USE_PROTOTYPES
static BOOL htz_byte_key_found (VOIDPTR key, INT key_length,
                                    HASH_BUCKET *this_bucket, INT length, INT i);
#else
static BOOL htz_byte_key_found ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Get hash table key.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ht_check_for_key(HASH_TABLE_P hash_table, VOIDPTR key, INT address, 
        HASH_BUCKET  **found_bucket, HASH_BUCKET  **prev_bucket, UINT *keyno);

Private_Functions:
    static BOOL htz_byte_key_found( VOIDPTR key, INT key_length,
            HASH_BUCKET *this_bucket, INT length, INT i);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Checks a hash table for the existence of a key

Prototype:
    publicdef INT ht_check_for_key(HASH_TABLE_P hash_table, VOIDPTR key, INT address, 
        HASH_BUCKET  **found_bucket, HASH_BUCKET  **prev_bucket, UINT *keyno);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table      -(HASH_TABLE_P) A pointer to the hash table.
    (I) key             -(VOIDPTR) A pointer to the key area.
    (I) address         -(INT) The address of the bucket which will
                            contain this key, if it exists. 
    (O) found_bucket    -(HASH_BUCKET **) A pointer to the bucket containing the key.
    (O) prev_bucket     -(HASH_BUCKET **) A pointer to the bucket preceeding this_bucket.
    (O) keyno           -(UINT *) Location (ie sequence no) of the key in
                            this_bucket.

Return Value/Status:
    HT_TABLE_NOT_INIT
    HT_TABLE_NOT_FOUND
    HT_KEY_NOT_FOUND
    SUCCESS              (key found)

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_check_for_key (HASH_TABLE_P hash_table, VOIDPTR key, INT address,
                                HASH_BUCKET **found_bucket, HASH_BUCKET **prev_bucket,
                                UINT *keyno)
#else
publicdef INT ht_check_for_key (hash_table, key, address, found_bucket, prev_bucket,
                                keyno)
HASH_TABLE_P hash_table;
VOIDPTR key;
INT address;
HASH_BUCKET **found_bucket;
HASH_BUCKET **prev_bucket;
UINT *keyno;
#endif
    {
    HASH_BUCKET *this_bucket;
    HASH_BUCKET *old_bucket;
    BYTE *curr_key;
    size_t length;
    BOOL found;
    UINT i;
    unsigned *key_ptr;
    
    /********************************************************************/
    
    key_ptr = (unsigned *)key;
    
    if (hash_table == (HASH_TABLE_P)NULL)
        {
        return HT_TABLE_NOT_INIT;
        }
    if (*(INT *)hash_table != HT_CHECK_CODE)
        {
        return HT_TABLE_NOT_FOUND;
        }
    length = hash_table->key_length + hash_table->data_length;
    
    /*  Search for the key  */
    
    this_bucket = &hash_table->bucket_array[address];
    *found_bucket = (HASH_BUCKET *)NULL;
    *prev_bucket = (HASH_BUCKET *)NULL;
    old_bucket = (HASH_BUCKET *)NULL;
    
    switch (hash_table->key_type)
        {
    case HT_INT_KEY:
        found = FALSE;
        while ((this_bucket != (HASH_BUCKET *)NULL) && !found)
            {
            curr_key = this_bucket->bucket;
            for (i = 0; i < this_bucket->no_of_keys; i++)
                {
                if (*(INT *)key == *(INT *)curr_key)
                    {
                    found = TRUE;
                    *found_bucket = this_bucket;
                    *prev_bucket = old_bucket;
                    *keyno = i;
                    break;
                    }
                curr_key += length;
                }
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            }
        break;
        
    case HT_FLOAT_KEY:
        found = FALSE;
        while ((this_bucket != (HASH_BUCKET *)NULL) && !found)
            {
            curr_key = this_bucket->bucket;
            for (i = 0; i < this_bucket->no_of_keys; i++)
                {
                if (*(FLOAT *)key == *(FLOAT *)curr_key)
                    {
                    found = TRUE;
                    *found_bucket = this_bucket;
                    *prev_bucket = old_bucket;
                    *keyno = i;
                    break;
                    }
                curr_key += length;
                }
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            }
        break;
        
    case HT_DOUBLE_KEY:
        found = FALSE;
        while ((this_bucket != (HASH_BUCKET *)NULL) && !found)
            {
            curr_key = this_bucket->bucket;
            for (i = 0; i < this_bucket->no_of_keys; i++)
                {
                if (*(DOUBLE *)key == *(DOUBLE *)curr_key)
                    {
                    found = TRUE;
                    *found_bucket = this_bucket;
                    *prev_bucket = old_bucket;
                    *keyno = i;
                    break;
                    }
                curr_key += length;
                }
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            }
        break;
        
    case HT_PTR_KEY:
        found = FALSE;
        while ((this_bucket != (HASH_BUCKET *)NULL) && !found)
            {
            curr_key = this_bucket->bucket;
            for (i = 0; i < this_bucket->no_of_keys; i++)
                {
                if (*(BYTE_P *)key == *(BYTE_P *)curr_key)
                    {
                    found = TRUE;
                    *found_bucket = this_bucket;
                    *prev_bucket = old_bucket;
                    *keyno = i;
                    break;
                    }
                curr_key += length;
                }
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            }
        break;
        
    case HT_CHAR_KEY:
        found = FALSE;
        while ((this_bucket != (HASH_BUCKET *)NULL) && !found)
            {
            curr_key = this_bucket->bucket;
            for (i = 0; i < this_bucket->no_of_keys; i++)
                {
                if (strcmp ((char *)key,
                            (char *)(this_bucket->bucket + (i * length))) EQUALS 0)
                    {
                    found = TRUE;
                    *found_bucket = this_bucket;
                    *prev_bucket = old_bucket;
                    *keyno = i;
                    break;
                    }
                curr_key += length;
                }
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            }
        break;
        
    case HT_BYTE_KEY:
        found = FALSE;
        while ((this_bucket != (HASH_BUCKET *)NULL) && !found)
            {
            curr_key = this_bucket->bucket;
            for (i = 0; i < this_bucket->no_of_keys; i++)
                {
                if (htz_byte_key_found (key, hash_table->key_length, this_bucket,
                                        length, i))
                    {
                    found = TRUE;
                    *found_bucket = this_bucket;
                    *prev_bucket = old_bucket;
                    *keyno = i;
                    break;
                    }
                curr_key += length;
                }
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            }
        break;
        
    default:
        return HT_INVALID_KEY_TYPE;
        
        }
    /*  end switch  */
    
    if (found)
        {
        return SUCCESS;
        }
    else
        {
        return HT_KEY_NOT_FOUND;
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static BOOL htz_byte_key_found(VOIDPTR  key,INT key_length,HASH_BUCKET *this_bucket,
        INT length,INT  i);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key             -(VOIDPTR)
    key_length      -(INT)
    this_bucket     -(HASH_BUCKET *)
    length          -(INT)
    i               -(INT)

Return Value/Status:
    Function returns TRUE/FALSE.
    
Scope:
    PRIVATE to <ht_chk_key.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static BOOL htz_byte_key_found (VOIDPTR key, INT key_length,
                                    HASH_BUCKET *this_bucket, INT length, INT i)
#else
static BOOL htz_byte_key_found (key, key_length, this_bucket, length, i)
VOIDPTR key;
INT key_length;
HASH_BUCKET *this_bucket;
INT length;
INT i;
#endif
    {
    INT j;
    
    /*********************************************************************/
    
    for (j = 0; j < key_length; j++)
        {
        if (((BYTE *)key)[j] != this_bucket->bucket[i * length + j])
            {
            return FALSE;
            }
        }
    return TRUE;
    }
/* END:     */
