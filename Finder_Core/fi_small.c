/* DEC/CMS REPLACEMENT HISTORY, Element FI_SMALL.C */
/* *4    28-AUG-1990 13:29:54 VINCE "(SPR 1) added include esi_ts.h and fixed a casting problem in qs_set_c call" */
/* *3    25-JUL-1990 15:21:49 JESSIE "(SPR 5628) Make LYNX functions iternally consistant" */
/* *2    23-MAR-1990 20:53:46 DOROTHY "(SPR -1) WELL FORMS DRIVER." */
/* *1    23-MAR-1990 20:52:45 DOROTHY "STANDALONE WELL FORMS DRIVER. " */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_SMALL.C */
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

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_fi.h"
#include "esi_qs.h"
#include "esi_ts.h"

INT  main (argc, argv)
INT  argc;
CHAR *argv[];
	{

	/* This assumes that the command line has the first string in the form
	NAME/PASSWORD@NODE, where NAME/PASSWORD are the name and password
	of a valid Oracle/Finder database account, and @NODE is the optional name
	of the network node where the database resides.  Actually, or_login looks for the
	first parameter that has a slash in it and uses it for the account name
	and/or password.  If the name and password are not specified on the command
	line, then or_login prompts for them. 
	Argument 2 is the name of the project */

	/*  Login to the database  */
	
	if (argc != 3 OR or_login (argc, argv) != SUCCESS)
		{
		printf("Usage: progname name/password[@database] project\n");
		exit (EXIT_FAILURE);
		}

	qs_set_c (QS_PROJECT_NAME, ts_sto_upper(argv[2]), 0);

	/* The following call sets the logical name paths at the process level for acessing
	the Finder database files.  It also establishes workspaces which identify project
	defaults */

	finder_config_project (TRUE);

	or_logout (TRUE);
	exit (EXIT_SUCCESS);
	}

