/* DEC/CMS REPLACEMENT HISTORY, Element MP_DSP_MP.C*/
/* *9     3-MAY-1991 12:38:20 MING "(SPR -1) fix tic map from charlie"*/
/* *8    18-DEC-1990 10:03:26 JESSIE "(SPR 6345) handle UNIT OF MEASURES are conflict"*/
/* *7     5-DEC-1990 10:32:52 JESSIE "(SPR 6233) accept xtic/ytic with DEGREES"*/
/* *6    13-NOV-1990 12:51:19 MING "(SPR 6156) use dynamic memory allocation for variable parameters to fix the bug when it*/
/*crashed on string is long"*/
/* *5    13-NOV-1990 10:47:50 JESSIE "(SPR 0) check DMS input format"*/
/* *4    17-AUG-1990 22:04:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    17-MAY-1990 11:28:26 WALTERS "(SPR 5122) Add checks for graticule overlays"*/
/* *2    26-JUN-1989 10:54:46 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:11:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MP_DSP_MP.C*/
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

#include    "esi_gl_defs.h"
#include    "esi_gl_project.h"
#include    "esi_mp.h"
#include    "esi_am.h"
#include    "esi_ts.h"
#include    "esi_qs.h"
#include    "esi_ct.h"
#include    "esi_wi.h"
#include    "esi_md_err.h"
#include    "esi_md_msg.h"

#ifndef ESI_MG_H

#include    "esi_mg.h"

#endif

#include    "md_mapdef.rh"
static BOOL mpz_graticule_ok ();

#define UNITS_NOT_DEFINED (INT)-1

/* File Description ---------------------------------------------------------
Overview:
    Drives drawing process for maps.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mp_display_map(BOOL full,BOOL screen);

Private_Functions:
    static BOOL mpz_graticule_ok (BOOL full);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Drives drawing process for maps.

Prototype:
    publicdef INT mp_display_map(BOOL full,BOOL screen);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    full            -(BOOL)
    screen          -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
   
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_display_map (BOOL full, BOOL screen)
#else
publicdef INT mp_display_map (full, screen)
BOOL full;
BOOL screen;
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    RESOURCE_ID *cdlg_id;
    MAP_NAME map_name;
    INT i;
    
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    
    /* * CHECK THAT A PREFERENCE OVERLAY HAS BEEN SELECTED IF NOT FULL * */
    /* ----------------------------------------------------------------- */
    
    if (NOT full AND md_state->crnt_pref_verb_start EQUALS 0)
        {
        wi_beep (1);
        wi_set_dialog_text (cdlg_id->overlays_id, MD_OVER_MSG,
                            mg_message (MD_SELECT_ONE_VERB));
        goto finished;
        }
    /* * CHECK VALIDITY OF GRATICULE OVERLAY * */
    /* ---------------------------------- */
    
    if (mpz_graticule_ok (full) != SUCCESS)
        {
        goto finished;
        }
    /* * CHECK VALIDITY OF LAST OVERLAY * */
    /* ---------------------------------- */
    
    if (NOT mpz_pending_ok ())
        {
        goto finished;
        }
    /* * CHECK VALIDITY OF OTHER OVERLAYS IF NECESSARY * */
    /* ------------------------------------------------- */
    
    if (NOT mapdef->valid AND mp_valid_check (full) != SUCCESS)
        {
        goto finished;
        }
    /* * CHECK THE EXISTENCE OF SETUP PARAMETERS * */
    /* ------------------------------------------- */
    
    if (NOT mapdef->setup_ok)
        {
        wi_set_dialog_text (cdlg_id->map_setup_id, MD_SETUP_MSG,
                            mg_message (MD_BAD_SETUP));
        wi_enable_dialog_item (cdlg_id->map_setup_id, MD_SETUP_CANCEL, OFF);
        mp_map_setup_pipe ();
        wi_enable_dialog_item (cdlg_id->map_setup_id, MD_SETUP_CANCEL, ON);
        }
    /* * CHECK MAP NAME * */
    /* ------------------ */
    
    wi_query_dialog_item (cdlg_id->overlays_id, MD_MAP_NAME, &i, map_name);
    if (strcmp (mapdef->map_name, map_name) != 0)
        {
        if (ho_chk_fname (map_name, strlen (map_name)) != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (MD_MAP_NOT_FOUND));
            goto finished;
            }
        strcpy (mapdef->map_name, map_name);
        }
    /* * SEND TO CORRECT GRAPHICS DEVICE * */
    /* ----------------------------------- */
    
    if (screen)
        {
        mpz_map_to_screen (full);
        }
    else
        {
        mpz_map_to_plotter (full);
        }
finished:
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static BOOL mpz_graticule_ok (BOOL full);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    full            -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    MD_UOM_CONFLICT
    MD_BAD_DEGREES
    
Scope:
    PRIVATE to <mp_dsp_mp.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static BOOL mpz_graticule_ok (BOOL full)
#else
static BOOL mpz_graticule_ok (full)
BOOL full;
#endif
    {
    BOOL is_a_graticule = FALSE;
    CHAR verbs[40];
    CHAR keywords[40];
    CHAR *parameters;
    CHAR str[40];
    CHAR msg[250];
    CHAR *s;
    DOUBLE xtic_value = 0;	/*****was undefined variable*****/
    DOUBLE ytic_value = 0;	/*****not nice              *****/
    DOUBLE x[2];
    DOUBLE y[2];
    INT uom_is_degrees = UNITS_NOT_DEFINED;
    INT horizontal_uom_is_degrees = FALSE;
    INT xtic_is_degrees = UNITS_NOT_DEFINED;
    INT ytic_is_degrees = UNITS_NOT_DEFINED;
    INT status = SUCCESS;
    INT status_dms = -1;
    INT i;
    INT start;
    INT end;
    INT ptr_to_current_verb;
    INT first_line_of_overlay;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    PROJECT_DEFAULT *project_default;
    PROJECTION_STRUCTURE *geodetic_projection;
#if 0
/* coordinates in mapdef are in MAP projection */
    PROJECTION_STRUCTURE *project_projection;
#endif
    PROJECTION_STRUCTURE *map_projection;
    RESOURCE_ID *cdlg_id;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR) & project_default);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
#if 0
    /* map coordinates are in MAP projection */
    qs_inq_c (QS_PROJECT_PROJECTION, str, (INT *)0);
#endif
    qs_inq_c (QS_MAP_PROJECTION, str, (INT *)0);
    ct_get_projection (str, &map_projection);
    ct_get_projection ("GEODETIC", &geodetic_projection);
    
    if (strstr (project_default->unit.horizontal, "DEGREES"))
        {
        horizontal_uom_is_degrees = TRUE;
        }
    ptr_to_current_verb = md_state->crnt_pref_verb_start;
    first_line_of_overlay = FALSE;
    
    if (full)
        {
        start = 0;
        end = display->last_row_used;
        }
    else
        {
        start = md_state->crnt_pref_verb_start - 1;
        end = md_state->crnt_pref_verb_end;
        }
    for (i = start; i < end; i++)
        {
        strcpy (verbs, display->verbs[i]);
        strcpy (keywords, display->keywords[i]);
        parameters = (CHAR *) tc_alloc (strlen (display->parameters[i]) + 1);
        strcpy (parameters, display->parameters[i]);
        ts_snowhite (parameters);
        
        if (is_a_graticule && strstr (verbs, "-----"))
            {
            is_a_graticule = FALSE;
            
            if (uom_is_degrees == UNITS_NOT_DEFINED)
                {
                uom_is_degrees = horizontal_uom_is_degrees;
                }
            if (uom_is_degrees == TRUE && xtic_is_degrees == FALSE)
                {
                sprintf (msg, mg_message (MD_UOM_CONFLICT), "X TIC INTERVAL");
                wi_beep (1);
                wi_alert (0, "", "WARNING", msg);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                    ptr_to_current_verb);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                tc_free (parameters);		
                return MD_UOM_CONFLICT;
                }
            else if (uom_is_degrees == TRUE && ytic_is_degrees == FALSE)
                {
                sprintf (msg, mg_message (MD_UOM_CONFLICT), "Y TIC INTERVAL");
                wi_beep (1);
                wi_alert (0, "", "WARNING", msg);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                    ptr_to_current_verb);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                tc_free (parameters);		
                return MD_UOM_CONFLICT;
                }
            else if (uom_is_degrees == FALSE && xtic_is_degrees == TRUE)
                {
                sprintf (msg, mg_message (MD_UOM_CONFLICT), "X TIC INTERVAL");
                wi_beep (1);
                wi_alert (0, "", "WARNING", msg);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                    ptr_to_current_verb);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                tc_free (parameters);		
                return MD_UOM_CONFLICT;
                }
            else if (uom_is_degrees == FALSE && ytic_is_degrees == TRUE)
                {
                sprintf (msg, mg_message (MD_UOM_CONFLICT), "Y TIC INTERVAL");
                wi_beep (1);
                wi_alert (0, "", "WARNING", msg);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                    ptr_to_current_verb);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                tc_free (parameters);		
                return MD_UOM_CONFLICT;
                }
            if (uom_is_degrees == TRUE)
                {
                ct_transform_point (mapdef->lower_left_xy.x, mapdef->lower_left_xy.y,
                                    map_projection, &x[0], &y[0],
                                    geodetic_projection);
                ct_transform_point (mapdef->upper_right_xy.x, mapdef->upper_right_xy.y,
                                    map_projection, &x[1], &y[1],
                                    geodetic_projection);
                
                if (xtic_value > (x[1] - x[0]))
                    {
                    sprintf (msg, mg_message (MD_BAD_DEGREES), "X TIC INTERVAL");
                    wi_beep (1);
                    wi_alert (0, "", "WARNING", msg);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                        ptr_to_current_verb);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                    tc_free (parameters);		
                    return MD_BAD_DEGREES;
                    }
                if (ytic_value > (y[1] - y[0]))
                    {
                    sprintf (msg, mg_message (MD_BAD_DEGREES), "Y TIC INTERVAL");
                    wi_beep (1);
                    wi_alert (0, "", "WARNING", msg);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                        ptr_to_current_verb);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                    wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                    tc_free (parameters);		
                    return MD_BAD_DEGREES;
                    }
                }
/*      we are resetting in case of more than one graticule in this map
        also need to reset xtic_value and ytic_value since one graticule could
	be geodetic (uom=degrees) and the other could be XY
        This gets rid of occasional error reporting tic interval invalid
	when it really is ok */

            uom_is_degrees = UNITS_NOT_DEFINED;
            horizontal_uom_is_degrees = FALSE;
            xtic_is_degrees = UNITS_NOT_DEFINED;
            ytic_is_degrees = UNITS_NOT_DEFINED;
	    xtic_value = 0.0;
	    ytic_value = 0.0;
            }
        if (is_a_graticule || (first_line_of_overlay = ARE_SAME (verbs, "GRATICULE")))
            {
            is_a_graticule = TRUE;
            
            if (first_line_of_overlay)
                {
                first_line_of_overlay = FALSE;
                ptr_to_current_verb = i + 1;
                }
            if (ARE_SAME (keywords, "X TIC INTERVAL"))
                {
                    s = ts_next_word (parameters, str);
                    xtic_value = (DOUBLE)atof (str);

		    if ((status_dms = con_dms2dec(parameters , &xtic_value))
				    == SUCCESS)
			{
/*			uom_is_degrees = TRUE;
*/			xtic_is_degrees = TRUE;
			}
                    else if (s == NULL)
                        {
                        xtic_is_degrees = UNITS_NOT_DEFINED;
                        }
                    else
                        {
                        xtic_is_degrees = FALSE;
                        }
                }
            else if (ARE_SAME (keywords, "Y TIC INTERVAL"))
                {
                    s = ts_next_word (parameters, str);
                    ytic_value = (DOUBLE)atof (str);

		    if ((status_dms = con_dms2dec(parameters , &ytic_value))
				    == SUCCESS)
			{
/*			uom_is_degrees = TRUE;
*/			ytic_is_degrees = TRUE;
			}
                    else if (s == NULL)
                        {
                        ytic_is_degrees = UNITS_NOT_DEFINED;
                        }
                    else
                        {
                        ytic_is_degrees = FALSE;
                        }
                }
            else if (ARE_SAME (keywords, "UNIT OF MEASURE"))
                {
                if (ARE_SAME (parameters, "DEGREES"))
                    {
                    uom_is_degrees = TRUE;
                    }
                else
                    {
                    uom_is_degrees = FALSE;
                    }
                }
            }
        tc_free (parameters);	/* want to free all, not just last one */
        }
    if (is_a_graticule)
        {
        
        if (uom_is_degrees == UNITS_NOT_DEFINED)
            {
            uom_is_degrees = horizontal_uom_is_degrees;
            }
        if (uom_is_degrees == TRUE && xtic_is_degrees == FALSE)
            {
            sprintf (msg, mg_message (MD_UOM_CONFLICT), "X TIC INTERVAL");
            wi_beep (1);
            wi_alert (0, "", "WARNING", msg);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                ptr_to_current_verb);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
            return MD_UOM_CONFLICT;
            }
        else if (uom_is_degrees == TRUE && ytic_is_degrees == FALSE)
            {
            sprintf (msg, mg_message (MD_UOM_CONFLICT), "Y TIC INTERVAL");
            wi_beep (1);
            wi_alert (0, "", "WARNING", msg);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                ptr_to_current_verb);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
            return MD_UOM_CONFLICT;
            }
        else if (uom_is_degrees == FALSE && xtic_is_degrees == TRUE)
            {
            sprintf (msg, mg_message (MD_UOM_CONFLICT), "X TIC INTERVAL");
            wi_beep (1);
            wi_alert (0, "", "WARNING", msg);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                ptr_to_current_verb);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
            return MD_UOM_CONFLICT;
            }
        else if (uom_is_degrees == FALSE && ytic_is_degrees == TRUE)
            {
            sprintf (msg, mg_message (MD_UOM_CONFLICT), "Y TIC INTERVAL");
            wi_beep (1);
            wi_alert (0, "", "WARNING", msg);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                ptr_to_current_verb);
            wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
            wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
            return MD_UOM_CONFLICT;
            }
        if (uom_is_degrees == TRUE)
            {
            if (status_dms == SUCCESS)
                {
                ct_transform_point (mapdef->lower_left_xy.x, mapdef->lower_left_xy.y,
                                map_projection, &x[0], &y[0], geodetic_projection);
                ct_transform_point (mapdef->upper_right_xy.x, mapdef->upper_right_xy.y,
                                map_projection, &x[1], &y[1], geodetic_projection);
                }
            else
                {
                x[0] = mapdef->lower_left_xy.x;
                x[1] = mapdef->upper_right_xy.x;
                y[0] = mapdef->lower_left_xy.y;
                y[1] = mapdef->upper_right_xy.y;
                }
            
            if (xtic_value > (x[1] - x[0]))
                {
                sprintf (msg, mg_message (MD_BAD_DEGREES), "X TIC INTERVAL");
                wi_beep (1);
                wi_alert (0, "", "WARNING", msg);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                    ptr_to_current_verb);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                return MD_BAD_DEGREES;
                }
            if (ytic_value > (y[1] - y[0]))
                {
                sprintf (msg, mg_message (MD_BAD_DEGREES), "Y TIC INTERVAL");
                wi_beep (1);
                wi_alert (0, "", "WARNING", msg);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                                    ptr_to_current_verb);
                wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
                wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
                return MD_BAD_DEGREES;
                }
            }
        }
    return status;
    }
/* END:     */
