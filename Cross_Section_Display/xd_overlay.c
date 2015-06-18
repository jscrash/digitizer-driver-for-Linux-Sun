/* DEC/CMS REPLACEMENT HISTORY, Element XD_OVERLAY.C*/
/* *2    17-AUG-1990 22:32:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:41:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XD_OVERLAY.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_OV_H

#include "esi_ov.h"

#endif

#ifndef ESI_XD_DEFS_H

#include "esi_xd_defs.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_CS_H

#include "esi_cs.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to process a "XYZ FILE DISPLAY" overlay.

Prototype:
    publicdef INT xd_overlay(INT display_line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xd_overlay (INT display_line)
#else
publicdef INT xd_overlay (display_line)
INT display_line;
#endif
    {
    
    PROGNAME ("GD_OVERLAY");
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT i;
    INT row;
    XYZ_DISPLAY xyz_display;        /* "XYZ DISPLAY" ORGANIZING STRUCT */
    SYMBOL symbol;
    INT status;                     /* RETURN STATUS */
    
    /***************************************************************************/
    
    /* * INITIALIZE STRUCTURES USED IN OVERLAY * */
    /* ----------------------------------------- */
    
    tc_zeroes (&xyz_display, sizeof(XYZ_DISPLAY));
    tc_zeroes (&symbol, sizeof(SYMBOL));
    strcpy (symbol.symbol_group, CS_POLYMARKER_GROUP);
    
    /* * FILL IN ORGANIZING STRUCTURES * */
    /* --------------------------------- */
    
    mp_get_current_map (&mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];
    
    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];
            
            switch (verbstruc->key_code[i])
                {
            case 1:                 /* LABEL POSITION */
                xyz_display.label_position = parmstruc->parmval.ival;
                break;
                
            case 2:                 /* NAME */
                xyz_display.name = parmstruc->parmval.cval;
                break;
                
            case 3:                 /* SAMPLE INTERVAL */
                xyz_display.sample_int = parmstruc->parmval.dbval;
                break;
                
            case 4:                 /* SYMBOL COLOR */
                symbol.symbol_color = parmstruc->parmval.ival;
                break;
                
            case 5:                 /* SYMBOL SIZE */
                symbol.symbol_size = parmstruc->parmval.dbval;
                break;
                
            case 6:                 /* SYMBOL TYPE */
                symbol.symbol_type = parmstruc->parmval.ival;
                break;
                
            case 7:                 /* TEXT COLOR */
                xyz_display.text_color = parmstruc->parmval.ival;
                break;
                
            case 8:                 /* TEXT SIZE */
                xyz_display.text_size = parmstruc->parmval.dbval;
                break;
                
            case 9:                 /* XYZ FILE */
                xyz_display.xyz_file = parmstruc->parmval.cval;
                break;
                
            case 10:                /* SUPPRESS SYMBOLS */
                xyz_display.suppress_symbols = TRUE;
                break;
                
            case 11:                /* SUPPRESS TEXT */
                xyz_display.suppress_text = TRUE;
                break;
                }
            }
        }
    /* * EXECUTE THE OVERLAY * */
    /* ----------------------- */
    
    /* OPEN THE OVERLAY */
    
    status = ov_open_overlay (display_line, xyz_display.name);
    if (status < 0)
        {
        goto finished;
        }
    /* PLOT THE OVERLAY */
    
    status = xd_post_scatter (&xyz_display, &symbol);
    
    /* CLOSE THE OVERLAY */
    
    ov_close_overlay (display_line);
    
    /* * RETURN * */
    /* ---------- */
    
finished:
    
    return status;
    }
/* END:     */
