/* DEC/CMS REPLACEMENT HISTORY, Element SX_PICK_LINE.C*/
/* *6    17-AUG-1990 22:22:22 VINCE "(SPR 5644) Code Cleanup"*/
/* *5     1-NOV-1989 10:37:27 GILLESPIE "(SPR 30) Make data types consistent"*/
/* *4    11-AUG-1989 20:24:30 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *3    25-JUL-1989 20:13:58 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:04:06 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:30:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_PICK_LINE.C*/
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
#include "esi_so.h"

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to software pick a seismic line      
    from given x, y coordinates.  Returns the   
    line name of the selected line.  This        
    subroutine has to be reviewed for more than  
    one cell in GIS.                     

Prototype:
    publicdef INT sx_pick_line(SO_STRUCT *pso, DOUBLE x, DOUBLE y, FULL_LINE_NAME *line_id_struct, 
        INT *segment_id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)
    x               -(DOUBLE)
    y               -(DOUBLE)
    line_id_struct  -(FULL_LINE_NAME *)
    segment_id      -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_pick_line (SO_STRUCT *pso, DOUBLE x, DOUBLE y,
                            FULL_LINE_NAME *line_id_struct, INT *segment_id)
#else
publicdef INT sx_pick_line (pso, x, y, line_id_struct, segment_id)
SO_STRUCT *pso;
DOUBLE x;
DOUBLE y;
FULL_LINE_NAME *line_id_struct;
INT *segment_id;
#endif
    {
    INT status, i;
    UINT dim_list[2];
    DOUBLE *var_list[2];
    DOUBLE min[2], d_xmin, d_ymin;
    DOUBLE max[2], d_xmax, d_ymax;
    UINT point, stroke;
    size_t nbytes;
    FLOAT wcsxmin, wcsxmax, wcsymin, wcsymax;   /* world coords */
    FLOAT nwcxmin, nwcxmax, nwcymin, nwcymax;   /* normalized window coords */
    SX_WORKSPACE *w;
    SX_USER_STRUCT user_struct;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    
    stroke = point = -1;
    
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *) & w);
    
    /* fetch the line names from the select list */
    
    wmtinq (pmap->map_tnr, &nwcxmin, &nwcxmax, &nwcymin, &nwcymax, &wcsxmin, &wcsxmax,
            &wcsymin, &wcsymax);
    
    d_xmin = wcsxmin;               /* current window limits */
    
    d_xmax = wcsxmax;
    d_ymin = wcsymin;
    d_ymax = wcsymax;
    
    min[0] = d_xmin;
    min[1] = d_ymin;
    max[0] = d_xmax;
    max[1] = d_ymax;
    
    /* now find the closest point */
    var_list[0] = &x;
    var_list[1] = &y;
    
    /* set up min and max arrays */
    dim_list[0] = 2;
    dim_list[1] = 3;
    status = nl_pick_stroke_range (w->temp_nlist, 2, dim_list, var_list, min, max,
                                   &stroke, &point);
    if (stroke > 0)
        {
        status = nl_set_current_point (w->temp_nlist, point, stroke);
        /* retrieve usr structure of stroke to get line_id_struct */
        status = nl_inq_stroke_user_struct (w->temp_nlist, stroke,
                                            sizeof(SX_USER_STRUCT),
                                            (VOIDPTR) & user_struct, &nbytes);
        *line_id_struct = user_struct.line_name;
        *segment_id = user_struct.segment_id;
        }
    else
        {
        tc_zeroes ((VOIDPTR)line_id_struct, sizeof(FULL_LINE_NAME));
        *segment_id = -1;
        }
    return SUCCESS;
    }
/* END:     */
