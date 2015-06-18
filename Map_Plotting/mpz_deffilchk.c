/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_DEFFILCHK.C*/
/*  *3    17-AUG-1990 22:02:53 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:50:17 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:09:52 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_DEFFILCHK.C*/
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
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_fm.h"
#include "esi_fm_err.h"
publicdef INT mpz_check_struct ();

/* File Description ---------------------------------------------------------
Overview:
    Routine to check if any of the defined
    files are now disk files.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT mpz_defined_file_check();

    publicdef INT mpz_check_struct(MP_FILES *file_struc,FM_FILE log_name,
        FM_TYPE type);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Routine to check if any of the defined
    files are now disk files.

Prototype:
    publicdef INT mpz_defined_file_check();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mpz_defined_file_check ()
#else
publicdef INT mpz_defined_file_check ()
#endif
    {
    
    MAP_STRUCTURE *map;
    FM_FILE log_name;
    FM_TYPE type;
    MP_FILES *file_struc;
    
    /* ********************************************************************** */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    
    strcpy (log_name, "ESI$CONTOURS");
    strcpy (type, "CONTOUR");
    file_struc = &(map->contours);
    mpz_check_struct (file_struc, log_name, type);
    
    strcpy (log_name, "ESI$GRIDS");
    strcpy (type, "GRID");
    file_struc = &(map->grids);
    mpz_check_struct (file_struc, log_name, type);
    
    strcpy (log_name, "ESI$SCATTERS");
    strcpy (type, "SCATTER");
    file_struc = &(map->scatters);
    mpz_check_struct (file_struc, log_name, type);
    
    strcpy (log_name, "ESI$PERSPECTIVES");
    strcpy (type, "PERSPECTIVE");
    file_struc = &(map->perspectives);
    mpz_check_struct (file_struc, log_name, type);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT mpz_check_struct(MP_FILES *file_struc,FM_FILE log_name,
        FM_TYPE type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    log_name        -(FM_FILE)
    type            -(FM_TYPE)
    file_struc      -(MP_FILES *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mpz_check_struct (MP_FILES *file_struc, FM_FILE log_name, FM_TYPE type)
#else
publicdef INT mpz_check_struct (file_struc, log_name, type)
FM_FILE log_name;
FM_TYPE type;
MP_FILES *file_struc;
#endif
    {
    INT i;
    INT status;
    INT move_steps = 0;
    
    /* ---------------------------------------------------- */
    
    for (i = 0; i < file_struc->n_names; i++)
        {
        if ((status = fm_check_access (log_name, file_struc->name[i], type,
                                       "r")) != FM_NO_SUCH_FILE)
            {
            tc_free (file_struc->name[i]);
            move_steps++;
            }
        else if (move_steps > 0)
            {
            file_struc->name[i - move_steps] = file_struc->name[i];
            file_struc->n_references[i - move_steps] = file_struc->n_references[i];
            }
        }
    if (move_steps > 0)
        {
        file_struc->n_names -= move_steps;
        file_struc->name[file_struc->n_names] = NULL;
        }
    return SUCCESS;
    }
/* END:     */
