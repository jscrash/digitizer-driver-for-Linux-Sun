/* DEC/CMS REPLACEMENT HISTORY, Element SO_TIE_TREE.C*/
/* *6     1-AUG-1990 13:19:16 VINCE "(SPR 5615) updated for integer line id"*/
/* *5    26-JUL-1990 17:22:50 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *4    25-JUL-1990 20:06:50 VINCE "(SPR 5644) Header Standardization"*/
/* *3    29-JUN-1990 19:04:34 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *2    25-JUL-1989 20:08:16 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_TIE_TREE.C*/

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

#include "esi_math.h"
#include "esi_so.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to load so_tie_struct btree.

Prototype:
    publicdef INT so_tie_tree(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_tie_tree (SO_STRUCT *pso)
#else
publicdef INT so_tie_tree (pso)
SO_STRUCT *pso;
#endif
    {
    
    INT status;
    
    SO_TIE_STRUCT *ptie;            /* pointer to new so_tie_struct */
    FLOAT x1, y1, x2, y2;           /* local nlist stuff */
    UINT Col_List[2];
    VOIDPTR Var_List[2];
    DOUBLE slope;
    
    status = SUCCESS;
    
    ptie = (SO_TIE_STRUCT *)am_allocate (AM_APPLICATION, sizeof(SO_TIE_STRUCT));
    
    ptie->nlist     = pso->seismic_surface.nlist;
    ptie->first_cdp = pso->seismic_surface.first_cdp;
    ptie->last_cdp  = pso->seismic_surface.last_cdp;
    ptie->cdp_inc   = pso->seismic_surface.cdp_inc;
    ptie->icoef     = 0;
    
    Col_List[0] = 2;
    Col_List[1] = 3;
    Var_List[0] = (VOIDPTR) & x1;
    Var_List[1] = (VOIDPTR) & y1;
    
    /* get first point in cdp_xy */
    
    nl_first_point (pso->cdp_xy, 2, Col_List, Var_List);
    
    /* get last point in cdp_xy */
    
    Var_List[0] = (VOIDPTR) &x2;
    Var_List[1] = (VOIDPTR) &y2;
    nl_last_point (pso->cdp_xy, 2, Col_List, Var_List);
    
    /* north or south */
    
    if (fabs (x2 - x1) < 1.0e-4)
        {
        slope = 99999.;    /* CLOSE to vertical */
        }
    else
        {
        slope = (y2 - y1) / (x2 - x1);
        }

    if (fabs (slope) > 1.0)
        {
        strcpy (ptie->north_or_south, "NORTH");
        }
    else
        {
        strcpy (ptie->north_or_south, "SOUTH");
        }

    btadd (&pso->seismic_mistie.mistree, &pso->current_line.id, &ptie);
    
    return status;
    }
/* END:     */
