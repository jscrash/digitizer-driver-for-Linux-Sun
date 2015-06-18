/* DEC/CMS REPLACEMENT HISTORY, Element MR_REG_MAP.C */
/* *7    24-OCT-1991 13:24:31 JULIAN "(SPR 3900) Added warning message when register map on works w/o digitizer" */
/* *6     6-AUG-1991 14:51:21 KEE "(SPR 8776) Fix lower/upper left/right wrong prompt problem (case 3487)" */
/* *5    11-JUN-1991 11:11:23 KEE "(SPR 7962) Fix SX_MAP_NOT_REGISTERED although user registered the map" */
/* *4    17-AUG-1990 22:11:12 VINCE "(SPR 5644) Code Cleanup" */
/* *3     7-MAR-1990 17:25:25 WALTERS "(SPR 0) Remove FLOAT types from system" */
/* *2    28-FEB-1990 08:05:46 GILLESPIE "(SPR 1) Called from mp_mp_srvr" */
/* *1    28-FEB-1990 08:04:18 GILLESPIE "Replace into cms" */
/* DEC/CMS REPLACEMENT HISTORY, Element MR_REG_MAP.C */
/* DEC/CMS REPLACEMENT HISTORY, Element MR_REG_MAP.C*/
/* *7    15-FEB-1990 10:45:52 WALTERS "(SPR 0) Changes in map setup dialog"*/
/* *6    13-NOV-1989 23:06:48 JULIAN "(SPR 1) Fix bad args to ct_transform_point()"*/
/* *5    13-NOV-1989 13:29:44 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *4    10-OCT-1989 09:59:45 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *3    28-JUL-1989 10:51:28 GILLESPIE "(SPR 1) fix minor casting problems"*/
/* *2     2-JUL-1989 12:06:05 JULIAN "(SPR 5) Tablet support"*/
/* *1    19-JUN-1989 13:15:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MR_REG_MAP.C*/
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
#include "esi_mr.h"
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_TG_H
#include "esi_tg.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "esi_mr_err.h"
#include "esi_mr_msg.h"

#include "mr_reg_dlg.rh"
#include "esi_tablet_ws.h"

#define NUMBER_OF_POINTS 30     /* Maximum number of control points */
static CHAR null_string[] =  "";

/* Function Description -----------------------------------------------------
Description:
    The root routine for map registration.

Prototype:
    publicdef INT mr_register_map ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mr_register_map ()
#else
publicdef INT mr_register_map ()
#endif
    {
    static INT reg_map_dialog_id;
    INT status = SUCCESS;
    DOUBLE llx, lly, urx, ury;
    FLOAT xpoints[3], ypoints[3];
    INT str_len, dialog_id, item_id, num_items, items[4];
    INT tnr_tab, tnr_map;
    CHAR msg[256];
    PROJECTION_ID pnt_projection_id;
    PROJECTION_STRUCTURE *pnt_projection, *map_projection;
    MAP_STRUCTURE *mapdef;
    CHAR ll_str[21], ur_str[21];
    CHAR coord_type[16];
    INT xin_tab, yin_tab;
    INT i;
    INT no_pnts, button, k_pnt;
    DOUBLE xy_cntl[NUMBER_OF_POINTS][2];
    DOUBLE xy_tablet[NUMBER_OF_POINTS][2];
    DOUBLE xy_backup[NUMBER_OF_POINTS][2];
    FLOAT x_tab, y_tab;
    DOUBLE x_cntl, y_cntl;
    FLOAT xy_tbl_0, xy_tbl_1;
    FLOAT xx_cntl, yy_cntl;
    DOUBLE xs, ys;
    FLOAT x_first, y_first;
    BOOL ifyes, t_flag;
    DOUBLE xy_temp_in[2];
    DOUBLE xy_temp_out[2];
    DOUBLE t_mat[2][2];
    TABLET_WORKSPACE *t;            /* pointer to the tablet workspace */
    INT use_tablet = FALSE;
    
    /*------------------------------------------------------*/
    /* the following declarations are used only to support  */
    /* the GPX tablet registration interface             */
    /*------------------------------------------------------*/
    
    INT points_in = 3;              /* number of points to gpx_wmtregistration         */
    POINT wcs_points[4];            /* users WCS control points to gpx_wmtregistration   */
    POINT tcs_points[4];            /* users TABLET control points to gpx_wmtregistration   */
    
    fi_push ();
    
    /* * GET MAP STRUCTURE * */
    /* --------------------- */
    
    if (mp_front_map (&mapdef) != SUCCESS)
        {
        fi_pop ();
        return SUCCESS;
        }

    /* GET TABLET DEVICE STRUCTURE */

    if ((am_open_workspace ("TABLET", AM_GLOBAL, (VOIDPTR *) & t)) == SUCCESS)
      {
        if (t->init != TRUE)
	  {
	  am_message(AM_ALERT,mg_message(MR_NO_DIGITIZER));
	  fi_pop();
	  return SUCCESS;
	  }
	else
	  use_tablet = TRUE;
      }

    /* * OPEN WINDOW BEFORE REGISTERING * */
    /* ---------------------------------- */
    
    wmwopen (mapdef->map_window_id);
    wmtselect (mapdef->map_tnr);
    
    /* * DISABLE REGISTRATION TO NOT DISTORT THE POINTS * */
    /* -------------------------------------------------- */
    
    wmtregonoff (OFF);
    
    /* * GET THE PROJECTION AND NUMBER OF POINTS * */
    /* ------------------------------------------- */
    
    no_pnts = 3;
    FOREVER
        {
        status = mr_get_projection (mapdef->map_name, null_string, pnt_projection_id,
                                    &no_pnts, coord_type);
        if (no_pnts == 2)
            {
            am_message (AM_ALERT, mg_message (MR_2_POINT_WARNING));
            }
        mr_get_projection_term ();
        
        /* CHECK FOR CANCEL (STATUS = 2) */
        if (status == 2)
            {
            wmwclose (mapdef->map_window_id);
            fi_pop ();
            return SUCCESS;
            }
        /* CHECK FOR CORRECT NUMBER OF CONTROL POINTS
                IF COORD_TYPE IS XY */
        
        if ((no_pnts < 2) &&
            ((strcmp (coord_type, "ATS") != 0) AND (strcmp (coord_type, "USPLS") != 0)))
            {
            am_message (AM_ALERT, mg_message (MR_XY_MORE_CONTROL));
            }
        else
            {
            /*------------------------------------------*/
            /* force 3 point registration if XY system  */
            /* and using VaxStation registration method */
            /*------------------------------------------*/
            if (ug_if_gpx ())
                {
                no_pnts = 3;
                }
            break;
            }
        }
    /* * GET THE CONTROL POINTS (XY COORD SYSTEM) * */
    /* -------------------------------------------- */
    
    if ((strcmp (coord_type, "ATS") != 0) AND (strcmp (coord_type, "USPLS") != 0))
        {
        
        /* GET PROJECTIONS */
        
        ct_get_projection (mapdef->map_projection, &map_projection);
        ct_get_projection (pnt_projection_id, &pnt_projection);
        t_flag = (map_projection != pnt_projection);
        
        /* LOOP ONCE FOR EACH CONTROL POINT */
        
        k_pnt = 0;
        xpoints[0] = ypoints[0] = 0.0;
        x_cntl = y_cntl = 0.0;
	if (NOT t_flag)
	  {
	    /* upper left xy */
	    xy_backup[0][0] = mapdef->lower_left_xy.x;
	    xy_backup[0][1] = mapdef->upper_right_xy.y;
	    /* lower left xy */
	    xy_backup[1][0] = mapdef->lower_left_xy.x;
	    xy_backup[1][1] = mapdef->lower_left_xy.y;
	    /* lower right xy */
	    xy_backup[2][0] = mapdef->upper_right_xy.x;
	    xy_backup[2][1] = mapdef->lower_left_xy.y;
	  }
        while (k_pnt < no_pnts)
            {
            sprintf (msg, mg_message (MR_ASK_COORDS), k_pnt + 1);
            if (t_flag)
                {
                xs = k_pnt ? xy_backup[k_pnt - 1][0] : 0.00;
                ys = k_pnt ? xy_backup[k_pnt - 1][1] : 0.00;
                }
            else
                {
		xs = xy_backup[k_pnt][0];
		ys = xy_backup[k_pnt][1];
                }
            /*--------------------------------------------------*/
            /* prompt user for points in order when registering */
            /* on a VaxStation                                  */
            /*--------------------------------------------------*/
            if (ug_if_gpx ())
                {
                switch (k_pnt)
                    {
                case 0:
                    sprintf (msg, "Enter Map Coord. of Upper Left");
                    break;
                case 1:
                    sprintf (msg, "Enter Map Coord. of Lower Left");
                    break;
                case 2:
                    sprintf (msg, "Enter Map Coord. of Lower Right");
                    break;
                    }
                }
            switch (mr_get_xy_control_point (null_string, msg, pnt_projection, &xs,
                                             &ys))
                {
            case SUCCESS:
                
                /* IF FIRST CONTROL POINT SAVE IT,
                   ELSE CHECK AGAINST FIRST */
                
                if (k_pnt == 0)
                    {
                    x_first = xs;
                    y_first = ys;
                    }
                else
                    {
                    if ((xs == x_first) AND (ys == y_first))
                        {
                        am_message (AM_ALERT,
                                    mg_message (MR_CONTROL_POINT_SAME_AS_FIRST));
                        break;
                        }
                    }
                x_cntl = xs;
                y_cntl = ys;
                
                xy_backup[k_pnt][0] = xs;
                xy_backup[k_pnt][1] = ys;
                
                /* TRANSFORM TO MAP PROJECTION, IF REQUIRED */
                
                if (t_flag)
                    {
                    if (pnt_projection->projection_type == 0)
                        {
                        xy_backup[k_pnt][0] = ys;
                        xy_backup[k_pnt][1] = xs;
                        }
                    xy_temp_in[0] = x_cntl;
                    xy_temp_in[1] = y_cntl;
                    ct_transform_point (xy_temp_in[0], xy_temp_in[1], pnt_projection,
                                        &xy_temp_out[0], &xy_temp_out[1],
                                        map_projection);
                    xy_cntl[k_pnt][0] = xy_temp_out[0];
                    xy_cntl[k_pnt][1] = xy_temp_out[1];
                    }
                else
                    {
                    xy_cntl[k_pnt][0] = x_cntl;
                    xy_cntl[k_pnt][1] = y_cntl;
                    }
                sprintf (msg, mg_message (MR_DIG_CONTROL), k_pnt + 1);
                am_message (AM_DIALOG, msg);
                status = 1;
                
                button = 12;
                if (use_tablet == TRUE)
                    {
                    status = tbh_request_locator (&xin_tab, &yin_tab, &button);
                    x_tab = xin_tab;
                    y_tab = yin_tab;
                    }
                else
                    {
                    wi_request_locate (&status, &tnr_tab, &x_tab, &y_tab, &button);
                    }
                /* CHECK FOR CANCEL (BUTTON 'C') */
                
                if (button == 12)
                    {
                    am_message (AM_DIALOG, null_string);
                    am_message (AM_STATUS, mg_message (MR_USER_CANCELLED));
                    mr_get_xy_control_point_term ();
                    wmwclose (mapdef->map_window_id);
                    fi_pop ();
                    return SUCCESS;
                    }
                wibeep (1);
                
                if (use_tablet == TRUE)
                    {
                    xy_tablet[k_pnt][0] = x_tab;
                    xy_tablet[k_pnt][1] = y_tab;
                    }
                else
                    {
                    tg_help_convert (tnr_tab, status, x_tab, y_tab, &xy_tbl_0,
                                     &xy_tbl_1);
                    xy_tablet[k_pnt][0] = xy_tbl_0;
                    xy_tablet[k_pnt][1] = xy_tbl_1;
                    }
                k_pnt++;
                am_message (AM_DIALOG, null_string);
                break;
                
                /* BACKUP */
            case 2:
                k_pnt = k_pnt ? k_pnt - 1 : k_pnt;
                break;
                
                /* CANCEL*/
            case 3:
                mr_get_xy_control_point_term ();
                wmwclose (mapdef->map_window_id);
                fi_pop ();
                return SUCCESS;
                
            default:
                break;
                
                }
            /* of case */
            
            }
        /* of while loop */
        
        mr_get_xy_control_point_term ();
        }
    /* * GET THE CONTROL POINTS (ATS OR USPLS) (ONLY 2) * */
    /* -------------------------------------------------- */
    
    else
        {
        if ((strcmp (mapdef->map_coordinate_system,
                     "ATS") == 0) OR (strcmp (mapdef->map_coordinate_system,
                                             "USPLS") == 0))
            {
            strcpy (ll_str, mapdef->lower_left_str);
            strcpy (ur_str, mapdef->upper_right_str);
            }
        else
            {
            ll_str[0] = ur_str[0] = 0;
            }
        no_pnts = 2;
        t_flag = FALSE;
        
        if (mr_get_str_control_points (xy_cntl, ll_str, ur_str) == 2)
            {
            mr_get_str_control_points_term ();
            wmwclose (mapdef->map_window_id);
            fi_pop ();
            return SUCCESS;
            }
        /* DIGITIZE THE LOWER LEFT POINT */
        
        sprintf (msg, mg_message (MR_LOWER_LEFT), ll_str);
        am_message (AM_DIALOG, msg);
        status = 1;
        
        if (use_tablet == TRUE)
            {
            status = tbh_request_locator (&x_tab, &y_tab, &button);
            }
        else
            {
            wi_request_locate (&status, &tnr_tab, &x_tab, &y_tab, &button);
            tg_help_convert (tnr_tab, status, x_tab, y_tab, &xx_cntl, &yy_cntl);
            x_cntl = xx_cntl;
            y_cntl = yy_cntl;
            }
        wibeep (1);
        xy_tablet[0][0] = x_cntl;
        xy_tablet[0][1] = y_cntl;
        am_message (AM_DIALOG, null_string);
        
        /* DIGITIZE THE UPPER RIGHT POINT */
        
        sprintf (msg, mg_message (MR_UPPER_RIGHT), ur_str);
        am_message (AM_DIALOG, msg);
        status = 1;
        
        if (use_tablet == TRUE)
            {
            status = tbh_request_locator (&x_tab, &y_tab, &button);
            }
        else
            {
            wi_request_locate (&status, &tnr_tab, &x_tab, &y_tab, &button);
            tg_help_convert (tnr_tab, status, x_tab, y_tab, &xx_cntl, &yy_cntl);
            x_cntl = xx_cntl;
            y_cntl = yy_cntl;
            }
        wibeep (1);
        xy_tablet[1][0] = x_cntl;
        xy_tablet[1][1] = y_cntl;
        am_message (AM_DIALOG, null_string);
        
        /* GET RID OF THE MAP REGISTRATION DIALOG */
        
        mr_get_str_control_points_term ();
        }
    if (ug_if_gpx ())
        {
        /*-------------------------------------------------*/
        /* The following transformation code is valid only */
        /* for the GPX tablet!!                            */
        /*-------------------------------------------------*/
        /*--------------------------------------------------------------------------*/
        /*  For now, assume that wcs_points & tcs_points contain points_in = 3      */
        /*  entries in the correct order (as expected by gpx_wmtlocate()):          */
        /*                                                                          */
        /*   [0] upper left UL                                                      */
        /*   [1] lower left LL                                                      */
        /*   [2] lower right LR                                                     */
        /*   [3] upper right UR this position is assummed to be undefined           */
        /*                                                                          */
        /*  Later we will ensure that points are arranged in the correct order      */
        /*--------------------------------------------------------------------------*/
        points_in = 3;
        for (i = 0; i < points_in; i++)
            {
            wcs_points[i].x = xy_cntl[i][0];
            wcs_points[i].y = xy_cntl[i][1];
            tcs_points[i].x = xy_tablet[i][0];
            tcs_points[i].y = xy_tablet[i][1];
            }
        if (use_tablet == TRUE)
	  {
	    gpx_wmtregistration (mapdef->map_tnr, wcs_points, tcs_points, points_in);
	  }
        }
    else
        {
        /*-------------------------------------------------*/
        /* The following transformation code is valid only */
        /* for the Raster Tek                              */
        /*-------------------------------------------------*/
        /* * CALCULATE THE TRANSFORMATION * */
        /* -------------------------------- */
        
        mr_calc_xform (xy_cntl, xy_tablet, no_pnts, t_mat);
        
        /* CALCULATE A 3 POINT REGISTRATION */
        
        llx = mapdef->lower_left_xy.x;
        lly = mapdef->lower_left_xy.y;
        urx = mapdef->upper_right_xy.x;
        ury = mapdef->upper_right_xy.y;
        
        if (use_tablet == TRUE)
            {
            mr_calc_4p (xy_cntl, xy_tablet, llx, lly, urx, ury, xpoints, ypoints,
                        t_mat);
            }
        else
            {
            mr_calc_3p (xy_cntl, xy_tablet, llx, lly, urx, ury, xpoints, ypoints,
                        t_mat);
            }
        /* * NOW REGISTER THE MAP * */
        /* ------------------------ */
        
        wmtregistration (mapdef->map_tnr, xpoints, ypoints, llx, lly, urx, ury);
        }

    /* * RENABLE REGISTRATION * */
    /* ------------------------ */
        
    wmtregonoff (ON);
    mapdef->map_is_registered = TRUE;
    mp_retrieve_workspace (mapdef->map_number);

    /* * DO CHECK POINTS * */
    /* ------------------- */
    
    wiyesno (mg_message (MR_CHECKPOINT), null_string, &ifyes, &status);
    if (ifyes)
        {
        mr_get_checkpoints (t_flag, map_projection, pnt_projection);
        }
    /* * CLOSE THE WINDOW * */
    /* -------------------- */
    
    am_message (AM_STATUS, mg_message (MR_NORMAL_TERMINATION));
    wmwclose (mapdef->map_window_id);
    fi_pop ();
    return status;
    }
/* END:     */
