/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_AID_RCVT.C */
/* *6    25-OCT-1990 12:17:43 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *5    25-JUL-1990 14:51:03 VINCE "(SPR 5644) Header Standardization" */
/* *4     9-APR-1990 10:31:26 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *3    24-MAR-1990 23:34:15 GILLESPIE "(SPR 1) Add lots more memory checks" */
/* *2     5-MAR-1990 10:44:32 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:33:32 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_AID_RCVT.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_AID_PCVT.C*/
/* *2    29-JUL-1989 23:59:28 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:16:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_AID_PCVT.C*/
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
#include "esi_c_lib.h"
#include "esi_tc.h"
#include "esi_nl_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Add or insert or delete one or more data rows to the current table in an n-List
    and perform any required data type conversion.  If the ADD_OR_INSERT is equals
    to 0 then add row(s), otherwise insert row(s).

Prototype:
    publicdef INT nlz_aid_rows_convert(NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List, INT *Data_Units, size_t *Data_Widths, 
        INT *Data_Types, INT add_or_insert);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist           -(NLIST_HEADER) N-List header.
    (I) nrows           -(UINT) Number of rows to be added.
    (I) nCol            -(UINT) Number of columns referenced.
    (I) Col_List;       -(UINT *) Array of column <nCol> index numbers.
    (I) Var_List        -(VOIDPTR *) Array of addresses of start of data for each
                            column.
    (I) Data_Units      -(INT *) Save for later use.                              
    (I) Data_Widths     -(size_t *) Array of data widths represented by input data
                            one per input column.
    (I) Data_Types      -(INT *) Array of data types represented by input data
                            (FLOAT_TYPE, INT_TYPE, CHAR_TYPE, BOOL_TYPE)
                            one per input column.
    (I) add_or_insert   -(INT) Flag to indicate to add or to insert row(s).
                            = 0 --- to add row(s).
                            = 1 --- to insert row(s).
                            = 2 --- to repalce row(s). 
    
Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_DATA_TYPE
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    1) The n-List has been initialized.
    2) A table has been initialized.
    3) The data rows will be added after the 'Current Point'.
    4) 1 <= nCol <= total # of columns in the n-List.
    5) Var_List coontains valid addresses of source data.
    6) All entries in Col_List satisfy 1 <= Col_List[i] <= total # columns.
    7) Nrows > 0.
    8) There is a correspondence between the ith position in each of the
        input arrays.  That is, the first value in each array describes
        the first column to be input, the second value in each array
        describes the second value, etc.
    9) Any column defined for the n-List but not referenced in Col_List
        that is not an indexed column will have a null value inserted.
   10) The input data will be converted, if necessary, from the current
        representation defined by Data_Widths[i] and Data_Types[i] into
        its internal representation as defined in the n-List header.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_aid_rows_convert (NLIST_HEADER nlist, UINT nrows, UINT nCol,
                                    UINT *Col_List, VOIDPTR *Var_List, INT *Data_Units,
                                    size_t *Data_Widths, INT *Data_Types,
                                    INT add_or_insert) 
#else
publicdef INT nlz_aid_rows_convert (nlist, nrows, nCol, Col_List, Var_List, Data_Units,
                                    Data_Widths, Data_Types, add_or_insert)
NLIST_HEADER nlist;
UINT nrows;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
INT *Data_Units;
size_t *Data_Widths;
INT *Data_Types;
INT add_or_insert;
#endif
    {
    COLUMN_HEADER *pinfo;
    UINT i, j;
    INT status = SUCCESS;
    VOIDPTR *paddr;
    /*  ************************************************************************** */
    
    /* error checking.      */
    INIT_TEST (nlist);
    
    /* make sure a valid table exists */
    TABLE_TEST (nlist, 1);
    
    /* it is invalid to add less than 0 row */
    NROWS_TEST (nrows);
    
    /* allocate the space for the actual addresses
       to be moved. */
    
    paddr = (VOIDPTR *)TALLOC (nCol * sizeof(VOIDPTR));
    if (paddr EQUALS (VOIDPTR *)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < nCol; i++)
        {
        /* check each data pointer and make
           sure it is valid (non-zero) */
        if (Var_List[i] EQUALS (VOIDPTR)0)
            {
            return NL_INVALID_POINTER;
            }
        /* compare the input data type and width
           with the stored data type and width
           perform a type conversion if necessary */
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        if ((pinfo->data_type IS_NOT_EQUAL_TO Data_Types[i]) OR (pinfo->
                                                             data_size IS_NOT_EQUAL_TO 
                                                                Data_Widths[i]))
            {
            switch (Data_Types[i])
                {
                /*---------------------------------------------------------------*/
                /* external data type is FLOAT */
            case FLOAT_TYPE:
                /* switch the external data type to
                   DOUBLE (if it is not already)
                   ptop will row to the top of the
                   list of doubles to be cast to an integer*/
                    {
                    DOUBLE *pdouble;
                    DOUBLE *ptop;
                    FLOAT *pfloat;
                    
                    if (Data_Widths[i] IS_NOT_EQUAL_TO sizeof(DOUBLE))
                        {
                        ptop = (DOUBLE *)TALLOC (nrows * sizeof(DOUBLE));
                        if (ptop EQUALS (DOUBLE *)0)
                            {
                            return NL_OVERFLOW;
                            }
                        for (pdouble = ptop, j = 0, pfloat = (FLOAT *)Var_List[i];
                             j < nrows; j++, pdouble++, pfloat++)
                            {
                            *pdouble = (DOUBLE)*pfloat;
                            }
                        }
                    else
                        {
                        ptop = (DOUBLE *)Var_List[i];
                        }
                    /* switch based on the internal data type */
                    switch (pinfo->data_type)
                        {
                        /* internal data type is FLOAT */
                    case FLOAT_TYPE:
                        if (pinfo->data_size EQUALS sizeof(FLOAT))
                            {
                            /* external rep is DOUBLE,
                               internal is FLOAT */
                            FLOAT *ptr;
                            DOUBLE *pdata;
                            
                            ptr = (FLOAT *)TALLOC (nrows * sizeof(FLOAT));
                            if (ptr EQUALS (FLOAT *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (pdata = (DOUBLE *)Var_List[i], j = 0; j < nrows;
                                 j++, ptr++, pdata++)
                                {
                                *ptr = (FLOAT)*pdata;
                                }
                            }
                        else
                            {
                            /* external rep is FLOAT,
                               internal is DOUBLE */
                            DOUBLE *ptr;
                            FLOAT *pdata;
                            
                            ptr = (DOUBLE *)TALLOC (nrows * sizeof(DOUBLE));
                            if (ptr EQUALS (DOUBLE *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (pdata = (FLOAT *)Var_List[i], j = 0; j < nrows;
                                 j++, ptr++, pdata++)
                                {
                                *ptr = (DOUBLE)*pdata;
                                }
                            }
                        break;
                        /* internal data type is INTEGER */
                    case INT_TYPE:
                        switch (pinfo->data_size)
                            {
                        case sizeof(BYTE):
                            /* internal data type is BYTE
                               external is 'DOUBLE' */
                                {
                                BYTE *ptr;
                                
                                ptr = (BYTE *)TALLOC (nrows * sizeof(BYTE));
                                if (ptr EQUALS (BYTE *)0)
                                    {
                                    return NL_OVERFLOW;
                                    }
                                *(paddr + i) = (VOIDPTR)ptr;
                                for (j = 0, pdouble = ptop; j < nrows;
                                     j++, pdouble++, ptr++)
                                    {
                                    *ptr = (BYTE)*pdouble;
                                    }
                                }
                            break;
                        case sizeof(SHORT):
                            /* internal data type is BYTE
                               external is 'DOUBLE' */
                                {
                                SHORT *ptr;
                                
                                ptr = (SHORT *)TALLOC (nrows * sizeof(SHORT));
                                if (ptr EQUALS (SHORT *)0)
                                    {
                                    return NL_OVERFLOW;
                                    }
                                *(paddr + i) = (VOIDPTR)ptr;
                                for (j = 0, pdouble = ptop; j < nrows;
                                     j++, pdouble++, ptr++)
                                    {
                                    *ptr = (SHORT)*pdouble;
                                    }
                                }
                            break;
                        case sizeof(INT):
                            /* internal data type is INT
                               external is 'DOUBLE' */
                                {
                                INT *ptr;
                                
                                ptr = (INT *)TALLOC (nrows * sizeof(INT));
                                if (ptr EQUALS (INT *)0)
                                    {
                                    return NL_OVERFLOW;
                                    }
                                *(paddr + i) = (VOIDPTR)ptr;
                                for (j = 0, pdouble = ptop; j < nrows;
                                     j++, pdouble++, ptr++)
                                    {
                                    *ptr = (INT)*pdouble;
                                    }
                                }
                            break;
                        default:
                            return NL_INVALID_DATA_SIZE;
                            }
                        break;
                    case BOOL_TYPE:
                            {
                            /* The internal data type is boolean (TBOOL is */
                            /* 1 byte)                     */
                            TBOOL *ptr;
                            
                            ptr = (TBOOL *)TALLOC (nrows * sizeof(TBOOL));
                            if (ptr EQUALS (TBOOL *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (j = 0, pdouble = ptop; j < nrows;
                                 j++, pdouble++, ptr++)
                                {
                                *ptr = (*pdouble != 0.0) ? 1 : 0;
                                }
                            }
                        break;
                    case CHAR_TYPE:
                            {
                            /* The internal data is CHAR_TYPE. */
                            CHAR *ptr;
                            DOUBLE double_value;
                            
                            ptr = (CHAR *)TALLOC (nrows * pinfo->data_size);
                            if (ptr EQUALS (CHAR *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (j = 0, pdouble = ptop; j < nrows;
                                                                                          j++
                                                                                          ,
                                                                                           
                                                                                          pdouble++
                                                                                          ,
                                                                                           
                                                                                          ptr
                                                                                           
                                                                                          +=
                                                                                           pinfo
                                                                                          ->
                                                                                          data_size
                                                                                          )
                                                                                          
                                {
                                double_value = *(DOUBLE *)pdouble;
                                sprintf ((char *)ptr, "%f", double_value);
                                }
                            }
                        break;
                    default:
                        break;
                        }
#if NEED_TO_FREE 
                    /* free up any temporary space allocated */
                    
                    if (ptop != (DOUBLE *)Var_List[i])
                        {
                        tc_free ((VOIDPTR)ptop);
                        }
#endif
                    }
                break;
                /*---------------------------------------------------------------*/
                /* external data type is INTEGER */
            case INT_TYPE:
                /* switch the external data type to
                   INT (if it is not already)
                   ptop will row to the top of the
                   list of integers */
                    {
                    INT *pint;
                    INT *ptop;
                    
                    if (Data_Widths[i] EQUALS sizeof(BYTE))
                        {
                        BYTE *ptr;
                        
                        ptop = (INT *)TALLOC (nrows * sizeof(INT));
                        if (ptop EQUALS (INT *)0)
                            {
                            return NL_OVERFLOW;
                            }
                        for (pint = ptop, j = 0, ptr = (BYTE *)Var_List[i]; j < nrows;
                             j++, pint++, ptr++)
                            {
                            *pint = (INT)*ptr;
                            }
                        }
                    else if (Data_Widths[i] EQUALS sizeof(SHORT))
                        {
                        SHORT *ptr;
                        
                        ptop = (INT *)TALLOC (nrows * sizeof(INT));
                        if (ptop EQUALS (INT *)0)
                            {
                            return NL_OVERFLOW;
                            }
                        for (pint = ptop, j = 0, ptr = (SHORT *)Var_List[i]; j < nrows;
                             j++, pint++)
                            {
                            *pint = (INT)*ptr;
                            }
                        }
                    else
                        {
                        ptop = (INT *)Var_List[i];
                        }
                    /* switch based on the internal data type */
                    switch (pinfo->data_type)
                        {
                        /* internal data type is FLOAT */
                    case FLOAT_TYPE:
                        if (pinfo->data_size EQUALS sizeof(DOUBLE))
                            {
                            /* external rep is 'INT',
                               internal is DOUBLE */
                            DOUBLE *ptr;
                            
                            ptr = (DOUBLE *)TALLOC (nrows * sizeof(DOUBLE));
                            if (ptr EQUALS (DOUBLE *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (pint = ptop, j = 0; j < nrows; j++, ptr++, pint++)
                                {
                                *ptr = (DOUBLE)*pint;
                                }
                            }
                        else
                            {
                            /* external rep is 'INT',
                               internal is FLOAT */
                            FLOAT *ptr;
                            
                            ptr = (FLOAT *)TALLOC (nrows * sizeof(FLOAT));
                            if (ptr EQUALS (FLOAT *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (pint = ptop, j = 0; j < nrows; j++, ptr++, pint++)
                                {
                                *ptr = (FLOAT)*pint;
                                }
                            }
                        break;
                        /* internal data type is INTEGER */
                    case INT_TYPE:
                        switch (pinfo->data_size)
                            {
                        case sizeof(BYTE):
                            /* internal data type is BYTE
                               external is 'DOUBLE' */
                                {
                                BYTE *ptr;
                                
                                ptr = (BYTE *)TALLOC (nrows * sizeof(BYTE));
                                if (ptr EQUALS (BYTE *)0)
                                    {
                                    return NL_OVERFLOW;
                                    }
                                *(paddr + i) = (VOIDPTR)ptr;
                                for (j = 0, pint = ptop; j < nrows; j++, pint++, ptr++)
                                    {
                                    *ptr = (BYTE)*pint;
                                    }
                                }
                            break;
                        case sizeof(SHORT):
                            /* internal data type is SHORT
                               external is 'INT' */
                                {
                                SHORT *ptr;
                                
                                ptr = (SHORT *)TALLOC (nrows * sizeof(SHORT));
                                if (ptr EQUALS (SHORT *)0)
                                    {
                                    return NL_OVERFLOW;
                                    }
                                *(paddr + i) = (VOIDPTR)ptr;
                                for (j = 0, pint = ptop; j < nrows; j++, pint++, ptr++)
                                    {
                                    *ptr = (SHORT)*pint;
                                    }
                                }
                            break;
                        case sizeof(INT):
                            /* internal data type is INT
                               external is 'INT' */
                            *(paddr + i) = (VOIDPTR)ptop;
                            break;
                        default:
                            return NL_INVALID_DATA_SIZE;
                            }
                        break;
                    case BOOL_TYPE:
                            {
                            /* The internal data type is */
                            /* boolean.          */
                            TBOOL *ptr;
                            
                            ptr = (TBOOL *)TALLOC (nrows * sizeof(TBOOL));
                            if (ptr EQUALS (TBOOL *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (j = 0, pint = ptop; j < nrows; j++, pint++, ptr++)
                                {
                                *ptr = (*pint != 0) ? 1 : 0;
                                }
                            }
                        break;
                    case CHAR_TYPE:
                            {
                            /* The internal data is CHAR_TYPE. */
                            CHAR *ptr;
                            INT int_value;
                            
                            ptr = (CHAR *)TALLOC (nrows * pinfo->data_size);
                            if (ptr EQUALS (CHAR *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (j = 0, pint = ptop; j < nrows;
                                                                                    j++,
                                                                                     pint++
                                                                                    , 
                                                                                    ptr 
                                                                                    += 
                                                                                    pinfo
                                                                                    ->data_size
                                                                                    )
                                {
                                int_value = *(INT *)pint;
                                sprintf ((char *)ptr, "%d", int_value);
                                }
                            }
                        break;
                    default:
                        return NL_INVALID_DATA_TYPE;
                        }
                    /* free up any temporary space allocated */
                    
                    if (ptop IS_NOT_EQUAL_TO (INT *)Var_List[i])
                        {
                        tc_free ((VOIDPTR)ptop);
                        }
                    }
                break;
                /*---------------------------------------------------------------*/
                /* external data type is BOOL */
            case BOOL_TYPE:
                    {
                    TBOOL *ptop;
                    TBOOL *pbool;
                    ptop = (TBOOL *)Var_List[i];
                    switch (pinfo->data_type)
                        {
                    case FLOAT_TYPE:
                            {
                            switch (pinfo->data_size)
                                {
                            case sizeof(DOUBLE):
                                /* The internal data type is DOUBLE */
                                    {
                                    DOUBLE *ptr;
                                    
                                    ptr = (DOUBLE *)TALLOC (nrows * sizeof(DOUBLE));
                                    if (ptr EQUALS (DOUBLE *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0, pbool = ptop; j < nrows;
                                         j++, pbool++, ptr++)
                                        {
                                        *ptr = (DOUBLE)*pbool;
                                        }
                                    }
                                break;
                            case sizeof(FLOAT):
                                /* The internal data type is FLOAT*/
                                    {
                                    FLOAT *ptr;
                                    
                                    ptr = (FLOAT *)TALLOC (nrows * sizeof(FLOAT));
                                    if (ptr EQUALS (FLOAT *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0, pbool = ptop; j < nrows;
                                         j++, pbool++, ptr++)
                                        {
                                        *ptr = (FLOAT)*pbool;
                                        }
                                    }
                                break;
                                }
                            }
                        break;
                        
                    case INT_TYPE:
                            {
                            switch (pinfo->data_size)
                                {
                            case sizeof(INT):
                                /* The internal data type is INT */
                                    {
                                    INT *ptr;
                                    
                                    ptr = (INT *)TALLOC (nrows * sizeof(INT));
                                    if (ptr EQUALS (INT *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0, pbool = ptop; j < nrows;
                                         j++, pbool++, ptr++)
                                        {
                                        *ptr = (INT)*pbool;
                                        }
                                    }
                                break;
                            case sizeof(SHORT):
                                /* The internal data type is SHORT*/
                                    {
                                    SHORT *ptr;
                                    
                                    ptr = (SHORT *)TALLOC (nrows * sizeof(SHORT));
                                    if (ptr EQUALS (SHORT *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0, pbool = ptop; j < nrows;
                                         j++, pbool++, ptr++)
                                        {
                                        *ptr = (SHORT)*pbool;
                                        }
                                    }
                                break;
                            case sizeof(BYTE):
                                /* The internal data type is BYTE */
                                    {
                                    BYTE *ptr;
                                    
                                    ptr = (BYTE *)TALLOC (nrows * sizeof(BYTE));
                                    if (ptr EQUALS (BYTE *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0, pbool = ptop; j < nrows;
                                         j++, pbool++, ptr++)
                                        {
                                        *ptr = (BYTE)*pbool;
                                        }
                                    }
                                break;
                                }
                            }
                        break;
                    case BOOL_TYPE:
                        *(paddr + i) = (VOIDPTR)ptop;
                        break;
                    case CHAR_TYPE:
                            {
                            /* The internal data is CHAR_TYPE. */
                            CHAR *ptr;
                            TBOOL bool_value;
                            
                            ptr = (CHAR *)TALLOC (nrows * pinfo->data_size);
                            if (ptr EQUALS (CHAR *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (j = 0, pbool = ptop; j < nrows;
                                                                                      j++
                                                                                      , 
                                                                                      pbool++
                                                                                      , 
                                                                                      ptr
                                                                                       +=
                                                                                       pinfo
                                                                                      ->
                                                                                      data_size
                                                                                      )
                                {
                                bool_value = *(TBOOL *)pbool;
                                sprintf ((char *)ptr, "%d", bool_value);
                                }
                            }
                        break;
                    default:
                        return NL_INVALID_DATA_TYPE;
                        }
                    break;
                    }
                /*---------------------------------------------------------------*/
                /* external data type is CHARACTER */
            case CHAR_TYPE:
                    {
                    CHAR *pchar_string;
                    
                    pchar_string = (CHAR *)Var_List[i];
                    switch (pinfo->data_type)
                        {
                    case FLOAT_TYPE:
                            {
                            switch (Data_Widths[i])
                                {
                            case sizeof(DOUBLE):
                                    {
                                    DOUBLE *ptr;
                                    
                                    ptr = (DOUBLE *)TALLOC (nrows * sizeof(DOUBLE));
                                    if (ptr EQUALS (DOUBLE *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0; j < nrows;
                                                                                       j++
                                                                                       ,
                                                                                        
                                                                                       ptr++
                                                                                       ,
                                                                                        
                                                                                       pchar_string
                                                                                        
                                                                                       +=
                                                                                        
                                                                                       Data_Widths
                                                                                       [
                                                                                        i
                                                                                        ]
                                                                                       )
                                                                                       
                                        {
                                        *ptr = (DOUBLE)atof ((char *)pchar_string);
                                        }
                                    }
                                break;
                            case sizeof(FLOAT):
                                    {
                                    FLOAT *ptr;
                                    
                                    ptr = (FLOAT *)TALLOC (nrows * sizeof(FLOAT));
                                    if (ptr EQUALS (FLOAT *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0; j < nrows;
                                                                                       j++
                                                                                       ,
                                                                                        
                                                                                       ptr++
                                                                                       ,
                                                                                        
                                                                                       pchar_string
                                                                                        
                                                                                       +=
                                                                                        
                                                                                       Data_Widths
                                                                                       [
                                                                                        i
                                                                                        ]
                                                                                       )
                                                                                       
                                        {
                                        *ptr = atof ((char *)pchar_string);
                                        }
                                    }
                                break;
                                }
                            }
                        break;
                    case INT_TYPE:
                            {
                            switch (Data_Widths[i])
                                {
                            case sizeof(INT):
                                    {
                                    INT *ptr;
                                    
                                    ptr = (INT *)TALLOC (nrows * sizeof(INT));
                                    if (ptr EQUALS (INT *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0; j < nrows;
                                                                                       j++
                                                                                       ,
                                                                                        
                                                                                       ptr++
                                                                                       ,
                                                                                        
                                                                                       pchar_string
                                                                                        
                                                                                       +=
                                                                                        
                                                                                       Data_Widths
                                                                                       [
                                                                                        i
                                                                                        ]
                                                                                       )
                                                                                       
                                        {
                                        *ptr = atoi ((char *)pchar_string);
                                        }
                                    }
                                break;
                            case sizeof(SHORT):
                                    {
                                    SHORT *ptr;
                                    
                                    ptr = (SHORT *)TALLOC (nrows * sizeof(SHORT));
                                    if (ptr EQUALS (SHORT *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0; j < nrows;
                                                                                       j++
                                                                                       ,
                                                                                        
                                                                                       ptr++
                                                                                       ,
                                                                                        
                                                                                       pchar_string
                                                                                        
                                                                                       +=
                                                                                        
                                                                                       Data_Widths
                                                                                       [
                                                                                        i
                                                                                        ]
                                                                                       )
                                                                                       
                                        {
                                        *ptr = atoi ((char *)pchar_string);
                                        }
                                    }
                                break;
                            case sizeof(BYTE):
                                    {
                                    BYTE *ptr;
                                    
                                    ptr = (BYTE *)TALLOC (nrows * sizeof(BYTE));
                                    if (ptr EQUALS (BYTE *)0)
                                        {
                                        return NL_OVERFLOW;
                                        }
                                    *(paddr + i) = (VOIDPTR)ptr;
                                    for (j = 0; j < nrows;
                                                                                       j++
                                                                                       ,
                                                                                        
                                                                                       ptr++
                                                                                       ,
                                                                                        
                                                                                       pchar_string
                                                                                        
                                                                                       +=
                                                                                        
                                                                                       Data_Widths
                                                                                       [
                                                                                        i
                                                                                        ]
                                                                                       )
                                                                                       
                                        {
                                        *ptr = atoi ((char *)pchar_string);
                                        }
                                    }
                                break;
                                }
                            break;
                            }
                    case BOOL_TYPE:
                            {
                            TBOOL *ptr;
                            TBOOL bool_value;
                            
                            ptr = (TBOOL *)TALLOC (nrows * sizeof(TBOOL));
                            if (ptr EQUALS (TBOOL *)0)
                                {
                                return NL_OVERFLOW;
                                }
                            *(paddr + i) = (VOIDPTR)ptr;
                            for (j = 0; j < nrows;
                                                                        j++,
                                                                        pchar_string +=
                                                                        Data_Widths[i],
                                                                        ptr++)
                                {
                                bool_value = FALSE;
                                sscanf ((char *)pchar_string, "%c", &bool_value);
                                *ptr = (bool_value EQUALS 0) ? FALSE : TRUE;
                                }
                            }
                        break;
                    case CHAR_TYPE:
                        if (Data_Widths[i] > pinfo->data_size)
                            {
                            return NL_CHAR_TYPE_CONVERT_FAIL;
                            }
                        *(paddr + i) = (VOIDPTR)Var_List[i];
                        break;
                        
                    default:
                        return NL_INVALID_DATA_TYPE;
                        }
                    break;
                    }
                /*---------------------------------------------------------------*/
            default:
                return NL_INVALID_DATA_TYPE;
                }
            }
        else
            {
            /* data types and widths match, so just
               include the pointer */
            *(paddr + i) = (VOIDPTR)Var_List[i];
            }
        }
    /* do the actual add of the converted data */
    switch (add_or_insert)
        {
    case 0:
        /* add_or_insert = 0, add row(s).        */
        status = nl_add_rows (nlist, nrows, nCol, Col_List, paddr);
        break;
        
    case 1:
        /* add_or_insert = 1, insert row(s).     */
        status = nl_insert_rows (nlist, nrows, nCol, Col_List, paddr);
        break;
        
    case 2:
        status = nl_replace_rows (nlist, nrows, nCol, Col_List, paddr);
        break;
        
    default:
        break;
        }
#if NEED_TO_FREE EQUALS TRUE
    /* free up any locally allocated areas */
    for (i = 0; i < nCol; i++)
        {
        if (*(paddr + i) != (VOIDPTR)Var_List[i])
            {
            tc_free ((VOIDPTR)*(paddr + i));
            }
        }
    tc_free ((VOIDPTR)paddr);
#endif
    return status;
    }
/* END:     */
