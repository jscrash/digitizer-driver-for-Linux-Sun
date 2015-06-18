/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PARM_OK.C*/
/*  *2    18-MAR-1991 17:10:24 JULIAN "(SPR 0) Lease Mapping init checkin"*/
/*  *1    19-JUN-1989 13:13:58 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PARM_OK.C*/

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

   NAME: MP_PARM_OK.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: ROUTINE TO VERIFY THAT THE INCOMING STRING IS A PROPER
		PARAMETER STRING FOR THE GIVEN VERB/KEY COMBINATION.
		ALLOCATES AND FILLS THE PARAMETER STRUCTURE.

   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#include "esi_am.h"
#include "esi_md_basis.x"



/* * FUNCTION TITLE * */
/* ------------------ */

publicdef BOOL mp_parm_ok(parm_string,verb_ndx,key_ndx,parmstruc,status)

/* * ARGUEMENT LIST * */
/* ------------------ */

CHAR parm_string[MD_MAX_PARM_STRING];
INT verb_ndx;
INT key_ndx;
MD_PARM_STRUCT **parmstruc;
INT *status;

/* * VARIABLE LIST * */
/* ----------------- */

{

MD_PARM_STRUCT *temp_parm;
MD_KEY_INFO *key_info;
BOOL parm_ok = FALSE;

/* ********************************************************************** */

key_info = &(overlay_verbs[verb_ndx].key_list[key_ndx]);


/* * VALIDATE PARAMETER * */
/* ---------------------- */

if (key_info->parm_req)
    {
    switch (key_info->val_code)
	{
	case ONAME$:
	    *status = md_verify_oname(key_info,parm_string,&temp_parm);
	    break;

	case TLU$:
	    *status = md_verify_tlu(key_info,parm_string,&temp_parm);
	    break;

	case TLIST$:
	    *status = md_verify_tlist(key_info,parm_string,&temp_parm);
	    break;

	case PLU$:
	    *status = md_verify_plu(key_info,parm_string,&temp_parm);
	    break;

	case PLIST$:
	    *status = md_verify_plist(key_info,parm_string,&temp_parm);
	    break;

	case UOM$:
	    *status = md_verify_uom(key_info,parm_string,&temp_parm);
	    break;

	case UOMD$:
	    *status = md_verify_uomd(key_info,parm_string,&temp_parm);
	    break;

	case UOMS$:
	    *status = md_verify_uoms(key_info,parm_string,&temp_parm);
	    break;

	case NUM$:
	    *status = md_verify_num(key_info,parm_string,&temp_parm);
	    break;

	case NLIST$:
	    *status = md_verify_numlist(key_info,parm_string,&temp_parm);
	    break;

	case FILER$: case FILEW$:
	    *status = md_verify_file(key_info,parm_string,&temp_parm);
	    break;

	case SLE$:
	    *status = md_verify_sle(key_info,parm_string,&temp_parm);
	    break;
 
	case SQL$:
	    *status = md_verify_sql(key_info,parm_string,&temp_parm);
	    break;

        case UDLG$:
            *status = md_verify_udlg(key_info,parm_string,&temp_parm);
            break;

        case TCP$:
            *status = md_verify_tcp(key_info,parm_string,&temp_parm);
            break;

	default:
	    temp_parm = (MD_PARM_STRUCT *) 
				am_allocate(AM_GLOBAL,sizeof(MD_PARM_STRUCT));
	    temp_parm->parmtype = 1;
	    temp_parm->parmval.cval = (CHAR *)
				am_allocate(AM_GLOBAL,strlen(parm_string)+1);
	    strcpy(temp_parm->parmval.cval,parm_string);
	    temp_parm->nxtparm = NULL_PARM;
	    *status = SUCCESS;
	    break;
	}


/* DETERMINE STATUS AND NEXT ACTION * */
/* ---------------------------------- */

    if (*status EQUALS SUCCESS)
	{
	*parmstruc = temp_parm;
	parm_ok = TRUE;
	}
    }

else
    {
    parm_ok = TRUE;
    *parmstruc = NULL_PARM;
    }

return parm_ok;
}
