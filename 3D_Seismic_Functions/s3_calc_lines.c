/* DEC/CMS REPLACEMENT HISTORY, Element S3_CALC_LINES.C*/
/* *5    17-AUG-1990 22:13:30 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    18-JUN-1990 12:05:58 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *3    17-JAN-1990 15:38:11 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *2    28-JUL-1989 12:16:52 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:23:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_CALC_LINES.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_CALC_LINES.C                             */
/* *3    17-JAN-1990 15:38:11 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"   */
/* *2    28-JUL-1989 12:16:52 GILLESPIE "(SPR 1) Fix minor casting problems"        */
/* *1    19-JUN-1989 13:23:00 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element S3_CALC_LINES.C                             */
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

#include "esi_s3.h"
#include "esi_math.h"

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This routine is designed to calculate the locations of CDPs and 
    LINEs by given GRID_3D structure.

Prototype:
    publicdef INT s3_calc_cdps_lines(GRID_3D *grid_3d, NLIST_HEADER *cdp_nlist,
                                 NLIST_HEADER *line_nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) grid_3d     -(GRID_3D *) The structure of GRID_3D of specified survey.
    (O) cdp_nlist   -(NLIST_HEADER *) The returned nlist of CDP lines. The returned
                        format is CDP, X(lower line), Y(lower line),
                        X(upper line), Y(upper line). 5-dims.
    (O) line_nlist  -(NLIST_HEADER *) The returned nlist of LINE lines.  The returned
                        format is LINE, X(lower cdp), Y(lower cdp),
                        X(upper cdp), Y(upper cdp).

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT s3_calc_cdps_lines (GRID_3D *grid_3d, NLIST_HEADER *cdp_nlist,
                                  NLIST_HEADER *line_nlist)
#else
publicdef INT s3_calc_cdps_lines (grid_3d, cdp_nlist, line_nlist)
GRID_3D *grid_3d;
NLIST_HEADER *cdp_nlist, *line_nlist;
#endif
    {
    INT status = SUCCESS;
    UINT cdp_index, line_index, table_num;
    INT i;
    DOUBLE const1, const2;
    DOUBLE PI2 = PI / 2.0;
    FLOAT x1, x2, y1, y2;
    DOUBLE delta, tx[4];
    UINT ndim = 5;
    static UINT dim_list[5] = 
        {
        1,
        2,
        3,
        4,
        5
        };
    static INT units[5] = 
        {
        0,
        0,
        0,
        0,
        0
        };
    static CHAR *c_name[6] = 
        {
        "CDP",
        "LOWER_X",
        "LOWER_Y",
        "UPPER_X",
        "UPPER_Y",
        NULL
        };
    static INT value_incs[5] = 
        {
        INCREMENT_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST
        };
    static size_t data_sizes[5] = 
        {
        sizeof(INT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT)
        };
    static INT data_types[5] = 
        {
        INT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE
        };
    VOIDPTR value_list[5];
    
    /* Initialize the returned nlist                                                */
    status = nl_init_nlist (cdp_nlist, ndim, units, data_sizes, data_types, value_incs,
                            0);
    status = nl_init_nlist (line_nlist, ndim, units, data_sizes, data_types,
                            value_incs, 0);
    status = nl_set_nlist_info (*cdp_nlist, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_set_nlist_info (*line_nlist, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_start_table (*cdp_nlist, &table_num);
    status = nl_start_table (*line_nlist, &table_num);
    
    /* Set the start_cdp, and increment_cdp.                                        */
    
    value_list[0] = (VOIDPTR) & grid_3d->lower_cdp;
    status = nl_set_table_info (*cdp_nlist, 1, NL_TABLE_START_VALUE, 1, dim_list,
                                value_list);
    
    value_list[0] = (VOIDPTR) & grid_3d->cdp_inc;
    status = nl_set_table_info (*cdp_nlist, 1, NL_TABLE_INCREMENT, 1, dim_list,
                                value_list);
    
    /* Set the start_line, and increment_line.                                      */
    value_list[0] = (VOIDPTR) & grid_3d->lower_line;
    status = nl_set_table_info (*line_nlist, 1, NL_TABLE_START_VALUE, 1, dim_list,
                                value_list);
    
    value_list[0] = (VOIDPTR) & grid_3d->line_inc;
    status = nl_set_table_info (*line_nlist, 1, NL_TABLE_INCREMENT, 1, dim_list,
                                value_list);
    
    /* To set the transformation matrix.                                            */
    
    delta = grid_3d->y_azimuth - grid_3d->x_azimuth;
    if ((delta <= 1.001 * (PI2)) AND (delta >= 0.999 * (PI2)))
        {
        /* This is a standard left hand rotation.                                   */
        tx[0] = cos (grid_3d->x_azimuth);
        tx[1] = sin (grid_3d->x_azimuth);
        tx[2] = -sin (grid_3d->x_azimuth);
        tx[3] = cos (grid_3d->x_azimuth);
        
        }
    else
        {
        /* This is a standard right hand rotation.                                  */
        tx[0] = cos (grid_3d->x_azimuth);
        tx[1] = sin (grid_3d->x_azimuth);
        tx[2] = sin (grid_3d->x_azimuth);
        tx[3] = -cos (grid_3d->x_azimuth);
        }
    /* To construct the CDP_NLIST                                                   */
    cdp_index = grid_3d->lower_cdp;
    const1 = grid_3d->y_origin;
    const2 = ((grid_3d->upper_line - grid_3d->lower_line) / grid_3d->line_inc)
    *grid_3d->line_interval + const1;
    
    value_list[0] = (VOIDPTR) & cdp_index;
    value_list[1] = (VOIDPTR) & x1;
    value_list[2] = (VOIDPTR) & y1;
    value_list[3] = (VOIDPTR) & x2;
    value_list[4] = (VOIDPTR) & y2;
    
    for (i = 0; cdp_index <= grid_3d->upper_cdp; i++)
        {
        x1 = x2 = grid_3d->x_origin + grid_3d->cdp_interval * i;
        
        x1 = x1 * tx[0] + const1 * tx[2];
        y1 = x1 * tx[1] + const1 * tx[3];
        x2 = x2 * tx[0] + const2 * tx[2];
        y2 = x2 * tx[1] + const2 * tx[3];
        cdp_index += grid_3d->cdp_inc;
        i++;
        status = nl_add_rows (*cdp_nlist, 1, ndim, dim_list, value_list);
        }
    /* To construct the LINE_NLIST.                                                 */
    line_index = grid_3d->lower_line;
    const1 = grid_3d->x_origin;
    const2 = ((grid_3d->upper_cdp - grid_3d->lower_cdp) / grid_3d->cdp_inc)
    *grid_3d->cdp_interval + const1;
    
    value_list[0] = (VOIDPTR) & line_index;
    value_list[1] = (VOIDPTR) & x1;
    value_list[2] = (VOIDPTR) & y1;
    value_list[3] = (VOIDPTR) & x2;
    value_list[4] = (VOIDPTR) & y2;
    
    for (i = 0; line_index <= grid_3d->upper_line; i++)
        {
        y1 = y2 = grid_3d->y_origin + grid_3d->line_interval * i;
        
        x1 = const1 * tx[0] + y1 * tx[2];
        y1 = const1 * tx[1] + y1 * tx[3];
        x2 = const2 * tx[0] + y2 * tx[2];
        y2 = const2 * tx[1] + y2 * tx[3];
        line_index += grid_3d->line_inc;
        status = nl_add_rows (*line_nlist, 1, ndim, dim_list, value_list);
        }
    return SUCCESS;
    }
/* END:     */
