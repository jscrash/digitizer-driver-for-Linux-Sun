/* DEC/CMS REPLACEMENT HISTORY, Element LD_S_G_C_SRVR.C*/
/* *8    16-AUG-1991 16:37:07 MING "(SPR 0) add / to the end of the directory path"*/
/* *7    17-DEC-1990 16:26:07 MING "(SPR 6244) bug fix"*/
/* *6     9-NOV-1990 10:33:39 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames"*/
/* *5    23-OCT-1990 13:52:03 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    19-JUN-1990 16:43:31 MING "(SPR 0) undone the former change"*/
/* *3    14-JUN-1990 16:35:26 MING "(SPR 0) ho_find_files2 rtn full path, fix here now, need to be undone when ho_find_files get*/
/*fix"*/
/* *2    26-JUN-1989 10:47:51 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:03:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_S_G_C_SRVR.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_S_G_C_SRVR.C                             */
/* *4    19-JUN-1990 16:43:31 MING "(SPR 0) undone the former change"               */
/* *3    14-JUN-1990 16:35:26 MING "(SPR 0) ho_find_files2 rtn full path, fix here now, need to be undone when ho_find_files get    */
/*fix"                                                                              */
/* *2    26-JUN-1989 10:47:51 GILLESPIE "(SPR 20) Fix castings"                     */
/* *1    19-JUN-1989 13:03:21 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_S_G_C_SRVR.C                             */

/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Exploration Systems, Inc.                                               */
/*          579 Market Street                                                       */
/*          San Francisco, CA  USA 94105                                            */
/*          (415) 974-1600                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */


/* * FILE DESCRIPTION *                                                             */
/* --------------------                                                             */

/******************************************************************************     */
/*                                                                                  */
/*   NAME: LD_S_G_C_SRVR.C                                                          */
/*                                                                                  */
/*   AUTHOR: JOHN CONROY                                                            */
/*                                                                                  */
/*   DESCRIPTION: SERVICE EVENTS FROM THE SCATTER/GRID/CONTOUR DATA                 */
/*      LOADER CUSTOM DIALOG.                                                       */
/*                                                                                  */
/*     Input:       Standard AM interface                                           */
/*     Output:      None                                                            */
/*                                                                                  */
/*     Status:      SUCCESS                                                         */
/*                                                                                  */
/******************************************************************************     */

/* * INCLUDE FILES *                                                                */
/* -----------------                                                                */

#include "esi_gl_defs.h"
#include "esi_gl_project.h"
#include "esi_am.h"
#include "esi_ho.h"
#include "esi_ho_files.h"
#include "esi_wi.h"
#include "esi_ts.h"
#include "esi_tc.h"
#include "esi_ld_s_g_c.h"
#include "ld_s_g_c_data.rh"

/* * FUNCTION TITLE *                                                               */
/* ------------------                                                               */

INT ld_s_g_c_srvr (id, item, type)     /* * ARGUEMENT LIST *                           */
/* ------------------                                                               */

INT id;
INT item;
INT *type;

/* * VARIABLE LIST *                                                                */
/* -----------------                                                                */
    {
    INT status = SUCCESS;
    INT i;
    INT next_id;
    INT selector_id;
    INT scroll_id;
    CHAR *temp;
    FILENAME directory;
    LD_SGC_IDS *rsrc_id;
    LD_SGC_LOAD_STRUCT *sgc_files;
    LD_SGC_FILE_STRUCT *dummy;
    PROJECT_DEFAULT *proj_defs;
    INT j;
    CHAR *pdot;
    /********************************************************************************/
    
    am_open_workspace ("SGC_LOADER_ID", AM_GLOBAL, (VOIDPTR *)&rsrc_id);
    am_open_workspace ("LD_SGC_FILES", AM_GLOBAL, (VOIDPTR *)&sgc_files);
    
    switch (item)
        {
    case LD_SCATTER_DATA:
        *type = 1;
        break;
        
    case LD_GRID_DATA:
        *type = 2;
        break;
        
    case LD_CONTOUR_DATA:
        *type = 3;
        break;
        
    case LD_PERSPEC_DATA:
        *type = 4;
        break;
        
    case LD_QUIT_1:
        *type = 1;
        
        ts_tcp_free (sgc_files->scatter.file_list);
        sgc_files->scatter.file_list = (CHAR **)0;
        
        ts_tcp_free (sgc_files->grid.file_list);
        sgc_files->grid.file_list = (CHAR **)0;
        
        ts_tcp_free (sgc_files->contour.file_list);
        sgc_files->contour.file_list = (CHAR **)0;
        
        am_quit ();
        break;
        
    case LD_OK_1:
        switch (*type)
            {
        case 1:
            if ((status = am_define_resource (AM_DIALOG, "LD_S_G_C_DATA",
                                              LD_SCATTER, ld_scatter_srvr,
                                              &(rsrc_id->scatter),
                                              NULL)) != SUCCESS)
                 {
                 return status;
                 }
            am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL,
                              (VOIDPTR *)&proj_defs);
            wi_set_dialog_text (rsrc_id->scatter, LD_PROJECTION,
                                proj_defs->map.projection);
            wi_set_dialog_text (rsrc_id->scatter, LD_NUM_RECORDS, "2");
            next_id = rsrc_id->scatter;
            selector_id = LD_XYZ_SELECT;
            scroll_id = LD_XYZ_SCROLL;
            dummy = &(sgc_files->scatter);
            break;
            
        case 2:
            if (rsrc_id->grid EQUALS 0)
                {
                if ((status = am_define_resource (AM_DIALOG, "LD_S_G_C_DATA", LD_GRID,
                                                  ld_grid_srvr, &(rsrc_id->grid),
                                                  NULL)) != SUCCESS)
                    {
                    return status;
                    }
                }
            next_id = rsrc_id->grid;
            selector_id = LD_GRID_SELECT;
            scroll_id = LD_GRID_SCROLL;
            dummy = &(sgc_files->grid);
            break;
            
        case 3:
            if (rsrc_id->contour EQUALS 0)
                {
                if ((status = am_define_resource (AM_DIALOG, "LD_S_G_C_DATA",
                                                  LD_CONTOUR, ld_contour_srvr,
                                                  &(rsrc_id->contour),
                                                  NULL)) != SUCCESS)
                    {
                    return status;
                    }
                }
            next_id = rsrc_id->contour;
            selector_id = LD_CONTOUR_SELECT;
            scroll_id = LD_CONTOUR_SCROLL;
            dummy = &(sgc_files->contour);
            break;
            
        case 4:
            if (rsrc_id->perspective EQUALS 0)
                {
                if ((status = am_define_resource (AM_DIALOG, "LD_S_G_C_DATA",
                                                  LD_PERSPECTIVE, ld_perspective_srvr,
                                                  &(rsrc_id->perspective),
                                                  NULL)) != SUCCESS)
                    {
                    return status;
                    }
                }
            next_id = rsrc_id->perspective;
            selector_id = LD_PERSPECTIVE_SELECT;
            scroll_id = LD_PERSPECTIVE_SCROLL;
            dummy = &(sgc_files->perspective);
            break;
            
        default:
            wi_beep (2);
            return SUCCESS;
            }
        /* * GET LIST OF FILES *                                                    */
        /* ---------------------                                                    */
        
        wi_query_dialog_item (id, LD_DIRECTORY, &i, directory);
        ho_translate_filename (directory, dummy->directory);
        /*                                                                          */
        /*  i = strlen(dummy->directory) - 2;                                       */
        /*  dummy->directory[i] = '\0';                                             */
        /*                                                                          */
        i = strlen (dummy->directory);
        temp = (CHAR *) tc_alloc (i + 6);
#ifdef vms
        sprintf (temp, "%s*.*;*", dummy->directory);
#else
	/* ming: 8/16/91 */
	/* concantenate / ro the directory, so users do not */
	/* need to put one themselves                       */
	/* ------------------------------------------------ */
        strcat (dummy->directory,"/");
        sprintf (temp, "%s*.*", dummy->directory);
#endif
        ts_tcp_free (dummy->file_list);
        status = ho_find_filenames (temp, &(dummy->file_list));
	if (status == SUCCESS)
	  {
          tc_free (temp);
	  }
        
        i = ts_tcp_len (dummy->file_list);
        
        wi_set_selections (next_id, selector_id, scroll_id, i, i, dummy->file_list);
        
        /* * OPEN DIALOG *                                                          */
        /* ---------------                                                          */
        
        wi_open_dialog (next_id);
        break;
        }
    return status;
    }
