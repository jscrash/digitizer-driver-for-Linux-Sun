/* DEC/CMS REPLACEMENT HISTORY, Element WE_SYS_INIT.C*/
/* *3    14-AUG-1990 13:39:51 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-NOV-1989 11:16:53 CONROY "(SPR 5063) Modifiy ht_init call"*/
/* *1    19-JUN-1989 13:35:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WE_SYS_INIT.C*/
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

#include "esi_we_wells.h"
#include "esi_ht.h"

/* these initialize the common tie points of the FINDER well system */

static HASH_TABLE_P well_chain;
static HASH_TABLE_P strat_column;
static BOOL wells_initialized = FALSE;

/* Function Description -----------------------------------------------------
Description:
   This routine initializes the dynamic memory and state variables for the
   Finder V5.0 system.

Prototype:
    publicdef VOID well_system_init();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns either well_chain, strat_column, or (HASH_TABLE_P)0.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID well_system_init () 
#else
publicdef VOID well_system_init ()
#endif
    {
    INT key_type = HT_CHAR_KEY;
    INT flags = HT_SYSTEM_TABLE;
    INT nbuckets = 1013;
    INT bucket_size = 5;
    
    if (!wells_initialized)
        {
        ht_init_hash_table (nbuckets, bucket_size, key_type, sizeof(UWI),
                            sizeof(VOIDPTR), flags, &well_chain);
        ht_init_hash_table (nbuckets, bucket_size, key_type, sizeof(INT),
                            sizeof(LITHOSTRAT), flags, &strat_column);
        wells_initialized = TRUE;
        }
    }

/******************************************************************************/

HASH_TABLE_P SYSTEM_ANCHOR_POINT (which)
INT which;
    {
    if (!wells_initialized)
        well_system_init ();
    
    switch (which)
        {
    case 1:
        return well_chain;
    case 2:
        return strat_column;
    default:
        return(HASH_TABLE_P)0;
        }
    }
/* END:     */
