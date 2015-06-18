/* DEC/CMS REPLACEMENT HISTORY, Element NL_TO_ASCII.C */
/* *3     8-MAR-1990 10:49:50 VINCE "(SPR -1) fixed a non-portability problem" */
/* *2     7-MAR-1990 18:02:16 VINCE "(SPR -1) Initial Checkin" */
/* *1     7-MAR-1990 17:57:33 VINCE "Program created for Enron to dump nLists in ascii format." */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_TO_ASCII.C */

#include "esi_gl_defs.h"
#include "esi_nl.h"
#include "esi_nl_meth_hdr.h"

int main (argc, argv)
    int  argc;
    char *argv[];
{
    int		     ii, status;
    int		     ncols, ntables;
    UINT	     collist[100];
    FILENAME	     name, outfile;
    FILE	     *out;
    NLIST_ID	     id;
    NLIST_HEADER     nlist;
    NLM_PRINT_HEADER prthdr;

    if (argc < 4) {
	printf ("\nUsage - NL_TO_ASCII nList_file nList_id output_file\n\n");
	return;
    }

    strcpy (name,    argv[1]);
    strcpy (id,      argv[2]);
    strcpy (outfile, argv[3]);

    status = nl_read_nlist (&nlist, name, id);
    if (status != SUCCESS) {
	printf ("Failed while tring to read nList with error code %d.\n\n",
		    status);
	return;
    }

    out = fopen(outfile, "w");
    if (out == (FILE *)0) {
	printf ("Can't open the output file %s.\n\n", outfile);
	status = nl_free_nlist (nlist);
	return;
    }

    status = nl_inq_nlist_int (nlist, NL_NLIST_NCOL,    &ncols);
    status = nl_inq_nlist_int (nlist, NL_NLIST_NTABLES, &ntables);

    for (ii=0; ii<ncols; ii++) collist[ii] = (UINT)(ii + 1);

    prthdr.nCol			= ncols;
    prthdr.Col_List		= collist;
    prthdr.first_table		= 1;
    prthdr.last_table		= ntables;
    prthdr.print_nlist_header	= FALSE;
    prthdr.print_table_header	= FALSE;
    prthdr.print_data		= TRUE;
    prthdr.outFile		= out;

    status = nl_invoke_nlist_method (nlist, NL_PRINT_METHOD, &prthdr);
    if (status != SUCCESS) {
	printf ("Error during conversion of %s/%s; status code = %d\n\n",
		    name, id, status);
    }
    fclose (out);

    status = nl_free_nlist (nlist);
    return;
}
