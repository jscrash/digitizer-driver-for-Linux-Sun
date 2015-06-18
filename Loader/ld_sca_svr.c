/* DEC/CMS REPLACEMENT HISTORY, Element LD_SCA_SVR.C*/
/* *9    27-NOV-1991 17:38:48 MING "(SPR 0) correct the batch command name to ld_sgc_batch"*/
/* *8    16-AUG-1991 16:23:46 JULIAN "(SPR 3671) added messages when error is detected"*/
/* *7    15-AUG-1991 17:14:22 JULIAN "(SPR 3671) renamed the call to batch job"*/
/* *6    14-AUG-1991 11:59:43 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *5    17-DEC-1990 16:28:52 MING "(SPR 6244) bug fix"*/
/* *4    14-AUG-1990 13:32:34 VINCE "(SPR 5644) Header Standardization"*/
/* *3     6-FEB-1990 11:00:06 MING "(SPR 0) change date/time declaration to new data type DATE_TIME"*/
/* *2    26-JUN-1989 10:47:35 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:03:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_SCA_SVR.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_math.h"
#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_am.h"
#include "esi_ts.h"
#include "esi_tc.h"
#include "esi_wi.h"
#include "esi_qs.h"
#include "esi_ho.h"
#include "esi_ho_files.h"
#include "esi_ld_s_g_c.h"
#include "ld_s_g_c_data.rh"
#include "esi_ct.h"

/* Function Description -----------------------------------------------------
Description:
    This function services events from the scatter file loader
    custom dialog.
    
Prototype:
    publicdef INT ld_scatter_srvr(INT id, INT item, BYTE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT)   \
    (I) item        -(INT)    \
    (I) p           -(BYTE *) -Standard AM interface.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_scatter_srvr (INT id, INT item, BYTE *p) 
#else
publicdef INT ld_scatter_srvr (id, item, p)
INT id;
INT item;
BYTE *p;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT status = SUCCESS;
    INT num_files;
    INT skip;
    INT i;
    INT *select_list;
    CHAR text[80];
    DATE_TIME date_time;
    CHAR account[31];
    CHAR password[31];
    CHAR database[31];
    CHAR arg_list[512];
    CHAR msg[512];
    PROJECT_NAME project;
    FILENAME temp_filename;
    FILE *temp_file;
    LD_SGC_LOAD_STRUCT *sgc_files;
    PROJECTION_STRUCTURE *projection_data;
    
    /**************************************************************************/
    
    /* * QUIT THE DIALOG * */
    /* ------------------- */
    
    if (item EQUALS LD_QUIT_2)
        {
        wi_close_dialog (id);
        return SUCCESS;
        }
    /* * DO THE LOADING * */
    /* ------------------ */
    
    am_open_workspace ("LD_SGC_FILES", AM_GLOBAL, (VOIDPTR *) & sgc_files);
    num_files = ts_tcp_len (sgc_files->scatter.file_list);
    if (num_files <= 0)
        {
        return SUCCESS;
        }
    select_list = (INT *)tc_alloc (num_files * sizeof(INT));
    
    wi_query_selections (id, LD_XYZ_SELECT, &num_files, select_list);
    if (num_files > 0)
        {
        wi_query_dialog_item (id, LD_NUM_RECORDS, &i, text);
        skip = atol (text);
        
        wi_query_dialog_item (id, LD_PROJECTION, &i, text);

	/* ---------------------------- */
	/* check if projection is valid */
	/* ---------------------------- */
	status = ct_get_projection (text, &projection_data);
	if (status != SUCCESS)
	    {
	    am_message (AM_ALERT,"The projection you specified does not exist for this project. Please add it to the project database and restart the load.");
            return FAIL;
	    }
       
        
        /* * RUN INTERACTIVE LOADING * */
        /* --------------------------- */
        
        if (item EQUALS LD_RUN_1)
            {
            ldz_load_scatter_files (&(sgc_files->scatter), select_list, num_files,
                                    skip, text);
            }
        /* * SUBMIT LOADING TO A BATCH PROCESS * */
        /* ------------------------------------- */
        
        else if (item EQUALS LD_SUBMIT_1)
            {
            
            /* * GET QS VARIABLES * */
            /* -------------------- */
            
            qs_inq_c (QS_PROJECT_NAME, project, 0);
            qs_inq_c (QS_FINDER_ACCOUNT, account, 0);
            qs_inq_c (QS_FINDER_PASSWORD, password, 0);
            qs_inq_c (QS_DATABASE, database, 0);
            
            /* * CONSTRUCT COMMUNICATION (temp) AND NEUTRAL PLOT FILE NAMES * */
            /* -------------------------------------------------------------- */
            
            ho_date_tag (date_time);
            sprintf (temp_filename, "ld_scatter_%s.data", date_time);
            
            /* * CONSTRUCT ARGUEMENT LIST FOR BATCH PLOT PROGRAM * */
            /* --------------------------------------------------- */
            
	    if(IS_STRING(database))
	      sprintf (arg_list, "\"%s/%s@%s\",%s,%s", 
		       account, password, database,
		       project, temp_filename);
	    else
	      sprintf (arg_list, "\"%s/%s\",%s,%s", 
		       account, password,
		       project, temp_filename);
            
            /* * OPEN COMMUNICATIONS FILE * */
            /* ---------------------------- */
            
            ho_add_path ("ESI$REPORTS", temp_filename, sizeof(FILENAME),
                         temp_filename);
            temp_file = ho_open (temp_filename, "w", NULL);
	    status = ho_lock (temp_file, ON);
	    if (status != SUCCESS)
	      {
		sprintf(msg,"Error in locking file %s. %s",
			temp_filename,mg_message(status));
		am_message(AM_ALERT,msg);
		return FAIL;
	      }
 
            /* * WRITE COMMUNICATIONS INFO * */
            /* ----------------------------- */
            
            fprintf (temp_file, "1");   /* DATA TYPE */
            fprintf (temp_file, "\n%s", sgc_files->scatter.directory);
            fprintf (temp_file, "\n%d", num_files);
            for (i = 0; i < num_files; i++)
                {
                fprintf (temp_file, "\n%s",
                         sgc_files->scatter.file_list[select_list[i] - 1]);
                }
            fprintf (temp_file, "\n%d", skip);
            fprintf (temp_file, "\n%s", text);
            
            /* * CLOSE COMMUNICATIONS FILE AND SPAWN BATCH PLOT PROGRAM * */
            /* ---------------------------------------------------------- */
            
	    status = ho_lock (temp_file, OFF);
            fclose (temp_file);

		/* ming:11/27/91 */
		/* The script name is ld_sgc_batch */
		/* ------------------------------- */
        status = ho_add_path ("ESI$BIN", "ld_sgc_batch",
				  sizeof(FILENAME), temp_filename);
	    if (status != SUCCESS)
	      {
		sprintf(msg,"Error in add path. %s",mg_message(status));
		am_message (AM_ALERT,msg);
		return FAIL;
	      }

            status = ho_spawn (1, temp_filename, arg_list, 0);
	    if (status != SUCCESS)
	      {
		sprintf(msg,"Error when sending batch job. %s",
			mg_message(status));
		am_message(AM_ALERT,msg);
		return FAIL;
	      }
            }
        /* * UNRECOGNIZED EVENT * */
        /* ---------------------- */
        
        else
            {
            wi_beep (2);
            }
        }
    tc_free (select_list);
    
    return status;
    }
/* END:     */
