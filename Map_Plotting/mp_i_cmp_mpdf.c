/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_CMP_MPDF.C*/
/*  *2    17-AUG-1990 22:06:13 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:12:30 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_CMP_MPDF.C*/
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
#include "esi_md_defs.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to allocate and initialize the compiled mapdef
    structure.  Must allocate enough space for "rownum"
    number of rows in the structure.

Prototype:
    publicdef INT mp_init_compiled_mapdef(MD_COMPILED_MAPDEF **compiled,INT rownum);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    compiled        -(MD_COMPILED_MAPDEF **)
    rownum          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_init_compiled_mapdef (MD_COMPILED_MAPDEF **compiled, INT rownum)
#else
publicdef INT mp_init_compiled_mapdef (compiled, rownum)
MD_COMPILED_MAPDEF **compiled;
INT rownum;
#endif
    {
    
    PROGNAME ("MP_COMPILED_SIZE_CHECK");
    INT status;
    MD_COMPILED_MAPDEF *temp_comp;
    INT i;
    INT size;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * ALLOCATE NEW COMPILED STRUCTURE * */
    /* ----------------------------------- */
    
    temp_comp = (MD_COMPILED_MAPDEF *)am_allocate (AM_GLOBAL,
                                                   sizeof(MD_COMPILED_MAPDEF));
    
    for (i = 100; i < rownum; i += 100)
        ;
    
    temp_comp->nrows = i;
    size = i * sizeof(INT);
    
    temp_comp->verb_code = (INT *)am_allocate (AM_GLOBAL, size);
    temp_comp->overlay_wm_id = (INT *)am_allocate (AM_GLOBAL, size);
    temp_comp->wm_arr_ndx = (INT *)am_allocate (AM_GLOBAL, size);
    temp_comp->verb_wm_id = (INT *)am_allocate (AM_GLOBAL, size);
    temp_comp->verb_ptr = (MD_COMPILED_VERB **)am_allocate (AM_GLOBAL,
                                                          i * sizeof(MD_COMPILED_VERB *
                                                            ));
    temp_comp->picks = (BYTE **)am_allocate (AM_GLOBAL, i * sizeof(BYTE *));
    
    *compiled = temp_comp;
    
    return status;
    }
/* END:     */
