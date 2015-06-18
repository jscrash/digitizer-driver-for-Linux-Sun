/* DEC/CMS REPLACEMENT HISTORY, Element HT_CLR_TAB.C*/
/* *2    17-AUG-1990 21:55:01 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_CLR_TAB.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Clears all the entries in a hash table.

Prototype:
    publicdef INT ht_clear_table(HASH_TABLE  *hash_table);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE *) Pointer to the table to be cleared.

Return Value/Status:
    SUCCESS - Successful completion.
    HT_TABLE_NOT_FOUND

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_clear_table (HASH_TABLE *hash_table)
#else
publicdef INT ht_clear_table (hash_table)
HASH_TABLE *hash_table;
#endif
    {
    UINT i;
    HASH_BUCKET *this_bucket;
    HASH_BUCKET *temp_bucket;
    INT length;
    
    /*  Check for a null or an invalid table  */
    
    if (hash_table == (HASH_TABLE *)NULL)
        {
        return SUCCESS;
        }
    if (hash_table->ht_check_field != HT_CHECK_CODE)
        return HT_TABLE_NOT_FOUND;
    
    /*  Clear the buckets  */
    
    length = hash_table->bucket_size * (hash_table->key_length +
                                        hash_table->data_length);
    for (i = 0; i < hash_table->actual_no_of_buckets; i++)
        {
        this_bucket = &hash_table->bucket_array[i];
        temp_bucket = this_bucket;
        
        this_bucket = this_bucket->next_bucket;
        tc_zeroes (temp_bucket->bucket, length);
        temp_bucket->no_of_keys = 0;
        temp_bucket->next_bucket = (HASH_BUCKET *)NULL;
        
        while (this_bucket != (HASH_BUCKET *)NULL)
            {
            tc_free (this_bucket->bucket);
            temp_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            tc_free (temp_bucket);
            }
        }
    /*  Zero out the key count  */
    
    hash_table->current_key_count = 0;
    
    return SUCCESS;
    }
/* END:     */
