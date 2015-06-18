/* DEC/CMS REPLACEMENT HISTORY, Element GR_POST_GRAT.C*/
/* *4     5-DEC-1990 10:32:21 JESSIE "(SPR 6233) accept xtic/ytic with DEGREES"*/
/* *3    13-NOV-1990 10:46:01 JESSIE "(SPR 0) check DMS input format"*/
/* *2    14-AUG-1990 13:30:00 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:54:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GR_POST_GRAT.C*/
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

#include "esi_gl_defs.h"
#include "esi_gr.h"                    /* graticule overlay defs */
#include "esi_mp.h"
#include "esi_wm.h"                    /* window manager */
#include "esi_wm_cls.h"                    /* window manager */
#include "esi_tu.h"

/* Function Description -----------------------------------------------------
Description:
    Post a gradicule for a land grid.
    A graticule consists of labels,tics,index points on
    an overlay.

Prototype:
    publicdef INT gr_post_grat(GRAT_OV_STRUCT     *graticule );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    graticule       -(GRAT_OV_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gr_post_grat (GRAT_OV_STRUCT *graticule)
#else
publicdef INT gr_post_grat (graticule)
GRAT_OV_STRUCT *graticule;
#endif
    {
    INT status = SUCCESS;
    INT segment;                    /* segment number */
    UOM_NAME uom_name;
    UOM_STRUCTURE uom;
    MAP_STRUCTURE *map;
    DOUBLE value;
    UOM_NAME uom1,uom2;
    
    /*********************************************************************/
    
    /* * OPEN A SEGMENT AND SET IT AS AN OBJECT * */
    /* ------------------------------------------ */
    
    wmsallocate (&segment);
    wmoset (segment, WM_CLASS_SEGMENT, "", "graticule");
    
    /* * DRAW THE GRATICULE * */
    /* ---------------------- */
    
    if (graticule->uom != NULL)
        {
        tu_verify_uom (graticule->uom, &uom);
        }
    else
        {
        mp_get_current_map (&map);
        ct_get_projection_uom (map->map_projection, uom_name);
        tu_verify_uom (uom_name, &uom);
        }
    if (strcmp (uom.uom_type, "ANGULAR") EQUALS 0)
	{    
	status = gr_draw_graticule_geo (graticule, &uom);
/*	if (((status = con_dms2dec(graticule->x_tic_interval, &value)) == SUCCESS)
	 || ((status = con_dms2dec(graticule->y_tic_interval, &value)) == SUCCESS))
	    {
	    status = gr_draw_graticule_geo (graticule, &uom);
	    }
	else if(((status = tu_parse_unit(graticule->x_tic_interval,&value,uom1,uom2)) == SUCCESS)
	     || ((status = tu_parse_unit(graticule->y_tic_interval,&value,uom1,uom2)) == SUCCESS))
	    {
	    if ((status = strcmp(uom1,"")) == SUCCESS)
		status = gr_draw_graticule_geo (graticule, &uom);
	    }
*/        }
    else if (((status = con_dms2dec(graticule->x_tic_interval, &value)) == SUCCESS)
          || ((status = con_dms2dec(graticule->y_tic_interval, &value)) == SUCCESS))
        {
        status = gr_draw_graticule_geo (graticule, &uom);
        }
    else
        {
        status = gr_draw_graticule (graticule);
        }
    /* * CLOSE THE SEGMENT * */
    /* --------------------- */
    
    wmsclose (segment);
    return(status);
    }
/* END:     */
