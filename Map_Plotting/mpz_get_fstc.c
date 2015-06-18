/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_GET_FSTC.C*/
/*  *3    17-AUG-1990 22:03:11 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:50:39 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:05 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_GET_FSTC.C*/
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

#include "esi_gl_defs.h"

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to get the parameter input from
    a list of files of specified type.

Prototype:
    publicdef INT mpz_get_file_struct(FM_TYPE type,MP_FILES **file_struct);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(FM_TYPE)
    file_struct     -(MP_FILES **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mpz_get_file_struct (FM_TYPE type, MP_FILES **file_struct)
#else
publicdef INT mpz_get_file_struct (type, file_struct)
FM_TYPE type;
MP_FILES **file_struct;
#endif
    {
    
    INT status = SUCCESS;
    MAP_STRUCTURE *map;
    
    /* ********************************************************************** */
    
    /* * SET FILE TYPE * */
    /* ----------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    switch (type[0])
        {
    case 'C':                       /* CONTOURS */
        *file_struct = &(map->contours);
        break;
        
    case 'G':                       /* GRIDS */
        *file_struct = &(map->grids);
        break;
        
    case 'S':                       /* SCATTERS */
        *file_struct = &(map->scatters);
        break;
        
    case 'P':                       /* PERSPECTIVES */
        *file_struct = &(map->perspectives);
        break;
        
    default:
        status = FAIL;
        }
    return status;
    }
/* END:     */
