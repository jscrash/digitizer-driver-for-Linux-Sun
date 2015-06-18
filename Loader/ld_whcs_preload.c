/* DEC/CMS REPLACEMENT HISTORY, Element LD_WHCS_PRELOAD.C*/
/* *12    2-NOV-1990 15:59:24 MING "(SPR 6051) add checking on the status return by fgets"*/
/* *11   28-AUG-1990 13:19:20 VINCE "(SPR 1) added include of esi_gl_defs.h to stop a compiler message"*/
/* *10   12-APR-1990 18:32:32 VINCE "(SPR 1) changed delete to ho_delete_file since Ultrix is not ANSI compatible in this area."*/
/* *9    12-APR-1990 18:30:29 VINCE "(SPR 1) changed delete to ho_de"*/
/* *8    19-MAR-1990 13:57:28 VINCE "(SPR -1) fixed endless loop in well_prod_treatment_2 logical record building"*/
/* *7    15-MAR-1990 18:49:00 VINCE "(SPR -1) fixed 8xxxx processing"*/
/* *6     8-MAR-1990 11:38:11 VINCE "(SPR -1) fixed 2xx01/51 processing"*/
/* *5     8-MAR-1990 10:40:49 VINCE "(SPR -1) fixed a non-portable comparison"*/
/* *4     7-MAR-1990 15:13:13 VINCE "(SPR -1) Fixed delete of TEMP_DST_MUD.DAT"*/
/* *3     6-MAR-1990 18:41:42 VINCE "(SPR -1) changed from seconds Enron trip as per Bill A."*/
/* *2    19-DEC-1989 11:23:55 VINCE "(SPR -1) Mods from Enron-Denver"*/
/* *1     1-JUL-1989 12:23:28 GILLESPIE ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_WHCS_PRELOAD.C*/
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

/* LD_WHCS_PRELOAD - This routine takes an input PI WHCS format file
   and breaks it into true 'flat' files which contains all records
   destined for a single table in the Finder database.  It is suggested
   that the user provide a separate directory for the created files
   for ease of deletion after the loader runs.

   It is assumed that the records being input are sorted by well
   and record type.

   The files built by this routine are then loaded using ORACLE's
   SQL Loader utility to load the database.

   The command line format is (VMS)
      $ LD_WHCS_PRELOAD :== $ESI$BIN:LD_WHCS_PRELOAD
      $ LD_WHCS_PRELOAD filename

 */

#include "esi_gl_defs.h"
#include "esi_stdio.h"
#include "esi_c_lib.h"

#define     MAXLINE         85

int pad (string)
    char *string;
{
    int i;
    for (i=strlen(string); i<MAXLINE; i++) string[i] = ' ';
    string[--i] = NUL;
    return;
}

int parse (record, type, seq, uwi)
    char *record, *type, *seq, *uwi;
{
    strncpy (type, record, 5);
    type[5] = NUL;

    strncpy (seq, record+1, 2);
    seq[2]  = NUL;

    strncpy (uwi,  record+5, 14);
    uwi[14] = NUL;

    return;
}

int match (record, pattern)
    char *record, *pattern;
{
    char *rp, *pp;

    rp = record;
    for (pp=pattern; *pp; pp++) {
	if ((*pp != 'X') && (*pp != 'x') && (*pp != *rp)) return FALSE;
	rp++;
    }

    return TRUE;
}

int filler (f, type, uwi, seq, cat)
    FILE *f;
    char *type, *uwi, *seq, *cat;
{
    char    temp[6];
    static  char blank[61] = 
	{ "                                                            " };

    strcpy (temp, type);
    if (seq) { temp[1] = seq[0]; temp[2] = seq[1]; }

    fprintf (f, "%s%14.14s %60.60s%s\n", temp, uwi, blank, cat);
    return;
}


int main(argc,argv)
    int     argc;
    char    *argv[];
{
    FILE *whcs_dat;

    FILE *well_hdr;
    FILE *well_dst;
    FILE *well_core;
    FILE *well_core_line;
    FILE *well_prod;
    FILE *well_dst_mud;
    FILE *well_4_point;
    FILE *well_cmpt_anal;
    FILE *well_locations;
    FILE *well_prod_treatment;
    FILE *well_producing_zones;

    FILE *temp_hdr;
    FILE *temp_dst;
    FILE *temp_core;
    FILE *temp_core_line;
    FILE *temp_prod_2;
    FILE *temp_prod_5;
    FILE *temp_dst_mud;
    FILE *temp_cmpt_anal;
    FILE *temp_locations;
    FILE *temp_4_point_2;
    FILE *temp_4_point_5;
    FILE *temp_producing_zones;
    FILE *temp_prod_treatment_2;
    FILE *temp_prod_treatment_5;

    char    line[MAXLINE];
    int     found, n_uwi, rec_count, sample, modval;
    int	    status;
    static  char card_type[6], seqno[3], uwi[15], curr_uwi[15], curr_seq[3];
    static  char temp[20];
	char *p;

    if (argc < 2)
    {
	printf("\n\tUsage - ld_whcs_preload filename [-sample nn]\n\n");
	exit(1);
    }

    found	= 0;
    n_uwi	= 0;
    rec_count	= 0;
    sample	= 999999999;

    strcpy (curr_uwi, "              ");
    strcpy (uwi,      "              ");

    if (argc > 2) {
	if ((strcmp(argv[2], "-SAMPLE") == 0) ||
	    (strcmp(argv[2], "-sample") == 0) ) {
	    sample = 100;
	    if (argc > 3) sscanf (argv[3], "%d", &sample);
	    printf ("Sample of %d wells requested.\n", sample);
	}
    }

    if ( (whcs_dat=fopen(argv[1], "r")) == (FILE *)NULL ) {
	printf("Couldn't find or open %s for reading.\n", argv[1]);
	exit(1);
    }

    temp_hdr		  = fopen ("TEMP_HDR.TMP",		"w");
    temp_dst		  = fopen ("TEMP_DST.TMP",		"w");
    temp_core		  = fopen ("TEMP_CORE.TMP",		"w");
    temp_core_line	  = fopen ("TEMP_CORE_LINE.TMP",	"w");
    temp_prod_2		  = fopen ("TEMP_PROD_2.TMP",		"w");
    temp_prod_5		  = fopen ("TEMP_PROD_5.TMP",		"w");
    temp_dst_mud	  = fopen ("TEMP_DST_MUD.TMP",		"w");
    temp_cmpt_anal	  = fopen ("TEMP_CMPT_ANAL.TMP",	"w");
    temp_locations	  = fopen ("TEMP_LOCATIONS.TMP",	"w");
    temp_4_point_2	  = fopen ("TEMP_4_POINT_2.TMP",	"w");
    temp_4_point_5	  = fopen ("TEMP_4_POINT_5.TMP",	"w");
    temp_producing_zones  = fopen ("TEMP_PRODUCING_ZONES.TMP",	"w");
    temp_prod_treatment_2 = fopen ("TEMP_PROD_TREATMENT_2.TMP", "w");
    temp_prod_treatment_5 = fopen ("TEMP_PROD_TREATMENT_5.TMP", "w");

    n_uwi = 0;
    printf("\n%7d records processed\n", rec_count);

    modval = 5;
    while ( (fgets(line, sizeof line, whcs_dat) != NULL) &&
	    (n_uwi < sample)				  ) {

	if ((++rec_count % modval) == 0)
	    printf("%7d records processed.\n", rec_count);

	switch (rec_count) {		/* slide the reporting level */
	    case   50:	modval =   10;	break;
	    case  100:	modval =   50;	break;
	    case 1000:	modval =  100;	break;
	    case 2000:	modval =  200;	break;
	    case 3000:	modval =  500;	break;
	    case 4000:	modval = 1000;	break;
	    default:	break;
	}

        line[strlen(line)-1] = NUL;	/* remove newline character	*/
	pad (line);			/* pad out to 80 characters	*/

        parse (line, card_type, seqno, uwi);

	if (strcmp(uwi, curr_uwi) != 0) {
	    n_uwi++;
	    strcpy (curr_uwi, uwi);
	    strcpy (curr_seq, seqno);
	}

/*****************  WELL_PRODUCING_ZONES*****************/
	if (match(line, "10010") ||
	    match(line, "10011")  ) {
	    fprintf(temp_producing_zones, "%80.80s\n", line);
	}

/*****************  WELL_HDR            *****************/
	if (match(line, "10002") ||
	    match(line, "10010") ||
	    match(line, "101  ") ||
	    match(line, "10201") ||
	    match(line, "103  ") ||
	    match(line, "105  ") ||
	    match(line, "106  ") ||
	    match(line, "107  ") ||
	    match(line, "660xx")  ) {
	    fprintf(temp_hdr, "%80.80s\n", line);
	}

/*****************  WELL_PROD_TREATMENT *****************/
	if (match(line, "2xx3x") ) {
	    fprintf(temp_prod_treatment_2, "%80.80s\n", line);
	}

	if (match(line, "5xx3x") ) {
	    fprintf(temp_prod_treatment_5, "%80.80s\n", line);
	}

/*****************  WELL_TESTS          *****************/
	if (match(line, "4xx01") ||
	    match(line, "4xx05") ||
	    match(line, "4xx32") ||
	    match(line, "4xx33") ||
	    match(line, "4xx34")  ) {
	    fprintf(temp_dst, "%80.80s\n", line);
	}

	if (match(line, "4xx41") ||
	    match(line, "4xx42")  ) {
	    fprintf(temp_dst_mud, "%80.80s\n", line);
	}

	if ( ( match(line, "2xx01") ||
	       match(line, "2xx51")  ) &&
	     ( (seqno[0] < '5') || match(seqno, "50") ) ) { /* (200-250)01/51 */
	    fprintf(temp_prod_2, "%80.80s\n", line);
	}

	if (match(line, "5xx01") ||
	    match(line, "5xx51")  ) {
	    fprintf(temp_prod_5, "%80.80s\n", line);
	}

/*****************  WELL_LOCATIONS      *****************/
	if (match(line, "10021") ||
	    match(line, "101  ")  ) {
	    fprintf(temp_locations, "%80.80s\n", line);
	}

/*****************  WELL_4_POINT       *****************/
	if (match(line, "2xx42") ||
	    match(line, "2xx43") ||
	    match(line, "2xx54") ) {
	    fprintf(temp_4_point_2, "%80.80s\n", line);
	}

	if (match(line, "5xx42") ||
	    match(line, "5xx43") ||
	    match(line, "5xx54") ) {
	    fprintf(temp_4_point_5, "%80.80s\n", line);
	}

/*****************  WELL_CORE       *****************/
	if (match(line, "3xx01") ||
	    match(line, "3xx97") ||
	    match(line, "3xx99") ||
	    match(line, "8xx01") ) {
	    fprintf(temp_core, "%80.80s\n", line);
	}

/*****************  WELL_CORE_LINE       *****************/
	if (match(line, "8xxxx") &&
	    (! match(line, "8xx01")) ) {
	    fprintf(temp_core_line, "%80.80s\n", line);
	}

/*****************  WELL_CMPT_ANAL       *****************/
	if (match(line, "4xx50") ||
	    match(line, "4xx51") ) {
	    fprintf(temp_cmpt_anal, "%80.80s\n", line);
	}

    }

    printf ("\n\n========================================\n");
    printf ("Total of %7d records processed ", rec_count);
    printf ("(%d wells).\n", n_uwi);
    printf ("========================================\n\n");

    fclose (temp_hdr);
    fclose (temp_dst);
    fclose (temp_core);
    fclose (temp_core_line);
    fclose (temp_prod_2);
    fclose (temp_prod_5);
    fclose (temp_cmpt_anal);
    fclose (temp_locations);
    fclose (temp_4_point_2);
    fclose (temp_4_point_5);
    fclose (temp_producing_zones);
    fclose (temp_prod_treatment_2);
    fclose (temp_prod_treatment_5);

    temp_hdr		  = fopen ("TEMP_HDR.TMP",		"r");
    temp_dst		  = fopen ("TEMP_DST.TMP",		"r");
    temp_core		  = fopen ("TEMP_CORE.TMP",		"r");
    temp_core_line	  = fopen ("TEMP_CORE_LINE.TMP",	"r");
    temp_prod_2		  = fopen ("TEMP_PROD_2.TMP",		"r");
    temp_prod_5		  = fopen ("TEMP_PROD_5.TMP",		"r");
    temp_cmpt_anal	  = fopen ("TEMP_CMPT_ANAL.TMP",	"r");
    temp_locations	  = fopen ("TEMP_LOCATIONS.TMP",	"r");
    temp_4_point_2	  = fopen ("TEMP_4_POINT_2.TMP",	"r");
    temp_4_point_5	  = fopen ("TEMP_4_POINT_5.TMP",	"r");
    temp_producing_zones  = fopen ("TEMP_PRODUCING_ZONES.TMP",	"r");
    temp_prod_treatment_2 = fopen ("TEMP_PROD_TREATMENT_2.TMP", "r");
    temp_prod_treatment_5 = fopen ("TEMP_PROD_TREATMENT_5.TMP", "r");

    well_hdr		  = fopen ("WELL_HDR.DAT",		"w");
    well_dst		  = fopen ("WELL_DST.DAT",		"w");
    well_core		  = fopen ("WELL_CORE.DAT",		"w");
    well_core_line	  = fopen ("WELL_CORE_LINE.DAT",	"w");
    well_prod		  = fopen ("WELL_PROD.DAT",		"w");
    well_4_point	  = fopen ("WELL_4_POINT.DAT",		"w");
    well_dst_mud	  = fopen ("WELL_DST_MUD.DAT",		"w");
    well_cmpt_anal	  = fopen ("WELL_CMPT_ANAL.DAT",	"w");
    well_locations	  = fopen ("WELL_LOCATIONS.DAT",	"w");
    well_prod_treatment   = fopen ("WELL_PROD_TREATMENT.DAT",	"w");
    well_producing_zones  = fopen ("WELL_PRODUCING_ZONES.DAT",	"w");

    rec_count = 0;
    printf("%7d logical records constructed.\n", rec_count);

/*************  WELL_PRODUCING_ZONES logical record building *********/

    p = fgets (line, sizeof line, temp_producing_zones);
    while ( p != NULL && (feof(temp_producing_zones)) == 0)
    {
	found = 0;
	parse (line, card_type, seqno, uwi);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (! match(line, "10010"))
	    filler (well_producing_zones, "10010", uwi, NULL, "@");
	else {
	    fprintf (well_producing_zones, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_producing_zones);
	    found = 1;
	}

	if (! match(line, "10011"))
	    filler (well_producing_zones, "10011", uwi, NULL, " ");
	else {
	    fprintf (well_producing_zones, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_producing_zones);
	    found = 1;
	}

	if (found == 0) fgets(line, sizeof line, temp_producing_zones);
    }

/*****************  WELL_HDR logical record building *****************/

    p = fgets (line, sizeof line, temp_hdr);
    while ( p != NULL && (feof(temp_hdr)) == 0)
    {
	found = 0;
	parse (line, card_type, seqno, uwi);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (! match(line, "10002"))
	    filler (well_hdr, "10002", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "10010"))
	    filler (well_hdr, "10010", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "101  "))
	    filler (well_hdr, "101  ", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "10201"))
	    filler (well_hdr, "10201", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "103  "))
	    filler (well_hdr, "103  ", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "105  "))
	    filler (well_hdr, "105  ", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "106  "))
	    filler (well_hdr, "106  ", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "107  "))
	    filler (well_hdr, "107  ", uwi, NULL, "@");
	else {
	    fprintf (well_hdr, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (! match(line, "660xx"))
	    filler (well_hdr, "660xx", uwi, NULL, " ");
	else {
	    fprintf (well_hdr, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_hdr);
	    found = 1;
	}

	if (found == 0) fgets(line, sizeof line, temp_hdr);
    }

/*****************  WELL_DST logical record building ************/

    p = fgets (line, sizeof line, temp_dst);
    parse (line, card_type, seqno, uwi);
    while ( p != NULL && (feof(temp_dst)) == 0)
    {
	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) &&
	    match(line, "4xx05") ) {
	    fprintf (well_dst, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_dst);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    if (match(uwi, curr_uwi) &&
		match(line, "4xx01")  ) {
		fprintf (well_dst, "%80.80s@\n", line);
		fgets(line, sizeof line, temp_dst);
		parse (line, card_type, seqno, uwi);
		found = 1;
	    }
	    else filler (well_dst, "4xx01", curr_uwi, curr_seq, "@");
        }

	if (match(uwi, curr_uwi) && match(line, "4xx32")) {
	    fprintf (well_dst, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_dst);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_dst, "4xx32", curr_uwi, curr_seq, "@");
	
	if (match(uwi, curr_uwi) && match(line, "4xx33")) {
	    fprintf (well_dst, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_dst);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_dst, "4xx33", curr_uwi, curr_seq, "@");
	
	if (match(uwi, curr_uwi) && match(line, "4xx34")) {
	    fprintf (well_dst, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_dst);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_dst, "4xx34", curr_uwi, curr_seq, " ");
	
	if (found == 0) {
	    fgets(line, sizeof line, temp_dst);
	    parse (line, card_type, seqno, uwi);
	}
    }


/*****************  WELL_DST_MUD logical record building ************/

    p = fgets(line, sizeof line, temp_dst_mud);
    parse (line, card_type, seqno, uwi);
    while ( p != NULL && (feof(temp_dst_mud)) == 0)
    {
	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) &&
	    match(line, "4xx41")  ) {
	    fprintf (well_dst_mud, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_dst_mud);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_dst_mud, "4xx41", curr_uwi, curr_seq, "@");

	if (match(uwi, curr_uwi) &&
	    match(line, "4xx42")  ) {
	    fprintf (well_dst_mud, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_dst_mud);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_dst_mud, "4xx42", curr_uwi, curr_seq, " ");

	if (found == 0) {
	    fgets(line, sizeof line, temp_dst_mud);
	    parse (line, card_type, seqno, uwi);
	}
    }

/*****************  WELL_CMPT_ANAL logical record building ************/

    p = fgets(line, sizeof line, temp_cmpt_anal);
    parse (line, card_type, seqno, uwi);
    while ( p != NULL && (feof(temp_cmpt_anal)) == 0)
    {
	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) &&
	    match(line, "4xx50")  ) {
	    fprintf (well_cmpt_anal, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_cmpt_anal);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_cmpt_anal, "4xx50", curr_uwi, curr_seq, "@");

	if (match(uwi, curr_uwi) &&
	    match(line, "4xx51")  ) {
	    fprintf (well_cmpt_anal, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_cmpt_anal);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_cmpt_anal, "4xx51", curr_uwi, curr_seq, " ");

	if (found == 0) {
	    fgets(line, sizeof line, temp_cmpt_anal);
	    parse (line, card_type, seqno, uwi);
	}
    }


/*****************  WELL_PROD logical record building ************/

    p = fgets(line, sizeof line, temp_prod_2);
    parse (line, card_type, seqno, uwi);
    while ( p != NULL && (feof(temp_prod_2)) == 0)
    {
	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) &&
	    match(line, "2xx01")  ) {
	    fprintf (well_prod, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod, "2xx01", curr_uwi, curr_seq, "@");

	if (match(uwi, curr_uwi) &&
	    match(line, "2xx51")  ) {
	    fprintf (well_prod, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod, "2xx51", curr_uwi, curr_seq, " ");

	if (found == 0) {
	    fgets(line, sizeof line, temp_prod_2);
	    parse (line, card_type, seqno, uwi);
	}
    }

    p = fgets(line, sizeof line, temp_prod_5);
    parse (line, card_type, seqno, uwi);
    while ( p != NULL && (feof(temp_prod_5)) == 0)
    {
	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) &&
	    match(line, "5xx01")  ) {
	    fprintf (well_prod, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod, "5xx01", curr_uwi, curr_seq, "@");

	if (match(uwi, curr_uwi) &&
	    match(line, "5xx51")  ) {
	    fprintf (well_prod, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod, "5xx51", curr_uwi, curr_seq, " ");

	if (found == 0) {
	    fgets(line, sizeof line, temp_prod_5);
	    parse (line, card_type, seqno, uwi);
	}
    }


/*****************  WELL_PROD_TREATMENT logical record building ************/

    found = 0;
    while ( (feof(temp_prod_treatment_2)) == 0)
    {
	if (found == 0) {
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	}

	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) && match(line, "2xx30")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "2xx31")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "2xx31", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "2xx32")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "2xx33")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "2xx33", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "2xx34")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "2xx35")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "2xx35", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "2xx36")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "2xx37")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "2xx37", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "2xx38")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "2xx39")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_2);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "2xx39", curr_uwi, curr_seq, " ");
	
    }

    found = 0;
    while ( (feof(temp_prod_treatment_5)) == 0)
    {
	if (found == 0) {
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	}

	found = 0;
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (match(uwi, curr_uwi) && match(line, "5xx30")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "5xx31")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "5xx31", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "5xx32")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "5xx33")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "5xx33", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "5xx34")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "5xx35")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "5xx35", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "5xx36")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "5xx37")) {
 	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "5xx37", curr_uwi, curr_seq, " ");
	
	if (match(uwi, curr_uwi) && match(line, "5xx38")) {
	    fprintf (well_prod_treatment, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else {
	    continue;
	}
	
	if (match(uwi, curr_uwi) && match(line, "5xx39")) {
	    fprintf (well_prod_treatment, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_prod_treatment_5);
	    parse (line, card_type, seqno, uwi);
	    found = 1;
	}
	else filler (well_prod_treatment, "5xx39", curr_uwi, curr_seq, " ");
    }

/*****************  WELL_LOCATIONS logical record building ************/
    p = fgets(line, sizeof line, temp_locations);
    while ( p != NULL && (feof(temp_locations)) == 0)
    {
	found = 0;
	parse (line, card_type, seqno, uwi);
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (! match(line, "10021"))
	    filler (well_locations, "10021", curr_uwi, NULL, "@");
	else {
	    fprintf (well_locations, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_locations);
	    found = 1;
	}

	if (! match(line, "101  "))
	    filler (well_locations, "101  ", curr_uwi, NULL, " ");
	else {
	    fprintf (well_locations, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_locations);
	    found = 1;
	}

	if (found == 0)
	    fgets(line, sizeof line, temp_locations);
    }

/*****************  WELL_CORE logical record building ************/
    p = fgets(line, sizeof line, temp_core);
    while ( p != NULL && (feof(temp_core)) == 0)
    {
	found = 0;
	parse (line, card_type, seqno, uwi);
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	if (! match(line, "3xx01"))
	    filler (well_core, "3xx01", curr_uwi, curr_seq, "@");
	else {
	    fprintf (well_core, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core);
	    found = 1;
	}

	if (! match(line, "3xx97"))
	    filler (well_core, "3xx97", curr_uwi, curr_seq, "@");
	else {
	    fprintf (well_core, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core);
	    found = 1;
	}

	if (! match(line, "3xx99"))
	    filler (well_core, "3xx99", curr_uwi, curr_seq, "@");
	else {
	    fprintf (well_core, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core);
	    found = 1;
	}

	if (! match(line, "8xx01"))
	    filler (well_core, "8xx01", curr_uwi, curr_seq, " ");
	else {
	    fprintf (well_core, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core);
	    found = 1;
	}

	if (found == 0)
	    fgets(line, sizeof line, temp_core);
    }

/*****************  WELL_CORE_LINE logical record building ************/
    found = 0;
    while ( (feof(temp_core_line)) == 0)
    {
	if (found == 0)
	    fgets(line, sizeof line, temp_core_line);

	found = 0;
	parse (line, card_type, seqno, uwi);
	strcpy (curr_uwi, uwi);
	strcpy (curr_seq, seqno);
	temp[9] = card_type[3];

	if ((++rec_count % modval) == 0)
	    printf("%7d logical records constructed.\n", rec_count);

	/* **************** */
	if (match(uwi, curr_uwi) && match(line, "8xxx3")) {
	    fprintf (well_core_line, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    continue;
	}

	if (match(uwi, curr_uwi) && match(line, "8xxx4")) {
	    fprintf (well_core_line, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    strcpy (temp, "8xxx4");
	    temp[3] = temp[9];
	    filler (well_core_line, temp, curr_uwi, curr_seq, " ");
	}
	/* **************** */

	if (match(uwi, curr_uwi) && match(line, "8xxx5")) {
	    fprintf (well_core_line, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    continue;
	}

	if (match(uwi, curr_uwi) && match(line, "8xxx6")) {
	    fprintf (well_core_line, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    strcpy (temp, "8xxx6");
	    temp[3] = temp[9];
	    filler (well_core_line, temp, curr_uwi, curr_seq, " ");
	}
	/* **************** */

	if (match(uwi, curr_uwi) && match(line, "8xxx7")) {
	    fprintf (well_core_line, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    continue;
	}

	if (match(uwi, curr_uwi) && match(line, "8xxx8")) {
	    fprintf (well_core_line, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    strcpy (temp, "8xxx8");
	    temp[3] = temp[9];
	    filler (well_core_line, temp, curr_uwi, curr_seq, " ");
	}
	/* **************** */

	if (match(uwi, curr_uwi) && match(line, "8xxx9")) {
	    fprintf (well_core_line, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    continue;
	}

	if (match(uwi, curr_uwi) && match(line, "8xxx0")) {
	    fprintf (well_core_line, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    strcpy (temp, "8xxx0");
	    temp[3] = temp[9] + '\001';
	    filler (well_core_line, temp, curr_uwi, curr_seq, " ");
	}
	/* **************** */

	if (match(uwi, curr_uwi) && match(line, "8xxx1")) {
	    fprintf (well_core_line, "%80.80s@\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    continue;
	}

	if (match(uwi, curr_uwi) && match(line, "8xxx2")) {
	    fprintf (well_core_line, "%80.80s\n", line);
	    fgets(line, sizeof line, temp_core_line);
	    found = 1;
	}
	else {
	    strcpy (temp, "8xxx2");
	    temp[3] = temp[9];
	    filler (well_core_line, temp, curr_uwi, curr_seq, " ");
	}
	/* **************** */
    }

    printf ("\n\nPre-processor done!\n");

    fclose (temp_hdr);
    fclose (temp_dst);
    fclose (temp_core);
    fclose (temp_core_line);
    fclose (temp_prod_2);
    fclose (temp_prod_5);
    fclose (temp_dst_mud);
    fclose (temp_locations);
    fclose (temp_4_point_2);
    fclose (temp_4_point_5);
    fclose (temp_cmpt_anal);
    fclose (temp_producing_zones);
    fclose (temp_prod_treatment_2);
    fclose (temp_prod_treatment_5);

    fclose (well_hdr);
    fclose (well_dst);
    fclose (well_core);
    fclose (well_core_line);
    fclose (well_prod);
    fclose (well_dst_mud);
    fclose (well_cmpt_anal);
    fclose (well_locations);
    fclose (well_prod_treatment);

    ho_delete_file ("TEMP_HDR.TMP",		&status);
    ho_delete_file ("TEMP_DST.TMP",		&status);
    ho_delete_file ("TEMP_CORE.TMP",		&status);
    ho_delete_file ("TEMP_CORE_LINE.TMP",	&status);
    ho_delete_file ("TEMP_PROD_2.TMP",		&status);
    ho_delete_file ("TEMP_PROD_5.TMP",		&status);
    ho_delete_file ("TEMP_DST_MUD.TMP",		&status);
    ho_delete_file ("TEMP_CMPT_ANAL.TMP",	&status);
    ho_delete_file ("TEMP_LOCATIONS.TMP",	&status);
    ho_delete_file ("TEMP_4_POINT_2.TMP",	&status);
    ho_delete_file ("TEMP_4_POINT_5.TMP",	&status);
    ho_delete_file ("TEMP_PRODUCING_ZONES.TMP",	&status);
    ho_delete_file ("TEMP_PROD_TREATMENT_2.TMP",&status);
    ho_delete_file ("TEMP_PROD_TREATMENT_5.TMP",&status);

    return;
}
