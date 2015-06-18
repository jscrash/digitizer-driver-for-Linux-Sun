/* DEC/CMS REPLACEMENT HISTORY, Element HT_FIND.C*/
/* *3    17-AUG-1990 21:55:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    21-NOV-1989 10:00:06 CONROY "(SPR 5059) Use subroutine HT_CHECK_FOR_KEY"*/
/* *1    19-JUN-1989 13:00:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_FIND.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Find a key in the hash table.

Prototype:
    publicdef INT ht_find(HASH_TABLE_P hash_table, VOIDPTR key, VOIDPTR data);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE_P) Pointer to the hash table to be searched.
    (I) key         -(VOIDPTR) Pointer to the desired key.
    (O) data        -(VOIDPTR) Pointer to the area to recieve the data, if any.

Return Value/Status:
    SUCCESS - Successful completion.
    HT_KEY_NOT_FOUND
    HT_TABLE_NOT_INIT
    HT_TABLE_NOT_FOUND
    HT_BAD_DATA_PTR

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_find (HASH_TABLE_P hash_table, VOIDPTR key, VOIDPTR data)
#else
publicdef INT ht_find (hash_table, key, data)
HASH_TABLE_P hash_table;
VOIDPTR key;
VOIDPTR data;
#endif
    {
    INT address;
    HASH_BUCKET *this_bucket;
    HASH_BUCKET *prev_bucket;
    BYTE *curr_data;
    UINT i;
    INT status;
    
    /***********************************************************************/
    
    /*  check that we have a valid hash table */
    
    if (hash_table == (HASH_TABLE_P)NULL)
        {
        return HT_TABLE_NOT_INIT;
        }
    if (*(INT *)hash_table != HT_CHECK_CODE)
        {
        return HT_TABLE_NOT_FOUND;
        }
    /*  Check the data pointer  */
    
    if ((hash_table->data_length > 0) && ((BYTE *)data == (BYTE *)NULL))
        {
        return HT_BAD_DATA_PTR;
        }
    /*  compute the address  */
    
    ht_hash_code (key, hash_table->key_type, hash_table->key_length,
                  hash_table->actual_no_of_buckets, &address);
    
    /*  Search for  key  */
    
    if ((status = ht_check_for_key (hash_table, key, address, &this_bucket,
                                    &prev_bucket, &i)) EQUALS SUCCESS)
        {
        if (hash_table->data_length != 0)
            {
            curr_data = (BYTE *)(this_bucket->bucket + (i + 1) *
                                 hash_table->key_length + i * hash_table->data_length);
            hoblockmove (curr_data, data, hash_table->data_length);
            }
        }
    return status;
    }
/* END:     */
