/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_HTERM.C */
/* *4    24-SEP-1990 17:20:51 GILLESPIE "Beta Deltas" */
/*  3A1   4-SEP-1990 13:00:27 GILLESPIE "Seismic overlay deltas" */
/* *3    26-JUL-1990 17:19:09 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting" */
/* *2    24-JUL-1990 17:42:39 GILLESPIE "(SPR 1)  New code from Charlie Pumroy" */
/* *1    24-JUL-1990 16:47:18 GILLESPIE "Post value of horizon at stroke termination" */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_HTERM.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_HTERM.C*/
/* *2    31-AUG-1990 18:27:15 SCD "(SPR 5750) Fix memory overwrite bugs, use fault tics if option selected"*/
/* *1    13-AUG-1990 17:11:35 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_HTERM.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_so.h"

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to draw horizon terminator symbol, number and data value.

Prototype:
    publicdef INT so_draw_horz_terminators (SO_STRUCT *pso, CDP first_cdp,
                                            CDP last_cdp)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)  Seismic overlay structure.
    first_cdp       -(CDP)          first cdp within map area
    last_cdp        -(CDP)          last cdp within map area

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_horz_terminators (SO_STRUCT *pso, CDP first_cdp,
                                        CDP last_cdp)
#else
publicdef INT so_draw_horz_terminators (pso, first_cdp, last_cdp)
SO_STRUCT *pso;
CDP first_cdp;
CDP last_cdp;
#endif
    {
    INT status;
    UINT i;
    UINT nstrokes, npoints;             /* number of strokes, rows in surface nlist */
    CDP cdp, *cdp_list;			/* load the end of strokes into array for display */
    DOUBLE time, *time_list;		/* currently this isn't necessary */
    INT code, *code_list;		/* currently ignored until loader does something with it */
    UINT ngot, nrows;                   /* returned values from nl calls */
    UINT nCol = 3;
    INT units[3];
    INT data_type[3];
    size_t data_size[3];                /* usual nl hocus-pocus */
    UINT col_list[3];
    VOIDPTR var_list[3];

    INT j;
    COORD tmpx, tmpy;
    DOUBLE x1, x2;
    DOUBLE y1, y2;
    DOUBLE dx, dy, ang;
    GKS_INT npts;
    GKS_REAL px[2], py[2];

    /* ---------------------------------------------------------------------- */
    /* first try, only for single horizons. Isopach,isochron on demand        */
    /* (CP) this would require looking also at top and base horizon surfaces  */
    /* first determine how many strokes in the horizon nlist                  */
    /* ---------------------------------------------------------------------- */
    status = nl_inq_nlist_int (pso->seismic_surface.nlist, NL_NLIST_NTABLES,
                               (INT *)&nstrokes);
    if (status < 0 || nstrokes == 0)
        return FAIL;

    /* ---------------------------------------------------------------------------- */
    /* for now, assume there are 2 * nstrokes terminators and the user wants all    */
    /* terminators (including beginning and end of data) (CP) this can be enhanced  */
    /* when someone bothers to provide a design (CP)                                */
    /* ---------------------------------------------------------------------------- */

    cdp_list = (CDP *)tc_alloc (nstrokes * 2 * sizeof(CDP));

    col_list[0] = 1;
    col_list[1] = 2;
    col_list[2] = 3;
    var_list[0] = (VOIDPTR)&cdp;
    var_list[1] = (VOIDPTR)&time;
    var_list[2] = (VOIDPTR)&code;
    units[0] = 0;
    units[1] = 0;
    units[2] = 0;
    data_size[0] = sizeof(CDP);
    data_size[1] = sizeof(DOUBLE);
    data_size[2] = sizeof(INT);
    data_type[0] = INT_TYPE;
    data_type[1] = FLOAT_TYPE;
    data_type[2] = INT_TYPE;

    for (npoints = 0, i = 1; i <= nstrokes; i++)
        {

        /* ---------------------------------------------------------------- */
        /* get start of stroke. note if this is stroke 1 we could ignore it,*/ 
        /* and not put it in the list (if user doesn't want endpoints noted */
        /* ---------------------------------------------------------------- */
        status = nl_set_current_row (pso->seismic_surface.nlist, 1, i);
        status = nl_get_rows_convert (pso->seismic_surface.nlist, 1, nCol,
                                      col_list, var_list, units, data_size,
                                      data_type, &ngot);
        if ( cdp >= first_cdp && cdp <= last_cdp )
            {       
	    npoints++;
            cdp_list[npoints-1] = cdp;            
	    }

        /* ----------------------------------------------------------------------------- */
        /* we now have the first row of this stroke. Now get the last row of this stroke */
        /* we determine how many rows in this stroke, and then go there                  */
        /* ----------------------------------------------------------------------------- */
        status = nl_inq_table_int (pso->seismic_surface.nlist, i,
                                   NL_TABLE_NROWS, (INT *)&nrows);
        status = nl_set_current_row (pso->seismic_surface.nlist, nrows, i);
        status = nl_get_rows_convert (pso->seismic_surface.nlist, 1, nCol,
                                      col_list, var_list, units, data_size,
                                      data_type, &ngot);
        if ( cdp >= first_cdp && cdp <= last_cdp )
            {       
            npoints++;
            cdp_list[npoints-1] = cdp;            
	    }
        }

    /* ---------------------------------------------------- */
    /* Reset the labeling context (up vector, etc.) because */
    /* the "enhanced labeling code" does not correctly      */
    /* compute the angle for the end point labels           */
    /* ---------------------------------------------------- */

    so_set_shotpoint_defaults ( pso );
    
    /* ------------------------------------------------------------------------ */
    /* first we draw the shot number (based upon the list created above).       */
    /* ------------------------------------------------------------------------ */
        
    so_draw_shot_labels (pso, cdp_list, npoints);

    /* ------------------------------------------------------------------- */
    /* now draw the data labels. The elegant approach would be to not make */
    /* so_draw_data_labels hunt up the data again (so_zvalue calls, etc.   */
    /* ------------------------------------------------------------------- */
        
    so_draw_data_labels (pso, cdp_list, npoints);
    
    /* --------------------------------------- */
    /* If user has not selected FAULTS option, */
    /* we need to draw the shot symbols.       */
    /* --------------------------------------- */

    if ( !pso->draw_fault_throw )
	{
	so_draw_symbols (pso, cdp_list, npoints);
	}

    tc_free ((VOIDPTR)cdp_list);
    return SUCCESS;
    }
/* END:     */
