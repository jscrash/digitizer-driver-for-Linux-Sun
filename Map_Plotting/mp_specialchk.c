/*  DEC/CMS REPLACEMENT HISTORY, Element MP_SPECIALCHK.C*/
/*  *6     7-JUN-1991 11:51:03 MING "(SPR 0) change seismic time to seismic data"*/
/*  *5    30-OCT-1990 12:04:54 GILLESPIE "(SPR 37) Fix up Sun compiles"*/
/*  *4    17-AUG-1990 22:09:35 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    20-FEB-1990 10:08:40 JULIAN "(SPR 5095) culture overlay"*/
/*  *2    26-JUN-1989 11:00:40 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:52 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_SPECIALCHK.C*/
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
#include "esi_md_defs.h"
#include "esi_mp.h"
#include "esi_mg.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_fm.h"
#include "esi_fm_err.h"
#include "esi_md_err.h"
#include "md_mapdef.rh"
#include "esi_md_basis.x"

#if USE_PROTOTYPES
static INT zzz_input_file_check (BOOL *used, INT top_limit, CHAR *mess);
static INT zzz_check_access (CHAR *file, CHAR *type, CHAR *mode, CHAR *mess);
#else
static INT zzz_input_file_check ();
static INT zzz_check_access ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Routine to make the needed overlay specific checks,
    especially with regards to file usage.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mp_special_checks(INT line);

Private_Functions:
    static INT zzz_input_file_check(BOOL *used, INT top_limit, CHAR *mess);

    static INT zzz_check_access(CHAR *file, CHAR *type, CHAR *mode, CHAR *mess);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT mp_special_checks(INT line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_special_checks (INT line)
#else
publicdef INT mp_special_checks (line)
INT line;
#endif
    {
    
    INT status = SUCCESS;
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_KEY_INFO *key_info;
    MD_VERB_INFO *verb_info;
    INT i, j, k;
    CHAR mess[240];
    FM_TYPE type;
    RESOURCE_ID *cdlg_id;
    MD_STATE_WORKSPACE *md_state;
    INT key_ndx[5];
    BOOL file_option_used[5];
    
    /* ********************************************************************** */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&map);
    display = map->display;
    compiled = map->compiled;
    
    i = line - 1;
    verb_info = &(overlay_verbs[display->verb_ndx[i]]);
    verbstruc = compiled->verb_ptr[display->compiled_row[i]];
    
    tc_zeroes (key_ndx, 5 * sizeof(INT));
    tc_zeroes (file_option_used, 5 * sizeof(BOOL));
    
    switch (verb_info->verb_code)
        {
        
        /*-------------------------------------------------------------------------*/
        /* OVERLAYS THAT HAVE BOTH "XYZ FILE" and "ADD TO XYZ FILE" KEYWORDS       */
        /*-------------------------------------------------------------------------*/
        
    case MP_GRA_OBJS:
    case MP_SEISDPTH:
    case MP_SEISCHRN:
    case MP_SEISPACH:
    case MP_MISTIE:
    case MP_SEISDATA:
    case MP_WELLS:
    case MP_WELLDPTH:
    case MP_WELLPACH:
    case MP_TOPOGPHY:
        
        /*  Check for use of both "XYZ FILE" and "ADD TO XYZ FILE". *
         *  The overlay should use at most one of these keywords.   *
         *  If both are used, then it's an error: put up alert box  *
         *  and return with error condition.  Else it's OK and can  *
         *  can return with SUCCESS.                                */
        
        /* * GET THE INDEXES FOR THE SCATTER FILE KEYWORDS * */
        /* ------------------------------------------------- */
        
        j = 0;
        for (i = 0; i < verbstruc->nrows; i++)
            {
            key_info = &(verb_info->key_list[i]);
            if (verbstruc->key_used[i] EQUALS 1 AND strcmp (key_info->val_arg,
                                                            "ESI$SCATTERS") EQUALS 0)
                {
                key_ndx[j] = i;
                j++;
                }
            }
        /* * BOTH USED * */
        /* ------------- */
        
        if (j EQUALS 2)
            {
            sprintf (mess, "%s '%s', '%s'", mg_message (MD_EXCLUSION_VIOLATION),
                     verb_info->key_tcp[key_ndx[0]], verb_info->key_tcp[key_ndx[1]]);
            status = FAIL;
            }
        /* * CHECK WRITE ACCESS * */
        /* ---------------------- */
        
        else if (j EQUALS 1)
            {
            status = zzz_check_access ((verbstruc->parm_ptr[key_ndx[0]])->parmval.cval,
                                       "SCATTER", "w", mess);
            }
        break;
        
        /*-------------------------------------------------------------------------*/
        /* GRID DISPLAY OVERLAY                                                    */
        /*-------------------------------------------------------------------------*/
        
    case MP_GRID_DSP:
        
        /*  If "GRID FILE: INPUT" is used then no other file    *
         *  should be specified in the definition.          *
         *  Else, "XYZ FILE" must be specified, and         *
         *  "GRID FILE: OUTPUT" is optional.            */
        
        /* * GET INDICES FOR FILE KEYWORDS * */
        /* --------------------------------- */
        
        for (i = 0; i < verbstruc->nrows; i++)
            {
            switch (verbstruc->key_code[i])
                {
            case 20:                /* GRID FILE: INPUT */
                file_option_used[0] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[0] = i;
                break;
                
            case 17:                /* SCATTER FILE */
                file_option_used[1] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[1] = i;
                break;
                
            case 3:                 /* GRID FILE: OUTPUT */
                file_option_used[2] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[2] = i;
                break;
                }
            }
        /* * CHECK THAT ONE AND ONLY ONE INPUT FILE IS USED * */
        /* -------------------------------------------------- */
        
        if (status = zzz_input_file_check (file_option_used, 2, mess) != SUCCESS)
            {
            break;
            }
        /* * CHECK IF GRID INPUT OPTION USED * */
        /* ----------------------------------- */
        
        if (file_option_used[0])
            {
            /* MUST BE USED ALONE */
            
            if (file_option_used[2])
                {
                status = FAIL;
                sprintf (mess, "%s '%s', '%s'", mg_message (MD_EXCLUSION_VIOLATION),
                         verb_info->key_tcp[key_ndx[0]],
                         verb_info->key_tcp[key_ndx[2]]);
                }
            /* CHECK READ ACCESS */
            else
                {
                status =
                    zzz_check_access ((verbstruc->parm_ptr[key_ndx[0]])->parmval.cval,
                                      "GRID", "r", mess);
                }
            }
        /* * GRID INPUT NOT USED, CHECK OUTPUT FILE * */
        /* ------------------------------------------- */
        
        else
            {
            if (file_option_used[2])
                {
                if ((status =
                     zzz_check_access ((verbstruc->parm_ptr[key_ndx[2]])->parmval.cval,
                                       "GRID", "w", mess)) != SUCCESS)
                    {
                    break;
                    }
                }
            /* * CHECK SCATTER FILE READ ACCESS * */
            /* ---------------------------------- */
            
            status = zzz_check_access ((verbstruc->parm_ptr[key_ndx[1]])->parmval.cval,
                                       "SCATTER", "r", mess);
            }
        break;
        
        /*-------------------------------------------------------------------------*/
        /* CONTOURS AND PERSPECTIVE OVERLAYS                                       */
        /*-------------------------------------------------------------------------*/
        
    case MP_CONTOURS:
    case MP_PERSPEC:
        
        /*  If "CONTOUR FILE: INPUT" is used it must be the only    *
         *  file option.  Else,                 *
         *      "CONTOUR FILE: OUTPUT" is optional.         *
         *      If "GRID FILE: INPUT" is used, no other file option *
         *      should be used.  Else,              *
         *      "GRID FILE: OUTPUT" is optional.        *
         *      "XYZ FILE" is mandatory.            */
        
        if (verb_info->verb_code EQUALS MP_CONTOURS)
            {
            strcpy (type, "CONTOUR");
            }
        else
            {
            strcpy (type, "PERSPECTIVE");
            }
        /* * GET INDICES FOR FILE KEYWORDS * */
        /* --------------------------------- */
        
        for (i = 0; i < verbstruc->nrows; i++)
            {
            switch (verbstruc->key_code[i])
                {
            case 26:                /* CONTOUR FILE: INPUT */
                file_option_used[0] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[0] = i;
                break;
                
            case 27:                /* GRID FILE: INPUT */
                file_option_used[1] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[1] = i;
                break;
                
            case 19:                /* SCATTER FILE */
                file_option_used[2] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[2] = i;
                break;
                
            case 4:                 /* CONTOUR FILE: OUTPUT */
                file_option_used[3] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[3] = i;
                break;
                
            case 10:                /* GRID FILE: OUTPUT */
                file_option_used[4] = (verbstruc->key_used[i] EQUALS 1);
                key_ndx[4] = i;
                break;
                }
            }
        /* * CHECK THAT ONE AND ONLY ONE INPUT FILE IS USED * */
        /* -------------------------------------------------- */
        
        if (status = zzz_input_file_check (file_option_used, 3, mess) != SUCCESS)
            {
            break;
            }
        /* * IF CONTOUR INPUT OPTION USED, NO OTHERS SHOULD BE * */
        /* ----------------------------------------------------- */
        
        if (file_option_used[0])
            {
            if (file_option_used[3] OR file_option_used[4])
                {
                status = FAIL;
                sprintf (mess, "%s '%s'", mg_message (MD_EXCLUSION_VIOLATION),
                         verb_info->key_tcp[key_ndx[0]]);
                
                for (i = 3; i < 5; i++)
                    {
                    if (file_option_used[i])
                        {
                        strcat (mess, ", '");
                        strcat (mess, verb_info->key_tcp[key_ndx[i]]);
                        strcat (mess, "'");
                        }
                    }
                }
            /* CHECK READ ACCESS */
            else
                {
                status =
                    zzz_check_access ((verbstruc->parm_ptr[key_ndx[0]])->parmval.cval,
                                      type, "r", mess);
                }
            }
        /* * CONTOUR INPUT FILE, NOT USED, CHECK CONTOUR OUTPUT * */
        /* ------------------------------------------------------ */
        
        else
            {
            if (file_option_used[3])
                {
                if ((status =
                     zzz_check_access ((verbstruc->parm_ptr[key_ndx[3]])->parmval.cval,
                                       type, "w", mess)) != SUCCESS)
                    {
                    break;
                    }
                }
            /* * CHECK GRID FILE OPTIONS * */
            /* --------------------------- */
            
            if (file_option_used[1])
                {
                if (file_option_used[4])
                    {
                    status = FAIL;
                    sprintf (mess, "%s '%s', '%s'",
                             mg_message (MD_EXCLUSION_VIOLATION),
                             verb_info->key_tcp[key_ndx[1]],
                             verb_info->key_tcp[key_ndx[4]]);
                    }
                /* CHECK READ ACCESS */
                else
                    {
                    status =
                             zzz_check_access ((verbstruc->parm_ptr[key_ndx[1]])->
                                               parmval.cval, "GRID", "r", mess);
                    }
                }
            /* * GRID INPUT NOT USED, CHECK OUTPUT * */
            /* ------------------------------------- */
            
            else
                {
                if (file_option_used[4])
                    {
                    if ((status =
                         zzz_check_access ((verbstruc->parm_ptr[key_ndx[4]])->parmval.
                                           cval, "GRID", "w", mess)) != SUCCESS)
                        {
                        break;
                        }
                    }
                /* * CHECK SCATTER FILE READ ACCESS * */
                /* ---------------------------------- */
                
                status =
                    zzz_check_access ((verbstruc->parm_ptr[key_ndx[2]])->parmval.cval,
                                      "SCATTER", "r", mess);
                }
            }
        break;
        }
    /* * UPDATE THE DIALOG STATE * */
    /* --------------------------- */
    
    if (status != SUCCESS)
        {
        am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *)&cdlg_id);
        am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *)&md_state);
        
        wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_SELECT, 0);
        wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS, 0);
        wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT, 0);
        wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_SELECT, 0);
        wi_set_default_sel (cdlg_id->overlays_id, MD_KEY_PREFS, 0);
        wi_set_default_sel (cdlg_id->overlays_id, MD_PARM_PREFS, 0);
        
        wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_PREFS,
                            md_state->crnt_pref_verb_start);
        wi_set_default_sel (cdlg_id->overlays_id, MD_OVER_SELECT,
                            md_state->active_verb_ndx + 1);
        md_state->active_key_ndx = -1;
        
        md_state->button_status[0] = OFF;
        md_state->button_status[1] = OFF;
        md_state->button_status[2] = OFF;
        md_state->button_status[3] = ON;
        md_state->button_status[4] = OFF;
        md_state->button_status[5] = OFF;
        mp_set_mapdef_buttons ();
        
        am_message (AM_ALERT, mess);
        }
    return status;
    }
/* END:     */

/*************************************************************************

            L O C A L   F U N C T I O N S
                                    
*************************************************************************/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zzz_input_file_check(BOOL *used,INT top_limit,CHAR *mess);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    used            -(BOOL *)
    top_limit       -(INT)
    mess            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <mp_specialchk.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zzz_input_file_check (BOOL *used, INT top_limit, CHAR *mess)
#else
static INT zzz_input_file_check (used, top_limit, mess)
BOOL *used;
INT top_limit;
CHAR *mess;
#endif
    {
    INT status = SUCCESS;
    INT j = 0;
    INT i;
    
    /***************************************/
    
    for (i = 0; i < top_limit; i++)
        {
        if (used[i])
            {
            j++;
            }
        }
    if (j EQUALS 0)
        {
        status = FAIL;
        strcpy (mess, mg_message (MD_NO_INPUT_FILE_SPECIFIED));
        }
    else if (j > 1)
        {
        status = FAIL;
        strcpy (mess, mg_message (MD_ONLY_ONE_INPUT_FILE));
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zzz_check_access(CHAR *file,CHAR *type,CHAR *mode,CHAR *mess);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    file            -(CHAR *)   
    type            -(CHAR *)
    mode            -(CHAR *)
    mess            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <mp_specialchk.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zzz_check_access (CHAR *file, CHAR *type, CHAR *mode, CHAR *mess)
#else
static INT zzz_check_access (file, type, mode, mess)
CHAR *file;
CHAR *type;
CHAR *mode;
CHAR *mess;
#endif
    {
    INT status;
    FM_PATH log_name;
    
    /***************************************/
    
    sprintf (log_name, "ESI$%sS", type);
    
    status = fm_check_access (log_name, file, type, mode);
    if (status EQUALS FM_NO_SUCH_FILE)
        {
        status = SUCCESS;
        }
    else if (status != SUCCESS)
        {
        sprintf (mess, "%s '%s'", mg_message (status), file);
        status = FAIL;
        }
    return status;
    }
/* END:     */
