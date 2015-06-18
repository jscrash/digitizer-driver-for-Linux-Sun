/* DEC/CMS REPLACEMENT HISTORY, Element LGZ_DM_U_R.C*/
/* *3    17-JUL-1991 16:24:48 JANTINA "(SPR 0) Get increment from nlist"*/
/* *2    27-JUL-1989 22:25:17 GILLESPIE "(WBS 1.1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:04:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LGZ_DM_U_R.C*/

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

/* **************************************************************************

    Author:   James Chang
    ************************************************************************ */

#include "esi_lg_defs.h"
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif



/* ***************************************************************************

    ROUTINE:  LGZ_UPDATE_RANGES

    DESCRIPTION
         To update TOP, BASE, MIN, AND MAX values in a log trace.  If the
    field is equals to zero, then set the field to be the value calculated 
    of log trace, otherwise keep it same.

    ************************************************************************ */

publicdef INT lgz_update_ranges(   traceStruct    )
    /*********************************************************************** */
LOG_TRACE_STRUCT *traceStruct;    /*   the pointer of new LOG_TRACE_STRUCT.    */
{
    UINT dimList[2];
    size_t dataSize[2];
    INT dataType[2];
    INT units[2];
    FLOAT value1, value2;
    VOIDPTR valueList[2];
    FLOAT inc;
    UINT Col_List[1];
    VOIDPTR Var_List[1];

    INT status;
/* ************************************************************************* */

    dimList[0] = 1, dataSize[0] = sizeof (FLOAT), dataType[0] = FLOAT_TYPE,
	units[0] = 0, valueList[0] = (VOIDPTR)&value1;
    dimList[1] = 2, dataSize[1] = sizeof (FLOAT), dataType[1] = FLOAT_TYPE,
	units[1] = 0, valueList[1] = (VOIDPTR)&value2;

					/* check top and min field, if it is 
                                           zero, set it to be the value of 
                                           Log Trace.   */

    status = nl_inq_nlist_info_convert(traceStruct -> ptrace, NL_NLIST_MIN, 
                       2, dimList, valueList, units, dataSize, dataType);
    if(  status )
        return status;
    traceStruct -> top = value1;
    traceStruct -> min = value2;

					/* check base and max field, if it is 
                                           zero, set it to be the value of 
                                           Log Trace.   */

    status = nl_inq_nlist_info_convert(traceStruct -> ptrace, NL_NLIST_MAX, 
                       2, dimList, valueList, units, dataSize, dataType);
    if(  status )
        return status;
    traceStruct -> base = value1;
    traceStruct -> max = value2;

    Col_List[0] = 1;
    Var_List[0] = (VOIDPTR)&inc;
    status = nl_inq_table_info(traceStruct -> ptrace, 1, NL_TABLE_INCREMENT, 
                       1, Col_List, Var_List);
    if(  status )
        return status;
    traceStruct -> dinc = inc;
    
    return status;
}
