/* DEC/CMS REPLACEMENT HISTORY, Element OV_GSCATTHORZ.C*/
/* *3    25-JUL-1990 19:59:09 VINCE "(SPR 5644) Header Standardization"*/
/* *2     8-NOV-1989 09:58:49 JULIAN "(SPR 30) fix casting"*/
/* *1    19-JUN-1989 13:21:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_GSCATTHORZ.C*/
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
#include "esi_ov_defs.h"
#include "esi_ov.h"
#include "esi_nl.h"

/* Function Description -----------------------------------------------------
Description:
    Function place the name of the horizons being used to
    create this scatter file into the user_struct.

Prototype:
    publicdef INT ov_get_scatter_horizons(NLIST_HEADER scatter,CHAR *base,CHAR *top);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    scatter         -(NLIST_HEADER) File pointer to return.
    base            -(CHAR *)       
    top             -(CHAR *)   

Return Value/Status:
    Function returns status from nl_inq_nlist_user_struct.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_get_scatter_horizons (NLIST_HEADER scatter, CHAR *base, CHAR *top)
#else
publicdef INT ov_get_scatter_horizons (scatter, base, top)
NLIST_HEADER scatter;
CHAR *base;
CHAR *top;
#endif
    {
    
    INT status;                     /* INTERNAL STATUS */
    OV_SCATTER_USER scatter_user;
    INT n;
    
    /* ********************************************************************** */
    
    if ((status = nl_inq_nlist_user_struct (scatter, sizeof(OV_SCATTER_USER),
                                            (VOIDPTR) & scatter_user,
                                            (size_t *) & n)) EQUALS SUCCESS)
        {
        strcpy (base, scatter_user.base_surface);
        strcpy (top, scatter_user.top_surface);
        }
    return status;
    }
/* END:     */
