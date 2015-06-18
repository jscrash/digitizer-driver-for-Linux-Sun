/* DEC/CMS REPLACEMENT HISTORY, Element HT_FREE_TABLE.C*/
/* *2    17-AUG-1990 21:55:26 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HT_FREE_TABLE.C*/
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
    Frees the space used by a hash table.

Prototype:
    publicdef INT ht_free_table(HASH_TABLE_P  *hash_table);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hash_table  -(HASH_TABLE_P *) Pointer to the table to be freed.

Return Value/Status:
    SUCCESS - Successful completion.
    HT_TABLE_NOT_FOUND

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ht_free_table (HASH_TABLE_P *hash_table)
#else
publicdef INT ht_free_table (hash_table)
HASH_TABLE_P *hash_table;
#endif
    {
    INT i;
    HASH_BUCKET *this_bucket;
    HASH_BUCKET *temp_bucket;
    HASH_TABLE_P this_table;
    

#if HT_TRACE  /* ht TRACE */
    extern FILE *ht_fp;
    fprintf (ht_fp, "t %x\n", *hash_table);
    fflush (ht_fp);
#endif
    
    /*  Check for a null or an invalid table  */
    
    if (*hash_table == (HASH_TABLE_P)NULL)
        {
        return SUCCESS;
        }
    if (*(INT *)(*hash_table) != HT_CHECK_CODE)
        return HT_TABLE_NOT_FOUND;
    
    /*  Free the buckets  */
    
    this_table = *hash_table;
    for (i = 0; i < this_table->actual_no_of_buckets; i++)
        {
        this_bucket = &this_table->bucket_array[i];
        tc_free (this_bucket->bucket);
        
        this_bucket = this_bucket->next_bucket;
        
        while (this_bucket != (HASH_BUCKET *)NULL)
            {
            tc_free (this_bucket->bucket);
            temp_bucket = this_bucket;
            this_bucket = this_bucket->next_bucket;
            tc_free (temp_bucket);
            }
        }
    tc_free (this_table->bucket_array);
    
    /*  Free the header  */
    
    tc_free (this_table);
    *hash_table = (HASH_TABLE *)NULL;
    
    return SUCCESS;
    }
/* END:     */
