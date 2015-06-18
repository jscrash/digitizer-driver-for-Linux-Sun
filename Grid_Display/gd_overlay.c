/* DEC/CMS REPLACEMENT HISTORY, Element GD_OVERLAY.C*/
/* *3    19-NOV-1990 14:51:09 JULIAN "(SPR 6171) add fault source option"*/
/* *2    14-AUG-1990 13:27:33 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:52:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GD_OVERLAY.C*/
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
#include "esi_mp.h"

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_OV_H

#include "esi_ov.h"

#endif

#ifndef ESI_OV_MSG_H

#include "esi_ov_msg.h"

#endif

#ifndef ESI_OV_ERR_H

#include "esi_ov_err.h"

#endif

#ifndef ESI_GD_DEFS_H

#include "esi_gd_defs.h"

#endif

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_CS_H

#include "esi_cs.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to process a "GRID DISPLAY" overlay.

Prototype:
    publicdef INT gd_overlay(INT display_line);

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
publicdef INT gd_overlay (INT display_line)
#else
publicdef INT gd_overlay (display_line)
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
    CONTOURS contours;              /* "CONTOURS" ORGANIZING STRUCT */
    GRID_DISPLAY grid_display;      /* "GRID DISPLAY" ORGANIZING STRUCT */
    SYMBOL symbol;
    INT status;                     /* RETURN STATUS */
    CHAR msg[200];
    
    /***************************************************************************/
    
    /* * INITIALIZE STRUCTURES USED IN OVERLAY * */
    /* ----------------------------------------- */
    
    tc_zeroes (&grid_display, sizeof(GRID_DISPLAY));
    tc_zeroes (&contours, sizeof(CONTOURS));
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
            case 1:                 /* BOUNDED BY DATA */
                contours.bounded_by_data = TRUE;
                break;
                
            case 2:                 /* FAULTED */
                contours.faulted = parmstruc->parmval.cval;
                break;
                
            case 3:
            case 20:                /* GRID FILE */
                contours.grid_file = parmstruc->parmval.cval;
                break;
                
            case 4:                 /* GRID INTERVAL */
                contours.grid_interval = parmstruc->parmval.cval;
                break;
                
            case 5:                 /* GRID MAXIMUM */
                contours.grid_maximum = parmstruc->parmval.cval;
                break;
                
            case 6:                 /* GRID MINIMUM */
                contours.grid_minimum = parmstruc->parmval.cval;
                break;
                
            case 7:                 /* ISOPACH */
                contours.isopach = TRUE;
                break;
                
            case 8:                 /* LABEL POSITION */
                grid_display.label_position = parmstruc->parmval.ival;
                break;
                
            case 9:                 /* NAME */
                grid_display.name = parmstruc->parmval.cval;
                break;
                
            case 10:                /* SAMPLE INTERVAL: COLUMN */
                grid_display.col_int = parmstruc->parmval.dbval;
                break;
                
            case 11:                /* SAMPLE INTERVAL: ROW */
                grid_display.row_int = parmstruc->parmval.dbval;
                break;
                
            case 12:                /* SYMBOL COLOR */
                symbol.symbol_color = parmstruc->parmval.ival;
                break;
                
            case 13:                /* SYMBOL SIZE */
                symbol.symbol_size = parmstruc->parmval.dbval;
                break;
                
            case 14:                /* SYMBOL TYPE */
                symbol.symbol_type = parmstruc->parmval.ival;
                break;
                
            case 15:                /* TEXT COLOR */
                grid_display.text_color = parmstruc->parmval.ival;
                break;
                
            case 16:                /* TEXT SIZE */
                grid_display.text_size = parmstruc->parmval.dbval;
                break;
                
            case 17:                /* XYZ FILE */
                contours.xyz_file = parmstruc->parmval.cval;
                break;
                
            case 18:                /* SUPPRESS SYMBOLS */
                grid_display.suppress_symbols = TRUE;
                break;
                
            case 19:                /* SUPPRESS TEXT */
                grid_display.suppress_text = TRUE;
                break;

	    case 21:			    /* FAULT SOURCE */
		contours.fault_source = parmstruc->parmval.cval;
		break;
                }
            }
        }
    /* * PRE-PROCESSING: MAKE GRID FILE * */
    /* ---------------------------------- */
    
    if (contours.xyz_file != NULL)
        {
        sprintf (msg, mg_message (OV_GRID_FROM_XYZ), contours.xyz_file);
        am_message (AM_DIALOG, msg);
        status = co_make_grid (&contours);
        am_message (AM_DIALOG, NULL);
        if (status < 0)
            {
            goto finished;
            }
        }
    /* * EXECUTE THE OVERLAY * */
    /* ----------------------- */
    
    /* OPEN THE OVERLAY */
    
    status = ov_open_overlay (display_line, grid_display.name);
    if (status < 0)
        {
        goto finished;
        }
    /* PLOT THE OVERLAY */
    
    sprintf (msg, mg_message (OV_DISP_GRID), contours.grid_file);
    am_message (AM_DIALOG, msg);
    status = gd_post_grid (&grid_display, &contours, &symbol);
    am_message (AM_DIALOG, NULL);
    
    /* CLOSE THE OVERLAY */
    
    ov_close_overlay (display_line);
    
    /* * RETURN * */
    /* ---------- */
    
finished:
    return status;
    }
/* END:     */
