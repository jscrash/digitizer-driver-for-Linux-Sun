/* DEC/CMS REPLACEMENT HISTORY, Element GR_OVERLAY.C*/
/* *2    14-AUG-1990 13:29:54 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:53:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GR_OVERLAY.C*/
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

#include "esi_gl_defs.h"
#include "esi_md_defs.h"
#include "esi_gks.h"
#include "esi_ov.h"                    /* overlay stuff */
#include "esi_ov_defs.h"               /* for overlay information */
#include "esi_gr.h"                    /* graticule overlay defs */
#include "esi_gr_err.h"                /* error codes for gr */
#include "esi_mp.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Create and post graticule for landgrid.
    A graticule consists of labels,tics,index points on
    an overlay.

Prototype:
    publicdef INT gr_overlay(INT display_line);

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
publicdef INT gr_overlay (INT display_line)
#else
publicdef INT gr_overlay (display_line)
INT display_line;
#endif
    {
    INT status = SUCCESS;
    GRAT_OV_STRUCT graticule;       /* structure of graticule info */
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT row;
    INT i;
    
    /* * INITIALIZE GRATICULE STRUCTURE * */
    /* ---------------------------------- */
    
    tc_zeroes (&graticule, sizeof(GRAT_OV_STRUCT));
    
    /* * FILL GRATICULE STRUCTURE * */
    /* ---------------------------- */
    
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
            case 1:                 /* COLOR */
                graticule.color = parmstruc->parmval.ival;
                break;
                
            case 2:                 /* INDEX POINT SIZE */
                graticule.index_point_size = parmstruc->parmval.dbval;
                break;
                
            case 3:                 /* INDEX POINTS */
                graticule.index_points = TRUE;
                break;
                
            case 4:                 /* LABEL SIZE */
                graticule.label_size = parmstruc->parmval.dbval;
                break;
                
            case 5:                 /* LABEL COLOR */
                graticule.label_color = parmstruc->parmval.ival;
                break;
                
            case 6:                 /* NAME */
                graticule.name = parmstruc->parmval.cval;
                break;
                
            case 7:                 /* X TICK INTERVAL */
                graticule.x_tic_interval = parmstruc->parmval.cval;
                break;
                
            case 8:                 /* Y TICK INTERVAL */
                graticule.y_tic_interval = parmstruc->parmval.cval;
                break;
                
            case 9:                 /* UNIT OF MEASURE */
                graticule.uom = parmstruc->parmval.cval;
                break;
                }
            }
        }
    /* now that the structure has been filled, post graticule overlay */
    
    ov_open_overlay (display_line, graticule.name);
    status = gr_post_grat (&graticule);
    ov_close_overlay (display_line);
    
    return(status);
    }
/* END:     */
