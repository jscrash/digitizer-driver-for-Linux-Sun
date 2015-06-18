/* DEC/CMS REPLACEMENT HISTORY, Element CO_POST_LAB.C*/
/*  6    11-MAR-1992 18:13:51 JTM "(SPR 0) checked changes for Don's additions"*/
/*  5    16-JAN-1992 12:05:26 JTM "(SPR 0)  Contour Annotation code from Don V."*/
/* *4    10-APR-1991 14:23:53 MING "(SPR -1) fix labelling angle problem"*/
/* *3    14-AUG-1990 23:37:55 GILLESPIE "(SPR 5644) Fix up last checkin"*/
/* *2    14-AUG-1990 13:06:11 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:43:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_POST_LAB.C*/
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

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#include "esi_c_lib.h"

#ifndef ESI_CO_H
#include "esi_co.h"
#endif

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to post the contour labels.

Prototype:
    publicdef INT co_post_labels(FILENAME file_name, NLIST_HEADER in_lines, 
                             NLIST_HEADER *out_lines, INT stroke, 
                             DOUBLE mins, DOOUBLE maxs, INT level);
                             
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) file_name   -(FILENAME) N-list file name.
    (I) in_lines    -(NLIST_HEADER) Incoming n-list for contour lines.
    (O) out_lines   -(NLIST_HEADER) Outgoing n-list for contour lines.
    (I) stroke      -(INT) Lines n-list stroke number.
    (I) mins        -(DOUBLE [2]) X and y minimums.
    (I) maxs        -(DOUBLE [2]) X and y maximums.
    (I) level       -(INT) Level number.

Return Value/Status:
    SUCCESS - Successful completion
    CO_NO_LABELS_FOR_CONTOUR - Error if n-list of contour names is empty.
    Function also returns status from nl_add_points and nl_get_points.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_post_labels (FILENAME file_name, NLIST_HEADER in_lines,
                              NLIST_HEADER *out_lines, INT stroke, DOUBLE mins[2],
                              DOUBLE maxs[2], INT level) 
#else
publicdef INT co_post_labels (file_name, in_lines, out_lines, stroke, mins, maxs,
                              level)
FILENAME file_name;
NLIST_HEADER in_lines;
NLIST_HEADER *out_lines;
INT stroke;
DOUBLE mins[2];
DOUBLE maxs[2];
INT level;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    NLIST_HEADER labels;            /* INITIALIZED LABEL NLIST */
    INT status;                     /* RETURN STATUS */
    CHAR nlist_name[10];            /* LABEL NLIST NAME */
    DG_LABEL_STROKE_USER_STRUCT lbl_stroke_user;    /* STROKE USER STRUCT FOR LABELS */
    FLOAT *linex = 0;               /* X ARRAY FOR LINE */
    FLOAT *liney;                   /* Y ARRAY FOR LINE */
    FLOAT *linex_save;              /* SAVED POINTER FOR FREEING */
    FLOAT *liney_save;              /* SAVED POINTER FOR FREEING */
    FLOAT *outx;                    /* OUTPUT X ARRAY */
    FLOAT *outy;                    /* OUTPUT Y ARRAY */
    FLOAT *outx_save;               /* SAVED POINTER FOR FREEING */
    FLOAT *outy_save;               /* SAVED POINTER FOR FREEING */
    static FLOAT xnull;         /* NULL VALUE FOR X */
    static FLOAT ynull;         /* NULL VALUE FOR Y */
    static BOOL null_inited = FALSE;    /* NULL VALUES INITIALIZED FLAG */
    FLOAT xmin;                     /* MINIMUM VALUE FOR LABEL X */
    FLOAT xmax;                     /* MAXIMUM VALUE FOR LABEL X */
    FLOAT ymin;                     /* MINIMUM VALUE FOR LABEL Y */
    FLOAT ymax;                     /* MAXIMUM VALUE FOR LABEL Y */
    FLOAT label_x[20];              /* X ARRAY FOR LABEL CHARACTER */
    FLOAT label_y[20];              /* X ARRAY FOR LABEL CHARACTER */
    CHAR label_c[20];               /* LABEL CHARACTER ARRAY */
    FLOAT angle[20];                /* ANGLE ARRAY FOR LABEL CHARACTERS */
    FLOAT old_angle;                /* PREVIOUS CHARACTER ANGLE */
    INT num_labels;                 /* NUMBER OF LABELS STORED */
    INT num_lbl_pnts;               /* NUMBER OF POINTS IN LABEL */
    INT num_line_pnts;              /* NUMBER OF POINTS IN LINE */
    UINT dummy;                     /* DUMMY INTEGER */
    INT i;                          /* DUMMY INDEX */
    INT j;                          /* DUMMY INDEX */
    INT k;                          /* DUMMY INDEX */
    UINT stroke_num;
    INT ii;
    INT end_of_line;
    INT last_point;
    INT npts;
    CHAR text[2];
    FLOAT text_up_x;
    FLOAT text_up_y;
    FLOAT last_text_up_x = 1.0;
    FLOAT last_text_up_y = -1.0;
    
    /* NLIST VARIABLES */
    static UINT d_list[4] = 
        {
        1,
        2,
        3,
        4
        };                          /*  LINE/LABEL DIM LIST */
    static UINT ext_d_list[2] = 
        {
        3,
        4
        };                          /*  MIN/MAX EXTENT DIM LIST */
    VOIDPTR lbl_v_list[4];          /*  LABEL VALUE LIST */
    VOIDPTR line_v_list[2];         /*  LINE VALUE LIST */
    VOIDPTR min_v_list[2];          /*  MINS VALUE LIST */
    VOIDPTR max_v_list[2];          /*  MAXS VALUE LIST */
    
    /* UP_X AND UP_Y VALUES TO USE */
    
    static FLOAT up_x[10] = 
        {
        0.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0,
        -1.0
        };
    static FLOAT up_y[10] = 
        {
        1.0,
        5.67,
        2.75,
        1.73,
        1.19,
        0.84,
        0.58,
        0.36,
        0.18,
        0.0
        };
    
    /* ********************************************************************** */
    
    /* * GET NLIST NAME * */
    /* ------------------ */
    
    i = 0;
    FOREVER
        {
        if (level EQUALS 0)
            {
            break;
            }
        nlist_name[i] = level % 10 + '0';
        level /= 10;
        i++;
        }
    nlist_name[i] = '\0';
    
    /* * READ IN NLIST * */
    /* ----------------- */
    
    status = nl_read_nlist (&labels, file_name, nlist_name);
    if (status < 0)
        {
        nl_copy_stroke (in_lines, stroke, *out_lines, &stroke_num);
        status = CO_NO_LABELS_FOR_CONTOUR;
        goto finished;
        }
    /* * GET LINE NULL VALUES FOR PROCESSING * */
    /* --------------------------------------- */
    
    if (NOT null_inited)
        {
        line_v_list[0] = (VOIDPTR) & xnull;
        line_v_list[1] = (VOIDPTR) & ynull;
        nl_inq_nlist_info (in_lines, NL_NLIST_NULL_VALUE, 2, d_list, line_v_list);
        null_inited = TRUE;
        }
    /* * ALLOCATE AND FILL LINE COORDINATE ARRAYS * */
    /* -------------------------------------------- */
    
    nl_inq_stroke_int (in_lines, stroke, NL_STROKE_NPOINTS, &num_line_pnts);
    linex = (FLOAT *)am_allocate (AM_APPLICATION, num_line_pnts * sizeof(FLOAT));
    liney = (FLOAT *)am_allocate (AM_APPLICATION, num_line_pnts * sizeof(FLOAT));
    
    linex_save = linex;
    liney_save = liney;
    
    line_v_list[0] = (VOIDPTR)linex;
    line_v_list[1] = (VOIDPTR)liney;
    nl_set_current_point (in_lines, 1, stroke);
    status = nl_get_points (in_lines, num_line_pnts, 2, d_list, line_v_list, &dummy);
    
    /* * ALLOCATE OUTPUT ARRAYS * */
    /* -------------------------- */
    
    nl_inq_nlist_int (labels, NL_NLIST_NSTROKES, &num_labels);
    outx = (FLOAT *)am_allocate (AM_APPLICATION,
                                 (num_line_pnts + 2 * num_labels) * sizeof(FLOAT));
    outy = (FLOAT *)am_allocate (AM_APPLICATION,
                                 (num_line_pnts + 2 * num_labels) * sizeof(FLOAT));
    
    outx_save = outx;
    outy_save = outy;
    
    /* * SET V_LIST POINTERS * */
    /* ----------------------- */
    
    lbl_v_list[0] = (VOIDPTR) & label_c[0];
    lbl_v_list[1] = (VOIDPTR) & angle[0];
    lbl_v_list[2] = (VOIDPTR) & label_x[0];
    lbl_v_list[3] = (VOIDPTR) & label_y[0];
    
    min_v_list[0] = (VOIDPTR) & xmin;
    min_v_list[1] = (VOIDPTR) & ymin;
    
    max_v_list[0] = (VOIDPTR) & xmax;
    max_v_list[1] = (VOIDPTR) & ymax;
    
    /* * PROCESS LABEL NLIST STROKE BY STROKE (LABEL BY LABEL) * */
    /* --------------------------------------------------------- */
    
    last_point = 0;
    text[1] = '\0';
    for (i = 1; i <= num_labels; i++)
        {
        
        /* CHECK LABEL INSIDE WINDOW */
        
        nl_inq_stroke_info (labels, i, NL_STROKE_MIN, 2, ext_d_list, min_v_list);
        nl_inq_stroke_info (labels, i, NL_STROKE_MAX, 2, ext_d_list, max_v_list);
        
        if ((xmin >= mins[0]) AND (ymin >= mins[1]) AND (xmax <=
                                                       maxs[0]) AND (ymax <= maxs[1]))
            {
            
            /* GET LABEL STROKE USER STRUCT */
            
            nl_inq_stroke_user_struct (labels, i, sizeof(DG_LABEL_STROKE_USER_STRUCT),
                                       &(lbl_stroke_user), (size_t *) & dummy);
            
            /* * TRANSFER CONTOUR LINE UP TO LABEL * */
            /* ------------------------------------- */
            
            linex = linex_save + last_point;
            liney = liney_save + last_point;
            
            npts = lbl_stroke_user.start_ptr - last_point;
            last_point = lbl_stroke_user.end_ptr - 1;
            
            hoblockmove (linex, outx, npts * sizeof(FLOAT));
            hoblockmove (liney, outy, npts * sizeof(FLOAT));
            
            outx += npts;
            outy += npts;
            
            end_of_line = FALSE;
            for (ii = lbl_stroke_user.start_ptr; ii < lbl_stroke_user.end_ptr; ii++)
                if (abs (linex_save[ii] - xnull) < 0.0001)
                    end_of_line = TRUE;
                
                /* * PROCESS LABEL * */
                /* ----------------- */
                
                /* GET THE CURRENT LABEL */
                
            nl_set_current_point (labels, 1, i);
            nl_inq_stroke_int (labels, i, NL_STROKE_NPOINTS, &num_lbl_pnts);
            nl_get_points (labels, num_lbl_pnts, 4, d_list, lbl_v_list, &dummy);
            
            /* LABEL POSTING LOOP */
            
            old_angle = 0.0;
            text_up_x = 0.0;
            text_up_y = 1.0;
            for (k = 0; k < num_lbl_pnts; k++)
                {
                /* CHARACTER AS STRING */
                
                text[0] = label_c[k];
                
#if 0
                /* CHARACTER UP VECTOR */
                
                if (fabs (old_angle - angle[k]) > 8.0)
                    {
                    old_angle = fabs (angle[k]);
                    for (j = 0; j <= 90; j += 10)
                        {
                        if (j - 5 < old_angle AND old_angle <= j + 5)
                            {
                            text_up_x = up_x[(j / 10)];
                            text_up_y = up_y[(j / 10)];
                            break;
                            }
                        }
                    if (angle[k] < 0)
                        {
                        old_angle *= -1.0;
                        text_up_x *= -1.0;
                        }
                    }
           
                /* DRAW CHARACTER */
                
                if (text_up_x != last_text_up_x OR text_up_y != last_text_up_y)
                    {
                    last_text_up_x = text_up_x;
                    last_text_up_y = text_up_y;
                    wm_gschup (text_up_x, text_up_y);
                    }
#endif
		/* ------------------------------------------------------- */
		/* the above codes are taken off because they can't handle */
		/* all cases of different angles. Here we use the x,y up   */
		/* vector directly from the angle generated by DGI to solve*/
		/* the problem.                                            */
		/* ------------------------------------------------------- */
		text_up_x = (FLOAT) -sin (D_TO_R (angle[k]));
                text_up_y = (FLOAT) cos (D_TO_R (angle[k]));
                wm_gschup (text_up_x, text_up_y);
                wm_gtx (label_x[k], label_y[k], text);
                }
            *outx = xnull;
            outx++;
            *outy = ynull;
            outy++;
            
            if (!end_of_line)
                {
                *outx = lbl_stroke_user.xend;
                outx++;
                *outy = lbl_stroke_user.yend;
                outy++;
                }
            }
        }
    /* * ADD END OF CONTOUR LINE * */
    /* --------------------------- */
    
    npts = num_line_pnts - last_point;
    if (npts > 0)
        {
        linex = linex_save + last_point;
        liney = liney_save + last_point;
        
        hoblockmove (linex, outx, npts * sizeof(FLOAT));
        hoblockmove (liney, outy, npts * sizeof(FLOAT));
        }
    else
        {
        outx -= 2;
        outy -= 2;
        }
    /* * WRITE OUT OUTPUT LINES * */
    /* -------------------------- */
    
    nl_start_stroke (*out_lines, &dummy);
    npts += (outx - outx_save);
    
    line_v_list[0] = (VOIDPTR)outx_save;
    line_v_list[1] = (VOIDPTR)outy_save;
    status = nl_add_points (*out_lines, npts, 2, d_list, line_v_list);
    
    /* * FREE MEMORY * */
    /* --------------- */
    
    am_free (linex_save);
    am_free (liney_save);
    am_free (outx_save);
    am_free (outy_save);
    nl_free_nlist (labels);
    
    /* * RETURN * */
    /* ---------- */
    
finished:
    return status;
    }
/* END:     */
