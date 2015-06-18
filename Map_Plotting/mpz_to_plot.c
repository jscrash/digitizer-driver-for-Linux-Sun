/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_TO_PLOT.C*/
/*  *15   11-OCT-1991 13:41:30 MING "(SPR 4475) change MARGIN 1.5 to 1.4 in order to be consistent in pl_read_map"*/
/*  *14   23-SEP-1991 16:03:50 MING "(SPR 4158) fix the max length put out for strings verbs, keywords, parameters"*/
/*  *13   14-AUG-1991 12:00:26 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/*  *12   11-JUL-1991 13:59:07 MING "(SPR 0) put a - in the display input line"*/
/*  *11   19-JUN-1991 16:15:35 MING "(SPR 0) add ho_translate_filename on data_file"*/
/*  *10    7-JUN-1991 13:52:08 MING "(SPR 0) change #if PRIOR_GKS to #ifdef PRIOR_GKS"*/
/*  *9     6-JUN-1991 15:19:37 MING "(SPR 0) new plot file managment"*/
/*  *8     7-DEC-1990 09:19:18 MING "(SPR -1) fix scaling in neutral plot file"*/
/*  *7    17-OCT-1990 16:08:07 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  *6    27-APR-1990 13:05:36 MING "(SPR 0) add metatype in PDS"*/
/*  *5    13-DEC-1989 14:32:17 CONROY "(SPR 5071) Mods for label plotting fix"*/
/*  *4    17-OCT-1989 15:43:05 GORDON "(SPR 0) Change tmpnam back to ho_get_unique_filename so it works for multiple sessions."*/
/*  *3     6-OCT-1989 09:47:37 GORDON "(SPR 4999) use ANSI function tmpnam to get temp file names"*/
/*  *2    26-JUN-1989 10:51:15 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:12 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_TO_PLOT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element MPZ_TO_PLOT.C*/
/* *2    11-OCT-1990 23:59:07 SCD "(SPR 6075) plot composer checkin"*/
/* *1    13-AUG-1990 16:58:55 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element MPZ_TO_PLOT.C*/

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

   NAME: MPZ_MAP_TO_PLOTTER.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: Function to set up and initiate the batch plotting of the
		current map.

   ************************************************************************ */
#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_pf.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_qs.h"
#include "esi_ho.h"
#include "esi_ho_files.h"

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#include "esi_md_err.h"
#include "esi_md_msg.h"

#include "esi_md_basis.x"

#define MARGIN 1.4


			     /*	* FUNCTION TITLE * ------------------	    */

publicdef INT mpz_map_to_plotter(full)

/* * ARGUMENT LIST * */
/* ------------------ */

BOOL full;

/* * VARIABLE LIST * */
/* ----------------- */

{

INT status;
MAP_STRUCTURE *mapdef;
MD_DISPLAY *display;
MD_COMPILED_MAPDEF *compiled;
MD_COMPILED_VERB *verbstruc;
MD_PARM_STRUCT *parmstruc;
MD_STATE_WORKSPACE *md_state;
FILE *fp;
PATHNAME cmd_file;
PATHNAME data_file;
PATHNAME plot_file;
PATHNAME tmp_file;
PATHNAME arg_file;
PATHNAME plot_dir;
PROJECT_NAME project;
CHAR layout[40];
CHAR account[31];
CHAR password[31];
CHAR database[31];
CHAR arg_list[512];
INT next_type;
INT sub_type;
INT string_length;
INT front_dialog;
CHAR msg[STMAX];
INT i,j,start,end;
PDS *pinfo;	/* Plotter description struct ptr. */
INT verb_ndx,key_ndx;
MD_KEY_INFO *key_info;
CHAR **tcp;
DOUBLE max_size;
PLOT_LAYOUT pl;
PF_INFO pf_info;
ORACLE_NAME plot_type;
DOUBLE wcs_to_inches;

/* ********************************************************************** */

/* * GET PLOTTER INFO * */
/* -------------------- */

pinfo = (PDS *)tc_zalloc(sizeof(PDS));

/* * OPEN WORKSPACES * */
/* ------------------- */

am_open_workspace("MAPDEF",AM_APPLICATION,(VOIDPTR *)&mapdef);
am_open_workspace("MAPDEF_STATE",AM_GLOBAL,(VOIDPTR *)&md_state);

/* * GET QS VARIABLES * */
/* -------------------- */
qs_inq_c(QS_PROJECT_NAME,project,(INT *) 0);
qs_inq_c(QS_FINDER_ACCOUNT,account,(INT *) 0);
qs_inq_c(QS_FINDER_PASSWORD,password,(INT *) 0);
qs_inq_c(QS_DATABASE,database,(INT *) 0);

    
strcpy (pf_info.analyst,account);
strcpy (pf_info.app,"MAPPING");
strcpy (pf_info.finder_path,"ESI$PLOTS");
strcpy (pf_info.filename,mapdef->map_name);

/* default plot size */
mp_get_plot_wcs_to_inches (mapdef,&wcs_to_inches);
pf_info.xsize = (mapdef->upper_right_xy.x - mapdef->lower_left_xy.x)
	          * wcs_to_inches + 2 * MARGIN;
pf_info.ysize = (mapdef->upper_right_xy.y - mapdef->lower_left_xy.y)
	          * wcs_to_inches + 2 * MARGIN;

#ifdef PRIOR_GKS
status = pf_get_map_plot_info (pinfo,layout,&pf_info,PL_MAP);
#else
status = pf_get_plot_info (pinfo,layout,&pf_info,PL_MAP);
#endif

if (status != SUCCESS)
    {
    goto finished;
    }

display = mapdef->display;
compiled = mapdef->compiled;


/* * CONSTRUCT COMMUNICATION FILE, PLOTFILE, ARGUMENT FILE NAMES * */
/* -------------------------------------------------------------- */

ho_get_unique_filename (data_file);
sprintf (data_file,"%s.DAT",data_file);
ho_add_path( "ESI$SCRATCH", data_file, sizeof(PATHNAME), data_file );
ho_translate_filename (data_file,data_file);

max_size = MAX(pf_info.xsize,pf_info.ysize);  

/* ------------------------------------------------------------- */
/* 0.01 small offset to make sure the plot size is not less than */
/* the original one due to rounding                              */
/* ------------------------------------------------------------- */
max_size = (max_size + 0.01);
  
/* * CONSTRUCT ARGUMENT LIST FOR BATCH PLOT PROGRAM * */
/* --------------------------------------------------- */

sprintf(arg_list,"\"%s/%s@%s\",%s,\"%s@%s@%s\",%.2f,\"%s\",\"%s@%s\",\"%2d@%1d\",\"%s@%s\"",
                                account,
				password,
				database,
				project,
				pf_info.filename,
				pf_info.meta_type,
				data_file,
				max_size,
                pf_info.layout,
				pinfo->driver,
				pinfo->queue,
				pf_info.copies,
				pf_info.save,
				pf_info.expired_date,
				pf_info.remark);

/* * OPEN COMMUNICATIONS FILE * */
/* ---------------------------- */

fp = ho_open(data_file,"w",NULL);
status = ho_lock(fp, ON);
if (status != SUCCESS)
    {
    goto finished;
    }

/* * PROCESS HEADER INFO * */
/* ----------------------- */

fprintf(fp,"%s",mapdef->map_name);
fprintf(fp,"\n%s",mapdef->map_projection);
fprintf(fp,"\n%s",mapdef->map_coordinate_system);
fprintf(fp,"\n%f %f",mapdef->lower_left_xy.x,mapdef->lower_left_xy.y);
fprintf(fp,"\n%f %f",mapdef->upper_right_xy.x,mapdef->upper_right_xy.y);

if (strlen(mapdef->lower_left_str) EQUALS 0)
    {
    fprintf(fp,"\n%s",NULL_FOR_COMM);
    }
else
    {
    fprintf(fp,"\n%s",mapdef->lower_left_str);
    }

if (strlen(mapdef->upper_right_str) EQUALS 0)
    {
    fprintf(fp,"\n%s",NULL_FOR_COMM);
    }
else
    {
    fprintf(fp,"\n%s",mapdef->upper_right_str);
    }

fprintf(fp,"\n%f %s",mapdef->map_scale,mapdef->map_scale_uom);


/* * PROCESS DISPLAY STRUCTURE * */
/* ----------------------------- */

if (full)
    {
    fprintf(fp,"\n%d",display->last_row_used);
    for (i=0; i<display->last_row_used; i++)
	{
	fprintf(fp,"\n%d %d %d",
	    display->verb_ndx[i],display->key_ndx[i],display->compiled_row[i]);
        fprintf(fp,"\n-%.80s",display->verbs[i]);
	fprintf(fp,"\n-%.80s",display->keywords[i]);
	fprintf(fp,"\n-%.250s",display->parameters[i]);
	}

    start = 0;
    end = compiled->last_row_used + 1;
    fprintf(fp,"\n%d",compiled->last_row_used);
    }
else
    {
    fprintf(fp,"\n1");
    i = md_state->crnt_pref_verb_start - 1;
    fprintf(fp,"\n%d %d 0",display->verb_ndx[i],display->key_ndx[i]);
    fprintf(fp,"\n-%.80s",display->verbs[i]);
    fprintf(fp,"\n-%.80s",display->keywords[i]);
    fprintf(fp,"\n-%.250s",display->parameters[i]);

    start = display->compiled_row[i];
    end = start + 1;
    fprintf(fp,"\n0");
    }


/* * PROCESS COMPILED STRUCTURES * */
/* ------------------------------- */

for (i=start; i<end; i++)
    {
    fprintf(fp,"\n%d",compiled->verb_code[i]);

				    /* CHECK FOR DELETED OVERLAY */

    if ( (verbstruc = compiled->verb_ptr[i]) EQUALS (MD_COMPILED_VERB *)0) 
	{
	continue;
	}
    mp_verb_lookup(compiled->verb_code[i],&verb_ndx);

    for (j=0; j<verbstruc->nrows; j++)
	{

				    /* GET KEY INFORMATION */

	mp_key_lookup(verbstruc->key_code[j],verb_ndx,&key_ndx);
	key_info = &(overlay_verbs[verb_ndx].key_list[key_ndx]);

				    /* WRITE OUT COMPILED KEY INFORMATION */

	parmstruc = verbstruc->parm_ptr[j];
	next_type = (parmstruc != NULL_PARM);
	fprintf(fp,"\n%d %d %d",
		verbstruc->key_code[j],verbstruc->key_used[j],next_type);

					    /* PARAMETER STRUCTURE LOOP */

	while (parmstruc != NULL_PARM)
	    {
	    next_type = (parmstruc->nxtparm != NULL_PARM);

	    sub_type = parmstruc->parmtype;
	    switch (sub_type)
		{
		case 1:
		    string_length = strlen(parmstruc->parmval.cval);
		    fprintf(fp,"\n%d %d %d",next_type,sub_type,
							    string_length);
		    fprintf(fp,"\n%s",parmstruc->parmval.cval);
		    break;

		case 2:
		    fprintf(fp,"\n%d %d 0",next_type,sub_type);
		    fprintf(fp,"\n%ld",parmstruc->parmval.ival);
		    break;

		case 3:
		    fprintf(fp,"\n%d %d 0",next_type,sub_type);
		    fprintf(fp,"\n%lf",parmstruc->parmval.dbval);
		    break;

		default:
		    sprintf(msg,
			    "Unrecognized parameter type; parameter = %s",
						    parmstruc->parmval.cval);
		    am_message(AM_ALERT,msg);
		}

	    parmstruc = parmstruc->nxtparm;
	    }
	}
    }


/* * CLOSE COMMUNICATIONS FILE AND SPAWN BATCH PLOT PROGRAM * */
/* ---------------------------------------------------------- */

status = ho_lock(fp, OFF);
fclose(fp);

ho_add_path("ESI$BIN","mp_plot_map",sizeof(FILENAME),cmd_file);
status = ho_spawn( SPAWN_BATCH, cmd_file, arg_list, (INT *)0 );

finished:
if (pinfo != (PDS *)0)
    free(pinfo);

return status;
}

