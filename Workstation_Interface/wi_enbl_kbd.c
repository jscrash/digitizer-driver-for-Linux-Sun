/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_KBD.C*/
/* *4     2-MAR-1990 12:07:28 JULIAN "(SPR 6012) fix if USE_X == 0 to be ifndef USE_X"*/
/* *3     2-MAR-1990 11:28:45 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:36:10 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_KBD.C*/

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

NAME:    WI_ENABLE_KEYBOARD

DESCRIPTION:

    Enable or disable keyboard entry.

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_wi_int.x"		/* external definitions required here */

/************************************************************************/
#define IDENT "wi_enable_keyboard"
INT wi_enable_keyboard(onoroff)
/************************************************************************/

INT onoroff;

    {
    wiz_jp_name(IDENT);

    E_CHECK_INIT;

#ifndef USE_X
    SEND_2_VALUES( onoroff, MC_ENABLE_KEYBOARD);
#endif

    RETURN (SUCCESS);
    }
