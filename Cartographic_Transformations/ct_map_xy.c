/* DEC/CMS REPLACEMENT HISTORY, Element CT_MAP_XY.C*/
/* *4     5-JUN-1990 09:59:02 JULIAN "(SPR 0) fix return status"*/
/* *3    28-FEB-1990 10:24:21 GILLESPIE "(SPR 1) Fix up error checking"*/
/* *2    10-OCT-1989 09:42:00 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    19-JUN-1989 12:45:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_MAP_XY.C*/


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


#include "esi_ct.h"
#include "esi_qs.h"
#include "esi_ts.h"

#ifndef ESI_CT_ERR_H
#include "esi_ct_err.h"
#endif
                                 
/* ********************************************************************** */

/* TITLE : CT_MAP_XY - Convert a single point from the project to 
                       the map projection */

/* ********************************************************************** */

publicdef INT ct_map_xy(x_coord_in, y_coord_in, x_coord_out, y_coord_out)

DOUBLE x_coord_in;              /* Point in project projection */
DOUBLE y_coord_in;
DOUBLE *x_coord_out;		/* Point to be in map projection */
DOUBLE *y_coord_out;

{
INT status;
INT len;
PROJECTION_ID project_projection;
PROJECTION_ID map_projection;
PROJECTION_STRUCTURE *project_data;
PROJECTION_STRUCTURE *map_data;

/* ********************************************************************** */

/* * COMPARE THE PROJECTION ID'S * */
/* ------------------------------- */

qs_inq_c(QS_PROJECT_PROJECTION, project_projection, &len);
if (len EQUALS 0)
    {
    return CT_BAD_PROJECTION;
    }
qs_inq_c(QS_MAP_PROJECTION, map_projection, &len);
if (len EQUALS 0)
    {
    return CT_BAD_PROJECTION;
    }
if (ARE_SAME(project_projection,map_projection))
    {
    *x_coord_out = x_coord_in;
    *y_coord_out = y_coord_in;
    status=SUCCESS;
    }
else
    {
    status = ct_get_projection(project_projection,&project_data);
    if (status EQUALS SUCCESS)
	{
	status = ct_get_projection(map_projection,&map_data);
	if (status EQUALS SUCCESS)
	    {
	    status=ct_transform_point(x_coord_in,y_coord_in,project_data,
				      x_coord_out,y_coord_out,map_data);
	    }
	}
    }

return status;
}
