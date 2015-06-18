/* DEC/CMS REPLACEMENT HISTORY, Element SE_LN_CDP_XY.C */
/* *4    17-AUG-1990 22:17:26 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:08:39 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:07:01 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:48:08 JULIAN " seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_LN_CDP_XY.C */
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

#include "esi_se.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT func (arg);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
FULL_LINE_NAME *seis_line;
SEIS_HILIGHT_TYPE flag;
COORD wx, wy;
CDP *cdp;
COORD *x, *y;
    (I) seis_line   -(FULL_LINE_NAME *) Seis_line is the structure composed of the
                        line name and the survey name.
    (I) flag        -(SEIS_HILIGHT_TYPE) PICK_XY if the input is an X Y and
                        the output CDP , X , Y FIRST_CDP if the
                        output is the first cdp on the line and the
                        X Y of this cdp LAST_CDP if the output is
                        the last cdp on the line and the X Y of
                        this cdp wx , wy x and y coord of a point.
    (O) cdp         -(CDP *) Coord of the cdp retrieved.
    (O) x           -(COORD *) -\
    (O) y           -(COORD *) 'True' coordinates of the cdp
                        retrieved.
    wx              -(COORD)
    wy              -(COORD)
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_line_cdp_from_xy (FULL_LINE_NAME *seis_line, SEIS_HILIGHT_TYPE flag,
                                   COORD wx, COORD wy, CDP *cdp, COORD *x, COORD *y)
#else
publicdef INT se_line_cdp_from_xy (seis_line, flag, wx, wy, cdp, x, y)
FULL_LINE_NAME *seis_line;
SEIS_HILIGHT_TYPE flag;
COORD wx, wy;
CDP *cdp;
COORD *x, *y;
#endif
    {
    
    CDP first_cdp, last_cdp, cdp_int;
    INT status;
    CDP_FLOAT cdp_float;
    DOUBLE cdp_dist;
    COORD x_proj, y_proj, x_end, y_end;
    
    /*  cdp_dist : inter cdp distance which can
        come from the trace files - if the
        distance is > 0 we take this distance as
        inter cdp distance if it is < 0 we take as
        inter_cdp distance the distance computed
        from the SP spacing we set it to negative
        in this case                               */
    
    cdp_dist = -1.;
    
    if (flag != PICK_XY)
        
        /* case where we want the first or the last    */
        /* cdp                                         */
        {
        status = se_first_last_cdp (seis_line, &first_cdp, &last_cdp);
        
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        if (flag EQUALS FIRST_CDP)
            {
            *cdp = first_cdp;
            }
        else
            {
            *cdp = last_cdp;
            }
        }
    else                            /* normal case retrieve cdp from x y coord     */
        {
        
        /* look for the closest cdp on the line using */
        /* the wx , wy coord */
        
        status = se_cdp_from_xy (seis_line, wx, wy, cdp_dist, &cdp_int, &cdp_float,
                                 &x_proj, &y_proj, cdp, &x_end, &y_end);
        
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /* retrieve the true coordinate of the cdp     */
    
    status = se_xy_from_cdp (seis_line, *cdp, cdp_dist, x, y);
    return status;
    }
/* END:     */
