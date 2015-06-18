/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_I_W_S.C*/
/* *1    19-JUN-1989 13:04:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_DM_I_W_S.C*/

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

#include "esi_gl_defs.h"
#include "esi_lg_defs.h"
#include "esi_lg_ldefs.h"
#include "esi_tc.h"
#include "esi_ts.h"

/* ***************************************************************************

    ROUTINE: LG_LTDM_INQ_WELL

    DESCRIPTION
         Obtain a list of all traces in a well meeting conditions specified
         by a SQL select phrase.

  ************************************************************************** */
INT lg_ltdm_inq_well(uwi, sqlString, tablePtr)
UWI uwi;                          /*   the requested UWI.                 */
CHAR sqlString[];                 /* the selecting SQL phrase.            */
LOG_TRACE_STRUCT ***tablePtr;     /* a pointer of table includes pointers */
                                  /* of char. string.                     */

/* ************************************************************************** */
{
     LOG_TRACE_STRUCT **returnPtr;
     CHAR **tableNamePtr;         /*  returned table.                      */
     INT status = SUCCESS;
     INT totalTraces = 0;
     INT i;

                                  /* retrieve all trace names in the well     */

     status = lg_ltdm_inq_well_trace(uwi, sqlString,
                                   &tableNamePtr, &totalTraces);

     if(  status != SUCCESS )
	{
         return status;
	}

     *tablePtr = returnPtr = (LOG_TRACE_STRUCT **) tc_zalloc
                         ( sizeof( LOG_TRACE_STRUCT *)  * (totalTraces + 1));

     for( i = 0;
          i < totalTraces;
          i++)
     {

                                  /* require the structure of specific trace.*/

         returnPtr[i] = (LOG_TRACE_STRUCT *) 
                        tc_zalloc( sizeof(LOG_TRACE_STRUCT) );
         status = lg_ltdm_inq_trace_struct(uwi, tableNamePtr[i],"",
							 returnPtr[i]);
	 if (status != SUCCESS)
	    {
	    break;
	    }
     }

    ts_tcp_free(tableNamePtr);

     return status;
}
