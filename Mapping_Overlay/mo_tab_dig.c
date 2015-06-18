/* DEC/CMS REPLACEMENT HISTORY, Element MO_TAB_DIG.C*/
/* *3    17-AUG-1990 22:02:28 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     6-NOV-1989 11:26:48 JULIAN "(SPR 30) add mg_message"*/
/* *1    19-JUN-1989 13:09:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_TAB_DIG.C*/
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
#include "esi_fi.h"
#include "mo_strat.rh"
#include "esi_c_lib.h"
#include "esi_math.h"
#include "esi_tg.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "esi_wm.h"
#include "esi_am.h"
#include "esi_mg.h"
#include "esi_mo_msg.h"
static CHAR NullString[] = "";
static CHAR resource_file[] = "MO_STRAT";

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mo_register_segment(STRAT_MODEL_STRUCT  *pModel);
    
    publicdef INT mo_get_tab_control_points(FLOAT xy_cntl[3][2]);
    
    publicdef INT mo_digitize_tab_control_points(FLOAT xy_tablet[][2]);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This routine performs the tablet registration which will 
    the user to digitize an empirical model from the tablet. 

Prototype:
    publicdef INT mo_register_segment(STRAT_MODEL_STRUCT  *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT  *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_register_segment (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_register_segment (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT status;
    FLOAT xy_cntl[3][2];
    FLOAT xy_tablet[3][2];
    FLOAT llx, lly, urx, ury, xpoints[3], ypoints[3];
    INT no_pnts = 3;
    INT i;
    FLOAT x_tab, y_tab, x_cntl, y_cntl;
    DOUBLE t_mat[2][2];
    FLOAT delta_y;
    INT Cancel = 2;
    
    /******************************************************************************/
    
    status = wmtregonoff (OFF);
    
    status = mo_get_tab_control_points (xy_cntl);
    if (status == FAIL)
        {
        return FAIL;
        }
    /* Get the control points  */
    
    status = mo_digitize_tab_control_points (xy_tablet);
    
    if (status == FAIL)
        {
        return FAIL;
        }
    /* Now actually register the transformation */
    
    llx = pModel->graphics.world.xmin;
    lly = pModel->graphics.world.ymin;
    urx = pModel->graphics.world.xmax;
    ury = pModel->graphics.world.ymax;
    
    /* Now actually register the transformation */
    
    mr_calc_xform (xy_cntl, xy_tablet, no_pnts, t_mat);
    
    /* Calculate a 3 point registration to fake out the window manager's goofyness */
    
    mr_calc_3p (xy_cntl, xy_tablet, llx, lly, urx, ury, xpoints, ypoints, t_mat);
    
    /* Now register the model segment  */
    
    status = wmtregistration (pModel->graphics.transform_id, xpoints, ypoints,
                              pModel->graphics.world.xmin, pModel->graphics.world.ymin,
                              pModel->graphics.world.xmax,
                              pModel->graphics.world.ymax);
    
    /* Renable registration */
    
    status = wmtregonoff (ON);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine brings up a dialog and gets log registrat-  
    ion (calibration of the grid) info from the abuser.     

Prototype:
    publicdef INT mo_get_tab_control_points(FLOAT xy_cntl[3][2]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xy_cntl         -(FLOAT [][])

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_get_tab_control_points (FLOAT xy_cntl[3][2])
#else
publicdef INT mo_get_tab_control_points (xy_cntl)
FLOAT xy_cntl[3][2];
#endif
    {
    INT ID = 0;
    INT status, td_flag;
    CHAR message[81];
    CHAR text[128];
    INT Item, value, done = FALSE;
    /******************************************************************************/
    
    fi_push ();
    /* define dialog resources     */
    status = am_define_resource (AM_DIALOG, resource_file, MO_REG_MOD_SEGMENT,
                                 NULL_FUNCTION_PTR, &ID, NULL);
    
    wi_open_dialog (ID);
    
    wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE, mg_message (MO_ENTER_CRLPOINT));
    
    wi_set_dialog_text (ID, MO_REG_LL_CP_X, NullString);
    wi_set_dialog_text (ID, MO_REG_LL_CP_Y, NullString);
    wi_set_dialog_text (ID, MO_REG_LR_CP_X, NullString);
    wi_set_dialog_text (ID, MO_REG_LR_CP_Y, NullString);
    wi_set_dialog_text (ID, MO_REG_UL_CP_X, NullString);
    wi_set_dialog_text (ID, MO_REG_UL_CP_Y, NullString);
    
    while (done == FALSE)
        {
        wi_request_dialog_item (&ID, &Item, &value, text);
        
        switch (Item)
            {
        case MO_REG_BTN_OK:
            wi_query_dialog_item (ID, MO_REG_LL_CP_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE,
                                    mg_message (MO_INVALID_LLX));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &xy_cntl[0][0]);
            wi_query_dialog_item (ID, MO_REG_LL_CP_Y, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE,
                                    mg_message (MO_INVALID_LLY));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &xy_cntl[0][1]);
            
            wi_query_dialog_item (ID, MO_REG_UL_CP_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE,
                                    mg_message (MO_INVALID_ULX));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &xy_cntl[1][0]);
            
            wi_query_dialog_item (ID, MO_REG_UL_CP_Y, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE,
                                    mg_message (MO_INVALID_ULY));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &xy_cntl[1][1]);
            
            wi_query_dialog_item (ID, MO_REG_LR_CP_X, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE,
                                    mg_message (MO_INVALID_LRX));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &xy_cntl[2][0]);
            
            wi_query_dialog_item (ID, MO_REG_LR_CP_Y, &value, text);
            if ((ts_isfloat (text) <= 0) || (strlen (text) == 0))
                {
                wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE,
                                    mg_message (MO_INVALID_LRY));
                wi_beep (1);
                break;
                }
            else
                sscanf (text, "%f", &xy_cntl[2][1]);
            done = TRUE;
            break;
            
        case MO_REG_BTN_CANCEL:
            wi_set_dialog_text (ID, MO_REG_STAT_MESSAGE, NullString);
            status = FAIL;
            done = TRUE;
            break;
            
        default:
            break;
            }
        }
    /* Release resource and restore envir.*/
    am_release_resource (ID);
    fi_pop ();
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine gets (3) log registration control points   
    from the user to set up the tablet transformation.      
    Direct GKS calls are used to return tablet coordin-      
    ates.                                                   

Prototype:
    publicdef INT mo_digitize_tab_control_points(FLOAT xy_tablet[][2]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xy_tablet       -(FLOAT [][]) Digitized values.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_digitize_tab_control_points (FLOAT xy_tablet[][2])
#else
publicdef INT mo_digitize_tab_control_points (xy_tablet)
FLOAT xy_tablet[][2];
#endif
    {
    FLOAT x_tablet, y_tablet;
    INT status, button;
    INT done;
    INT locate_tnr;
    /******************************************************************************/
    
    done = FALSE;
    
    while (done == FALSE)
        {
        /*  Get lower left point  */
        
        am_message (AM_DIALOG, mg_message (MO_DIGIT_LL));
        
        /* To properly register for JOURNAL/PLAYBACK... */
        wi_request_locate (&status, &locate_tnr, &x_tablet, &y_tablet, &button);
        
        if (status == 0)
            {
            return FAIL;
            }
        xy_tablet[0][0] = x_tablet;
        xy_tablet[0][1] = y_tablet;
        wi_beep (1);
        
        /*  Get the upper left control point */
        
        while (done == FALSE)
            {
            am_message (AM_DIALOG, mg_message (MO_DIGIT_UL));
            
            /* To properly register for JOURNAL/PLAYBACK... */
            wi_request_locate (&status, &locate_tnr, &x_tablet, &y_tablet, &button);
            
            if (status == 0)
                break;
            xy_tablet[1][0] = x_tablet;
            xy_tablet[1][1] = y_tablet;
            wi_beep (1);
            
            /*  Get the lower right control point */
            
            am_message (AM_DIALOG, mg_message (MO_DIGIT_LR));
            
            /* To properly register for JOURNAL/PLAYBACK... */
            wi_request_locate (&status, &locate_tnr, &x_tablet, &y_tablet, &button);
            
            if (status == 0)
                continue;
            xy_tablet[2][0] = x_tablet;
            xy_tablet[2][1] = y_tablet;
            wi_beep (1);
            done = TRUE;
            }
        }
    am_message (AM_DIALOG, NullString);
    
    return SUCCESS;
    }
/* END:     */
