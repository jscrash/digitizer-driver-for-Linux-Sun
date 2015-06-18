/* DEC/CMS REPLACEMENT HISTORY, Element EE_TOOLS_B.C*/
/* *8    14-AUG-1990 13:11:56 VINCE "(SPR 5644) Header Standardization"*/
/* *7    24-MAR-1990 21:44:29 GILLESPIE "(SPR 1) Fix c_name stuff"*/
/* *6    23-MAR-1990 18:47:14 GILLESPIE "(SPR 1) Add nl_inq_nlist_ingo call to left && right test bank"*/
/* *5    26-JAN-1990 14:27:10 WALTERS "(SPR 0) Fix bug in nl_set_nlist_info"*/
/* *4    17-JAN-1990 15:31:08 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    18-SEP-1989 12:42:57 JULIAN "Gulf mods under SPR 100"*/
/* *2    27-JUL-1989 13:50:19 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 12:49:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element EE_TOOLS_B.C*/
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
#include "esi_ee.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_DG_DEFS_H

#include "esi_dg_defs.h"

#endif

#ifndef ESI_EE_ERR_H

#include "esi_ee_err.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ee_perform_operation (CHAR *result_nl, CHAR *token, CHAR *left_nl, 
        CHAR *right_nl, FILENAME temp_nlists);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    result_nl       -(CHAR *) The result nlist name - may be tempxx.
    token           -(CHAR *) The single character token.
    left_nl         -(CHAR *) The left operand nlist name - vms file name.
    right_nl        -(CHAR *) The right operand nlist name.
    temp_nlists     -(FILENAME) Temporary nlist file:
                        put all result nlists in here except the final one,
                        look here first for operand nlists. 

Return Value/Status:
    SUCCESS - Successful completion.
    EE_NLIST_NOT_FOUND
    EE_NO_ACCESS
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ee_perform_operation (CHAR *result_nl, CHAR *token, CHAR *left_nl,
                                    CHAR *right_nl, FILENAME temp_nlists)
#else
publicdef INT ee_perform_operation (result_nl, token, left_nl, right_nl, temp_nlists)
CHAR *result_nl;
CHAR *token;
CHAR *left_nl;
CHAR *right_nl;
FILENAME temp_nlists;
#endif
    {
    FILENAME left_file_name;        /* the fully expanded name */
    FILENAME right_file_name;
    FILENAME result_file_name;
    NLIST_HEADER left_op_nl;        /* the internal nlists used in calculations */
    NLIST_HEADER right_op_nl;
    NLIST_HEADER result_op_nl;
    FLOAT nl_null;
    UINT left_rows, right_rows;     /* vars to verify nlists are equal grids */
    UINT left_tables, right_tables;
    UINT junk;
    INT loop, status, index, index2;
    UINT ndim = 1;
    INT units[1];
    size_t data_sizes[1];
    INT data_types[1];
    INT value_inc[1];
    VOIDPTR val_list1_addr[1], val_list2_addr[1], val_list3_addr[1];
    UINT dim_list[1];
    FLOAT *value_list1, *value_list2, *value_list3;
    BOOL left_is_nlist, right_is_nlist;
    FLOAT left_val, right_val;      /* converted value used if operand is float */
    FLOAT temp_val;
    GRID_NLIST_USER_STRUCT *user_struct_ptr;    /* for min,max and projection */
    CHAR description[241];
    static CHAR directory[] = "ESI$GRIDS";
    CHAR col_name[32];
    CHAR *c_name[1];
    
    description[0] = '\0';
    units[0] = 0;
    data_sizes[0] = sizeof(FLOAT);
    data_types[0] = FLOAT_TYPE;
    value_inc[0] = VALUE_LIST;
    dim_list[0] = 1;
    c_name[0] = col_name;
    
    /* if token is an equal sign this is a copy nlist */
    if (ARE_SAME (token, "="))
        {
        /* Since we are now outputting the result nlist, it goes through FM */
        status = fm_init_file (directory, result_nl, "GRID", "w", TRUE, description,
                               fm_grid_term_func, result_file_name, "GRID_OPS");
        if (status != SUCCESS)
            return EE_NO_ACCESS;
        
        /* assume right operand is an nlist - the result of previous operations */
        /* First, try to read it from the temporary nlist file. */
        status = nl_read_nlist (&right_op_nl, temp_nlists, right_nl);
        if (status != SUCCESS)
            {
            /* OK, it's not a temporary grid op nlist, so get it from file manager */
            status = fm_init_file (directory, right_nl, "GRID", "r", TRUE, description,
                                   NULL_FUNCTION_PTR, right_file_name, "");
            status = nl_read_nlist (&right_op_nl, right_file_name, "GRIDS");
            fm_release_file (directory, right_nl, "GRID");
            if (status != SUCCESS)
                return EE_NLIST_NOT_FOUND;
            }
        status = nl_inq_nlist_int (right_op_nl, NL_NLIST_NSTROKES,
                                   (INT *) & right_tables);
        val_list1_addr[0] = (VOIDPTR) & nl_null;
        status = nl_inq_nlist_info (right_op_nl, NL_NLIST_NULL_VALUE, ndim, dim_list,
                                    val_list1_addr);
        
        /* init the output nl */
        status = nl_init_nlist (&result_op_nl, ndim, units, data_sizes, data_types,
                                value_inc, 0);
        status = nl_set_nlist_info (result_op_nl, NL_NLIST_NULL_VALUE, ndim, dim_list,
                                    val_list1_addr);
        status = nl_inq_nlist_info (right_op_nl, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                    (VOIDPTR)c_name);
        if (status != SUCCESS)
            {
            return status;
            }
        status = nl_set_nlist_info (result_op_nl, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                    (VOIDPTR)c_name);
        if (status != SUCCESS)
            {
            return status;
            }
        /* copy all tables (rows) */
        for (index = 1; index <= right_tables; index++)
            {
            /* start new table */
            status = nl_set_current_point (right_op_nl, 1, index);
            /* get number of rows in the table */
            status = nl_inq_table_int (right_op_nl, index, NL_STROKE_NPOINTS,
                                       (INT *) & right_rows);
            value_list1 = (FLOAT *)tc_alloc (sizeof(FLOAT) * right_rows);
            val_list1_addr[0] = (VOIDPTR)value_list1;
            status = nl_get_rows (right_op_nl, right_rows, ndim, dim_list,
                                  val_list1_addr, &junk);
            status = nl_start_table (result_op_nl, &junk);
            status = nl_add_rows (result_op_nl, right_rows, ndim, dim_list,
                                  val_list1_addr);
            status = tc_free ((BYTE *)value_list1);
            }
        /* copy user struct ( projection, min/max ) */
        user_struct_ptr = (GRID_NLIST_USER_STRUCT *
                           )tc_alloc (sizeof(GRID_NLIST_USER_STRUCT));
        status = nl_inq_nlist_user_struct (right_op_nl, sizeof(GRID_NLIST_USER_STRUCT),
                                           user_struct_ptr, (size_t *) & junk);
        status = nl_set_nlist_user_struct (result_op_nl, user_struct_ptr,
                                           sizeof(GRID_NLIST_USER_STRUCT));
        
        status = nl_delete_nlist (result_file_name, "GRIDS");
        status = nl_write_nlist (result_op_nl, result_file_name, "GRIDS");
        status = tc_free ((BYTE *)user_struct_ptr);
        
        fm_release_file (directory, result_nl, "GRID");
        
        return SUCCESS;
        }
    /* if the left side is an nlist name then do this */
    left_is_nlist = FALSE;
    if (ts_isfloat (left_nl) <= 0)  /* assume this is an nlist */
        {
        left_is_nlist = TRUE;
        
        status = nl_read_nlist (&left_op_nl, temp_nlists, left_nl);
        if (status != SUCCESS)
            {
            /* OK, it's not a temporary grid op nlist, so get it from file manager */
            status = fm_init_file (directory, left_nl, "GRID", "r", TRUE, description,
                                   NULL_FUNCTION_PTR, left_file_name, "");
            status = nl_read_nlist (&left_op_nl, left_file_name, "GRIDS");
            fm_release_file (directory, left_nl, "GRID");
            if (status != SUCCESS)
                return EE_NLIST_NOT_FOUND;
            }
        /*  get total number of rows */
        status = nl_inq_nlist_int (left_op_nl, NL_NLIST_NPOINTS, (INT *) & left_rows);
        val_list1_addr[0] = (VOIDPTR) & nl_null;
        status = nl_inq_nlist_info (left_op_nl, NL_NLIST_NULL_VALUE, ndim, dim_list,
                                    val_list1_addr);
        }
    /* if right side is an nlist then do this */
    right_is_nlist = FALSE;
    if (ts_isfloat (right_nl) <= 0)
        {
        right_is_nlist = TRUE;
        
        /* Now do the same for the right operand -- First, try to read it from
           the temporary nlist file. */
        
        status = nl_read_nlist (&right_op_nl, temp_nlists, right_nl);
        if (status != SUCCESS)
            {
            /* OK, it's not a temporary grid op nlist, so get it from file manager */
            status = fm_init_file (directory, right_nl, "GRID", "r", TRUE, description,
                                   NULL_FUNCTION_PTR, right_file_name, "");
            status = nl_read_nlist (&right_op_nl, right_file_name, "GRIDS");
            fm_release_file (directory, right_nl, "GRID");
            if (status != SUCCESS)
                return EE_NLIST_NOT_FOUND;
            }
        status = nl_inq_nlist_int (right_op_nl, NL_NLIST_NPOINTS,
                                   (INT *) & right_rows);
        val_list1_addr[0] = (VOIDPTR) & nl_null;
        status = nl_inq_nlist_info (right_op_nl, NL_NLIST_NULL_VALUE, ndim, dim_list,
                                    val_list1_addr);
        }
    /* if both operands are nlists then make sure they are ok */
    if (left_is_nlist && right_is_nlist)
        {
        /* rows and tables must match for ok */
        if (right_rows != left_rows)
            return EE_NLIST_MISMATCH;
        status = nl_inq_nlist_int (left_op_nl, NL_NLIST_NSTROKES,
                                   (INT *) & left_tables);
        status = nl_inq_nlist_int (right_op_nl, NL_NLIST_NSTROKES,
                                   (INT *) & right_tables);
        if (right_tables != left_tables)
            return EE_NLIST_MISMATCH;
        status = nl_inq_nlist_info (left_op_nl, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                    (VOIDPTR)c_name);
        if (status != SUCCESS)
            {
            return status;
            }
        }
    else if (left_is_nlist)
        {
        right_rows = left_rows;
        status = nl_inq_nlist_int (left_op_nl, NL_NLIST_NSTROKES,
                                   (INT *) & left_tables);
        status = nl_inq_nlist_info (left_op_nl, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                    (VOIDPTR)c_name);
        if (status != SUCCESS)
            {
            return status;
            }
        }
    else if (right_is_nlist)
        {
        left_rows = right_rows;
        status = nl_inq_nlist_int (right_op_nl, NL_NLIST_NSTROKES,
                                   (INT *) & right_tables);
        status = nl_inq_nlist_info (right_op_nl, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                    (VOIDPTR)c_name);
        if (status != SUCCESS)
            {
            return status;
            }
        }
    /* create new temp_nl nlist. results to be put in it */
    status = nl_init_nlist (&result_op_nl, ndim, units, data_sizes, data_types,
                            value_inc, 0);
    status = nl_set_nlist_info (result_op_nl, NL_NLIST_NULL_VALUE, ndim, dim_list,
                                val_list1_addr);
    status = nl_set_nlist_info (result_op_nl, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    /* if left is an nlist set number of tables for left */
    /* if left is not an nlist and right is an nlist set number of tables */
    /* if one is not an nlist then make it a flat array */
    
    if (left_is_nlist)
        right_tables = left_tables;
    else if (right_is_nlist)
        left_tables = right_tables;
    
    /* note memory for each row is allocated (each table). this is slower than
       just allocating the memory once. however, should uneven grids be allowed
       this technique is necessary */
    
    for (index = 1; index <= left_tables; index++)  /* for each table */
        {
        if (left_is_nlist)
            {
            /* get number of rows per table and allocate them */
            status = nl_set_current_point (left_op_nl, 1, index);
            status = nl_inq_table_int (left_op_nl, index, NL_STROKE_NPOINTS,
                                       (INT *) & left_rows);
            value_list1 = (FLOAT *)tc_alloc (sizeof(FLOAT) * left_rows);
            val_list1_addr[0] = (VOIDPTR)value_list1;
            status = nl_get_rows (left_op_nl, left_rows, ndim, dim_list,
                                  val_list1_addr, &junk);
            }
        if (right_is_nlist)
            {
            status = nl_set_current_point (right_op_nl, 1, index);
            status = nl_inq_table_int (right_op_nl, index, NL_STROKE_NPOINTS,
                                       (INT *) & right_rows);
            value_list2 = (FLOAT *)tc_alloc (sizeof(FLOAT) * right_rows);
            val_list2_addr[0] = (VOIDPTR)value_list2;
            status = nl_get_rows (right_op_nl, right_rows, ndim, dim_list,
                                  val_list2_addr, &junk);
            }
        if (!left_is_nlist)
            /* populate value_list1 as a constant using right_rows as argument */
            {
            left_rows = right_rows;
            value_list1 = (FLOAT *)tc_alloc (sizeof(FLOAT) * left_rows);
            sscanf (left_nl, "%f", &left_val);
            for (index2 = 0; index2 < left_rows; index2++)
                value_list1[index2] = left_val;
            }
        if (!right_is_nlist)
            /* populate value_list2 as a constant using left_rows as argument */
            {
            right_rows = left_rows;
            value_list2 = (FLOAT *)tc_alloc (sizeof(FLOAT) * right_rows);
            sscanf (right_nl, "%f", &right_val);
            for (index2 = 0; index2 < right_rows; index2++)
                value_list2[index2] = right_val;
            }
        /* alloc for the  result */
        value_list3 = (FLOAT *)tc_alloc (sizeof(FLOAT) * left_rows);
        val_list3_addr[0] = (VOIDPTR)value_list3;
        
        /* do the operation taking into account the nulls */
        /* take into account the operand may be a constant */
        /* return the data in the result nlist */
        /* for each case do the operation on the value_list and put it into the */
        /* temp nl for the result */
        
        switch (token[0])
            {
            
        case '+':
            for (index2 = 0; index2 < left_rows; index2++)
                {
                if (value_list1[index2] == nl_null OR value_list2[index2] == nl_null)
                    value_list3[index2] = nl_null;
                else
                    value_list3[index2] = value_list1[index2] + value_list2[index2];
                }
            break;
            
        case '-':
            for (index2 = 0; index2 < left_rows; index2++)
                {
                if (value_list1[index2] == nl_null OR value_list2[index2] == nl_null)
                    value_list3[index2] = nl_null;
                else
                    value_list3[index2] = value_list1[index2] - value_list2[index2];
                }
            break;
            
        case '*':
            for (index2 = 0; index2 < left_rows; index2++)
                {
                if (value_list1[index2] == nl_null OR value_list2[index2] == nl_null)
                    value_list3[index2] = nl_null;
                else
                    value_list3[index2] = value_list1[index2] * value_list2[index2];
                }
            break;
            
        case '/':
            for (index2 = 0; index2 < left_rows; index2++)
                {
                if (value_list1[index2] == nl_null OR value_list2[index2] == nl_null)
                    value_list3[index2] = nl_null;
                else
                    value_list3[index2] = value_list1[index2] / value_list2[index2];
                }
            break;
            
        case '^':
            for (index2 = 0; index2 < left_rows; index2++)
                {
                if (value_list1[index2] == nl_null)
                    value_list3[index2] = nl_null;
                else
                    {
                    if (right_val == 0)
                        value_list3[index2] = 1.0;
                    else
                        {
                        temp_val = value_list1[index2];
                        for (loop = (INT)(right_val - 1.0); loop > 0; --loop)
                            temp_val = temp_val * value_list1[index2];
                        value_list3[index2] = temp_val;
                        }
                    }
                }
            break;
            
        default:
            printf ("invalid operator in evaluator\n");
            
            }
        /* put value_list3 into result nl grid */
        /* nl add rows for this table */
        status = nl_start_table (result_op_nl, &junk);
        status = nl_add_rows (result_op_nl, left_rows, ndim, dim_list, val_list3_addr);
        status = tc_free ((BYTE *)value_list1);
        status = tc_free ((BYTE *)value_list2);
        status = tc_free ((BYTE *)value_list3);
        
        }
    /* end for each table */
    user_struct_ptr = (GRID_NLIST_USER_STRUCT *
                       )tc_alloc (sizeof(GRID_NLIST_USER_STRUCT));
    if (right_is_nlist)
        status = nl_inq_nlist_user_struct (right_op_nl, sizeof(GRID_NLIST_USER_STRUCT),
                                           user_struct_ptr, (size_t *) & junk);
    else
        status = nl_inq_nlist_user_struct (left_op_nl, sizeof(GRID_NLIST_USER_STRUCT),
                                           user_struct_ptr, (size_t *) & junk);
    status = nl_set_nlist_user_struct (result_op_nl, user_struct_ptr,
                                       sizeof(GRID_NLIST_USER_STRUCT));
    
    status = nl_write_nlist (result_op_nl, temp_nlists, result_nl);
    fm_release_file (directory, result_op_nl, "GRID");
    
    return SUCCESS;
    
    }
/* END:     */
