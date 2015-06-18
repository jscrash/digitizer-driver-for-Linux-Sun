/* DEC/CMS REPLACEMENT HISTORY, Element SYN_PLOTTER.C*/
/* *2    18-DEC-1989 16:46:37 PURNA "(SPR 5069) export facility mods"*/
/* *1     1-JUL-1989 12:24:01 GILLESPIE ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SYN_PLOTTER.C*/
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

/* DEC/CMS REPLACEMENT HISTORY, Element SYN_PLOTTER.C*/
/* *6    12-JAN-1989 15:42:52 GORDON "(SPR 4981) new file name conventions, cleanup"*/
/* *5    17-DEC-1988 10:48:09 GILLESPIE "(SPR 4981) Unisource"*/
/* *4    18-JUL-1988 08:57:16 CHARLIE "(SPR 4654) move 'if batch' switch before or_login"*/
/* *3    12-MAY-1988 04:43:41 JULIAN "(SPR 4132) added 3rd arg to qs_set_i"*/
/* *2    30-JUL-1987 15:11:23 SPENCE "SYNTHETIC PLOT SETUP MAIN PROGRAM"*/
/* *1    30-JUL-1987 15:10:45 SPENCE "Reserved"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SYN_PLOTTER.C*/
/*
 *  File   : 
 * 
 *  Routine : 
 * 
 *  Description : Stand alone program involked as a batch job to 
 *                create a neutral plot file for modeling.
 *  Expected command line parameters :
 *           argv[1] - oracle login name/password@database
 *	     argv[2] - param file
 *           argv[3] - finder project name
 *   
 *  History:
 *     01/05/89 GS - Inserted param file argument to match other plotting
 *		     systems, pass it to syn_plot_driver thru glue.
 *
 */

/* include files */
#include "esi_c_lib.h"
#include "esi_fi.h"
#include "esi_qs.h"
#include "esi_ug.h"

/* number of arguments */
#define NUMARGS 3


main( argc, argv)
int argc;
char *argv[];
{
   int status;


/*
 * make sure there are arguments to the program.
 */
   if ( argc != (NUMARGS+1) ) 
   {
      printf("%d command line arguments are required to run this program.\n",
              NUMARGS);
      exit(EXIT_FAILURE);
   }


/*
 * login to oracle.
 */
   qs_set_i(QS_IF_BATCH, TRUE);
   status = or_login( argc,argv);
   if ( status != SUCCESS )
   {
      printf("\nCan't login to oracle.\n");
      exit(EXIT_FAILURE);
   }

/*
 * set up finder VAX/VMS project specific things.
 */
   qs_set_c( QS_PROJECT_NAME, argv[3], strlen(argv[3]) );
   status = finder_config_project(FALSE);
   if ( status != SUCCESS )
   {
      printf("\nProblem configure project.\n");
      or_logout(FALSE);
      exit(EXIT_FAILURE);
   }

/*
 * initialize plotting gks.
 */
   status = ug_init_plot_gks();
   if ( status != SUCCESS )
   {
      printf("\nProblem starting Neutral plot GKS.\n");
      or_logout(FALSE);
      exit(EXIT_FAILURE);
   }

/* 
 * Call routine to read parameters and call synthetic drawing routine.
 */
   syg_plot_driver( argv[2] );

/*
 * terminate plotting gks.
 */
   status = ug_term_plot_gks();

   or_logout(TRUE);
   exit(EXIT_SUCCESS);
}
