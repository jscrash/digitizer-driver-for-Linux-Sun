/* DEC/CMS REPLACEMENT HISTORY, Element NLMWCLP_PLY.C*/
/* *9    25-OCT-1990 12:16:52 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *8     4-SEP-1990 12:25:44 GILLESPIE "(SPR 5284) Fix problem when only one table being clipped - needed to reset 1st point"*/
/* *7    14-AUG-1990 12:36:11 GILLESPIE "(SPR 5284) Major fix to polygon clipper - still does not handle nulls"*/
/* *6    19-JUL-1990 18:41:20 GILLESPIE "(SPR 1) Fix to polygon clip algorithm"*/
/* *5    13-APR-1990 10:49:34 GILLESPIE "(SPR 1) Check n-List for updated condition before window test"*/
/* *4    25-MAR-1990 13:04:29 GILLESPIE "(SPR 1) Add trivial test for n-List outside clipping rectangle"*/
/* *3    25-MAR-1990 10:14:32 GILLESPIE "(SPR 1) Fix problem wrt dropping last point of tables"*/
/* *2    12-MAR-1990 19:02:24 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 12:47:18 GILLESPIE "CHANGED NAME"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLMWCLP_PLY.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_nl_int.h"
#include "esi_nl_meth_hdr.h"

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_AG_H
#include "esi_ag.h"
#endif

typedef struct 
    {
    DOUBLE x, y;
    } VERTEX;

#define BOUNDARY_COUNT 4
static VERTEX s[BOUNDARY_COUNT];
static VERTEX first_point[BOUNDARY_COUNT];
static BOOL new_edge[BOUNDARY_COUNT];
static size_t *double_size;
static INT *double_type;
static INT *units;
static NLIST_HEADER outNlist;       /*     Output n-List                        */
static UINT *totalDimList;
static DOUBLE maxX, minX;           /*   the minmum and maxmum value of 'X' nCol.   */
static DOUBLE maxY, minY;           /*   the minmum and maxmum value of 'Y' nCol.   */

#if USE_PROTOTYPES
static VOID clip_this (VERTEX *p, INT edge);
static BOOL cross (VERTEX *p, VERTEX *s, INT edge);
static VOID find_intersection (VERTEX *p, VERTEX *s, INT edge, VERTEX *i);
static BOOL inside (VERTEX *p, INT edge);
static VOID output_vertex (VERTEX *p);
#else
static VOID clip_this ();
static BOOL cross ();
static VOID find_intersection ();
static BOOL inside ();
static VOID output_vertex ();
#endif

/* *****************************************************************************    */
/*                                                                                  */
/*    ROUTINE:  NLM_WINDOW_CLIP_POLY                                                */
/*                                                                                  */
/*    DESCRIPTION                                                                   */
/*     The routine only does two dimensional clipping.  Given an n-List and         */
/*     specified column, maximum , minimum boundaries, then returns a clipped       */
/*     n-List.  The maximum and minimum value should be same as the specified       */
/*     the data type of the column.                                                 */
/*                                                                                  */
/*     Reference: Hearn, Donald, and Baker, M Pauline, 1986, Computer Graphics,     */
/*      Prentice-Hall, Inc, p. 134 - 139.  after Sutherland & Hodgman               */
/*                                                                                  */
/*    RESTRICTION                                                                   */
/*     All columns in the given n-List must be a VALUE_LIST, but not                */
/*     INCREMENT_LIST.                                                              */
/*                                                                                  */
/*    **************************************************************************    */
#if USE_PROTOTYPES
publicdef INT nlm_window_clip_poly (NLIST_HEADER nlist, VOIDPTR param)
#else
publicdef INT nlm_window_clip_poly (nlist, param)
NLIST_HEADER nlist;                     /* the pointer given N-list.                */
VOIDPTR param;
#endif
    {
    /* **************************************************************************** */
    
    NLM_CLIP_HEADER *paramBlk = (NLM_CLIP_HEADER *)param;
    UINT nCol = 2;
    DATA_HEADER *pxdata;                /*   workspace for current data header.     */
    DATA_HEADER *pydata;                /*   workspace for current data header.     */
    COLUMN_HEADER *pinfo;               /*   workspace for current column.          */
    COLUMN_HEADER *poutinfo;            /*   workspace for current column.          */
    COLUMN_HEADER *pxinfo;              /*   workspace for x column.                */
    COLUMN_HEADER *pyinfo;              /*   workspace for y column.                */
    TABLE_HEADER *ptable;               /*   workspace for current table.           */
    UINT nallocated_points;
    UINT i, j, indexPt;                 /*   index.                                 */
    UINT totalNdim;                     /*   the total number of columns in N-list  */
    INT *incValue;                      /*   workspace for initializing a N-list.   */
    size_t *size;
    UINT blockSize;
    INT *type;                          /*   workspace for initializing a N-list.   */
    UINT table_num;                     /*   workspace for table number.            */
    UINT returnPts;                     /*   the really return rows from NL_GET_ROWS*/
    INT status = SUCCESS;               /*   returned flag.                         */
    UINT xdim;                          /*   first (x) column                       */
    UINT ydim;                          /*   second (y) column                      */
    DOUBLE **doubleArray;               /*   workspace for retrieving DOUBLE points.*/
    DOUBLE **clipArray;                 /*   the array of been clipped points.      */
    DOUBLE x1;                          /*   positions of two points.               */
    /*   others columns.                                                            */
    DOUBLE high_null[2];                /*   the high null value of two specified dims. */
    DOUBLE low_null[2];                 /*   the low null value of two specified dims.  */
    INT edge;
    
    if (param EQUALS (VOIDPTR)0)
        {
        return FAIL;
        }
    paramBlk->outNlist = (NLIST_HEADER)0;       /* set to null in case of failure   */
    
    INIT_TEST (nlist);
    
    if (nCol != 2)
        {
        return NL_INVALID_COLUMN_SIZE;
        }
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, paramBlk->Col_List[i]);
        }
    xdim = paramBlk->Col_List[0] - 1;
    ydim = paramBlk->Col_List[1] - 1;
    
    /* check for updated n-List                                                     */
    
    if (nlist->updated)
        {
        BOOL defer = nlist->defer_stats;
        
        nlist->defer_stats = FALSE;
        nlz_compute_nlist_stats (nlist);
        nlist->defer_stats = defer;
        }
    /* **************************************************************************** */
    /*   error checking.                                                            */
    /* If there is NULL VALUE in any                                                */
    /* table, return error status.                                                  */
    for (i = 0; i < nlist->total_tables; i++)
        {
        ptable = nlz_set_table (nlist, i + 1);
        
        if (ptable == NULL_TABLE)
            {
            continue;
            }
        if ((ptable->nrows != (ptable->data_ptr + xdim)->n_valid_rows) OR
            (ptable->nrows != (ptable->data_ptr + ydim)->n_valid_rows))
            {
            return NL_INVALID_NULL_VALUE;
            }
        }
    /*   convert the minmum and maxmum value into DOUBLE                            */
    
    minX = paramBlk->minList[0];
    minY = paramBlk->minList[1];
    maxX = paramBlk->maxList[0];
    maxY = paramBlk->maxList[1];
    
    /* Make sure there is clipping work to perform before getting
       too much farther  */
    pxinfo = nlist->info_ptr + xdim;
    pyinfo = nlist->info_ptr + ydim;
    
    if (pxinfo->max < minX OR
        pxinfo->min > maxX OR
        pyinfo->max < minY OR
        pyinfo->min > maxY)
        {
        paramBlk->outNlist = (NLIST_HEADER)0;
        return NL_EMPTY_NLIST;
        }

    /* Now see if everything is within the window, in which case there's
       also very little work required */

    if (pxinfo->max <= maxX AND
        pxinfo->min >= minX AND
        pyinfo->max <= maxY AND
        pyinfo->min >= minY)
        {
        return nl_copy_nlist(nlist, &paramBlk->outNlist);
        }

    /* No such luck...  back to work */

    totalNdim = nlist->nCol;
    blockSize = nlist->blocksize;
    
    /* Allocate temporary memory                                                    */
    
    units = (INT *)TALLOC (sizeof(INT) * totalNdim);
    if (units EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    incValue = (INT *)TALLOC (sizeof(INT) * (totalNdim));
    if (units EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    type = (INT *)TALLOC (sizeof(INT) * (totalNdim));
    if (units EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    size = (size_t *)TALLOC (sizeof(size_t) * (totalNdim));
    if (units EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    totalDimList = (UINT *)TALLOC (sizeof(UINT) * (totalNdim));
    if (totalDimList EQUALS (UINT *)0)
        {
        return NL_OVERFLOW;
        }
    clipArray = (DOUBLE **)TALLOC (sizeof(DOUBLE *) * (totalNdim));
    if (clipArray EQUALS (DOUBLE **)0)
        {
        return NL_OVERFLOW;
        }
    doubleArray = (DOUBLE **)TALLOC (sizeof(DOUBLE *) * (totalNdim));
    if (doubleArray EQUALS (DOUBLE **)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < totalNdim; i++)
        {
        totalDimList[i] = i + 1;
        pinfo = nlist->info_ptr + i;
        units[i] = pinfo->unit;
        size[i] = pinfo->data_size;
        type[i] = pinfo->data_type;
        incValue[i] = VALUE_LIST;
        
        /* set the tolerance range of NULL for two                                  */
        /* specified column.                                                        */
        if (i == xdim)
            {
            x1 = pinfo->null_value;
            high_null[0] = (x1 > 0) ? x1 * 1.01 : x1 * 0.99;
            low_null[0] = (x1 > 0) ? x1 * 0.99 : x1 * 1.01;
            }
        if (i == ydim)
            {
            x1 = pinfo->null_value;
            high_null[1] = (x1 > 0) ? x1 * 1.01 : x1 * 0.99;
            low_null[1] = (x1 > 0) ? x1 * 0.99 : x1 * 1.01;
            }
        }
    nl_init_nlist (&(paramBlk->outNlist), totalNdim, units, size, type, incValue,
                   blockSize);
    outNlist = paramBlk->outNlist;
    /* copy the column names and null value for each column...                      */
    for (i = 0, pinfo = nlist->info_ptr, poutinfo = outNlist->info_ptr; i < totalNdim;
         i++, pinfo++, poutinfo++)
        {
        strcpy ((char *)poutinfo->column_name, (char *)pinfo->column_name);
        strcpy ((char *)poutinfo->convert_format, (char *)pinfo->convert_format);
        poutinfo->null_value = pinfo->null_value;
        }
    /* set the user structure in the NLIST_HEADER.                                  */
    
    if (nlist->user_struct_len > 0)
        {
        status = nlz_copy_structure (nlist->user_struct, nlist->user_struct_len,
                                     &outNlist->user_struct, &outNlist->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /*   clips each table.                                                          */
    
    double_size = (size_t *)TALLOC (totalNdim * sizeof(size_t));
    if (double_size EQUALS (size_t *)0)
        {
        return NL_OVERFLOW;
        }
    double_type = (INT *)TALLOC (totalNdim * sizeof(INT));
    if (double_type EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < totalNdim; i++)
        {
        double_size[i] = sizeof(DOUBLE);
        double_type[i] = FLOAT_TYPE;
        }
    /*   allocate space for returned DOUBLE points                                  */
    /*   and clipped points array.                                                  */
    
    nallocated_points = MIN (500, nlist->total_rows);
    
    for (j = 0; j < totalNdim; j++)
        {
        doubleArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) * nallocated_points);
        if (doubleArray[j] == (DOUBLE *)0)
            {
            return NL_OVERFLOW;
            }
        clipArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) * nallocated_points);
        if (clipArray[j] == (DOUBLE *)0)
            {
            return NL_OVERFLOW;
            }
        }
    for (i = 1; i <= nlist->total_tables; i++)
        {
        if (paramBlk->autopurge AND i > 1)
            {
            nl_purge_table (nlist, (UINT)0);
            }
        ptable = nlz_set_table (nlist, i);
	nlist -> current_row = 1;   /* Set to 1st row in this table */
        
        if (ptable == NULL_TABLE OR ptable->nrows == 1)
            {
            continue;
            }
        pxdata = ptable->data_ptr + xdim;
        if (pxdata->max < minX OR pxdata->min > maxX)
            {
            continue;
            }
        pydata = ptable->data_ptr + ydim;
        if (pydata->max < minY OR pydata->min > maxY)
            {
            continue;
            }
        /* If necessary, enlarge the allocated space to receive the data rows       */
        
        if (ptable->nrows > nallocated_points)
            {
            nallocated_points = ptable->nrows;
            for (j = 0; j < totalNdim; j++)
                {
                tc_free ((VOIDPTR)doubleArray[j]);
                tc_free ((VOIDPTR)clipArray[j]);
                doubleArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) * nallocated_points);
                if (doubleArray[j] == (DOUBLE *)0)
                    {
                    return NL_OVERFLOW;
                    }
                clipArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) * (nallocated_points +
                                                                     2));
                if (clipArray[j] == (DOUBLE *)0)
                    {
                    return NL_OVERFLOW;
                    }
                }
            }
        /*   get all rows in the current table, and converted to be DOUBLE.         */
        
        status = nl_get_rows_convert (nlist, ptable->nrows, totalNdim, totalDimList,
                                      (VOIDPTR *)doubleArray, units, double_size,
                                      double_type, &returnPts);
        if (status < 0)
            {
            break;
            }
        /* Address the trivial case where the entire stroke is inside the window    */
        
        if (pxdata->min >= minX AND pxdata->max <= maxX AND pydata->min >=
            minY AND pydata->max <= maxY)
            {
            nl_start_table (outNlist, &table_num);
            status = nlz_copy_structure (ptable->user_struct, ptable->user_struct_len,
                                         &outNlist->current_table_ptr->user_struct,
                                         &outNlist->current_table_ptr->user_struct_len);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            if (paramBlk->autopurge)
                {
                nl_purge_table (nlist, (UINT)0);
                }
            status = nl_add_rows_convert (outNlist, ptable->nrows, totalNdim,
                                          totalDimList, (VOIDPTR *)doubleArray, units,
                                          double_size, double_type);
            
            continue;
            }
        /* SOMETHING will be output, so initialize the table ...                    */
        
        nl_start_table (outNlist, &table_num);
        status = nlz_copy_structure (ptable->user_struct, ptable->user_struct_len,
                                     &outNlist->current_table_ptr->user_struct,
                                     &outNlist->current_table_ptr->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        if (paramBlk->autopurge)
            {
            nl_purge_table (nlist, (UINT)0);
            }

        for (j = 0; j < BOUNDARY_COUNT; j++)
            {
            new_edge[j] = TRUE;
            }
        for (indexPt = 0; indexPt < returnPts; indexPt++)
            {
            VERTEX point;
            
            point.x = doubleArray[xdim][indexPt];
            point.y = doubleArray[ydim][indexPt];
            clip_this (&point, (INT)0);
            }

        /* Finish up clipping (Clip closer - rhymes with bulldozer) - in original code */
        
        for (edge = 0; edge < BOUNDARY_COUNT; edge++)
            {
            VERTEX point;
            
            if (cross (s + edge, first_point + edge, edge))
                {
                find_intersection (s + edge, first_point + edge, edge, &point);
                if (edge < (BOUNDARY_COUNT-1))
                    {
                    clip_this (&point, edge + 1);
                    }
                else
                    {
                    output_vertex (&point);
                    }
                }
            }
        }
    /*   free the allocated temp. workspace.                                        */
    for (j = 0; j < totalNdim; j++)
        {
        tc_free ((VOIDPTR)doubleArray[j]);
        tc_free ((VOIDPTR)clipArray[j]);
        }

#if NEED_TO_FREE == TRUE
    /* free all allocated memory.                                                   */
    
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)incValue);
    tc_free ((VOIDPTR)type);
    tc_free ((VOIDPTR)size);
    tc_free ((VOIDPTR)totalDimList);
    tc_free ((VOIDPTR)clipArray);
    tc_free ((VOIDPTR)doubleArray);
    tc_free ((VOIDPTR)double_size);
    tc_free ((VOIDPTR)double_type);
#endif
    if (outNlist->total_rows == 0)
        {
        nl_free_nlist (paramBlk->outNlist);
        paramBlk->outNlist = (NLIST_HEADER)0;
        return NL_EMPTY_NLIST;
        }
    else
        {
        return SUCCESS;
        }
    }

/**************************************************************************************/
/*                                                                                    */
/*        Rosette Stone     The Following numbers correspond to the EDGE number       */
/*           xmax                 1                                                   */
/*             +------------------------------------+ ymax                            */
/*             |                                    |                                 */
/*             |                                    |                                 */
/*             |                                    |                                 */
/*            0|                                    |2                                */
/*             |                                    |                                 */
/*             |                                    |                                 */
/*             |                                    |                                 */
/*        ymin +------------------------------------+                                 */
/*                                3               xmin                                */
/*                                                                                    */
/**************************************************************************************/

#if USE_PROTOTYPES
static VOID clip_this (VERTEX *p, INT edge)
#else
static VOID clip_this (p, edge)
VERTEX *p;
INT edge;
#endif
    {
    VERTEX i;
    
    if (new_edge[edge] EQUALS TRUE)
        {
        first_point[edge] = *p;
        new_edge[edge] = FALSE;
        }
    else
        {
        if (cross (p, s + edge, edge))
            {
            find_intersection (p, s + edge, edge, &i);
            if (edge < (BOUNDARY_COUNT-1))
                {
                clip_this (&i, edge + 1);
                }
            else
                {
                output_vertex (&i);
                }
            }
        }
    s[edge] = *p;
    if (inside (p, edge))
        {
        if (edge < (BOUNDARY_COUNT-1))
            {
            clip_this (p, edge + 1);
            }
        else
            {
            output_vertex (p);
            }
        }
    }

/**************************************************************************************/

#if USE_PROTOTYPES
static VOID output_vertex (VERTEX *p)
#else
static VOID output_vertex (p)
VERTEX *p;
#endif
    {
    VOIDPTR VarList[2];
    
    VarList[0] = (VOIDPTR)&p->x;
    VarList[1] = (VOIDPTR)&p->y;
    
    nl_add_rows_convert (outNlist, (UINT)1, (UINT)2, totalDimList, VarList, units, double_size,
                         double_type);
    }

/**************************************************************************************/

/* returns TRUE if vertex p is inside window edge                                   */
#if USE_PROTOTYPES
static BOOL inside (VERTEX *p, INT edge)
#else
static BOOL inside (p, edge)
VERTEX *p;
INT edge;
#endif
    {
    if (edge < 2)
        {
        if (edge EQUALS 0)
            {
            return (BOOL)(p->x > minX);
            }
        else                            /* edge EQUALS 1                            */
            {
            return (BOOL)(p->y < maxY);
            }
        }
    else
        {
        if (edge EQUALS 2)
            {
            return (BOOL)(p->x < maxX);
            }
        else                            /* edge EQUALS 3                            */
            {
            return (BOOL)(p->y > minY);
            }
        }
    }

/**************************************************************************************/

/* returns TRUE if polygon side ps intersects window edge                           */
#if USE_PROTOTYPES
static BOOL cross (VERTEX *p, VERTEX *s, INT edge)
#else
static BOOL cross (p, s, edge)
VERTEX *p;
VERTEX *s;
INT edge;
#endif
    {
    if (edge < 2)
        {
        if (edge EQUALS 0)
            {
            if (p->x < s->x)
                {
                return (BOOL)(p->x <= minX AND s->x >= minX);
                }
            else
                {
                return (BOOL)(s->x <= minX AND p->x >= minX);
                }
            }
        else                            /* edge EQUALS 1                            */
            {
            if (p->y < s->y)
                {
                return (BOOL)(p->y <= maxY AND s->y >= maxY);
                }
            else
                {
                return (BOOL)(s->y <= maxY AND p->y >= maxY);
                }
            }
        }
    else
        {
        if (edge EQUALS 2)
            {
            if (p->x < s->x)
                {
                return (BOOL)(p->x <= maxX AND s->x >= maxX);
                }
            else
                {
                return (BOOL)(s->x <= maxX AND p->x >= maxX);
                }
            }
        else                            /* edge EQUALS 3                            */
            {
            if (p->y < s->y)
                {
                return (BOOL)(p->y <= minY AND s->y >= minY);
                }
            else
                {
                return (BOOL)(s->y <= minY AND p->y >= minY);
                }
            }
        }
    }

/**************************************************************************************/

#if USE_PROTOTYPES
static VOID find_intersection (VERTEX *p, VERTEX *s, INT edge, VERTEX *i)
#else
static VOID find_intersection (p, s, edge, i)
VERTEX *p;
VERTEX *s;
INT edge;
VERTEX *i;
#endif
    {
    DOUBLE m, b;
    
    if (p->x EQUALS s->x)
        {
	/* Slope is undefined */
	if (edge < 2)
	    {
	    if (edge EQUALS 0)
		{
		i->x = minX;
		i->y = s->y;
		}
	    else                            /* edge EQUALS 1                            */
		{
		i->x = s->x;
		i->y = maxY;
		}
	    }
	else
	    {
	    if (edge EQUALS 2)
		{
		i->x = maxX;
		i->y = s->y;
		}
	    else                            /* edge EQUALS 3                            */
		{
		i->x = p->x;
		i->y = minY;
		}
	    }
	return;
	}

    m = (s->y - p->y) / (s->x - p->x);
    b = p->y - m * p->x;
    
    if (edge < 2)
        {
        if (edge EQUALS 0)
            {
            i->x = minX;
            i->y = m * minX + b;
            }
        else                            /* edge EQUALS 1                            */
            {
            i->x = (m EQUALS 0.0) ? (s->x) : ((maxY - b) / m);
            i->y = maxY;
            }
        }
    else
        {
        if (edge EQUALS 2)
            {
            i->x = maxX;
            i->y = m * maxX + b;
            }
        else                            /* edge EQUALS 3                            */
            {
            i->x = (m EQUALS 0.0) ? (s->x) : ((minY - b) / m);
            i->y = minY;
            }
        }
    }
