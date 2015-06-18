/* DEC/CMS REPLACEMENT HISTORY, Element TO_DRAW_LN_METH.C */
/* *11    6-NOV-1990 15:30:51 JULIAN "(SPR 6156) township & section labels mods" */
/* *10   14-AUG-1990 16:36:09 GILLESPIE "(SPR 2001) Use LINE_GC" */
/* *9    25-JUL-1990 09:35:11 GILLESPIE "(SPR 1) Fix calling args to wm_gs..." */
/* *8    18-JUL-1990 17:57:26 JESSIE "(SPR 1) change include files" */
/* *7    26-MAR-1990 23:24:32 JULIAN "(SPR 5095) text bundle mods" */
/* *6    25-MAR-1990 22:52:38 SCD "(SPR 1) First round of cleanup -- much more to do" */
/* *5    23-MAR-1990 16:34:39 JULIAN "(SPR 0) remove debugging analyzer" */
/* *4    23-MAR-1990 15:31:41 JULIAN "(SPR 5095) change method in text clip" */
/* *3    21-MAR-1990 16:26:58 JULIAN "(SPR 0) more changes" */
/* *2    16-MAR-1990 09:12:39 JULIAN "(SPR 5095) culture" */
/* *1    16-MAR-1990 08:52:46 JULIAN "culture overlay" */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_DRAW_LN_METH.C */

#include "esi_ly.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_TO_H
#include "esi_to.h"                      /* graphics overlay */
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"                      /* window manager */
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"                      /* mapping */
#endif

#include "esi_nl_meth_hdr.h"

/*************************************************************

    FUNCTION : TO_DRAW_LINE_METHOD
    DESCRIPTION : given an nlist of nodes (x and y coordinates) 
    and line properties for specified nlist, post in map

    AUTHOR : Purna Tjahjana (Feb 8,1990)

*****************************************************************/

#if USE_PROTOTYPES
publicdef INT to_draw_line_method(NLIST_HEADER nlist, VOIDPTR param)
#else
publicdef INT to_draw_line_method(nlist, param)
NLIST_HEADER nlist;
VOIDPTR param;
#endif
    {
    LINE_GC *l_gc = (LINE_GC *)param;
    MAP_STRUCTURE *mapdef;          /* current map structure                        */
    NLIST_HEADER nlist_out;
    DOUBLE min_point[2], max_point[2];
    UINT Col_List[2];
    VOIDPTR Var_List[2];
    INT status;
    NLM_CLIP_HEADER clip;
    FILENAME nlist_file;
    size_t f_size;
    
    /* * GET THE MAP *                                                              */
    status = mp_get_current_map(&mapdef);
    if (status != SUCCESS)
        {
        return status;
        }
    Col_List[0] = 1;
    Col_List[1] = 2;
    
    Var_List[0] = (VOIDPTR) & min_point[0];
    Var_List[1] = (VOIDPTR) & min_point[1];
    nl_inq_nlist_info(nlist, NL_NLIST_MIN, 2, Col_List, Var_List);
    Var_List[0] = (VOIDPTR) & max_point[0];
    Var_List[1] = (VOIDPTR) & max_point[1];
    nl_inq_nlist_info(nlist, NL_NLIST_MAX, 2, Col_List, Var_List);
    /* IS CLIPPED WINDOW NECESSARY ?                                                */
    
    if ((min_point[0] < mapdef->lower_left_xy.x) ||
        (min_point[1] < mapdef->lower_left_xy.y) ||
        (max_point[0] > mapdef->upper_right_xy.x) ||
        (max_point[1] > mapdef->upper_right_xy.y))
        {
        clip.Col_List[0] = 1;
        clip.Col_List[1] = 2;
        clip.minList[0] = mapdef->lower_left_xy.x;
        clip.minList[1] = mapdef->lower_left_xy.y;
        clip.maxList[0] = mapdef->upper_right_xy.x;
        clip.maxList[1] = mapdef->upper_right_xy.y;
        status = nl_invoke_nlist_method(nlist, NL_CLIP_METHOD, &clip);
        if (status EQUALS SUCCESS)
            {
            nl_free_nlist(nlist);
            nlist = clip.outNlist;
            }
	else
	  {
	    nl_free_nlist(nlist);
	    return SUCCESS;
	  }
        }
    /* DRAW NLIST                                                                   */
    
    wm_gsln(l_gc->line.style);
    wm_gslwsc(l_gc->line.width);
    wm_gsplci(l_gc->line.color);
    
    status = nl_draw_nlist(nlist, 1, 2);
    return SUCCESS;
    }
