/* DEC/CMS REPLACEMENT HISTORY, Element LY_G_DEF_GC.C */
/* *7     6-NOV-1990 15:30:43 JULIAN "(SPR 6156) township & section labels mods" */
/*  4B1   3-OCT-1990 11:43:40 GILLESPIE "Change file type to stream" */
/* *6    20-AUG-1990 17:07:51 KEE "(SPR -1) fixing compile error at sun" */
/* *5    14-AUG-1990 17:25:28 GILLESPIE "(SPR 2001) Use LINE_GC" */
/* *4    23-JUL-1990 13:18:39 VINCE "(SPR 5628) Make LYNX functions internally consistant" */
/* *3    27-MAR-1990 14:09:43 JULIAN "(SPR 1) Fixup topologic stuff" */
/* *2    26-MAR-1990 20:54:51 VINCE "(SPR 1) Initial checkin" */
/* *1    26-MAR-1990 20:52:25 VINCE "fill a GC structure with the default context" */
/* DEC/CMS REPLACEMENT HISTORY, Element LY_G_DEF_GC.C */
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
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ly_get_default_gc (VOIDPTR gc_ptr, CHAR   *type);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    gc_ptr          -(VOIDPTR)
    type            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ly_get_default_gc (VOIDPTR gc_ptr, CHAR *type) 
#else
publicdef INT ly_get_default_gc (gc_ptr, type)
VOIDPTR gc_ptr;
CHAR *type;
#endif
    {
    if (ARE_SAME (type, "TEXT"))
        {
        TEXT_GC *text_ptr = (TEXT_GC *)gc_ptr;
        
        text_ptr->text_bundle_id = 0;
        lu_text_to_index("COLOR", (INT *)&text_ptr->text_color, "NORMAL");
        lu_text_to_index("FONT", (INT *)&text_ptr->text_font,"SIMPLEX");
        text_ptr->text_precision = GSTRKP;
        text_ptr->text_path = GRIGHT;
        text_ptr->string_h_just = GACENT;
        text_ptr->string_v_just = GAHALF;
        text_ptr->string_spacing = 1.0;
        text_ptr->char_height = 0.10;
        text_ptr->char_spacing = 0.0;
        return SUCCESS;
        }
    else if (ARE_SAME (type, "LINE"))
        {
        LINE_GC *line_ptr = (LINE_GC *)gc_ptr;
        
        line_ptr->line_bundle_id = 0;
        lu_text_to_index("COLOR", (INT *)&line_ptr->line.color, "NORMAL");
        lu_text_to_index("LINE_STYLE", (INT *)&line_ptr->line.style,"SOLID");
        line_ptr->line.width = 1.0;
        return SUCCESS;
        }
    else if (ARE_SAME (type, "SYMBOL"))
        {
        return FAIL;
        }
    else if (ARE_SAME (type, "AREA"))
        {
        return FAIL;
        }
    else
        return FAIL;
    
    }
/* END:     */
