/* DEC/CMS REPLACEMENT HISTORY, Element SX_LOCATE.C*/
/* *4     2-NOV-1989 09:55:07 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *3    31-JUL-1989 14:47:53 GORDON "(SPR 101) add call to wm_set_mouse_menu for default menu "*/
/* *2    25-JUL-1989 20:12:54 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_LOCATE.C*/

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

/*****************************************************************************/
/*                                                                           */
/* PROGRAM:  SX_LOCATE.C                                                     */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: LOCATE POINT IN MAP                                             */
/*                                                                           */
/*****************************************************************************/

#include "esi_sx.h"
#include "esi_wm.h"
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

publicdef INT sx_locate(psx,stat,x,y,button)
SX_STRUCT    *psx;
INT          *stat;
FLOAT        *x;
FLOAT        *y;
INT          *button;
{
    INT status;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;

    if (ug_if_gpx())
	{
	wm_set_mouse_menu( (CHAR**)0, (INT*)0 );  /* use default menu */      
	}

    *x = pmap->lower_left_xy.x - 1.0;

    while (*x < pmap->lower_left_xy.x ||
           *x > pmap->upper_right_xy.x ||
           *y < pmap->lower_left_xy.y ||
           *y > pmap->upper_right_xy.y)
        {
        wmtlocate(stat,x,y,button);
        if(*button == WM_BUTTON_CANCEL OR *stat != SUCCESS OR
           *button == WM_BUTTON_DONE)
            {
            break;
            }

        if( *stat == SUCCESS &&
            (*button == WM_BUTTON_ZOOMIN ||
	     *button == WM_BUTTON_PAN ||
	     *button == WM_BUTTON_RESET))
            {
            so_draw_seismic_line(psx->pso);
            break;
            }

        if(*x < pmap->lower_left_xy.x ||
           *x > pmap->upper_right_xy.x ||
           *y < pmap->lower_left_xy.y ||
           *y > pmap->upper_right_xy.y)
           wi_beep(1);
        }

    status = SUCCESS;
    SX_RETURN;
}
        

