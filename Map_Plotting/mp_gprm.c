/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GPRM.C*/
/*  *9    18-MAR-1991 17:10:20 JULIAN "(SPR 0) Lease Mapping init checkin"*/
/*  *8    28-NOV-1990 14:20:10 MING "(SPR -1) check status return by mp_parm_from_text"*/
/*  *7    27-NOV-1990 17:20:43 JULIAN "(SPR 0) complete prev change"*/
/*  *6    19-NOV-1990 10:33:07 JULIAN "(SPR 6173) select by phrase option is not uppercased"*/
/*  *5     6-NOV-1990 11:30:56 JULIAN "(SPR 5882) uppercased all parameter"*/
/*  *4    27-DEC-1989 13:54:33 JULIAN "(SPR -1) comment out fi_push fi_pop for X (Kee)"*/
/*  *3    23-OCT-1989 10:35:31 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/*  *2    26-JUN-1989 10:56:19 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:11 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GPRM.C*/

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

   NAME: MP_GET_PARAMETER.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: ROUTINE TO GET THE INITIAL PARAMETER INPUT FOR THE
		MAP DEFINITION BEING BUILT BY THE MAP DEFINITION DIALOG.
		PARAMETER INPUT DIALOG IS DEPENDENT ON PARAMETER TYPE.

   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_fi.h"
#include "esi_md_dbdefs.h"
#include "esi_md.h"


/* * FUNCTION TITLE * */
/* ------------------ */

publicdef INT mp_get_parameter(edit_flag,key_info,parm_string,parmstruc)

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
MD_STATE_WORKSPACE *md_state;
INT i;
BOOL temp_status[6];

/* ********************************************************************** */

/*fi_push();*/
am_open_workspace("MAPDEF_STATE",AM_GLOBAL,(VOIDPTR *)&md_state);


/* * TURN OFF BUTTONS UNTIL PARAMETER ADDED * */
/* ------------------------------------------ */

for (i=0; i<6; i++)
    {
    temp_status[i] = md_state->button_status[i];
    }

md_state->button_status[0] = OFF;
md_state->button_status[1] = OFF;
md_state->button_status[2] = OFF;
md_state->button_status[3] = OFF;
md_state->button_status[4] = OFF;
md_state->button_status[5] = OFF;


/* * SET UP TYPE OF DIALOG * */
/* ------------------------- */

switch (key_info->val_code)
    {
/* ------------------------------------------------------------------------ */
    case TLU$: case TLIST$: case PLU$: case PLIST$: case UOM$: case SLE$:
/* ------------------------------------------------------------------------ */

	status = mp_parm_from_box(edit_flag,key_info,parm_string,parmstruc);
	break;


/* ------------------------------------------------------------------------ */
    case FILER$: case FILEW$:
/* ------------------------------------------------------------------------ */

	status = mp_parm_from_file(edit_flag,key_info,parm_string,parmstruc);
	break;

/* ------------------------------------------------------------------------ */
    case UDLG$:
/* ------------------------------------------------------------------------ */

        status = mp_parm_from_dialog(edit_flag,key_info,parm_string,parmstruc);
        break;

/* ------------------------------------------------------------------------ */
    case TCP$:
/* ------------------------------------------------------------------------ */

        status = mp_parm_from_tcp(edit_flag,key_info,parm_string,parmstruc);
        break;


/* ------------------------------------------------------------------------ */
    default:			    /* INPUT TEXT DIALOG		    */
/* ------------------------------------------------------------------------ */

	status = mp_parm_from_text(edit_flag,key_info,parm_string,parmstruc);
	break;
    }


    /* upper case the parameter */
    if (status == SUCCESS AND key_info->val_code != SQL$)
	{
	ts_sto_upper(parm_string);
	if((*parmstruc)->parmtype EQUALS 1)
	    ts_sto_upper((*parmstruc)->parmval.cval);
	}

/* * RESET BUTTONS * */
/* ----------------- */

for (i=0; i<6; i++)
    {
    md_state->button_status[i] = temp_status[i];
    }
/*fi_pop();*/

return status;
}
