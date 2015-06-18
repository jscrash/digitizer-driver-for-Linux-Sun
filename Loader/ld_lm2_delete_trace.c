/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_DELETE_TRACE.C */
/* *2     1-MAY-1991 22:14:33 KEE "(SPR -1) Landmark Import & Export" */
/* *1     1-MAY-1991 20:53:48 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_DELETE_TRACE.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* This module is exactly like the one LG_LTDM_DELETE_TRACE from FINDER
   except that this one works.  I added two patches below to fix it.

	Rod Hanks		June 11th, 1990
*/
#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif
#ifndef ESI_LG_ERR_H
#include "esi_lg_err.h"
#endif
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#if USE_PROTOTYPES
publicdef INT ld_lm2_delete_trace(UWI uwi, CHAR traceCname[], INT traceID)
#else
publicdef INT ld_lm2_delete_trace( uwi, traceCname, traceID)
UWI uwi;                          /*%P Unique Well Identifier.              */
CHAR traceCname[];
INT traceID;
#endif
{
    INT status = SUCCESS;         /*   result status.                        */
    LOG_TRACE_STRUCT trace;       /*   workspace of LOG_TRACE_STRUCT.        */
    FILENAME	tempFilename;	  /*   One of my patches */

/* ************************************************************************* */

    if( uwi  EQUALS NULL OR traceCname EQUALS NULL )
         return LG_INVALID_POINTER;

    if( lgz_check_exist( uwi) OR lgz_check_exist(     traceCname     ) )
         return LG_INVALID_POINTER;

    tc_zeroes(&trace, sizeof(     LOG_TRACE_STRUCT    )  );
    strcpy(trace.uwi, uwi);
    strcpy(trace.trace_name, traceCname );
    
    status = lg_ltdm_parser( &trace    );

    if(  status IS_NOT_EQUAL_TO SUCCESS     )
         return status;

                                /*   If there is no version number, returns  */
                                /*   error message.                          */

    if(  trace.version <= 0 )
         return LG_INVALID_VERSION;

                                /*   check the existance of the trace.       */

    status = lg_ltdm_check_write( trace.uwi, trace.trace_name);
    if( ! status  )
         return LG_TRACE_NOT_FOUND;

                                /*   delete the entry from the database, if  */
                                /*   succeed, then deletes the trace from the*/
                                /*   trace file.                             */

    if ((status =  lgz_get_trace_names( &trace )) IS_NOT_EQUAL_TO SUCCESS)
	{
	return status;
	}


    trace.trace_id = traceID;
    ho_add_path("ESI$LOGS", trace.filename, sizeof (FILENAME), tempFilename);
/*
					Here are my patches.  First, the 
					program could not delete the oracle
					entry, because LGZ_DELETE_DB_ENTRY
					depends on trace_id, and this routine
					doesn't set it up.  Second, the
					program could not delete the nlist,
					because the database does not contain
					the pathname for the file, and this
					routine didn't add it.
*/
    if ((status = lgz_delete_db_entry( &trace )) IS_NOT_EQUAL_TO SUCCESS)
	{
	return status;
	}

    return nl_delete_nlist(tempFilename, trace.nlist_id);
/*
					I replaced trace.filename with
					tempFilename in the call above.
*/
}

/* END-CODE */
