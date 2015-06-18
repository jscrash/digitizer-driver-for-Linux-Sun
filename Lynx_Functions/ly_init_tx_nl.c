/* DEC/CMS REPLACEMENT HISTORY, Element LY_INIT_TX_NL.C */
/* *8    19-JUL-1990 10:19:33 VINCE "(SPR 5628) Make LYNX functions internally consistant" */
/* *7    27-MAR-1990 14:10:01 JULIAN "(SPR 1) Fixup topologic stuff" */
/* *6    25-MAR-1990 14:32:48 VINCE "(SPR 1) fixed setting of draw_text method" */
/* *5    25-MAR-1990 12:45:21 SCD "(SPR 1) Set text drawing method when initializing the nlist." */
/* *4    21-MAR-1990 12:09:41 VINCE "(SPR -1) added setting of column names in nlist" */
/* *3    20-MAR-1990 13:13:28 MING "(SPR 0) mod " */
/* *2    20-MAR-1990 12:23:43 MING "(SPR 0) creat" */
/* *1    20-MAR-1990 12:19:38 MING "lynx text nlist initialization routine" */
/* DEC/CMS REPLACEMENT HISTORY, Element LY_INIT_TX_NL.C */
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

#include "esi_ly.h"

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_NL_METH_HDR_H
#include "esi_nl_meth_hdr.h"
#endif

#ifndef ESI_NL_METHODS_H
#include "esi_nl_methods.h"
#endif

#ifndef ESI_TO_H
#include "esi_to.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Initialize the lynx text nlist, including appropriate methods.

Prototype:
    publicdef INT ly_init_text_nlist (NLIST_HEADER *nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ly_init_text_nlist (NLIST_HEADER *nlist) 
#else
publicdef INT ly_init_text_nlist (nlist)
NLIST_HEADER *nlist;
#endif
    {
    INT status;
    static UINT ndim = 10;
    static UINT dim_list[10] = 
        {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10
        };
    static INT units[10] = 
        {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
        };
    static INT dim_type[10] = 
        {
        INT_TYPE,                   /* text id          */
        CHAR_TYPE,                  /* text string      */
        FLOAT_TYPE,                 /* node_x           */
        FLOAT_TYPE,                 /* node_y           */
        FLOAT_TYPE,                 /* x offset         */
        FLOAT_TYPE,                 /* y offset         */
        INT_TYPE,                   /* offset unit      */
        FLOAT_TYPE,                 /* text_up_x        */
        FLOAT_TYPE,                 /* text_up_y        */
        INT_TYPE                    /* text_bundle_id   */
        };
    
    static size_t dim_size[10] = 
        {
        sizeof(INT), 80 * sizeof(CHAR), sizeof(DOUBLE), sizeof(DOUBLE), sizeof(DOUBLE),
        sizeof(DOUBLE), sizeof(INT), sizeof(DOUBLE), sizeof(DOUBLE), sizeof(INT)
        };
    
    static INT value_inc[10] = 
        {
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        INCREMENT_LIST
        };
    
    static CHAR *dim_names[] = 
        {
        "TEXT_ID",
        "TEXT_STRING",
        "NODE_X",
        "NODE_Y",
        "X_OFFSET",
        "Y_OFFSET",
        "OFFSET_UNIT",
        "TEXT_UP_X",
        "TEXT_UP_Y",
        "TEXT_BUNDLE_ID",
        0
        };
    
    status = nl_init_nlist (nlist, ndim, units, dim_size, dim_type, value_inc, 500);
    
    if (status == SUCCESS)
        {
        status = nl_set_nlist_info (*nlist, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                    (VOIDPTR)dim_names);
        }
    /* ----------------------- */
    /* set text drawing method */
    /* ----------------------- */
    status = nl_set_nlist_method (*nlist, NL_DRAW_METHOD, "TO_DRAW_TEXT_METHOD",
                                  to_draw_text_method);
    
    /* ----------------------- */
    /*    set clip method      */
    /* ----------------------- */
    
    status = nl_set_nlist_method (*nlist, NL_CLIP_METHOD, "NLM_WINDOW_CLIP_TEXT",
                                  nlm_window_clip_text);
    
    return status;
    }
/* END:     */
