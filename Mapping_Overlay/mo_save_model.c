/* DEC/CMS REPLACEMENT HISTORY, Element MO_SAVE_MODEL.C*/
/* *5    17-AUG-1990 22:02:18 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     6-NOV-1989 11:26:40 JULIAN "(SPR 30) add mg_message"*/
/* *3    29-JUL-1989 16:42:56 GILLESPIE "(SPR 1) OOPS - IS_EMPTY__STRING SHOULD HAVE BEEN IS_EMPTY_STRING"*/
/* *2    28-JUL-1989 11:35:45 GILLESPIE "(SPR 1) fix some minor casting problems"*/
/* *1    19-JUN-1989 13:09:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_SAVE_MODEL.C*/
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
#include "esi_mo.h"

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_HO_ERR_H

#include "esi_ho_err.h"

#endif

#include "esi_mg.h"
#include "esi_mo_msg.h"

#define WARNING (INT)1

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mo_save_model(STRAT_MODEL_STRUCT *pModel);
    
    publicdef INT mo_check_save(STRAT_MODEL_STRUCT *pModel);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    To save a model N-list file.

Prototype:
    publicdef INT mo_save_model(STRAT_MODEL_STRUCT *pModel);

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
publicdef INT mo_save_model (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_save_model (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    CHAR text[STMAX];
    FILENAME filename;
    CHAR *p;
    INT status;
    BOOL filename_is_bad = TRUE;
    BOOL ifyes;
    INT file_status;
    UINT npts;
    /* ************************************************************************** */
    
    /* must have some points */
    
    nl_inq_nlist_int (pModel->model.nlist, NL_NLIST_NPOINTS, (INT *) & npts);
    
    if (npts EQUALS 0)
        {
        wialert (WARNING, "MODEL/SAVE", mg_message (MO_NOPOINT_MODEL),
                 mg_message (MO_DIGIT_NEWMODEL));
        return FAIL;
        }
    while (filename_is_bad)
        {
        if (IS_EMPTY_STRING (pModel->name)) /* is a name required? */
            {
            
            /* get file name from user and clean it up and verify it */
            
            wirqst (mg_message (MO_ENTER_FNAME), " ", "STRAT_MODEL", STMAX, text,
                    &status);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                pModel->need_to_save = FALSE;
                return FAIL;
                }
            p = ts_snowhite (ts_sto_upper (text));
            if (IS_EMPTY_STRING (p))
                return FAIL;
            
            /* check to see if file name is ok                                          */
            
            status = ho_chk_fname (p, strlen (p));
            if (status EQUALS HO_BAD_FILENAME)
                {                   /* file name is illegal */
                sprintf (text, "%s is an illegal file name.", p);
                wialert (WARNING, "MODEL/SAVE", text, mg_message (MO_ENTER_VALFNAME));
                continue;
                }
            strcpy (pModel->name, p);
            strcpy (pModel->model.id, "MODEL");
            }
        /* construct full file spec.     */
        mo_make_pl_filename (filename, pModel);
        /* check if file exists or not   */
        file_status = hoexists (filename);
        
        if (file_status EQUALS 1)    /* file already exists */
            {
            sprintf (text, "%s already exists. Do you wish to overwrite it?",
                     filename);
            wiyesno (text, "MODEL/SAVE", &ifyes, &status);
            
            /* delete old file so we can make a new one */
            
            if (ifyes)
                {
                sprintf (text, "%s.;", filename);
                ho_delete_file (text, &status);
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    {
                    wialert (WARNING, "MODEL/SAVE", mg_message (MO_NODELETE_FILE),
                             mg_message (MO_NEW_NAME));
                    strcpy (pModel->name, "");
                    }
                else
                    {
                    filename_is_bad = FALSE;
                    }
                }
            else
                {
                strcpy (pModel->name, "");
                continue;
                }
            }
        /* file name is ok and does not exist */
        
        filename_is_bad = FALSE;
        }
    /*
       * fill up and load the NL user structure into the pointlist
    */
    
    /* graphics wcs window coordinates              */
    
    pModel->nl_user_struct.wcs_xmin = pModel->graphics.world.xmin;
    pModel->nl_user_struct.wcs_xmax = pModel->graphics.world.xmax;
    pModel->nl_user_struct.wcs_ymin = pModel->graphics.world.ymin;
    pModel->nl_user_struct.wcs_ymax = pModel->graphics.world.ymax;
    
    /* wcs coordinates of corners of model          */
    /* note model ymin has already been set         */
    
    pModel->nl_user_struct.model_xmin = pModel->well.xmin;
    pModel->nl_user_struct.model_xmax = pModel->well.xmax;
    pModel->nl_user_struct.model_ymax = pModel->log_start;
    
    nl_set_nlist_user_struct (pModel->model.nlist, (BYTE *) & pModel->nl_user_struct,
                              sizeof(pModel->nl_user_struct));
    
    /* write nlist */
    
    nl_write_nlist (pModel->model.nlist, filename, pModel->model.id);
    
    if (IS_STRING (pModel->velocity.id))
        {
        nl_inq_nlist_int (pModel->velocity.nlist, NL_NLIST_NPOINTS, (INT *) & npts);
        if (npts != 0)
            {
            nl_write_nlist (pModel->velocity.nlist, filename, pModel->velocity.id);
            }
        }
    /* unsetneed to save flag */
    
    pModel->need_to_save = FALSE;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Check if a model has been saved and if not, query the   
    hoser if he wishes to save it.                            

Prototype:
    publicdef INT mo_check_save(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_check_save (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_check_save (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    BOOL ifyes;
    INT status;
    
    /* model state does not require saving */
    
    if (NOT pModel->need_to_save)
        return SUCCESS;
    
    /* query if want saved */
    
    wiyesno (mg_message (MO_SAVE_MODEL), "MODEL/SAVE", &ifyes, &status);
    
    /* save if so */
    if (ifyes)
        mo_save_model (pModel);
    return SUCCESS;
    }
/* END:     */
