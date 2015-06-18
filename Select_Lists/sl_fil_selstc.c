/* DEC/CMS REPLACEMENT HISTORY, Element SL_FIL_SELSTC.C*/
/*  6    20-FEB-1992 16:26:19 LOEBL "(SPR 6138) Change to ZZ_ prefix for temp names"*/
/* *5    30-JUL-1990 17:23:59 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *4     6-FEB-1990 13:02:23 MING "(SPR 0) change date/time declaration to new data type DATE_TIME"*/
/* *3     1-NOV-1989 14:59:55 GILLESPIE "(SPR 30) Remove ref to graphic object data type"*/
/* *2    12-AUG-1989 21:27:49 CONROY "(SPR 0) Revised SL convention of $$ hidden lists"*/
/* *1    19-JUN-1989 13:25:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_FIL_SELSTC.C*/
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

#include "esi_sl.h"
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#include "esi_sl_err.h"

/* ***********************************************************************

	ROUTINE: SL_FILL_SELECT_STRUCT

	DESCRIPTION:
		 This function will make up a default name, and query then 
	assign default values for whatever select_structure values it can. 

	STATUS CODES:
		SUCCESS;
		SL_INVALID_DATA_TYPE;
		NL ERROR MESSAGES.
	
	INPUT:
		DATA_TYPE =(INT )The data type of select list.
		SELECT_LIST =(NLIST_HEADER )The N-list structured select list.

	OUTPUT:	
		SELECT_LIST =(NLIST_HEADER )The N-list structured select list.
		STATUS CODES.

	AUTHOR: James Chang.

     ************************************************************************ */
publicdef INT sl_fill_select_struct( data_type, select_list )
     /* ********************************************************************* */
SL_DATA_TYPE data_type;		    /* wells, seismic, leases, other */
NLIST_HEADER select_list;  /* pointer to nlist */
{

    SELECT_STRUCT temp_struct;
    INT status;
    DATE_TIME date_time;          /* date time string for default name */
    CHAR data_type_name[32];
    
    tc_zeroes((VOIDPTR)&temp_struct, sizeof(SELECT_STRUCT) );
    
				/* Current version number of select list */
				/* structure is in esi_gb.h 		 */

    temp_struct.version_of_select = SELECT_VERSION;
    
				/* Name is created by concatonating data */
				/* type name with time & date 		 */
    
    ho_date_tag(date_time);
    status = sl_inq_dtype_string("DATA_TYPE_NAME",data_type,data_type_name);
    if(status != SUCCESS) return status;
    sprintf( temp_struct.name, "$$%s_%s",data_type_name, date_time );
    
					/* add project name */

    qs_inq_c( QS_PROJECT_NAME, temp_struct.project_name, (INT *) 0);

					/* add account name */
    
    qs_inq_c( QS_FINDER_ACCOUNT, temp_struct.account_name, (INT *) 0);

					/* set the permanent_falg to be false*/

    temp_struct.permanent_flag = FALSE;
    tc_today(temp_struct.date_created);
    strcpy(temp_struct.expiration_date, temp_struct.date_created);

					/* Current item is set to the first */
					/* element (0) of nlist.	    */

    temp_struct.current_item = 0;
    
					/* Insert data type code.	    */

    temp_struct.data_type = data_type;
    
					/* put updated structure back into  */
					/* the nlist */

    status = nl_set_nlist_user_struct( select_list, (BYTE *)&temp_struct, 
				sizeof(SELECT_STRUCT));
    
    return status;
}   
