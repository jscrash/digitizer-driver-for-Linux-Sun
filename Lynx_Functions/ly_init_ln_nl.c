/* DEC/CMS REPLACEMENT HISTORY, Element LY_INIT_LN_NL.C */
/* *5    23-JUL-1990 13:24:21 VINCE "(SPR 5628) Make LYNX functions internally consistant" */
/* *4    27-MAR-1990 14:09:53 JULIAN "(SPR 1) Fixup topologic stuff" */
/* *3    20-MAR-1990 13:15:49 MING "(SPR 0) mod " */
/* *2    20-MAR-1990 12:36:54 MING "(SPR 0) create" */
/* *1    20-MAR-1990 12:31:03 MING "to initial lynx line nlist" */
/* DEC/CMS REPLACEMENT HISTORY, Element LY_INIT_LN_NL.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_TO_H

#include "esi_to.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This function initializes the lynx line nlist.

Prototype:
    publicdef INT ly_init_line_nlist(NLIST_HEADER *nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    Status is returned by nl_set_nlist_method.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ly_init_line_nlist (NLIST_HEADER *nlist) 
#else
publicdef INT ly_init_line_nlist (nlist)
NLIST_HEADER *nlist;
#endif
    {
    INT status;
    
    static UINT ndim = 2;
    static INT units[2] = 
        {
        0,
        0
        };
    static INT dim_type[2] = 
        {
        FLOAT_TYPE,
        FLOAT_TYPE
        };
    static size_t dim_size[2] = 
        {
        sizeof(DOUBLE), sizeof(DOUBLE)
        };
    static INT value_inc[2] = 
        {
        VALUE_LIST,
        VALUE_LIST
        };
    
    status = nl_init_nlist (nlist, ndim, units, dim_size, dim_type, value_inc, 500);
    
    /* ----------------------- */
    /* set text drawing method */
    /* ----------------------- */
    
    status = nl_set_nlist_method (*nlist, NL_DRAW_METHOD, "TO_DRAW_LINE_METHOD",
                                  to_draw_line_method);
    
    return status;
    }
/* END:     */
