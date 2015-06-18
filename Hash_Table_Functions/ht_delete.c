/* DEC/CMS REPLACEMENT HISTORY, Element HT_DELETE.C*/
/* *2    17-AUG-1990 21:55:11 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_DELETE.C*/
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
#include "esi_tc.h"
#include "esi_ht_err.h"

#if HT_TRACE

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Delete an entry from the hash table.

Prototype:
    publicdef INT ht_delete(HASH_TABLE_P hash_table, VOIDPTR    key);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE_P) Pointer to the hash table.
    (I) key         -(VOIDPTR) Pointer to the key to be deleted.

Return Value/Status:
    SUCCESS - Successful completion.
    HT_TABLE_NOT_INIT
    HT_TABLE_NOT_FOUND
    HT_KEY_NOT_FOUND

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_delete (HASH_TABLE_P hash_table, VOIDPTR key)
#else
publicdef INT ht_delete (hash_table, key)
HASH_TABLE_P hash_table;
VOIDPTR key;
#endif
    {
    INT status;
    INT address;
    HASH_BUCKET *this_bucket;
    HASH_BUCKET *old_bucket;
    UINT length;
    UINT i, j;
    

#if HT_TRACE
    /* ht TRACE */
    extern FILE *ht_fp;
    unsigned *key_ptr;
    key_ptr = (unsigned *)key;
#endif
    
    /*  Check that we have a valid hash table */
    
    if (hash_table == (HASH_TABLE_P)NULL)
        {
        return HT_TABLE_NOT_INIT;
        }
    if (*(INT *)hash_table != HT_CHECK_CODE)
        return HT_TABLE_NOT_FOUND;
    
    length = hash_table->key_length + hash_table->data_length;
    
    /*  compute the address  */
    
    ht_hash_code (key, hash_table->key_type, hash_table->key_length,
                  hash_table->actual_no_of_buckets, &address);
    
    /*  Search for the key  */
    
    status = ht_check_for_key (hash_table, key, address, &this_bucket, &old_bucket,
                               &i);
    

#if HT_TRACE
    fprintf (ht_fp, "d %x %x %d %d\n", hash_table, *key_ptr, address, status);
    fflush (ht_fp);
#endif
    if (status == SUCCESS)
        {
        if (this_bucket->no_of_keys == 1)
            {
            if (old_bucket != (HASH_BUCKET *)NULL)
                {
                tc_free (this_bucket->bucket);
                tc_free (this_bucket);
                old_bucket->next_bucket = (HASH_BUCKET *)NULL;
                return SUCCESS;
                }
            else
                {
                this_bucket->no_of_keys = 0;
                tc_zeroes (this_bucket->bucket, length);
                return SUCCESS;
                }
            }
        else
            {
            for (j = i * length; j < (this_bucket->no_of_keys - 1) * length; j++)
                *(this_bucket->bucket + j) = *(this_bucket->bucket + (j + length));
            old_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            if (this_bucket == (HASH_BUCKET *)NULL)
                {
                old_bucket->no_of_keys--;
                return SUCCESS;
                }
            /*            Move all the keys up  */
            
            while (this_bucket != (HASH_BUCKET *)NULL)
                {
                
                /*            Copy the first field to the last field of the previous bucket  */
                
                for (j = 0; j < length; j++)
                    old_bucket->bucket[(old_bucket->no_of_keys - 1) *length +
                                                                                   j] =
                                                                                   this_bucket
                                                                                   ->bucket
                                                                                   [j];
                
                /*             Move the rest of the fields    */
                
                for (j = 0; j < (this_bucket->no_of_keys - 1) * length; j++)
                    *(this_bucket->bucket + j) = *(this_bucket->bucket + (j + length));
                
                if (this_bucket->no_of_keys - 1 == 0)
                    {
                    tc_free (this_bucket->bucket);
                    tc_free (this_bucket);
                    old_bucket->next_bucket = (HASH_BUCKET *)NULL;
                    return SUCCESS;
                    }
                else
                    {
                    old_bucket = this_bucket;
                    this_bucket = this_bucket->next_bucket;
                    }
                }
            old_bucket->no_of_keys--;
            return SUCCESS;
            }
        }
    return status;
    }
/* END:     */
