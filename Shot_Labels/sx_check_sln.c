/* DEC/CMS REPLACEMENT HISTORY, Element SX_CHECK_SLN.C*/
/* *4    17-AUG-1990 22:19:50 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    17-JAN-1990 15:40:19 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *2    25-JUL-1989 20:09:48 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_CHECK_SLN.C*/
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

#include "esi_sx.h"

/* Function Description -----------------------------------------------------
Description:
    Check if shot label nlist ok, if not init.

Prototype:
    publicdef INT sx_check_sln(SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_check_sln (SX_STRUCT *psx)
#else
publicdef INT sx_check_sln (psx)
SX_STRUCT *psx;
#endif
    {
    INT units[3];
    INT value_inc[3];
    INT data_types[3];
    size_t data_sizes[3];
    INT status = SUCCESS;
    UINT stroke;
    static UINT d_list[3] = 
        {
        1,
        2,
        3
        };
    static CHAR *c_name[4] = 
        {
        "X",
        "Y",
        "CDP",
        NULL
        };
    
    if (psx->pso->disp_opt.shot_label_nlist != (NLIST_HEADER)0)
        {
        SX_RETURN;
        }
    /* set up line */
    
    units[0] = units[1] = units[2] = 0;
    value_inc[0] = value_inc[1] = value_inc[2] = VALUE_LIST;
    data_types[0] = INT_TYPE;
    data_types[1] = FLOAT_TYPE;
    data_types[2] = INT_TYPE;
    data_sizes[0] = sizeof(INT);
    data_sizes[1] = sizeof(FLOAT);
    data_sizes[2] = sizeof(INT);
    
    nl_init_nlist (&psx->pso->disp_opt.shot_label_nlist, 3, units, data_sizes,
                   data_types, value_inc, 0);
    
    status = nl_set_nlist_info (psx->pso->disp_opt.shot_label_nlist,
                                NL_NLIST_COLUMN_NAME, 3, d_list, (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_stroke (psx->pso->disp_opt.shot_label_nlist, &stroke);
    
    SX_RETURN;
    }
/* END:     */
