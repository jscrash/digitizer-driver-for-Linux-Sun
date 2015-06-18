/* DEC/CMS REPLACEMENT HISTORY, Element S3_OVERLAY.C*/
/* *4    12-OCT-1990 23:08:55 GILLESPIE "(SPR 100) Merge Ernie deltas"*/
/*  2B1  12-OCT-1990 17:48:27 GILLESPIE "Merge Ernie Deltas"*/
/* *3    17-AUG-1990 22:13:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JUN-1990 12:06:37 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *1    19-JUN-1989 13:23:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_OVERLAY.C*/
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

#include "esi_s3.h"

#ifndef ESI_AM_H

#include "esi_am.h"         /* am routines          */

#endif

#ifndef ESI_CS_H

#include "esi_cs.h"         /* centered symbol keywords     */

#endif

#ifndef ESI_OV_H

#include "esi_ov.h"         /* overlay routines         */

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Main entry point for processing three-dimensional seismic mapping               

    Allocates the S3 structure, processes the keywords,                
    sets up the file manager calls for scatterfiles, opens the overlay,              
    and calls the overlay handler.                                                   

Prototype:
    publicdef INT s3_overlay(INT display_line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT) From mapping.                  

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT s3_overlay (INT display_line)
#else
publicdef INT s3_overlay (display_line)
INT display_line;
#endif
    {
    
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    
    INT i;
    INT row;
    INT segment;
    
    S3_STRUCT *ps3;                 /* seismic overlay structure                    */
    
    INT status;                     /* status code                                  */
    
    status = SUCCESS;
    
    /* Allocate structure                                                           */
    
    ps3 = (S3_STRUCT *)am_allocate (AM_GLOBAL, sizeof(S3_STRUCT));
    if (ps3 == (S3_STRUCT *)0)
        {
        return FAIL;
        }
    /* determine overlay type                                                       */
    
    mp_get_current_map (&mapdef);
    
    ps3->map = mapdef;
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];
    
    /* set up some defaults                                                         */
    
    ps3->seismic_generic.draw_shotpoint_label = TRUE;
    ps3->seismic_generic.draw_shotpoint_symbol = TRUE;
    ps3->seismic_generic.draw_seismic_line = TRUE;
    ps3->seismic_generic.draw_seismic_label = TRUE;
    ps3->seismic_generic.xyzfile_append = FALSE;
    ps3->seismic_generic.do_scatter = FALSE;
    ps3->seismic_generic.exclusive_fault = FALSE;
    ps3->seismic_generic.horizon_only_on_drawn = FALSE;
    ps3->seismic_surface.post_at_fault = FALSE;
    ps3->zzzz = FALSE;
    strcpy (ps3->seismic_generic.shotpoint_xsymbol.symbol_group, CS_POLYMARKER_GROUP);
    strcpy (ps3->seismic_generic.shotpoint_ysymbol.symbol_group, CS_POLYMARKER_GROUP);
    strcpy (ps3->s3_current_survey, "");
    ps3->seismic_generic.seismic_xline.line.style = 1;
    ps3->seismic_generic.seismic_xline.line.width = 1.0;
    ps3->seismic_generic.seismic_yline.line.style = 1;
    ps3->seismic_generic.seismic_yline.line.width = 1.0;
    
    /* fill in SO_STRUCT                                                            */
    
    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];
            
            switch (verbstruc->key_code[i])
                {
            case 1:                 /* ADD TO XYZ FILE                              */
                ps3->seismic_generic.xyzfile_append = TRUE;
                ps3->seismic_generic.xyzfile = parmstruc->parmval.cval;
                break;
                
            case 2:                 /* BEST SOURCES                                 */
                ps3->generic.best_sources = parmstruc;
                break;
                
            case 3:                 /* EXCLUSIVE FAULT                              */
                ps3->seismic_generic.exclusive_fault = TRUE;
                break;
                
            case 4:                 /* FAULT COLOR                                  */
                ps3->seismic_surface.fault_line.line.color =
                                                             ps3->seismic_surface.
                                                             fault_label.text_color =
                                                             parmstruc->parmval.ival;
                break;
                
            case 5:                 /* FAULT LABEL SIZE                             */
                ps3->seismic_surface.fault_label.text_size = parmstruc->parmval.dbval;
                break;
                
                /* post depth/time at fault                                         */
            case 6:                 /* FAULTS                                       */
                ps3->seismic_surface.post_at_fault = TRUE;
                break;
                
            case 7:                 /* HORIZON                                      */
                ps3->seismic_surface.horizon = parmstruc->parmval.cval;
                break;
                
            case 8:                 /* HORIZON LABEL SIZE                           */
                ps3->seismic_generic.zvalue_label.text_size = parmstruc->parmval.dbval;
                break;
                
            case 9:                 /* HORIZON ONLY ON DRAWN                        */
                ps3->seismic_generic.horizon_only_on_drawn = TRUE;
                break;
                
            case 10:                /* NAME                                         */
                ps3->generic.name = parmstruc->parmval.cval;
                break;
                
            case 11:                /* SEISMIC LINE ONLY                            */
                ps3->seismic_generic.draw_seismic_label =
                                                          ps3->seismic_generic.
                                                          draw_shotpoint_symbol =
                                                          ps3->seismic_generic.
                                                          draw_shotpoint_label = FALSE;
                break;
                
            case 12:                /* SELECT BY PHRASE                             */
                ps3->generic.select_by_phrase = parmstruc->parmval.cval;
                break;
                
            case 13:                /* SELECT LIST                                  */
                ps3->generic.select_list = parmstruc->parmval.cval;
                break;
                
            case 14:                /* SHOT LABEL SIZE                              */
                ps3->seismic_generic.shotpoint_label.text_size =
                                                                 parmstruc->parmval.
                                                                 dbval;
                break;
                
            case 15:                /* SUPPRESS SHOT LABELS                         */
                ps3->seismic_generic.draw_shotpoint_label = FALSE;
                break;
                
            case 16:                /* SUPPRESS SHOT LINE                           */
                ps3->seismic_generic.draw_seismic_line = FALSE;
                break;
                
            case 44:                /* VERSION                                      */
                ps3->seismic_surface.version = parmstruc->parmval.dbval;
                break;
                
            case 17:                /* XYZ FILE                                     */
                ps3->seismic_generic.xyzfile_append = FALSE;
                ps3->seismic_generic.xyzfile = parmstruc->parmval.cval;
                break;
                
            case 18:                /* X CDP INCR                                   */
                ps3->disp_opt.x_cdp_incr = parmstruc->parmval.dbval;
                break;
                
            case 19:                /* Y CDP INCR                                   */
                ps3->disp_opt.y_cdp_incr = parmstruc->parmval.dbval;
                break;
                
            case 20:                /* X HORIZ INCR                                 */
                ps3->disp_opt.x_horiz_incr = parmstruc->parmval.dbval;
                break;
                
            case 21:                /* Y HORIZ INCR                                 */
                ps3->disp_opt.y_horiz_incr = parmstruc->parmval.dbval;
                break;
                
            case 22:                /* X HORIZ POST INCR                            */
                ps3->disp_opt.x_horiz_post_incr = parmstruc->parmval.dbval;
                break;
                
            case 23:                /* Y HORIZ POST INCR                            */
                ps3->disp_opt.y_horiz_post_incr = parmstruc->parmval.dbval;
                break;
                
            case 24:                /* X LABEL COLOR                                */
                ps3->seismic_generic.seismic_xlabel1.text_color =
                                                                  ps3->seismic_generic.
                                                                  seismic_xlabel2.
                                                                  text_color =
                                                                  parmstruc->parmval.
                                                                  ival;
                break;
                
            case 25:                /* Y LABEL COLOR                                */
                ps3->seismic_generic.seismic_ylabel1.text_color =
                                                                  ps3->seismic_generic.
                                                                  seismic_ylabel2.
                                                                  text_color =
                                                                  parmstruc->parmval.
                                                                  ival;
                break;
                
            case 26:                /* X LABEL SIZE                                 */
                ps3->seismic_generic.seismic_xlabel1.text_size =
                                                                 ps3->seismic_generic.
                                                                 seismic_xlabel2.
                                                                 text_size =
                                                                 parmstruc->parmval.
                                                                 dbval;
                break;
                
            case 27:                /* Y LABEL SIZE                                 */
                ps3->seismic_generic.seismic_ylabel1.text_size =
                                                                 ps3->seismic_generic.
                                                                 seismic_ylabel2.
                                                                 text_size =
                                                                 parmstruc->parmval.
                                                                 dbval;
                break;
                
            case 28:                /* X LINE COLOR                                 */
                ps3->seismic_generic.seismic_xline.line.color =
                    parmstruc->parmval.ival;
                break;
                
            case 29:                /* Y LINE COLOR                                 */
                ps3->seismic_generic.seismic_yline.line.color =
                    parmstruc->parmval.ival;
                break;
                
            case 30:                /* X LINE STYLE                                 */
                ps3->seismic_generic.seismic_xline.line.style =
                    parmstruc->parmval.ival;
                break;
                
            case 31:                /* Y LINE STYLE                                 */
                ps3->seismic_generic.seismic_yline.line.style =
                    parmstruc->parmval.ival;
                break;
                
            case 32:                /* X LINE INCR                                  */
                ps3->disp_opt.x_line_plot_incr = parmstruc->parmval.dbval;
                break;
                
            case 33:                /* Y LINE INCR                                  */
                ps3->disp_opt.y_line_plot_incr = parmstruc->parmval.dbval;
                break;
                
            case 34:                /* X SYMBOL COLOR                               */
                ps3->seismic_generic.shotpoint_xsymbol.symbol_color =
                                                                      parmstruc->
                                                                      parmval.ival;
                break;
                
            case 35:                /* Y SYMBOL COLOR                               */
                ps3->seismic_generic.shotpoint_ysymbol.symbol_color =
                                                                      parmstruc->
                                                                      parmval.ival;
                break;
                
            case 36:                /* X SYM POST INCR                              */
                ps3->disp_opt.x_symb_line_plot_incr = parmstruc->parmval.dbval;
                break;
                
            case 37:                /* Y SYM POST INCR                              */
                ps3->disp_opt.y_symb_line_plot_incr = parmstruc->parmval.dbval;
                break;
                
            case 38:                /* X SYMBOL INCR                                */
                ps3->disp_opt.x_symb_incr = parmstruc->parmval.dbval;
                break;
                
            case 39:                /* Y SYMBOL INCR                                */
                ps3->disp_opt.y_symb_incr = parmstruc->parmval.dbval;
                break;
                
            case 40:                /* X SYMBOL SIZE                                */
                ps3->seismic_generic.shotpoint_xsymbol.symbol_size =
                                                                     parmstruc->
                                                                     parmval.dbval;
                break;
                
            case 41:                /* Y SYMBOL SIZE                                */
                ps3->seismic_generic.shotpoint_ysymbol.symbol_size =
                                                                     parmstruc->
                                                                     parmval.dbval;
                break;
                
            case 42:                /* X SYMBOL TYPE                                */
                ps3->seismic_generic.shotpoint_xsymbol.symbol_type =
                                                                     parmstruc->
                                                                     parmval.ival;
                break;
                
            case 43:                /* Y SYMBOL TYPE                                */
                ps3->seismic_generic.shotpoint_ysymbol.symbol_type =
                                                                     parmstruc->
                                                                     parmval.ival;
                break;
                
                }
            }
        }
    /* open the overlay                                                             */
    
    status = ov_open_overlay (display_line, ps3->generic.name);
    
    /* good open, so process                                                        */
    
    if (status == SUCCESS)
        {
        if (ps3->generic.best_sources !=
            (MD_PARM_STRUCT *)0 AND ps3->generic.best_sources->parmval.cval !=
            (CHAR *)0)
            {
            strcpy (ps3->seismic_generic.source,
                    ps3->generic.best_sources->parmval.cval);
            }
        else
            {
            strcpy (ps3->seismic_generic.source, "");
            }
        /* FOR NOW  - switch to wm_class_seismic_3d when finished                   */
#if 0
        wmsallocate (&segment);
        wmoset (segment, WM_CLASS_SEISMIC, ps3->s3_current_survey,
                ps3->s3_current_survey);
#endif
        status = s3_process_overlay (ps3);  /* lets go to work                      */
        
        /* FOR NOW                                                                  */
#if 0
        wmsclose (segment);
#endif
        ov_close_overlay (display_line);    /* and now we're done                       */
        
        }
    return status;
    }
/* END:     */
