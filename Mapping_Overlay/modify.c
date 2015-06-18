/* DEC/CMS REPLACEMENT HISTORY, Element MODIFY.C */
/* *1    13-NOV-1990 11:26:05 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element MODIFY.C */
/* DEC/CMS REPLACEMENT HISTORY, Element MODIFY.C */
/* *2     9-NOV-1990 10:42:07 MING "change ho_find_files2 to ho_find_filenames" */
/* *1    24-MAY-1990 13:11:30 JULIAN "perform a substitution in multiple files from the command line." */
/* DEC/CMS REPLACEMENT HISTORY, Element MODIFY.C */

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_ts.h"

INT main (argc, argv)
INT  argc;
CHAR *argv[];
{
    INT	    ii, nfiles, pfiles, nlines;
    TCP	    file_list;
    CHAR    file[130], line[300];
    CHAR    *cp;
    BOOL    match_case;
    FILE    *in, *out;

    if (argc < 4) {
	printf ("\nUsage - MODIFY filename search_string replace_string ");
	printf ("[-MATCH]\n\n");
	printf ("filename may contain a wildcard.\n");
	printf ("-MATCH will replace only an exact case match.\n\n");
	return EXIT_SUCCESS;
    }

    match_case = FALSE;
    if ((argc >= 5) && (argv[4][1] == 'M')) match_case = TRUE;

    ho_find_filenames (argv[1], &file_list);
    nfiles = ts_tcp_len (file_list);

    if (nfiles == 0) {
	printf ("No files found using '%s'\n", argv[1]);
	return EXIT_SUCCESS;
    }

    pfiles = 0;
    for (ii=0; ii<nfiles; ii++) {
	cp = strrchr (file_list[ii], ';');
	if (cp != NULL) *cp = NUL;

	if ((in=fopen(file_list[ii],"r")) == (FILE *)0) {
	    printf ("Can't open '%s' for reading.\n", file_list[ii]);
	    continue;
	}

	if ((out=fopen(file_list[ii],"w")) == (FILE *)0) {
	    printf ("Can't open '%s' for writing.\n", file_list[ii]);
	    continue;
	}

	cp = strrchr (file_list[ii], ']');
	if (cp == NULL) cp = strrchr(file_list[ii],':');
	if (cp == NULL) cp = file_list[ii];
	else cp++;
	printf ("Processing (%2d of %2d) '%s'... ", ii+1, nfiles, cp);
	pfiles++;

	nlines = 0;
	while (fgets(line, sizeof line, in) != NULL) {
	    fputs(ts_subs(line, argv[2], argv[3], match_case), out);
	    nlines++;
	}

	printf ("%4d lines.\n", nlines);

	fclose (in);
	fclose (out);
    }

    printf ("\n%d files processed out of %d\n\n", pfiles, nfiles);

    return EXIT_SUCCESS;
}
