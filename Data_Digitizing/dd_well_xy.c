/* DEC/CMS REPLACEMENT HISTORY, Element DD_WELL_XY.C*/
/* *5    21-MAY-1990 15:04:52 JULIAN "(SPR 5311) fix return xpos,ypos return value"*/
/* *4    10-OCT-1989 09:48:05 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *3    14-SEP-1989 16:14:45 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    13-JUL-1989 14:10:21 GORDON "(SPR 0) add call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:45:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DD_WELL_XY.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*
 * FILE:  DD_GET_WELL_XY.C
 *
 * ROUTINES : DD_GET_WELL_XY();
 *
 */

/* INCLUDE FILES */
#include "esi_c_lib.h"
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"               /* mapping structures */
#endif
#ifndef ESI_CS_H
#include "esi_cs.h"              /* Centered Symbol graphics routines */
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif
#ifndef ESI_LU_H
#include "esi_lu.h"
#endif
#ifndef ESI_DD_ERR_H
#include "esi_dd_err.h"           /* error codes for data digitization */
#endif

/*
 * Routine : DD_GET_WELL_XY()
 * 
 * Description :
 *   This will be called from an ORACLE USER EXIT.
 *   Locate from a user and return the coordinates
 *     of a well on a map.
 *
 * Requirements to run program :
 *   map drawn on screen
 *   the correct area has been selected
 *
 * Method :
 *   Check if map is registered. If not register it.
 *   Query and open window containing map. 
 *   call digitize point through locator.
 *
 */


/* ********************************************************************** */

INT dd_get_well_xy( object_name, plot_symbol, color, xpos, ypos)

CHAR *object_name;                /* name of object */
INT    plot_symbol;               /* well symbol type specification */
CHAR *color;                      /* character string containing color */
DOUBLE *xpos, *ypos;              /* x and y position of a well on a */
                                  /* registered map. */
{
    PROGNAME( "dd_get_well_xy" );

    INT     status = SUCCESS ;
    DOUBLE  oldx,oldy;                   /* does the well already exist */
    DOUBLE scale;                        /* centered symbol scale factor */
    DOUBLE symbol_size;                  /* pick out a good one */
    FLOAT   wm_x;			 /* needed for the &^%&* window manager */
    FLOAT   wm_y;                        /* ditto */
    INT     exists;
    INT     tnr, group, window_id, button;
    INT     color_code;
    MAP_STRUCTURE  *map;                 /* map struct for current map */
    INT defmod,regmod;

    exists = FALSE;

    wm_inq_deferral(&defmod,&regmod);
    wm_hold_updates();

/* 
 * get a pointer to the current map workspace.
 */
    if (mp_front_map(&map) != SUCCESS)
    {
       status = DD_ERR_NO_MAP_DEF ;
       wm_set_deferral(defmod,regmod);
       return( status );
    }
    wm_update();
/* 
 * Check to see if map is registered 
 */
#if 0
    if ( map -> map_is_registered EQUALS FALSE )
    {
       status = DD_ERR_MAP_NOT_REG ;
       return( status );
    }
#endif

/* 
 * Get transformation for the map (and abort if there is none)
 */
    tnr = map->map_tnr;
    if ( tnr EQUALS 0 )
    {
       status = DD_ERR_NO_TNR_DEF ;
       wm_set_deferral(defmod,regmod);
       return( status );
    }

/* 
 * Open window before registering 
 */
    window_id = map->map_window_id;
    if ( window_id EQUALS 0 )
    {
       status = DD_ERR_BAD_WIN_ID;
       wm_set_deferral(defmod,regmod);
       return( status );
    }


/* 
 * open window, select transfromation, and enable registration.
 */
    wmwopen( window_id );
    wmtselect( tnr );
    wmtregonoff( ON );

/* if points are zeros then will exists, and it should be removed from */
/* the window manager before it can be added as a well symbol */
   if ( *xpos != 0.0  AND  *ypos != 0.0 )
   {
       oldx = *xpos;
       oldy = *ypos;
       exists = TRUE;
   }
       
/* 
 * allocate group (overlay) and set attributes of group
 */
    wmgallocate( &group );
    wmoset( group, WM_CLASS_OVERLAY, "", object_name );

/* Get a location from window manager */
    if (ug_if_gpx())
	wm_set_mouse_menu( (CHAR**)0, (INT*)0 );   /* use default menu */

    wmtlocate( &status, &wm_x, &wm_y, &button );

    if ( button EQUALS WM_BUTTON_CANCEL)
    {
        status = DD_ERR_HIT_CANCEL_BUT ;
       wm_set_deferral(defmod,regmod);
        return( status );
    }

/*
 * Perform Cartographic conversion   
 */
    if ( ct_project_xy((DOUBLE)wm_x,(DOUBLE)wm_y,xpos,ypos) != SUCCESS )
    {
       status = DD_ERR_BAD_CART_TNR;
       wm_set_deferral(defmod,regmod);
       return ( status );
    }

/* get the size to use to draw the symbol */
   symbol_size = (map->upper_right_xy.y - map->lower_left_xy.y)/40.0;
   cs_set_height(symbol_size);
/* 
 * Draw the well symbol. if it exist, draw it in black and draw new one.
 */
   scale = 1.0;
   if ( exists )
   {
      lu_text_to_index("COLOR", &color_code, "BACKGROUND" );
      status = cs_draw_symbol( CS_V4_WELLS_GROUP, plot_symbol, scale, 
                               color_code, oldx, oldy );
   }
   lu_text_to_index("COLOR", &color_code, color );
   status = cs_draw_symbol( CS_V4_WELLS_GROUP, plot_symbol, scale, 
                            color_code, wm_x, wm_y );

/* Close  overlay group and close window */
   wmgclose( group );
   wmwclose( window_id );
   wm_update();
       wm_set_deferral(defmod,regmod);

   return( status );
}
