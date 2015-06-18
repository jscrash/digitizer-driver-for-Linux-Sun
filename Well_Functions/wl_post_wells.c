/* DEC/CMS REPLACEMENT HISTORY, Element WL_POST_WELLS.C*/
/* *16   18-OCT-1991 16:27:22 JTM "(SPR 0) Fixes by Charlie for WELL DEPTH and ISOPACH - SOURCE option was ignored. Case 4568."*/
/* *15    7-AUG-1991 14:43:06 KEE "(SPR 8686) Fix well posting inconsistent bug (case 3388)"*/
/* *14   30-JUL-1991 15:33:23 CHARLIE "(SPR 8704) more clipping for deviated well text"*/
/* *13   23-MAY-1991 10:07:52 MING "(SPR -1) trap wells->ov_type = depth or isopach regenerate tvd label (CHARLIE)"*/
/* *12   29-SEP-1990 12:01:21 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  10B1 24-SEP-1990 17:33:43 GILLESPIE "Beta Deltas"*/
/* *11   14-AUG-1990 13:42:33 VINCE "(SPR 5644) Header Standardization"*/
/* *10   13-FEB-1990 17:29:10 GILLESPIE "(SPR 1) Fix ncol argument in nl_init_nlist (from 7 to 8)"*/
/* *9     9-FEB-1990 16:26:12 MING "(SPR 6007) change node id declaration to long int"*/
/* *8    17-JAN-1990 23:03:31 GILLESPIE "(SPR 6001) Changes for new wells struct stuff"*/
/* *7    17-JAN-1990 15:41:20 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *6    17-NOV-1989 14:13:44 GILLESPIE "(SPR 1) Change blocksize to 500; qualify the SOURCE column"*/
/* *5    14-SEP-1989 18:44:23 GORDON "(SPR -1) checkin from gulf"*/
/* *4     4-AUG-1989 17:18:46 VINCE "(SPR -1) added initialization from Gulf mods."*/
/* *3    20-JUL-1989 11:04:42 GORDON "(SPR 100) Sun port mods"*/
/* *2    30-JUN-1989 09:52:52 GILLESPIE "(SPR 32) Remove word 'go'"*/
/* *1    19-JUN-1989 13:40:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_POST_WELLS.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"

#ifndef ESI_WL_H
#include "esi_wl.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h" 
#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function to post well symbols and annotations.                                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wl_post_wells(WELLS *wells,NLIST_HEADER nodev_nlist,NLIST_HEADER dev_nlist, */
/*        PICKABLE_WELLS *picks);                                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    wells           -(WELLS *) Well overlay structure.                            */
/*    nodev_nlist     -(NLIST_HEADER)                                               */
/*    dev_nlist       -(NLIST_HEADER)                                               */
/*    picks           -(PICKABLE_WELLS *)                                           */
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
publicdef INT wl_post_wells (WELLS *wells, NLIST_HEADER nodev_nlist,
                             NLIST_HEADER dev_nlist, PICKABLE_WELLS *picks)
#else
publicdef INT wl_post_wells (wells, nodev_nlist, dev_nlist, picks)
WELLS *wells;
NLIST_HEADER nodev_nlist;
NLIST_HEADER dev_nlist;
PICKABLE_WELLS *picks;
#endif
    {
    
    INT status;                     /* RETURN STATUS                                */
    INT pstatus;
    SEGMENT_ID segment_id = 0;      /* WINDOW MANAGER SEGMENT ID                    */
    
    UWI *uwi = (UWI *)0;
    INT *deviated = (INT *)0;
    FLOAT *base_x = (FLOAT *)0;
    FLOAT *base_y = (FLOAT *)0;
    FLOAT *offset_x = (FLOAT *)0;
    FLOAT *offset_y = (FLOAT *)0;
    FLOAT *up_x = (FLOAT *)0;
    FLOAT *up_y = (FLOAT *)0;
    
    CHAR *sql = NULL;               /* SQL STATEMENT                                */
    FLOAT z;                        /* Z VALUE TO WRITE TO SCATTER                  */
    INT i;
    UINT j;
    
    FLOAT text_up_x = 0.0;
    FLOAT text_up_y = 1.0;
    FLOAT nom_up_x = 0.0;
    FLOAT nom_up_y = 1.0;
    FLOAT x_up, y_up;
    
    INT hor_align;
    INT vert_align;
    INT nom_hor_al = 0;             /* initialization added by GULF                 */
    INT nom_vert_al = 0;
    INT align_x = 0, align_y = 0;
    
    FLOAT nom_x_offset;
    FLOAT nom_y_offset;
    UINT num_labels;
    CHAR *label;
    FLOAT label_x;
    FLOAT label_y;
    NLIST_HEADER label_nlist = (NLIST_HEADER)0;
    BOOL suppress_text;
    TCP label_tcp;
    TCP sql_tcp;
    DOUBLE md, elevation, dx, dy, tvd, base_tvd;
    INT label_status;
    MAP_STRUCTURE *map;
    DOUBLE ll_x, ll_y, ur_x, ur_y;
    
    /* NLIST VARIABLES                                                              */
    static INT units[8] = 
        {
        0, 0, 0, 0, 0, 0, 0, 0
        };
    static size_t d_size[8] = 
        {
        sizeof(UWI), sizeof(INT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT),
        sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT)
        };
    static INT d_type[8] = 
        {
        CHAR_TYPE, INT_TYPE, FLOAT_TYPE, FLOAT_TYPE, FLOAT_TYPE, FLOAT_TYPE,
        FLOAT_TYPE, FLOAT_TYPE
        };
    static INT v_type[8] = 
        {
        VALUE_LIST, VALUE_LIST, VALUE_LIST, VALUE_LIST, VALUE_LIST, VALUE_LIST,
        VALUE_LIST, VALUE_LIST
        };
    static UINT d_list[8] = 
        {
        1, 2, 3, 4, 5, 6, 7, 8
        };
    static CHAR *c_name[9] = 
        {
        "UWI", "DEVIATED", "BASE_X", "BASE_Y", "OFFSET_X", "OFFSET_Y", "UP_X", "UP_Y",
        NULL
        };
    VOIDPTR v_list[8];
    INT blocksize = 500;
    
    /* **********************************************************************       */
    
    status = SUCCESS;
    mp_get_current_map(&map);
    ll_x = map->lower_left_xy.x;
    ll_y = map->lower_left_xy.y;
    ur_x = map->upper_right_xy.x;
    ur_y = map->upper_right_xy.y;

    /* * INIT LABELLING NLIST *                                                     */
    /* ------------------------                                                     */
    
    suppress_text = wells->suppress_text;
    if (NOT wells->suppress_text AND NOT wells->above AND NOT wells->
        below AND NOT wells->left AND NOT wells->right AND NOT wells->centered)
        {
        suppress_text = TRUE;
        }
    nl_init_nlist (&label_nlist, 8, units, d_size, d_type, v_type, blocksize);
    status = nl_set_nlist_info (label_nlist, NL_NLIST_COLUMN_NAME, 8, d_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_stroke (label_nlist, (UINT *)&i);
    
    /* * POST SYMBOLS *                                                             */
    /* ----------------                                                             */
    
    if (NOT wells->suppress_symbols)
        {
        wmsallocate (&segment_id);
        wmoset (segment_id, WM_CLASS_WELL_SYMBOL, "", "SYMBOL");
        }
    picks->seg_id = segment_id;
    wl_do_nodev_wells (wells, nodev_nlist, suppress_text, label_nlist, picks);
    wl_do_deviated_wells (wells, dev_nlist, suppress_text, label_nlist, picks);
    
    if (NOT wells->suppress_symbols)
        {
        wmsclose (segment_id);
        segment_id = 0;
        }
    /* * POST ANNOTATIONS *                                                         */
    /* --------------------                                                         */
    
    if (suppress_text AND wells->zfield EQUALS 0)
        {
        label = NULL;
        goto finished;
        }
    label = (CHAR *) TALLOC (241);
    
    /* * INIT GKS TEXT ATTRIBUTES *                                                 */
    /* ----------------------------                                                 */
    
    if (NOT suppress_text)
        {
        wm_gstxci (wells->text_color);
        wm_gschh (wells->text_size);
        wm_gschup (text_up_x, text_up_y);
        
        i = 0;
        wm_gstxp (i);
        
        wm_gstxfp (SIMPLEX, GSTRKP);
        
        label_x = 1.0;
        wm_gschxp (label_x);
        
        label_x = 0.0;
        wm_gschsp (label_x);
        
        }
    /* * GET LABEL NLIST INFO *                                                     */
    /* ------------------------                                                     */
    
    nl_inq_nlist_int (label_nlist, NL_NLIST_NPOINTS, (INT *)&num_labels);
    uwi = (UWI *) TALLOC (num_labels * sizeof(UWI));
    deviated = (INT *) TALLOC (num_labels * sizeof(INT));
    base_x = (FLOAT *) TALLOC (num_labels * sizeof(FLOAT));
    base_y = (FLOAT *) TALLOC (num_labels * sizeof(FLOAT));
    offset_x = (FLOAT *) TALLOC (num_labels * sizeof(FLOAT));
    offset_y = (FLOAT *) TALLOC (num_labels * sizeof(FLOAT));
    up_x = (FLOAT *) TALLOC (num_labels * sizeof(FLOAT));
    up_y = (FLOAT *) TALLOC (num_labels * sizeof(FLOAT));
    
    v_list[0] = (VOIDPTR)uwi;
    v_list[1] = (VOIDPTR)deviated;
    v_list[2] = (VOIDPTR)base_x;
    v_list[3] = (VOIDPTR)base_y;
    v_list[4] = (VOIDPTR)offset_x;
    v_list[5] = (VOIDPTR)offset_y;
    v_list[6] = (VOIDPTR)up_x;
    v_list[7] = (VOIDPTR)up_y;
    
    nl_set_current_point (label_nlist, 1, 1);
    nl_get_points (label_nlist, num_labels, 8, d_list, v_list, &j);
    
    /* * LOOP ONCE FOR EACH POSSIBLE LABEL POSITION *                               */
    /* ----------------------------------------------                               */
    
    for (j = 1; j <= 5; j++)
        {
        /* 0 or WL_DO_TCP for stack options                                         */
        pstatus = 0;
        switch (j)
            {
            /* * ABOVE *                                                            */
            /* ---------                                                            */
            
        case 1:
            /* TEST FOR LABELLING THIS POSITION                                     */
            
            if (wells->above EQUALS NULL)
                {
                continue;
                }
            /* INITIALIZE SQL STATEMENT STRING                                      */
            
            if (wells->ov_type)
                {
                sql = STRING_ALLOCATE (wells->above);
                strcpy (sql, wells->above);
                }
            else
                {
                pstatus = wl_parse_label (wells->above, &sql);
                if (pstatus == WL_DO_TCP)
                    {
                    status = wl_parse_label_tcp (sql, wells->source, &sql_tcp);
                    }
                if (pstatus < 0 OR status < 0)
                    {
                    goto finished;
                    }
                }
            /* INITIALIZE LABEL VARIABLES                                           */
            
            if (NOT suppress_text)
                {
                wmsallocate (&segment_id);
                wmoset (segment_id, WM_CLASS_TEXT, "", "ABOVE");
                nom_hor_al = 2;
                nom_vert_al = 5;
                nom_x_offset = 0.0;
                nom_y_offset = 0.75 * MAX (wells->symbol_size, wells->text_size);
                }
            break;
            
            /* * BELOW *                                                            */
            /* ---------                                                            */
            
        case 2:
            /* TEST FOR LABELLING THIS POSITION                                     */
            
            if (wells->below EQUALS NULL)
                {
                continue;
                }
            /* INITIALIZE SQL STATEMENT STRING                                      */
            
            if (wells->ov_type)
                {
                sql = STRING_ALLOCATE (wells->below);
                strcpy (sql, wells->below);
                }
            else
                {
                pstatus = wl_parse_label (wells->below, &sql);
                if (pstatus == WL_DO_TCP)
                    {
                    status = wl_parse_label_tcp (sql, wells->source, &sql_tcp);
                    }
                if (pstatus < 0 OR status < 0)
                    {
                    goto finished;
                    }
                }
            /* INITIALIZE LABEL VARIABLES                                           */
            
            if (NOT suppress_text)
                {
                wmsallocate (&segment_id);
                wmoset (segment_id, WM_CLASS_TEXT, "", "BELOW");
                nom_hor_al = 2;
                nom_vert_al = 1;
                nom_x_offset = 0.0;
                nom_y_offset = (-0.75) * MAX (wells->symbol_size, wells->text_size);
                }
            break;
            
            /* * CENTERED *                                                         */
            /* ------------                                                         */
            
        case 3:
            /* TEST FOR LABELLING THIS POSITION                                     */
            
            if (wells->centered EQUALS NULL)
                {
                continue;
                }
            /* INITIALIZE SQL STATEMENT STRING                                      */
            
            if (wells->ov_type)
                {
                sql = STRING_ALLOCATE (wells->centered);
                strcpy (sql, wells->centered);
                }
            else
                {
                pstatus = wl_parse_label (wells->centered, &sql);
                if (pstatus == WL_DO_TCP)
                    {
                    status = wl_parse_label_tcp (sql, wells->source, &sql_tcp);
                    }
                if (pstatus < 0 OR status < 0)
                    {
                    goto finished;
                    }
                }
            /* INITIALIZE LABEL VARIABLES                                           */
            
            if (NOT suppress_text)
                {
                wmsallocate (&segment_id);
                wmoset (segment_id, WM_CLASS_TEXT, "", "CENTERED");
                nom_hor_al = 2;
                nom_vert_al = 3;
                nom_x_offset = 0.0;
                nom_y_offset = 0.0;
                }
            break;
            
            /* * LEFT *                                                             */
            /* --------                                                             */
            
        case 4:
            /* TEST FOR LABELLING THIS POSITION                                     */
            
            if (wells->left EQUALS NULL)
                {
                continue;
                }
            /* INITIALIZE SQL STATEMENT STRING                                      */
            
            if (wells->ov_type)
                {
                sql = STRING_ALLOCATE (wells->left);
                strcpy (sql, wells->left);
                }
            else
                {
                pstatus = wl_parse_label (wells->left, &sql);
                if (pstatus == WL_DO_TCP)
                    {
                    status = wl_parse_label_tcp (sql, wells->source, &sql_tcp);
                    }
                if (pstatus < 0 OR status < 0)
                    {
                    goto finished;
                    }
                }
            /* INITIALIZE LABEL VARIABLES                                           */
            
            if (NOT suppress_text)
                {
                wmsallocate (&segment_id);
                wmoset (segment_id, WM_CLASS_TEXT, "", "LEFT");
                nom_hor_al = 3;
                nom_vert_al = 3;
                nom_x_offset = (-0.75) * wells->symbol_size;
                nom_y_offset = 0.0;
                }
            break;
            
            /* * RIGHT *                                                            */
            /* ---------                                                            */
            
        case 5:
            /* TEST FOR LABELLING THIS POSITION                                     */
            
            if (wells->right EQUALS NULL)
                {
                continue;
                }
            /* INITIALIZE SQL STATEMENT STRING                                      */
            
            if (wells->ov_type)
                {
                sql = STRING_ALLOCATE (wells->right);
                strcpy (sql, wells->right);
                }
            else
                {
                pstatus = wl_parse_label (wells->right, &sql);
                if (pstatus == WL_DO_TCP)
                    {
                    status = wl_parse_label_tcp (sql, wells->source, &sql_tcp);
                    }
                if (pstatus < 0 OR status < 0)
                    {
                    goto finished;
                    }
                }
            /* INITIALIZE LABEL VARIABLES                                           */
            
            if (NOT suppress_text)
                {
                wmsallocate (&segment_id);
                wmoset (segment_id, WM_CLASS_TEXT, "", "RIGHT");
                nom_hor_al = 1;
                nom_vert_al = 3;
                nom_x_offset = 0.75 * wells->symbol_size;
                nom_y_offset = 0.0;
                }
            break;
            }
        hor_align = nom_hor_al;
        vert_align = nom_vert_al;
        wm_gstxal (hor_align, vert_align);
        
        /* * LOOP ON WELLS *                                                        */
        /* -----------------                                                        */
        
        for (i = 0; i < num_labels; i++)
            {
            if (deviated[i])
                {
                if (up_x[i] EQUALS 0.0 AND up_y[i] EQUALS 0.0)
                    {
                    deviated[i] = FALSE;
                    }
                else if (wells->ov_type != 0)
                    {
                    label_x = base_x[i] + offset_x[i];
                    label_y = base_y[i] + offset_y[i];
                    align_x = 1;
                    align_y = 3;
                    x_up = up_x[i];
                    y_up = up_y[i];
                    }
                }
            if (NOT deviated[i] OR wells->ov_type EQUALS 0)
                {
                label_x = base_x[i] + nom_x_offset;
                label_y = base_y[i] + nom_y_offset;
                align_x = nom_hor_al;
                align_y = nom_vert_al;
                x_up = nom_up_x;
                y_up = nom_up_y;
                }
            if (align_x != hor_align OR align_y != vert_align)
                {
                hor_align = align_x;
                vert_align = align_y;
                wm_gstxal (hor_align, vert_align);
                }
            if (x_up != text_up_x OR y_up != text_up_y)
                {
                text_up_x = x_up;
                text_up_y = y_up;
                wm_gschup (text_up_x, text_up_y);
                }
            /* GET LABEL, IF ANY                                                    */
/* (CP) here is the 'quick' fix. We regenerate the variable 'label'
   IF wells->ov_type != 0 and pstatus != WL_DO_TCP. The FINAL version
   should hook into both wl_get_label_tcp and wl_get_label (also need to
   keep in mind the well depth editing function needs to know where strings are
   located so they can be 'erased'). The sql string will need to
   be examined for such things as 'SELECT ELEVATION-MD' for subsea_depth
   then handle specially for deviated wells. Same for isopach. */

            if (pstatus == WL_DO_TCP)
                {
                status = wl_get_label_tcp (uwi[i], sql_tcp, 1, &label_tcp);
                }
            else
                {
                status = wl_get_label (uwi[i], sql, 0, label);
                }
	    if (wells->ov_type != 0 AND pstatus != WL_DO_TCP AND deviated[i])
	    {
	    /* go get new label */
	    label_status = wl_get_md( uwi[i], wells->bottom_horizon, wells->source,
			&md, &elevation, FALSE);
	    wes_get_dir_tvd( uwi[i], md, &dx, &dy, &tvd);
	    if (wells->ov_type == 1 AND label_status == SUCCESS)
	    {
	    sprintf(label, "%.1lf", elevation-tvd);
	    }
	    else
	    if (wells->ov_type == 2 AND label_status == SUCCESS)
	    {
	    base_tvd = tvd;
	    label_status = wl_get_md( uwi[i], wells->top_horizon, wells->source,
			&md, &elevation, FALSE);
	    wes_get_dir_tvd( uwi[i], md, &dx, &dy, &tvd);
	    if (label_status == SUCCESS)
	    {
	    sprintf(label, "%.1lf", (elevation-tvd)-(elevation-base_tvd));
	    }
	    }
	    } 

            if (status < 0)
                {
                goto finished;
                }
/* (CP) don't post if label is outside map (clipped deviated wells).
   this code is placed here so that ov_write_scatter will still pick up
   value (well may be on map but current location of label may start
   off map - this could change, ie. user changes RIGHT to ABOVE
   thereby moving label onto map) Also want to include it for contouring
   even if label is off map */

            else if ((status EQUALS SUCCESS) AND (label_x >= ll_x) AND
		(label_x <= ur_x) AND (label_y >= ll_y) AND (label_y <= ur_y))
                {
		  if (IS_EMPTY_STRING(label) == FALSE && 
		      strlen(label) > 2 &&
		      label[strlen(label) - 1] == '0' &&
		      label[strlen(label) - 2] == '.')
		    {
		      label[strlen(label) - 2] = 0;
		    }
                /* POST LABEL                                                       */
                if (NOT suppress_text)
                    {
                    (pstatus != WL_DO_TCP) ? wm_gtx (label_x, label_y,
                                                     label) :
                        wl_post_tcp (label_x, label_y, wells->text_size, j, label_tcp);
                    }
                /* WRITE TO XYZ FILE                                                */
                if (wells->zfield EQUALS j)
                    {
                    z = atof (label);
                    ov_write_scatter (wells->scatter, &(base_x[i]), &(base_y[i]), &z,
                                      1);
                    }
                }
            }
        if (NOT suppress_text)
            {
            wmsclose (segment_id);
            segment_id = 0;
            }
        if (pstatus == WL_DO_TCP)
            {
            ts_tcp_free (sql_tcp);
            }
        wl_get_label (uwi[0], sql, 2, label);
	wl_get_md( uwi[0], wells->top_horizon, wells->source,
		&md, &elevation, TRUE);
        tc_free (sql);
        sql = NULL;
        }
finished:
    
    if (sql != NULL)
        tc_free (sql);

#if NEED_TO_FREE EQUALS TRUE
    tc_free (label);
    tc_free (uwi);
    tc_free (deviated);
    tc_free (base_x);
    tc_free (base_y);
    tc_free (offset_x);
    tc_free (offset_y);
    tc_free (up_x);
    tc_free (up_y);
#endif
    nl_free_nlist (label_nlist);
    
    if (status > 0)
        {
        status = SUCCESS;
        }
    if (segment_id)
        {
        wmsclose (segment_id);
        }
    return status;
    }
/* END:                                                                             */
