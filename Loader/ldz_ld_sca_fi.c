/* DEC/CMS REPLACEMENT HISTORY, Element LDZ_LD_SCA_FI.C*/
/* *7    19-AUG-1991 19:27:14 JULIAN "(SPR 3671) changed sscanf format to be able to read float numbers"*/
/* *6    14-AUG-1991 11:57:28 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *5    29-JUL-1991 16:13:24 JESSIE "(SPR 8644) adding / between dir and file name for UNIX case#3315"*/
/* *4     6-JUN-1991 17:25:19 MING "(SPR 0) fix message from thank your to thank you"*/
/* *3    17-DEC-1990 16:25:16 MING "(SPR 6244) bug fix "*/
/* *2    14-AUG-1990 13:31:12 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:02:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LDZ_LD_SCA_FI.C*/
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
#include "esi_mg.h"
#include "esi_ts.h"
#include "esi_tc.h"
#include "esi_am.h"
#include "esi_fm.h"
#include "esi_ov.h"
#include "esi_qs.h"
#include "esi_ho_files.h"
#include "esi_ld_s_g_c.h"
#include "esi_ld_err.h"
#include "esi_ld_msg.h"
#include "esi_co_err.h"

#define NUM_POINTS 256

/* Function Description -----------------------------------------------------
Description:
    Routine to process the selected files and load them into the
    file management system.

Prototype:
    publicdef INT ldz_load_scatter_files(LD_SGC_FILE_STRUCT *xyz_struct, INT *selected,
                        INT number, INT skip, CHAR *projection);
                        
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xyz_struct      -(LD_SGC_FILE_STRUCT *)
    selected        -(INT *)
    number          -(INT)
    skip            -(INT)
    projection      -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ldz_load_scatter_files (LD_SGC_FILE_STRUCT *xyz_struct, INT *selected,
                                      INT number, INT skip, CHAR *projection) 
#else
publicdef INT ldz_load_scatter_files (xyz_struct, selected, number, skip, projection)
LD_SGC_FILE_STRUCT *xyz_struct;
INT *selected;
INT number;
INT skip;
CHAR *projection;
#endif

/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT status = SUCCESS;
    INT i, j;
    INT num;
    CHAR map_proj[20];
    CHAR message[240];
    FILENAME os_name;
    FILENAME temp_name;
    FLOAT x[NUM_POINTS];
    FLOAT y[NUM_POINTS];
    FLOAT z[NUM_POINTS];
    NLIST_HEADER scatter;
    FILE *file_ptr;
    INT dummy;
    INT batch;
    INT message_type;
    INT alert_type;
    OV_SCATTER_USER user_struct;
    CHAR line[256];	
    INT line_no;	
    
    /**************************************************************************/
    
    /* * GET CURRENT SETTING FOR MAP PROJECTION * */
    /* ------------------------------------------ */
    
    qs_inq_c (QS_MAP_PROJECTION, map_proj, 0);
    
    /* * SET MAP PROJECTION TO SUPPLIED PROJECTION FOR LOAD LOOP * */
    /* ----------------------------------------------------------- */
    
    qs_set_c (QS_MAP_PROJECTION, projection, 0);
    
    /* * INITIALIZE USER STRUCTURE * */
    /* ----------------------------- */
    
    strcpy (user_struct.projection, projection);
    strcpy (user_struct.top_surface, "");
    strcpy (user_struct.base_surface, "");
    
    /* * SET UP FOR PROPER MESSAGE HANDLING * */
    /* -------------------------------------- */
    
    qs_inq_i (QS_IF_BATCH, &batch);
    message_type = (batch) ? AM_STATUS : AM_DIALOG;
    alert_type = (batch) ? AM_STATUS : AM_ALERT;				    
    
    /* * LOAD LOOP * */
    /* ------------- */
    
    for (i = 0; i < number; i++)
        {
        num = selected[i] - 1;
        sprintf (message, "%s %s", mg_message (LD_SGC_LOADING_FILE),
                 xyz_struct->file_list[num]);
        am_message (message_type, message);
       
#if UNIX
        sprintf (temp_name, "%s/%s", xyz_struct->directory, xyz_struct->file_list[num]);
#endif
#ifdef vms
        sprintf (temp_name, "%s%s", xyz_struct->directory, xyz_struct->file_list[num]);
#endif

        file_ptr = ho_open (temp_name, "r", NULL);
        if (file_ptr EQUALS (FILE *)0)
            {
            am_message (message_type, mg_message (LD_SGC_OPEN_ERROR));
            continue;
            }
        if ((status = ov_open_scatter (&scatter, xyz_struct->file_list[num], "w",
                                       os_name)) != SUCCESS)
            {
            am_message (message_type, mg_message (status));
            goto close_file;
            }
        /* * SKIP RECORDS * */
        /* ---------------- */
        
        for (j = 0; j < skip; j++)
            {
            while ((dummy = fgetc (file_ptr)) != '\n' AND dummy != EOF)
                ;
            if (dummy EQUALS EOF)
                {
                am_message (message_type, mg_message (CO_NO_SCATTER_DATA));
                goto close_file;
                }
            }
        /* * READ RECORDS FROM INPUT FILE AND WRITE TO OUTPUT * */
        /* ---------------------------------------------------- */
        
        line_no = skip;
        FOREVER
            {
            j = 0;
            while (j < NUM_POINTS)
                {
                line_no ++;
                if (fgets (line,256,file_ptr) EQUALS NULL)
                    {
                    break;
                    }
                if ((status = sscanf (line, "%f %f %f\n", &x[j], &y[j], &z[j])) != 3)
                    {
sprintf (message,"Please note! Invalid data on line %d in the data file %s.Each line should be of the format 'X Y Z' seperated by space. Please check the rest of the file. Loading stop.",line_no,
xyz_struct->file_list[num]);
                    am_message (alert_type,message);
                    break;
                    }
                j++;
                }
            ov_write_scatter (scatter, x, y, z, j);
            if (j != NUM_POINTS)
                {
                break;
                }
            }
        /* * CLOSE UP AND GET NEXT * */
        /* ------------------------- */
        
        nl_set_nlist_user_struct (scatter, &user_struct, sizeof(OV_SCATTER_USER));
        if ((status = ov_close_scatter (scatter, xyz_struct->file_list[num], "w",
                                        os_name)) != SUCCESS)
            {
            am_message (message_type, mg_message (status));
            }
        else
            {
            sprintf (message, "Created by Loading file: %s", temp_name);
            fm_add_history_record ("ESI$SCATTERS", xyz_struct->file_list[num],
                                   "SCATTER", "", "", "", message);
            }
    close_file:
	status = ho_lock (file_ptr, OFF);
        fclose (file_ptr);
        }
    if (NOT batch)
        {
        am_message (message_type, NULL);
        }
    /* * RESTORE SETTING FOR MAP PROJECTION * */
    /* -------------------------------------- */
    
    qs_set_c (QS_MAP_PROJECTION, map_proj, 0);
/*
    am_message (alert_type,"The scattered data loading has finished. Thank you for waiting!");
 */   
    return SUCCESS;
    }
/* END:     */
