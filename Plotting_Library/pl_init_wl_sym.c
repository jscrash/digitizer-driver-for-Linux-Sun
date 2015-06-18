/* DEC/CMS REPLACEMENT HISTORY, Element PL_INIT_WL_SYM.C */
/* *2    15-OCT-1990 12:18:21 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:45:08 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_INIT_WL_SYM.C */
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


#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_AM_ERR_H
#include "esi_am_err.h"
#endif

publicdef INT pl_init_well_symbol ()
{
  INT status;
  INT *wl_sym_number;
  WELL_LEGEND_LIST *wl_sym;

  status = am_open_workspace ("WL_SYM_NUMBER",AM_APPLICATION,
                                        (VOIDPTR *)&wl_sym_number);
  if (status == AM_ERR_NOT_DEFINED)
    {
    am_define_workspace ("WL_SYM_NUMBER",AM_APPLICATION,
			   sizeof (INT),(VOIDPTR *)&wl_sym_number);
    }

  status = am_open_workspace ("WL_SYM",AM_APPLICATION,
                                        (VOIDPTR *)&wl_sym);

/* ------------------------------------------- */
/* assume no more than 1000 well legend symbol */
/* ------------------------------------------- */
  if (status == AM_ERR_NOT_DEFINED)
    {
    am_define_workspace ("WL_SYM",AM_APPLICATION,
			   sizeof (WELL_LEGEND_LIST),(VOIDPTR *)&wl_sym);
    }

  *wl_sym_number = 0;

}
