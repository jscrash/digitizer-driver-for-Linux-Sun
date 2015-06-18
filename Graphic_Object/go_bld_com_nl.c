/* DEC/CMS REPLACEMENT HISTORY, Element GO_BLD_COM_NL.C*/
/* *7    24-SEP-1990 16:22:25 GILLESPIE "Beta Deltas"*/
/* *6    19-JUL-1990 19:00:29 GILLESPIE "(SPR 5411) Add status checks"*/
/* *5     6-JUL-1990 18:42:40 VINCE "(SPR 5557) fix graphic object picking problem"*/
/* *4    17-JAN-1990 15:31:21 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    24-OCT-1989 09:38:05 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *2    27-JUL-1989 12:49:13 GILLESPIE "(SPR 1) fix minor casting problems"*/
/* *1    19-JUN-1989 12:53:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GO_BLD_COM_NL.C*/
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

/*****************************************************************************/
/*                                                                           */
/* PROGRAM:  GO_BUILD_COMMON_NLIST                                           */
/* AUTHOR:   LUCIA LONGHI                                                    */
/* FUNCTION: CREATE NLIST WITH ALL SELECTABLE GRAPHIC OBJECTS                */
/*                                                                           */
/*****************************************************************************/

#include "esi_go.h"

#ifndef ESI_NL
#include "esi_nl.h"
#endif

#ifndef ESI_MD_DBDEFS_H
#include "esi_md_dbdefs.h"
#endif

#ifndef ESI_CT
#include "esi_ct.h"
#endif

#ifndef ESI_DR
#include "esi_dr.h"
#endif

#ifndef ESI_MM
#include "esi_mm.h"
#endif

#ifndef ESI_MP
#include "esi_mp.h"
#endif

#ifndef ESI_WM
#include "esi_wm.h"
#endif

#ifndef ESI_GKS
#include "esi_gks.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#if USE_PROTOTYPES
publicdef INT go_build_common_nlist(NLIST_HEADER *nlist)
#else
publicdef INT go_build_common_nlist(nlist)
NLIST_HEADER *nlist;
#endif
    {
    INT status;
    UINT ndim = 2;
    UINT Col_List[2];
    INT units[2];
    size_t data_sizes[2];
    INT data_types[2];
    INT value_inc[2];
    VOIDPTR Var_List[2];
    static CHAR *c_name[3] = 
        {
        "X", "Y", NULL
        };
    static CHAR *tc_name[3] = 
        {
        "X", "Y", NULL
        };
    INT i, j, k, l;
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT row;
    MD_COMPILED_VERB *verbstruc;
    INT nobj, nlines, ntext, item_id;
    UINT table_used, total_tables_in_line;
    BOOL inited = FALSE;
    INT verb_ndx;
    PICKABLE_GRA_OBJ **pick_gra_obj;
    PICKABLE_OBJECT pick_obj;
    CHAR *object_type;
    CHAR *horizon_name;
    BOOL isfault;
    LINE line;
    FILENAME nlist_file;
    TEXT text;
    NLIST_HEADER text_nlist;
    GO_USER_STRUCT user_struct;
    FLOAT cx, cy;
    
    /* assume SUCCESS                                                               */
    
    status = SUCCESS;
    object_type = (CHAR *) NULL;
    text_nlist = (NLIST_HEADER)0;
    verb_ndx = -1;
    mp_get_current_map(&map);
    display = map->display;
    compiled = map->compiled;
    units[0] = units[1] = 0;
    data_sizes[0] = data_sizes[1] = sizeof(FLOAT);
    Col_List[0] = 1, Col_List[1] = 2;
    data_types[0] = data_types[1] = FLOAT_TYPE;
    value_inc[0] = value_inc[1] = VALUE_LIST;
    
    for (i = 0; i < display->last_row_used; i++)
        {
        if (verb_ndx == display->verb_ndx[i])
            continue;

        verb_ndx = display->verb_ndx[i];
        if (verb_ndx == -1)
            continue;

        if (!display->overlay_displayed[i])
            continue;

        if (compiled->verb_code[display->compiled_row[i]] != MP_GRA_OBJS)
            continue;

        row = display->compiled_row[i];
        pick_gra_obj = (PICKABLE_GRA_OBJ **)compiled->picks[row];
        
        /* find object type                                                         */
        verbstruc = compiled->verb_ptr[row];
        for (j = 0; j < verbstruc->nrows; j++)
            if ((verbstruc->key_used[j] != 0) AND (verbstruc->key_code[j] == 20))
                {
                object_type = verbstruc->parm_ptr[j]->parmval.cval;
                break;
                }

        /* if object type was FAULTS then get the horizon name */
	if (object_type != (CHAR *) NULL AND 
		strcmp(object_type, FAULTS_TYPE) == 0)
	    {
            for (j=0; j<verbstruc->nrows; j++)
		{
                if ((verbstruc->key_used[j] != 0) AND (verbstruc->key_code[j] == 6))
                    {
                    horizon_name = verbstruc->parm_ptr[j]->parmval.cval;
                    break;
                    }
		}
	    }

        nobj = pick_gra_obj[0]->nused;
        
        /* read the graphic_objects into the common static nlist                    */
        for (j = 0; j < nobj; j++)
            {
            pick_obj = pick_gra_obj[0]->pick_obj[j];
            nlines   = pick_obj.line_nused;
            ntext    = pick_obj.text_nused;
            
            /* if nlists not cloned, do so                                          */
            if ((nlines + ntext) > 0)
                {
                if (NOT inited)
                    {
                    status = nl_init_nlist(nlist, ndim, units, data_sizes, data_types,
                                           value_inc, 20);
                    if (status != SUCCESS)
                        {
                        return status;
                        }

                    inited = TRUE;
                    status = nl_set_nlist_info(*nlist, NL_NLIST_COLUMN_NAME, ndim,
                                               Col_List, (VOIDPTR)c_name);
                    if (status != SUCCESS)
                        {
                        return status;
                        }
                    }

                if (ntext > 0)
                    {
                    status = nl_init_nlist(&text_nlist, ndim, units, data_sizes,
                                           data_types, value_inc, 20);
                    if (status != SUCCESS)
                        {
                        return status;
                        }
                    
                    status = nl_set_nlist_info(text_nlist, NL_NLIST_COLUMN_NAME, ndim,
                                               Col_List, (VOIDPTR)tc_name);
                    if (status != SUCCESS)
                        {
                        return status;
                        }
                    }
                }
            else
                {
                continue;
                }

            /* copy object data from each table to a table in the common nlist      */
            for (k = 0; k < nlines; k++)
                {
                line.line_nlist = (NLIST_HEADER)0;
                item_id = pick_obj.line_id[k];

                isfault = (strcmp(object_type, FAULTS_TYPE) == 0);

                if (! isfault)
                    {
                    ho_add_path("ESI$CULTURE", object_type, sizeof nlist_file,
                                nlist_file);
                    }

                status = dr_get_line(&line, item_id, nlist_file, isfault,
                                     horizon_name);
                if (status != SUCCESS)
                    {
                    return status;
                    }

                status = nl_inq_nlist_int(line.line_nlist, NL_NLIST_NTABLES,
                                          (INT *) & total_tables_in_line);
                if (status != SUCCESS)
                    {
                    return status;
                    }

                user_struct.segment_id = pick_obj.line_segm[k];
                user_struct.elem_id    = item_id;
                user_struct.if_text    = FALSE;
                user_struct.obj_id     = pick_gra_obj[0]->object_id[j];
                user_struct.group_id   = pick_gra_obj[0]->group_id[j];
                
                for (l = 1; l <= total_tables_in_line; l++)
                    {
                    status = go_copy_stroke(line.line_nlist, l, *nlist, &table_used);
                    if (status != SUCCESS)
                        {
                        return status;
                        }

                    status = nl_set_table_user_struct(*nlist, table_used, &user_struct,
                                                      sizeof(GO_USER_STRUCT));
                    if (status != SUCCESS)
                        {
                        return status;
                        }
                    }
                }

            for (k = 0; k < ntext; k++)
                {
                item_id = pick_obj.text_id[k];
                status = ct_get_text(&text, item_id);
                if (status != SUCCESS)
                    {
                    return status;
                    }
                
                /* build nlist with boundary box of text to add to static nlist for picking */
                wm_gqtxx(MAIN_WKID, text.text_position.x, text.text_position.y,
                         text.text_string, &status, &cx, &cy, text.box_boundary[0],
                         text.box_boundary[1]);

                Var_List[0] = (VOIDPTR)text.box_boundary[0];
                Var_List[1] = (VOIDPTR)text.box_boundary[1];

                status = nl_start_table(text_nlist, &table_used);
                if (status != SUCCESS)
                    {
                    return status;
                    }

                status = nl_add_rows(text_nlist, 4, ndim, Col_List, Var_List);
                if (status != SUCCESS)
                    {
                    return status;
                    }
                
                status = go_copy_stroke(text_nlist, table_used, *nlist, &table_used);
                if (status != SUCCESS)
                    {
                    return status;
                    }

                user_struct.segment_id = pick_obj.text_segm[k];
                user_struct.elem_id    = item_id;
                user_struct.if_text    = TRUE;
                user_struct.obj_id     = pick_gra_obj[0]->object_id[j];
                user_struct.group_id   = pick_gra_obj[0]->group_id[j];
                status = nl_set_table_user_struct(*nlist, table_used, &user_struct,
                                                  sizeof(GO_USER_STRUCT));
                if (status != SUCCESS)
                    {
                    return status;
                    }
                }
            }
        }

    if (NOT inited)
        {
        status = nl_init_nlist(nlist, ndim, units, data_sizes, data_types,
                               value_inc, 20);
        if (status != SUCCESS)
            {
            return status;
            }

        status = nl_set_nlist_info(*nlist, NL_NLIST_COLUMN_NAME, ndim, Col_List,
                                   (VOIDPTR)c_name);
        if (status != SUCCESS)
            {
            return status;
            }

        inited = TRUE;
        }

    if (text_nlist != (NLIST_HEADER)0)
        nl_free_nlist(text_nlist);
    return(status);
    }

/* **********************************************************************

    ROUTINE: GO_COPY_STROKE

    DESCRIPTION:
    To copy a two dimensional stroke from one nlist to another one and
    convert the data type.

    INPUT:
    FROM_NLIST = (NLIST_HEADER )the original nlist.
    FROM_STROKE= ( INT) The copied from stroke of original nlist.
    TO_NLIST   = (NLIST_HEADER )The copied to nlist.

    OUTPUT:
    TO_STROKE  = ( INT *) The new generated stroke.
    ********************************************************************** */
#if USE_PROTOTYPES
publicdef INT go_copy_stroke(NLIST_HEADER from_nlist, UINT from_stroke,
                             NLIST_HEADER to_nlist, UINT *to_stroke)
#else
publicdef INT go_copy_stroke(from_nlist, from_stroke, to_nlist, to_stroke)
NLIST_HEADER from_nlist, to_nlist;
UINT from_stroke, *to_stroke;
#endif
    {
    UINT Col_List[2];
    INT units[2];
    size_t data_size[2];
    INT data_type[2];
    UINT total_rows;
    INT status;
    VOIDPTR Var_List[2];
    
    /* inquire the total number of rows in FROM_STROKE.                             */
    status = nl_set_current_row(from_nlist, 1, from_stroke);
    status = nl_inq_table_int(from_nlist, from_stroke, NL_TABLE_NROWS,
                              (INT *) & total_rows);
    if (status < SUCCESS)
        {
        return status;
        }
    /* create a new stroke in the TO_NLIST.                                         */
    status = nl_start_table(to_nlist, to_stroke);
    if (status != SUCCESS)
        {
        return status;
        }
    
    if (total_rows EQUALS 0)
        {
        return SUCCESS;
        }
    /* To inquire all necessary information.                                        */
    Col_List[0] = 1, Col_List[1] = 2;
    Var_List[0] = (VOIDPTR) & units[0];
    Var_List[1] = (VOIDPTR) & units[1];
    status = nl_inq_nlist_info(from_nlist, NL_NLIST_UNITS, 2, Col_List, Var_List);

    Var_List[0] = (VOIDPTR) & data_size[0];
    Var_List[1] = (VOIDPTR) & data_size[1];
    status = nl_inq_nlist_info(from_nlist, NL_NLIST_DATA_WIDTHS, 2, Col_List,
                               Var_List);

    Var_List[0] = (VOIDPTR) & data_type[0];
    Var_List[1] = (VOIDPTR) & data_type[1];
    status = nl_inq_nlist_info(from_nlist, NL_NLIST_DATA_TYPES, 2, Col_List, Var_List);
    
    /* To Allocate the space for retrieving the data.                               */
    Var_List[0] = (VOIDPTR)TALLOC(total_rows * data_size[0]);
    if (Var_List[0] EQUALS (VOIDPTR)0)
        {
        return FAIL;
        }

    Var_List[1] = (VOIDPTR)TALLOC(total_rows * data_size[1]);
    if (Var_List[0] EQUALS (VOIDPTR)0)
        {
        return FAIL;
        }
    
    status = nl_get_rows(from_nlist, total_rows, 2, Col_List, Var_List, &total_rows);
    if (status < SUCCESS)
        {
        return status;
        }

    status = nl_add_rows_convert(to_nlist, total_rows, 2, Col_List, Var_List, units,
                                 data_size, data_type);
    status = (status >= 0) ? SUCCESS : status;

#if NEED_TO_FREE == TRUE
    tc_free(Var_List[0]);
    tc_free(Var_List[1]);
#endif
    return status;
    }
