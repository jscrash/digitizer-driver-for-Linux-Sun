/* DEC/CMS REPLACEMENT HISTORY, Element SL_ISEL_LEASE.C */
/* *4     9-NOV-1990 16:16:07 JULIAN "(SPR 0) initial change" */
/* *3     6-NOV-1990 11:39:10 JULIAN "(SPR 0) initial changes" */
/* *2    30-JUL-1990 17:24:16 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:17:02 JULIAN "init lease nlist" */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_ISEL_LEASE.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_ISEL_LEASE.C*/
/* *1    13-AUG-1990 17:10:59 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_ISEL_LEASE.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_ISEL_LIST.C*/
/* *6    23-JAN-1990 07:45:59 WALTERS "(SPR 0) Fix bug in nl_init_nlist_info"*/
/* *5    17-JAN-1990 15:39:17 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4     9-NOV-1989 16:37:36 GILLESPIE "(SPR 1) Up blocksize to 500 from 100"*/
/* *3    30-OCT-1989 14:37:50 JULIAN "(SPR 5039) Seismic Select List Mods"*/
/* *2    11-AUG-1989 20:19:47 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *1    19-JUN-1989 13:25:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_ISEL_LIST.C*/

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

#include "esi_sl.h"
#ifndef ESI_NL_H
#include "esi_nl.h" 
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
typedef CHAR LM_LEASE_ID[24];

/* *************************************************************************

	ROUTINE: SL_INIT_SELECT_LIST

	DESCRIPTION:
		 This function will initialize the nlist, and set a user 
	structure into the nlist.
	
	STATUS CODES:
		SUCCESS;
		SL_INVALID_DATA_TYPE;

	INPUT:
		DATA_TYPE =(INT )the data type of select list.
		SELECT_LIST = (NLIST_HEADER *)a pointer points to the returned
				select list.
	OUTPUT:
		SELECT_LIST = (NLIST_HEADER *)a pointer points to the returned
				select list.

		STATUS CODES.

	AUTHOR: James Chang

    ********************************************************************** */

publicdef INT slz_init_lease_sl(select_list )
NLIST_HEADER *select_list;   /* nlist */
{
    /* ******************************************************************* */

    static UINT ndim = 1;	    /* number of dimensions in the nlist */
    static INT units[1] = { 0 };
    static size_t data_sizes[1] = { sizeof ( LM_LEASE_ID) }; 
    static INT data_types[1] = { CHAR_TYPE }; 
    static INT value_inc[1] = { VALUE_LIST }; 
    static UINT dim_list[1] = { 1 };
    static CHAR *c_name[2] = {"LEASE_ID",NULL };
    UINT blocksize = 500;  /* blocksize to allocate on each stroke, in #points */
    INT status;
    UINT new_stroke = 0;

/* *********************************************************************** */

    status = nl_init_nlist( select_list, ndim, units, (size_t *)data_sizes, data_types, 
	    value_inc, blocksize );
    if (status != SUCCESS) return status;

    status = nl_set_nlist_info(*select_list, NL_NLIST_COLUMN_NAME, 
		      ndim, dim_list, (VOIDPTR)c_name);
    if (status != SUCCESS) return status;

/* set the default values into the user structure */
    status = sl_fill_select_struct(LEASE_DATA_TYPE, *select_list );
    return status;
}

