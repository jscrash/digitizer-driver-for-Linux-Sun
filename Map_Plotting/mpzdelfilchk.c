/*  DEC/CMS REPLACEMENT HISTORY, Element MPZDELFILCHK.C*/
/*  *5     7-JUN-1991 11:50:13 MING "(SPR 0) change seismic time to seismic data"*/
/*  *4    17-AUG-1990 22:02:35 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    20-FEB-1990 10:08:03 JULIAN "(SPR 5095) culture overlay"*/
/*  *2    26-JUN-1989 10:50:07 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:09:44 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZDELFILCHK.C*/
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
#include "esi_mp.h"
#include "esi_fm.h"
#include "esi_am.h"
#include "esi_md_basis.x"

/* Function Description -----------------------------------------------------
Description:
    Routine to check if any of the keywords being
    deleted are filenames of a type that is tracked in the MP_FILES
    structures in the MAP_STRUCTURE.  If so, then update
    the MP_FILES structure.

Prototype:
    publicdef INT mpz_delete_file_check(BOOL overlay);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    overlay         -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mpz_delete_file_check (BOOL overlay)
#else
publicdef INT mpz_delete_file_check (overlay)
BOOL overlay;
#endif
    {
    
    MAP_STRUCTURE *map;
    MD_STATE_WORKSPACE *md_state;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_KEY_INFO *key_info;
    MD_VERB_INFO *verb_info;
    INT i, j, k, m, n;
    INT dum_key_ndx[3];
    MP_FILES *file_struc;
    CHAR *type;
    CHAR *cdum;
    
    /* ********************************************************************** */
    
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    display = map->display;
    compiled = map->compiled;
    
    i = (overlay) ?
        md_state->crnt_pref_verb_start - 1 : md_state->crnt_pref_key_parm - 1;
    
    verb_info = &(overlay_verbs[display->verb_ndx[i]]);
    verbstruc = compiled->verb_ptr[display->compiled_row[i]];
    
    /* * ONLY NEED TO DEAL WITH THOSE OVERLAYS WHICH MIGHT USE A FILE * */
    /* ---------------------------------------------------------------- */
    
    if ((verb_info->verb_code EQUALS MP_GRA_OBJS) OR (verb_info->
                                                     verb_code EQUALS 
                                                     MP_SEISDPTH) OR (verb_info->
                                                                     verb_code EQUALS 
                                                                     MP_SEISCHRN) OR (
                                                                             verb_info->
                                                                                     verb_code
                                                                                      EQUALS
                                                                                      MP_SEISPACH
                                                                                     ) OR 
        (verb_info->verb_code EQUALS MP_MISTIE) OR (verb_info->
                                                   verb_code EQUALS 
                                                   MP_SEISDATA) OR (verb_info->
                                                                   verb_code EQUALS 
                                                                   MP_WELLS) OR (
                                                                             verb_info->
                                                                                verb_code
                                                                                 EQUALS 
                                                                           MP_WELLDPTH) OR 
        (verb_info->verb_code EQUALS MP_WELLPACH) OR (verb_info->
                                                     verb_code EQUALS 
                                                     MP_GRID_DSP) OR (verb_info->
                                                                     verb_code EQUALS 
                                                                     MP_CONTOURS) OR (
                                                                             verb_info->
                                                                                     verb_code
                                                                                      EQUALS
                                                                                      MP_PERSPEC
                                                                                     ) OR 
        (verb_info->verb_code EQUALS MP_XYZ_DSP) OR (verb_info->
                                                    verb_code EQUALS MP_TOPOGPHY))
        {
        
        /* * GET THE INDEXES FOR THE FILE KEYWORDS AND CHECK IF THEY ARE USED * */
        /* -------------------------------------------------------------------- */
        
        if (overlay)
            {
            m = 0;
            n = verb_info->no_of_keywords;
            }
        else
            {
            m = display->key_ndx[i];
            n = m + 1;
            }
        j = 0;
        for (i = m; i < n; i++)
            {
            key_info = &(verb_info->key_list[i]);
            if (((key_info->val_code EQUALS FILER$) OR (key_info->
                                                       val_code EQUALS 
                                                       FILEW$)) AND (verbstruc->
                                                                    key_used[
                                                                           i] EQUALS 1))
                {
                dum_key_ndx[j] = i;
                j++;
                if ((NOT overlay) AND (NOT md_state->
                                      verb_pending) AND ((verb_info->
                                                         verb_code EQUALS 
                                                         MP_CONTOURS) OR (
                                                                  verb_info->
                                                                      verb_code 
                                                                     EQUALS 
                                                                        MP_PERSPEC) OR (
                                                                             verb_info->
                                                                                       verb_code
                                                                                        EQUALS
                                                                                        MP_GRID_DSP
                                                                                       )
                                                        ))
                    {
                    md_state->verb_pending = TRUE;
                    }
                }
            }
        /* * HAVE A FILE KEYWORD THAT WAS USED, CHECK AGAINST DEFINED TABLE * */
        /* ------------------------------------------------------------------ */
        
        for (i = 0; i < j; i++)
            {
            
            /* * SET FILE TYPE * */
            /* ----------------- */
            
            key_info = &(verb_info->key_list[dum_key_ndx[i]]);
            type = key_info->val_arg + 4;
            mpz_get_file_struct (type, &file_struc);
            mpz_drop_from_file_struct ((verbstruc->parm_ptr[dum_key_ndx[i]])->parmval.
                                       cval, file_struc);
            }
        }
    return SUCCESS;
    }
/* END:     */
