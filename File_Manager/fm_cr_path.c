/* DEC/CMS REPLACEMENT HISTORY, Element FM_CR_PATH.C*/
/* *4    14-AUG-1990 13:24:37 VINCE "(SPR 5644) Header Standardization"*/
/* *3    29-NOV-1989 07:29:33 GORDON "(SPR 0) fix include line syntax"*/
/* *2     7-NOV-1989 13:23:21 GILLESPIE "(SPR 30) Use esi_fm_int.h include file"*/
/* *1    19-JUN-1989 12:50:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FM_CR_PATH.C*/
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
#include "esi_fm_int.h"

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to create and return the full OS pathname.
    Specification for the file with the given id and Finder
    path specification.

Prototype:
    publicdef INT fm_create_full_path(INT file_id,FM_PATH fi_path,FILENAME os_path);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    file_id         -(INT)
    fi_path         -(FM_PATH)
    os_path         -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fm_create_full_path (INT file_id, FM_PATH fi_path, FILENAME os_path)
#else
publicdef INT fm_create_full_path (file_id, fi_path, os_path)
INT file_id;
FM_PATH fi_path;
FILENAME os_path;
#endif
    {
    FILENAME temp;
    
    /**************************************************************************/
    
    fm_create_os_filename (file_id, temp);
    if (IS_STRING (fi_path))
        {
        sprintf (os_path, "%s:", fi_path);
        strcat (os_path, temp);
        }
    else
        {
        strcpy (os_path, temp);
        }
    ho_translate_filename (os_path, os_path);
    
    return SUCCESS;
    }
/* END:     */
