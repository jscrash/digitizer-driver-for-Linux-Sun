/* DEC/CMS REPLACEMENT HISTORY, Element MO_TRC_PLOTTER.C*/
/* *1     1-JUL-1989 12:23:43 GILLESPIE ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_TRC_PLOTTER.C*/
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

/* DEC/CMS REPLACEMENT HISTORY, Element MO_TRC_PLOTTER.C*/
/* *5    12-JAN-1989 15:40:40 GORDON "(SPR 4981) new filename standards, cleanup"*/
/* *4    18-JUL-1988 09:01:33 CHARLIE "(SPR 4654) move 'if batch' switch before or_login"*/
/* *3    12-MAY-1988 05:01:55 JULIAN "(SPR 4132) added 3rd arg to qs_set_c"*/
/* *2    23-JUL-1987 15:04:13 MCLEOD "First Checkin of VMS version"*/
/* *1    23-JUL-1987 15:02:10 MCLEOD "Reserved"*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_TRC_PLOTTER.C*/
/*
 *  File   : 
 * 
 *  Routine :     MO_TRC_PLOTTER.C 
 * 
 *  Description : Stand alone program involked as a batch job to 
 *                create a neutral plot file for modeling.
 *                This routine is for the model trace plot.
 *
 *  Expected command line parameters :
 *           argv[1] - oracle login name/password@database
 *           argv[2] - parameter file name
 *           argv[3] - finder project name
 *  History:
 *     01/04/89 GS - inserted parameter file name argument
 */

/* include files */
#include "esi_gl_defs.h"
#include "esi_qs.h"

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
   if ( argc < NUMARGS ) 
   {
      printf("%d command line arguments are required to run this program.\n",
              NUMARGS);
      exit(1);
   }


/*
 * login to oracle.
 */
   qs_set_i(QS_IF_BATCH, TRUE);
   status = or_login( argc,argv);
   if ( status != SUCCESS )
   {
      printf("\nCan't login to oracle.\n");
      exit(1);
   }

/*
 * set up finder VAX/VMS project specific things.
 */
   qs_set_c( QS_PROJECT_NAME, argv[3] ,strlen(argv[3]) );
   status = finder_config_project();
   if ( status != SUCCESS )
   {
      printf("\nProblem configure project.\n");
      exit(1);
   }

/*
 * initialize plotting gks.
 */
   status = ug_init_plot_gks();
   if ( status != SUCCESS )
   {
      printf("\nProblem starting Neutral plot GKS.\n");
      exit(1);
   }

/* 
 * Call routine to read parameters and call modeling drawing routine.
 */
   mog_trc_plt_driver(argv[2]);   /* 01/04/89 GS - added argument & call glue */

/*
 * terminate plotting gks.
 */
   status = ug_term_plot_gks();

   exit(1);
}
