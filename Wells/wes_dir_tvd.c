/* DEC/CMS REPLACEMENT HISTORY, Element WES_DIR_TVD.C*/
/* *6    26-AUG-1991 09:55:39 JTM "(SPR 0) Fixes for deviated well traces, by Charlie."*/
/* *5     1-MAY-1991 22:51:23 KEE "(SPR -1) Landmark Import & Export"*/
/* *4    14-AUG-1990 13:38:56 VINCE "(SPR 5644) Header Standardization"*/
/* *3    18-JAN-1990 08:54:35 GILLESPIE "(SPR 6001) Fix for new wells struct"*/
/* *2    17-NOV-1989 16:09:01 CONROY "(SPR 5059) Change dev flag to Y/N"*/
/* *1    19-JUN-1989 13:35:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WES_DIR_TVD.C*/
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
#include "esi_we_wells.h"
#include "esi_we_err.h"
#include "esi_we_msg.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    This function gets dx, dy, tvd for a well given its uwi and a measured
            depth.

    If the well is not deviated, then dx = dy = 0
    and tvd = md.

    If the well is deviated, then either get the detailed
    survey, if available, and return the best possible
    value, or if not available then perform an
    interpolation between the top and bottom hole
    positions

    METHODOLOGY:

    1) Fetch the well from the database.  The WELL_STRUCT contains
    information about the location of the well, including whether
    the well is deviated (pWell->deviation.flag), and the name
    of the n-List which contains the directional survey.

    2) Test to see if the well is deviated.  The deviation.flag
    can have two meaningful values: '+' means that a directional
    survey exists; '-' means that the well is deviated, but no official
    directional survey information exists.  Anything else is
    interpreted to represent a non-deviated well.

    3) For non-deviated wells, return dx = dy = 0 and tvd = md.

    4) For deviated wells, first check to see if the n-List has been
    initialized.  If not, read the n-List file.  If that failed, then
    construct the n-List by reading the values from the table
    WELL_DIRECTIONAL_SURVEY.  If no values are found, then treat the
    well as if it was deviated, but no survey exists.

    5) For deviated wells that have an n-List available, scan the
    list either forwards or backwards until the two points which
    span the request measured depth are located.  If the measured
    depth is shallower than the first station, interpolate from
    the top hole position.  If the measured depth is deeper than
    the last available station, use the last two points to extrapolate
    to the requested point.

    The n-List has four dimensions: md, dx, dy and tvd.  Each dimension
    is a floating point value (4 bytes wide) stored in a single stroke,
    and sorted on measured depth.

    6) For deviated wells with no survey available, use the fields
    BASE_DELTA_X, BASE_DELTA_Y and TRUE_VERTICAL_DEPTH in the WELL_HDR
    table to calculate the requested point, assuming a simple linear
    path between the top hole and the specified bottom hole.


Prototype:
    publicdef INT wes_get_dir_tvd(UWI uwi, DOUBLE in_md, DOUBLE *dx, DOUBLE *dy, 
    DOUBLE *tvd);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uwi             -(UWI)
    in_md           -(DOUBLE)
    dx              -(DOUBLE *)
    dy              -(DOUBLE *)
    tvd             -(DOUBLE *)

Return Value/Status:
    SUCCESS - Successful completion.
    WE_NOT_DEVIATED
    WE_INCOMPLETE_SURVEY
    WE_NO_SUCH_WELL
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wes_get_dir_tvd (UWI uwi, DOUBLE in_md, DOUBLE *dx, DOUBLE *dy,
                               DOUBLE *tvd) 
#else
publicdef INT wes_get_dir_tvd (uwi, in_md, dx, dy, tvd)
UWI uwi;
DOUBLE in_md;
DOUBLE *dx;
DOUBLE *dy;
DOUBLE *tvd;
#endif
    {
    INT status;
    WELL_STRUCT *pWell;
    DEPTHS above;
    DEPTHS current;
    DOUBLE frac;
    UINT npoints;
    UINT ndim;
    UINT Dim_List[4];
    VOIDPTR Value_List[4];
    NLIST_HEADER nlist;
    
    /*  get the well data structure, that has at a
        minimum the X, Y and deviation survey file
        name filled in                  */
    
    status = we_get_well_header (uwi, WE_LOCATION_ONLY, &pWell);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        *dx = *dy = 0.0;
        *tvd = in_md;
        return WE_NO_SUCH_WELL;
        }
    /*  check to see if the well is deviated        */
    
    switch (pWell->deviation.flag)
        {
    case 'Y':
        
        Dim_List[0] = 1, Value_List[0] = (VOIDPTR) & current.md;
        Dim_List[1] = 2, Value_List[1] = (VOIDPTR) & current.dx;
        Dim_List[2] = 3, Value_List[2] = (VOIDPTR) & current.dy;
        Dim_List[3] = 4, Value_List[3] = (VOIDPTR) & current.tvd;
        ndim = 1;
        
        if (pWell->deviation.nlist EQUALS (NLIST_HEADER)0)
            {
            /*  this path is acheieved on the first access
                request                     */
            
            status = wes_load_dir_nlist (uwi, &nlist);
            
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                WELL_HDR_1 *pwell1;
                
                /*  OK - we've tried reading it from the file -
                    that failed.  We tried getting it from the
                    table - that failed.  There's not much else
                    we can do, so fall back on whatever is
                    stored in the WELL_HDR table (i.e. treat it
                    as if the well had no directional survey
                    stored                      */
                
                status = we_get_well_header (uwi, WELL_HDR_1_DETAIL, &pWell);
                tc_zeroes (&above, sizeof above);
                
                pwell1 = (WELL_HDR_1 *)pWell->detail.ptr;
                current.md = pwell1->td.driller.md;
                current.dx = pWell->bottomhole_xy.x - pWell->tophole_xy.x;
                current.dy = pWell->bottomhole_xy.y - pWell->tophole_xy.y;
                current.tvd = pwell1->td.driller.tvd;
                status = WE_INCOMPLETE_SURVEY;
                break;
                }
            status = nl_set_current_point (pWell->deviation.nlist, 1, 1);
            }
        /*  set up the n-List retrieval         */
        
        nlist = pWell->deviation.nlist;
        
        /*  retrieve the measured depth of the current  */
        /*  point                       */
        
        status = nl_get_points (nlist, 1, ndim, Dim_List, Value_List, &npoints);
        
        /*  back up until you find a measured depth <=  */
        /*  the desired measured depth          */
        
        while (status >= 0 AND NOT (status & NL_START_OF_STROKE) AND current.md > in_md)
            {
            status = nl_previous_point (nlist, ndim, Dim_List, Value_List);
            }
        if (status >= 0 AND (current.md > in_md))
            {
            
            /*  this case is reached when the deviation
                survey does not go up far enough to span
                the upper point.  Interpolate between the
                highest point available and the top hole
                position to get the value           */
            
            ndim = 4;
            nl_get_points (nlist, 1, ndim, Dim_List, Value_List, &npoints);
            tc_zeroes (&above, sizeof above);
            status = WE_INCOMPLETE_SURVEY;
            break;
            }
        /*  OK.  we're at a point that exists somewhere
            above the requested point.  now scan down
            until we find a point which exists on the
            other side of the requested point       */
        
	/* (CP) back up one row just in case the md is between the
	first and second records since the current row is now at least
	row 2 (may cause an extra nl_next_point call but it sets
	the current row properly when the data is above the second row) */
        nl_previous_point (nlist, ndim, Dim_List, Value_List);
        
        for (status = 0;
             status >= 0 AND NOT (status & NL_END_OF_STROKE) AND current.md < in_md; )
            {
            above = current;
            status = nl_next_point (nlist, ndim, Dim_List, Value_List);
            }
        if (status >= 0 AND (current.md < in_md))
            {
            /*  this case is reached when the deviation
                survey does not go down far enough to span
                the data point.  Extrapolate between the
                last two survey pointsto get the value      */
            
            status = WE_INCOMPLETE_SURVEY;
            }
        else
            {
            status = SUCCESS;
            }
        /*  now that we have found the two points which
            span the requested depth, retrieve the
            actual data points.  Note that this
            mechanism allows for the fewest number of
            bytes to flow during the search phase       */
        
        ndim = 4;
        nl_previous_point (nlist, ndim, Dim_List, Value_List);
        above = current;
        nl_next_point (nlist, ndim, Dim_List, Value_List);
        break;
        
    default:                        /*  well not deviated               */
        *dx = *dy = 0.0;
        *tvd = in_md;
        return WE_NOT_DEVIATED;
        }
    /*  The ABOVE structure contains a point above
        or at the request measured depth The
        CURRENT structure contains a point at or
        below the requested depth           */
    
    if (current.md EQUALS above.md)
        {
        *dx = current.dx;
        *dy = current.dy;
        *tvd = current.tvd;
        }
    else
        {
        frac = (current.md - in_md) / (current.md - above.md);
        *dx = current.dx - frac * (current.dx - above.dx);
        *dy = current.dy - frac * (current.dy - above.dy);
        *tvd = current.tvd - frac * (current.tvd - above.tvd);
        }
    return status;
    }
/* END:     */
