/* DEC/CMS REPLACEMENT HISTORY, Element LD_CONVERT_ASC_LOG.C */
/* *3    20-SEP-1991 11:28:11 LOEBL "(SPR 0) Fix compiler Warning" */
/* *2     8-MAR-1990 10:28:42 VINCE "(SPR -1) fixed init of FMT" */
/* *1     8-MAR-1990 10:17:16 VINCE "name changed from convert_asc_log.c" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_CONVERT_ASC_LOG.C */
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

/*****************************************************************************
    This program converts logs from the format shown below to
    Finder Ascii Log Loader format.

    Usage - CONVERT_ASC_LOG input_file_name output_file_name

input file format:
line 1: <the wells UWI>
line 2: number_of_samples starting_depth depth_sample_interval
line 3: depth_of_first_sample dt dt dt dt dt dt dt dt dt dt 10
line 4: depth_of_11th_sample  dt dt dt dt dt dt dt dt dt dt 10
...
line N: depth_of_Xth_sample   dt dt dt dt                   4

    The last number on the line is the number of samples on the line.  It is
    normally 10 except on the last line.

    The numbers on each line are delimited by spaces.

 *****************************************************************************/

#include "stdio.h"
#include "stdlib.h"

#define NUL  '\0'

int main (argc, argv)
    int  argc;
    char *argv[];
{
    int    ii, qq, nsamples;
    double depth, interval, s[10];
    FILE   *in, *out;
    char   *cp, uwi[20];
    char   inline[85],  outline[85];
    char   infile[300], outfile[300];
    static char   *fmt[] = {
			"",
			"%lf",
			"%lf %lf",
			"%lf %lf %lf",
			"%lf %lf %lf %lf",
			"%lf %lf %lf %lf %lf",
			"%lf %lf %lf %lf %lf %lf",
			"%lf %lf %lf %lf %lf %lf %lf",
			"%lf %lf %lf %lf %lf %lf %lf %lf",
			"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
			"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf" };

    infile[0]  = NUL;
    outfile[0] = NUL;

    if (argc > 1) strcpy (infile,  argv[1]);
    if (argc > 2) strcpy (outfile, argv[2]);

    if (strlen(infile) == 0) {
	printf ("Enter the input file name : ");
	gets (infile);
    }
    if (strlen(infile) == 0) return;

    if (strlen(outfile) == 0) {
	printf ("Enter the output file name : ");
	gets (outfile);
    }
    if (strlen(outfile) == 0) return;

    if ( (in=fopen(infile, "r")) == (FILE *)0) {
	printf ("Can't open input file '%s'.", infile);
	return;
    }

    if ( (out=fopen(outfile, "w")) == (FILE *)0) {
	printf ("Can't open output file '%s'.", outfile);
	return;
    }

    /* uwi		*/
    cp=fgets(uwi, sizeof uwi, in);
    uwi[strlen(uwi)-1] = NUL;
    fprintf (out, "UWI    (%s)\n", uwi);

    /* samples, starting depth, interval	*/
    cp=fgets(inline, sizeof inline, in);
    sscanf (inline, "%d %lf %lf", &nsamples, &depth, &interval);
    fprintf (out, "SAMPLE (%f2.1)\n", interval);
    fprintf (out, "CURVES (DEPTH,DT)\n");

    /* data points */
    while ( (cp=fgets(inline, sizeof inline, in)) != NULL) {
	inline[strlen(inline)-1] = NUL;

        qq = 10;
	if (nsamples < qq) qq = nsamples;

	sscanf (inline, fmt[qq], 
			&s[0], &s[1], &s[2], &s[3], &s[4],
			&s[5], &s[6], &s[7], &s[8], &s[9]);

	for (ii=0; ii<qq; ii++) {
	    fprintf (out, "%7.1f %7.1f\n", depth, s[ii]);
	    depth += interval;
	}

	nsamples -= qq;
    }

    fclose(in);
    fclose(out);

    return;
}
