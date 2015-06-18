/* DEC/CMS REPLACEMENT HISTORY, Element GO_OVERLAY.C*/
/* *12   12-JUN-1991 11:42:26 KEE "(SPR 7094) Fix bug of display graphic object overlay, case 1308"*/
/* *11   30-JAN-1991 10:39:29 MING "(SPR -12) fix the former change that screw up user defined options in mapdef"*/
/* *10    4-JAN-1991 14:04:33 JESSIE "(SPR 6397) reset default values"*/
/* *9    25-OCT-1990 12:46:24 VINCE "(SPR 1) changes values.top to values.marker in horizon test when type is FAULTS"*/
/* *8    23-OCT-1990 11:14:01 GILLESPIE "(SPR 1000) Merge from Ernie"*/
/*  4B1V1B1 23-OCT-1990 10:40:02 GILLESPIE "Merge from Ernie"*/
/* *7    17-OCT-1990 10:39:15 VINCE "(SPR 100) resolve Beta Merge conflicts"*/
/*  4B1V1 17-OCT-1990 09:29:56 VINCE "Deltas from BETA"*/
/* *6    15-OCT-1990 23:05:02 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  4B1  15-OCT-1990 17:01:48 GILLESPIE "Merge Ernie deltas"*/
/* *5    14-AUG-1990 18:10:54 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *4    18-JUL-1990 11:22:36 GILLESPIE "(SPR 1) Cleanup"*/
/* *3    10-AUG-1989 09:26:59 GORDON "(SPR 105) move mp_get_current_map line, remove variable map"*/
/* *2    26-JUN-1989 10:45:29 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:53:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GO_OVERLAY.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*
 * FILE : GO_OVERLAY.C
 *
 * ROUTINES : GO_OVERLAY()
 *
 * DESCRIPTION : This module will parse and draw a maps graphics overlay.
 * 
 * Associated Files : GO_POST_GROV.C  - draws the overlay.
 *
 */

/* Include Files                                                             */
#include "esi_go.h"                /* graphics overlay   */
#include "esi_go_err.h"
#include "esi_ov.h"                /* overlay defintions */
#include "esi_mp.h"                /* mapping structures */
#include "esi_fm.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_am.h"

/* default interpolation spacing for xyz scatter file generation */ 
#define MAX_DATA_INTERVAL_FACTOR .01

/*****************************************************************************/
/*                                                                           */
/*  Function : GO_OVERLAY()                                                  */
/*                                                                           */
/*  Description :                                                            */
/*                                                                           */
/*****************************************************************************/
#if USE_PROTOTYPES
publicdef INT go_overlay (INT display_line)
#else
publicdef INT go_overlay (display_line)
INT display_line;
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT row;
    INT i;
    GO_STRUCT values;                   /* struct to override defaults       */
    PICKABLE_GRA_OBJ **pick_gra_obj;    /* definitions for picking           */
    INT ncells;
    GO_WORKSPACE *w;
    FM_FILE name;
    FILENAME os_name;
    CHAR mode[2];
    CHAR desc[241];
    CHAR go_of_map[128];
    DOUBLE map_width, map_height;
    
    /* -------------------------------------------------------------------- */
    /* start workspace for holding common nlists for software picking       */
    /* If there is a graphics object software picking workpace, release it. */
    /* open the GO workspace for this specified map.                        */
    /* -------------------------------------------------------------------- */
    
    mp_get_current_map (&mapdef);
    sprintf (go_of_map, "GO_SPACE_%d", mapdef->map_number);

    status = am_open_workspace (go_of_map, AM_GLOBAL, (VOIDPTR *) &w);
    if (status EQUALS SUCCESS)
        {
        if (w->temp_nlist != (NLIST_HEADER)NULL)
            status = nl_free_nlist (w->temp_nlist);
        
        if (w->master_nlist != (NLIST_HEADER)NULL)
            status = nl_free_nlist (w->master_nlist);
        status = am_release_workspace (go_of_map, AM_GLOBAL);
        }

    status = SUCCESS;
    
    /* * INITIALIZE GRAPHIC OBJECT STRUCTURE * */
    /* --------------------------------------- */
    
    tc_zeroes (&values, sizeof(GO_STRUCT));
    values.text_color = -1; /* init for go_draw_text */
    values.text_size = 300.0;
    values.text_font  = -999;
    values.text_expansion_factor = -1.0;
    values.text_precision = -1;

    values.line.color = -1;
    values.line.style = -1;
    values.line.width = -1.0;
    
    /* * FILL THE STRUCTURES * */
    /* ----------------------- */
    
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
                case 1:                 /* ADD TO XYZ FILE       */
                    values.add_to_xyz_file = parmstruc->parmval.cval;
                    break;
                
                case 2:                 /* BOTTOM                */
                    values.bottom = parmstruc->parmval.cval;
                    break;
                
                case 3:                 /* LINE COLOR            */
                    values.line.color = parmstruc->parmval.ival;
                    break;
                
                case 4:                 /* LINE STYLE            */
                    values.line.style = parmstruc->parmval.ival;
                    break;
                
                case 5:                 /* LINE THICKNESS        */
                    values.line.width = parmstruc->parmval.ival;
                    break;
                
                case 6:                 /* MARKER                */
                    values.marker = parmstruc->parmval.cval;
                    break;
                
                case 7:                 /* MAXIMUM DATA INTERVAL */
                    values.maximum_data_interval = parmstruc->parmval.dbval;
                    break;
                
                case 8:                 /* NAME                  */
                    values.name = parmstruc->parmval.cval;
                    break;
                
                case 9:                 /* SELECT BY PHRASE      */
                    values.select_by_phrase = parmstruc->parmval.cval;
                    break;
                
                case 10:                /* SELECT LIST           */
                    values.select_list = parmstruc->parmval.cval;
                    break;
                
                case 11:                /* SUPPRESS EDITING      */
                    values.suppress_editing = TRUE;
                    break;
                  
                case 12:                /* SUPPRESS LINES        */
                    values.suppress_lines = TRUE;
                    break;
                
                case 13:                /* SUPPRESS TEXT         */
                    values.suppress_text = TRUE;
                    break;
                
                case 14:                /* TEXT COLOR            */
                    values.text_color = parmstruc->parmval.ival;
                    break;
                
                case 15:                /* TEXT EXPANSION        */
                    values.text_expansion_factor = parmstruc->parmval.dbval;
                    break;
                
                case 16:                /* TEXT FONT             */
                    values.text_font = parmstruc->parmval.ival;
                    break;
                
                case 17:                /* TEXT PRECISION        */
                    values.text_precision = parmstruc->parmval.ival;
                    break;
                
                case 18:                /* TEXT SIZE             */
                    values.text_size = parmstruc->parmval.dbval;
                    break;
                                                             
                case 19:                /* TOP                   */
                    values.top = parmstruc->parmval.cval;
                    break;
                
                case 20:                /* TYPE                  */
                    values.object_type = parmstruc->parmval.cval;
                    break;
                
                case 21:                /* XYZ FILE              */
                    values.xyz_file = parmstruc->parmval.cval;
                    break;
 
                case 22:               /* SOURCE                 */
                    values.source = parmstruc->parmval.cval;
                    break;
                
                default:
                    break;
                }
            }
        }


    /* * IF THE OBJECT_TYPE IS FAULTS CHECK FOR A HORIZON NAME * */
    /* --------------------------------------------------------- */

    if ( ARE_SAME(values.object_type, "FAULTS") &&
         (values.marker == NULL)                 )
        {
	return GO_FAULT_HORIZON_REQUIRED;
        }

    /* * SET UP FOR SCATTER FILE GENERATION * */
    /* -------------------------------------- */
    
    if ( (values.xyz_file        != NULL) OR
         (values.add_to_xyz_file != NULL) )
        {
        if (values.xyz_file EQUALS NULL)
            {
            strcpy (name, values.add_to_xyz_file);
            strcpy (mode, "a");
            strcpy (desc, "Appended ");
            }
        else
            {
            strcpy (name, values.xyz_file);
            strcpy (mode, "w");
            strcpy (desc, "Created ");
            }

        status = ov_open_scatter (&(values.scatter), name, mode, os_name);
        if (status != SUCCESS)
            {
            return status;
            }

        /* CHECK FOR MAX DATA INTERVAL */
        
        if (values.maximum_data_interval <= 0)
            {
            map_width  = (DOUBLE)(mapdef->upper_right_xy.x -
                                     mapdef->lower_left_xy.x);
            map_height = (DOUBLE)(mapdef->upper_right_xy.y -
                                     mapdef->lower_left_xy.y);
            map_width  = (map_width  >= 0.0 ? map_width  :  - map_width);
            map_height = (map_height >= 0.0 ? map_height :  - map_height);
            values.maximum_data_interval = MAX_DATA_INTERVAL_FACTOR *
                                           ((map_width < map_height) ?
                                            map_width : map_height);
            }
        }

    /* * CALL THE POSTING ROUTINE * */
    /* ---------------------------- */
    
    status = ov_open_overlay (display_line, values.name);
    if (status EQUALS SUCCESS)
        {
        pick_gra_obj = (PICKABLE_GRA_OBJ **)(compiled->picks[row]);
        
        /* ------------------------------------------------------------ */
        /* allocate pointer to array of pickable-object structures for  */
        /* this overlay, this array will have as many elements as cells */
        /* in GIS, for now only one                                     */
        /* ------------------------------------------------------------ */

        ncells = 1;
        if (pick_gra_obj == (PICKABLE_GRA_OBJ **)NULL)
            {
            pick_gra_obj = (PICKABLE_GRA_OBJ **)am_allocate (AM_GLOBAL,
                               ncells * sizeof(PICKABLE_GRA_OBJ *));
            compiled->picks[row] = (BYTE *)pick_gra_obj;
            }

        /* allocate pointer to picking for all cells in GIS, now only one */

        if (pick_gra_obj[0] == (PICKABLE_GRA_OBJ *)NULL)
            {
            pick_gra_obj[0] = (PICKABLE_GRA_OBJ *)am_allocate (AM_GLOBAL,
                                                  sizeof(PICKABLE_GRA_OBJ));
            }
        else
            {
            pick_gra_obj[0]->nused = 0;
            }

        status = go_post_grov (&values, pick_gra_obj);
        ov_close_overlay (display_line);
        }

    /* * CLOSE SCATTER FILE * */
    /* ---------------------- */
    
    if (values.scatter != (NLIST_HEADER)0)
        {
        ov_close_scatter (values.scatter, name, mode, os_name);
        
        strcat (desc, "in overlay \"");
        strcat (desc, display->verbs[display_line]);
        strcat (desc, "\" in map \"");
        strcat (desc, mapdef->map_name);
        strcat (desc, "\"");
        fm_add_history_record ("ESI$SCATTERS", name, "SCATTER",
                               "", "", "", desc);
        }

    return status;
    }
