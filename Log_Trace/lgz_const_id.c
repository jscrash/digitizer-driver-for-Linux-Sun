/* DEC/CMS REPLACEMENT HISTORY, Element LGZ_CONST_ID.C */
/* *2     6-SEP-1989 08:10:13 GORDON "(SPR 200) Gulf changes (August trip)" */
/* *1     6-SEP-1989 08:09:04 GORDON "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element LGZ_CONST_ID.C */
#include "esi_c_lib.h"
#include "esi_lg_defs.h"
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
/******************************************************************************

		Copyright Finder Graphics Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF 

			Finder Graphics Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work) 


******************************************************************************/

/* given the fields SOURCE, TRACE, SERVICE and VERSION
in the log trace structure, construct an identifier of the form

                 [SOURCE:]TRACE[.SERVICE];VERSION

The only fields required are TRACE and VERSION.

The results are placed in the field nlist_id of the trace structure.
Any previous entry in this field is overwritten.

INPUT : a valid pointer to a log trace structure
OUTPUT: the field nlist_id of the structure
RETURNS: SUCCESS
*/

publicdef INT lgz_construct_trace_id(traceStruct)
LOG_TRACE_STRUCT *traceStruct;    /*%P pointer of trace structure.         */
{
    CHAR *ps;

     ps = (CHAR *)traceStruct -> nlist_id;
     if(  IS_STRING( traceStruct -> source)  )
     {
          ps += sprintf( ps, "%s:", traceStruct -> source);
     }
     ps += sprintf( ps, "%s", traceStruct -> trace);
     if(   IS_STRING(  traceStruct -> service )  )
     {
           ps += sprintf(ps, ".%s", traceStruct -> service );
     }     
     sprintf( ps, ";%d", traceStruct -> version);

     return SUCCESS;
}
