/* DEC/CMS REPLACEMENT HISTORY, Element GB_WELL_PLYGN.C */
/* *7    14-AUG-1990 13:27:26 VINCE "(SPR 5644) Header Standardization" */
/* *6    12-FEB-1990 10:19:28 GILLESPIE "(SPR 6008) Use ag_side instead of jg_side" */
/* *5    22-SEP-1989 15:33:35 GILLESPIE "(SPR 101) Add esi_mg.h" */
/* *4    19-SEP-1989 13:37:07 GILLESPIE "(SPR 100) GULF MODS" */
/* *3    19-SEP-1989 11:07:48 GORDON "(SPR 100) fix casting" */
/* *2    14-SEP-1989 18:18:31 GORDON "(SPR -1) checkin from gulf" */
/* *1    14-SEP-1989 17:33:43 GORDON "a" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_WELL_PLYGN.C */
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

#include "esi_gb.h"
#include "esi_c_lib.h"
#include "esi_gb_err.h"
#include "esi_gb_msg.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_LE_NLED_H

#include "esi_le_nled.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_AG_H

#include "esi_ag.h"

#endif
/* wm classes                                                           */

static WINDOW_ID gbWindowId = 0;
static TNR_ID gbTnrId = 0;

static APPLICATION_ID appSave;
static WINDOW_ID winSave;
static TNR_ID tnrSave;
static GROUP_ID groSave;

/* macros                                                               */
#define WARNING_RETURN(s) \
    { \
       am_message( AM_ALERT,  s ) ; \
       return ( FAIL ) ; \
    }
        
#define ALERT_RETURN(s) \
    { \
       am_message(AM_ALERT, s ) ; \
       return ( FAIL ) ; \
    }



/* Internal routines                                                    */
static INT zzInit ();
static INT zzTerm ();

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gb_well_polygon (NLIST_HEADER *select_list);

Private_Functions:
    static INT zzInit ();

    static INT zzTerm ();

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT gb_well_polygon (NLIST_HEADER *select_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    select_list     -(NLIST_HEADER *) Pointer to created select list.            

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_well_polygon (NLIST_HEADER *select_list)
#else
publicdef INT gb_well_polygon (select_list)
NLIST_HEADER *select_list;
#endif
    {
    INT status = SUCCESS, nwells, w;
    DOUBLE xmin, ymin, xmax, ymax, xtest, ytest;
    
    UINT npoints;
    UINT dim_list[2];
    VOIDPTR value_list[2];
    UINT ndim = 2, stroke_num;
    
    HASH_TABLE *ht;                 /* hash table of UWI,X,Y for wells inside rectangle */
    DOUBLE *poly_x, *poly_y;        /* array form of polygon for inside tests */
    
    SELECT_KEY_STRING *uwi_list;
    struct 
        {                           /* array of points returned from hash table */
        DOUBLE x;
        DOUBLE y;
        }  *xy;
    UWI uwi;                        /* current well identifier to be added to select list */
    
    
    status = zzInit ();
    if (status == SUCCESS)
        {
        wmwopen (gbWindowId);
        wmtselect (gbTnrId);
        
        am_message (AM_DIALOG, mg_message (GB_DRAW_POLYGON_PROMPT));
        
        /* allow user to sketch polygon -- returns polygon with explicit closure */
        
        if (ug_if_gpx ())
            wm_set_mouse_menu ((CHAR **)0, (INT *)0);
        status = gb_draw_polygon_array ((INT *) & npoints, &xmin, &ymin, &xmax, &ymax,
                                        &poly_x, &poly_y);  /* status is FAIL if user cancelled */
        
        if (status == SUCCESS)
            {
            
            /* Build hash table of UWI, X, Y of wells which are in overlays of the
            current map and are within the bounding rectangle */
            
            status = gb_rect_map_wells (xmin, ymin, xmax, ymax, &ht, FALSE);
            
            if (status == SUCCESS)
                {
                /* First, extract list of UWI's and XY's from the hash table */
                
                ht_get_all (ht, (UINT *) & nwells, (VOIDPTR *) & uwi_list,
                            (VOIDPTR *) & xy);
                
                /* Eliminate wells in the list which are outside the polygon */
                
                if (nwells > 0)
                    {
                    status = SUCCESS;
                    dim_list[0] = 1;
                    ndim = 1;
                    value_list[0] = (VOIDPTR)uwi;
                    sl_init_select_list (WELLS_DATA_TYPE, select_list);
                    nl_start_table (*select_list, (UINT *) & stroke_num);
                    
                    for (w = 0; w < nwells; w++)
                        {
                        xtest = xy[w].x;
                        ytest = xy[w].y;
                        if (ag_inside (xtest, ytest, poly_x, poly_y, 0, npoints))
                            {
                            strcpy (uwi, uwi_list[w]);
                            nl_add_rows (*select_list, 1, (UINT)ndim, (UINT *)dim_list,
                                         (VOIDPTR)value_list);
                            }
                        }
                    /* free the arrays allocated by ht_get_all */
                    tc_free ((VOIDPTR)uwi_list);
                    tc_free ((VOIDPTR)xy);
                    }
                else
                    {
                    status = FAIL;
                    }
                }
            /* free the hash table and the polygon arrays */
            ht_free_table (&ht);
            tc_free ((VOIDPTR)poly_x);
            tc_free ((VOIDPTR)poly_y);
            }
        }
    zzTerm ();
    
    am_message (AM_DIALOG, (CHAR *)"");
    
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zzInit ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <gb_well_plygn.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zzInit ()
#else
static INT zzInit ()
#endif
    {
    INT status = SUCCESS;
    INT wmStatus, ifExists;
    MAP_STRUCTURE *cur_map_workspace;
    
    /* clear global stuff                                               */
    gbWindowId = 0;
    gbTnrId = 0;
    
    /* save wm state and close out to application                       */
    wmowhere (&appSave, &winSave, &tnrSave, &groSave, &wmStatus);
    
    /* front the map                                                    */
    status = mp_front_map (&cur_map_workspace);
    if (status < SUCCESS)
        {
        ALERT_RETURN (mg_message (GB_ERR_NO_MAP_WINDOW));
        }
    gbWindowId = cur_map_workspace->map_window_id;
    
    /* map tnr id                                                       */
    gbTnrId = cur_map_workspace->map_tnr;
    if (!(gbTnrId))
        {
        ALERT_RETURN (mg_message (GB_ERR_BAD_MAP_XFORM));
        }
    wmoexists (appSave, gbWindowId, gbTnrId, 0, &ifExists);
    if (!(ifExists))
        {
        WARNING_RETURN (mg_message (GB_ERR_NO_MAP_WINDOW));
        }
    /* attach to top level                                              */
    wmoattach (appSave, (WINDOW_ID)0, (TNR_ID)0, (GROUP_ID)0, &wmStatus);
    if (wmStatus != SUCCESS)
        {
        WARNING_RETURN (mg_message (wmStatus));
        }
    status = SUCCESS;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zzTerm ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gb_well_plygn.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zzTerm ()
#else
static INT zzTerm ()
#endif
    {
    INT status = SUCCESS;
    INT wmStatus;
    
    /* return to saved wm state                                         */
    wmoattach (appSave, winSave, tnrSave, groSave, &wmStatus);
    
    return status;
    }
/* END:     */
