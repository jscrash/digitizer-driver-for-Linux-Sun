/*  DEC/CMS REPLACEMENT HISTORY, Element MP_WIN_HEAD.C*/
/*  *10   15-AUG-1991 17:34:23 JTM "(SPR 0) Cast args to DOUBLE for wm_gschup(), wm_gschxp(), and wm_gtx()."*/
/*  *9    17-AUG-1990 22:11:00 VINCE "(SPR 5644) Code Cleanup"*/
/*  *8    15-MAR-1990 13:04:36 WALTERS "(SPR 0) Fix draw scale bar bug"*/
/*  *7     7-MAR-1990 17:25:00 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/*  *6    18-DEC-1989 18:14:26 JULIAN "(SPR 100) Clean non-printing characters from string from ctime()"*/
/*  *5    27-OCT-1989 11:27:50 GILLESPIE "(SPR 1) Change data type of curr_time to time_t"*/
/*  *4    14-SEP-1989 16:19:00 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/*  *3    18-JUL-1989 10:56:01 GORDON "(SPR 100) Merge in UNIX changes"*/
/*  *2    26-JUN-1989 11:01:49 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:15:25 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_WIN_HEAD.C*/
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
#include "esi_gl_project.h"
#include "esi_mp.h"
#include "esi_wm.h"
#include "esi_wm_cls.h"
#include "esi_qs.h"
#include "esi_tu.h"
#include "esi_am.h"
#include "esi_time.h"
#include "esi_lu.h"
#include "esi_ts.h"
#include "esi_gks.h"

/*****************************************************************************/

#define NUM_SECTIONS 4

/*****************************************************************************/

/* Function Description -----------------------------------------------------
Description:
    Construct a window header block.

Prototype:
    publicdef INT mp_window_header();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_window_header ()
#else
publicdef INT mp_window_header ()
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    CHAR strings[NUM_SECTIONS][41];
    FLOAT wcsxmin, wcsxmax, wcsymin, wcsymax;
    INT i, j;
    time_t curr_time;
    DOUBLE dummy;
    FLOAT dum1, dum2, dum3, dum4;
    FLOAT background_x[5], background_y[5];
    FLOAT divider_length;
    FLOAT char_height;
    INT max_str;
    FLOAT cpx, cpy, txex[4], txey[4];
    UOM_NAME uom1, uom2, map_uom;
    DOUBLE db1, db2, db3, db4, db5, db6;
    CHAR user_string[60];
    INT color;
    
    /**************************************************************************/
    
    /* * OPEN MAP WINDOW * */
    /* ------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    if ((status = wmwopen (mapdef->map_window_id)) != SUCCESS)
        {
        goto finished;
        }
    /* * GET TRANSFORMATION AND WINDOW EXTENTS * */
    /* ----------------------------------------- */
    
    wmtselect (mapdef->map_tnr);
    wmtinq (mapdef->map_tnr, &dum1, &dum2, &dum3, &dum4, &wcsxmin, &wcsxmax, &wcsymin,
            &wcsymax);
    
    /* * ALLOCATE SEGMENT FOR MAP WINDOW HEADER * */
    /* ------------------------------------------ */
    
    if (mapdef->map_header_id != 0)
        {
        wmsdelete (mapdef->map_header_id);
        }
    wmsallocate (&(mapdef->map_header_id));
    wmoset (mapdef->map_header_id, WM_CLASS_SEGMENT, "", "Map Header");
    
    /* * SET GKS LINE AND COLOR ATTRIBUTES * */
    /* ------------------------------------- */
    
    lu_text_to_index ("FILL_STYLE", &i, "SOLID");
    wm_gsfais (i);
    lu_text_to_index ("LINE_STYLE", &i, "SOLID");
    wm_gsln (i);
    dum1 = 1.0;
    wm_gslwsc ((DOUBLE)dum1);
    
    lu_text_to_index ("COLOR", &color, "BLUE");
    wm_gsfaci (color);
    lu_text_to_index ("COLOR", &color, "WHITE");
    wm_gsplci (color);
    
    /* * DRAW THE BACKGROUND AREA AND BORDER * */
    /* --------------------------------------- */
    
    background_x[0] = wcsxmin;
    background_x[1] = wcsxmin;
    background_x[2] = wcsxmax;
    background_x[3] = wcsxmax;
    background_x[4] = wcsxmin;
    
    background_y[0] = wcsymax - (0.75 / mapdef->wcs_to_inches);
    background_y[1] = wcsymax;
    background_y[2] = wcsymax;
    background_y[3] = background_y[0];
    background_y[4] = background_y[0];
    i = 5;
    wm_gfa (i, background_x, background_y);
    wm_gpl (i, background_x, background_y);
    
    /* * SECTION OFF SCALE BAR AND NORTH ARROW AREAS * */
    /* ----------------------------------------------- */
    
    background_x[1] = wcsxmax;
    background_y[0] = wcsymax - (0.25 / mapdef->wcs_to_inches);
    background_y[1] = background_y[0];
    i = 2;
    wm_gpl (i, background_x, background_y);
    
    background_x[0] = wcsxmax - (0.5 / mapdef->wcs_to_inches);
    background_x[1] = background_x[0];
    background_y[1] = background_y[4];
    wm_gpl (i, background_x, background_y);
    
    /* * DIVIDE TOP HEADER LINE * */
    /* -------------------------- */
    
    divider_length = (wcsxmax - wcsxmin) / (FLOAT)NUM_SECTIONS;
    background_x[0] = wcsxmin;
    background_x[1] = wcsxmin;
    background_y[1] = wcsymax;
    for (j = 0; j < (NUM_SECTIONS - 1); j++)
        {
        background_x[0] += divider_length;
        background_x[1] += divider_length;
        wm_gpl (i, background_x, background_y);
        }
    /* * INITIALIZE GKS TEXT PARAMETERS * */
    /* ---------------------------------- */
    
    char_height = 0.125 / mapdef->wcs_to_inches;
    wm_gschh (char_height);
    
    dum1 = 0.0;
    dum2 = 1.0;
    wm_gschup ((DOUBLE)dum1, (DOUBLE)dum2);
    wm_gschxp ((DOUBLE)dum2);
    
    wm_gstxp (GRIGHT);
    wm_gstxal (GAHNOR, GAHALF);
    wm_gstxfp (SIMPLEX, GSTRKP);
    lu_text_to_index ("COLOR", &color, "WHITE");
    wm_gstxci (color);
    
    /* * GET HEADER TEXT STRINGS * */
    /* --------------------------- */
    
    qs_inq_c (QS_PROJECT_NAME, strings[0], (INT *)0);
    qs_inq_c (QS_MAP_PROJECTION, strings[1], (INT *)0);
    sprintf (strings[2], "#%d: %s", mapdef->map_number, mapdef->map_name);
    time (&curr_time);
    strcpy (strings[3], ts_snowhite (ctime (&curr_time)));
    
    /* FIND THE LONGEST STRING */
    i = MAIN_WKID;
    dum1 = 0.0;
    for (j = NUM_SECTIONS - 1; j >= 0; j--)
        {
        wm_gqtxx (i, wcsxmin, wcsymin, strings[j], &status, &cpx, &cpy, txex, txey);
        dum2 = txex[1] - txex[0];
        if (dum1 < dum2)
            {
            dum1 = dum2;
            max_str = j;
            }
        }
    /* * CALCULATE CHARACTER HEIGHT AND TEXT EXPANSION FACTOR * */
    /* -------------------------------------------------------- */
    
    dum2 = 0.9 * divider_length;
    FOREVER
        {
        wm_gqtxx (i, wcsxmin, wcsymin, strings[max_str], &status, &cpx, &cpy, txex,
                  txey);
        dum1 = dum2 / (txex[1] - txex[0]);
        if (dum1 < 1.0)
            {
            if (dum1 < 0.5)
                {
                char_height *= 0.75;
                wm_gschh (char_height);
                }
            else
                {
                wm_gschxp (dum1);
                break;
                }
            }
        else
            {
            break;
            }
        }
    /* * FILL IN TEXT FOR TOP HEADER LINE * */
    /* ------------------------------------ */
    
    dum1 = wcsxmin + (0.05 * divider_length);
    dum2 = wcsymax - 0.125 / mapdef->wcs_to_inches;
    
    for (i = 0; i < NUM_SECTIONS; i++)
        {
        wm_gtx ((DOUBLE)dum1, (DOUBLE)dum2, strings[i]);
        dum1 += divider_length;
        }
    /* * DRAW SCALE BAR * */
    /* ------------------ */
    
    /* COMPUTE SCALE IN SCALE BAR UOMS */
    
    sprintf (user_string, "1.0 %s", mapdef->map_scale_uom);
    tu_parse_unit (user_string, &dummy, uom1, uom2);
    
    db1 = 1.0;
    ct_get_projection_uom (mapdef->map_projection, map_uom);
    
    tu_unit_conversion (db1, map_uom, &dummy, uom1);
    dum1 = dummy;
    
    tu_unit_conversion (db1, uom2, &dummy, "INCHES");
    dum2 = dummy;
    
    db6 = (dum1 * dum2) / mapdef->wcs_to_inches;
    
    /* COMPUTE ENCLOSING BOX */
    
    db1 = wcsxmin;
    db2 = wcsxmax - (0.5 / mapdef->wcs_to_inches);
    db3 = wcsymax - (0.75 / mapdef->wcs_to_inches);
    db4 = wcsymax - (0.25 / mapdef->wcs_to_inches);
    
    db5 = mapdef->wcs_to_inches;
    
    i = (mapdef->bar_type) ? mapdef->bar_type : 1;
    mp_draw_scale_bar (db1, db2, db3, db4, db5, db6, mapdef->map_scale_uom, i);
    
    /* * DRAW NORTH ARROW * */
    /* -------------------- */
    
    /* COMPUTE ENCLOSING BOX */
    
    db1 = wcsxmax - (0.5 / mapdef->wcs_to_inches);
    db2 = wcsxmax;
    db3 = wcsymax - (0.75 / mapdef->wcs_to_inches);
    db4 = wcsymax - (0.25 / mapdef->wcs_to_inches);
    
    i = (mapdef->arrow_type) ? mapdef->bar_type : 1;
    mp_draw_north_arrow (db1, db2, db3, db4, i);
    
    /* * CLOSE UP * */
    /* ------------ */
    
    wmsclose (mapdef->map_header_id);
    wmwclose (mapdef->map_window_id);
    sprintf (user_string, "%s: %s", WM_CLSTR_MAP, strings[2]);
    wmoset (mapdef->map_window_id, WM_CLASS_MAP, "", user_string);
    
finished:
    return(status);
    }
/* END:     */
