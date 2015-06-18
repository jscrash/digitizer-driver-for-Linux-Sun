/* DEC/CMS REPLACEMENT HISTORY, Element LDZ_LD_NL_FI.C*/
/* *2    14-AUG-1990 13:31:05 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:02:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LDZ_LD_NL_FI.C*/
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

#include "esi_gl_defs.h"
#include "esi_mg.h"
#include "esi_am.h"
#include "esi_fm.h"
#include "esi_ho.h"
#include "esi_ho_files.h"
#include "esi_ld_s_g_c.h"
#include "esi_ld_err.h"
#include "esi_ld_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to process the selected files and load them into the
    file management system.

Prototype:
    publicdef INT ldz_load_nlist_files(INT data_type, LD_SGC_FILE_STRUCT *file_struct,
                                INT *selected, INT number);
                                
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) data_type   -(INT)
    (I) file_struct -(LD_SGC_FILE_STRUCT *)
    (I) selected    -(INT *)
    (I) number      -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ldz_load_nlist_files (INT data_type, LD_SGC_FILE_STRUCT *file_struct,
                                    INT *selected, INT number) 
#else
publicdef INT ldz_load_nlist_files (data_type, file_struct, selected, number)
INT data_type;
LD_SGC_FILE_STRUCT *file_struct;
INT *selected;
INT number;
#endif

/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT status;
    INT i, j;
    CHAR message[240];
    FM_FILE pathname;
    FM_TYPE fm_type;
    FILENAME temp_name;
    FILENAME os_name;
    BOOL perm = TRUE;
    INT (*term_func)();
    
    /**************************************************************************/
    
    /* * INITIALIZE FOR DATA TYPE * */
    /* ---------------------------- */
    
    switch (data_type)
        {
    case 2:                         /* GRIDS */
        strcpy (pathname, "ESI$GRIDS");
        strcpy (fm_type, "GRID");
        term_func = fm_grid_term_func;
        break;
        
    case 3:                         /* CONTOURS */
        strcpy (pathname, "ESI$CONTOURS");
        strcpy (fm_type, "CONTOUR");
        term_func = fm_contour_term_func;
        break;
        
    case 4:                         /* PERSPECTIVES */
        strcpy (pathname, "ESI$PERSPECTIVES");
        strcpy (fm_type, "PERSPECTIVE");
        term_func = fm_contour_term_func;
        break;
        }
    /* * LOAD LOOP * */
    /* ------------- */
    
    for (i = 0; i < number; i++)
        {
        j = selected[i] - 1;
        sprintf (message, "%s %s", mg_message (LD_SGC_LOADING_FILE),
                 file_struct->file_list[j]);
        am_message (AM_DIALOG, message);
        
        /* * COPY TO OUTPUT FILE * */
        /* ----------------------- */
        
        sprintf (temp_name, "%s%s", file_struct->directory, file_struct->file_list[j]);
        strcpy (message, "");
        if ((status = fm_init_file (pathname, file_struct->file_list[j], fm_type, "w",
                                    perm, message, term_func, os_name,
                                    "LOADER")) EQUALS SUCCESS)
            {
            
            ho_delete_file (os_name, &status);
            sprintf (message, "Copy %s %s", temp_name, os_name);
            ho_syscom (message);
            
            fm_release_file (pathname, file_struct->file_list[j], fm_type);
            
            sprintf (message, "Created by Loading file: %s", temp_name);
            fm_add_history_record (pathname, file_struct->file_list[j], fm_type, "",
                                   "", "", message);
            }
        else
            {
            am_message (AM_DIALOG, mg_message (status));
            }
        }
    am_message (AM_DIALOG, NULL);
    return SUCCESS;
    }
/* END:     */
