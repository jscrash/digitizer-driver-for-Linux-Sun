/* DEC/CMS REPLACEMENT HISTORY, Element DR_TXT_DLG.C*/
/* *14    3-MAY-1991 16:10:47 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *13   12-OCT-1990 16:40:38 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/*  11B1 12-OCT-1990 16:39:34 GILLESPIE "Merge Ernie Deltas"*/
/* *12   23-AUG-1990 16:11:55 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *11   18-JUL-1990 08:52:29 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *10   15-JUN-1990 17:51:27 KEE "(SPR -1) Bug fixing and new enhancement for Electronic Drafting Text edit"*/
/* *9    12-JUN-1990 10:36:07 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *8    17-JAN-1990 15:30:58 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *7    18-OCT-1989 10:15:42 JULIAN "(SPR 0) mod param passing on nl_init_nlist"*/
/* *6    25-SEP-1989 11:32:26 GORDON "(SPR 201) Fix 2 bad lu call arguments"*/
/* *5    19-SEP-1989 13:36:46 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    10-AUG-1989 08:45:27 GORDON "(SPR 105) changed calls to dr_table_lookup & dr_selection_list, don't use views"*/
/* *3    13-JUL-1989 13:47:57 GORDON "(SPR 0) added call to wm_set_mouse_menu"*/
/* *2    26-JUN-1989 10:42:44 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:48:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_TXT_DLG.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_TXT_DLG.C*/
/* *11   18-JUL-1990 08:52:29 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *10   15-JUN-1990 17:51:27 KEE "(SPR -1) Bug fixing and new enhancement for Electronic Drafting Text edit"*/
/* *9    12-JUN-1990 10:36:07 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *8    17-JAN-1990 15:30:58 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *7    18-OCT-1989 10:15:42 JULIAN "(SPR 0) mod param passing on nl_init_nlist"*/
/* *6    25-SEP-1989 11:32:26 GORDON "(SPR 201) Fix 2 bad lu call arguments"*/
/* *5    19-SEP-1989 13:36:46 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    10-AUG-1989 08:45:27 GORDON "(SPR 105) changed calls to dr_table_lookup & dr_selection_list, don't use views"*/
/* *3    13-JUL-1989 13:47:57 GORDON "(SPR 0) added call to wm_set_mouse_menu"*/
/* *2    26-JUN-1989 10:42:44 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:48:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_TXT_DLG.C*/

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




/******************************************************************************

NAME:           dr_text_dialog_server

DESCRIPTION:    Handle all of the user interaction for editing
                of text elements.


     Input:     dialog_data - Structure containing the dialog state.
                dr_data     - Standard DR data structure pointer.
 
     Output:    dialog_data - Updated

     Status:    SUCCESS
                DR_CANCEL
                DR_TEXT_EDIT_ERROR

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "dr_txt_dlg.rh"

#ifndef ESI_AM_H
#include    "esi_am.h"
#endif
#ifndef ESI_GKS_H
#include    "esi_gks.h"
#endif
#ifndef ESI_HO_H
#include    "esi_ho.h"
#endif
#ifndef ESI_TC_H
#include    "esi_tc.h"
#endif
#ifndef ESI_WI_H
#include    "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include    "esi_wm.h"
#endif
#ifndef ESI_WI_RSRC_H
#include    "esi_wi_rsrc.h"
#endif
#ifndef ESI_WM_CLS_H
#include    "esi_wm_cls.h"
#endif
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif
#ifndef ESI_NL_H
#include    "esi_nl.h"
#endif
#ifndef ESI_LU_H
#include "esi_lu.h"
#endif
#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#include "esi_dr_msg.h"

INT drz_update_pick_nlist ();
static INT drz_copy_text ( );
/******************************************************************************/
/*   Global or Static Definitions                                             */
/******************************************************************************/

#define null_string ""
#define DR_TEXT_EDIT_ERROR -2

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/


#if USE_PROTOTYPES
publicdef INT dr_text_dialog_server (DR_DIALOG_DATA *dialog_data, DR_DATA *dr_data)
#else
publicdef INT dr_text_dialog_server (dialog_data, dr_data)
DR_DIALOG_DATA *dialog_data;
DR_DATA *dr_data;
#endif
    {
    /*  static storage */

    static BOOL codes_loaded = FALSE;
    static TCP color_list = 
        {
        0
        };
    static INT no_colors;
    static CHAR **font_list = 
        {
        0
        };
    static INT no_fonts;
    static CHAR **precision_list = 
        {
        0
        };
    static INT no_precisions;
    static CHAR **alignment_list = 
        {
        0
        };
    static INT no_alignments;

    /*  Automatic storage */

    INT status;
    INT cancel = 0;
    CDLG_ID *cdlg_id;
    BOOL done_editing;
    BOOL canceled_editing;
    BOOL any_changes;
    BOOL text_item_error;
    BOOL changed;
    INT dialog_id;
    INT item_id;
    INT value;
    CHAR text[DR_MAX_TEXT_STRING];
    INT items_changed[32];
    INT nchanges;
    INT new_len, old_len;
    INT button, nl;
    FLOAT fx1, fy1, fx2, fy2;
    DOUBLE dx1, dy1, dx2, dy2;
    DOUBLE text_wd, text_ht;
    CHAR new_color[30];
    CHAR new_font[30];
    CHAR new_precision[30];
    CHAR new_alignment[30];
    INT selected;
    INT new_code;
    INT simplex_font;
    INT i, j;
    INT main_wkid = MAIN_WKID;
    NLIST_HEADER text_nlist;
    UINT ndim = 2;
    INT units[2];
    INT data_sizes[2];
    INT data_types[2];
    INT value_inc[2];
    static CHAR *c_name[3] = 
        {
        "X", "Y", NULL
        };
    UINT dim_list[2];
    GO_WORKSPACE *w, **global_w;
    UINT stroke, point;
    BOOL new_text = FALSE;
    TEXT *old_text;

    /*  Open the dialog workspace  */

    am_open_workspace ("CDLG_ID", AM_APPLICATION, (VOIDPTR *)&cdlg_id);
    status = am_open_workspace ("GO_STATIC_SPACE", AM_GLOBAL,
                                (VOIDPTR *)&global_w);
    w = *global_w;
    for (i = 0; i < ndim; i++)
        {
        units[i] = 0;
        data_sizes[i] = sizeof(FLOAT);
        data_types[i] = FLOAT_TYPE;
        value_inc[i] = VALUE_LIST;
        dim_list[i] = i + 1;
        }
    status = nl_init_nlist (&text_nlist, ndim, units, (size_t *)data_sizes,
                            data_types, value_inc, 20);
    status = nl_set_nlist_info (text_nlist, NL_NLIST_COLUMN_NAME, ndim,
                                dim_list, (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }

    done_editing = FALSE;
    canceled_editing = FALSE;
    any_changes = FALSE;
    text_item_error = FALSE;

    if (!codes_loaded)
        {
        status = lu_get_tcp ("FONT", &font_list, &no_fonts);
        status = lu_get_tcp ("TEXT_PRECISION", &precision_list,
                             &no_precisions);
        status = lu_get_tcp ("COLOR", &color_list, &no_colors);

#ifdef USE_X    
    /* ------------------------------------------- */
    /* TEXT ALIGNMENT:                             */
    /* We create the alignment_list directly as we */
    /* only want horizontal alignment options      */
    /* ------------------------------------------- */
    ts_tcp_append(&alignment_list,"CENTER");
    ts_tcp_append(&alignment_list,"LEFT");
    ts_tcp_append(&alignment_list,"RIGHT");
#endif
        codes_loaded = TRUE;
        }

#ifdef USE_X
    /* ------------------------------- */
    /* Initialize the pop up menus for */
    /* COLOR, FONT & ALIGNMENT         */
    /* This is being done every time   */
    /* because DR is releasing and     */
    /* redefining its resources every  */
    /* activation.                     */
    /* ------------------------------- */
    wi_create_option_menu (cdlg_id->text_dialog_id, DR_ET_FONT_MENU,
                           font_list);
    lu_index_to_text ("FONT", new_font, dr_data->current_text->text_font);
    for (i = 0, j = ts_tcp_len (font_list); i < j; i++)
        {
        if (strcmp (new_font, font_list[i]) == 0)
            {
            break;
            }
        }
    wi_set_dialog_value (cdlg_id->text_dialog_id, DR_ET_FONT_MENU, i + 1);

    wi_create_option_menu (cdlg_id->text_dialog_id, DR_ET_COLOR_MENU,
                           color_list);
    lu_index_to_text ("COLOR", new_color, dr_data->current_text->text_color);
    for (i = 0, j = ts_tcp_len (color_list); i < j; i++)
        {
        if (strcmp (new_color, color_list[i]) == 0)
            {
            break;
            }
        }
    wi_set_dialog_value (cdlg_id->text_dialog_id, DR_ET_COLOR_MENU, i + 1);

    wi_create_option_menu (cdlg_id->text_dialog_id, DR_ET_ALIGNMENT_MENU,
                           alignment_list);
    lu_index_to_text ("TEXT_HORIZ_ALIGN", new_alignment,
                                  dr_data->current_text->text_alignment[0]);
    for (i = 0, j = ts_tcp_len (alignment_list); i < j; i++)
        {
        if (strcmp (new_alignment, alignment_list[i]) == 0)
            {
            break;
            }
        }
    wi_set_dialog_value (cdlg_id->text_dialog_id, DR_ET_ALIGNMENT_MENU,
                         i + 1);
                         
#endif

    wi_open_dialog (cdlg_id->text_dialog_id);

    /* -------------------------------------------------------------------- */
    /*  Display current text upon activation of text editing:               */
    /*  This forces graphics screen representation to stay synchronized     */
    /*  with the contents of the edit text window, as this module is        */
    /*  always entered with a new text initialized with the default string  */
    /*  or an existing text element picked by user.                         */
    /* -------------------------------------------------------------------- */

    if (dr_data->current_text_seg == 0)
        {
        dr_redraw_text (dr_data);
        status = drz_update_pick_nlist (dr_data, w, text_nlist);
        }

    if (ug_if_gpx ())
        {
        wm_set_mouse_menu ((CHAR * *)0, (INT *)0);  /* use default menu */
        }

    /* ------------------------------------------ */            
    /* Save the state of the current text display */
    /* so we can erase it when changed.  We also  */
    /* save the old text state at the bottom of   */
    /* the event loop.  Any operation which needs */
    /* to erase the old text just does an         */
    /* mm_draw_text(old_text).                    */
    /* ------------------------------------------ */
    drz_copy_text ( dr_data, &old_text );
    
    /* ------------------- */
    /*  Modal Event Loop   */
    /* ------------------- */

    do
        {
        changed = FALSE;
        new_text = FALSE;

        status = dr_update_dialog (dialog_data, dr_data, cdlg_id);
        wi_request_dialog_item (&dialog_id, &item_id, &value, text);

        if (dialog_id == cdlg_id->text_dialog_id)
            {
            wi_query_dialog_changes (cdlg_id->text_dialog_id,
                                     dialog_data->no_dialog_items, &nchanges,
                                     items_changed);
            for (i = 0; i < nchanges; i++)
                {
                if (items_changed[i] == DR_ET_TEXT_LINES)
                    {
                    changed = TRUE;
                    wi_query_dialog_item (cdlg_id->text_dialog_id,
                                          DR_ET_TEXT_LINES, &value, text);
                    old_len = strlen (dr_data->current_text->text_string);
                    new_len = strlen (text);
                    if (new_len <= old_len)
                        {
                        strcpy (dr_data->current_text->text_string, text);
                        }
                    else
                        {
                        am_free (dr_data->current_text->text_string);
                        dr_data->current_text->text_string =
                            (CHAR *)am_allocate (AM_APPLICATION, new_len + 1);
                        strcpy (dr_data->current_text->text_string, text);
                        }
                    break;
                    }
                }
            }

        /*  Process events  */

        switch (item_id)
            {

            /*------------------------------------------------------------------------*/
        case DR_ET_PLACE:
            /*------------------------------------------------------------------------*/

            am_message (AM_DIALOG, mg_message (DR_BAND_CHAR_SIZE));
            wmtlocate (&status, &fx1, &fy1, &button);
            if (button != WM_BUTTON_CANCEL && button != WM_BUTTON_DONE)
                {
                dx1 = (DOUBLE)fx1;
                dy1 = (DOUBLE)fy1;
                wmtbandrectangle (&status, dx1, dy1, &fx2, &fy2, &button);
                if (button != WM_BUTTON_CANCEL && button != WM_BUTTON_DONE)
                    {
                    /* --------------------------- */
                    /* Force alignment to CENTERed */
                    /* --------------------------- */
                    lu_text_to_index ("TEXT_HORIZ_ALIGN",
                                      &dr_data->current_text->
                                      text_alignment[0], "CENTER");
                    /* -------------------------------- */
                    /* Now calculate best fit text size */
                    /* -------------------------------- */
                    dx2 = (DOUBLE)fx2;
                    dy2 = (DOUBLE)fy2;

                    text_wd = ABS (dx1 - dx2);
                    text_ht = ABS (dy1 - dy2);
                    status = dr_calc_char_size (dr_data, MIN (dx1, dx2),
                                                MIN (dy1, dy2),
                                                MAX (dx1, dx2),
                                                MAX (dy1, dy2), &nl);
                    /* --------------------------------- */
                    /* Anchor the text block as follows: */
                    /* Singleton strings are centered in */
                    /* both X and Y.  Multi-line strings */
                    /* are centered in X, and positioned */
                    /* vertically at the top of the box  */
                    /* --------------------------------- */
                    dr_data->current_text->text_position.x = (dx1 + dx2) /
                        2.0;
                    if (nl > 1)
                        {
                        dr_data->current_text->text_position.y =
                            MAX (dy1,
                                 dy2) - dr_data->current_text->text_size /
                            2.0;
                        }
                    else
                        {
                        dr_data->current_text->text_position.y = (dy1 + dy2) /
                            2.0;
                        }
                    changed = TRUE;
                    }
                }
            am_message (AM_DIALOG, NULL);
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_ANGLE:
            /*------------------------------------------------------------------------*/

            am_message (AM_DIALOG, mg_message (DR_BAND_ANGLE));
            wmtlocate (&status, &fx1, &fy1, &button);
            if (button != WM_BUTTON_CANCEL && button != WM_BUTTON_DONE)
                {
                dx1 = (DOUBLE)fx1;
                dy1 = (DOUBLE)fy1;
                wmtbandvector (&status, dx1, dy1, &fx2, &fy2, &button);
                if (button != WM_BUTTON_CANCEL && button != WM_BUTTON_DONE)
                    {
                    dx2 = (DOUBLE)fx2;
                    dy2 = (DOUBLE)fy2;
                    dr_calc_char_up_vector (dr_data, (dx2 - dx1),
                                            (dy2 - dy1));
                    changed = TRUE;
                    }
                }
            am_message (AM_DIALOG, NULL);
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_POSITION:
            /*------------------------------------------------------------------------*/    

            if (changed)
                {
                changed = FALSE;
                any_changes = TRUE;
#if 0
                /* ------------------------------------ */
                /* If "drag text" function is available */
                /* we need to refresh the text to show  */
                /* the current state before we drag it  */
                /* ------------------------------------ */
                dr_redraw_text (dr_data);
#endif
                status = nl_inq_current_point (w->temp_nlist, &point,
                                               &stroke);
                status = nl_delete_stroke (w->temp_nlist, stroke);
                status = drz_update_pick_nlist (dr_data, w, text_nlist);
                }

            if (dr_data->current_text_seg != 0)
                {
                am_message (AM_DIALOG, mg_message (DR_DRAG_TEXT));

                wmtlocate (&status, &fx1, &fy1, &button);
                if (button != WM_BUTTON_CANCEL && button != WM_BUTTON_DONE)
                    {
                    changed = TRUE;

                    dr_data->current_text->text_position.x = fx1;
                    dr_data->current_text->text_position.y = fy1;
                    }
                am_message (AM_DIALOG, NULL);
                }
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_COLOR:   /*  Change Color   */
            /*------------------------------------------------------------------------*/
            /* only mac version will execute this case statement */

            if (color_list == (CHAR * *)NULL)
                status = lu_get_tcp ("COLOR", &color_list, &no_colors);

            /* To inquire the default value for setting in the select box. */
            lu_index_to_text ("COLOR", new_color,
                              dr_data->current_text->text_color);
            dr_get_selection (mg_message (DR_CHANGE_COLOR), color_list,
                              no_colors, new_color, &cancel);
            if (!cancel)
                {
                lu_text_to_index ("COLOR", &dr_data->current_text->text_color,
                                  new_color);
                dr_data->default_text.text_color =
                    dr_data->current_text->text_color;
                changed = TRUE;
                }
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_COLOR_MENU:  /*  Change Color   */
            /*------------------------------------------------------------------------*/
            /* only x version will execute this case statement */

            wi_query_dialog_item (cdlg_id->text_dialog_id, item_id, &i, text);
            strcpy (new_color, color_list[i - 1]);
            lu_text_to_index ("COLOR", &dr_data->current_text->text_color,
                              new_color);
            dr_data->default_text.text_color =
                dr_data->current_text->text_color;
            changed = TRUE;
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_FONT:    /*  Change Font   */
            /*------------------------------------------------------------------------*/
            /* only mac version will execute this case statement */

            if (font_list == (CHAR * *)NULL)
                status = lu_get_tcp ("FONT", &font_list, &no_fonts);
            /* To inquire the default value for setting in the select box. */
            lu_index_to_text ("FONT", new_font,
                              dr_data->current_text->text_font);

            dr_get_selection (mg_message (DR_CHANGE_FONT), font_list,
                              no_fonts, new_font, &cancel);
            if (!cancel)
                {
                lu_text_to_index ("FONT", &dr_data->current_text->text_font,
                                  new_font);
                dr_data->default_text.text_font =
                    dr_data->current_text->text_font;
#ifdef vms
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ========== */
                /* ------------------------------------------- */
                /* Manage precision to conform to the selected */
                /* font.  SIMPLEX font will always  be CHAR,   */
                /* the bitmap fonts will always be STRING.     */
                /* ------------------------------------------- */
                lu_text_to_index ("FONT", &simplex_font, "SIMPLEX");
                if (dr_data->current_text->text_font == simplex_font)
                    {
                    lu_text_to_index ("TEXT_PRECISION",
                                      &dr_data->current_text->text_precision,
                                      "CHARACTER");
                    }
                else
                    {
                    lu_text_to_index ("TEXT_PRECISION",
                                      &dr_data->current_text->text_precision,
                                      "STRING");
                    }
                dr_data->default_text.text_precision =
                    dr_data->current_text->text_precision;
#endif  /* ========== END DEC GKS ========== */
#endif
                changed = TRUE;
                }
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_FONT_MENU:   /*  Change Font   */
            /*------------------------------------------------------------------------*/
            /* only x version will execute this case statement */

            wi_query_dialog_item (cdlg_id->text_dialog_id, item_id, &i, text);
            strcpy (new_font, font_list[i - 1]);
            lu_text_to_index ("FONT", &dr_data->current_text->text_font,
                              new_font);
            dr_data->default_text.text_font =
                dr_data->current_text->text_font;
#ifdef vms
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ========== */
            /* ------------------------------------------- */
            /* Manage precision to conform to the selected */
            /* font.  SIMPLEX font will always  be CHAR,   */
            /* the bitmap fonts will always be STRING.     */
            /* ------------------------------------------- */
            lu_text_to_index ("FONT", &simplex_font, "SIMPLEX");
            if (dr_data->current_text->text_font == simplex_font)
                {
                lu_text_to_index ("TEXT_PRECISION",
                                  &dr_data->current_text->text_precision,
                                  "CHARACTER");
                }
            else
                {
                lu_text_to_index ("TEXT_PRECISION",
                                  &dr_data->current_text->text_precision,
                                  "STRING");
                }
            dr_data->default_text.text_precision =
                dr_data->current_text->text_precision;
#endif  /* ========== END DEC GKS ========== */
#endif
            changed = TRUE;
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_ALIGNMENT:   /*  Change Alignment  */
            /*------------------------------------------------------------------------*/
            /* only mac version will execute this case statement */

            if (alignment_list == (CHAR * *)NULL)
                status = lu_get_tcp ("TEXT_HORIZ_ALIGN", &alignment_list,
                                     &no_alignments);

            dr_get_alignment_selection (mg_message (DR_CHANGE_ALIGNMENT),
                                        dr_data->current_text->
                                        text_alignment[0], new_alignment,
                                        &cancel);

            if (!cancel)
                {
                lu_text_to_index ("TEXT_HORIZ_ALIGN", &new_code, new_alignment);
                dr_data->current_text->text_alignment[0] = new_code;

                dr_data->default_text.text_alignment[0] =
                    dr_data->current_text->text_alignment[0];
                changed = TRUE;
                }
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_ALIGNMENT_MENU:  /*  Change Alignment  */
            /*------------------------------------------------------------------------*/
            /* only x version will execute this case statement */

            wi_query_dialog_item (cdlg_id->text_dialog_id, item_id, &i, text);
            strcpy (new_alignment, alignment_list[i - 1]);
            lu_text_to_index ("TEXT_HORIZ_ALIGN", &new_code, new_alignment);
            dr_data->current_text->text_alignment[0] = new_code;
            dr_data->default_text.text_alignment[0] =
                dr_data->current_text->text_alignment[0];
            changed = TRUE;
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_NEW:     /* New text element  */
            /*------------------------------------------------------------------------*/
            status = dr_new_text (dr_data);
            wi_set_dialog_text (cdlg_id->text_dialog_id, DR_ET_TEXT_LINES,
                                dr_data->current_text->text_string);
            new_text = changed = TRUE;
            /* ------------------------------------- */
            /* Update the old text buffer to reflect */
            /* the state of the new text string      */
            /* ------------------------------------- */
            drz_copy_text ( dr_data, &old_text );
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_SELECT:  /*  Pick a text element  */
            /*------------------------------------------------------------------------*/

            /* --------------------------------------------- */
            /* If last operation left a pick on, turn it off */
            /* --------------------------------------------- */

            if (dr_data->pick_echo)
                {
                dr_data->pick_echo = FALSE;
                wm_grsgwk (main_wkid);
                }
    
            status = dr_pick_text (dr_data); 
           
            /* ------------------------------------- */
            /* Update the old text buffer to reflect */
            /* the state of the selected text string */
            /* ------------------------------------- */
            drz_copy_text ( dr_data, &old_text );

#ifdef USE_X

            /* ---------------------------------- */
            /* The following code is X dependent, */
            /* because it uses pop-up menus.      */
            /* ---------------------------------- */
            if (status == SUCCESS)
                {
                /* -------------------------------------- */
                /* Set FONT menu to match selected object */
                /* -------------------------------------- */
                lu_index_to_text ("FONT", new_font,
                                  dr_data->current_text->text_font);
                for (i = 0, j = ts_tcp_len (font_list); i < j; i++)
                    {
                    if (strcmp (new_font, font_list[i]) == 0)
                        {
                        break;
                        }
                    }
                wi_set_dialog_value (cdlg_id->text_dialog_id, DR_ET_FONT_MENU,
                                     i + 1);
                                     
                /* --------------------------------------- */
                /* Set COLOR menu to match selected object */
                /* --------------------------------------- */
                lu_index_to_text ("COLOR", new_color,
                                  dr_data->current_text->text_color);
                for (i = 0, j = ts_tcp_len (color_list); i < j; i++)
                    {
                    if (strcmp (new_color, color_list[i]) == 0)
                        {
                        break;
                        }
                    }
                wi_set_dialog_value (cdlg_id->text_dialog_id,
                                     DR_ET_COLOR_MENU, i + 1);

                /* ------------------------------------------- */
                /* Set ALIGNMENT menu to match selected object */
                /* ------------------------------------------- */
                lu_index_to_text ("TEXT_HORIZ_ALIGN", new_alignment,
                                  dr_data->current_text->text_alignment[0]);
                for (i = 0, j = ts_tcp_len (alignment_list); i < j; i++)
                    {
                    if (strcmp (new_alignment, alignment_list[i]) == 0)
                        {
                        break;
                        }
                    }
                wi_set_dialog_value (cdlg_id->text_dialog_id,
                                     DR_ET_ALIGNMENT_MENU, i + 1);
                }
#endif          
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_DELETE:  /*  Delete a text element  */
            /*------------------------------------------------------------------------*/
            
            if (dr_data->current_text_seg != 0 && old_text != (TEXT *)0 )
                {
                /* ------------------------------------------- */
                /* Erase the old text.  dr_delete_text_element */
                /* will delete the segment. Then clear the     */
                /* old text buffer                             */
                /* ------------------------------------------- */
                mm_draw_text(old_text,TRUE);
                }
            old_text = (TEXT *) 0;
            
            status = dr_delete_text_element (dr_data);

            changed = FALSE;
            any_changes = TRUE;
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_DISPLAY: /*  Refresh text on display  */
            /*------------------------------------------------------------------------*/
            
            /* --------------------------------------- */
            /* No need to do anything, because we will */
            /* update the display at bottom of the     */
            /* event loop                              */
            /* --------------------------------------- */
            break;

            /*------------------------------------------------------------------------*/
        case DR_ET_OK:      /*  Exit  */
            /*------------------------------------------------------------------------*/

            done_editing = TRUE;
            break;

            /*------------------------------------------------------------------------*/
        default:
            break;
            /*------------------------------------------------------------------------*/
            }

        /* --------------------------------- */
        /* If user changed any aspect of the */
        /* text, then update the screen      */
        /* --------------------------------- */
        if (changed)
            {
            any_changes = TRUE;

            if ( dr_data->current_text_seg != 0 && old_text != (TEXT *)0 )
                {
                mm_draw_text(old_text,TRUE);
                }

            /* ------------------------------------------- */
            /* Update the display to show the current text */
            /* ------------------------------------------- */
            dr_redraw_text (dr_data);

            /* ------------------------------------------ */            
            /* Save the state of the current text display */
            /* so we can erase it later when changed.     */
            /* ------------------------------------------ */
            drz_copy_text ( dr_data, &old_text );
            
            /* ------------------------------------ */
            /* Update the KLUDGE text picking nlist */
            /* ------------------------------------ */
            if (new_text == FALSE)
                {
                status = nl_inq_current_point (w->temp_nlist, &point,
                                               &stroke);
                status = nl_delete_stroke (w->temp_nlist, stroke);
                }
            status = drz_update_pick_nlist (dr_data, w, text_nlist);
            }

        }
    while (!done_editing);

    if (any_changes)
        dr_data->object_changed = TRUE;
    if (status < 0)
        status = DR_TEXT_EDIT_ERROR;
    status = nl_free_nlist (text_nlist);
    return status;

    }

/* Function Description -----------------------------------------------------
Description:
    Make a local static copy of a text object. 
    The text color is automatically set to the background color,
    so that the return text object is ready for erasing.

Prototype:
    static INT drz_copy_text ( DR_DATA *dr_data, TEXT **text_object );
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) dr_data         -(DR_DATA *) Pointer to electronic drafting struct.
    (O) text_object     -(TEXT **)   Pointer to copy of text object returned.


Return Value/Status:
    SUCCESS - Successful completion always.
    
Scope:
    PRIVATE
    
Limitations/Assumptions:
    None.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT drz_copy_text ( DR_DATA *dr_data, TEXT **text_object )
#else
static INT drz_copy_text ( dr_data, text_object )
DR_DATA *dr_data;
TEXT **text_object;
#endif
    {
    static TEXT local_text = {0};
    static INT erase_color = -1;

    /* ---------------------------------- */
    /* Remember the erase color for speed */
    /* ---------------------------------- */
    if ( erase_color < 0 )
    {
    lu_text_to_index ("COLOR", &erase_color, "BACKGROUND");
    }
    
    /* ----------------------------------------------- */
    /* Before we copy the structure, free any residual */
    /* copy of the text string.                        */
    /* ----------------------------------------------- */
    if ( local_text.text_string != (CHAR *)0 )
        {
        tc_free( local_text.text_string );
        }
    hoblockmove( (VOIDPTR)dr_data->current_text, (VOIDPTR)&local_text, 
    (size_t)sizeof(TEXT) );
    local_text.text_string = 
        STRING_ALLOCATE( dr_data->current_text->text_string);
    strcpy( local_text.text_string, dr_data->current_text->text_string );
    local_text.text_color = erase_color;

    *text_object = &local_text;
    return SUCCESS;
    }

INT drz_update_pick_nlist (dr_data, w, text_nlist)
DR_DATA *dr_data;
GO_WORKSPACE *w;
NLIST_HEADER text_nlist;
    {
    UINT dim_list[2];
    VOIDPTR value_list[2];
    INT status;
    FLOAT cx, cy;
    UINT stroke;
    GO_USER_STRUCT user_struct;
    TEXT *text;

    text = dr_data->current_text;
    wm_gqtxx (MAIN_WKID, text->text_position.x, text->text_position.y,
              text->text_string, &status, &cx, &cy, text->box_boundary[0],
              text->box_boundary[1]);
    dim_list[0] = 1, value_list[0] = (VOIDPTR)text->box_boundary[0];
    dim_list[1] = 2, value_list[1] = (VOIDPTR)text->box_boundary[1];
    status = nl_start_stroke (text_nlist, &stroke);
    status = nl_add_points (text_nlist, 4, 2, dim_list, value_list);
    status = go_copy_stroke (text_nlist, stroke, w->temp_nlist, &stroke);
    user_struct.elem_id = dr_data->current_text->text_id;
    user_struct.if_text = TRUE;
    user_struct.segment_id = dr_data->current_text_seg;
    user_struct.obj_id = dr_data->current_object_key;
    user_struct.group_id = dr_data->current_object;
    status = nl_set_stroke_user_struct (w->temp_nlist, stroke,
                                        (VOIDPTR) & user_struct,
                                        sizeof(GO_USER_STRUCT));
    return(status);
    }

