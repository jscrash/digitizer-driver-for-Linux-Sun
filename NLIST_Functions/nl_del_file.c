/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_FILE.C*/
/* *2    25-JUL-1990 14:57:37 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:03 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_FILE.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_NLFILE.C*/
/* *3    20-SEP-1989 16:26:36 GILLESPIE "(SPR 100) Guld mods"*/
/* *2    18-SEP-1989 17:46:29 GILLESPIE "(SPR 100) Gulf mods"*/
/* *1    19-JUN-1989 13:18:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_NLFILE.C*/
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

#include "esi_nl_int.h"
#include "esi_nl_io.h"

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To delete an existing nlist file.  Because the new nlist
    module always keeps the last five opened nlist file open, if 
    we want to delete a specfic nlist file, we have to close from 
    the nlist internal tree first.

Prototype:
    publicdef INT nl_delete_nlist_file(FILENAME in_filename);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_NOT_FOUND
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_delete_nlist_file (FILENAME in_filename)
#else
publicdef INT nl_delete_nlist_file (in_filename)
FILENAME in_filename;
#endif
    {
    FILENAME filename;
    INT status;
    INT target;
    
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return(NL_FILE_NOT_FOUND);
        }
    target = nlz_get_target (filename);
    if (target >= 0)
        {
        status = nlz_delete_index (target);
        }
    ho_delete_file (filename, &status);
    return status;
    }
/* END:     */
