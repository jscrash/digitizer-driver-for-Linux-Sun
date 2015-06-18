/* DEC/CMS REPLACEMENT HISTORY, Element CO_POST_CON.C*/
/*  20   11-MAR-1992 18:13:28 JTM "(SPR 0) checked changes for Don's additions"*/
/*  19   16-JAN-1992 12:04:51 JTM "(SPR 0)  Contour Annotation code from Don V."*/
/* *18   23-JUL-1991 15:16:39 MING "(SPR 0) put area of interest mismatch message up so that it only prints once"*/
/* *17   15-JUL-1991 11:22:34 MING "(SPR 0) add message when map area of interest is not the same as the contour file"*/
/* *16   20-JUN-1991 11:17:58 KEE "(SPR -1) Turn Clipping on for case 2929"*/
/* *15   18-JUN-1991 14:38:23 JULIAN "(SPR 2940) (Jantina) Removed check on coordinate system flag - only need to check map*/
/*projections"*/
/* *14    3-MAY-1991 16:31:17 KEE "(SPR -1) Bug fix when porting MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *13   27-NOV-1990 16:27:56 JULIAN "(SPR 6198) resize contour labels"*/
/* *12   14-AUG-1990 13:05:59 VINCE "(SPR 5644) Header Standardization"*/
/* *11   25-APR-1990 08:32:02 JULIAN "(SPR 0) remove line_color option from co_overlay"*/
/* *10   25-MAR-1990 10:24:18 GILLESPIE "(SPR 1) Add autopurge"*/
/* *9    14-FEB-1990 13:17:15 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *8    17-JAN-1990 15:29:48 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *7    24-OCT-1989 14:01:51 JULIAN "(SPR 0) mod param passing on nl_inq_nlist_user_struct"*/
/* *6    20-SEP-1989 07:47:38 GORDON "(SPR 100) Undo last checkin"*/
/* *5    19-SEP-1989 13:35:20 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    18-SEP-1989 12:42:43 JULIAN "Gulf mods under SPR 100"*/
/* *3    14-SEP-1989 16:14:25 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    18-JUL-1989 14:29:47 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:43:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_POST_CON.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_math.h"
#include "esi_c_lib.h"
#include "esi_co.h"
#include "esi_co_err.h"
#include "esi_ov_err.h"
#include "esi_dg_defs.h"

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

#include "esi_ts.h"
#include "esi_nl_meth_hdr.h"

/* Function Description -----------------------------------------------------
Description:
    Function to post and label contour lines.

Prototype:
    publicdef INT co_post_contours(CONTOURS *contours);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour information structure.

Return Value/Status:
    SUCCESS - Successful completion
    OV_PROJECTION_MISMATCH - Error if projections or coordinate systems are different.
    CO_CONTOUR_FILE_ERROR - Error with contour file.
    Function also returns status returned by nl_inq_nlist_info, nl_set_nlist_info,
        nl_init_nlist, fm_init_file.
        
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_post_contours (CONTOURS *contours) 
#else
publicdef INT co_post_contours (contours)
CONTOURS *contours;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;                     /* RETURN STATUS */
    MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE */
    FILENAME file_name;             /* LINES NLIST FILE */
    NLIST_HEADER contour_lines = (NLIST_HEADER)0;   /* NLIST FOR LINES */
    CONTOUR_NLIST_USER_STRUCT contour_user; /* LINES NLIST USER STRUCT */
    DG_LINE_STROKE_USER_STRUCT line_stroke_user;    /* LINES STROKE USER STRUCT */
    NLIST_HEADER line_nlist = (NLIST_HEADER)0;  /* LINE STRUCTURE FOR POSTING */
    NLM_CLIP_HEADER clip;
    INT num_tables;                 /* NUMBER OF TABLES IN LINES NLIST */
    INT bold_line_interval;         /* BOLD LINE INTERVAL */
    INT label_interval;             /* LABEL INTERVAL */
    INT bold_line_color;            /* BOLD LINE COLOR */
    INT fine_line_color;            /* FINE LINE COLOR */
    INT fine_code;                  /* CODE FOR FINE LINE */
    INT bold_code;                  /* CODE FOR BOLD LINE */
    INT interval_num;               /* INTERVAL NUMBER */
    INT crnt_color = -1;
    DOUBLE crnt_thick = -1.0;
    INT crnt_text_color = -1;
    SEGMENT_ID segment_id;          /* WINDOW MANAGER SEGMENT ID */
    INT i;
    UINT j;                         /* DUMMY INDEX */
    BOOL clipping;
    INT lineStyle;
    FLOAT dummy;
    FLOAT xnull;
    FLOAT ynull;
    DOUBLE adjust;
    FM_PATH log_name;
    FM_TYPE fi_type;
    FM_DESCRIB desc;
    FLOAT contour_xmin, contour_ymin;
    FLOAT contour_xmax, contour_ymax;
    CHAR message[240];
    
    /* NEEDED NLIST VARIABLES */
    INT units[2];                   /*      UNITS */
    size_t d_size[2];               /*      DATA SIZES */
    INT d_type[2];                  /*      DATA TYPES */
    INT v_type[2];                  /*      VALUE TYPE */
    INT blocksize = 1024;           /*      BLOCK SIZE */
    UINT d_list[2];                 /*      DIM LIST */
    VOIDPTR v_list[2];              /*      VALUE LIST */
    static CHAR *c_name[3] = 
        {
        "X",
        "Y",
        NULL
        };
    
    /* ********************************************************************** */
    
    mp_get_current_map (&map);
    
    /* * CREATE LINE NLIST FILE NAME * */
    /* ------------------------------- */
    
    if (contours->view_type == 0)
        {
        strcpy (log_name, "ESI$CONTOURS");
        strcpy (fi_type, "CONTOUR");
        clipping = TRUE;
        }
    else
        {
        strcpy (log_name, "ESI$PERSPECTIVES");
        strcpy (fi_type, "PERSPECTIVE");
        clipping = TRUE;
        }
    /* * INIT AND READ LINES NLIST * */
    /* ----------------------------- */
    
    strcpy (desc, "");
    if ((status = fm_init_file (log_name, contours->contour_file, fi_type, "r", -1,
                                desc, NULL_FUNCTION_PTR, file_name,
                                "MAPPING")) != SUCCESS)
        {
        goto finished;
        }
    if (nl_read_nlist (&contour_lines, file_name, "LINES") != SUCCESS)
        {
        status = CO_CONTOUR_FILE_ERROR;
        goto finished;
        }
    nl_inq_nlist_user_struct (contour_lines, sizeof(CONTOUR_NLIST_USER_STRUCT),
                              (VOIDPTR) & contour_user, (size_t *) & i);
    if (ARE_DIFFERENT (contour_user.projection, map->map_projection))
        {
        status = OV_PROJECTION_MISMATCH;
        goto finished;
        }
    for (i = 0; i < 2; i++)
        {
        units[i] = 0;
        d_size[i] = 4;
        d_type[i] = FLOAT_TYPE;
        v_type[i] = VALUE_LIST;
        d_list[i] = i + 1;
        }
    status = nl_init_nlist (&line_nlist, 2, units, d_size, d_type, v_type, blocksize);
    if (status != SUCCESS)
        {
        return status;
        }
    status = nl_set_nlist_info (line_nlist, NL_NLIST_COLUMN_NAME, 2, d_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    v_list[0] = (VOIDPTR) & xnull;
    v_list[1] = (VOIDPTR) & ynull;
    status = nl_inq_nlist_info (contour_lines, NL_NLIST_NULL_VALUE, 2, d_list, v_list);
    status = nl_set_nlist_info (line_nlist, NL_NLIST_NULL_VALUE, 2, d_list, v_list);
    v_list[0] = (VOIDPTR) & contour_xmin;
    v_list[1] = (VOIDPTR) & contour_ymin;
    status = nl_inq_nlist_info (contour_lines, NL_NLIST_MIN, 2,
				d_list, v_list);
    v_list[0] = (VOIDPTR) & contour_xmax;
    v_list[1] = (VOIDPTR) & contour_ymax;
    status = nl_inq_nlist_info (contour_lines, NL_NLIST_MAX, 2,
				d_list, v_list);
    
    /* * FILL INTERVAL AND COLOR VARIABLES FROM NLIST FILE * */
    /* ----------------------------------------------------- */
    
    if (contours->use_file_defaults)
        {
        bold_line_interval = contour_user.bold_line_interval;
        label_interval = contour_user.label_interval;
        bold_line_color = contour_user.bold_line_color;
        fine_line_color = contour_user.fine_line_color;
        }
    /* * FILL INTERVAL AND COLOR VARIABLES FROM CONTOUR INFO STRUCT * */
    /* -------------------------------------------------------------- */
    
    else
        {
        bold_line_interval = contours->bold_line_interval;
        label_interval = contours->label_interval;
        bold_line_color = contours->bold_line_color;
        fine_line_color = contours->fine_line_color;
        }
    /* * INITIALIZE GKS TEXT PARAMETERS * */
    /* ---------------------------------- */
    
    if (label_interval)
        {
        wm_gstxal (GALEFT, GABASE);
        wm_gstxfp (SIMPLEX, GSTRKP);
        wm_gstxp (GRIGHT);
        wm_gschxp (1.0);
        
        dummy = 0.01 * (map->upper_right_xy.y - map->lower_left_xy.y);
        wm_gschh (dummy);
        }
    /* * GET MAP WINDOW * */
    /* ------------------ */
    
    clip.Col_List[0] = 1;
    clip.Col_List[1] = 2;
    clip.minList[0] = map->lower_left_xy.x;
    clip.minList[1] = map->lower_left_xy.y;
    clip.maxList[0] = map->upper_right_xy.x;
    clip.maxList[1] = map->upper_right_xy.y;
    clip.autopurge = TRUE;
    
    /* ------------------------------------------------ */
    /* print out a message when map area of interest is */
    /* not the same as contour nlist file               */
    /* ------------------------------------------------ */
    if (map->lower_left_xy.x != contour_xmin OR
        map->lower_left_xy.y != contour_ymin OR
        map->upper_right_xy.x != contour_xmax OR
        map->upper_right_xy.y != contour_ymax)
        {
        sprintf (message,"'%s' overlay does not exactly match the map area of interest.",contours->contour_file);
        am_message (AM_STATUS, message);
	}

    /* * PROCESS LINES TABLE BY TABLE * */
    /* ---------------------------------- */
    
    adjust = 0.5 * contour_user.contour_interval;
    lu_text_to_index ("THICKNESS", &fine_code, "FINE");
    lu_text_to_index ("THICKNESS", &bold_code, "BOLD");
    lu_text_to_index ("LINE_STYLE", &lineStyle, "SOLID");
    wm_gsln (lineStyle);
    
    nl_inq_nlist_int (contour_lines, NL_NLIST_NTABLES, &num_tables);
    for (i = 1; i <= num_tables; i++)
        {
        wmsallocate (&segment_id);
        wmoset (segment_id, WM_CLASS_LINE, "", "CONTOUR");
        
        /* GET LINES TABLE USER STRUCT */
        
        nl_inq_table_user_struct (contour_lines, i, sizeof(DG_LINE_STROKE_USER_STRUCT),
                                  (VOIDPTR) & line_stroke_user, (size_t *) & j);
        
        interval_num = (fabs (line_stroke_user.value) + adjust) /
                       contour_user.contour_interval;
        
        /* SET LINE THICKNESS AND COLOR */
        
        if (bold_line_interval)
            {
            /* AT A BOLD LINE */
            
            if (interval_num % bold_line_interval EQUALS 0)
                {
                if (crnt_thick != bold_code)
                    {
                    crnt_thick = (DOUBLE) bold_code;
                    wm_gslwsc (crnt_thick);
                    }
                if (crnt_color != bold_line_color)
                    {
                    crnt_color = bold_line_color;
                    wm_gsplci (crnt_color);
                    }
                }
            /* AT A FINE LINE */
            else
                {
                if (crnt_thick != fine_code)
                    {
                    crnt_thick = (DOUBLE) fine_code;
                    wm_gslwsc (crnt_thick);
                    }
                if (crnt_color != fine_line_color)
                    {
                    crnt_color = fine_line_color;
                    wm_gsplci (crnt_color);
                    }
                }
            }
        /* * LABELED TABLE CHECKING AND TABLE POSTING * */
        /* ---------------------------------------------- */
        
        if (label_interval AND (interval_num % label_interval EQUALS 0))
            {
            if (crnt_color != crnt_text_color)
                {
                crnt_text_color = crnt_color;
                wm_gstxci (crnt_text_color);
                }
            co_post_labels (file_name, contour_lines, &line_nlist, i, clip.minList,
                            clip.maxList, line_stroke_user.level);
            }
        else
            {
            nl_copy_table (contour_lines, i, line_nlist, &j);
            nl_purge_table (contour_lines, i);  /* release internal storage */
            }
        /* incorporating gordon's change  preventing bogus nl_empty_nlist status. CP */
        if (clipping)
            {
            if (nl_invoke_nlist_method (line_nlist, NL_CLIP_METHOD, &clip) == SUCCESS)
                {
                nl_draw_nlist (clip.outNlist, d_list[0], d_list[1]);
                nl_free_nlist (clip.outNlist);
                }
            }
        else
            {
            nl_draw_nlist (line_nlist, d_list[0], d_list[1]);
            }
        nl_clear_nlist (line_nlist);
        wmsclose (segment_id);
        }
    /* * FINISH UP * */
    /* ------------- */
    
finished:
    fm_release_file (log_name, contours->contour_file, fi_type);
    nl_free_nlist (contour_lines);
    nl_free_nlist (line_nlist);
    
    return status;
    }
/* END:     */
