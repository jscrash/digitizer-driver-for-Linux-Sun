/* DEC/CMS REPLACEMENT HISTORY, Element NLGQTAB_INF.C */
/* *2     9-AUG-1990 19:10:52 GILLESPIE "(SPR 2001) Change #if HOST to #ifdef" */
/* *1     5-MAR-1990 12:25:30 GILLESPIE "Renamed files (SPR 6015)" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLGQTAB_INF.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NLG_INQ_table_INFO.C*/
/* *2    11-AUG-1989 09:05:05 GORDON "(SPR 101) Quiet compiler info messages"*/
/* *1    21-JUL-1989 09:38:02 GILLESPIE "Fortran binding for n-List function"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLG_INQ_table_INFO.C*/

/******************************************************************************

                Copyright Finder Graphic Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphic Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphic, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl.h"

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifdef vms
publicdef INT nlg_inq_table_info(nlist, table, opcode, nCol,
				  Col_List, Var_List)
NLIST_HEADER *nlist;
INT *opcode;
UINT *nCol, *Col_List,*table;
DOUBLE * Var_List;
{
    INT status;
    VOIDPTR *newval;

    mk_int(Var_List,&newval,*nCol);
    status = nl_inq_table_info(*nlist,*table,*opcode,*nCol,Col_List,newval);
    tc_free(newval);
    return status;
}
#endif

#if UNIX
publicdef INT nlg_inq_table_info_(nlist, table, opcode, nCol,
				  Col_List, Var_List)
NLIST_HEADER *nlist;
INT *opcode;
UINT *nCol, *Col_List,*table;
DOUBLE * Var_List;
{
    INT status;
    VOIDPTR *newval;

    mk_int(Var_List,&newval,*nCol);
    status = nl_inq_table_info(*nlist,*table,*opcode,*nCol,Col_List,newval);
    tc_free(newval);
    return status;
}
#endif

#ifdef primos
publicdef INT nlg_inq_table_info(nlist, table, opcode, nCol, Col_List,
				  Var_List)
NLIST_HEADER nlist;
INT opcode;
UINT nCol, Col_List,table;
DOUBLE Var_List;
{
    INT status;
    VOIDPTR *newval;

    mk_int(&Var_List,&newval,nCol);
    status = nl_inq_table_info(nlist,table,opcode,nCol,&Col_List,newval);
    tc_free(newval);
    return status;
}
#endif
