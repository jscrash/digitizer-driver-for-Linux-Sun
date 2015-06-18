/* DEC/CMS REPLACEMENT HISTORY, Element LD_ZYCOR_EXP_SERVER.C */
/* *3    14-AUG-1991 12:00:09 KEE "(SPR -1) Put in File Lock/Unlock logic" */
/* *2     1-MAY-1991 22:09:09 KEE "(SPR -1) Zycor Export" */
/* *1     1-MAY-1991 21:32:28 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_ZYCOR_EXP_SERVER.C */
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

/* TITLE : LD_ZYCOR_EXP_SERVER - main server for export to ZYCOR*/

/* START-HISTORY :
   05/17/90	Rod Hanks (BHP - Consultant)  Original Development.

   MODIFY: Jim Green (Unocal)
   DATE:   November, 1990

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
#ifndef LD_ZYCOR_WORK_H
#include "ld_zycor_work.h"
#endif

#include "ld_zycor_exp_main.rh"

#if USE_PROTOTYPES
static INT ld_zycor_exp_batch_job (ZYCOR_WORKSPACE *workspace);
#else
static INT  ld_zycor_exp_batch_job();
#endif

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   **
   **   ROUTINE:  ld_zycor_exp_server (id, item, pointer)
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
publicdef INT ld_zycor_exp_server(INT id, INT item, ZYCOR_WORKSPACE *workspace)
#else
publicdef INT ld_zycor_exp_server(id, item, workspace)
INT                 id;
INT                 item;
ZYCOR_WORKSPACE *workspace;
#endif
{
    CHAR text[STMAX];
    INT item_count;
    INT items[32];
    BOOL goFlag;
    INT  faultVal;
    INT  cultureVal;
    INT  xyzVal;
    INT  status;

    PROGNAME ("ld_zycor_exp_server") ;

    switch (item)
    {
	case ZEX_OKAY:

	    goFlag = TRUE;
	    wi_query_selections(workspace->dialogID, ZEX_WHICH_MAP,
					&item_count, items);
	    if (item_count == 0)
		{
		wi_beep(1);
		wi_alert(0, "", "",
			"You must choose a map.");
		goFlag = FALSE;
	        }		
	    if (goFlag == TRUE)
		{
		status = wi_query_dialog_item(workspace->dialogID, ZEX_FAULT, 
					&faultVal, text);
		status = wi_query_dialog_item(workspace->dialogID, ZEX_CULTURE, 
					&cultureVal, text);
		status = wi_query_dialog_item(workspace->dialogID, ZEX_XYZ, 
					&xyzVal, text);
		if (faultVal == 0
		&&  cultureVal == 0
		&&  xyzVal == 0)
		    {
		    wi_beep(1);
		    wi_alert(0, "", "",
			"You must mark at least one checkbox.");
		    goFlag = FALSE;
		    }
		}
	    if (goFlag)
		{
	        ld_zycor_file_pipe(workspace, items[0], faultVal, 
				   cultureVal, xyzVal, &goFlag);
		}
	    if (goFlag)
		{
	     	ld_zycor_exp_batch_job(workspace);
	     	am_quit();
		}
	     break;

	 case ZEX_CANCEL:
	     am_quit();
	     break;

     }

     return SUCCESS;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   **
   **   ROUTINE:  ld_zycor_exp_batch_job(workspace)
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

static INT ld_zycor_exp_batch_job (workspace)
ZYCOR_WORKSPACE     *workspace;
{
     INT       code, job_user_no;
     CHAR      login_oracle[STMAX], message[STMAX];
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

     PROGNAME ("ld_zycor_exp_batch_job") ;

     qs_inq_c ( QS_FINDER_ACCOUNT , account , &len) ;
     qs_inq_c ( QS_FINDER_PASSWORD , password , &len) ;
     qs_inq_c ( QS_DATABASE , database , &len ) ;
     qs_inq_c ( QS_PROJECT_NAME, project, (INT *)0);

     ho_date_tag (date_tag);

/*   name of the batch loader   */
     ho_add_path("ESI$BIN", "ld_zycor_batch", 
				sizeof batch_loader, batch_loader);

#ifdef vms

/*  name of the DCL file which is submitted to run the loader  */
     ho_get_unique_filename(baseFilename);
     sprintf(comi_file, "ZYE_%s.COM", baseFilename);

#endif
#if UNIX
/*    name of the script file which is submitted to run the loader  */
     sprintf(comi_file, "zycorexp%s", date_tag);

#endif


#ifdef primos

/*  name of the CPL file which is submitted to run the loader  */
     sprintf(comi_file, "ZYE_%s.CPL", date_tag);

#endif

     /* Create command file */
     comi_fp = ho_open( comi_file, "w", NULL );
     status = ho_lock( comi_fp, ON);

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
     status = wi_query_dialog_item(workspace->dialogID, ZEX_FAULT, 
					&value, text);
     if (value == 1)
	{
        fprintf(comi_fp, "FAULT:%1s\n", workspace->faultFile);
	}
     status = wi_query_dialog_item(workspace->dialogID, ZEX_CULTURE, 
					&value, text);
     if (value == 1)
	{
     	fprintf(comi_fp, "CULTURE:%1s\n", workspace->cultureFile);
	}
     status = wi_query_dialog_item(workspace->dialogID, ZEX_XYZ, 
					&value, text);
     if (value == 1)
	{
        fprintf(comi_fp, "XYZ:%1s\n", workspace->xyzFile);
	}
     status = wi_query_selections(workspace->dialogID, ZEX_WHICH_MAP,
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
	sprintf(message, "Export to ZYCOR, can't spawn %s, code %d",
			comi_file, code);
	}
     else
	{
	sprintf(message, "Export to ZYCOR being started as job #%d", 
				job_user_no);
	}
     am_message(AM_STATUS, message);

     return SUCCESS;
}
                                                                                                          
