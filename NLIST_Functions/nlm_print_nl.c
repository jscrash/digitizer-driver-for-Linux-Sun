/* DEC/CMS REPLACEMENT HISTORY, Element NLM_PRINT_NL.C*/
/* *9    25-OCT-1990 12:17:05 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *8    25-JUL-1990 14:49:56 VINCE "(SPR 5644) Header Standardization"*/
/* *7     9-APR-1990 10:31:19 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *6    22-MAR-1990 01:34:17 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *5    12-MAR-1990 22:20:36 GILLESPIE "(SPR 1) fix printing of n-List name"*/
/* *4    12-MAR-1990 19:02:32 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *3     5-MAR-1990 10:43:45 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    25-OCT-1989 12:45:50 GILLESPIE "(SPR 1) Fixer upper"*/
/* *1    12-OCT-1989 09:53:36 GILLESPIE ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_PRINT_NL.C*/
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
#include "esi_nl_meth_hdr.h"
#include "esi_tc.h"
#include "esi_ts.h"
typedef char STRING[255];

static char string_fmt[] = " %10s";
static CHAR double_fmt[] = " %10.2f";
static CHAR int_fmt[] = " %10ld";

#if USE_PROTOTYPES
static void printheader (TCP column_names, FILE *pFile, UINT nCol);
static void printline (FILE *pFile, CHAR *s, UINT nCol, BYTE *array, INT dtype,
                           size_t dsize);
#else
static void printheader ();
static void printline ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    An n-List methods function to print the n-List.
    Param points to a structure which is used to affect
    the printing.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT nlm_print_nlist(NLIST_HEADER nlist, VOIDPTR param);

Private_Functions:
    static void printheader(TCP column_names, FILE *pFile, UINT nCol);
    
    static void printline(FILE *pFile, CHAR *s, UINT nCol, BYTE *array, INT dtype, size_t dsize);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlm_print_nlist(NLIST_HEADER nlist, VOIDPTR param);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    param           -(VOIDPTR)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_EMPTY_TABLE
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlm_print_nlist (NLIST_HEADER nlist, VOIDPTR param) 
#else
publicdef INT nlm_print_nlist (nlist, param)
NLIST_HEADER nlist;
VOIDPTR param;
#endif
    {
    NLM_PRINT_HEADER temp_header;
    NLM_PRINT_HEADER *pPrint = (NLM_PRINT_HEADER *)param;
    FILE *pFile;
    UINT nrows, ntables;
    UINT nCol;
    INT status;
    UINT i, j, k;
    UINT n_num_dims;
    UINT *dlist;
    UINT *Col_List;
    DOUBLE *s = (DOUBLE *)0;
    DOUBLE **sv = (DOUBLE **)0;
    INT *tempData_Types = (INT *)0;
    INT *ptype = (INT *)0;
    size_t *pwidth = (size_t *)0;
    size_t *ptempwidth = (size_t *)0;
    VOIDPTR *Var_List = (VOIDPTR *)0;
    STRING *strings = (STRING *)0;
    TCP column_names;
    DOUBLE *doubles = (DOUBLE *)0;
    INT *Units = (INT *)0;
    TABLE_HEADER *ptab;
    UINT return_p;
    
    INIT_TEST (nlist);
    if (pPrint EQUALS (NLM_PRINT_HEADER *)0)
        {
        /* No header was supplied - fall back on a default header, which
            prints only header information to stdout */
        pPrint = &temp_header;
        return FAIL;
        }
    pFile = pPrint->outFile;
    if (pFile EQUALS NULL_FILE_PTR)
        {
        pFile = stdout;
        }
    nCol = pPrint->nCol;
    Col_List = pPrint->Col_List;
    
    /* Allocate temporary storage for column data */
    
    dlist = (UINT *)TALLOC (nCol * sizeof(UINT));
    s = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    sv = (DOUBLE **)TALLOC (nCol * sizeof(DOUBLE *));
    tempData_Types = (INT *)TALLOC (nCol * sizeof(INT));
    ptype = (INT *)TALLOC (nCol * sizeof(INT));
    pwidth = (size_t *)TALLOC (nCol * sizeof(size_t));
    ptempwidth = (size_t *)TALLOC (nCol * sizeof(size_t));
    Var_List = (VOIDPTR *)TALLOC (nCol * sizeof(VOIDPTR));
    strings = (STRING *)TALLOC (nCol * sizeof(STRING));
    doubles = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    Units = (INT *)TALLOC (nCol * sizeof(INT));
    
    column_names = ts_tcp_alloc ((INT)nCol);
    for (i = 0; i < nCol; i++)
        {
        column_names[i] = (CHAR *)tc_zalloc (32);
        if (column_names[i] == (CHAR *)0)
            {
            return NL_OVERFLOW;
            }
        }
    nl_inq_nlist_info (nlist, NL_NLIST_COLUMN_NAME, nCol, Col_List,
                       (VOIDPTR *)column_names);
    
    for (i = 0, n_num_dims = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        ptype[i] = nlist->info_ptr[Col_List[i] - 1].data_type;
        pwidth[i] = nlist->info_ptr[Col_List[i] - 1].data_size;
        if (IS_EMPTY_STRING (column_names[i]))
            {
            sprintf ((char *)column_names[i], "Col:%6d", i + 1);
            }
        /*    For statistics, we want only to retrieve
            numeric columns, since they do not make
            sense for character strings            */
        
        if (ptype[i] EQUALS FLOAT_TYPE OR ptype[i] EQUALS INT_TYPE)
            {
            dlist[n_num_dims] = Col_List[i];
            tempData_Types[n_num_dims] = FLOAT_TYPE;
            ptempwidth[n_num_dims] = sizeof(DOUBLE);
            Var_List[n_num_dims] = (VOIDPTR)(doubles + i);
            sv[n_num_dims] = s + i;
            n_num_dims++;
            }
        else
            {
            doubles[i] = 0.0;
            s[i] = 0.0;
            }
        }
    if (pPrint->print_nlist_header)
        {
        nrows = nlist->total_rows;
        ntables = nlist->total_tables;
        
        fprintf (pFile, "n-List has a total of %ld column%s, \
%ld table%s and %ld row%s.\n"
                 , nlist->nCol, nlist->nCol EQUALS 1 ? "" : "s", ntables,
                 ntables EQUALS 1 ? "" : "s", nrows, nrows EQUALS 1 ? "" : "s");
        
        if (IS_STRING (nlist->name))
            {
            fprintf (pFile, "n-List name = %s\n", nlist->name);
            }
        if (nlist->user_struct_len > 0)
            {
            fprintf (pFile, "n-List user_struct has %ld bytes.\n",
                     nlist->user_struct_len);
            }
        else
            {
            fprintf (pFile, "No n-List user structure.\n");
            }
        printheader (column_names, pFile, nCol);
        for (i = 0; i < nCol; i++)
            {
            if (ptype[i] EQUALS CHAR_TYPE)
                {
                strcpy ((char *)strings[i], "CHAR_TYPE");
                }
            else if (ptype[i] EQUALS INT_TYPE)
                {
                strcpy ((char *)strings[i], "INT_TYPE");
                }
            else if (ptype[i] EQUALS FLOAT_TYPE)
                {
                strcpy ((char *)strings[i], "FLOAT_TYPE");
                }
            else if (ptype[i] EQUALS BOOL_TYPE)
                {
                strcpy ((char *)strings[i], "BOOL_TYPE");
                }
            else
                {
                strcpy ((char *)strings[i], "UNKNOWN");
                }
            }
        printline (pFile, (CHAR *)"Types", nCol, (BYTE *)strings, CHAR_TYPE, sizeof(STRING));
        printline (pFile, (CHAR *)"Widths", nCol, (BYTE *)pwidth, INT_TYPE, sizeof(INT));
        
        nl_inq_nlist_info_convert (nlist, NL_NLIST_NULL_VALUE, n_num_dims, dlist,
                                   Var_List, Units, ptempwidth, tempData_Types);
        printline (pFile, (CHAR *)"Nulls", nCol, (BYTE *)doubles, FLOAT_TYPE, sizeof(DOUBLE));
        nl_inq_nlist_info_convert (nlist, NL_NLIST_MIN, n_num_dims, dlist, Var_List,
                                   Units, ptempwidth, tempData_Types);
        printline (pFile, (CHAR *)"Minima", nCol, (BYTE *)doubles, FLOAT_TYPE, sizeof(DOUBLE));
        nl_inq_nlist_info_convert (nlist, NL_NLIST_MAX, n_num_dims, dlist, Var_List,
                                   Units, ptempwidth, tempData_Types);
        printline (pFile, (CHAR *)"Maxima", nCol, (BYTE *)doubles, FLOAT_TYPE, sizeof(DOUBLE));
        nl_inq_nlist_info (nlist, NL_NLIST_AVG, n_num_dims, dlist, (VOIDPTR *)sv);
        printline (pFile, (CHAR *)"Average", nCol, (BYTE *)s, FLOAT_TYPE, sizeof(DOUBLE));
        nl_inq_nlist_info (nlist, NL_NLIST_STD_DEV, n_num_dims, dlist, (VOIDPTR *)sv);
        printline (pFile, (CHAR *)"Std Dev", nCol, (BYTE *)s, FLOAT_TYPE, sizeof(DOUBLE));
        
        }
    if (pPrint->print_table_header OR pPrint->print_data)
        {
        TABLE_TEST (nlist, pPrint->first_table);
        TABLE_TEST (nlist, pPrint->last_table);
        for (i = pPrint->first_table; i <= pPrint->last_table; i++)
            {
            ptab = nlz_set_table (nlist, i);
            if (ptab == NULL_TABLE)
                {
                return NL_EMPTY_TABLE;
                }
            if (pPrint->print_table_header)
                {
                fprintf (pFile, "\n *** Table number = %ld has %ld row%s\n", i,
                         ptab->nrows, ptab->nrows EQUALS 1 ? "" : "s");
                if (ptab->user_struct_len > 0)
                    {
                    fprintf (pFile, " Table # %ld user structure length = %ld bytes\n",
                             i, ptab->user_struct_len);
                    }
                else
                    {
                    fprintf (pFile, "No user structure\n");
                    }
                printheader (column_names, pFile, nCol);
                nl_inq_table_info_convert (nlist, i, NL_TABLE_MIN, n_num_dims, dlist,
                                           Var_List, Units, ptempwidth,
                                           tempData_Types);
                printline (pFile, (CHAR *)"Minima", nCol, (BYTE *)doubles, FLOAT_TYPE,
                           sizeof(DOUBLE));
                nl_inq_table_info_convert (nlist, i, NL_TABLE_MAX, n_num_dims, dlist,
                                           Var_List, Units, ptempwidth,
                                           tempData_Types);
                printline (pFile, (CHAR *)"Maxima", nCol, (BYTE *)doubles, FLOAT_TYPE,
                           sizeof(DOUBLE));
                nl_inq_table_info (nlist, i, NL_TABLE_AVG, n_num_dims, dlist,
                                   (VOIDPTR *)sv);
                printline (pFile, (CHAR *)"Average", nCol, (BYTE *)s, FLOAT_TYPE,
                           sizeof(DOUBLE));
                nl_inq_table_info (nlist, i, NL_TABLE_STD_DEV, n_num_dims, dlist,
                                   (VOIDPTR *)sv);
                printline (pFile, (CHAR *)"Std Dev", nCol, (BYTE *)s, FLOAT_TYPE,
                           sizeof(DOUBLE));
                }
            if (pPrint->print_data)
                {
                /*    For now - format ALL numeric values as
                    floating point numbers for printing puposes */
                
                for (j = 0; j < nCol; j++)
                    {
                    if (ptype[j] EQUALS CHAR_TYPE)
                        {
                        Var_List[j] = (VOIDPTR)strings[j];
                        pwidth[j] = sizeof(STRING);
                        }
                    else
                        {
                        ptype[j] = FLOAT_TYPE;
                        pwidth[j] = sizeof(DOUBLE);
                        Var_List[j] = (VOIDPTR) & doubles[j];
                        }
                    }
                printheader (column_names, pFile, nCol);
                
                nl_set_current_row (nlist, (UINT)1, (UINT)0);
                for (j = 1; j <= ptab->nrows; j++)
                    {
                    
                    status = nl_get_points_convert (nlist, (UINT)1, nCol, Col_List, Var_List,
                                                    Units, pwidth, ptype, &return_p);
                    
					if (status < 0)
						{
						return status;
						}
					fprintf (pFile, "%3ld%5ld:", i, j);
                    for (k = 0; k < nCol; k++)
                        {
                        if (ptype[k] EQUALS CHAR_TYPE)
                            {
                            fprintf (pFile, (char *)string_fmt, strings[k]);
                            }
                        else
                            {
                            fprintf (pFile, (char *)double_fmt, doubles[k]);
                            }
                        }
                    fprintf (pFile, "\n");
                    }
                }
            }
        }
#if NEED_TO_FREE
    tc_free ((VOIDPTR)dlist);
    tc_free ((VOIDPTR)s);
    tc_free ((VOIDPTR)sv);
    tc_free ((VOIDPTR)ptype);
    tc_free ((VOIDPTR)tempData_Types);
    tc_free ((VOIDPTR)pwidth);
    tc_free ((VOIDPTR)ptempwidth);
    tc_free ((VOIDPTR)Var_List);
    tc_free ((VOIDPTR)strings);
    tc_free ((VOIDPTR)doubles);
    tc_free ((VOIDPTR)Units);
#endif
    ts_tcp_free (column_names);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static void printheader(TCP column_names, FILE *pFile, UINT nCol);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    column_names    -(TCP)
    pFile           -(FILE *)
    nCol            -(UINT)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static void printheader (TCP column_names, FILE *pFile, UINT nCol) 
#else
static void printheader (column_names, pFile, nCol)
TCP column_names;
FILE *pFile;
UINT nCol;
#endif
    {
    UINT i;
    static CHAR spaces[] = "\n         ";
    static CHAR dashes[] = " ----------";
    
    fprintf (pFile, (char *)spaces);
    for (i = 0; i < nCol; i++)
        {
        fprintf (pFile, " %10.10s", column_names[i]);
        }
    fprintf (pFile, (char *)spaces);
    for (i = 0; i < nCol; i++)
        {
        fprintf (pFile, (char *)dashes);
        }
    fprintf (pFile, "\n");
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static void printline(FILE *pFile, CHAR *s, UINT nCol, BYTE *array, 
        INT dtype, size_t dsize);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pFile           -(FILE *)
    s               -(CHAR *)
    nCol            -(UINT)
    array           -(BYTE *)
    dtype           -(INT)
    dsize           -(size_t)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static void printline (FILE *pFile, CHAR *s, UINT nCol, BYTE *array, INT dtype,
                           size_t dsize) 
#else
static void printline (pFile, s, nCol, array, dtype, dsize)
FILE *pFile;
CHAR *s;
UINT nCol;
BYTE *array;
INT dtype;
size_t dsize;
#endif
    {
    UINT i;
    static CHAR stats_hdr[] = "%-8s:";
    
    fprintf (pFile, (char *)stats_hdr, s);
    for (i = 0; i < nCol; i++)
        {
        switch (dtype)
            {
        case FLOAT_TYPE:
            switch (dsize)
                {
            case sizeof(FLOAT):
                fprintf (pFile, (char *)double_fmt, *(FLOAT *)(array + i * dsize));
                break;
            case sizeof(DOUBLE):
                fprintf (pFile, (char *)double_fmt, *(DOUBLE *)(array + i * dsize));
                break;
            default:
                break;
                }
            break;
        case INT_TYPE:
            switch (dsize)
                {
            case sizeof(INT):
                fprintf (pFile, (char *)int_fmt, *(INT *)(array + i * dsize));
                break;
            case sizeof(SHORT):
                fprintf (pFile, (char *)int_fmt, *(SHORT *)(array + i * dsize));
                break;
            default:
                break;
                }
            break;
        case CHAR_TYPE:
            fprintf (pFile, string_fmt, array + i * dsize);
            break;
        default:
            break;
            }
        }
    fprintf (pFile, "\n");
    }
/* END:     */
