/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_FILE.C*/
/* *4    17-AUG-1990 21:59:12 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    24-OCT-1989 09:38:34 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *2    26-JUN-1989 10:48:56 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:07:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_FILE.C*/
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
#include "esi_gl_defs.h"
#include "esi_md_defs.h"
#include "esi_md_dbdefs.h"
#include "esi_mp.h"
#include "esi_fm.h"
#include "esi_fm_err.h"
#include "esi_am.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Function to check a file parameter.

Prototype:
    publicdef INT md_verify_file(MD_KEY_INFO *key_info,FM_LONG parm_string,
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(FM_LONG)
    parmstruc       -(MD_PARM_STRUCT **)        

Return Value/Status:
    SUCCESS - Successful completion.
    FM_NO_SUCH_FILE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_file (MD_KEY_INFO *key_info, FM_LONG parm_string,
                              MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_file (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
FM_LONG parm_string;
MD_PARM_STRUCT **parmstruc;
#endif
    {
    INT status;                     /* RETURN STATUS */
    MAP_STRUCTURE *map;
    MD_PARM_STRUCT *temp_parm;
    CHAR type[31];
    MP_FILES *file_struct;
    INT i;
    CHAR mode[2];
    FM_LONG test;
    
    /* ********************************************************************** */
    
    /* * SET UP FOR CORRECT FILE TYPE * */
    /* -------------------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    strcpy (type, key_info->val_arg + 4);
    type[strlen (type) - 1] = 0;
    mpz_get_file_struct (type, &file_struct);
    
    /* * CREATE TEST NAME * */
    /* -------------------- */
    
    fm_long_name (parm_string, test);
    
    /* * CHECK IF FILE IN DEFINED FILE LIST, IF SO ASSUME WILL HAVE ALL ACCESS * */
    /* ------------------------------------------------------------------------- */
    
    for (i = 0; i < file_struct->n_names; i++)
        {
        if (ARE_SAME(test, file_struct->name[i]))
            {
            goto file_ok;
            }
        }
    /* * FILE NOT IN DEFINED LIST, CHECK ACCESS AND EXISTENCE * */
    /* -------------------------------------------------------- */
    
    /* NOTE:    A non-existing file for either write (FILEW$) or 
            read (FILER$) access is OK for this routine.
            The routine MP_SPECIAL_CHECKS will on an overlay by
            overlay basis make the judgement if this is really true. */
    
    if (key_info->val_code EQUALS FILER$)
        {
        strcpy (mode, "r");
        }
    else
        {
        strcpy (mode, "w");
        }
    status = fm_check_access (key_info->val_arg, test, type, mode);
    if (status != FM_NO_SUCH_FILE AND status != SUCCESS)
        {
        goto finished;
        }
    /* * FILE SEEMS OK AT THIS TIME, RETURN PARM STRUCTURE * */
    /* ----------------------------------------------------- */
    
file_ok:
    temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_PARM_STRUCT));
    temp_parm->parmtype = 1;
    temp_parm->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL, strlen (test) + 1);
    strcpy (temp_parm->parmval.cval, test);
    temp_parm->nxtparm = NULL_PARM;
    *parmstruc = temp_parm;
    status = SUCCESS;
    
finished:
    return status;
    }
/* END:     */
