/* DEC/CMS REPLACEMENT HISTORY, Element LD_BATCH_SGC_LOAD.C*/
/* *1    28-JUN-1990 11:50:31 GILLESPIE "Resurrect from long ago and far away"*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_BATCH_SGC_LOAD.C*/
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





/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: LD_BATCH_SGC_LOAD.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: Standalone program to load scatter, grid, and/or contour
		files specified in the loader in FINDER.

   ************************************************************************ */


/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_gl_project.h"
#include "esi_qs.h"
#include "esi_am.h"
#include "esi_fm.h"
#include "esi_ts.h"
#include "esi_tc.h"
#include "esi_ho.h"
#include "esi_ho_files.h"
#include "esi_ld_s_g_c.h"


/* * FUNCTION TITLE * */
/* ------------------ */

main(argc,argv)

/* * ARGUEMENT LIST * */
/* ------------------ */

INT argc;
CHAR *argv[];

/* * VARIABLE LIST * */
/* ----------------- */

{

INT status;
INT i,j,k;
INT data_type;
INT num_files;
INT skip;
INT *select_list;
CHAR projection[21];
FILENAME comm_file;
FM_FILE file_name;
FILE *file;
LD_SGC_FILE_STRUCT sgc_struct;

/* ********************************************************************** */


/* * CHECK ARGUEMENT LIST * */
/* ------------------------ */

if (argc != 4)
	{
	printf("\nERROR: WRONG NUMBER OF PARAMTERS (%d) PASSED !",argc);
	goto finished;
	}


/* * LOGIN TO ORACLE * */
/* ------------------- */

qs_set_i(QS_IF_BATCH,TRUE);
status = or_login(2,argv);
if (status < 0)
	{
	printf("\nORACLE LOGIN ERROR: status = %d",status);
	goto finished;
	}

/* * WHICH PROJECT * */
/* ----------------- */

qs_set_c(QS_PROJECT_NAME,argv[2],0);
status = am_initialize();
status = finder_config_project();


/* * OPEN COMMUNICATIONS FILE * */
/* ---------------------------- */

ho_add_path("ESI$REPORTS",argv[3],sizeof(FILENAME),comm_file);
file = ho_open( comm_file, "r", NULL );
if (file EQUALS (FILE *)0)
	{
	printf("\nCouldn't open communications file\n");
	exit(1);
	}

fscanf(file,"%d\n",&data_type);
fgets(sgc_struct.directory,sizeof(FM_FILE),file);
sgc_struct.directory[strlen(sgc_struct.directory) - 1] = '\0';
fscanf(file,"%d\n",&num_files);

sgc_struct.file_list = ts_tcp_alloc(num_files);
select_list = (INT *) tc_alloc(num_files * sizeof(INT));

for (i=0; i<num_files; i++)
    {
    select_list[i] = i+1;

    fgets(file_name,sizeof(FM_FILE),file);
    file_name[strlen(file_name) - 1] = '\0';
    ts_tcp_append(&(sgc_struct.file_list),file_name);
    }


if (data_type EQUALS 1)
    {
    fscanf(file,"%d\n",&skip);
    fgets(projection,22,file);

    ldz_load_scatter_files(&sgc_struct,select_list,num_files,skip,projection);
    }

else
    {
    ldz_load_nlist_files(data_type,&sgc_struct,select_list,num_files);
    }

tc_free(select_list);
ts_tcp_free(sgc_struct.file_list);
fclose(file);
ho_delete_file(comm_file,&status);

finished:
    ;
}
