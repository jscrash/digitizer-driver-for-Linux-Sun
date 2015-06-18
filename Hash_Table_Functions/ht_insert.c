/* DEC/CMS REPLACEMENT HISTORY, Element HT_INSERT.C*/
/* *2    17-AUG-1990 21:55:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:01:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_INSERT.C*/
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
#include "esi_ho.h"
#include "esi_ht_err.h"

#if HT_TRACE

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Insert a key/data pair into a hash table.

Prototype:
    publicdef INT ht_insert(HASH_TABLE_P hash_table, VOIDPTR key, VOIDPTR data);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE_P) A pointer to the hash table.
    (I) key         -(VOIDPTR) A pointer to the key area.
    (I) data        -(VOIDPTR) A pointer to the data area (may be null).

Return Value/Status:
    SUCCESS - Successful completion.
    HT_TABLE_NOT_INIT
    HT_DUPLICATE_KEY

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_insert (HASH_TABLE_P hash_table, VOIDPTR key, VOIDPTR data)
#else
publicdef INT ht_insert (hash_table, key, data)
HASH_TABLE_P hash_table;
VOIDPTR key;
VOIDPTR data;
#endif
    {
    INT status;
    INT address;
    HASH_BUCKET *this_bucket;
    HASH_BUCKET *prev_bucket;
    INT length;
    UINT i;
    UINT curr_bucket_no = 0;
    BOOL need_to_allocate = FALSE;
    

#if HT_TRACE
    extern FILE *ht_fp;             /*  hash table TRACE file */
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
    
    /*  Compute the bucket address  */
    
    ht_hash_code (key, hash_table->key_type, hash_table->key_length,
                  hash_table->actual_no_of_buckets, &address);
    
    length = hash_table->key_length + hash_table->data_length;
    
    /*  Search for a duplicate key  */
    
    status = ht_check_for_key (hash_table, key, address, &this_bucket, &prev_bucket,
                               &i);
    if (status == SUCCESS)
        status = HT_DUPLICATE_KEY;
    
    if (status != HT_KEY_NOT_FOUND)
        {

#if HT_TRACE   /* ht TRACE */
        fprintf (ht_fp, "a %x %x %d 0 0 0 %d\n", hash_table, *key_ptr, address,
                 status);
        fflush (ht_fp);
#endif
        return status;
        }
    /*  Move the key into the table  */
    
    this_bucket = &hash_table->bucket_array[address];
    while (this_bucket->next_bucket != (HASH_BUCKET *)NULL)
        {
        curr_bucket_no++;
        this_bucket = this_bucket->next_bucket;
        }
    /*  Check to see if the bucket is full  */
    
    if (this_bucket->no_of_keys == hash_table->bucket_size)
        {
        this_bucket->next_bucket = (HASH_BUCKET *)htz_zalloc (sizeof(HASH_BUCKET),
                                                              hash_table->flag);
        this_bucket = this_bucket->next_bucket;
        this_bucket->bucket = (BYTE *)htz_zalloc (hash_table->bucket_size * length,
                                                  hash_table->flag);
        curr_bucket_no++;
        need_to_allocate = TRUE;
        }
    /*  Move the key and data  */
    
    hoblockmove (key, this_bucket->bucket + (this_bucket->no_of_keys * length),
                 hash_table->key_length);
    if (hash_table->data_length != 0)
        {
        hoblockmove (data,
                 this_bucket->bucket + (this_bucket->no_of_keys * length +
                                        hash_table->key_length),
                     hash_table->data_length);
        }
    this_bucket->no_of_keys++;
    

#if HT_TRACE  /* ht TRACE */
    fprintf (ht_fp, "a %x %x %d %d %d %d %d\n", hash_table, *key_ptr, address,
             need_to_allocate, curr_bucket_no, this_bucket->no_of_keys, SUCCESS);
    fflush (ht_fp);
#endif
    return SUCCESS;
    }
/* END:     */
