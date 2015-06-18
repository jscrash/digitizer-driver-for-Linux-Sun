/* DEC/CMS REPLACEMENT HISTORY, Element HT_GET_ALL.C*/
/* *2    17-AUG-1990 21:55:29 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_GET_ALL.C*/
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
#include "esi_tc.h"
#include "esi_am_defs.h"
#include "esi_ht_err.h"

#if HT_TRACE

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Retrieve all keys and data from a hash table.

Prototype:
    publicdef INT ht_get_all(HASH_TABLE_P hash_table, UINT *nkeys, VOIDPTR *keys_out, 
        VOIDPTR *data_out);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE_P) A hash table pointer.
    (O) nkeys       -(UINT *) The number of keys returned.
    (O) keys_out    -(VOIDPTR *) A vector of keys:
                        HT_INT_KEY    - a vector of INT.
                        HT_FLOAT_KEY  - a vector or either FLOAT or DOUBLE.
                        HT_DOUBLE_KEY -
                        HT_PTR_KEY    - a vector of pointers.
                        HT_CHAR_KEY   - a vector of CHAR, nkeys*key_length long.
    (O) data_out    -(VOIDPTR *) A BYTE vector nkeys*data_length long.

Return Value/Status:
    SUCCESS - Successful completion.
    HT_TABLE_NOT_INIT
    HT_TABLE_NOT_FOUND

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_get_all (HASH_TABLE_P hash_table, UINT *nkeys, VOIDPTR *keys_out,
                          VOIDPTR *data_out)
#else
publicdef INT ht_get_all (hash_table, nkeys, keys_out, data_out)
HASH_TABLE_P hash_table;
UINT *nkeys;
VOIDPTR *keys_out;
VOIDPTR *data_out;
#endif
    {
    INT i, j;
    HASH_BUCKET *this_bucket;
    BYTE *data;
    BYTE *keys;
    INT length;
    INT ovcount;
    

#if HT_TRACE         /* ht TRACE */
    extern FILE *ht_fp;
    fprintf (ht_fp, "g %x\n", hash_table);
#endif
    
    /*  Check that we have a hash table  */
    
    if (hash_table == (HASH_TABLE_P)NULL)
        {
        return HT_TABLE_NOT_INIT;
        }
    if (*(INT *)hash_table != HT_CHECK_CODE)
        return HT_TABLE_NOT_FOUND;
    
    /*  Count the keys  */
    
    *nkeys = 0;
    for (i = 0; i < hash_table->actual_no_of_buckets; i++)
        {
        this_bucket = &hash_table->bucket_array[i];
        while (this_bucket != (HASH_BUCKET *)NULL)
            {
            *nkeys += this_bucket->no_of_keys;
            this_bucket = this_bucket->next_bucket;
            }
        }
    if (*nkeys == 0)
        {
        *keys_out = (VOIDPTR)NULL;
        *data_out = (VOIDPTR)NULL;
        return SUCCESS;
        }
#if HT_TRACE
    printf ("num keys = %d\n\n", *nkeys);   /*DB*/
#endif
    
    /*  Retrieve the keys and data  */
    
    length = hash_table->key_length + hash_table->data_length;
    *keys_out = htz_zalloc (*nkeys * hash_table->key_length, AM_SYSTEM);
    if (hash_table->data_length > 0)
        *data_out = htz_zalloc (*nkeys * hash_table->data_length, AM_SYSTEM);
    else
        *data_out = (VOIDPTR)NULL;
    keys = (BYTE *) * keys_out;
    data = (BYTE *) * data_out;
    for (i = 0; i < hash_table->actual_no_of_buckets; i++)
        {
        this_bucket = &hash_table->bucket_array[i];
        ovcount = 0;                /*DB*/
        while (this_bucket != (HASH_BUCKET *)NULL)
            {
#if HT_TRACE
            printf ("bucket %d    overflow %d   numkeys %d\n", i + 1, ovcount,
                    this_bucket->no_of_keys);   /*DB*/
#endif
            for (j = 0; j < this_bucket->no_of_keys; j++)
                {
                hoblockmove (this_bucket->bucket + (j * length), keys,
                             hash_table->key_length);
#if HT_TRACE
                printf ("  k = %d", *keys); /*DB*/
#endif
                keys += hash_table->key_length;
                if (hash_table->data_length > 0)
                    {
                    hoblockmove (this_bucket->bucket + (j * length +
                                                        hash_table->key_length), data,
                                 hash_table->data_length);
#if HT_TRACE
                    printf ("             d =  %c\n", *data);   /*DB*/
#endif
                    data += hash_table->data_length;
                    }
                }
            this_bucket = this_bucket->next_bucket;
            ovcount++;              /*DB*/
            }
        }
    /*  Done  */
    
    return SUCCESS;
    }
/* END:     */
