/* DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FM_TCP.C */
/* *3    11-SEP-1991 13:51:36 JULIAN "(SPR 0) add comments to lease overlay" */
/* *2    18-MAR-1991 18:09:44 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:58:36 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element MP_PRM_FM_TCP.C */
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





/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: MP_PARM_FROM_DIALOG.C

   DESCRIPTION: ROUTINE TO GET THE PARAMETER INPUT FROM THE INPUT
		TEXT DIALOG BOX.

   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_LM_H
#include "esi_lm.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_MD_H
#include "esi_md.h"
#endif


/* * FUNCTION TITLE * */
/* ------------------ */

INT mp_parm_from_tcp(edit_flag,key_info,parm_string,parmstruc)

/* * ARGUEMENT LIST * */
/* ------------------ */

BOOL edit_flag;
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;

/* * VARIABLE LIST * */
/* ----------------- */

{

INT status;
MD_PARM_STRUCT *temp_parm;
MD_STATE_WORKSPACE *md_state;
CHAR temp_string[MD_MAX_PARM_STRING];

/* ********************************************************************** */

    am_open_workspace("MAPDEF_STATE",AM_GLOBAL,(VOIDPTR *)&md_state);

/* * GET THE PARAMETER FROM THE USER * */
/* ----------------------------------- */

    status = lm_annotation_init(parm_string,temp_string);

/* * VALIDATE PARAMETER * */
/* ---------------------- */

    if(status == SUCCESS)
	{   
        mp_parm_ok(temp_string,md_state->active_verb_ndx,
		    md_state->active_key_ndx,&temp_parm,&status);
        }


    if(status == SUCCESS)
	{
/* DETERMINE STATUS AND NEXT ACTION * */
/* ---------------------------------- */
	md_free_parms(*parmstruc);
	*parmstruc = temp_parm;
	strcpy(parm_string,temp_string);
	}
    else if (status != CANCEL AND status != FAIL)
	{
	wi_alert(0,"","PARAMETER ENTRY ERROR:",mg_message(status));
	status = FAIL;
	}

    return status;
}
