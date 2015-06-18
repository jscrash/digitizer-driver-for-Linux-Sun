/* DEC/CMS REPLACEMENT HISTORY, Element DB_MV_BTSTRP.C */
/* *3    14-AUG-1990 13:09:22 VINCE "(SPR 5644) Header Standardization" */
/* *2    11-AUG-1989 13:51:04 CONROY "(SPR 9999) Initial checkin" */
/* *1    11-AUG-1989 13:23:51 CONROY "Move bootstrap tables to correct tablespaces" */
/* DEC/CMS REPLACEMENT HISTORY, Element DB_MV_BTSTRP.C */
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
#include "esi_gl_defs.h"
#include "esi_db.h"

/* Function Description -----------------------------------------------------
Description:
    This function moves the bootstrap tables of the specified account.
    
Prototype:
    publicdef INT db_move_bootstraps(DB_ACCOUNT_STRUCT *db_account);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) db_account  -(DB_ACCOUNT_STRUCT *) Account structure.
    
Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT db_move_bootstraps (DB_ACCOUNT_STRUCT *db_account) 
#else
publicdef INT db_move_bootstraps (db_account)
DB_ACCOUNT_STRUCT *db_account;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    /* ********************************************************************** */
    
    db_move_table ("LOGICAL_NAMES", "DATA");
    db_move_table ("ACCOUNT_TB_DEFS", "DATA");
    db_move_table ("TEMP_TBSPACE_DEFS", "DATA");
    return SUCCESS;
    }
/* END:     */
