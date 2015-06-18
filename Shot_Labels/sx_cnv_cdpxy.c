/* DEC/CMS REPLACEMENT HISTORY, Element SX_CNV_CDPXY.C*/
/* *5    17-AUG-1990 22:20:13 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    17-JAN-1990 15:40:35 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    11-AUG-1989 14:18:03 JULIAN "(SPR -1) illegal pointer combination"*/
/* *2    25-JUL-1989 20:10:25 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_CNV_CDPXY.C*/
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

#include "esi_sx.h"
#include "esi_tc.h"
#include "esi_nl.h"

/* Function Description -----------------------------------------------------
Description:
    Convert cdpxy nlist to float shot from int cdp.

Prototype:
    publicdef INT sx_convert_cdpxy(SX_STRUCT *psx, NLIST_HEADER *converted_cdp_xy);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx                 -(SX_STRUCT *)
    converted_cdp_xy    -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_convert_cdpxy (SX_STRUCT *psx, NLIST_HEADER *converted_cdp_xy)
#else
publicdef INT sx_convert_cdpxy (psx, converted_cdp_xy)
SX_STRUCT *psx;
NLIST_HEADER *converted_cdp_xy;
#endif
    {
    INT status;
    INT units[3];                   /* nlist init stuff */
    INT value_inc[3];
    INT data_types[3];
    size_t data_sizes[3];
    UINT npoints;                   /* nlist attribute stuff */
    UINT nstrokes;
    UINT istroke;
    UINT stroke;
    INT i;
    UINT ngot;
    FLOAT *xs, *ys, *fshots;
    CDP *fcdps;
    CDP_FLOAT fshot;
    UINT dim_list[3];
    VOIDPTR value_list[3];
    static CHAR *c_name[4] = 
        {
        "CDP",
        "X",
        "Y",
        NULL
        };
    UINT d_list[3];
    UINT last_size = 0;
    
    /* initialize new nlist */
    
    units[0] = units[1] = units[2] = 0;
    value_inc[0] = value_inc[1] = value_inc[2] = VALUE_LIST;
    data_types[0] = data_types[1] = data_types[2] = FLOAT_TYPE;
    data_sizes[0] = data_sizes[1] = data_sizes[2] = sizeof(FLOAT);
    d_list[0] = 1;
    d_list[1] = 2;
    d_list[2] = 3;
    
    nl_init_nlist (converted_cdp_xy, 3, units, data_sizes, data_types, value_inc, 0);
    status = nl_set_nlist_info (*converted_cdp_xy, NL_NLIST_COLUMN_NAME, 3, d_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    /* see how many strokes */
    
    nl_inq_nlist_int (psx->pso->cdp_xy, NL_NLIST_NSTROKES, (INT *) & nstrokes);
    
    /* reformat the nlist */
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    for (istroke = 0; istroke < nstrokes; istroke++)
        {
        
        /* if no points in stroke, skip it */
        nl_inq_stroke_int (psx->pso->cdp_xy, istroke + 1, NL_STROKE_NPOINTS,
                           (INT *) & npoints);
        if (npoints == 0)
            {
            continue;
            }
        else if (npoints > last_size)
            {
            if (last_size > 0)
                {
                fcdps = (CDP *)tc_realloc (fcdps, npoints * sizeof(CDP));
                xs = (FLOAT *)tc_realloc (xs, npoints * sizeof(FLOAT));
                ys = (FLOAT *)tc_realloc (ys, npoints * sizeof(FLOAT));
                fshots = (FLOAT *)tc_realloc (fshots, npoints * sizeof(FLOAT));
                }
            else
                {
                fcdps = (CDP *)tc_alloc (npoints * sizeof(CDP));
                xs = (FLOAT *)tc_alloc (npoints * sizeof(FLOAT));
                ys = (FLOAT *)tc_alloc (npoints * sizeof(FLOAT));
                fshots = (FLOAT *)tc_alloc (npoints * sizeof(FLOAT));
                }
            last_size = npoints;
            }
        /* get points */
        nl_set_current_point (psx->pso->cdp_xy, 1, istroke + 1);
        nl_start_stroke (*converted_cdp_xy, &stroke);
        
        value_list[0] = (VOIDPTR)fcdps;
        value_list[1] = (VOIDPTR)xs;
        value_list[2] = (VOIDPTR)ys;
        data_types[0] = INT_TYPE;
        data_sizes[0] = sizeof(INT);
        
        nl_get_points_convert (psx->pso->cdp_xy, npoints, 3, dim_list, value_list,
                               units, data_sizes, data_types, &ngot);
        for (i = 0; i < ngot; i++)
            {
            
            /* convert to shotpoint */
            sx_sp_from_cdp (psx, fcdps[i], &fshot);
            fshots[i] = fshot;
            }
        /* add them */
        
        value_list[0] = (VOIDPTR)fshots;
        value_list[1] = (VOIDPTR)xs;
        value_list[2] = (VOIDPTR)ys;
        nl_add_points (*converted_cdp_xy, ngot, 3, dim_list, value_list);
        }
    /* free arrays */
    
    tc_free (fcdps);
    tc_free (fshots);
    tc_free (xs);
    tc_free (ys);
    
    status = SUCCESS;
    SX_RETURN;
    }
/* END:     */
