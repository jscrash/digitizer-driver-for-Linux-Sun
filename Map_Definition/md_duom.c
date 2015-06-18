/* DEC/CMS REPLACEMENT HISTORY, Element MD_DUOM.C*/
/* *3    17-AUG-1990 21:56:39 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     7-MAR-1990 17:24:01 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *1    19-JUN-1989 13:06:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_DUOM.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif 

#ifndef ESI_MD_DEFS_H 

#include "esi_md_defs.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_AM_DEFS_H

#include "esi_am_defs.h"

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
    publicdef INT md_duom(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_duom (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_duom (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_DUOM");
    INT status;                     /* RETURN STATUS */
    DOUBLE value;
    UOM_NAME uom1;
    UOM_NAME uom2;
    CHAR dummy[22];
    INT slen;
    CHAR *cptr;
    MAP_STRUCTURE *map;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * GET MAP INFO STRUCTURE * */
    /* -------------------------- */
    
    mp_get_current_map (&map);
    
    /* * ENSURE THAT PARAMETER HAS UNITS INCLUDED * */
    /* -------------------------------------------- */
    
    status = tu_parse_unit (parmstruc->parmval.cval, &value, uom1, uom2);
    if (status != SUCCESS)
        {
        goto finished;
        }
    /* * ADD DEFAULT DUOM IF NO UOM IN STRING * */
    /* ---------------------------------------- */
    
    if (strlen (uom1) EQUALS 0)
        {
        strcpy (dummy, "1 ");
        strcat (dummy, map->map_scale_uom);
        tu_parse_unit (dummy, &value, uom1, uom2);
        if ((slen = strlen (uom2)) > 0)
            {
            cptr = (CHAR *)am_allocate (AM_GLOBAL,
                                        strlen (parmstruc->parmval.cval) + slen + 2);
            strcpy (cptr, parmstruc->parmval.cval);
            strcat (cptr, " ");
            strcat (cptr, uom2);
            am_free (parmstruc->parmval.cval);
            parmstruc->parmval.cval = cptr;
            }
        }
    /* * CONVERT TO WORLD UNITS * */
    /* -------------------------- */
    
    status = tu_disp_units_to_wcs (parmstruc->parmval.cval, &value,
                                   map->wcs_to_inches);
    
    /* * TRANSLATE PARAMETER TO WORLD UNITS * */
    /* -------------------------------------- */
    
    if (status EQUALS SUCCESS)
        {
        if (keylist->tran_flg EQUALS 'Y')
            {
            am_free (parmstruc->parmval.cval);
            parmstruc->parmtype = 3;
            parmstruc->parmval.dbval = value;
            }
        else
            {
            parmstruc->parmtype = 1;
            }
        }
finished:
    return status;
    }
/* END:     */
