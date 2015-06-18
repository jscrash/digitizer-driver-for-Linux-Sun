/* DEC/CMS REPLACEMENT HISTORY, Element HT_COUNT_KEYS.C*/
/* *2    17-AUG-1990 21:55:05 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_COUNT_KEYS.C*/
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
#include "esi_ht_err.h"

/* Function Description -----------------------------------------------------
Description:
    Retrieve the number of keys in a hash table.

Prototype:
    publicdef INT ht_count_keys(HASH_TABLE_P hash_table, UINT *nkeys);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE_P) A hash table pointer.
    (O) nkeys       -(UINT *) The number of keys returned.

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
publicdef INT ht_count_keys (HASH_TABLE_P hash_table, UINT *nkeys)
#else
publicdef INT ht_count_keys (hash_table, nkeys)
HASH_TABLE_P hash_table;
UINT *nkeys;
#endif
    {
    UINT i;
    HASH_BUCKET *this_bucket;
    
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
    /*  Done  */
    
    return SUCCESS;
    }
/* END:     */
