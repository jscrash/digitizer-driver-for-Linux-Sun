/* DEC/CMS REPLACEMENT HISTORY, Element WI_REQ_LOC.C*/
/* *4     5-FEB-1990 23:43:30 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *3    26-JUN-1989 11:05:34 GILLESPIE "(SPR 20) Fix castings"*/
/* *2    20-JUN-1989 11:45:32 GILLESPIE "(SPR 20) Fix a minor casting problem"*/
/* *1    19-JUN-1989 13:38:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_REQ_LOC.C*/

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
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

/*************************************************************************

NAME:    WI_REQUEST_LOCATE

DESCRIPTION:

    Perform a graphics locator operation in GKS.

    OutPut:
	status = (INT *) return status, 0=CANCEL, 1=COMPLETE.
	tnr    = (INT *) GKS transformation number.
	xpos   = (FLOAT *) x position.
	ypos   = (FLOAT *) y position.
	button = (INT *) Button pressed on input device, "puck key".

*************************************************************************/

#include "esi_gl_defs.h"
#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#include "esi_conf_ws.h"


/************************************************************************/
#define IDENT "wi_request_locate"
publicdef INT wi_request_locate(status, tnr, xpos, ypos, button)
/************************************************************************/

INT *status;
INT *tnr;
INT *button;
FLOAT *xpos;
FLOAT *ypos;
{
    INT tnr_temp, status_temp, button_temp;
    FLOAT xpos_temp, ypos_temp;
    CONFIGURATION_WORKSPACE *c;   /* Pointer to CONFIGURATION global workspace */

    INT locator_device = 1; /*%? May vary from one GKS to another. */

/************************************************************************/

   wiz_jp_name(IDENT);
   E_CHECK_INIT;


   if (wiv.jpmode == WI_PLAYBACK)  {
	 wiz_jp_read_long(&status_temp);
	 wiz_jp_read_long(&tnr_temp);
	 wiz_jp_read_float(&xpos_temp);
	 wiz_jp_read_float(&ypos_temp);
   }
   else {

	 /* Request position from GKS. */
	 wm_grqlc(wiv.iwkid, locator_device, &status_temp, &tnr_temp, &xpos_temp,
		&ypos_temp);

	 if (wiv.jpmode == WI_JOURNAL)  {
	    wiz_jp_write_long(status_temp);
	    wiz_jp_write_long(tnr_temp);
	    wiz_jp_write_float(xpos_temp);
	    wiz_jp_write_float(ypos_temp);
	 }
   }

   *xpos = xpos_temp;
   *ypos = ypos_temp;
   *status = status_temp;
   *tnr = tnr_temp;

   if (wiv.jpmode == WI_PLAYBACK)  {
	wiz_jp_read_long(&button_temp);
   }
   else  {
	 am_open_workspace("CONFIGURATION",AM_GLOBAL,(VOIDPTR *)(VOIDPTR *)&c);
/* %D Get button illegaly from GKS device driver for RasterTech. */
	if ((strcmp("180",c->graphics_terminal_type) == 0) OR
		(strcmp("110",c->graphics_terminal_type) == 0))
	    {
	    wm_gkzqbtn(&button_temp);
	    }
	else {
	   button_temp = -1;
	}
   }

   *button = button_temp;

   if  (wiv.jpmode == WI_JOURNAL)  {
	  wiz_jp_write_long(button_temp);
   }

   return SUCCESS;
}

                                                                                               
