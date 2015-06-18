/* DEC/CMS REPLACEMENT HISTORY, Element NL_PICK_ROW.C */
/* *7    25-OCT-1990 12:24:34 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *6    25-JUL-1990 15:01:46 VINCE "(SPR 5644) Header Standardization" */
/* *5    12-MAR-1990 19:08:33 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *4     5-MAR-1990 10:48:15 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *3    26-JAN-1990 14:27:51 WALTERS "(SPR 0) Fix bug in nl_set_nlist_info" */
/* *2    17-JAN-1990 15:37:09 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions" */
/* *1    24-SEP-1989 22:35:37 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PICK_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PCK_STK.C*/
/* *2    30-JUL-1989 00:00:15 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:19:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PCK_STK.C*/
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

#include "esi_nl_int.h"
#include "esi_ag.h"
#include "esi_tc.h"
#include "esi_math.h"

/*
 * this macro defines a normalized value...
 */
#define NORMALIZE(x,xmin,xrange) (DOUBLE)(((xrange) > 0.0) ? \
                                  (((x) - (xmin)) / (xrange)) : 1.0)
        

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nl_pick_row(NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        DOUBLE *Value_List[], DOUBLE min[], DOUBLE max[], NLIST_HEADER *Segment_nList);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    nCol            -(UINT) This version only handles 2 dims...
    Col_List        -(UINT *)
    Value_List      -(DOUBLE *[])
    min             -(DOUBLE [])
    max             -(DOUBLE [])
    Segment_nList   -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_ROW
    NL_INVALID_COLUMN
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_pick_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                           DOUBLE *Value_List[], DOUBLE min[], DOUBLE max[],
                           NLIST_HEADER *Segment_nList)
#else
publicdef INT nl_pick_row (nlist, nCol, Col_List, Value_List, min, max, Segment_nList)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
DOUBLE *Value_List[];
DOUBLE min[], max[];
NLIST_HEADER *Segment_nList;
#endif
    {
    UINT i;
    UINT nsorts;
    INT status = SUCCESS;
    UINT nsldim = 3;
    UINT nrows;
    UINT ntable;
    DOUBLE minx, maxx, miny, maxy, rangex, rangey;
    DOUBLE normalized_x, normalized_y;
    POINT2D out_point, proj_point;
    DOUBLE dist1, dist2, distance;
    DOUBLE slope, intercept;
    NLIST_HEADER Sorted_nList;
    INT Units[3];
    INT Value_Types[3];
    UINT DL[3];
    INT Data_Types[3];
    size_t Widths[3];
    INT Sort_Flags[1];
    UINT Sort_Dim[1];
    VOIDPTR VL[3];
    DOUBLE *px, *py, *Double_List[2];
    CHAR c_name[3][32];
    
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
       * initialize output Segment_nList to contain the numbers, segment numbers
       * and distances from the specified row
    */
    
    for (i = 0; i < nsldim; i++)
        {
        Units[i] = 0;
        Value_Types[i] = VALUE_LIST;
        DL[i] = i + 1;
        }
    Data_Types[0] = INT_TYPE;       /* table number */
    Data_Types[1] = INT_TYPE;       /* segment number */
    Data_Types[2] = FLOAT_TYPE;     /* normalized distance */
    Widths[0] = sizeof(INT);
    Widths[1] = sizeof(INT);
    Widths[2] = sizeof(DOUBLE);
    VL[0] = (VOIDPTR)&ntable;
    VL[1] = (VOIDPTR)&i;
    VL[2] = (VOIDPTR)&distance;
    status = nl_init_nlist (Segment_nList, nsldim, Units, Widths, Data_Types,
                            Value_Types, (UINT)0);
    if (status)
        {
        return status;
        }
    nl_inq_nlist_info (nlist, NL_NLIST_COLUMN_NAME, nsldim, DL, (VOIDPTR *)c_name);
    status = nl_set_nlist_info (*Segment_nList, NL_NLIST_COLUMN_NAME, nsldim, DL,
                                (VOIDPTR *)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_table (*Segment_nList, &ntable);
    /*
       * for each table, get the rows for the two columns to be tested
       * normalize the data prior to 'distance' calculation
    */
    
    minx = min[0];
    maxx = max[0];
    miny = min[1];
    maxy = max[1];
    
    rangex = (nlist->total_rows < 2) ? 0.0 : maxx - minx;
    normalized_x = NORMALIZE (*Value_List[0], minx, rangex);
    rangey = (nlist->total_rows < 2) ? 0.0 : maxy - miny;
    normalized_y = NORMALIZE (*Value_List[1], miny, rangey);
    
    for (i = 0; i < nCol; i++)
        {
        Data_Types[i] = FLOAT_TYPE;
        Widths[i] = sizeof(DOUBLE);
        }
    for (status = nl_set_current_row (nlist, (UINT)1, ntable = 1);
         ntable <= nlist->total_tables;
         status = nl_set_current_row (nlist, (UINT)1, ++ntable))
        {
        /*
           * we may want to put a test in here that compares the row to an enclosing
           * rectangle in the table... if the row is farther than the internal
           * tolerance from this window, then don't even consider this table...
        */
        
        /*
           * get the number of rows in this table
           * allocate enough doubles to contain the data rows
        */
        if (status < 0)
            continue;
        nl_inq_table_int (nlist, ntable, NL_TABLE_NROWS, (INT *)&i);
        nrows = i;
        if (nrows < 2)
            continue;
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
        /*
           * retrieve the data rows; convert them to doubles on the way out
        */
        nl_get_rows_convert (nlist, nrows, nCol, Col_List, (VOIDPTR *)Double_List,
                             Units, Widths, Data_Types, &nrows);
        /*
           * normalize the data rows
        */
        for (i = 0; i < nrows; i++, px++, py++)
            {
            *px = NORMALIZE (*px, minx, rangex);
            *py = NORMALIZE (*py, miny, rangey);
            }
        /*
           * measure the distance between the 'current' segment
           * (defined by the line segment between the 'current' position and the next position
           * in the n-List) and the test row
        */
        proj_point.x = normalized_x;
        proj_point.y = normalized_y;
        
        /*
           * if the current table only has one row, the distance is equals to the
           * distance bewteen the spcified row and the row.
        */
        if (nrows EQUALS 1)
            {
            distance = sqrt ((normalized_x - *px) * (normalized_x - *px) +
                             (normalized_y - *py) * (normalized_y - *py));
            nl_add_rows (*Segment_nList, (UINT)1, nsldim, DL, VL);
            }
        else
            for (i = 1, px = Double_List[0], py = Double_List[1]; i < nrows;
                 i++, px++, py++)
                {
                if (*px EQUALS * (px + 1))
                    slope = 99999.99;
                else
                    slope = (*py - *(py + 1)) / (*px - *(px + 1));
                /*
                   * compute the distance between the row and this line segment normal
                   * (perpendicular) to the line...
                */
                
                /*
                   * If the slope is equals to zero, the the distance is equals to the distance
                   * between two Y value.
                */
                if (slope EQUALS 0.0)
                    {
                    distance = sqrt ((*py - normalized_y) * (*py - normalized_y));
                    out_point.y = *py;
                    out_point.x = normalized_x;
                    }
                else
                    {
                    intercept = *py - slope * *px;
                    distance = ag_proj_2dpoint_line (&proj_point, slope, intercept,
                                                     atan (-1 / slope), &out_point);
                    }
                /*
                   * if this projected row does not fall in the line segment,
                   * then take the closest row
                */
                if ((out_point.x < MIN (*px,
                                        *(px + 1))) OR (out_point.x >
                                                        MAX (*px,
                                                            *(px +
                                                              1))) 
                    OR (out_point.y < MIN (*py,
                                           *(py + 1))) OR (out_point.y >
                                                           MAX (*py, *(py + 1))))
                    {
                    dist1 = hypot (proj_point.x - *px, proj_point.y - *py);
                    dist2 = hypot (proj_point.x - *(px + 1), proj_point.y - *(py + 1));
                    distance = MIN (dist1, dist2);
                    }
                nl_add_rows (*Segment_nList, (UINT)1, nsldim, DL, VL);
                }
        tc_free ((VOIDPTR)Double_List[0]);
        tc_free ((VOIDPTR)Double_List[1]);
        }
    /*
       * now that we have a distance from the test row to each segment,
       * sort this n-List based on the distance
       * The closest table/segment combination will be listed first
    */
    nsorts = 1;
    Sort_Dim[0] = 3;
    Sort_Flags[0] = NL_ASCENDING;
    status = nl_sort_table (*Segment_nList, nsorts, Sort_Dim, Sort_Flags,
                            &Sorted_nList);
    
    /*
       * return the results to the programmer.  They are responsible for calling
       * nl_free_nlist for the (now sorted) list of distances...
    */
    nl_free_nlist (*Segment_nList);
    *Segment_nList = Sorted_nList;
    
    return status;
    }
/* END:     */
