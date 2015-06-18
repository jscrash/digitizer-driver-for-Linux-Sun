/* DEC/CMS REPLACEMENT HISTORY, Element WL_DO_NODEV.C*/
/* *7    30-NOV-1990 15:48:04 JULIAN "(SPR 6234) Add clipping after transform well nlist"*/
/* *6    17-OCT-1990 16:46:10 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  4B1  17-OCT-1990 16:15:08 GILLESPIE "Merge Ernie deltas"*/
/* *5    14-AUG-1990 13:40:42 VINCE "(SPR 5644) Header Standardization"*/
/* *4    17-JAN-1990 23:02:52 GILLESPIE "(SPR 6001) Changes for new well struct (remove filename dimension in n-List)"*/
/* *3    18-DEC-1989 11:53:22 CONROY "(SPR 5078) Mods for plotting GKS polymarkers"*/
/* *2    10-OCT-1989 10:04:50 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    19-JUN-1989 13:40:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_DO_NODEV.C*/
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

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_NL_METH_HDR_H
#include "esi_nl_meth_hdr.h"
#endif

#ifndef ESI_NL_METHODS_H
#include "esi_nl_methods.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_CS_H
#include "esi_cs.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_LY_H
#include "esi_ly.h"
#endif

typedef DOUBLE WL_POINT[2];

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function to post well symbols and annotations.                                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wl_do_nodev_wells(WELLS *wells,NLIST_HEADER nodev_nlist,        */
/*        BOOL suppress_text, NLIST_HEADER label_nlist,PICKABLE_WELLS *picks);      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    wells           -(WELLS *) Well overlay structure.                            */
/*    nodev_nlist     -(NLIST_HEADER)                                               */
/*    suppress_text   -(BOOL)                                                       */
/*    label_nlist     -(NLIST_HEADER)                                               */
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
publicdef INT wl_do_nodev_wells (WELLS *wells, NLIST_HEADER nodev_nlist,
                                 BOOL suppress_text, NLIST_HEADER label_nlist,
                                 PICKABLE_WELLS *picks)
#else
publicdef INT wl_do_nodev_wells (wells, nodev_nlist, suppress_text, label_nlist, picks)
WELLS *wells;
NLIST_HEADER nodev_nlist;
BOOL suppress_text;
NLIST_HEADER label_nlist;
PICKABLE_WELLS *picks;
#endif
    {
    
    INT status;                     /* RETURN STATUS                                */
    BOOL batch;
    UWI *uwi;
    INT *well_symbol;
    FLOAT *x;
    FLOAT *y;
    
    MAP_STRUCTURE *map;
    INT i, j;
    SYMBOL symbol;
    UINT num_wells;
    PROJECTION_NAME projection;
    FLOAT dummy;
    NLIST_HEADER temp_nlist;
    NLM_CLIP_HEADER clip;
    
    WL_POINT xy_in;
    WL_POINT xy_out;
    
    PROJECTION_STRUCTURE *project_data;
    PROJECTION_STRUCTURE *map_data;

    /* NLIST VARIABLES                                                              */
    static UINT d_list[4] = 
        {
        1, 2, 3, 4
        };
    VOIDPTR v_list[4];

    static UINT ct_col_list[2]= { 3, 4 };
    /* **********************************************************************       */
    
    status = SUCCESS;
    
    /* * TRANSFORM POINTS IF NECESSARY  AND CLIP AFTERWARDS *                       */
    /* ---------------------------------                                            */
    /* NOTE : tranformation has to go first (before clipping),
              then we are sure that all wells are clipped against the
	      map boundaries. Anyway, clipping is always done in
	      map projection. (purna) */

    if (NOT wells->suppress_symbols OR NOT suppress_text OR wells->zfield)
        {
        mp_get_current_map (&map);
        qs_inq_c (QS_PROJECT_PROJECTION, projection, (INT *)0);
        if (strcmp (map->map_projection, projection) != 0)
          {
	    status = ct_get_projection(projection,&project_data);
	    if(status != SUCCESS) return status;
	    status = ct_get_projection(map->map_projection,&map_data);
	    if(status != SUCCESS) return status;
	    status = ct_transform_nlist(nodev_nlist,project_data,&temp_nlist,
				        map_data,ct_col_list,0.0);
	    if(status EQUALS SUCCESS)
	      {
		nl_free_nlist(nodev_nlist);
		nodev_nlist=temp_nlist;
	      }
	  }

    /* temporarily use the text clipping method, since it works
       the way we want to work for this case. We have to change
       the name later. (purna - 11/29/90) */
       status = nl_set_nlist_method(nodev_nlist, NL_CLIP_METHOD,
				 "NLM_WINDOW_CLIP_TEXT",
				 nlm_window_clip_text);

	clip.Col_List[0] = 3;
	clip.Col_List[1] = 4;
	clip.minList[0] = map->lower_left_xy.x;
	clip.minList[1] = map->lower_left_xy.y;
	clip.maxList[0] = map->upper_right_xy.x;
	clip.maxList[1] = map->upper_right_xy.y;

	status = nl_invoke_nlist_method (nodev_nlist, NL_CLIP_METHOD, 
				     &clip);
	if (status == SUCCESS)
	   {
	     nl_free_nlist (nodev_nlist);
	     nodev_nlist = clip.outNlist;
	    }
        else
            {
	      nl_free_nlist(nodev_nlist);
	      return SUCCESS;
	    }
        }
    
    /* * GET DATA *                                                                 */
    /* ------------                                                                 */
    
    nl_inq_nlist_int (nodev_nlist, NL_NLIST_NPOINTS, (INT *)&num_wells);
    if (num_wells EQUALS 0)
        {
        goto finished;
        }
    uwi = (UWI *) tc_alloc (num_wells * sizeof(UWI));
    well_symbol = (INT *) tc_alloc (num_wells * sizeof(INT));
    x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
    y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
    
    v_list[0] = (VOIDPTR)uwi;
    v_list[1] = (VOIDPTR)well_symbol;
    v_list[2] = (VOIDPTR)x;
    v_list[3] = (VOIDPTR)y;
    
    nl_set_current_point (nodev_nlist, 1, 1);
    nl_get_points (nodev_nlist, num_wells, 4, d_list, v_list, (UINT *)&i);
    
    /* * POST SYMBOLS *                                                             */
    /* ----------------                                                             */
    
    if (NOT wells->suppress_symbols)
        {
        
        /* DEFAULT SYMBOL - CENTERED SYMBOLS                                        */
        if (NOT wells->symbol_type)
            {
            strcpy (symbol.symbol_group, CS_V4_WELLS_GROUP);
            symbol.symbol_color = wells->symbol_color;
            symbol.symbol_size = wells->symbol_size;
            
            for (i = 0; i < num_wells; i++)
                {
                symbol.symbol_location.x = x[i];
                symbol.symbol_location.y = y[i];
                symbol.symbol_type = well_symbol[i];
                mm_draw_symbol (&symbol);
                
                /* ---------------------------------------------                    */
                /* add into the will legend list during plotting                    */
                /*----------------------------------------------                    */
                qs_inq_i (QS_IF_BATCH, &batch);
                
                if (batch)
                    {
                    pl_add_well_symbol (symbol.symbol_type, symbol.symbol_color);
                    }
                wlz_add_pick (picks, uwi[i]);
                }
            }
        /* GKS POLYMARKER                                                           */
        else
            {
            wm_gspmci (wells->symbol_color);
            wm_gsmk (wells->symbol_type);
            dummy = wells->symbol_size * map->wcs_to_inches;
            wm_gsmksc (dummy);
            wm_gpm (num_wells, x, y);
            
            for (i = 0; i < num_wells; i++)
                {
                wlz_add_pick (picks, uwi[i]);
                }
            }
        }
    /* * FILL LABEL_NLIST *                                                         */
    /* --------------------                                                         */
    
    tc_zeroes (well_symbol, num_wells * sizeof(INT));
    nl_add_points (label_nlist, num_wells, 4, d_list, v_list);
    
    tc_free ((VOIDPTR)uwi);
    tc_free ((VOIDPTR)well_symbol);
    tc_free ((VOIDPTR)x);
    tc_free ((VOIDPTR)y);
    
finished:
    return status;
    }
/* END:                                                                             */
