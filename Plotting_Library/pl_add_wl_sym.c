/* DEC/CMS REPLACEMENT HISTORY, Element PL_ADD_WL_SYM.C */
/*  2     3-MAR-1992 13:50:21 MING "(SPR 5967) fix wl_sym reference, from wl_sym[i]->color to wl_sym[0][i]->color" */
/* *1    12-OCT-1990 17:33:45 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_ADD_WL_SYM.C */
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

#if USE_PROTOTYPES
publicdef INT pl_add_well_symbol (INT symbol, INT color)
#else
publicdef INT pl_add_well_symbol (symbol,color)
INT symbol;
INT color;
#endif
{
  INT i;
  INT status;
  BOOL found;
  INT *wl_sym_number;
  WELL_LEGEND_LIST *wl_sym;

  status = am_open_workspace ("WL_SYM_NUMBER",AM_APPLICATION,
                                        (VOIDPTR *)&wl_sym_number);

  if (*wl_sym_number < MAX_WELL_SYMBOL)
    {
    status = am_open_workspace ("WL_SYM",AM_APPLICATION,(VOIDPTR *)&wl_sym);

    found = FALSE;
    for (i = 0; i < *wl_sym_number; ++i)
      {
      if (symbol == wl_sym[i]->symbol)
        {
        found = TRUE;
        break;
        }
      }

    if (found == FALSE)
      {
      wl_sym[*wl_sym_number]->symbol = symbol;
      wl_sym[*wl_sym_number]->color = color;
      (*wl_sym_number)++;
      }
    }
}	

