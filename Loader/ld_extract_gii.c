/* DEC/CMS REPLACEMENT HISTORY, Element LD_EXTRACT_GII.C */
/* *4    20-SEP-1991 11:27:46 LOEBL "(SPR 0) Fix compiler Warning" */
/* *3    16-MAR-1990 09:55:43 VINCE "(SPR -1) fixed a problem in str parsing" */
/* *2     8-MAR-1990 12:34:38 VINCE "(SPR -1) fixed history definition in CMS" */
/* *1     8-MAR-1990 10:59:43 VINCE "name changed from extract_gii.c" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_EXTRACT_GII.C */

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    This program will extract a subset of data from a GII format land grid
    file.  It takes lower left and/or upper right STR identifiers to delimite
    the extraction.  It will also produce data with a "hole" in it or odd 
    shaped areas by use of the -INVERT option.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUL   '\0'
#define TRUE 1
#define FALSE 0

typedef struct {
    int    section;
    double town;
    double range;
    int    pm;
} STR ;

char *next_field(dest, source, c)
     char *dest, *source, c;
{
    char *dp=dest, *sp=source;
    
    while (*sp && (*sp != c)) *dp++ = *sp++;
    *dp = NUL;
    if (*sp == c) sp++;
    return sp;
}

int crackstr (def, str)
    char *def;
    STR *str;
{
    char *cp, *dp, buffer[30];
    int dir;

    str->section = 0;
    str->town    = 0.0;
    str->range   = 0.0;
    str->pm      = 0;

    dp = next_field(buffer, def, '-');

    /* check for section number */
    if (strlen(buffer)) {
	cp = buffer + strlen(buffer) - 1;
	if (isdigit(*cp)) {
	    str->section = atol(buffer);
	    dp = next_field(buffer, dp, '-');
	}
    }

    /* get township number and direction */
    if (strlen(buffer)) {
	dir = 1;
	cp = buffer + strlen(buffer) - 1;
	if ((*cp == 'S') || (*cp == 's')) dir = -1;
	*cp = NUL;
	str->town = atof(buffer) * dir;

	/* get range number and direction */
	dp = next_field(buffer, dp, '-');
	if (strlen(buffer)) {
	    dir = 1;
	    cp = buffer + strlen(buffer) - 1;
	    if ((*cp == 'W') || (*cp == 'w')) dir = -1;
	    *cp = NUL;
	    str->range = atof(buffer) * dir;

	    /* get principal meridian, if any */
	    dp = next_field(buffer, dp, '-');
	    if (strlen(buffer)) {
		str->pm = atol(buffer);
	    }
	}
    }
    return;
}

int cnvi (cp, len)
    char *cp;
    int  len;
{
    char buffer[30];
    
    strncpy (buffer, cp, len);
    buffer[len] = NUL;
    return atoi(buffer);
}

double cnvf (cp, len)
    char *cp;
    int  len;
{
    char buffer[30];
    double f;
    
    strncpy (buffer, cp, len);
    buffer[len] = NUL;
    f = atof(buffer);
    return f;
}

int sec_to_rc (sec, row, col)
    int sec, *row, *col;
{
    *row = 6 - ((sec - 1) / 6);
    *col = 6 - ((sec - 1) % 6);
    if ((*row % 2) != 0) *col = 7 - *col;
    return;
}

int isinside (cur, ll, ur)
    STR *cur, *ll, *ur;
{
    int llrow, llcol, urrow, urcol, currow, curcol;

    if ((ll->pm != 0) && (cur->pm != ll->pm)) return FALSE;
    if ((ur->pm != 0) && (cur->pm != ur->pm)) return FALSE;

    if ((ll->town  != 0.0) && (cur->town  < ll->town))  return FALSE;
    if ((ll->range != 0.0) && (cur->range < ll->range)) return FALSE;

    if ((ur->town  != 0.0) && (cur->town  > ur->town))  return FALSE;
    if ((ur->range != 0.0) && (cur->range > ur->range)) return FALSE;

    if ((ll->town == cur->town) && (ll->range == cur->range)) {
	if ((ll->section != 0) && (cur->section != 0)) {
	    sec_to_rc (ll->section,  &llrow,  &llcol);
	    sec_to_rc (cur->section, &currow, &curcol);
	    if (currow < llrow) return FALSE;
	    if (curcol < llcol) return FALSE;
	}
    }

    if ((ur->town == cur->town) && (ur->range == cur->range)) {
	if ((ur->section != 0) && (cur->section != 0)) {
	    sec_to_rc (ur->section,  &urrow,  &urcol);
	    sec_to_rc (cur->section, &currow, &curcol);
	    if (currow > urrow) return FALSE;
	    if (curcol > urcol) return FALSE;
	}
    }

    return TRUE;
}

int newstr (current, last)
    STR *current, *last;
{
    if ((current->town  != last->town)  ||
	(current->range != last->range) ||
	(current->pm    != last->pm)     ) {
	last->town  = current->town;
	last->range = current->range;
	last->pm    = current->pm;
	return TRUE;
    }
    return FALSE;
}

int main (argc, argv)
    int  argc;
    char *argv[];
{
    int   i, total_lines, copied_lines;
    int   total_townships, copied_townships, modulo, extension;
    int   invert, block, copy_rec, new, trname;
    char  *arg, *cp;
    char  in_file[300], out_file[300];
    char  ll_def[30], ur_def[30], pm_def[5];
    char  line[133], temp[30];
    STR   ll_str, ur_str, str, last_str;
    FILE *in, *out;

    in_file[0]  = NUL;
    out_file[0] = NUL;
    ll_def[0]   = NUL;
    ur_def[0]   = NUL;
    pm_def[0]   = NUL;
    in  = (FILE *)0;
    out = (FILE *)0;
    crackstr ("", &last_str);

    if (argc < 2) {
	printf ("\nUsage - EXTRACT_GII infilespec outfilespec [options]\n\n");
	printf ("\tOptions --\n");
	printf ("\t    -LL <str_def>\n");
	printf ("\t    -UR <str_def>\n");
	printf ("\t    -PM PrincipalMeridianCode\n");
	printf ("\t    -INVERT\n");
	printf ("\t    -BLOCK number\n");
	printf ("\t    -TRNAME\n");
	printf ("\t<str_def> is in any of the following forms:\n");
	printf ("\t  Township-Range\n");
	printf ("\t  Township-Range-PrincipalMeridianCode\n");
	printf ("\t  Section-Township-Range\n");
	printf ("\t  Section-Township-Range-PrincipalMeridianCode\n");
	printf ("\t   Township and Range must have direction specifiers\n");
	printf ("\t   N/S or E/W.\n\n");
	printf ("\tINVERT will copy only records that are outside the given\n");
	printf ("\tarea.  This allows you to create areas of arbitrary shape\n");
	printf ("\tsuch as diagonal or 'L' shaped.\n\n");
	printf ("\tBLOCK will create multiple output files with NUMBER townships\n");
	printf ("\tin each file.  The file names will begin with the root name\n");
	printf ("\tof OUTFILESPEC and have extensions of B1, B2, ..., Bnn.\n\n");
	printf ("\tTRNAME will cause each output file to be names the same as the\n");
	printf ("\ttownship it contains.  I.E.  T31N_15W_11.GII.  This option also\n");
	printf ("\tset -BLOCK equal to 1.\n\n");
	return;
    }

    invert = FALSE;
    block  = FALSE;
    trname = FALSE;
    modulo = 0;

    for (i=1; i<argc; i++) {
	arg = argv[i];
	if (*arg != '-') {
	    if	    (strlen(in_file)  == 0)	strcpy (in_file,  arg);
	    else if (strlen(out_file) == 0)	strcpy (out_file, arg);
	    else {
		printf ("Unknown command line argument '%s'.\n", arg);
		return;
	    }
	}
	else {
	    switch (arg[1]) {
		case 'L': case 'l':
		    if (((i+1)<argc) && (*argv[i+1]!='-'))
			strcpy(ll_def, argv[++i]);
		    break;
		case 'U': case 'u':
		    if (((i+1)<argc) && (*argv[i+1]!='-'))
			strcpy(ur_def, argv[++i]);
		    break;
		case 'P': case 'p':
		    if (((i+1)<argc) && (*argv[i+1]!='-'))
			strcpy(pm_def, argv[++i]);
		    break;
		case 'I': case 'i':
		    invert = TRUE;
		    break;
		case 'B': case 'b':
		    block = TRUE;
		    if (((i+1)<argc) && (*argv[i+1]!='-'))
			modulo = atol(argv[++i]);
		    else
			modulo = 100;
		    break;
		case 'T': case 't':
		    trname = TRUE;
		    break;
		default:
		    printf ("Unknown option '%s'.\n", arg);
		    return;
		    break;
	    }
	}
    }

    if (trname) {
	block = TRUE;
	modulo = 1;
    }

    crackstr(ll_def, &ll_str);
    crackstr(ur_def, &ur_str);
    if (strlen(pm_def)) {
	i = atol(pm_def);
	ll_str.pm = i;
	ur_str.pm = i;
    }

    if (ll_str.pm != ur_str.pm) {
	printf ("The Principal Meridian of the lower left and upper right\n");
	printf ("STR must match.\n");
	return;
    }

    if ( (in=fopen(in_file, "r")) == (FILE *)0) {
	printf ("Can't open the input file '%s'\n", in_file);
	return;
    }

    total_lines = 0;
    copied_lines = 0;
    total_townships = 0;
    copied_townships = 0;
    extension = 0;

    if (block) {  /* remove any extension from out_file and add .Pxx */
	cp = strrchr(out_file, ']');
	if (cp) cp = strrchr(cp, '.');
	else cp = strrchr(out_file, '.');
	if (cp) *cp = NUL;
	strcat (out_file, ".Bxx");
    }
    else {
	if ( (out=fopen(out_file, "w")) == (FILE *)0) {
	    fclose (in);
	    printf ("Can't open the output file '%s'\n", in_file);
	    return;
	}
    }

    while ( fgets(line, sizeof line, in) != NULL) {
	total_lines++;
	if (strlen(line) < 128) continue;

	str.pm = cnvi(line+4, 3);
	if ((ll_str.pm == 0) || (str.pm == ll_str.pm)) {
	    str.section = cnvi(line+20, 2);
	    str.town    = cnvf(line+8,  5);
	    str.range   = cnvf(line+14, 5);
	    if (line[13] == 'S') str.town  = -str.town;
	    if (line[19] == 'W') str.range = -str.range;

	    copy_rec = isinside(&str, &ll_str, &ur_str);

	    if (invert) {
		if (copy_rec) copy_rec = FALSE;
		else copy_rec = TRUE;
	    }

	    new = newstr(&str, &last_str);
	    if ( new ) total_townships++;

	    if (copy_rec) {
		if ( block && new && (((total_townships - 1) % modulo) == 0)) {
		    if (trname) {
			cp = strrchr(out_file, ']');
			if (cp) *(++cp) = NUL;
			else out_file[0] = NUL;
			strncpy(temp, line+8, 12);
			strncpy(temp+13, line+5, 3);
			temp[3] = '_'; temp[9] = '_'; temp[12] = '-';
			temp[16] = NUL;
			if (temp[4]  == '0') strncpy(temp+3,"  ",2);
			if (temp[10] == '0') strncpy(temp+9,"  ",2);
			for (cp=temp; *cp; cp++)
			    while (*cp == ' ') strcpy (cp, cp+1);
			strcat (out_file, "T");
			strcat (out_file, temp);
			strcat (out_file, ".GII");
			++extension;
		    }
		    else {
			cp = strrchr(out_file, '.');
			if (cp) *cp = NUL;
			sprintf (out_file, "%s.B%d", out_file, ++extension);
		    }
		    if (out != (FILE *)0) fclose (out);
		    if ( (out=fopen(out_file, "w")) == (FILE *)0) {
			fclose (in);
			printf ("Can't open the output file '%s'\n", in_file);
			return;
		    }
		}

		fprintf (out, "%s", line);
		copied_lines++;
		if ( new ) copied_townships++;
	    }
	}
    }

    fclose (out);
    fclose (in);

    printf ("\n%d lines copied out of %d.\n", copied_lines, total_lines);
    printf ("%d townships copied out of %d", copied_townships, total_townships);
    if (block) printf (" into %d files of %d townships each", extension, modulo);
    printf (".\n");

    return;    
}
