/* DEC/CMS REPLACEMENT HISTORY, Element SX_LOAD_PSX.C*/
/* *6    25-SEP-1990 09:53:48 GILLESPIE "(SPR 1) Merge Ernie deltas"*/
/*  4B1  24-SEP-1990 17:29:01 GILLESPIE "Beta Deltas"*/
/* *5    17-AUG-1990 22:21:46 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    29-JUN-1990 21:19:49 GILLESPIE "(SPR 1) Deltas for tumbleweed removal"*/
/* *3    23-OCT-1989 10:37:22 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/* *2    25-JUL-1989 20:12:44 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_LOAD_PSX.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"
#include "esi_sx.h"
#include "esi_am.h"
#include "esi_cs.h"
#include "esi_md_basis.x"

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Create pso struct for new seismic overlay in sle.                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT sx_load_psx(SX_STRUCT *psx, INT *group_id);                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    psx             -(SX_STRUCT *)                                                */
/*    group_id        -(INT *)                                                      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT sx_load_psx (SX_STRUCT *psx, INT *group_id)
#else
publicdef INT sx_load_psx (psx, group_id)
SX_STRUCT *psx;
INT *group_id;
#endif
    {
    INT status;                     /* status code                                  */
    SO_STRUCT *pso;
    MD_VERB_INFO *verb_info;
    MD_KEY_INFO *key_info;
    MD_PARM_STRUCT *parmstruc;
    INT verb_ndx;
    INT i;
    
    /* **********************************************************************       */
    
    status = SUCCESS;
    
    /* Allocate structure                                                           */
    
    pso = psx->pso = (SO_STRUCT *) am_allocate (AM_GLOBAL, sizeof(SO_STRUCT));
    if (psx->pso == (SO_STRUCT *)0)
        {
        SX_RETURN;
        }
    /* determine overlay type                                                       */
    
    pso->ov_type = SO_SEISMIC;
    
    /* set up some defaults                                                         */
    
    strcpy (pso->seismic_generic.source, "");
    pso->data_source = SO_INTERPRET;
    pso->seismic_generic.draw_shotpoint_label = TRUE;
    pso->seismic_generic.draw_shotpoint_symbol = TRUE;
    pso->seismic_generic.draw_seismic_line = TRUE;
    pso->seismic_generic.draw_seismic_label = TRUE;
    pso->seismic_generic.xyzfile_append = FALSE;
    pso->seismic_generic.do_scatter = FALSE;
    pso->draw_fault_throw = FALSE;
    strcpy (pso->seismic_mistie.default_direction, "NORTH");
    strcpy (pso->seismic_generic.shotpoint_symbol.symbol_group, CS_POLYMARKER_GROUP);
    pso->seismic_generic.shotpoint_symbol.symbol_color = 2;
    
    /* get overlay default information                                              */
    
    for (i = 0; i < MD_MAX_OVERLAY_TYPES; i++)
        {
        if (mp_is_seismic_overlay (overlay_verbs[i].verb_code))
            {
            verb_info = &(overlay_verbs[i]);
            verb_ndx = i;
            break;
            }
        }
    for (i = 0; i < verb_info->no_of_keywords; i++)
        {
        key_info = &(verb_info->key_list[i]);
        if (IS_STRING (key_info->key_default))
            {
            mp_parm_ok (key_info->key_default, verb_ndx, i, &parmstruc, &status);
            if (status != SUCCESS)
                {
                SX_RETURN;
                }
            switch (key_info->key_code)
                {
            case 1:                 /* ADD TO XYZ FILE                              */
                pso->seismic_generic.xyzfile_append = TRUE;
                strcpy (pso->seismic_generic.scatter_filename,
                        parmstruc->parmval.cval);
                break;
                
            case 2:                 /* FAULT COLOR                                  */
                pso->fault_line.line.color = pso->fault_label.text_color =
                    parmstruc->parmval.ival;
                break;
                
            case 3:                 /* FAULT LABEL SIZE                             */
                pso->fault_label.text_size = parmstruc->parmval.dbval;
                break;
                
            case 4:                 /* FAULTS                                       */
                pso->draw_fault_throw = TRUE;
                break;
                
            case 5:                 /* HORIZON                                      */
                strcpy (pso->seismic_surface.name, parmstruc->parmval.cval);
                break;
                
            case 6:                 /* LINE LABEL COLOR                             */
                pso->seismic_generic.seismic_label1.text_color =
                    pso->seismic_generic.seismic_label2.text_color =
                    parmstruc->parmval.ival;
                break;
                
            case 7:                 /* LINE LABEL FONT                              */
                pso->seismic_generic.seismic_label1.text_font =
                    pso->seismic_generic.seismic_label2.text_font =
                    parmstruc->parmval.ival;
                break;
                
            case 8:                 /* LINE LABEL SIZE                              */
                pso->seismic_generic.seismic_label1.text_size =
                    pso->seismic_generic.seismic_label2.text_size =
                    parmstruc->parmval.dbval;
                break;
                
            case 9:                 /* NAME                                         */
                pso->generic.name = parmstruc->parmval.cval;
                break;
                
            case 10:                /* SEISMIC LINE COLOR                           */
                pso->seismic_generic.seismic_line.line.color = parmstruc->parmval.ival;
                break;
                
            case 11:                /* SEISMIC LINE ONLY                            */
                pso->seismic_generic.draw_shotpoint_symbol = FALSE;
                pso->seismic_generic.draw_shotpoint_label = FALSE;
                break;
                
            case 12:                /* SEISMIC LINE STYLE                           */
                pso->seismic_generic.seismic_line.line.style = parmstruc->parmval.ival;
                break;
                
            case 13:                /* SEISMIC LINE THICKNESS                       */
                pso->seismic_generic.seismic_line.line.width = parmstruc->parmval.ival;
                break;
                
            case 14:                /* SELECT BY PHRASE                             */
                pso->generic.select_by_phrase = parmstruc->parmval.cval;
                break;
                
            case 15:                /* SELECT LIST                                  */
                pso->generic.select_list = parmstruc->parmval.cval;
                break;
                
            case 16:                /* SHOTPOINT LABEL COLOR                        */
                pso->seismic_generic.shotpoint_label.text_color =
                    parmstruc->parmval.ival;
                break;
                
            case 17:                /* SHOTPOINT LABEL FONT                         */
                pso->seismic_generic.shotpoint_label.text_font =
                    parmstruc->parmval.ival;
                break;
                
            case 18:                /* SHOTPOINT LABEL SIZE                         */
                pso->seismic_generic.shotpoint_label.text_size =
                    parmstruc->parmval.dbval;
                break;
                
            case 19:                /* SHOTPOINT SYMBOL SIZE                        */
                pso->seismic_generic.shotpoint_symbol.symbol_size =
                    parmstruc->parmval.dbval;
                break;
                
            case 20:                /* SHOTPOINT SYMBOL TYPE                        */
                pso->seismic_generic.shotpoint_symbol.symbol_type =
                    pso->disp_opt.shot_symbol_code = parmstruc->parmval.ival;
                break;
                
            case 21:                /* SUPPRESS SHOT LABELS                         */
                pso->seismic_generic.draw_shotpoint_label = FALSE;
                break;
                
            case 22:                /* SUPPRESS SHOT LINE                           */
                pso->seismic_generic.draw_seismic_line = FALSE;
                break;
                
            case 23:                /* VERSION                                      */
                pso->seismic_surface.version = parmstruc->parmval.dbval;
                break;
                
            case 24:                /* XYZ FILE                                     */
                pso->seismic_generic.xyzfile_append = FALSE;
                strcpy (pso->seismic_generic.scatter_filename,
                        parmstruc->parmval.cval);
                break;
                
            case 25:                /* VELOCITY GRID                                */
                strcpy (pso->grid.velocity_filename, parmstruc->parmval.cval);
                break;
                
            case 26:                /* BOTTOM HORIZON                               */
                strcpy (pso->seismic_interval.base_horizon.name,
                        parmstruc->parmval.cval);
                break;
                
            case 27:                /* TOP HORIZON                                  */
                strcpy (pso->seismic_interval.top_horizon.name,
                        parmstruc->parmval.cval);
                break;
                
            case 28:                /* BOTTOM VERSION                               */
                pso->seismic_interval.base_horizon.version = parmstruc->parmval.dbval;
                break;
                
            case 29:                /* TOP VERSION                                  */
                pso->seismic_interval.top_horizon.version = parmstruc->parmval.dbval;
                break;
                
            case 30:                /* DATUM LINE LIST                              */
                pso->seismic_mistie.datum_line_list = parmstruc->parmval.cval;
                break;
                
            case 31:                /* TIE LABEL COLOR                              */
                pso->seismic_mistie.tie_label.text_color = parmstruc->parmval.ival;
                break;
                
            case 32:                /* TIE LABEL FONT                               */
                pso->seismic_mistie.tie_label.text_font = parmstruc->parmval.ival;
                break;
                
            case 33:                /* TIE LABEL SIZE                               */
                pso->seismic_mistie.tie_label.text_size = parmstruc->parmval.dbval;
                break;
                
            case 34:                /* BEST SOURCES                                 */
                pso->generic.best_sources = parmstruc;
                break;
                
            default:
                break;
                }
            /* The following line added to prevent the                              */
            /*   call to md_free_parms from freeing the                             */
            /*   string pointer allocated within the                                */
            /*   parmstruc.  Otherwise, any 'cval' assigned                         */
            /*   above would become invalid (the memory                             */
            /*   pointer would have been freed)                                     */
            
            if (parmstruc->parmtype EQUALS 1)
                {
                parmstruc->parmtype = 0;
                }
            md_free_parms (parmstruc);
            
            }
        }
    wmgallocate (group_id);
    i = MD_NAMED_OVERLAY + MP_SEISMIC;
    wmoset (*group_id, i, "", overlay_verbs[verb_ndx].verb);
    wmgclose (*group_id);
    
    pso->disp_opt.default_symbol = pso->seismic_generic.shotpoint_symbol;
    
    SX_RETURN;
    }
/* END:                                                                             */
