/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_UOMD.C*/
/* *4    17-AUG-1990 21:59:36 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     7-MAR-1990 17:24:18 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *2    26-JUN-1989 10:49:06 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:08:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_UOMD.C*/
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

#ifndef ESI_MD_DEFS_H

#include "esi_md_defs.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to validate DUOM type parameter.

Prototype:
    publicdef INT md_verify_uomd(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **) Map def parameter structure.       

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_uomd (MD_KEY_INFO *key_info,
                              CHAR parm_string[MD_MAX_PARM_STRING],
                              MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_uomd (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    MD_PARM_STRUCT *temp_parm;
    DOUBLE value;
    UOM_NAME uom1;
    UOM_NAME uom2;
    CHAR dummy[22];
    INT slen;
    CHAR temp_string[MD_MAX_PARM_STRING + 100];
    CHAR mapname[40];
    MAP_STRUCTURE *map;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * GET MAP INFO STRUCTURE * */
    /* -------------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    
    /* * ENSURE THAT PARAMETER HAS UNITS INCLUDED * */
    /* -------------------------------------------- */
    
    status = tu_parse_unit (parm_string, &value, uom1, uom2);
    if (status EQUALS SUCCESS)
        {
        
        /* * ADD DEFAULT DUOM IF NO UOM IN STRING * */
        /* ---------------------------------------- */
        
        strcpy (temp_string, parm_string);
        if (strlen (uom1) EQUALS 0)
            {
            strcpy (dummy, "1 ");
            strcat (dummy, map->map_scale_uom);
            tu_parse_unit (dummy, &value, uom1, uom2);
            if ((slen = strlen (uom2)) > 0)
                {
                strcat (temp_string, " ");
                strcat (temp_string, uom2);
                }
            }
        /* * CONVERT TO WORLD UNITS * */
        /* -------------------------- */
        
        status = tu_disp_units_to_wcs (temp_string, &value, map->wcs_to_inches);
        
        /* * TRANSLATE PARAMETER TO WORLD UNITS * */
        /* -------------------------------------- */
        
        if (status EQUALS SUCCESS)
            {
            temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL,
                                                       sizeof(MD_PARM_STRUCT));
            if (key_info->translate)
                {
                temp_parm->parmtype = 3;
                temp_parm->parmval.dbval = value;
                }
            else
                {
                temp_parm->parmtype = 1;
                temp_parm->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL,
                                                               strlen (temp_string) +
                                                               1);
                strcpy (temp_parm->parmval.cval, temp_string);
                }
            temp_parm->nxtparm = NULL_PARM;
            *parmstruc = temp_parm;
            }
        }
    return status;
    }
/* END:     */
