/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRINT_NLIST.C */
/* *2     6-JUN-1991 15:21:18 MING "(SPR 0) new checkin" */
/* *1     6-JUN-1991 15:20:30 MING "debug tool to print nlist data" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRINT_NLIST.C */
#include "esi_gl_defs.h"
#include "esi_nl.h"
#include "esi_nl_meth_hdr.h"

publicdef INT nl_print_nlist (nlist)
NLIST_HEADER nlist;
{
    int              ii, status = SUCCESS;
    int              ncols, ntables;
    UINT             collist[100];
    NLM_PRINT_HEADER prthdr;

    status = nl_inq_nlist_int (nlist, NL_NLIST_NCOL,    &ncols);
    status = nl_inq_nlist_int (nlist, NL_NLIST_NTABLES, &ntables);

    for (ii=0; ii<ncols; ii++) collist[ii] = (UINT)(ii + 1);

    prthdr.nCol                 = ncols;
    prthdr.Col_List             = collist;
    prthdr.first_table          = 1;
    prthdr.last_table           = ntables;
    prthdr.print_nlist_header   = TRUE;
    prthdr.print_table_header   = FALSE;
    prthdr.print_data           =  (ntables > 0) ? TRUE : FALSE;
    prthdr.outFile              = stdout;

    status = nl_invoke_nlist_method (nlist, NL_PRINT_METHOD, &prthdr);
    if (status != SUCCESS) 
	{
        printf ("%s\n",mg_message(status));
    	}
    return status;
}
