/* DEC/CMS REPLACEMENT HISTORY, Element JG_OVERLAY.C*/
/* *13   12-OCT-1990 16:58:33 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/*  11B1 12-OCT-1990 16:57:34 GILLESPIE "Merge Ernie Deltas"*/
/* *12   14-AUG-1990 18:13:54 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *11   18-JUL-1990 17:34:55 JESSIE "(SPR 1) change include files"*/
/* *10   28-MAY-1990 09:16:20 SCD "(SPR 1) Add our own STR graticule, and manage text alignment."*/
/* *9    10-MAY-1990 11:43:16 JULIAN "(SPR 0) add nl_free_nlist on township labels"*/
/* *8     9-APR-1990 11:04:22 VINCE "(SPR 5186) draw section lines before township lines"*/
/* *7    29-MAR-1990 14:33:06 JULIAN "(SPR 0) fix text size problem"*/
/* *6    27-MAR-1990 15:08:13 JULIAN "(SPR 0) add wmsallocate&wmsclose to draw objects"*/
/* *5    27-MAR-1990 14:00:44 VINCE "(SPR 1) panic fixes"*/
/* *4    26-MAR-1990 23:46:58 JULIAN "(SPR 1) Make n-List print method a debug option"*/
/* *3    25-MAR-1990 22:41:50 SCD "(SPR 1) Another go"*/
/* *2    25-MAR-1990 12:00:11 SCD "(SPR 1) Replace Barber land grid overlay with composite object drawing engi."*/
/* *1    19-JUN-1989 13:01:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element JG_OVERLAY.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element JG_OVERLAY.C*/
/* *11   18-JUL-1990 17:34:55 JESSIE "(SPR 1) change include files"*/
/* *10   28-MAY-1990 09:16:20 SCD "(SPR 1) Add our own STR graticule, and manage text alignment."*/
/* *9    10-MAY-1990 11:43:16 JULIAN "(SPR 0) add nl_free_nlist on township labels"*/
/* *8     9-APR-1990 11:04:22 VINCE "(SPR 5186) draw section lines before township lines"*/
/* *7    29-MAR-1990 14:33:06 JULIAN "(SPR 0) fix text size problem"*/
/* *6    27-MAR-1990 15:08:13 JULIAN "(SPR 0) add wmsallocate&wmsclose to draw objects"*/
/* *5    27-MAR-1990 14:00:44 VINCE "(SPR 1) panic fixes"*/
/* *4    26-MAR-1990 23:46:58 JULIAN "(SPR 1) Make n-List print method a debug option"*/
/* *3    25-MAR-1990 22:41:50 SCD "(SPR 1) Another go"*/
/* *2    25-MAR-1990 12:00:11 SCD "(SPR 1) Replace Barber land grid overlay with composite object drawing engi."*/
/* *1    19-JUN-1989 13:01:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element JG_OVERLAY.C*/

/******************************************************************************

                Copyright FINDER Graphics Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  && MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            FINDER Graphics Systems, Inc.
            201 Tamal Vista
            Corte Madera, CA 94925
            415/927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/



/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: JG_OVERLAY.C

   AUTHOR: JOHN CONROY

   DESCRIPTION:     Function to process a Jeffersonian 'LAND GRID' overlay.
                    Modified 3/23/90 to be a "shell" for displaying composite
                    objects.
                    
                    Uses old LAND GRID overlay name and options, then drives
                    the to_draw_composite_objects engine for each of the
                    data types C_TOWNSHIPS and C_SECTIONS.

   ************************************************************************ */


/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_ly.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_MD_DEFS_H
#include "esi_md_defs.h"
#endif

#ifndef ESI_OV_H
#include "esi_ov.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_JG_DEFS_H
#include "esi_jg_defs.h"
#endif

#ifndef ESI_JG_H
#include "esi_jg.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TO_H
#include "esi_to.h"
#endif

#include "esi_nl_meth_hdr.h"
#include "esi_nl_methods.h"

#ifndef ESI_WM_H
#include "esi_wm.h"                      /* window manager */
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

/* pre-defined graphic object names of the  *
 * composited township and section lines.   */

static CHAR townships[] = "C_TOWNSHIP";
static CHAR sections[] = "C_SECTION";
static CHAR section_labels[] = "C_SECTION_LABEL";

/* * FUNCTION TITLE * */
/* ------------------ */

publicdef INT jg_overlay (display_line)

/* * ARGUEMENT LIST * */
/* ------------------ */

INT display_line;

/* * VARIABLE LIST * */
/* ----------------- */
    {

    INT status;
    LAND_GRID land_grid;    /* LAND GRID options & parameters */
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT row;
    INT i, n_tabs;
    INT dummy;
    INT segment_no;

    TEXT_GC text;
    TO_STRUCT values;
    NLIST_HEADER town_label_nlist;
    NLM_PRINT_HEADER print_hdr;
    UINT Col_List[4];
    GKS_REAL x_vect[6], y_vect[6];

    status = SUCCESS;

    /* --------------------------------- */
    /* Register required drawing methods */
    /* --------------------------------- */

    status = nl_register_nlist_method (NL_DRAW_METHOD, "TO_DRAW_TEXT_METHOD",
                                       to_draw_text_method);
    status = nl_register_nlist_method (NL_DRAW_METHOD, "TO_DRAW_LINE_METHOD",
                                       to_draw_line_method);

    /* ----------------------------------- */
    /* INITIALIZE GRAPHIC OBJECT STRUCTURE */
    /* ----------------------------------- */

    tc_zeroes (&values, sizeof(TO_STRUCT));
    lu_text_to_index ("TEXT_HORIZ_ALIGN", (INT *)&values.text_h_just, "CENTER");
    lu_text_to_index ("TEXT_VERT_ALIGN", (INT *)&values.text_v_just, "HALF");
    values.text_color = -1;
    values.text_height = -1.0;
    values.text_spacing = -1.0;
    values.text_font = -1;
    values.line.color = -1;
    values.line.style = -1;
    values.line.width = -1.0;

    status = SUCCESS;

    /* * INITIALIZE LAND GRID STRUCTURES USED IN OVERLAY * */
    /* --------------------------------------------------- */

    land_grid.name = NULL;
    land_grid.section_color = -1;
    land_grid.section_line_thickness = -1;
    land_grid.section_text_size = -1.0;
    land_grid.township_color = -1;
    land_grid.township_line_thickness = -1;
    land_grid.township_text_size = -1.0;
    land_grid.suppress_sections = FALSE;


    /* * FILL IN ORGANIZING STRUCTURES * */
    /* --------------------------------- */

    mp_get_current_map (&map);
    display = map->display;
    compiled = map->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];

    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];

            switch (verbstruc->key_code[i])
                {
            case 1:         /* NAME */
                values.name = parmstruc->parmval.cval;
                break;

            case 2:         /* SECTION LINE COLOR */
                land_grid.section_color = parmstruc->parmval.ival;
                break;

            case 3:         /* SECTION LINE THICKNESS */
                land_grid.section_line_thickness = parmstruc->parmval.ival;
                break;

            case 4:         /* SECTION TEXT SIZE */
                land_grid.section_text_size = parmstruc->parmval.dbval;
                break;

            case 5:         /* TOWNSHIP LINE COLOR */
                land_grid.township_color = parmstruc->parmval.ival;
                break;

            case 6:         /* TOWNSHIP LINE THICKNESS */
                land_grid.township_line_thickness = parmstruc->parmval.ival;
                break;

            case 7:         /* TOWNSHIP TEXT SIZE */
                land_grid.township_text_size = parmstruc->parmval.dbval;
                break;

            case 8:         /* SUPPRESS SECTIONS */
                land_grid.suppress_sections = TRUE;
                break;

            default:
                break;
                }
            }
        }

    /* * RESET TEXT SIZES * */
    /* -------------------- */

    if (land_grid.township_text_size == 0.0)
        {
        land_grid.township_text_size = MIN (1000.0,
                                            (0.10 / map->wcs_to_inches));
        }

    if (land_grid.section_text_size == 0.0)
        {
        land_grid.section_text_size = MIN (150.0,
                                           (0.10 / map->wcs_to_inches));
        }

    status = ov_open_overlay (display_line, values.name);

    if (status == SUCCESS)
        {

        /* ----------------------------------- */
        /* Draw a fine line "graticule" border */
        /* Use township line color             */
        /* ----------------------------------- */

        segment_no = 0;
        wmsallocate (&segment_no);
        wm_gsplci (land_grid.township_color);
        wm_gsln (1);
        wm_gslwsc (1.0);
        x_vect[0] = map->lower_left_xy.x;
        y_vect[0] = map->lower_left_xy.y;
        x_vect[1] = map->lower_left_xy.x;
        y_vect[1] = map->upper_right_xy.y;
        x_vect[2] = map->upper_right_xy.x;
        y_vect[2] = map->upper_right_xy.y;
        x_vect[3] = map->upper_right_xy.x;
        y_vect[3] = map->lower_left_xy.y;
        x_vect[4] = map->lower_left_xy.x;
        y_vect[4] = map->lower_left_xy.y;
        wm_gpl (5, x_vect, y_vect);
        wmsclose (segment_no);
	wmoset   (segment_no, WM_CLASS_OVERLAY, "", "Land Grid Graticule");

        if (!land_grid.suppress_sections)
            {
            /* ---------------------------------------------------- */
            /* Execute the composite drawing engine:                */
            /* - for section lines and labels with their attributes */
            /* ---------------------------------------------------- */

            values.line.color = land_grid.section_color;
            values.line.width = land_grid.section_line_thickness;
            values.text_color = land_grid.section_color;
            values.text_height = land_grid.section_text_size;
            values.suppress_lines = land_grid.suppress_sections;

            /* Draw the section lines */
            wmsallocate (&segment_no);
            status = to_draw_composite_objects (sections, &values);
            wmsclose (segment_no);
	    wmoset   (segment_no, WM_CLASS_OVERLAY, "", "Section Lines");

            /* Draw the section labels */
            wmsallocate (&segment_no);
            status = to_draw_composite_objects (section_labels, &values);
            wmsclose (segment_no);
	    wmoset   (segment_no, WM_CLASS_OVERLAY, "", "Section Labels");
            }


        /* ---------------------------------------------------- */
        /* Execute the composite drawing engine:                */
        /* - for township lines with their attributes           */
        /* ---------------------------------------------------- */

        values.line.color = land_grid.township_color;
        values.line.width = land_grid.township_line_thickness;
        values.text_color = land_grid.township_color;
        values.text_height = land_grid.township_text_size;

        segment_no = 0;
        wmsallocate (&segment_no);

        status = to_draw_composite_objects (townships, &values);

        wmsclose (segment_no);
	wmoset   (segment_no, WM_CLASS_OVERLAY, "", "Township Lines");

        /* ------------------------------- */
        /* Draw the township labels n-list */
        /* ------------------------------- */

        status = ly_init_text_nlist (&town_label_nlist);
        if (status == SUCCESS)
            {
            status = ly_generate_township_labels (town_label_nlist);
#ifdef DEBUG
            status = nl_inq_nlist_int (town_label_nlist, NL_NLIST_NTABLES,
                                       (VOIDPTR) & n_tabs);
            Col_List[0] = 1, Col_List[1] = 2, Col_List[2] = 3,
                Col_List[3] = 4;
            print_hdr.nCol = 4;
            print_hdr.Col_List = Col_List;
            print_hdr.first_table = 1;
            print_hdr.last_table = n_tabs;
            print_hdr.print_nlist_header = TRUE;
            print_hdr.print_table_header = TRUE;
            print_hdr.print_data = TRUE;
            print_hdr.outFile = NULL_FILE_PTR;
            nl_invoke_nlist_method (town_label_nlist, NL_PRINT_METHOD,
                                    (VOIDPTR) & print_hdr);
#endif
            if (status == SUCCESS)
                {
                /* --------------------------- */
                /* Draw the township labels    */
                /* --------------------------- */
                segment_no = 0;
                wmsallocate (&segment_no);

                text.text_color = land_grid.township_color;
                text.char_height = land_grid.township_text_size;
                text.char_spacing = -1.0;
                text.text_font = -1;
		text.string_h_just = -1;
		text.string_v_just = -1;
                nl_invoke_nlist_method (town_label_nlist, NL_DRAW_METHOD,
                                        (VOIDPTR) & text);
                wmsclose (segment_no);
	    	wmoset   (segment_no, WM_CLASS_OVERLAY, "", "Township Labels");
                nl_free_nlist (town_label_nlist);
                }
            }
        ov_close_overlay (display_line);
        }

    return(status);
    }

