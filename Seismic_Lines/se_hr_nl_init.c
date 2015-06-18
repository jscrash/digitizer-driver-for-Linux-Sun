/* DEC/CMS REPLACEMENT HISTORY, Element SE_HR_NL_INIT.C */
/* *6    17-AUG-1990 22:16:53 VINCE "(SPR 5644) Code Cleanup" */
/* *5    25-JUL-1990 16:13:49 GILLESPIE "(SPR 1) Change Var_List to correct type" */
/* *4    11-JUL-1990 12:27:22 GILLESPIE "(SPR 1) Change TIME to VALUE as label of 2nd column" */
/* *3    11-JUL-1990 12:23:21 GILLESPIE "(SPR 1) Change c_name to var_list on setting null value" */
/* *2    18-JUN-1990 12:08:32 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *1     5-MAR-1990 17:00:25 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_HR_NL_INIT.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"         /* string stuff */

#ifndef ESI_NL_H

#include "esi_nl.h"            

#endif

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT se_init_hor_nlist(NLIST_HEADER *nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_init_hor_nlist (NLIST_HEADER *nlist)
#else
publicdef INT se_init_hor_nlist (nlist)
NLIST_HEADER *nlist;
#endif
    {
    
    INT data_type[3], units[3], value_inc[3], status;
    size_t data_width[3];
    UINT Col_List[3];
    VOIDPTR Var_List[3];
    UINT block_size = 100;
    UINT nCol = 3;
    static CHAR *c_name[3] = 
        {
        "CDP",
        "VALUE",
        "FLAG"
        };
    INT null_cdp = -99999;
    INT null_flag = 0;
    FLOAT null_time = -9999.0;
    
    Col_List[0] = 1;
    Col_List[1] = 2;
    Col_List[2] = 3;
    data_type[0] = INT_TYPE;
    data_type[1] = FLOAT_TYPE;
    data_type[2] = INT_TYPE;
    data_width[0] = sizeof(CDP);
    data_width[1] = sizeof(FLOAT);
    data_width[2] = sizeof(TINY);
    units[0] = units[1] = units[2] = 0;
    value_inc[0] = value_inc[1] = value_inc[2] = VALUE_LIST;
    
    /* this nlist will hold the cdp value (int)  */
    /* and the time value(float)           */
    
    status = nl_init_nlist (nlist, nCol, units, data_width, data_type, value_inc,
                            block_size);
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_set_nlist_info (*nlist, NL_NLIST_COLUMN_NAME, nCol, Col_List,
                                (VOIDPTR)c_name);
    
    Var_List[0] = (VOIDPTR) & null_cdp;
    Var_List[1] = (VOIDPTR) & null_time;
    Var_List[2] = (VOIDPTR) & null_flag;
    status = nl_set_nlist_info (*nlist, NL_NLIST_NULL_VALUE, nCol, Col_List, Var_List);
    return status;
    }
/* END:     */
