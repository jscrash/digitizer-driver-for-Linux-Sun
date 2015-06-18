/* DEC/CMS REPLACEMENT HISTORY, Element DR_UPD_NL.C*/
/* *3    17-OCT-1990 10:39:09 VINCE "(SPR 100) resolve Beta Merge conflicts"*/
/*  1V1  17-OCT-1990 09:28:54 VINCE "Deltas from BETA"*/
/* *2    30-AUG-1990 13:07:19 GILLESPIE "(SPR 1) Remove esi_qs.h"*/
/* *1    19-JUN-1989 12:49:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_UPD_NL.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


#include "esi_gl_defs.h"

#ifndef ESI_DR_H
#include "esi_dr.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#include "esi_ct_msg.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

/* ******************************************************************* */
/*  Convert an nList to the project default projection and update the  */
/*  file.                                                              */
/* ******************************************************************* */

#if USE_PROTOTYPES
publicdef INT dr_update_nlist (NLIST_HEADER nlist, FILENAME nlist_name,
                               NLIST_ID nlist_id, UINT *dim_list,
                               DOUBLE conversion_tolerance, DR_DATA *dr_data)
#else
publicdef INT dr_update_nlist (nlist, nlist_name, nlist_id, dim_list,
                               conversion_tolerance, dr_data)
NLIST_HEADER  nlist;                /* nlist to be updated                 */
FILENAME      nlist_name;           /* nlist file name                     */
NLIST_ID      nlist_id;             /* nlist ID                            */
UINT          dim_list[];           /* Array of dimension numbers          */
DOUBLE        conversion_tolerance; /* Minimum arc length                  */
DR_DATA       *dr_data;             /* Electronic Drafting data structure  */
#endif
{
    INT status, info_status, free_status;

    NLIST_HEADER nl_dummy;               /* Intermediate version  */
    BOOL free_nlist = TRUE;
    UINT ndim = 2;
    UINT info_dim_list[2];
    VOIDPTR value_list[2];
    FLOAT x_extent,y_extent;

    status = ct_project_nlist_xy (nlist, &nl_dummy, dim_list,
                                  conversion_tolerance);
    if ( (status != SUCCESS) && (status != CT_SAME_PROJECTIONS) )
        {
        return status;
        }

    if (status == CT_SAME_PROJECTIONS)
        {
        nl_dummy = nlist;
        free_nlist = FALSE;
        }

    /* set up parameters for nl_inq_nlist_info calls */
    info_dim_list[0] = 1; value_list[0] = (VOIDPTR)&x_extent;
    info_dim_list[1] = 2; value_list[1] = (VOIDPTR)&y_extent;

    /*  Update the nlist  */
    status = nl_delete_nlist (nlist_name, nlist_id);
    if (status < SUCCESS) goto abort_update;

    status = nl_write_nlist (nl_dummy, nlist_name, nlist_id);
    if (status < SUCCESS) goto abort_update;

    info_status = nl_inq_nlist_info (nl_dummy, NL_NLIST_MIN, ndim,
                                     info_dim_list, value_list); 

    if ((info_status == SUCCESS) && (dr_data->lower_left_extent.x > x_extent))
                    dr_data->lower_left_extent.x = x_extent;
    if ((info_status == SUCCESS) && (dr_data->lower_left_extent.y > y_extent))
                    dr_data->lower_left_extent.y = y_extent;

    info_status = nl_inq_nlist_info (nl_dummy, NL_NLIST_MAX, ndim,
                                     info_dim_list, value_list);

    if ((info_status == SUCCESS) && (dr_data->upper_right_extent.x < x_extent))
	             dr_data->upper_right_extent.x = x_extent;
    if ((info_status == SUCCESS) && (dr_data->upper_right_extent.y < y_extent))
	             dr_data->upper_right_extent.y = y_extent;

abort_update:
    if (free_nlist)
            free_status = nl_free_nlist (nl_dummy);

    return status;
}
