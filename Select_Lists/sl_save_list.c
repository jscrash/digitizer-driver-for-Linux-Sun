/* DEC/CMS REPLACEMENT HISTORY, Element SL_SAVE_LIST.C*/
/*  7    13-FEB-1992 15:46:50 JULIAN "(SPR 5101) added some checking before deleting&replacing nlist"*/
/* *6    24-JUL-1991 16:05:15 JULIAN "(SPR 3373) added check on select list with ' mark"*/
/* *5    30-JUL-1990 17:25:04 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *4    17-MAR-1990 17:19:23 GILLESPIE "(SPR 1) Free up any temporary list found"*/
/* *3     1-NOV-1989 14:55:00 GILLESPIE "(SPR 1) Remove ref to graphic object type"*/
/* *2    29-JUL-1989 09:32:35 GILLESPIE "(SPR 1) Add esi_nl_err.h"*/
/* *1    19-JUN-1989 13:25:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SL_SAVE_LIST.C*/

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

/* This function writes a select list to disk */

#include "esi_c_lib.h"
#include "esi_sl.h"
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
#ifndef ESI_NL_ERR_H
#include "esi_nl_err.h"
#endif
#ifndef ESI_SL_MSG_H
#include "esi_sl_msg.h"
#endif
#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

/* **********************************************************************

    ROUTINE: SL_SAVE_LIST

    DESCRIPTION:
        This function writes a nlist with a select list user structure onto
        disk in a file for the type of select lists that is indicated within
        the user structure. Whether an existing nlist with the same name
	is overwritten is determined by the overwrite_flag. If batch mode,
        then don't question user. 

    STATUS CODES:
	SUCCESS;
	SQL ERROR CODES;
	SL_INVALID_SELECT_LIST;
	SL_INVALID_DATA_TYPE;
	SL_INVALID_OVERWRITE_FLAG;
	SL_SELECT_LIST_EXIST.

    INPUT:
	SELECT_LIST  =(NLIST_HEADER )A N-list structured select list.
	OVERWRITE_FALG=(INT ) The flag of overwritten.

    OUTPUT:
	STATUS CODES.

    ******************************************************************** */
publicdef INT sl_save_list( select_list , overwrite_flag )
    /* *******************************************************************/
NLIST_HEADER select_list;   /* select list structure. Contains select
                                list user structure */
INT overwrite_flag;	    /* =0 don't overwrite
                               =1 question user before overwriting
                               =2 always overwrite  */
                               
{
	INT status;
	INT data_type = 0;
	INT if_batch;		    /* flag for if batch mode */
	size_t num_bytes;	    /* Number of bytes in user structure */
	INT overwrite = TRUE;
	INT num_items =0;
	SELECT_STRUCT temp_struct, temp_struct_old;  
        CHAR data_type_name[32];				    
				    /*  temporary list structure.	 */
	FILENAME nlist_file;
	CHAR prompt[STMAX];
	ANALYST_NAME current_account;
	NLIST_HEADER temp_list;

/* Test if select_list is null */
    if (select_list == NULL )
	return( SL_INVALID_SELECT_LIST );
    
/* Test if a valid overwrite_flag is given */
    if (( overwrite_flag < 0) || (overwrite_flag > 2))
	return( SL_INVALID_OVERWRITE_FLAG );

    overwrite = ( overwrite_flag EQUALS 0 ) ? FALSE : TRUE;

/* If being run in batch mode don't query user */
    qs_inq_i( QS_IF_BATCH, &if_batch );
    if ((if_batch) && (overwrite_flag EQUALS 1 ))
	overwrite = FALSE;
    

/* Extract data type from select list */    
    status = nl_inq_nlist_user_struct( select_list, sizeof( SELECT_STRUCT ),
	    (VOIDPTR)&temp_struct, &num_bytes );
    if (status)
	return( status );

/* Test if select list name has apostrophe in the middle that will mess up
    the sql stmt when trying to update/insert into the data base */	
    if(strstr(temp_struct.name,"'") != NULL)
	return SL_INVALID_NAME;

    status = nl_inq_nlist_int( select_list, NL_NLIST_NPOINTS, &num_items);
    if (status)
	return( status );


/* Update the Nlist_structured selected list.	     */
	
/* Extract data_type from select list user structure */
    data_type = (INT)temp_struct.data_type;
    
    status = sl_inq_dtype_string("DATA_TYPE_NAME",data_type,data_type_name);
    if(status != SUCCESS) return status;

    strcat(data_type_name,"_SELECTS");
    ho_add_path( "ESI$SELECTS", data_type_name, sizeof (FILENAME), nlist_file );

					/* try to read the specified select */
					/* list, if it exists, compares their*/
					/* accounts, if they are not same,  */
					/* returns error status.	    */

    status = nl_read_nlist( &temp_list, nlist_file, temp_struct.name );
    if( status EQUALS SUCCESS )
    {
       status = nl_inq_nlist_user_struct( temp_list, sizeof( SELECT_STRUCT ),
	    (VOIDPTR)&temp_struct_old, &num_bytes );
	nl_free_nlist(temp_list);
        if(  ARE_DIFFERENT(temp_struct_old.account_name,
		           temp_struct.account_name) )
	    return SL_NOT_SAME_ACCOUNT;	
    }


    status = nl_write_nlist(select_list, nlist_file, temp_struct.name);
    if(  (status EQUALS NL_INVALID_NLIST_NAME) AND  overwrite )
    {

/* check the overwrite_flag, if it is equals to 1 and finder is not in the
   batch mode, asks user to decide whether ove-written or not.
*/
	if( overwrite_flag EQUALS 1)
	{
	    sprintf(prompt, mg_message(SL_LIST_EXISTS),	temp_struct.name);
	    wiyesno(prompt,"", &overwrite, &status);
            if( ! overwrite )
	        return SL_SELECT_LIST_EXIST;
        }
	status = nl_delete_nlist(nlist_file, temp_struct.name);
        if( status )
		return status;
	status = nl_write_nlist(select_list, nlist_file, temp_struct.name);

/* Update the DATA BASE.	*/
        status = slz_save_struct(&temp_struct, num_items, overwrite);

    }
    else
    if(  status EQUALS SUCCESS )
    {
	status = slz_save_struct(&temp_struct, num_items, overwrite);
    }
    return status;
}
