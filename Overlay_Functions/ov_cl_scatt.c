/* DEC/CMS REPLACEMENT HISTORY, Element OV_CL_SCATT.C*/
/* *2    25-JUL-1990 19:59:05 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:21:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_CL_SCATT.C*/
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
#include "esi_ov.h"
#include "esi_fm.h"

/* Function Description -----------------------------------------------------
Description:
    Function to close the supplied scatter file.

Prototype:
    publicdef INT ov_close_scatter(NLIST_HEADER scatter,FM_LONG name,CHAR *mode,
        FILENAME os_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    scatter         -(NLIST_HEADER) File to close.
    name            -(FM_LONG)
    mode            -(CHAR *)
    os_name         -(FILENAME)

Return Value/Status:
    Function returns status from nl_write_nlist and fm_release_file.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_close_scatter (NLIST_HEADER scatter, FM_LONG name, CHAR *mode,
                                FILENAME os_name)
#else
publicdef INT ov_close_scatter (scatter, name, mode, os_name)
NLIST_HEADER scatter;
FM_LONG name;
CHAR *mode;
FILENAME os_name;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    
    /* ********************************************************************** */
    
    if (strcmp (mode, "r") != 0)
        {
        status = nl_write_nlist (scatter, os_name, "SCATTER");
        }
    nl_free_nlist (scatter);
    status = fm_release_file ("ESI$SCATTERS", name, "SCATTER");
    
    return status;
    }
/* END:     */
