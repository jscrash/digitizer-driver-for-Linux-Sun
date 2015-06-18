/* DEC/CMS REPLACEMENT HISTORY, Element FM_SYS_CONT.C*/
/* *6    14-AUG-1990 13:25:40 VINCE "(SPR 5644) Header Standardization"*/
/* *5     7-NOV-1989 13:23:54 GILLESPIE "(SPR 30) Use esi_fm_int.h include file"*/
/* *4     7-NOV-1989 12:18:11 GILLESPIE "(SPR 30) Add call to fm_sys_index to get hash table ptr"*/
/* *3     2-NOV-1989 15:26:17 JULIAN "(SPR 30) mod fm_hash_table as static"*/
/* *2    27-JUL-1989 15:22:59 GILLESPIE "(SPR 1) Fix monor casting problem"*/
/* *1    19-JUN-1989 12:50:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FM_SYS_CONT.C*/
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
#include "esi_fm_int.h"
#include "esi_fm_msg.h"
#include "esi_fm_err.h"
#include "esi_ho_files.h"

/* * private data objects * */
/* ----------------------- */

static HASH_TABLE_P fm_hash_table = (HASH_TABLE_P)
0;
static BOOL inited = FALSE;

/* File Description ---------------------------------------------------------
Overview:
    Two Functions for controlling the file management system.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT fm_sys_init();
    
    publicdef INT fm_sys_term();
    
    publicdef HASH_TABLE_P fm_sys_index();

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Function to initialize the FILE MANAGEMENT SYSTEM by
    allocating the HASH TABLE which stores the necessary
    information to be passed between FM_INIT_FILE and
    FM_RELEASE_FILE.

Prototype:
    publicdef INT fm_sys_init();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    FM_CANT_INIT_SYSTEM
    FM_ALREADY_INITED
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fm_sys_init ()
#else
publicdef INT fm_sys_init ()
#endif
    {
    INT status;
    
    /* ********************************************************************** */
    
    if (NOT inited)
        {
        if ((status = ht_init_hash_table (100, 10, HT_CHAR_KEY, FM_KEY_SIZE,
                                          sizeof(FM_STRUCT *), HT_SYSTEM_TABLE,
                                          &fm_hash_table)) != SUCCESS)
            {
            status = FM_CANT_INIT_SYSTEM;
            }
        else
            {
            inited = TRUE;
            }
        }
    else
        {
        status = FM_ALREADY_INITED;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef HASH_TABLE_P fm_sys_index();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of fm_hash_table.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef HASH_TABLE_P fm_sys_index ()
#else
publicdef HASH_TABLE_P fm_sys_index ()
#endif
    {
    return fm_hash_table;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to terminate the FILE MANAGEMENT SYSTEM by
    retrieving from the FM HASH TABLE all still
    existing entries and sending them off to their
    appropriate termination function, and then
    freeing the hash table.

Prototype:
    publicdef INT fm_sys_term();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    FM_NOT_INITED
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fm_sys_term ()
#else
publicdef INT fm_sys_term ()
#endif
    {
    INT status;
    UINT n_items;
    BYTE *keys;
    FM_STRUCT **fm;
    FM_STRUCT *temp;
    FILENAME os_name;
    INT i;
    
    /* ********************************************************************** */
    
    if (inited)
        {
        if ((status = ht_get_all (fm_hash_table, &n_items, (VOIDPTR *) & keys,
                                  (VOIDPTR *) & fm)) != SUCCESS)
            {
            status = (n_items EQUALS 0) ? SUCCESS : FM_TERM_ERROR;
            }
        else
            {
            for (i = 0; i < n_items; i++)
                {
                temp = fm[i];
                if (temp->in_use AND temp->term_func != NULL_FUNCTION_PTR)
                    {
                    fm_create_os_filename (temp->file_id, os_name);
                    ho_add_path (temp->fi_path, os_name, sizeof(FILENAME), os_name);
                    (*(temp->term_func))(temp->file_id, os_name);
                    }
                }
            ht_free_table (&fm_hash_table);
            inited = FALSE;
            }
        }
    else
        {
        status = FM_NOT_INITED;
        }
    return status;
    }
/* END:     */
