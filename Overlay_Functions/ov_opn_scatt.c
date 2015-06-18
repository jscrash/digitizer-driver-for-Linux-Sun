/* DEC/CMS REPLACEMENT HISTORY, Element OV_OPN_SCATT.C*/
/* *7    28-NOV-1990 16:08:14 GILLESPIE "(SPR 1) Change column names"*/
/* *6    25-JUL-1990 19:59:32 VINCE "(SPR 5644) Header Standardization"*/
/* *5    17-JAN-1990 15:38:03 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4    21-DEC-1989 15:33:35 GORDON "(SPR 0) cleanup variables"*/
/* *3     8-NOV-1989 09:58:54 JULIAN "(SPR 30) fix casting"*/
/* *2    14-SEP-1989 18:27:27 GORDON "(SPR -1) checkin from gulf"*/
/* *1    19-JUN-1989 13:22:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_OPN_SCATT.C*/
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
#include "esi_ov_defs.h"

#ifndef ESI_OV_H

#include "esi_ov.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_OV_ERR_H

#include "esi_ov_err.h"

#endif

#ifndef ESI_FM_MSG_H

#include "esi_fm_msg.h"

#endif

#ifndef ESI_FM_ERR_H

#include "esi_fm_err.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to open a scatter file. Returns file pointer.

Prototype:
    publicdef INT ov_open_scatter(NLIST_HEADER *scatter,FM_LONG name,CHAR *mode,
        FILENAME os_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    scatter         -(NLIST_HEADER *) File pointer to return.
    name            -(FM_LONG) Scatter file name.
    mode            -(CHAR *) Open mode.
    os_name         -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    OV_CANT_OPEN_SCATTER
    OV_PROJECTION_MISMATCH
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_open_scatter (NLIST_HEADER *scatter, FM_LONG name, CHAR *mode,
                               FILENAME os_name)
#else
publicdef INT ov_open_scatter (scatter, name, mode, os_name)
NLIST_HEADER *scatter;
FM_LONG name;
CHAR *mode;
FILENAME os_name;
#endif
    {
    
    INT status;                     /* INTERNAL STATUS */
    CHAR map_proj[80];              /* MAP PROJECTION */
    CHAR scat_proj[80];             /* MAP PROJECTION */
    BOOL perm = TRUE;
    FM_DESCRIB desc;
    CHAR temp_mode[10];
    BOOL exists = FALSE;
    OV_SCATTER_USER scatter_user;
    UINT i, ncol, temp;
    FLOAT null = -20000000.0;
    
    INT units[3];
    INT d_size[3];
    INT d_type[3];
    INT v_type[3];
    INT block = 256;
    
    UINT Col_List[3];
    VOIDPTR Var_List[3];
    static CHAR *c_name[4] = 
        {
        "X",
        "Y",
        "Z",
        NULL
        };
    
    /* ********************************************************************** */
    
    if (ARE_SAME (mode, "a"))
        {
        strcpy (temp_mode, "w");
        }
    else
        {
        strcpy (temp_mode, mode);
        }
    exists = (fm_check_access ("ESI$SCATTERS", name, "SCATTER",
                               temp_mode) != FM_NO_SUCH_FILE);
    
    /* * FIRST TRY TO INITIALIZE FILE IN FM SYSTEM * */
    /* --------------------------------------------- */
    
    desc[0] = NUL;
    if ((status = fm_init_file ("ESI$SCATTERS", name, "SCATTER", temp_mode, perm, desc,
                                fm_scatter_term_func, os_name, "MAPPING")) == SUCCESS)
        {
        qs_inq_c (QS_MAP_PROJECTION, map_proj, (INT *)0);
        
        /* * MODE IS READ OR APPEND TO EXISTING * */
        /* -------------------------------------- */
        
        if (ARE_SAME (mode, "r") OR (ARE_SAME (mode, "a") && exists))
            {
            /* GET SCATTER NLIST */
            
            if ((status = nl_read_nlist (scatter, os_name, "SCATTER")) EQUALS SUCCESS)
                {
                /* CHECK PROJECTION */
                
                nl_inq_nlist_user_struct (*scatter, sizeof(OV_SCATTER_USER),
                                          (VOIDPTR) & scatter_user, (size_t *) & temp);
                if (ARE_DIFFERENT (scatter_user.projection, map_proj))
                    {
                    status = OV_PROJECTION_MISMATCH;
                    }
                /* SET IT TO CORRECT POINT */
                else
                    {
                    if (ARE_SAME (mode, "a"))
                        {
                        nl_inq_nlist_int (*scatter, NL_NLIST_NROWS, (INT *) & temp);
                        status = nl_set_current_row (*scatter, temp, 1);
                        status = nl_delete_nlist (os_name, "SCATTER");
                        }
                    else
                        {
                        status = nl_set_current_row (*scatter, 1, 1);
                        }
                    }
                }
            }
        /* * MODE IS WRITE OR APPEND TO NON-EXISTING * */
        /* ------------------------------------------- */
        
        else
            {
            if (exists)
                {
                nl_delete_nlist (os_name, "SCATTER");
                }
            ncol = 3;
            for (i = 0; i < ncol; i++)
                {
                units[i] = 0;
                d_size[i] = sizeof(FLOAT);
                d_type[i] = FLOAT_TYPE;
                v_type[i] = VALUE_LIST;
                Col_List[i] = i + 1;
                }
            if ((status = nl_init_nlist (scatter, ncol, units, (size_t *)d_size,
                                         d_type, v_type, block)) EQUALS SUCCESS)
                {
                status = nl_set_nlist_info (*scatter, NL_NLIST_COLUMN_NAME, ncol,
                                            Col_List, (VOIDPTR)c_name);
                if (status != SUCCESS)
                    {
                    return status;
                    }
                Var_List[0] = (VOIDPTR)(&null);
                Var_List[1] = (VOIDPTR)(&null);
                Var_List[2] = (VOIDPTR)(&null);
                nl_set_nlist_info (*scatter, NL_NLIST_NULL_VALUE, ncol, Col_List,
                                   Var_List);
                nl_start_table (*scatter, &temp);
                strcpy (scatter_user.projection, map_proj);
                status = nl_set_nlist_user_struct (*scatter, (VOIDPTR) & scatter_user,
                                                   sizeof(OV_SCATTER_USER));
                }
            }
        /* * OPEN ERROR SO RELEASE FROM FM SYSTEM * */
        /* ---------------------------------------- */
        
        if (status >= SUCCESS)
            {
            status = SUCCESS;
            }
        else
            {
            fm_release_file ("ESI$SCATTERS", name, "SCATTER");
            }
        }
    else
        {
        status = OV_CANT_OPEN_SCATTER;
        }
    return status;
    }
/* END:     */
