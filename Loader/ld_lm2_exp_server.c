/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_EXP_SERVER.C */
/* *3    14-AUG-1991 11:58:59 KEE "(SPR -1) Put in File Lock/Unlock logic" */
/* *2     1-MAY-1991 21:58:34 KEE "(SPR -1) Landmark Export" */
/* *1     1-MAY-1991 21:07:47 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_EXP_SERVER.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* TITLE : LD_LM2_EXP_SERVER - main server for export to landmark*/

/* START-HISTORY :
   05/02/90	Rod Hanks	Original Development.
   05/30/90	Rod Hanks	Add server for curve names.
   END-HISTORY */

/* START-CODE : */
/*
   **                                 INCLUDE FILES
*/
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
#ifndef ESI_LD_MSG_H
#include "esi_ld_msg.h"
#endif
#ifndef ESI_LD_ERR_H
#include "esi_ld_err.h"
#endif
#ifndef LD_LM2_EXP_WORK_H
#include "ld_lm2_exp_work.h"
#endif

#include "ld_lm2_exp_main.rh"

#if USE_PROTOTYPES
static INT ld_lm2_exp_batch_job (LANDMARK_WORKSPACE *workspace);
#else
static INT  ld_lm2_exp_batch_job();
#endif

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   **
   **   ROUTINE:  ld_lm2_exp_server (id, item, pointer)
   **
   **   DESCRIPTION:
   **
   **        Main custom dialog server.
   **
   **   PARAMETERS:
   **
   **        id        (INT) Dialog id for the landmark application driver's
   **                  custom dialog, assigned by the am_define_resource service.
   **
   **        item      (INT) Resource item to process.
   **
   **        pointer   (WORKSPACE *) Pointer to the application workspace,
   **                  which was defined in the calling routine.
   **
*/
#if USE_PROTOTYPES
publicdef INT ld_lm2_exp_server(INT id, INT item, LANDMARK_WORKSPACE *workspace)
#else
publicdef INT ld_lm2_exp_server(id, item, workspace)
INT                 id;
INT                 item;
LANDMARK_WORKSPACE *workspace;
#endif
{
    CHAR text[STMAX];
    INT item_count;
    INT items[32];
    BOOL goFlag;
    INT xyVal;
    INT spVal;
    INT fpVal;
    INT fltVal;
    INT wellVal;
    INT cultVal;
    INT logVal;
    INT topVal;
    INT hrzVal;
    INT status;

    PROGNAME ("ld_lm2_exp_server") ;

    switch (item)
    {
	 case LEX_OKAY:

	     goFlag = TRUE;
	     wi_query_selections(workspace->dialogID, 
					LMP_PROJECTION,
					&item_count, items);
	     if (item_count == 0)
		{
		wi_beep(1);
		wi_alert(0, "", "",
			"You must choose the projection for the output data.");
		goFlag = FALSE;
		}		
	     if (goFlag)
		{
		wi_query_selections(workspace->dialogID, LEX_WHICH_MAP,
					&item_count, items);
	 	if (item_count == 0)
		    {
		    wi_beep(1);
		    wi_alert(0, "", "",
			"You must choose a map.");
		    goFlag = FALSE;
		    }		
		}
	    if (goFlag)
		{
		status = wi_query_dialog_item(workspace->dialogID, LEX_SEISMIC, 
					&xyVal, text);
		spVal = xyVal;
		status = wi_query_dialog_item(workspace->dialogID, 
					LEX_HORIZONS, 
					&hrzVal, text);
		status = wi_query_dialog_item(workspace->dialogID, LEX_FAULTS, 
					&fpVal, text);
		fltVal = fpVal;
		status = wi_query_dialog_item(workspace->dialogID, LEX_WELLS, 
					&wellVal, text);
		status = wi_query_dialog_item(workspace->dialogID, LEX_TOPS, 
					&topVal, text);
		status = wi_query_dialog_item(workspace->dialogID, LEX_LOGS, 
					&logVal, text);
		status = wi_query_dialog_item(workspace->dialogID, LEX_CULTURE, 
					&cultVal, text);
		if (xyVal == 0
		&&  spVal == 0
		&&  fltVal == 0
		&&  wellVal == 0
		&&  cultVal == 0
		&&  logVal == 0
		&&  topVal == 0
		&&  hrzVal == 0
		&&  fpVal == 0)
		    {
		    wi_beep(1);
		    wi_alert(0, "", "",
			"You must mark at least one checkbox.");
		    goFlag = FALSE;
		    }
		else
		    {
		    ld_lm2_exp_files(workspace, items[0], xyVal, 
				   spVal, fpVal, fltVal, wellVal,
				   cultVal, logVal, topVal, hrzVal, &goFlag);
		    }
		}
	    if (goFlag)
		{
		if (logVal != 0)
		    {
		    ld_lm2_curve_pipe(workspace, &goFlag);
		    }
		}
	    if (goFlag)
		{
		if (logVal != 0 || topVal != 0)
		    {
		    ld_lm2_source_pipe(workspace, &goFlag);
		    }
		}
	    if (goFlag)
		{
		ld_lm2_exp_batch_job(workspace);
		am_quit();
		}
	     break;

	 case LEX_CANCEL:
	     am_quit();
	     break;

     }

     return SUCCESS;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   **
   **   ROUTINE:  ld_lm2_exp_batch_job
   **
   **   DESCRIPTION:
   **
   **        Batch job generator. The command file necessary for the 
   **	     batch processing of the map data is created. 
   **
   **        A unique name will be generated for these files and for files
   **
   **   PARAMETERS:
   **
   **        workspace (WORKSPACE *) Pointer to the application workspace.
   **
*/

static INT ld_lm2_exp_batch_job (workspace)
LANDMARK_WORKSPACE     *workspace;
{
     INT       code, job_user_no;
     CHAR      message[STMAX];
     CHAR      date_tag[15];
     FILENAME  batch_loader;
     FILE      *comi_fp;
     FILENAME  comi_file;
     CHAR   directory [200] ;
     PROJECT_NAME project;
     PROJECT_NAME database;
     ANALYST_NAME account;
     ORACLE_NAME password;
     INT value;
     CHAR text[255];
     INT item_count;
     INT items[32];
     INT len;
     INT status;
     INT loop;
     FILENAME baseFilename;

     PROGNAME ("ld_lm2_exp_batch_job") ;

/*   allocate memory for protocol structure for the server YESNO   */

     qs_inq_c ( QS_FINDER_ACCOUNT , account , &len) ;
     qs_inq_c ( QS_FINDER_PASSWORD , password , &len) ;
     qs_inq_c ( QS_DATABASE , database , &len ) ;
     qs_inq_c ( QS_PROJECT_NAME, project, (INT *)0);

     ho_date_tag (date_tag);

/*   name of the batch loader   */
     ho_add_path("ESI$BIN", "ld_lm2_exp_batch", 
				sizeof batch_loader, batch_loader);

#ifdef vms

/*  name of the DCL file which is submitted to run the loader  */
     ho_get_unique_filename(baseFilename);
     sprintf(comi_file, "LME_%s.COM", baseFilename);

#endif
#if UNIX
/*    name of the script file which is submitted to run the loader  */
     sprintf(comi_file, "lm2exp%s", date_tag);

#endif


#ifdef primos

/*  name of the CPL file which is submitted to run the loader  */
     sprintf(comi_file, "LME_%s.CPL", date_tag);

#endif

     /* Create command file */
     comi_fp = ho_open( comi_file, "w", NULL );
     status = ho_lock(comi_fp, ON);

     if (comi_fp EQUALS NULL_FILE_PTR || status != SUCCESS)
     {
	  sprintf(message, mg_message(LD_COMFILE_NOT_OPEN));
	  am_message(AM_ALERT, message);

	  return FAIL;
     }

     /* Write commands to cominput file */

     /*   inquire for the default directory where we are running */

      ho_get_current_path (directory ) ;

#ifdef vms
     fprintf(comi_fp, "$SET DEFAULT %s\n", directory );
     fprintf(comi_fp, "$RUN/NODEBUG %s\n", batch_loader);
#endif
#if UNIX
     fprintf(comi_fp, "cd %s\n", directory );
     fprintf(comi_fp, "%s << ENDOFDATA\n", batch_loader);
#endif

#ifdef primos
#endif
     fprintf(comi_fp, "ACCOUNT:%s\n", account);
     fprintf(comi_fp, "PASSWORD:%s\n", password);
     fprintf(comi_fp, "DATABASE:%s\n", database);
     fprintf(comi_fp, "PROJECT:%s\n", project);
     status = wi_query_selections(workspace->dialogID, LMP_PROJECTION,
					&item_count, items);
     fprintf(comi_fp, "PROJECTION:%s\n", 
			workspace->projectionList[items[0]-1]);

     status = wi_query_dialog_item(workspace->dialogID, LEX_SEISMIC, 
					&value, text);
     if (value != 0)
	{
        fprintf(comi_fp, "SEISMIC:%s\n", workspace->file_names[0]);
	fprintf(comi_fp, "TRACES:%s\n", workspace->file_names[1]);
	}
     status = wi_query_dialog_item(workspace->dialogID, LEX_HORIZONS, 
					&value, text);
     if (value != 0)
	{
	for (loop = 10; loop < workspace->fileCount; loop++)
	    {
            fprintf(comi_fp, "HORIZON NAME:%s\n", workspace->titles[loop]);
            fprintf(comi_fp, "HORIZON:%s\n", workspace->file_names[loop]);
	    }
	}
     status = wi_query_dialog_item(workspace->dialogID, LEX_FAULTS, 
					&value, text);
     if (value != 0)
	{
        fprintf(comi_fp, "FAULT:%s\n", workspace->file_names[2]);
        fprintf(comi_fp, "FPLANE:%s\n", workspace->file_names[3]);
	}
     status = wi_query_dialog_item(workspace->dialogID, LEX_WELLS, 
					&value, text);
     if (value != 0)
	{
        fprintf(comi_fp, "WELL:%s\n", workspace->file_names[4]);
        fprintf(comi_fp, "CHKSHOT:%s\n", workspace->file_names[5]);
        fprintf(comi_fp, "DIRSRVY:%s\n", workspace->file_names[6]);
	}
     status = wi_query_dialog_item(workspace->dialogID, LEX_TOPS, 
					&value, text);
     if (value != 0)
	{
        fprintf(comi_fp, "TOP:%s\n", workspace->file_names[8]);
	}
     status = wi_query_dialog_item(workspace->dialogID, LEX_LOGS, 
					&value, text);
     if (value != 0)
	{
        fprintf(comi_fp, "LOG:%s\n", workspace->file_names[7]);
	if (value != 0 && workspace->ncurves > 0)
	    {
	    for (loop = 0; loop < workspace->ncurvesPicked; loop++)
	        {
	        fprintf(comi_fp, "CURVNAM:%s\n", 
		workspace->tcp_of_curves[workspace->curvesPicked[loop] - 1]);
	        }
	    }
	}
     if (workspace->sourcePicked > 0 
     && workspace->sourcePicked <= workspace->nsources)
	{
	fprintf (comi_fp, "SOURCE:%s\n", 
		workspace->tcp_of_sources[workspace->sourcePicked - 1]);
	}
     status = wi_query_dialog_item(workspace->dialogID, LEX_CULTURE, 
					&value, text);
     if (value != 0)
	{
        fprintf(comi_fp, "CULTURE:%s\n", workspace->file_names[9]);
	}

     status = wi_query_selections(workspace->dialogID, LEX_WHICH_MAP,
					&item_count, items);
     for (loop = 0; loop < item_count; loop++)
	{
	fprintf(comi_fp, "MAPNAME:%s\n", 
			workspace->tcp_of_maps[items[loop]-1]);
	}

#ifdef vms
     fprintf(comi_fp, "$EXIT \n");
#endif

#if UNIX
     fprintf(comi_fp, "ENDOFDATA\n", batch_loader);
#endif

#ifdef primos
#endif

     status = ho_lock(comi_fp, OFF);
     fclose(comi_fp);

     code = ho_spawn(SPAWN_BATCH, comi_file, "DELETE", &job_user_no);

     if (code)
	{
	sprintf(message, "Export to Landmark 2d, can't spawn %s, code %d",
			comi_file, code);
	}
     else
	{
	sprintf(message, "Export to Landmark 2d being started as job #%d", 
				job_user_no);
	}
     am_message(AM_STATUS, message);

     return SUCCESS;
}
                                                                                                          
