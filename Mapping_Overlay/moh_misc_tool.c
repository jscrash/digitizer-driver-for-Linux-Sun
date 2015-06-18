/* DEC/CMS REPLACEMENT HISTORY, Element MOH_MISC_TOOL.C*/
/* *6    30-AUG-1990 12:39:54 GILLESPIE "(SPR 1) Change esi_ho.h to esi_ho_files.h"*/
/* *5    17-AUG-1990 22:00:22 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     6-NOV-1989 11:25:48 JULIAN "(SPR 30) add mg_message"*/
/* *3     3-NOV-1989 12:32:30 GILLESPIE "(SPR 1) Changes to keep up with mo_start changes"*/
/* *2    26-JUN-1989 10:49:17 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:08:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MOH_MISC_TOOL.C*/
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

#include "esi_c_lib.h"

#ifndef ESI_MO_H
#include "esi_mo.h"
#endif

#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#include "mo_strat.rh"
#include "esi_mg.h"
#include "esi_mo_msg.h"

#define MAXPTS 2000
#define WARNING 1
#define MO_OPEN_DIALOG wi_open_dialog(pModel->main_dialog_id),\
               wivismn(pModel->main_menu_id,ON);
        

#if USE_PROTOTYPES
static INT mo_change_type (STRAT_MODEL_STRUCT *pModel);
static INT mo_change_type_server (INT cdlgid, INT item,
                                      STRAT_MODEL_STRUCT *pModel);
static INT mo_corr_lines (STRAT_MODEL_STRUCT *pModel);
static INT mo_delete_model (STRAT_MODEL_STRUCT *pModel);
publicdef INT mo_shutdown (STRAT_MODEL_STRUCT *pModel);

#else
static INT mo_change_type ();
static INT mo_change_type_server ();
static INT mo_corr_lines ();
static INT mo_delete_model ();
publicdef INT mo_shutdown ();

#endif

/* File Description ---------------------------------------------------------
Overview:
    Host specific miscellaneous tools.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mo_make_pl_filename(FILENAME filename, STRAT_MODEL_STRUCT *pModel);
    
    publicdef CHAR * mo_make_dirname(CHAR *filename,STRAT_MODEL_STRUCT *pModel);
    
    publicdef INT mo_main_menu_server(INT menuid, INT item, STRAT_MODEL_STRUCT *pModel);
    
    publicdef INT mo_main_dialog_server(INT cdlgid, INT item, STRAT_MODEL_STRUCT *pModel);
    
    publicdef INT mo_sort_strokes(NLIST_HEADER *nlist);
    
    publicdef  INT mo_shutdown(STRAT_MODEL_STRUCT *pModel);

Private_Functions:
    static INT mo_change_type (STRAT_MODEL_STRUCT *pModel);

    static INT mo_change_type_server(INT cdlgid,INT item,
                 STRAT_MODEL_STRUCT *pModel);

    static INT mo_corr_lines(STRAT_MODEL_STRUCT *pModel);

    static INT mo_delete_model(STRAT_MODEL_STRUCT *pModel);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Construct the horizon point (N-list) file name,           
    with the supplied logicals.                          

Prototype:
    publicdef INT mo_make_pl_filename(FILENAME filename, STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    Function returns status from ho_add_path.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_make_pl_filename (FILENAME filename, STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_make_pl_filename (filename, pModel)
FILENAME filename;
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT status;
    
    if (pModel->model_type EQUALS WELL_TIE)
        {
        status = ho_add_path ("ESI$WELL_TIE", pModel->name, sizeof(FILENAME),
                              filename);
        }
    else
        {
        status = ho_add_path ("ESI$EMPIRICAL", pModel->name, sizeof(FILENAME),
                              filename);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Construct the directory name for the points list, acc- 
    ording to model type.                                

Prototype:
    publicdef CHAR * mo_make_dirname(CHAR *filename,STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    See code.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *mo_make_dirname (CHAR *filename, STRAT_MODEL_STRUCT *pModel)
#else
publicdef CHAR *mo_make_dirname (filename, pModel)
CHAR *filename;
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    if (pModel->model_type EQUALS WELL_TIE)
#ifdef vms
            {
            return strcpy (filename, "ESI$WELL_TIE");
            }
        else
            {
            return strcpy (filename, "ESI$EMPIRICAL");
            }
#else
        {
        ho_add_path ("ESI$WELL_TIE", "", sizeof(FILENAME), filename);
        filename[strlen (filename) - 1] = '\0';
        return filename;
        }
    else
        {
        ho_add_path ("ESI$EMPIRICAL", "", sizeof(FILENAME), filename);
        filename[strlen (filename) - 1] = '\0';
        return filename;
        }
#endif
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To process main menu selections.                        

Prototype:
    publicdef INT mo_main_menu_server(INT menuid, INT item, STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    menuid          -(INT)
    item            -(INT)
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
publicdef INT mo_main_menu_server (INT menuid, INT item, STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_main_menu_server (menuid, item, pModel)
INT menuid;
INT item;
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT status;
    /* ************************************************************************** */
    
    switch (item)
        {
        
        /* Digitize the model                                                        */
        
    case MO_DIGITIZE:
        wiclcd (pModel->main_dialog_id);
        
        status = mo_digitize_model (pModel);
        if (status EQUALS SUCCESS)
            {
            pModel->need_to_save = TRUE;
            wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
            pModel->mo_tieplot_onoff = OFF;
            wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, OFF);
            pModel->generate_traces_onoff = OFF;
            wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
            pModel->mo_traces_onoff = OFF;
            }
        MO_OPEN_DIALOG;
        break;
        
        /* Change the model type from well tie to empirical or vice-versa            */
        
    case MO_CHANGE_TYPE:
        wiclcd (pModel->main_dialog_id);
        mo_change_type (pModel);
        break;
        
        /* Enter velocity, density data (empirical model)                            */
        
    case MO_DATA_ENTRY:
        wiclcd (pModel->main_dialog_id);
        if (mo_data_entry (pModel) EQUALS SUCCESS)
            {
            pModel->need_to_save = TRUE;
            wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL, ON);
            pModel->generate_model_onoff = ON;
            strcpy (pModel->velocity.id, "VELOCITY");
            }
        break;
        
        /* Generate the model synthetic sonic traces                                 */
        
    case GENERATE_MODEL:
        wiclcd (pModel->main_dialog_id);
        switch (pModel->model_type)
            {
            
        case WELL_TIE:
            status = mog_tie_driver (pModel->model.nlist, pModel, GENERATE_MODEL);
            break;
            
        case PURE_EMPIRICAL:
            status = mog_emp_driver (pModel->model.nlist, pModel->velocity.nlist,
                                     pModel, GENERATE_MODEL);
            break;
            }
        MO_OPEN_DIALOG;
        if (status EQUALS SUCCESS)
            {
            wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, ON);
            pModel->mo_tieplot_onoff = ON;
            wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, ON);
            pModel->generate_traces_onoff = ON;
            wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
            pModel->mo_traces_onoff = OFF;
            }
        else
            {
            wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
            pModel->mo_tieplot_onoff = OFF;
            wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, OFF);
            pModel->generate_traces_onoff = OFF;
            wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
            pModel->mo_traces_onoff = OFF;
            }
        break;
        
        /*Generate a display of the computed model sonic traces and overlay the layers*/
        
    case MO_DRAW_TIEPLOT:
        
        wiclcd (pModel->main_dialog_id);
        switch (pModel->model_type)
            {
            
        case WELL_TIE:
            status = mog_tie_driver (pModel->model.nlist, pModel, MO_DRAW_TIEPLOT);
            break;
        case PURE_EMPIRICAL:
            status = mog_emp_driver (pModel->model.nlist, pModel->velocity.nlist,
                                     pModel, MO_DRAW_TIEPLOT);
            break;
            }
        MO_OPEN_DIALOG;
        break;
        
    case GENERATE_TRACES:
        wiclcd (pModel->main_dialog_id);
        switch (pModel->model_type)
            {
            
        case WELL_TIE:
            status = mog_tie_driver (pModel->model.nlist, pModel, GENERATE_TRACES);
            break;
        case PURE_EMPIRICAL:
            status = mog_emp_driver (pModel->model.nlist, pModel->velocity.nlist,
                                     pModel, GENERATE_TRACES);
            break;
            }
        MO_OPEN_DIALOG;
        
        if (status EQUALS SUCCESS)
            {
            wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, ON);
            pModel->mo_traces_onoff = ON;
            }
        else
            {
            wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
            pModel->mo_traces_onoff = OFF;
            }
        break;
        
    case MO_TRACES:
        
        wiclcd (pModel->main_dialog_id);
        switch (pModel->model_type)
            {
            
        case WELL_TIE:
            status = mog_tie_driver (pModel->model.nlist, pModel, MO_TRACES);
            break;
            
        case PURE_EMPIRICAL:
            status = mog_emp_driver (pModel->model.nlist, pModel->velocity.nlist,
                                     pModel, MO_TRACES);
            break;
            }
        MO_OPEN_DIALOG;
        break;
        
    case MO_QUIT:
        mo_shutdown (pModel);
        mo_free (pModel);
        am_quit ();
        return SUCCESS;
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To process file menu selections (NEW,SAVE,DELETE,OPEN)   
    etc.                                                     

Prototype:
    publicdef INT mo_main_dialog_server(INT cdlgid, INT item, STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    cdlgid          -(INT)
    item            -(INT)
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
publicdef INT mo_main_dialog_server (INT cdlgid, INT item, STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_main_dialog_server (cdlgid, item, pModel)
INT cdlgid;
INT item;
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    
    MO_DATA_STRUCT current;
    INT status;
    INT main_wkid = MAIN_WKID;
    /* ************************************************************************** */
    
    
    switch (item)
        {
        
        /* Retrieve an existing model and draw the sucker                           */
        
    case WI_FILE_MENU_OPEN:
        
        mo_check_save (pModel);
        wiclcd (pModel->main_dialog_id);
        am_message (AM_DIALOG, "Request to open a model file");
        
        status = mo_open_model (pModel);
        
        if (status EQUALS SUCCESS)
            {
            wmwfront (pModel->graphics.window_id);
            wmwopen (pModel->graphics.window_id);
            if (pModel->corr_group != 0)
                {
                wmgvis (pModel->corr_group, GINVIS);
                wm_grsgwk (main_wkid);
                }
            current.point = FALSE;
            wm_gsplci (pModel->line.color);
            wmtselect (pModel->graphics.transform_id);
            
            /* Draw the Layers          */
            status = nl_draw_nlist (pModel->model.nlist, 1, 2);
            mo_mark_layer_ends (pModel, pModel->model.nlist);
            mo_mark_sed_type (pModel, pModel->model.nlist);
            wm_gstxci (pModel->line.color);
            /* If emp. draw velocities  */
            
            if (pModel->model_type == PURE_EMPIRICAL)
                {
                mog_reset_text ();
                mo_draw_vels (pModel);
                }
            wmwclose (pModel->graphics.window_id);
            }
        am_message (AM_DIALOG, NULL);
        MO_OPEN_DIALOG;
        return SUCCESS;
        
        /* Open a new model, initialize according to type WELL TIE or EMPIRICAL       */
        
    case WI_FILE_MENU_NEW:
        
        /* See if existing model not saved      */
        mo_check_save (pModel);
        pModel->need_to_save = FALSE;
        wiclcd (pModel->main_dialog_id);
        /* Set menu on,offs                     */
        wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL, OFF);
        pModel->generate_model_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE, OFF);
        pModel->mo_digitize_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
        pModel->mo_tieplot_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
        pModel->mo_traces_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, OFF);
        pModel->generate_traces_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTRY, OFF);
        pModel->mo_data_entry_onoff = OFF;
        /* Clear the depth, velocity nlists*/
        nl_clear_nlist (pModel->model.nlist);
        nl_clear_nlist (pModel->velocity.nlist);
        /* Check model type and initialize
           accordingly                     */
        if (pModel->model_type == WELL_TIE)
            {
            status = mo_init_well_tie (pModel);
            }
        else
            {
            status = mo_init_empirical (pModel);
            }
        if (status == SUCCESS)
            {
            /* Draw existing layers if present */
            mo_corr_lines (pModel);
            strcpy (pModel->name, "");
            
            /* Digitize the model, then turn on apprpriate
               menu items depending on type   */
            if (mo_digitize_model (pModel) EQUALS SUCCESS)
                {
                wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE, ON);
                pModel->mo_digitize_onoff = ON;
                pModel->need_to_save = TRUE;
                if (pModel->model_type EQUALS PURE_EMPIRICAL)
                    {
                    wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTRY, ON);
                    pModel->mo_data_entry_onoff = ON;
                    }
                else
                    {
                    wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL, ON);
                    pModel->generate_model_onoff = ON;
                    }
                }
            }
        MO_OPEN_DIALOG;
        return SUCCESS;
        
        /* delete a  model file */
        
    case WI_FILE_MENU_DELETE:
        wiclcd (pModel->main_dialog_id);
        am_message (AM_DIALOG, mg_message (MO_DELETE_MODEL));
        mo_delete_model (pModel);
        
        am_message (AM_DIALOG, NULL);
        MO_OPEN_DIALOG;
        return SUCCESS;
        
        /* Save the model                                                             */
        
    case WI_FILE_MENU_SAVE:
        
        mo_save_model (pModel);
        return SUCCESS;
        
    case WI_FILE_MENU_QUIT:
        return SUCCESS;
        }
    return FAIL;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To close out modelling, free up structures, restore the   
    x-section , etc.                                      

Prototype:
    publicdef INT mo_shutdown(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_shutdown (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_shutdown (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT main_wkid = MAIN_WKID;
    CHAR text[80];
    
    /* %S Free up anything that needs freeing up */
    
    /* see if a save is required */
    
    mo_check_save (pModel);
    
    /* restore cross section to its original state */
    
    wmwopen (pModel->graphics.window_id);
    wmsdelete (pModel->grat_segment_id);
    wmgvis (pModel->corr_group, GVISI);
    qs_inq_c (QS_WORKSTATION_NAME, text, 0);    /* test for maconly */
    if (text[0] != NUL)
        {
        wm_grsgwk (main_wkid);
        }
    wmwclose (pModel->graphics.window_id);
    
    strcpy (pModel->name, "");
    pModel->need_to_save = FALSE;
    mog_emp_driver (pModel->model.nlist, pModel->velocity.nlist, pModel,
                    MO_F77_SHUTDOWN);
    mog_tie_driver (pModel->model.nlist, pModel, MO_F77_SHUTDOWN);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To sort the layer strokes in ascending x order, since     
    they are digitized in both directions.              

Prototype:
    publicdef INT mo_sort_strokes(NLIST_HEADER *nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_sort_strokes (NLIST_HEADER *nlist)
#else
publicdef INT mo_sort_strokes (nlist)
NLIST_HEADER *nlist;
#endif
    {
    UINT npoints;
    INT status;
    INT i;
    NLIST_HEADER sorts;
    UINT dimlist[1];
    INT sortflags[1];
    /* ************************************************************************** */
    
    nl_inq_nlist_int (*nlist, NL_NLIST_NPOINTS, (INT *) & npoints);
    
    if (npoints EQUALS 0)
        return FAIL;
    
    dimlist[0] = 1, sortflags[0] = NL_ASCENDING;
    nl_sort_nlist (*nlist, 1, dimlist, sortflags, &sorts);
    nl_free_nlist (*nlist);
    *nlist = sorts;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To select and delete an existing model point list file.  

Prototype:
    static INT mo_delete_model(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <moh_misc_tool.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_delete_model (STRAT_MODEL_STRUCT *pModel)
#else
static INT mo_delete_model (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    FILENAME filename;
    CHAR savename[STMAX];
    CHAR text[STMAX];
    INT status;
    INT ifyes;
    /* *************************************************************************** */
    
    /* select model */
    
    status = mo_select_model (pModel, filename);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return FAIL;
    
    /* save current model name in temp buf */
    
    strcpy (savename, pModel->name);
    
    /* build delete file name */
    
    strcpy (pModel->name, filename);
    mo_make_pl_filename (filename, pModel);
#ifdef vms
    strcat (filename, ".");
#endif
    
    /* restore current model name */
    strcpy (pModel->name, savename);
    
    /* verify delete */
    
    sprintf (text, "OK to delete file %s ?", filename);
    wiyesno (text, "MODEL/DELETE", &ifyes, &status);
    
    if (ifyes)
        {
        ho_delete_file (filename, &status);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            sprintf (text, "Unable to delete file %s at this time.", filename);
            wialert (WARNING, "MODEL/DELETE", text, mg_message (MO_NO_PRIVILIDGE));
            return FAIL;
            }
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine checks to see if tops were displayed on the 
    x-section and if the user wishes to use the correlation   
    lines in the model. If so the N-list of lines is retrie-  
    ved and points are checked to be in range.                

Prototype:
    static INT mo_corr_lines(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <moh_misc_tool.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_corr_lines (STRAT_MODEL_STRUCT *pModel)
#else
static INT mo_corr_lines (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT yes, status;
    INT i;
    UINT npoints;
    UINT nstrokes, nstroke_pts;
    UINT stroke, point;
    XS_STRUCT *pxsec;
    LAYOUT_STRUCT *p_layout;
    SED_DATA_STRUCT sed_struct;
    NLIST_HEADER nl;
    VOIDPTR value_list[2];
    VOIDPTR value_list2[2];
    UINT dim_list[2];
    UINT cstroke, cpoint, nrecv;
    FLOAT x, y, px, py;
    FLOAT new_nullx, new_nully;
    FLOAT xscale_min, yscale_min;
    FLOAT x_offset;
    
    /* ************************************************************************** */
    
    /* open workspace and retrieve XS structure                                   */
    
    status = am_open_workspace (XS_WORKSPACE, AM_GLOBAL, (VOIDPTR *) & pxsec);
    
    if (status != SUCCESS)          /* No x-section, return          */
        {
        return SUCCESS;
        }
    /* Check if N-list of layers in xsec struct is NULL                           */
    
    if (pxsec->layers EQUALS (NLIST_HEADER)0)
        return SUCCESS;
    
    /* Check N-list from xsec struct has npoints > 0                              */
    
    status = nl_inq_nlist_int (pxsec->layers, NL_NLIST_NPOINTS, (INT *) & npoints);
    if (npoints < 1)
        return SUCCESS;
    
    /* Has points, make local copy so we don't screw up the original              */
    
    status = nl_copy_nlist (pxsec->layers, &nl);
    
    /* ask if user wishes to use lines */
    
    wiyesno (mg_message (MO_CORR_LINES), "MODEL/DIGITIZE", &yes, &status);
    if (NOT yes)
        return SUCCESS;
    
    /* reset the NULL value in the nlist to avoid problems with drawing tops      */
    
    new_nullx = -999999.99;
    new_nully = 999999.99;
    dim_list[0] = 1, value_list[0] = (VOIDPTR) & new_nullx;
    dim_list[1] = 2, value_list[1] = (VOIDPTR) & new_nully;
    
    status = nl_set_nlist_info (nl, NL_NLIST_NULL_VALUE, 2, dim_list, value_list);
    
    /* get x offset since we get points relative to left edge of layout-need points
       relative to center of layout so divide layout width by 2                   */
    
    status = llfirst (&pxsec->drawing.log_plots, &p_layout);
    x_offset = p_layout->width / 2.0;
    
    /* get wcs dimensions so we can scale x,y input values (which are in xs units)*/
    
    xscale_min = pModel->graphics.world.xmin;
    yscale_min = pModel->graphics.world.ymin;
    
    /* loop through nlist strokes and delete any that have no points in them
     (as in the case where a top was not displayed because out of y limits)       */
    
    status = nl_inq_nlist_int (nl, NL_NLIST_NSTROKES, (INT *) & nstrokes);
    
    for (i = 0; i < nstrokes; i++)
        {
        status = nl_inq_stroke_int (nl, i + 1, NL_STROKE_NPOINTS,
                                    (INT *) & nstroke_pts);
        
        if (nstroke_pts == 0)
            {
            status = nl_delete_stroke (nl, i + 1);
            --nstrokes;
            --i;
            }
        else
            {
            sed_struct.sed_type = 1;    /*init. sediment type to normal */
            sed_struct.x = 0.0;
            sed_struct.y = 0.0;
            status = nl_set_stroke_user_struct (nl, i + 1, &sed_struct,
                                                sizeof(SED_DATA_STRUCT));
            }
        }
    /* loop through nlist points, and change x,y values to correct coordinates    */
    
    value_list[0] = (VOIDPTR) & x;
    value_list[1] = (VOIDPTR) & y;
    
    for (i = 0; i < npoints; i++)
        {
        status = (i == 0) ? nl_first_point (nl, 2, dim_list,
                                            value_list) :
                 nl_next_point (nl, 2, dim_list, value_list);
        
        x = xscale_min + (x + x_offset) / pModel->xscale;
        y = yscale_min + (y - pxsec->drawing.yorigin) / pModel->yscale;
        
        status = nl_replace_points (nl, 1, 2, dim_list, value_list);
        }
    /* delete any points out of range */
    
    status = nl_inq_nlist_int (nl, NL_NLIST_NSTROKES, &i);
    
    if (i > 0)
        {
        value_list2[0] = (VOIDPTR) & px;
        value_list2[1] = (VOIDPTR) & py;
        status = nl_last_point (nl, 2, dim_list, value_list);
        status = nl_inq_current_point (nl, &cpoint, &cstroke);
        status = nl_inq_nlist_int (nl, NL_NLIST_NPOINTS, (INT *) & npoints);
        }
    else
        {
        npoints = 0;
        }
    for (i = npoints; i > 0; i--)
        {
        status = nl_set_current_point (nl, cpoint, cstroke);
        status = nl_get_points (nl, 1, 2, dim_list, value_list, &nrecv);
        if (i != 1)
            {
            nl_previous_point (nl, 2, dim_list, value_list);
            nl_inq_current_point (nl, &cpoint, &cstroke);
            nl_next_point (nl, 2, dim_list, value_list);
            }
        if (y > pModel->log_start - MINFROMTOP)
            {
            nl_delete_points (nl, 1);
            continue;
            }
        if (x < pModel->well.xmin - 0.01)
            {
            nl_inq_current_point (nl, &point, &stroke);
            nl_next_point (nl, 2, dim_list, value_list2);
            
            if (px > pModel->well.xmin + 0.01)
                {
                x = pModel->well.xmin;
                nl_set_current_point (nl, point, stroke);
                nl_replace_points (nl, 1, 2, dim_list, value_list);
                }
            else
                {
                nl_set_current_point (nl, point, stroke);
                nl_delete_points (nl, 1);
                }
            }
        if (x > pModel->well.xmax + 0.01)
            {
            nl_inq_current_point (nl, &point, &stroke);
            nl_previous_point (nl, 2, dim_list, value_list2);
            
            if (px < pModel->well.xmax - 0.01)
                {
                x = pModel->well.xmax;
                nl_set_current_point (nl, point, stroke);
                nl_replace_points (nl, 1, 2, dim_list, value_list);
                }
            else
                {
                nl_set_current_point (nl, point, stroke);
                nl_delete_points (nl, 1);
                }
            }
        }
    /* copy nlist to model structure */
    pModel->model.nlist = nl;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Change model type from well_tie to pure_emp.

Prototype:
    static INT mo_change_type_server(INT cdlgid,INT item,STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    cdlgid          -(INT)
    item            -(INT)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <moh_misc_tool.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_change_type_server (INT cdlgid, INT item, STRAT_MODEL_STRUCT *pModel)
#else
static INT mo_change_type_server (cdlgid, item, pModel)
INT cdlgid;
INT item;
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT status;
    static INT state = -999;
    
    if (state EQUALS - 999)
        state = pModel->model_type;
    switch (item)
        {
        
        /* set users choice in state and update structure only when hit OK */
        
    case MO_CT_WELL_TIE:
        state = WELL_TIE;
        break;
        
    case MO_CT_PURE_EMP:
        state = PURE_EMPIRICAL;
        break;
        
        /* cancel - restore menus picks to the way they were */
        
    case MO_CT_CANCEL:              /* WAS WICANCEL - NEED TO FIX... */
        state = -999;
        am_release_resource (cdlgid);
        MO_OPEN_DIALOG;
        wi_enable_menu_item (pModel->main_menu_id, MO_CHANGE_TYPE, ON);
        wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTRY,
                             pModel->mo_data_entry_onoff);
        wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL,
                             pModel->generate_model_onoff);
        wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE,
                             pModel->mo_digitize_onoff);
        wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT,
                             pModel->mo_tieplot_onoff);
        wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, pModel->mo_traces_onoff);
        wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES,
                             pModel->generate_traces_onoff);
        return SUCCESS;
        
        /* new type */
    case MO_CT_OK:
        
        /* if state is the same, just restore menus picks and get out */
        
        if (state EQUALS pModel->model_type)
            {
            am_release_resource (cdlgid);
            MO_OPEN_DIALOG;
            wi_enable_menu_item (pModel->main_menu_id, MO_CHANGE_TYPE, ON);
            wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTRY,
                                 pModel->mo_data_entry_onoff);
            wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL,
                                 pModel->generate_model_onoff);
            wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE,
                                 pModel->mo_digitize_onoff);
            wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT,
                                 pModel->mo_tieplot_onoff);
            wi_enable_menu_item (pModel->main_menu_id, MO_TRACES,
                                 pModel->mo_traces_onoff);
            wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES,
                                 pModel->generate_traces_onoff);
            return SUCCESS;
            }
        /* new type, see if a save is necessary */
        
        am_release_resource (cdlgid);
        mo_check_save (pModel);
        pModel->need_to_save = FALSE;
        
        /* update type */
        
        pModel->model_type = state;
        
        /* regroup for new type */
        
        if (pModel->model_type EQUALS WELL_TIE)
            {
            
            wi_set_dialog_text (pModel->main_dialog_id, MO_TEXT,
                                mg_message (MO_MODEL_WELL_TIE));
            nl_clear_nlist (pModel->velocity.nlist);
            strcpy (pModel->velocity.id, "");
            }
        else
            {
            /* Change to empirical - delete graticule segment for well tie just in case   */
            
            status = wmwopen (pModel->graphics.window_id);
            status = wmsdelete (pModel->grat_segment_id);
            status = wmwclose (pModel->graphics.window_id);
            
            wi_set_dialog_text (pModel->main_dialog_id, MO_TEXT,
                                mg_message (MO_MODEL_EMPIRICAL));
            strcpy (pModel->velocity.id, "VELOCITY");
            }
        /* set appropirate menu picks */
        
        state = -999;
        wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL, OFF);
        pModel->generate_model_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTRY, OFF);
        pModel->mo_data_entry_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE, OFF);
        pModel->mo_digitize_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
        pModel->mo_tieplot_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, OFF);
        pModel->generate_traces_onoff = OFF;
        wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
        pModel->mo_traces_onoff = OFF;
        MO_OPEN_DIALOG;
        wi_enable_menu_item (pModel->main_menu_id, MO_CHANGE_TYPE, ON);
        break;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Setup dialog box to change type.

Prototype:
    static INT mo_change_type(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <moh_misc_tool.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mo_change_type (STRAT_MODEL_STRUCT *pModel)
#else
static INT mo_change_type (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT cdlgid;
    
    /* turn off all other menu picks that could be dangerous if you change type
       while doing something else */
    
    wi_enable_menu_item (pModel->main_menu_id, MO_DATA_ENTRY, OFF);
    wi_enable_menu_item (pModel->main_menu_id, GENERATE_MODEL, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_CHANGE_TYPE, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_DIGITIZE, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_DRAW_TIEPLOT, OFF);
    wi_enable_menu_item (pModel->main_menu_id, GENERATE_TRACES, OFF);
    wi_enable_menu_item (pModel->main_menu_id, MO_TRACES, OFF);
    
    /* open and define box */
    
    am_define_resource (AM_DIALOG, "mo_strat", MO_SELECT_TYPE, mo_change_type_server,
                        &cdlgid, pModel);
    wi_open_dialog (cdlgid);
    if (pModel->model_type EQUALS WELL_TIE)
        {
        wi_set_dialog_value (cdlgid, MO_CT_WELL_TIE, ON);
        }
    else
        {
        wi_set_dialog_value (cdlgid, MO_CT_PURE_EMP, ON);
        }
    }
/* END:     */
