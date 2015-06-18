/* DEC/CMS REPLACEMENT HISTORY, Element AM_INITIALIZE.C*/
/* *4    14-AUG-1990 11:40:00 VINCE "(SPR 5644) Header Standardization"*/
/* *3    19-NOV-1989 08:58:40 GILLESPIE "(SPR 5075) Fix call to am_start_application"*/
/* *2    20-JUN-1989 10:42:27 GILLESPIE "(SPR 21) Fix minor casting problems"*/
/* *1    19-JUN-1989 11:59:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_INITIALIZE.C*/
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

#include "esi_c_lib.h"

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#ifndef ESI_HT_H

#include "esi_ht.h"

#endif

#ifndef ESI_AM_STAT_H

#include "esi_am_stat.h"

#endif

#ifndef ESI_AM_INT_H

#include "esi_am_int.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    AM_INITIALIZE initializes the data structures needed in order for the
    application manager to operate.  AM_INITIALIZE should be called once,
    before any application manager services are called.

Prototype:
    publicdef INT am_initialize();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.

Return Value/Status:
    SUCCESS - Successfull initialization.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_initialize () 
#else
publicdef INT am_initialize ()
#endif
    {
    APPLICATION_STATIC *application_static = am_static ();
    
    /*   Initialize the RESOURCE_LIST. */
    
    btinit (&application_static->resource_list, 0, sizeof(INT), sizeof(RESOURCE_LIST),
            BT_TREE_SYSTEM);
    
    /*   Initialize the DATA_LIST. */
    
    ht_init_hash_table (AM_NUM_BUCKETS, AM_BUCKET_SIZE, HT_PTR_KEY, sizeof(BYTE *),
                        sizeof(DATA_LIST), HT_SYSTEM_TABLE,
                        &application_static->data_list);
    
    /*   Initialize the WORKSPACE_LIST. */
    
    llinit (&application_static->workspace_list, 0, sizeof(WORKSPACE_LIST),
            LL_LIST_SYSTEM);
    
    /*   Initialize the APPLICATION_LIST. */
    
    btinit (&application_static->application_list, 0, sizeof(INT),
            sizeof(APPLICATION_LIST), BT_TREE_SYSTEM);
    
    /*   Initialize current application id. */
    
    application_static->application_id = 0;
    application_static->next_application_id = 0;
    
    /*  Indicate no journal file active. */
    
    application_static->journal_file = NULL_FILE_PTR;
    
    am_start_application (NULL_FUNCTION_PTR, (VOIDPTR)0);
    am_declare ("APP MGR");
    
    application_static->initialized = TRUE;
    
    return SUCCESS;
    }
/* END:     */
