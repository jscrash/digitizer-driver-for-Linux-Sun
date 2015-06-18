/* DEC/CMS REPLACEMENT HISTORY, Element SO_OVERLAY.C*/
/* *21    6-JUN-1991 17:40:45 MING "(SPR 0) change seismic time overlay to seismic surface overlay"*/
/* *20   30-OCT-1990 16:48:34 GILLESPIE "(SPR 37) Restore last lost checkin"*/
/* *19   30-OCT-1990 11:53:54 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *18   24-SEP-1990 17:24:36 GILLESPIE "Merge: Beta Deltas"*/
/*  15B1 24-SEP-1990 17:23:59 GILLESPIE "Beta Deltas"*/
/* *17    4-SEP-1990 13:12:34 GILLESPIE "Incorporate Ernie changes into latest gen"*/
/*  15A1  4-SEP-1990 13:06:55 GILLESPIE "Seismic overlay enhancements from Ernie"*/
/* *16   23-AUG-1990 17:20:19 JESSIE "(SPR 1) fix include files"*/
/* *15    1-AUG-1990 13:18:39 VINCE "(SPR 5615) updated for integer line id"*/
/* *14   26-JUL-1990 17:21:42 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *13   25-JUL-1990 20:05:20 VINCE "(SPR 5644) Header Standardization"*/
/* *12    1-JUL-1990 12:31:57 GILLESPIE "(SPR 1) Changes to allow ANY surface type - Phase 1"*/
/* *11   29-JUN-1990 21:25:09 GILLESPIE "(SPR 1) One boo-boo from last checkin"*/
/* *10   29-JUN-1990 19:04:21 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *9    15-JAN-1990 08:11:44 GORDON "(SPR 0) Fix castings"*/
/* *8     2-NOV-1989 09:54:16 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *7    24-OCT-1989 09:39:33 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *6     6-SEP-1989 09:26:56 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *5    11-AUG-1989 20:21:54 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *4    28-JUL-1989 18:12:03 CHARLIE "(SPR 0) add line_label_annotation to 2d seismic overlays"*/
/* *3    25-JUL-1989 20:07:16 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:02:43 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:27:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_OVERLAY.C*/
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
#include "esi_so.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_CS_H

#include "esi_cs.h"

#endif

#ifndef ESI_OV_H

#include "esi_ov.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_MD_DBDEFS_H

#include "esi_md_dbdefs.h"

#endif

#ifndef ESI_SO_H
#include "esi_so.h"
#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to process a seismic overlay.

Prototype:
    publicdef INT so_overlay(INT display_line);

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
publicdef INT so_overlay (INT display_line)
#else
publicdef INT so_overlay (display_line)
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
    SO_STRUCT *pso;                 /* seismic overlay structure */
    INT status;                     /* status code */
    PICKABLE_SEISMIC **pick_seis;   /* picking structures */
    INT ncells;                     /* number of cells in GIS */
    SX_WORKSPACE *w;
    CHAR mode[2];
    FILENAME os_name;
    CHAR *dum1 = NULL;
    CHAR *dum2 = NULL;
    CHAR desc[241];

    /* the order of the surface data type is from esi_se.h */
    /* if things change in SEIS_SURFACE_CODES, this codes  */
    /* need to be changed */
    static CHAR *surface_data_type_codes[] = {"TIME",
						  "DEPTH",
						  "AMPLITUDE",
						  "VELOCITY STACKING",
						  "PHASE",
						  "TIME REFINED",
						  "UNKNOWN",
						  "TIME UNSNAPPED",
						  "VELOCITY RMS",
						  "VELOCITY AVERAGE",
						  "VELOCITY INTERVAL",
						  "FREQUENCY",
						  0};

    
    /*  Start workspace for holding common nlists for software picking */
    
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *)&w);
    
    if (status != SUCCESS)
        {
        status = am_define_workspace ("SX_STATIC_SPACE", AM_GLOBAL,
                                      sizeof(SX_WORKSPACE), (VOIDPTR *)&w);
        }
    w->changed = TRUE;
    
    status = SUCCESS;
    
    /* Allocate structure */
    
    pso = (SO_STRUCT *) am_allocate (AM_GLOBAL, sizeof(SO_STRUCT));
    if (pso == (SO_STRUCT *)0)
        {
        return FAIL;
        }
    /* determine overlay type */
    
    mp_get_current_map (&mapdef);
    
    pso->map = (VOIDPTR)mapdef;
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];
    
    switch (compiled->verb_code[row])
        {
    case MP_SEISMIC:
        pso->ov_type = SO_SEISMIC;
        break;
        
    case MP_SEISDPTH:
        pso->ov_type = SO_SEISMIC_DEPTH;
        break;
        
    case MP_SEISCHRN:
        pso->ov_type = SO_SEISMIC_ISOCHRON;
        break;
        
    case MP_SEISPACH:
        pso->ov_type = SO_SEISMIC_ISOPACH;
        break;
        
    case MP_MISTIE:
        pso->ov_type = SO_SEISMIC_MISTIE;
        break;
        
    case MP_SEISDATA:
        pso->ov_type = SO_SEISMIC_DATA;
        break;
        }
    /* set up some defaults */
    
    pso->data_source = SO_INTERPRET;
    pso->seismic_generic.line_label_annotation = SO_LINE_ONLY;
    pso->seismic_generic.draw_shotpoint_label = TRUE;
    pso->seismic_generic.draw_shotpoint_symbol = TRUE;
    pso->seismic_generic.draw_seismic_line = TRUE;
    pso->seismic_generic.draw_seismic_label = TRUE;
    pso->seismic_generic.draw_endpoints_only = FALSE;
    pso->seismic_generic.xyzfile_append = FALSE;
    pso->seismic_generic.do_scatter = FALSE;
    pso->seismic_generic.draw_horz_terminator = FALSE;
    pso->seismic_generic.enhanced_shot_label_option = FALSE;
    pso->seismic_generic.surface_data_type = SE_TIME;
    pso->draw_fault_throw = FALSE;
    pso->zzzz = FALSE;
    pso->seismic_generic.shotpoint_symbol.symbol_color = 2;
    pso->psx = (VOIDPTR)0;
    /* ------------------------------------ */
    /* The following defaults should be set */
    /* by the overlay syntax defaults       */
    /* ------------------------------------ */
    pso->disp_opt.shot_symbol_int = 50;
    pso->disp_opt.shot_label_int = 50;
    pso->disp_opt.data_label_int = 50;
    
    strcpy ((char *)pso->seismic_mistie.default_direction, "NORTH");
    strcpy ((char *)pso->seismic_generic.shotpoint_symbol.symbol_group,
            CS_POLYMARKER_GROUP);
    
    /* fill in SO_STRUCT */
    
    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];
            
            switch (verbstruc->key_code[i])
                {
            case 1:                 /* ADD TO XYZ FILE */
                pso->seismic_generic.xyzfile_append = TRUE;
                strcpy ((char *)pso->seismic_generic.scatter_filename,
                        (char *)parmstruc->parmval.cval);
                break;
                
            case 2:                 /* FAULT COLOR */
                pso->fault_line.line.color = pso->fault_label.text_color =
                    parmstruc->parmval.ival;
                break;
                
            case 3:                 /* FAULT LABEL SIZE */
                pso->fault_label.text_size = parmstruc->parmval.dbval;
                break;
                
            case 4:                 /* FAULTS */
                pso->draw_fault_throw = TRUE;
                break;
                
            case 5:                 /* HORIZON */
                strcpy ((char *)pso->seismic_surface.name,
                        (char *)parmstruc->parmval.cval);
                break;
                
            case 6:                 /* LINE LABEL COLOR */
                pso->seismic_generic.seismic_label1.text_color =
                    pso->seismic_generic.seismic_label2.text_color =
                    parmstruc->parmval.ival;
                break;
                
            case 7:                 /* LINE LABEL FONT */
                pso->seismic_generic.seismic_label1.text_font =
                    pso->seismic_generic.seismic_label2.text_font =
                    parmstruc->parmval.ival;
                break;
                
            case 8:                 /* LINE LABEL SIZE */
                pso->seismic_generic.seismic_label1.text_size =
                    pso->seismic_generic.seismic_label2.text_size =
                    parmstruc->parmval.dbval;
                break;
                
            case 9:                 /* NAME */
                pso->generic.name = parmstruc->parmval.cval;
                break;
                
            case 10:                /* SEISMIC LINE COLOR */
                pso->seismic_generic.seismic_line.line.color = parmstruc->parmval.ival;
                break;
                
            case 11:                /* SEISMIC LINE ONLY */
                pso->seismic_generic.draw_shotpoint_symbol = FALSE;
                pso->seismic_generic.draw_shotpoint_label = FALSE;
                break;
                
            case 12:                /* SEISMIC LINE STYLE */
                pso->seismic_generic.seismic_line.line.style = parmstruc->parmval.ival;
                break;
                
            case 13:                /* SEISMIC LINE THICKNESS */
                pso->seismic_generic.seismic_line.line.width = parmstruc->parmval.ival;
                break;
                
            case 14:                /* SELECT BY PHRASE */
                pso->generic.select_by_phrase = parmstruc->parmval.cval;
                break;
                
            case 15:                /* SELECT LIST */
                pso->generic.select_list = parmstruc->parmval.cval;
                break;
                
            case 16:                /* SHOTPOINT LABEL COLOR */
                pso->seismic_generic.shotpoint_label.text_color =
                    parmstruc->parmval.ival;
                break;
                
            case 17:                /* SHOTPOINT LABEL FONT */
                pso->seismic_generic.shotpoint_label.text_font =
                    parmstruc->parmval.ival;
                break;
                
            case 18:                /* SHOTPOINT LABEL SIZE */
                pso->seismic_generic.shotpoint_label.text_size =
                    parmstruc->parmval.dbval;
                break;
                
            case 19:                /* SHOTPOINT SYMBOL SIZE */
                pso->seismic_generic.shotpoint_symbol.symbol_size =
                    parmstruc->parmval.dbval;
                break;
                
            case 20:                /* SHOTPOINT SYMBOL TYPE */
                pso->seismic_generic.shotpoint_symbol.symbol_type =
                    pso->disp_opt.shot_symbol_code = parmstruc->parmval.ival;
                break;
                
            case 21:                /* SUPPRESS SHOT LABELS */
                pso->seismic_generic.draw_shotpoint_label = FALSE;
                break;
                
            case 22:                /* SUPPRESS SHOT LINE */
                pso->seismic_generic.draw_seismic_line = FALSE;
                break;
                
            case 23:                /* VERSION */
                pso->seismic_surface.version = parmstruc->parmval.dbval;
                break;
                
            case 24:                /* XYZ FILE */
                pso->seismic_generic.xyzfile_append = FALSE;
                strcpy ((char *)pso->seismic_generic.scatter_filename,
                        (char *)parmstruc->parmval.cval);
                break;
                
            case 25:                /* VELOCITY GRID */
                strcpy ((char *)pso->grid.velocity_filename,
                        (char *)parmstruc->parmval.cval);
                break;
                
            case 26:                /* BOTTOM HORIZON */
                strcpy ((char *)pso->seismic_interval.base_horizon.name,
                        (char *)parmstruc->parmval.cval);
                break;
                
            case 27:                /* TOP HORIZON */
                strcpy ((char *)pso->seismic_interval.top_horizon.name,
                        (char *)parmstruc->parmval.cval);
                break;
                
            case 28:                /* BOTTOM VERSION */
                pso->seismic_interval.base_horizon.version = parmstruc->parmval.dbval;
                break;
                
            case 29:                /* TOP VERSION */
                pso->seismic_interval.top_horizon.version = parmstruc->parmval.dbval;
                break;
                
            case 30:                /* DATUM LINE LIST */
                pso->seismic_mistie.datum_line_list = parmstruc->parmval.cval;
                break;
                
            case 31:                /* TIE LABEL COLOR */
                pso->seismic_mistie.tie_label.text_color = parmstruc->parmval.ival;
                break;
                
            case 32:                /* TIE LABEL FONT */
                pso->seismic_mistie.tie_label.text_font = parmstruc->parmval.ival;
                break;
                
            case 33:                /* TIE LABEL SIZE */
                pso->seismic_mistie.tie_label.text_size = parmstruc->parmval.dbval;
                break;
                
            case 34:                /* BEST SOURCES*/
                pso->generic.best_sources = parmstruc;
                break;
                
            case 35:                /* END POINT ONLY */
                pso->seismic_generic.draw_endpoints_only = TRUE;
                break;
                
            case 36:                /* SHOTPOINT SYMBOL COLOR */
                pso->seismic_generic.shotpoint_symbol.symbol_color =
                    parmstruc->parmval.ival;
                break;
                
            case 37:                /* SHOT LABEL INTERVAL */
                pso->disp_opt.shot_label_int = parmstruc->parmval.dbval;
                if (verbstruc->key_used[i] EQUALS 1)
                    pso->disp_opt.use_map_shot_lbl_parms = TRUE;
                break;
                
            case 38:                /* SHOT LABEL INTERVAL OFFSET */
                pso->disp_opt.shot_label_int_offset = parmstruc->parmval.dbval;
                if (verbstruc->key_used[i] EQUALS 1)
                    pso->disp_opt.use_map_shot_lbl_parms = TRUE;
                break;
                
            case 39:                /* SHOT SYMBOL INTERVAL */
                pso->disp_opt.shot_symbol_int = parmstruc->parmval.dbval;
                if (verbstruc->key_used[i] EQUALS 1)
                    pso->disp_opt.use_map_shot_sym_parms = TRUE;
                break;
                
            case 40:                /* SHOT SYMBOL INTERVAL OFFSET */
                pso->disp_opt.shot_symbol_int_offset = parmstruc->parmval.dbval;
                if (verbstruc->key_used[i] EQUALS 1)
                    pso->disp_opt.use_map_shot_sym_parms = TRUE;
                break;
                
            case 41:                /* DATA LABEL INTERVAL */
                pso->disp_opt.data_label_int = parmstruc->parmval.dbval;
                if (verbstruc->key_used[i] EQUALS 1)
                    pso->disp_opt.use_map_data_lbl_parms = TRUE;
                break;
                
            case 42:                /* DATA LABEL INTERVAL */
                pso->disp_opt.data_label_int_offset = parmstruc->parmval.dbval;
                if (verbstruc->key_used[i] EQUALS 1)
                    pso->disp_opt.use_map_data_lbl_parms = TRUE;
                break;
                
            case 43:                /* LINE LABEL ANNOTATION */
                pso->seismic_generic.line_label_annotation = parmstruc->parmval.ival;
                if (pso->seismic_generic.line_label_annotation == SO_NO_LABEL)
                    pso->seismic_generic.draw_seismic_label = FALSE;
                break;
                /* ------------------------------------ */
                /* (CP) remove conditional compiliation */
                /* when mapping include file is updated */
                /* with the proper key words            */
                /* (and this comment too) (CP)          */
                /* ------------------------------------ */
                
            case 44:                /* HORIZON TERMINATORS */
                pso->seismic_generic.draw_horz_terminator = TRUE;
                break;
                
            case 45:                /* PRECISION POSTING ANGLE */
                pso->seismic_generic.enhanced_shot_label_option = TRUE;
                break;

            case 46:                /* SEISMIC SURFACE DATA TYPE */
		pso->seismic_generic.surface_data_type = 
		  (SEIS_SURFACE_DATA_TYPE) 
		  (ts_switch ((CHAR *) parmstruc->parmval.cval,
		  surface_data_type_codes) + 1);
                break;
                }
            }
        }
    /* open scatter file if necessary */
    
    if (IS_STRING (pso->seismic_generic.scatter_filename))
        {
        if (pso->seismic_generic.xyzfile_append)
            {
            strcpy ((char *)mode, "a");
            strcpy ((char *)desc, "Appended ");
            }
        else
            {
            strcpy ((char *)mode, "w");
            strcpy ((char *)desc, "Created ");
            }
        status = ov_open_scatter (&pso->seismic_generic.scatter,
                                  pso->seismic_generic.scatter_filename, mode,
                                  os_name);
        if (status != SUCCESS)
            {
            return status;
            }
        else
            {
            if (IS_STRING (pso->seismic_surface.name))
                {
                dum1 = pso->seismic_surface.name;
                }
            else
                {
                dum1 = pso->seismic_interval.base_horizon.name;
                dum2 = pso->seismic_interval.top_horizon.name;
                }
            status = ov_set_scatter_horizons (pso->seismic_generic.scatter, dum1,
                                              dum2);
            if (status != SUCCESS)
                {
                return status;
                }
            else
                {
                pso->seismic_generic.do_scatter = TRUE;
                }
            /* end status if */
            
            }
        /* end status if */
        
        }
    /* end IS_STRING if */
    /* open the overlay */
    
    status = ov_open_overlay (display_line, pso->generic.name);
    
    /* good open, so process */
    
    if (status == SUCCESS)
        {
        pso->disp_opt.default_symbol = pso->seismic_generic.shotpoint_symbol;
        if (pso->generic.best_sources !=
            (MD_PARM_STRUCT *)0 AND pso->generic.best_sources->parmval.cval !=
            (CHAR *)0)
            {
            strcpy ((char *)pso->seismic_generic.source,
                    (char *)pso->generic.best_sources->parmval.cval);
            }
        else
            {
            strcpy ((char *)pso->seismic_generic.source, "");
            }
        pick_seis = (PICKABLE_SEISMIC **)(compiled->picks[row]);
        
        /*-------------------------------------------------------------------*/
        /* allocate pointer to array of picking structures for this overlay, */
        /* array will have as many elements as cells in GIS, now only 1      */
        /*-------------------------------------------------------------------*/
        
        ncells = 1;
        if (pick_seis == (PICKABLE_SEISMIC **)NULL)
            {
            pick_seis = (PICKABLE_SEISMIC **) am_allocate (AM_GLOBAL,
                                                     ncells *
                                                           sizeof(PICKABLE_SEISMIC *));
            compiled->picks[row] = (BYTE *)pick_seis;
            }
        /*------------------------------------------------------------------*/
        /* allocate a pointer to picking structure for all cells in GIS,    */
        /* now only one                                                     */
        /*------------------------------------------------------------------*/
        
        if (pick_seis[0] == (PICKABLE_SEISMIC *)NULL)
            {
            pick_seis[0] = (PICKABLE_SEISMIC *) am_allocate (AM_GLOBAL,
                                                             sizeof(PICKABLE_SEISMIC));
            }
        else
            {
            pick_seis[0]->nused = 0;
            }
        status = so_process_overlay (pso, pick_seis);
        ov_close_overlay (display_line);
        }
    /* all done */
    /* close scatter file */
    
    if (pso->seismic_generic.do_scatter)
        {
        ov_close_scatter (pso->seismic_generic.scatter,
                          pso->seismic_generic.scatter_filename, mode, os_name);
        
        sprintf (desc, "in overlay \"%s\" in map \"%s\"", display->verbs[display_line],
                 mapdef->map_name);
        
        fm_add_history_record ("ESI$SCATTERS", pso->seismic_generic.scatter_filename,
                               "SCATTER", "", "", "", desc);
        }
    /*----------------------------------------------------------------------*/
    /* save pso in map structure if plain "SEISMIC" for seismic line editor */
    /*----------------------------------------------------------------------*/
    
    mapdef->pso = (VOIDPTR)pso;
    mp_retrieve_workspace (mapdef->map_number);
    
    return status;
    }
/* END:     */
