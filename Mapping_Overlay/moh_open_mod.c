/* DEC/CMS REPLACEMENT HISTORY, Element MOH_OPEN_MOD.C*/
/* *7     9-NOV-1990 10:33:45 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames"*/
/* *6    17-AUG-1990 22:00:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *5     6-NOV-1989 11:25:59 JULIAN "(SPR 30) add mg_message"*/
/* *4     3-NOV-1989 14:40:40 GILLESPIE "(SPR 1) Changes to keep in sync with mo_strat resource"*/
/* *3    22-SEP-1989 16:29:55 GILLESPIE "(SPR 101) Remove ts_cpystr call - replace with sprintf"*/
/* *2    20-JUL-1989 07:37:39 GORDON "(SPR 100) From Sun port - use ho_find_files, not ho_find_files2"*/
/* *1    19-JUN-1989 13:09:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MOH_OPEN_MOD.C*/
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

#include "esi_mo.h"
#include "mo_strat.rh"
#include "esi_am.h"
#include "esi_c_lib.h"
#include "esi_ho.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "esi_xs_defs.h"
#include "esi_mg.h"
#include "esi_mo_msg.h"

#define WARNING 1

/* File Description ---------------------------------------------------------
Overview:
    See below.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mo_open_model(STRAT_MODEL_STRUCT *pModel);
    
    publicdef INT mo_select_model(STRAT_MODEL_STRUCT *pModel,CHAR *dir);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    To select and open an existing model file, read the       
    layer point list, check the model limits in x,y, etc.     
    If we have a Well Tie model, we must check that the re-   
    called model x,y limits match those of the current win-   
    dow or we are screwed.  If recalling an empirical model   
    it doesn't matter, we can reset the window.              

Prototype:
    publicdef INT mo_open_model(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_open_model (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_open_model (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    FILENAME filename;
    INT status, yes;
    NLIST_HEADER nl;
    FLOAT model_xmin, model_xmax;   /* Model dimension Limits        */
    FLOAT model_ymin, model_ymax;
    FLOAT x_border, y_border;
    UINT Dim_List[2];               /* Nlist Crap                    */
    VOIDPTR Value_List[2];
    size_t nbytes;
    
    /* ************************************************************************** */
    
    /* select model */
    
    status = mo_select_model (pModel, filename);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return FAIL;
    
    /* turn off all menu picks not valid because of new menu */
    
    wienmni (pModel->main_menu_id, GENERATE_MODEL, OFF);
    pModel->generate_model_onoff = OFF;
    wienmni (pModel->main_menu_id, MO_DIGITIZE, OFF);
    pModel->mo_digitize_onoff = OFF;
    wienmni (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
    pModel->mo_tieplot_onoff = OFF;
    wienmni (pModel->main_menu_id, MO_TRACES, OFF);
    pModel->mo_traces_onoff = OFF;
    wienmni (pModel->main_menu_id, GENERATE_TRACES, OFF);
    pModel->generate_traces_onoff = OFF;
    wienmni (pModel->main_menu_id, MO_DATA_ENTER, OFF);
    
    /* make model file name */
    
    strcpy (pModel->name, filename);
    mo_make_pl_filename (filename, pModel);
    strcpy (pModel->model.id, "MODEL");
    
    /* read layer point list */
    
    status = nl_read_nlist (&nl, filename, pModel->model.id);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        wialert (1, "MODEL/OPEN", mg_message (MO_READ_NLIST), filename);
        return FAIL;
        }
    /* Get Model N-list user structure and reset window coords,etc.               */
    
    status = nl_inq_nlist_user_struct (nl, sizeof(pModel->nl_user_struct),
                                       (VOIDPTR) & pModel->nl_user_struct, &nbytes);
    
    /* If Well Tie model, check limits of retrieved model against existing coordin-
       ates, if they are different we can't use it                               */
    
    if (pModel->model_type == WELL_TIE)
        {
        status = mo_init_well_tie (pModel);
        
        if (pModel->nl_user_struct.model_xmin < pModel->well.xmin ||
            pModel->nl_user_struct.model_xmax > pModel->well.xmax ||
            pModel->nl_user_struct.model_ymax > (FLOAT)pModel->log_start)
            {
            am_message (AM_ALERT, mg_message (MO_MODEL_DIMENSION));
            return FAIL;
            }
        }
    if (pModel->model_type == PURE_EMPIRICAL)
        {
        /* Recalling Empirical Model, reset mins,max's */
        /* graphics wcs window coordinates             */
        
        pModel->graphics.world.xmin = pModel->nl_user_struct.wcs_xmin;
        pModel->graphics.world.xmax = pModel->nl_user_struct.wcs_xmax;
        pModel->graphics.world.ymin = pModel->nl_user_struct.wcs_ymin;
        pModel->graphics.world.ymax = pModel->nl_user_struct.wcs_ymax;
        
        /* wcs coordinates of corners of model          */
        
        pModel->well.xmin = pModel->nl_user_struct.model_xmin;
        pModel->well.xmax = pModel->nl_user_struct.model_xmax;
        pModel->log_start = pModel->nl_user_struct.model_ymax;
        
        /* Initialize the empirical model*/
        status = mo_init_empirical (pModel);
        if (status == FAIL)
            return FAIL;
        }
    nl_free_nlist (pModel->model.nlist);
    pModel->model.nlist = nl;
    
    /* read velocity and density n-Lists if PURE EMPIRICAL */
    
    if (pModel->model_type EQUALS PURE_EMPIRICAL)
        {
        strcpy (pModel->velocity.id, "VELOCITY");
        status = nl_read_nlist (&nl, filename, pModel->velocity.id);
        wienmni (pModel->main_menu_id, MO_DATA_ENTRY, ON);
        pModel->mo_data_entry_onoff = ON;
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            wienmni (pModel->main_menu_id, GENERATE_MODEL, OFF);
            pModel->generate_model_onoff = OFF;
            }
        else
            {
            nl_free_nlist (pModel->velocity.nlist);
            pModel->velocity.nlist = nl;
            wienmni (pModel->main_menu_id, GENERATE_MODEL, ON);
            pModel->generate_model_onoff = ON;
            }
        }
    else
        {
        wienmni (pModel->main_menu_id, GENERATE_MODEL, ON);
        pModel->generate_model_onoff = ON;
        }
    wienmni (pModel->main_menu_id, MO_DIGITIZE, ON);
    pModel->mo_digitize_onoff = ON;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine brings up a dialog with a select list of     
    model files.  The user is queried for a model file sel-   
    ection for recall or deletion purposes.                  

Prototype:
    publicdef INT mo_select_model(STRAT_MODEL_STRUCT *pModel,CHAR *dir);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)
    dir             -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_select_model (STRAT_MODEL_STRUCT *pModel, CHAR *dir)
#else
publicdef INT mo_select_model (pModel, dir)
STRAT_MODEL_STRUCT *pModel;
CHAR *dir;
#endif
    {
    CHAR *p;
    CHAR **files;
    INT numfiles, sel, numsel;
    INT status;
    CHAR lbfm_dir[256];
    INT i;
    
    /* construct directory description*/
    mo_make_dirname (dir, pModel);
#ifdef vms
    sprintf (lbfm_dir, "%s:*.*;", dir);
#endif

#ifdef primos
    sprintf (lbfm_dir, "%s>@@", dir);
#endif

#if UNIX
    sprintf (lbfm_dir, "%s/*", dir);
#endif
    /* retrieve filenames from dir.   */
    status = ho_find_pathnames (lbfm_dir, &files);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        wialert (WARNING, "MODEL/SELECT", mg_message (MO_READ_MODEL_DIR), dir);
        return FAIL;
        }
    numfiles = ts_tcp_len (files);
    
    if (numfiles EQUALS 0)
        {
        wialert (WARNING, "MODEL/SELECT", mg_message (MO_NO_MODEL), dir);
        return FAIL;
        }
    /* if vax, get rid of version numbers, not supported */

#ifdef vms
    for (i = 0; i < numfiles; i++)
        {
        p = strrchr (files[i], '.');
        if (p != (CHAR *)0)
            {
            *p = '\0';
            }
        }
#endif
    ts_tcp_sort (files);
    
    /* pick from stock selector */
    
    wissst ("MODEL/SELECT", numfiles, 1, files);
    wirqss ("Select a model", &numsel, &sel, &status);
    
    /* act on response */
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        ts_tcp_free (files);
        return FAIL;
        }
    if (numsel IS_NOT_EQUAL_TO 1)
        {
        ts_tcp_free (files);
        return FAIL;
        }
    strcpy (dir, files[sel - 1]);
    
    ts_tcp_free (files);
    return SUCCESS;
    }
/* END:     */
