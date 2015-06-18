/* DEC/CMS REPLACEMENT HISTORY, Element LD_GRID_SRVR.C*/
/* *5    14-AUG-1991 11:57:37 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *4    14-AUG-1990 13:31:37 VINCE "(SPR 5644) Header Standardization"*/
/* *3     6-FEB-1990 10:52:57 MING "(SPR 0) change date/time declaration to new data type DATE_TIME"*/
/* *2    26-JUN-1989 10:46:34 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:02:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_GRID_SRVR.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Service events from the grid file loader custom dialog.
    
Prototype:
    publicdef INT ld_grid_srvr(INT id, INT item, BYTE *p);
    
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
publicdef INT ld_grid_srvr (INT id, INT item, BYTE *p) 
#else
publicdef INT ld_grid_srvr (id, item, p)
INT id;
INT item;
BYTE *p;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT status = SUCCESS;
    INT num_files;
    INT i;
    INT *select_list;
    DATE_TIME date_time;
    CHAR account[31];
    CHAR password[31];
    CHAR database[31];
    CHAR arg_list[512];
    PROJECT_NAME project;
    FILENAME temp_filename;
    FILE *temp_file;
    LD_SGC_LOAD_STRUCT *sgc_files;
    
    /**************************************************************************/
    
    /* * QUIT THE DIALOG * */
    /* ------------------- */
    
    if (item EQUALS LD_QUIT_3)
        {
        wi_close_dialog (id);
        return SUCCESS;
        }
    /* * DO THE LOADING * */
    /* ------------------ */
    
    am_open_workspace ("LD_SGC_FILES", AM_GLOBAL, (VOIDPTR *) & sgc_files);
    num_files = ts_tcp_len (sgc_files->grid.file_list);
    if (num_files <= 0)
        {
        return SUCCESS;
        }
    select_list = (INT *)tc_alloc (num_files * sizeof(INT));
    
    wi_query_selections (id, LD_GRID_SELECT, &num_files, select_list);
    if (num_files > 0)
        {
        
        /* * RUN INTERACTIVE LOADING * */
        /* --------------------------- */
        
        if (item EQUALS LD_RUN_2)
            {
            ldz_load_nlist_files (2, &(sgc_files->grid), select_list, num_files);
            }
        /* * SUBMIT LOADING TO A BATCH PROCESS * */
        /* ------------------------------------- */
        
        else if (item EQUALS LD_SUBMIT_2)
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
            sprintf (temp_filename, "ld_grid_%s.data", date_time);
            
            /* * CONSTRUCT ARGUEMENT LIST FOR BATCH PLOT PROGRAM * */
            /* --------------------------------------------------- */
            
            sprintf (arg_list, "\"%s/%s@%s\",%s,%s", account, password, database,
                     project, temp_filename);
            
            /* * OPEN COMMUNICATIONS FILE * */
            /* ---------------------------- */
            
            ho_add_path ("ESI$REPORTS", temp_filename, sizeof(FILENAME),
                         temp_filename);
            temp_file = ho_open (temp_filename, "w", NULL);
	    status = ho_lock (temp_file, ON);
	    if (status != SUCCESS)
	      return status;

            
            /* * WRITE COMMUNICATIONS INFO * */
            /* ----------------------------- */
            
            fprintf (temp_file, "2");   /* DATA TYPE */
            fprintf (temp_file, "\n%s", sgc_files->grid.directory);
            fprintf (temp_file, "\n%d", num_files);
            for (i = 0; i < num_files; i++)
                {
                fprintf (temp_file, "\n%s",
                         sgc_files->grid.file_list[select_list[i] - 1]);
                }
            /* * CLOSE COMMUNICATIONS FILE AND SPAWN BATCH PLOT PROGRAM * */
            /* ---------------------------------------------------------- */

	    status = ho_lock (temp_file, OFF);                        
            fclose (temp_file);
            
            ho_add_path ("ESI$BIN", "LD_SGC_BATCH", sizeof(FILENAME), temp_filename);
            status = ho_spawn (1, temp_filename, arg_list, 0);
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
