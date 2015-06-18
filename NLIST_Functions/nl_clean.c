/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLEAN.C*/
/* *1     1-JUL-1989 12:23:51 GILLESPIE ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLEAN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLEAN.C*/
/* *3     8-APR-1989 14:11:59 GILLESPIE "Add copyright notice"*/
/* *2    17-DEC-1988 10:31:33 GILLESPIE "(SPR 4981) Unisource"*/
/* *1     7-JUL-1987 11:11:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLEAN.C*/
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


/* NL_CLEAN.CC,  JAMES>NEWNL>SOURCE,  JAMES CHANG,  12/08/86
   To clean a list of file, garbage collecting those dummy space produced by
   deletion.
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : NL_CLEAN - To clean a list of file., garbage collecting those dummy space prod */

#include "esi_nl.h"
#include "esi_c_lib.h"
#include "esi_ho_files.h"

/* ************************************************************* */

main()
{
    FILENAME filename;
    FILENAME s;
    CHAR *eof;
    NLIST_ID nlistName;
    FILE *fp;
    FLOAT percent = 0.1;

    printf(" Enter filename of the list of n-lists file. ");
    gets(s);
    sscanf(s, "%s", filename);
    printf("  Enter maximum percent of dummy space allowed: --default = .1 ");
    gets(s);
    sscanf(s, "%f", &percent);

    fp = ho_open(filename, "r", NULL );
    eof = fgets(s, STMAX, fp);
    while(  eof != NULL  )
    {
         sscanf(s, "%s", nlistName);
         printf( " Cleaning file < %s > by percent = %f \n", nlistName, percent);
         nl_clean_file(nlistName, percent);
         eof = fgets(s, STMAX, fp);
    }
    fclose(fp);
}


/* END-CODE */
