/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_FRE_T.C*/
/* *1    19-JUN-1989 13:04:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_FRE_T.C*/

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


/* ******************************************************************************

    MODULE: LG_LTDM_DB_TOOLS

    DESCRIPTION
         A module of routines of Log Trace Data Manager.  This module includes
         all routines which perform function on database, for instance, to
         retrieve the ZONE VALUE, to retrieve the TOP and BASE of Log traces,
         to lock and unlock the record, to update the data base,...

         This module will access two tables in data base, <AREA>.LOGS,
         <AREA>.TOPS.

    Author:   James Chang
    *************************************************************************** */

#include "esi_gl_defs.h"
#ifndef ESI_LG_DEFS_H
#include "esi_lg_defs.h"
#endif
#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif


/*  *****************************************************************************

    ROUTINE:  LG_LTDM_FREE_TRACE

    DESCRIPTION
	Frees structure and nlist.

    *************************************************************************** */
publicdef INT lg_ltdm_free_trace( traceStruct     )
    /*   ********************************************************************* */
LOG_TRACE_STRUCT   *traceStruct;  /*%P a pointer of LOG_TRACE_STRCUT.          */

{
    INT  status = SUCCESS;        /*   the returned status.                    */
 
/*  ************************************************************************** */

    if(  traceStruct EQUALS (LOG_TRACE_STRUCT *)NULL )
         return status;
  
				  /*   free the nlist.                        */

    if(  traceStruct -> ptrace IS_NOT_EQUAL_TO NULL  )
          status = nl_free_nlist( traceStruct -> ptrace );

    return status;
}
