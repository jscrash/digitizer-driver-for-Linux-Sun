/* DEC/CMS REPLACEMENT HISTORY, Element NL_PICK_ROWR.C*/
/* *5    25-OCT-1990 12:24:39 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 15:01:54 VINCE "(SPR 5644) Header Standardization"*/
/* *3     9-APR-1990 14:49:06 GILLESPIE "(SPR 1) Use nlz_set_table"*/
/* *2    12-MAR-1990 19:08:40 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:59:55 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PICK_ROWR.C*/
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

#include "esi_nl_int.h"

#ifndef ESI_AG_H

#include "esi_ag.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This function is an optimized version of nl_pick_row_range.
    Given a test row and an nlist, it finds the number of the 
    table that is closest to the test row, and the number of
    the row within that table which is closest to the test
    row.

Prototype:
    publicdef INT nl_pick_row_range(NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        DOUBLE *Value_List[], DOUBLE min[], DOUBLE max[], UINT *table_num, 
        UINT *row_num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    nCol            -(UINT) This version only handles 2 dims...
    Col_List        -(UINT *)
    Value_List      -(DOUBLE *[])
    min             -(DOUBLE [])
    max             -(DOUBLE [])
    table_num       -(UINT *)
    row_num         -(UINT *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_INVALID_COLUMN
    NL_INVALID_ROW
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    Note: only handles two columns (ie x and y coords).

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_pick_row_range (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                                 DOUBLE *Value_List[], DOUBLE min[], DOUBLE max[],
                                 UINT *table_num, UINT *row_num)
#else
publicdef INT nl_pick_row_range (nlist, nCol, Col_List, Value_List, min, max,
                                 table_num, row_num)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
DOUBLE *Value_List[];
DOUBLE min[], max[];
UINT *table_num, *row_num;
#endif
    {
    /*
       * this macro defines a normalized value...
    */
#define NORMALIZE(x,xmin,xrange) (DOUBLE)(((xrange) > 0.0) ? \
                                  (((x) - (xmin)) / (xrange)) : 1.0)
            
    INT status = SUCCESS;
    UINT i;
    UINT row, min_row;
    INT Units[3];
    INT Data_Types[3];
    size_t Widths[3];
    UINT nrows;
    UINT nTable;
    DOUBLE rect_xmin, rect_xmax, rect_ymin, rect_ymax;
    DOUBLE temp_1, temp_2;
    DOUBLE Table_xmin, Table_xmax, Table_ymin, Table_ymax;
    DOUBLE minx, maxx, miny, maxy, rangex, rangey;
    DOUBLE dist1, dist2, distance, min_distance, min_row_distance;
    DOUBLE slope, intercept;
    DOUBLE *px, *py;
    DOUBLE *Double_List[2];
    DOUBLE *temp_list[2];
    POINT2D out_row, proj_row;
    DATA_HEADER *pdata;
    TABLE_HEADER *ptable;
    struct 
        {
        UINT row;
        UINT table;
        DOUBLE dist;
        } closest_so_far;
    
    INIT_TEST (nlist);
    EMPTY_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        COLUMN_TEST (nlist, Col_List[i]);
    
    if (nCol IS_NOT_EQUAL_TO 2)
        {
        return NL_INVALID_COLUMN;
        }
    if (nlist->total_rows < 2)
        {
        return NL_INVALID_ROW;
        }
    /*
       * for each table, get the rows for the two columns to be tested
       * normalize the data prior to 'distance' calculation
    */
    
    minx = min[0];
    maxx = max[0];
    miny = min[1];
    maxy = max[1];
    
    for (i = 0; i < nCol; i++)
        {
        Data_Types[i] = FLOAT_TYPE;
        Widths[i] = sizeof(DOUBLE);
        Units[i] = 0;
        }
    temp_list[0] = &Table_xmin;
    temp_list[1] = &Table_ymin;
    
    rangex = (nlist->total_rows < 2) ? 0.0 : maxx - minx;
    proj_row.x = NORMALIZE (*Value_List[0], minx, rangex);
    rangey = (nlist->total_rows < 2) ? 0.0 : maxy - miny;
    proj_row.y = NORMALIZE (*Value_List[1], miny, rangey);
    
    /*  compute the minimums and maximums of the rectangle around the 
        search row and normalize these         */
    
    temp_1 = *Value_List[0] + rangex * 0.05;
    temp_2 = *Value_List[0] - rangex * 0.05;
    rect_xmin = MIN (temp_1, temp_2);
    rect_xmax = MAX (temp_1, temp_2);
    
    temp_1 = *Value_List[1] + rangey * 0.05;
    temp_2 = *Value_List[1] - rangey * 0.05;
    rect_ymin = MIN (temp_1, temp_2);
    rect_ymax = MAX (temp_1, temp_2);
    
    rect_xmin = NORMALIZE (rect_xmin, minx, rangex);
    rect_xmax = NORMALIZE (rect_xmax, minx, rangex);
    rect_ymin = NORMALIZE (rect_ymin, miny, rangey);
    rect_ymax = NORMALIZE (rect_ymax, miny, rangey);
    
    closest_so_far.row = 0;
    closest_so_far.table = 0;
    closest_so_far.dist = HUGE_VAL;
    
    /*-----------------------------------------------------------------------*/
    /*  OUTER LOOP - cycle through each table  */
    
    for (status = nl_set_current_row (nlist, (UINT)1, nTable = 1);
         nTable <= nlist->total_tables;
         status = nl_set_current_row (nlist, (UINT)1, ++nTable))
        {
        if (status < 0)
            continue;
        
        nl_inq_table_int (nlist, nTable, NL_TABLE_NROWS, (INT *)&i);
        if (i < 1)
            continue;
        
        nrows = i;
        
        /*      find the min and max values in this table and normalize */
        
        status = nl_inq_table_info (nlist, nTable, NL_TABLE_MIN, (UINT)2, Col_List,
                                    (VOIDPTR *)temp_list);
        
        ptable = nlz_set_table (nlist, nlist->current_table);
        if (ptable EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        pdata = ptable->data_ptr + Col_List[0] - 1;
        Table_xmin = pdata->min;
        Table_xmax = pdata->max;
        pdata = ptable->data_ptr + Col_List[1] - 1;
        Table_ymin = pdata->min;
        Table_ymax = pdata->max;
        
        Table_xmin = NORMALIZE (Table_xmin, minx, rangex);
        Table_xmax = NORMALIZE (Table_xmax, minx, rangex);
        Table_ymin = NORMALIZE (Table_ymin, miny, rangey);
        Table_ymax = NORMALIZE (Table_ymax, miny, rangey);
        
        /*      test whether this table needs to be considered  */
        
        if ((Table_xmin > rect_xmax) || (Table_xmax < rect_xmin) ||
            (Table_ymin > rect_ymax) || (Table_ymax < rect_ymin))
            continue;               /* discard this table */
        
        /* ...otherwise, set up to consider each two row segment in the table */
        /*     allocate enough doubles to contain the data rows  */
        
        Double_List[0] = px = (DOUBLE *) tc_alloc (nrows * sizeof(DOUBLE));
        if (Double_List[0] == (DOUBLE *)0)
            {
            return NL_OVERFLOW;
            }
        Double_List[1] = py = (DOUBLE *) tc_alloc (nrows * sizeof(DOUBLE));
        if (Double_List[1] == (DOUBLE *)0)
            {
            return NL_OVERFLOW;
            }
        /*     retrieve the data rows; convert them to doubles on the way out */
        
        status = nl_get_rows_convert (nlist, nrows, nCol, Col_List,
                                      (VOIDPTR *)Double_List, Units, Widths,
                                      Data_Types, &nrows);
        
        /*     normalize the data rows */
        
        for (i = 0; i < nrows; i++, px++, py++)
            {
            *px = NORMALIZE (*px, minx, rangex);
            *py = NORMALIZE (*py, miny, rangey);
            }
        /*
           * measure the distance between the 'current' segment
           * (defined by the line segment between the 'current' position and the next position
           * in the n-List) and the test row
           *
           * if the current table only has one row, the distance is equals to the
           * distance bewteen the specified row and the row.
        */
        if (nrows EQUALS 1)
            {
            min_distance = hypot ((proj_row.x - *px), (proj_row.y - *py));
            min_row = 1;
            }
        else
            {
            
            /*     INNER LOOP - cycle through all two row segments in the table */
            
            min_distance = HUGE_VAL;
            
            for (i = 1, px = Double_List[0], py = Double_List[1]; i < nrows;
                 i++, px++, py++)
                {
                /*               Check to see if this segment is worth considering  */
                
                if ((MIN ((*px), (*(px + 1))) > rect_xmax) OR (MAX ((*px),
                                                                    (*(px + 1))) <
                                                               rect_xmin) 
                    OR (MIN ((*py),
                             (*(py + 1))) > rect_ymax) OR (MAX ((*py),
                                                             (*(py + 1))) < rect_ymin))
                    continue;
                
                /*                if segment is in range, do the full distance calculations */
                /*                First do the special slope cases  */
                
                if (*px == *(px + 1))   /*  slope is infinite  */
                    {
                    distance = ABS (*px - proj_row.x);
                    out_row.x = *px;
                    out_row.y = proj_row.y;
                    }
                else if (*py == *(py + 1))  /*  slope is zero  */
                    {
                    distance = ABS (*py - proj_row.y);
                    out_row.x = proj_row.x;
                    out_row.y = *py;
                    }
                else
                    {
                    slope = (*py - *(py + 1)) / (*px - *(px + 1));
                    intercept = *py - slope * *px;
                    
                    distance = ag_proj_2dpoint_line (&proj_row, slope, intercept,
                                                     atan (-1 / slope), &out_row);
                    }
                /*             For this segment, find which of the two end rows is closer
                           to the given row                                   */
                dist1 = hypot (proj_row.x - *px, proj_row.y - *py);
                dist2 = hypot (proj_row.x - *(px + 1), proj_row.y - *(py + 1));
                if (dist1 < dist2)
                    {
                    min_row_distance = dist1;
                    row = i;
                    }
                else
                    {
                    min_row_distance = dist2;
                    row = i + 1;
                    }
                /*         check if the projected row lies on the segment.  If it does not,
                       then the distance to the segment becomes the distance to the closest
                       end row. Otherwise, the distance to the segment remains the perp
                        distance from the row to the line                     */
                
                if ((out_row.x < MIN (*px,
                                      *(px + 1))) OR (out_row.x >
                                                      MAX (*px,
                                                           *(px +
                                                            1))) OR (out_row.y <
                                                                     MIN (
                                                                        *py,
                                                                          *(
                                                                         py 
                                                                    + 1))) 
                    OR (out_row.y > MAX (*py, *(py + 1))))
                    {
                    distance = min_row_distance;
                    }
                /*        is this segment the closest to date within this table ? If so, 
                      revise min_distance and min_row for this table      */
                
                if (distance < min_distance)
                    {
                    min_distance = distance;
                    min_row = row;
                    }
                }
            /*  end of inner loop  */
            
            }
        /*  end of big else  */
        
        tc_free ((VOIDPTR)Double_List[0]);
        tc_free ((VOIDPTR)Double_List[1]);
        
        /*      Did this table contain the minimum distance recorded so far. If so, 
                make note of the fact  */
        
        if (min_distance < closest_so_far.dist)
            {
            closest_so_far.dist = min_distance;
            closest_so_far.row = min_row;
            closest_so_far.table = nTable;
            }
        }
    /* end of outer loop  */
    
    *table_num = closest_so_far.table;
    *row_num = closest_so_far.row;
    
    if (*table_num <= 0)
        status = FAIL;
    else
        status = SUCCESS;
    
    return status;
    
    }
/* END:     */
