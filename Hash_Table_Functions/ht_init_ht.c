/* DEC/CMS REPLACEMENT HISTORY, Element HT_INIT_HT.C*/
/* *4    17-AUG-1990 21:55:38 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    21-NOV-1989 10:02:14 CONROY "(SPR 5059) Add new key type HT_BYTE_KEY"*/
/* *2    11-AUG-1989 10:28:46 JULIAN "(SPR -1) statement not reached"*/
/* *1    19-JUN-1989 13:01:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_INIT_HT.C*/
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

#include    "esi_ht.h"
#include    "esi_am.h"
#include    "esi_tc.h"
#include 	"esi_ht_err.h"

#if HT_TRACE

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Creates and initializes a hash table.

Prototype:
    publicdef INT ht_init_hash_table(UINT no_of_buckets, UINT   bucket_size, INT key_type, 
        size_t key_length, size_t data_length, INT  flag, HASH_TABLE_P  *hash_table);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) no_of_buckets   -(UINT) The number of buckets to allocate.
    (I) bucket_size     -(UINT) The number of keys each bucket will hold.
    (I) key_type        -(INT) The key type - one of the following as
                            defined in "esi_ht.h" :
                            HT_INT_KEY, HT_FLOAT_KEY, HT_DOUBLE_TYPE,
                            HT_CHAR_TYPE, HT_PTR_TYPE.
    (I) key_length      -(size_t) The length of the key in bytes. This is
                            only used if a key type of CHAR is
                            specified. In all other cases it is ignored.
    (I) data_length     -(data_length) The length of the data field (may be zero).
    (I) flag            -(INT) One of HT_APPLICATION_TABLE or
                            HT_SYSTEM_TABLE as defined in "esi_ht.h"
                            or AM_SYSTEM or AM_APPLICATION as defined in
                            "esi_am_int.h". If either of the HT type 
                            flags is used, it is converted to the 
                            corresponding AM type flag.
    (O) hash_table      -(HASH_TABLE_P *) A pointer to the hash table which has been
                            created.

Return Value/Status:
    SUCCESS - Successful completion.
    HT_ALLOCATIION_ERROR
    HT_INVALID_KEY_TYPE
    HT_INVALID_KEY_LENGTH
    HT_INVALID_FLAG_TYPE

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_init_hash_table (UINT no_of_buckets, UINT bucket_size, INT key_type,
                                  size_t key_length, size_t data_length, INT flag,
                                  HASH_TABLE_P *hash_table)
#else
publicdef INT ht_init_hash_table (no_of_buckets, bucket_size, key_type, key_length,
                                  data_length, flag, hash_table)
UINT no_of_buckets;
UINT bucket_size;
INT key_type;
size_t key_length;
size_t data_length;
INT flag;
HASH_TABLE_P *hash_table;
#endif
    {
    HASH_TABLE_P new_table;
    HASH_BUCKET this_bucket;
    INT i, j;
    BOOL prime;

#if HT_TRACE
    extern FILE *ht_fp;             /*   TRACE file  */
#endif
    
    /*  Validate flag  */
    
    switch (flag)
        {
    case AM_SYSTEM:
    case AM_APPLICATION:
        break;
        
    case HT_APPLICATION_TABLE:
        flag = AM_APPLICATION;
        break;
        
    case HT_SYSTEM_TABLE:
        flag = AM_SYSTEM;
        break;
        
    default:
        return HT_INVALID_FLAG_TYPE;
        }
    /*  Validate key type and set key length     */
    
    switch (key_type)
        {
    case HT_INT_KEY:
        key_length = sizeof(INT);
        break;
        
    case HT_FLOAT_KEY:
        key_length = sizeof(FLOAT);
        break;
        
    case HT_DOUBLE_KEY:
        key_length = sizeof(DOUBLE);
        break;
        
    case HT_PTR_KEY:
        key_length = sizeof(BYTE_P);
        break;
        
    case HT_CHAR_KEY:
    case HT_BYTE_KEY:
        if (key_length < 1)
            {
            return HT_INVALID_KEY_LENGTH;
            }
        break;
        
    default:
        return HT_INVALID_KEY_TYPE;
        }
    /*  Allocate memory for the table header  */
    
    new_table = (HASH_TABLE_P)htz_zalloc (sizeof(HASH_TABLE), flag);
    *hash_table = new_table;
    if (new_table == (HASH_TABLE *)NULL)
        {
        return HT_ALLOCATION_ERROR;
        }
    /*  Build table header   */
    
    new_table->ht_check_field = HT_CHECK_CODE;
    new_table->user_no_of_buckets = no_of_buckets;
    new_table->bucket_size = bucket_size;
    new_table->key_type = key_type;
    new_table->key_length = key_length;
    new_table->data_length = data_length;
    new_table->flag = flag;
    
    /*  Compute the greatest prime number <= to no_of_buckets  */
    
    for (i = no_of_buckets; i >= 3; i--)
        {
        prime = TRUE;
        for (j = 2; j < i; j++)
            {
            if (i % j == 0)
                {
                prime = FALSE;
                break;
                }
            }
        if (prime)
            break;
        }
    new_table->actual_no_of_buckets = i;
    
    /*  Allocate the bucket pointer array  */
    
    new_table->bucket_array = (HASH_BUCKET *)htz_zalloc (i * sizeof(HASH_BUCKET),
                                                         flag);
    if (new_table->bucket_array == (HASH_BUCKET *)NULL)
        {
        tc_free (new_table);
        *hash_table = (HASH_TABLE_P)NULL;
        return HT_ALLOCATION_ERROR;
        }
    /*  Allocate the buckets  */
    
    for (i = 0; i < new_table->actual_no_of_buckets; i++)
        {
        new_table->bucket_array[i].bucket = (BYTE *)htz_zalloc (bucket_size *
                                                                (key_length +
                                                                 data_length), flag);
        this_bucket = new_table->bucket_array[i];
        if (new_table->bucket_array[i].bucket == (BYTE *)NULL)
            {
            tc_free (new_table->bucket_array);
            tc_free (new_table);
            *hash_table = (HASH_TABLE_P)NULL;
            return HT_ALLOCATION_ERROR;
            }
        }
    /*  Done  */

#if HT_TRACE
    fprintf (ht_fp, "I %x %d %d %d %d %d %d\n", new_table, no_of_buckets,
             new_table->actual_no_of_buckets, bucket_size, key_type, key_length,
             data_length);
    fflush (ht_fp);
#endif
    return SUCCESS;
    }
/* END:     */
