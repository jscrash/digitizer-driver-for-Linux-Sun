/* DEC/CMS REPLACEMENT HISTORY, Element SX_ZZZZ.C*/
/* *2    25-JUL-1989 20:15:32 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_ZZZZ.C*/

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
/* PROGRAM:  SX_ZZZZ                                                         */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: DRAW LITTLE ZZZZ LABELS                                         */
/*                                                                           */
/*****************************************************************************/
#include "esi_sx.h"

publicdef INT sx_zzzz(psx)
SX_STRUCT *psx;
{
    INT status;


    /* assume SUCCESS */

    status = SUCCESS;

    psx->pso->zzzz = TRUE;
    so_draw_seismic_line(psx->pso);
    psx->pso->zzzz = FALSE;

    SX_RETURN;
}
