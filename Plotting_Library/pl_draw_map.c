/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_MAP.C */
/* *2     6-JUN-1991 16:51:07 MING "(SPR 0) new plot file managment" */
/* *1    12-OCT-1990 17:39:47 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_MAP.C */
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



/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT func (arg);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg	- description

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/


#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_draw_map (PL_BASE_WINDOW *bw)
#else
publicdef INT pl_draw_map (bw)
PL_BASE_WINDOW *bw;
#endif
{
  INT status = SUCCESS;
  INT i,j;
  MAP_STRUCTURE *map;

  status = am_open_workspace ("MAPDEF",AM_APPLICATION,(VOIDPTR *)&map);
  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"open mapdef fail");
    return FAIL;
    }
  i = 0;
  j = map->display->last_row_used;
  map->map_window_id = bw->win_id;
  map->map_tnr = bw->tnr_id;
  mpz_display_overlays (i,j);

  return status;
}
