/* DEC/CMS REPLACEMENT HISTORY, Element DR_WR_NL.C*/
/* *2    30-AUG-1990 13:07:25 GILLESPIE "(SPR 1) Remove esi_qs.h"*/
/* *1    19-JUN-1989 12:49:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_WR_NL.C*/

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


#include "esi_c_lib.h"
#include "esi_dr.h"
#ifndef ESI_CT_H
#include "esi_ct.h"
#include "esi_ct_msg.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

/* ********************************************************************** */

#if USE_PROTOTYPES
publicdef INT dr_write_nlist(NLIST_HEADER nlist, FILENAME nlist_file,
    NLIST_ID nlist_id,UINT *dim_list, DOUBLE conversion_tolerance,
    DR_DATA *dr_data)
#else
publicdef INT dr_write_nlist(nlist, nlist_file, nlist_id,dim_list,
                       conversion_tolerance, dr_data)

/*  Convert a nlist to the project default projection and write the
    file
*/

NLIST_HEADER  nlist;                   /* nlist to be updated */
FILENAME      nlist_file;               /* nlist file name     */
NLIST_ID      nlist_id;                 /* nlist ID            */
UINT           dim_list[];               /* Array of dimension numbers */
DOUBLE        conversion_tolerance;     /* Minimum arc length      */
DR_DATA       *dr_data;
#endif
{

    INT status;
    NLIST_HEADER nl_dummy;                /* Intermediate version  */
    UINT ndim = 2;
    UINT info_dim_list[2];
    VOIDPTR value_list[2];
    FLOAT x_extent,y_extent;
    BOOL free_nlist = TRUE;

/*  Convert the nlist  */


    status = ct_project_nlist_xy(nlist, &nl_dummy, dim_list, 
					    conversion_tolerance);
    if(status != CT_SAME_PROJECTIONS)
    {
	 if(status != 0)
         {
              return status;
         }
    }
    else 
    {
	nl_dummy = nlist;
	free_nlist = FALSE;
    }

/*  Write the nlist  */

    status = nl_write_nlist(nl_dummy,nlist_file, nlist_id);

    if(status < 0)
    {
         return( status );
    }

		    /* To update the extents of all items of this object. */
    
    info_dim_list[0] = 1, value_list[0] = (VOIDPTR)&x_extent;
    info_dim_list[1] = 2, value_list[1] = (VOIDPTR)&y_extent;

    status = nl_inq_nlist_info(nl_dummy,NL_NLIST_MIN,ndim,
					    info_dim_list,value_list); 
	            
    if(dr_data->lower_left_extent.x > x_extent)
	             dr_data->lower_left_extent.x = x_extent;
    if(dr_data->lower_left_extent.y > y_extent)
	             dr_data->lower_left_extent.y = y_extent;
	
    status = nl_inq_nlist_info(nl_dummy,NL_NLIST_MAX,ndim,
						info_dim_list,value_list); 
	            	
    if(dr_data->upper_right_extent.x < x_extent)
	             dr_data->upper_right_extent.x = x_extent;
    if(dr_data->upper_right_extent.y < y_extent)
	             dr_data->upper_right_extent.y = y_extent;
	

    if( free_nlist )
        status = nl_free_nlist(nl_dummy);

    return SUCCESS;
}
