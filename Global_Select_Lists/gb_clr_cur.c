/* DEC/CMS REPLACEMENT HISTORY, Element GB_CLR_CUR.C*/
/* *5    30-JUL-1990 17:21:45 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *4     1-NOV-1989 09:21:51 GILLESPIE "(SPR 30) REMOVE REFS TO GRAPHIC OBJECT SELECTS"*/
/* *3    11-AUG-1989 20:36:13 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *2    11-AUG-1989 09:35:20 JULIAN "(SPR -1) fixing statement not reached problem"*/
/* *1    19-JUN-1989 12:51:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_CLR_CUR.C*/
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

#include "esi_gb.h"
#include "esi_qs.h"
#include "esi_gb_err.h"

publicdef INT  gb_clear_current_list (data_type)
SL_DATA_TYPE data_type;

			     /*	 The QS variables for the given data type
			     	 are cleared to create a state of
			     	 no-current-list for that data type.	    */
{
    INT qs_var;
    INT status;   

    status = sl_inq_dtype_int("QS_LIST_EXISTS",data_type,&qs_var);
    if(status != SUCCESS) 
         return ( GB_BAD_DATA_TYPE );
    qs_set_i (qs_var, FALSE);

    status = sl_inq_dtype_int("QS_CUR_COUNT",data_type,&qs_var);
    qs_set_i (qs_var, 0);
    
    status = sl_inq_dtype_int("QS_CUR",data_type,&qs_var);
    qs_set_c (qs_var, "",0);

    status = sl_inq_dtype_int("QS_CUR_LIST",data_type,&qs_var);
    qs_set_c (qs_var, "",0);

   return (SUCCESS);
}
