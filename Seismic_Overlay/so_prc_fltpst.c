/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_FLTPST.C */
/* *11   21-DEC-1990 18:25:59 VINCE "(SPR 6174) fixed half fault processing to pass two points to so_draw_fault_posts" */
/* *10    7-DEC-1990 14:51:54 VINCE "(SPR 1) Fixed up the C formatting" */
/*  6A1B1  3-OCT-1990 11:52:37 GILLESPIE "Change file type to stream" */
/* *9    29-SEP-1990 11:32:38 GILLESPIE "(SPR 1) Beta Deltas" */
/*  6A1  29-SEP-1990 11:31:43 GILLESPIE "Beta Deltas" */
/* *8    20-AUG-1990 17:11:24 KEE "(SPR -1) fixing compile error at sun" */
/* *7    14-AUG-1990 22:04:25 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs" */
/* *6    26-JUL-1990 17:21:54 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting" */
/* *5    25-JUL-1990 20:05:38 VINCE "(SPR 5644) Header Standardization" */
/* *4    18-JUL-1990 18:34:17 KEE "(SPR -1) Bug fixing change fault_throw_label to fault_throw at so_draw_fault_posts call" */
/* *3     1-JUL-1990 13:34:12 GILLESPIE "(SPR 1) Fix data type of flag variables" */
/* *2    30-JUN-1990 10:26:28 GILLESPIE "(SPR 1) Replacement for old .pc code" */
/* *1    30-JUN-1990 10:22:37 GILLESPIE "New seismic fault map view posting code (uses structures in horizon n-List)" */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_FLTPST.C */
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
#include "esi_so.h"

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Function to process fault posts.

    Methodology
    
    Fauts are modeled as gaps between seismic horizon segments.  To annotate a
    horizon, we walk through a seismic horizon, paying attention only to the
    ends of each segment.
    
    The user structure of each horizon contains an optional fault structure
    which names the fault at each end.  Additionally, a one byte flag is stored
    with each 'pick' that describes in more detail the nature of the
    termination.
    
    We demark each end point and additionally compute the throw of the fault
    where enough information exists.  Note that this requires special handling
    for the first and last end points (*)...
    
    
    *---------------x
                     \ 18
                      x-----------------x                      x--------------*
                                         \ 20                 / 17
                                          x------------------x

Prototype:
    publicdef INT so_process_fault_posts(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_process_fault_posts (SO_STRUCT *pso)
#else
publicdef INT so_process_fault_posts (pso)
SO_STRUCT *pso;
#endif
    {
    INT status;
    INT fault_throw;
    COORD ix, iy, fx, fy;
    UINT total_tables;
    UINT nRows;
    size_t returnedLength;
    DOUBLE cdp_dist = -1.0;
    CDP cdp;
    UINT nCol = 3;
    UINT theTable;
    UINT Col_List[3];
    VOIDPTR Var_List[3];
    INT Units[3];
    size_t Widths[3];
    INT Types[3];
    DOUBLE value;
    TINY flag;
    SEIS_HOR_PICK_TYPE iflag, fflag;
    DOUBLE fvalue, ivalue;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    SEIS_SURFACE_HDR *pSurface = &pso->seismic_surface;
    SEIS_SURFACE_CONTACTS FaultNames;
    SEIS_SURFACE_NAME iFaultName;
    SEIS_SURFACE_NAME fFaultName;
    NLIST_HEADER nlist;
    
    status = SUCCESS;
    
    /* see if drawing faults */
    
    if (NOT pso->draw_fault_throw)
        {
        return status;
        }

    if (IS_EMPTY_STRING (pSurface->name))
        {
        return status;
        }

    nlist = pSurface->nlist;
    
    Col_List[0] = 1;
    Widths[0]   = sizeof cdp;
    Types[0]    = INT_TYPE;
    Units[0]    = 0;
    Var_List[0] = (VOIDPTR) & cdp;
    
    Col_List[1] = 2;
    Widths[1]   = sizeof value;
    Types[1]    = FLOAT_TYPE;
    Units[1]    = 0;
    Var_List[1] = (VOIDPTR) & value;
    
    Col_List[2] = 3;
    Widths[2]   = sizeof flag;
    Types[2]    = INT_TYPE;
    Units[2]    = 0;
    Var_List[2] = (VOIDPTR) & flag;
    
    wm_gsplci (pso->fault_line.line.color);
    wm_gstxci (pso->fault_label.text_color);
    wm_gschh  (pso->fault_label.text_size);

    /* Get the symbol and fault name of the first (half) fault */
    theTable = 1;
    tc_zeroes (&FaultNames, sizeof FaultNames);
    status = nl_inq_table_user_struct (nlist, theTable, sizeof FaultNames,
                                       (VOIDPTR)&FaultNames, &returnedLength);
    status = nl_set_current_row (nlist, 1, theTable);
    if (status >= SUCCESS)
        {
        nl_get_rows_convert (nlist, 1, nCol, Col_List, Var_List, Units,
                             Widths, Types, &nRows);
        
        se_xy_from_cdp (&pso->current_line, cdp, cdp_dist, &ix, &iy);
        
        ct_map_xy (ix, iy, &ix, &iy);
        
        if (ix >= pmap->lower_left_xy.x AND ix <= pmap->upper_right_xy.x AND
            iy >= pmap->lower_left_xy.y AND iy <= pmap->upper_right_xy.y   )
            {
            iflag = (SEIS_HOR_PICK_TYPE)flag;
	    ivalue = value;

	    /* ------------------------------------------------ */
	    /* get the next XY just to give so_draw_fault_posts */
	    /* the angle of the line.				*/
	    /* ------------------------------------------------ */

            fflag = (SEIS_HOR_PICK_TYPE) -1;
	    fvalue = 0.0;

	    nl_get_rows_convert (nlist, 1, nCol, Col_List, Var_List, Units,
				 Widths, Types, &nRows);

	    se_xy_from_cdp (&pso->current_line, cdp, cdp_dist, &fx, &fy);
	    ct_map_xy (fx, fy, &fx, &fy);

            so_draw_fault_posts (ix, iy, iflag, FaultNames.initial, fx, fy,
				 fflag, FaultNames.final, ivalue,
				 pso->fault_label.text_size);
            }
        }

    nl_inq_nlist_int (nlist, NL_NLIST_NTABLES, (INT *) & total_tables);
    
    while (theTable < total_tables)
        {
        /* Get the symbol and fault name of the last fault */
        status = nl_inq_table_int (nlist, theTable, NL_TABLE_NROWS,
				   (INT *)&nRows);
        status = nl_set_current_row (nlist, nRows, theTable);
        if (status < SUCCESS)
            {
            break;
            }

        nl_get_rows_convert (nlist, 1, nCol, Col_List, Var_List, Units,
			     Widths, Types, &nRows);
        se_xy_from_cdp (&pso->current_line, cdp, cdp_dist, &fx, &fy);
        
        ct_map_xy (fx, fy, (DOUBLE *) & fx, (DOUBLE *) & fy);
        
        if (fx < pmap->lower_left_xy.x OR fx > pmap->upper_right_xy.x OR
	    fy < pmap->lower_left_xy.y OR fy > pmap->upper_right_xy.y  )
            {
            fflag = (SEIS_HOR_PICK_TYPE) -1, fvalue = 0.0, fFaultName[0] = 0;
            }
        else
            {
            fflag = (SEIS_HOR_PICK_TYPE)flag, fvalue = value,
                    strcpy (fFaultName, FaultNames.final);
            }

        /* Now process the (left) part of the next segment */
        tc_zeroes (&FaultNames, sizeof FaultNames);
        theTable++;
        status = nl_inq_table_user_struct (nlist, theTable, sizeof FaultNames,
                                           &FaultNames, &returnedLength);
        
        /* Get the symbol and fault name of the first fault */
        status = nl_set_current_row (nlist, 1, theTable);
        if (status < SUCCESS)
            {
            continue;
            }
        nl_get_rows_convert (nlist, 1, nCol, Col_List, Var_List, Units,
			     Widths, Types, &nRows);
        
        se_xy_from_cdp (&pso->current_line, cdp, cdp_dist, &ix, &iy);
        
        ct_map_xy (ix, iy, (DOUBLE *) & ix, (DOUBLE *) & iy);
        
        if (ix < pmap->lower_left_xy.x OR ix > pmap->upper_right_xy.x OR
	    iy < pmap->lower_left_xy.y OR iy > pmap->upper_right_xy.y  )
            {
            iflag = (SEIS_HOR_PICK_TYPE) -1, ivalue = 0.0;
            }
        else
            {
            iflag = (SEIS_HOR_PICK_TYPE)flag, ivalue = value,
                    strcpy (iFaultName, FaultNames.initial);
            }

        if (fflag EQUALS (SEIS_HOR_PICK_TYPE) -1 AND
	    iflag EQUALS (SEIS_HOR_PICK_TYPE) -1   )
            {
            fault_throw = 0;
            }
        else
            {
            fault_throw = fvalue - ivalue;
            }

        so_draw_fault_posts (ix, iy, iflag, FaultNames.initial, fx, fy, fflag,
                             FaultNames.final, fault_throw,
                             pso->fault_label.text_size);
        }

    /* -------------------------------------------------------------------- */
    /* Handle the LAST horizon endpoint; get the symbol and fault name of   */
    /* the last fault							    */
    /* -------------------------------------------------------------------- */

    status = nl_inq_table_int (nlist, theTable, NL_TABLE_NROWS,
			       (INT *)&nRows);
    status = nl_set_current_row (nlist, nRows, theTable);
    if (status < SUCCESS)
        {
        return status;
        }

    nl_get_rows_convert (nlist, 1, nCol, Col_List, Var_List, Units,
			 Widths, Types, &nRows);

    se_xy_from_cdp (&pso->current_line, cdp, cdp_dist, &fx, &fy);
    
    ct_map_xy (fx, fy, (DOUBLE *) & fx, (DOUBLE *) & fy);
    
    if (fx >= pmap->lower_left_xy.x AND fx <= pmap->upper_right_xy.x AND
	fy >= pmap->lower_left_xy.y AND fy <= pmap->upper_right_xy.y   )
        {
	fvalue = value;
        fflag  = (SEIS_HOR_PICK_TYPE)flag;

	/* ---------------------------------------------------- */
	/* get the previous XY just to give so_draw_fault_posts */
	/* the angle of the line.				*/
	/* ---------------------------------------------------- */

        iflag  = (SEIS_HOR_PICK_TYPE) -1;
	ivalue = 0.0;

	status = nl_inq_table_int (nlist, theTable, NL_TABLE_NROWS,
				   (INT *)&nRows);
	status = nl_set_current_row (nlist, --nRows, theTable);

	nl_get_rows_convert (nlist, 1, nCol, Col_List, Var_List, Units,
			     Widths, Types, &nRows);

	se_xy_from_cdp (&pso->current_line, cdp, cdp_dist, &fx, &fy);
	ct_map_xy (fx, fy, &fx, &fy);

        so_draw_fault_posts (ix, iy, iflag, FaultNames.initial, fx, fy, fflag,
                             FaultNames.final, fvalue,
                             pso->fault_label.text_size);
        }

    return status;
    }
/* END:     */
