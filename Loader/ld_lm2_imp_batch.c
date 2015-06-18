/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_BATCH.C */
/* *2     1-MAY-1991 12:30:23 KEE "(SPR -1) Landmark import batch main" */
/* *1     1-MAY-1991 12:28:11 KEE "Landmark import batch main" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_BATCH.C */
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

/* ************************************************************************

   NAME: LD_LM2_IMP_BATCH.PC

   AUTHOR: Rod Hanks
   DATE:   May 7th, 1990
   DESCRIPTION: Batch job to import information from a files output by
		landmark.

   ************************************************************************ */

#ifndef ESI_ORACLE_H
#include "esi_oracle.h"
#endif
#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_CT_ERR_H
#include "esi_ct_err.h"
#endif
#ifndef ESI_SL_METH_AH
#include "esi_sl_meth.ah"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_FI_H
#include "esi_fi.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef LD_LM2_IMP_WORK_H
#include "ld_lm2_imp_work.h"
#endif

publicdef INT main()
    {
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    PROJECT_NAME project;
    PROJECT_NAME database;
    ANALYST_NAME account;
    ORACLE_NAME password;
    CHAR *pos;
    CHAR dummy[255];
    CHAR shot_xy[128];
    CHAR shot_trace[128];
    INT  horizonFCount;
    INT  horizonNCount;
    CHAR horizons[32][128];
    CHAR horizonName[32][128];
    CHAR fault_planes[128];
    CHAR fault_polygons[128];
    CHAR well_info[128];
    CHAR well_dirsrvys[128];
    CHAR well_chkshots[128];
    CHAR well_log[128];
    CHAR well_tops[128];
    CHAR culture[128];
    CHAR source[13];
    INT  overwrite;
    CHAR *login_list[3];
    INT status;
    BOOL convertProj;
    PROJECTION_NAME	  default_projection;
    PROJECTION_NAME	  input_projection;
    PROJECTION_STRUCTURE *defaultProj;
    PROJECTION_STRUCTURE *inputProj;
    INT loop;

    DOUBLE	lower_left_x;
    DOUBLE	lower_left_y;
    DOUBLE	upper_right_x;
    DOUBLE	upper_right_y;
    CHAR	tempValue[241];
/*
				General variable declarations.
*/

/*
				Oracle variable declarations.
*/
/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    account[0] = 0;
    password[0] = 0;
    database[0] = 0;
    project[0] = 0;
    shot_xy[0] = 0;
    shot_trace[0] = 0;
    horizonFCount = 0;
    horizonNCount = 0;
    fault_planes[0] = 0;
    fault_polygons[0] = 0;
    well_info[0] = 0;
    well_dirsrvys[0] = 0;
    well_chkshots[0] = 0;
    well_log[0] = 0;
    well_tops[0] = 0;
    culture[0] = 0;
    source[0] = 0;
    overwrite = 0;
/*
				Initialize in case the dialog file doesn't
				contain one of these.
*/
    while (gets(dummy) != (CHAR *) 0)
        {
            pos = dummy;
	    while (*pos != ':' && *pos != 0)
	        {
		pos++;
		}
	    if (*pos == ':')
	        {
		*pos = 0;
		*pos++;
		}
	    if (strcmp(dummy, "ACCOUNT") == 0)
		{
		strcpy(account, pos);
		}
	    else if (strcmp(dummy, "PASSWORD") == 0)
		{
		strcpy(password, pos);
		}
	    else if (strcmp(dummy, "DATABASE") == 0)
		{
		strcpy(database, pos);
		}
	    else if (strcmp(dummy, "PROJECT") == 0)
		{
		strcpy(project, pos);
		}
	    else if (strcmp(dummy, "LOWER_LEFT_X") == 0)
	        {
	        strcpy(tempValue, pos);
		sscanf(tempValue, "%lf", &lower_left_x);
	        }
	    else if (strcmp(dummy, "LOWER_LEFT_Y") == 0)
	        {
	        strcpy(tempValue, pos);
	        sscanf(tempValue, "%lf", &lower_left_y);
	        }
	    else if (strcmp(dummy, "UPPER_RIGHT_X") == 0)
	        {
	        strcpy(tempValue, pos);
	        sscanf(tempValue, "%lf", &upper_right_x);
	        }
	    else if (strcmp(dummy, "UPPER_RIGHT_Y") == 0)
	        {
	        strcpy(tempValue, pos);
	        sscanf(tempValue, "%lf", &upper_right_y);
	        }
	    else if (strcmp(dummy, "PROJECTION") == 0)
		{
		strcpy(input_projection, pos);
		}
	    else if (strcmp(dummy, "SHOT_XY") == 0)
		{
		strcpy(shot_xy, pos);
		}
	    else if (strcmp(dummy, "SHOT_TRACE") == 0)
		{
		strcpy(shot_trace, pos);
		}
	    else if (strcmp(dummy, "HORIZONS") == 0 && strlen(pos) > 0)
		{
		strcpy(horizons[horizonFCount], pos);
		horizonFCount++;
		}
	    else if (strcmp(dummy, "HORIZON_NAME") == 0)
		{
		strcpy(horizonName[horizonNCount], pos);
		horizonNCount++;
		}
	    else if (strcmp(dummy, "FAULT_PLANES") == 0)
		{
		strcpy(fault_planes, pos);
		}
	    else if (strcmp(dummy, "FAULT_POLYGONS") == 0)
		{
		strcpy(fault_polygons, pos);
		}
	    else if (strcmp(dummy, "WELL_INFO") == 0)
		{
		strcpy(well_info, pos);
		}
	    else if (strcmp(dummy, "WELL_CHKSHOTS") == 0)
		{
		strcpy(well_chkshots, pos);
		}
	    else if (strcmp(dummy, "WELL_DIRSRVYS") == 0)
		{
		strcpy(well_dirsrvys, pos);
		}
	    else if (strcmp(dummy, "WELL_LOG") == 0)
		{
		strcpy(well_log, pos);
		}
	    else if (strcmp(dummy, "WELL_TOPS") == 0)
		{
		strcpy(well_tops, pos);
		}
	    else if (strcmp(dummy, "CULTURE") == 0)
		{
		strcpy(culture, pos);
		}
	    else if (strcmp(dummy, "SOURCE") == 0)
		{
		strncpy(source, pos, 12);
		source[12] = 0;
		}
	    else if (strcmp(dummy, "OVERWRITE") == 0)
		{
		sscanf(pos, "%d", &overwrite);
		}
	}
/*
				Above, retrieve all the lines in the command
				file and parse them out by label.
*/
    strcpy(dummy, account);
    strcat(dummy, "/");
    strcat(dummy, password);
    if (strlen(database) > 0)
	{
	strcat(dummy, "@");
	strcat(dummy, database);
	} 
    login_list[0] = dummy;
    login_list[1] = login_list[0];
    login_list[2] = login_list[0];
    status = or_login(3, login_list);
/*
				I give up, why DO we pass it 3 times?
*/
    if (status != 0)
        {
        printf("Unable to login to Oracle, code = %d\n", status);
        }
/*
				Above, pull out the initial info from the
				command file, and log on to Oracle.
*/
    else
	{
	am_initialize();
        qs_set_c(QS_PROJECT_NAME, project, 0);
        if ((status = finder_config_project(FALSE)) != SUCCESS)
            {
    	    printf("Unable to connect to project\n");
	    }
	else
	    {
	    status = ct_get_default_project(default_projection);
	    if (status != SUCCESS) ld_show_error(status, mg_message(status));
	    if (strcmp(default_projection, input_projection) != 0)
		{
		status = ct_get_projection(default_projection, &defaultProj);
		if (status != SUCCESS) ld_show_error(status, mg_message(status));
		status = ct_get_projection(input_projection, &inputProj);
		if (status != SUCCESS) ld_show_error(status, mg_message(status));
		convertProj = TRUE;
		}
	    else
		{
		convertProj = FALSE;
		}
/*
					Above, decide on whether we must 
					convert the projection on the way
					in.
*/
/*****************************************************************************

	M a i n   L o g i c .

*****************************************************************************/
	    if (strlen(shot_xy) > 0)
		{
		ld_lm2_imp_shot_xy(shot_xy, project,
				      convertProj, defaultProj, 
				      inputProj, source, overwrite,
				      lower_left_x, lower_left_y,
				      upper_right_x, upper_right_y);
		}
	    if (strlen(shot_trace) > 0)
		{
		ld_lm2_imp_shot_trace(shot_trace, project,
				      convertProj, defaultProj, 
				      inputProj, source, overwrite);
		}
	    if (strlen(fault_planes) > 0)
		{
		ld_lm2_imp_fp(fault_planes, project,
				      convertProj, defaultProj, 
				      inputProj, source, overwrite);
		}
	    for (loop = 0; loop < horizonFCount && loop < horizonNCount; loop++)
		{
		ld_lm2_imp_hrz(horizons[loop], project, horizonName[loop],
				      convertProj, defaultProj, 
				      inputProj, source, overwrite,
				      lower_left_x, lower_left_y,
				      upper_right_x, upper_right_y);
		}
	    if (strlen(fault_polygons) > 0 && horizonNCount > 0)
	      {
		for (loop = 0; loop < horizonNCount; loop++)
		  {
		    ld_lm2_imp_fault(fault_polygons, project,
				     convertProj, defaultProj, 
				     inputProj, source, overwrite,
				     lower_left_x, lower_left_y,
				     upper_right_x, upper_right_y,
				     horizonName[loop]);
		  }
	      }
	    if (strlen(well_info) > 0)
		{
		ld_lm2_imp_wells(well_info, project,
				      convertProj, defaultProj, 
				      inputProj, source, overwrite,
				      lower_left_x, lower_left_y,
				      upper_right_x, upper_right_y);
		}
	    if (strlen(well_dirsrvys) > 0)
		{
		ld_lm2_imp_dirsrvys(well_dirsrvys, project,
				      convertProj, defaultProj, 
				      inputProj, source, overwrite);
		}
	    if (strlen(well_chkshots) > 0)
		{
		ld_lm2_imp_chkshots(well_chkshots, project,
				    source, overwrite);
		}
	    if (strlen(well_log) > 0)
		{
		ld_lm2_imp_logs(project, well_log, source, overwrite);
		}
	    if (strlen(well_tops) > 0)
		{
		ld_lm2_imp_tops(well_tops, project, source, overwrite);
		}
	    if (strlen(culture) > 0)
		{
		ld_lm2_imp_culture(culture, project,
				      convertProj, defaultProj, 
				      inputProj, source, overwrite,
				      lower_left_x, lower_left_y,
				      upper_right_x, upper_right_y);
		}
/****************************************************************************

	C l e a n   U p   A n d   E x i t .

******************************************************************************/
	    exit(EXIT_SUCCESS);
	    }
	}
    }
