/* DEC/CMS REPLACEMENT HISTORY, Element NL_GET_ROWSC.C*/
/* *5    16-APR-1991 10:10:10 GILLESPIE "(SPR 1) Fix up character conversions - avoid useless allocations"*/
/* *4    25-OCT-1990 12:22:46 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3     2-AUG-1990 17:14:05 GILLESPIE "(SPR 5735) Add COLUMN_CHECK"*/
/* *2    25-JUL-1990 14:59:29 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:59:29 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_GET_ROWSC.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#include "esi_nl_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Get an array of rows, starting from the current row. The returned
    rows, are converted to the data type and width specified in
    Data_Type and Data_Width;

Prototype:
    publicdef INT nl_get_rows_convert (NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT Col_List[], VOIDPTR returnArray[], INT Data_Units[], size_t Data_Width[], 
        INT Data_Type[], UINT *returnPts);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.       
    nrows           -(UINT) Allocated space in the returned array.  
    nCol            -(UINT)
    Col_List        -(UINT []) List of requested columns.      
    returnArray     -(VOIDPTR []) The returned data array.           
    Data_Units      -(INT []) Units of measure of returned data.     
    Data_Type       -(INT []) Types of returned data.          
    Data_Width      -(size_t []) Widths of returned data.        
    returnPts       -(UINT *) Totally returned rows.      

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    NL_OVERFLOW
    NL_INVALID_DATA_TYPE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_get_rows_convert (NLIST_HEADER nlist, UINT nrows, UINT nCol,
                                   UINT Col_List[], VOIDPTR returnArray[],
                                   INT Data_Units[], size_t Data_Width[],
                                   INT Data_Type[], UINT *returnPts)
#else
publicdef INT nl_get_rows_convert (nlist, nrows, nCol, Col_List, returnArray,
                                   Data_Units, Data_Width, Data_Type, returnPts)
NLIST_HEADER nlist;
UINT nrows;
UINT nCol;
UINT Col_List[];
VOIDPTR returnArray[];
INT Data_Units[];
INT Data_Type[];
size_t Data_Width[];
UINT *returnPts;
#endif
    {
    VOIDPTR *Var_List;
    BYTE *bytePtr;
    TBOOL *boolPtr;
    COLUMN_HEADER *pinfo;
    register UINT i, j;             /*    indexes.                    */
    size_t internal_size;           /*    size of column in n-List to be converted */
    /*-------------------------------------*/
    /* data type of column in n-List to be */
    /* converted                           */
    /*-------------------------------------*/
    INT internal_type;
    
    size_t external_size;           /*    size of n-List column being requested    */
    /*-------------------------------------*/
    /* data type of column in n-List being */
    /* requested                           */
    /*-------------------------------------*/
    INT external_type;
    
    INT status;
    UINT nreturned;
    INT *intPtr;                    /* pointer for INT value.                */
    size_t length;
    FLOAT *floatPtr;                /* pointer for FLOAT value.            */
    DOUBLE *doublePtr;              /* pointer for DOUBLE value.            */
    SHORT *shortPtr;                /* pointer for SHORT value.            */
    CHAR *charPtr;                  /* pointer for CHAR value.                */
    BYTE *buffer;                   /* workspace for converting CHAR_TYPE.        */
    CHAR *format;
    INT int_value;
    DOUBLE double_value;
    BYTE convertBuffer[64];         /*    the temp. buffer for CHAR_TYPE.            */
    
    /* ************************************************************************* */
    /*    error checking.                    */
    
    INIT_TEST (nlist);
    
    NROWS_TEST (nrows);
    
    EMPTY_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        }
    /*    create a Var_List for nl_get_rows().        */
    
    Var_List = (VOIDPTR *) TALLOC (sizeof(VOIDPTR) * nCol);
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        if (Data_Width[i] EQUALS pinfo->data_size AND Data_Type[i] EQUALS pinfo->
            data_type)
            {
            Var_List[i] = (VOIDPTR)returnArray[i];
            }
        else
            {
            Var_List[i] = (VOIDPTR) TALLOC (nrows * pinfo->data_size);
            }
        }
    status = nl_get_rows (nlist, nrows, nCol, Col_List, Var_List, returnPts);
    
    /*    check for each column, if the data type  */
    /*    and size are same as the specification,        */
    /*    moves rows to the returned array,        */
    /*    otherwise do the convertion.            */
    
    if (status < 0)
        {
        return status;
        }
    nreturned = *returnPts;
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        internal_size = pinfo->data_size;
        external_size = Data_Width[i];
        internal_type = pinfo->data_type;
        external_type = Data_Type[i];
        
        /*    if the data type and size are same, moves   */
        /*    the rows directly.                */
        
        if (external_type EQUALS internal_type AND external_size EQUALS internal_size)
            {
            continue;
            }
        else
            {
            charPtr = (CHAR *)returnArray[i];		/* Goes back to caller */
            bytePtr = (BYTE *)Var_List[i];			/* Local storage buffer */
            
            switch (external_type)
                {
                /* ----------------------------------- */
            case INT_TYPE:
                switch (external_size)
                    {
                    
                case sizeof(CHAR):  /*    if the data type is not CHAR_TYPE or    */
                    /*    INT_TYPE and sizeof(CHAR) returns error.    */
                    
                    if (
                    internal_type EQUALS INT_TYPE AND internal_size EQUALS 
                        sizeof(CHAR))
                        {
                        hoblockmove ((VOIDPTR)Var_List[i], (VOIDPTR)returnArray[i],
                                     nreturned * sizeof(CHAR));
                        }
                    else if (internal_type EQUALS CHAR_TYPE)
                        {
                        for (j = 0; j < nreturned;
                             j++, charPtr++, bytePtr += internal_size)
                            {
                            *charPtr = (CHAR)*bytePtr;
                            }
                        }
                    else
                        {
                        return NL_INVALID_DATA_TYPE;
                        }
                    break;
                case sizeof(INT):
                    for (j = 0, intPtr = (INT *)charPtr; j < nreturned;
                         j++, intPtr++, bytePtr += internal_size)
                        {
                        switch (internal_type)
                            {
                        case INT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(INT):
                                *intPtr = *(INT *)bytePtr;
                                break;
                            case sizeof(SHORT):
                                *intPtr = (INT)*(SHORT *)bytePtr;
                                break;
                            case sizeof(CHAR):
                                *intPtr = (INT)*(CHAR *)bytePtr;
                                break;
                                }
                            break;
                        case FLOAT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(FLOAT):
                                *intPtr = (INT)*(FLOAT *)bytePtr;
                                break;
                            case sizeof(DOUBLE):
                                *intPtr = (INT)*(DOUBLE *)bytePtr;
                                break;
                                }
                            break;
                        case BOOL_TYPE:
                            *intPtr = (INT)*(TBOOL *)bytePtr;
                            break;
                        case CHAR_TYPE:
                            *intPtr = (INT)atoi (bytePtr);
                            break;
                            }
                        }
                    break;
                case sizeof(SHORT):
                    for (j = 0, shortPtr = (SHORT *)charPtr; j < nreturned;
                         j++, shortPtr++, bytePtr += internal_size)
                        {
                        switch (internal_type)
                            {
                        case INT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(INT):
                                *shortPtr = (SHORT)*(INT *)bytePtr;
                                break;
                            case sizeof(SHORT):
                                *shortPtr = *(SHORT *)bytePtr;
                                break;
                            case sizeof(CHAR):
                                *shortPtr = (SHORT)*(CHAR *)bytePtr;
                                break;
                                }
                            break;
                        case FLOAT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(FLOAT):
                                *shortPtr = (SHORT)*(FLOAT *)bytePtr;
                                break;
                            case sizeof(DOUBLE):
                                *shortPtr = (SHORT)*(DOUBLE *)bytePtr;
                                break;
                                }
                            break;
                        case BOOL_TYPE:
                            *shortPtr = (SHORT)*(TBOOL *)bytePtr;
                            break;
                        case CHAR_TYPE:
                            *shortPtr = (SHORT)atoi (bytePtr);
                            break;
                            }
                        }
                    break;
                    }
                break;
                
                /*-------------------------------------*/
                
            case FLOAT_TYPE:
                switch (external_size)
                    {
                case sizeof(FLOAT):
                    for (j = 0, floatPtr = (FLOAT *)charPtr; j < nreturned;
                         j++, floatPtr++, bytePtr += internal_size)
                        {
                        switch (internal_type)
                            {
                        case INT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(INT):
                                *floatPtr = (FLOAT)*(INT *)bytePtr;
                                break;
                            case sizeof(SHORT):
                                *floatPtr = (FLOAT)*(SHORT *)bytePtr;
                                break;
                            case sizeof(CHAR):
                                *floatPtr = (FLOAT)*(CHAR *)bytePtr;
                                break;
                                }
                            break;
                        case FLOAT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(FLOAT):
                                *floatPtr = *(FLOAT *)bytePtr;
                                break;
                            case sizeof(DOUBLE):
                                *floatPtr = (FLOAT)*(DOUBLE *)bytePtr;
                                break;
                                }
                            break;
                        case BOOL_TYPE:
                            *floatPtr = (FLOAT)*(TBOOL *)bytePtr;
                            break;
                        case CHAR_TYPE:
                            *floatPtr = (FLOAT)atof (bytePtr);
                            break;
                            }
                        }
                    break;
                case sizeof(DOUBLE):
                    for (j = 0, doublePtr = (DOUBLE *)charPtr; j < nreturned;
                         j++, doublePtr++, bytePtr += internal_size)
                        {
                        switch (internal_type)
                            {
                        case INT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(INT):
                                *doublePtr = (DOUBLE)*(INT *)bytePtr;
                                break;
                            case sizeof(SHORT):
                                *doublePtr = (DOUBLE)*(SHORT *)bytePtr;
                                break;
                            case sizeof(CHAR):
                                *doublePtr = (DOUBLE)*(CHAR *)bytePtr;
                                break;
                                }
                            break;
                        case FLOAT_TYPE:
                            switch (internal_size)
                                {
                            case sizeof(FLOAT):
                                *doublePtr = (DOUBLE)*(FLOAT *)bytePtr;
                                break;
                            case sizeof(DOUBLE):
                                *doublePtr = *(DOUBLE *)bytePtr;
                                break;
                                }
                            break;
                        case BOOL_TYPE:
                            *doublePtr = (DOUBLE)*(TBOOL *)bytePtr;
                            break;
                        case CHAR_TYPE:
                            *doublePtr = (DOUBLE)atof (bytePtr);
                            }
                        }
                    break;
                    }
                break;
                
                /*-------------------------------------- */
                
            case BOOL_TYPE:
                for (j = 0, boolPtr = (TBOOL *)charPtr; j < nreturned;
                     j++, boolPtr++, bytePtr += internal_size)
                    {
                    switch (internal_type)
                        {
                    case INT_TYPE:
                        switch (internal_size)
                            {
                        case sizeof(INT):
                            *boolPtr = (TBOOL)*(INT *)bytePtr;
                            break;
                        case sizeof(SHORT):
                            *boolPtr = (TBOOL)*(SHORT *)bytePtr;
                            break;
                        case sizeof(CHAR):
                            *boolPtr = (TBOOL)*(CHAR *)bytePtr;
                            break;
                            }
                        break;
                    case FLOAT_TYPE:
                        switch (internal_size)
                            {
                        case sizeof(FLOAT):
                            *boolPtr = (TBOOL)*(FLOAT *)bytePtr;
                            break;
                        case sizeof(DOUBLE):
                            *boolPtr = (TBOOL)*(DOUBLE *)bytePtr;
                            break;
                            }
                        break;
                    case BOOL_TYPE:
                        *boolPtr = *(TBOOL *)bytePtr;
                        break;
                    case CHAR_TYPE:
                        *boolPtr = (TBOOL)atoi (bytePtr);
                        break;
                        }
                    }
                break;
                
                /*---------------------------------- */
                
            case CHAR_TYPE:
                format = pinfo->convert_format;
                for (j = 0; j < nreturned;
                     j++, charPtr += external_size, bytePtr += internal_size)
                    {
                    switch (internal_type)
                        {
                    case BOOL_TYPE:
                    case INT_TYPE:
                        switch (internal_size)
                            {
                        case sizeof(INT):
                            int_value = *(INT *)bytePtr;
                            break;
                        case sizeof(SHORT):
                            int_value = (INT)*(SHORT *)bytePtr;
                            break;
                        case sizeof(CHAR):
                            int_value = (INT)*(CHAR *)bytePtr;
                            break;
                            }
                        length = sprintf ((char *)convertBuffer, (char *)format,
                                          int_value);
                        if (external_size < length)
                            {
                            status = NL_OVERFLOW;
                            continue;
                            }
                        else
                            {
                            strcpy ((char *)charPtr, (char *)convertBuffer);
                            }
                        break;
                    case FLOAT_TYPE:
                        switch (internal_size)
                            {
                        case sizeof(FLOAT):
                            double_value = (DOUBLE)*(FLOAT *)bytePtr;
                            break;
                        case sizeof(DOUBLE):
                            double_value = *(DOUBLE *)bytePtr;
                            break;
                            }
                        length = sprintf ((char *)convertBuffer, (char *)format,
                                          double_value);
                        if (external_size < length)
                            {
                            status = NL_OVERFLOW;
                            continue;
                            }
                        else
                            {
                            strcpy ((char *)charPtr, (char *)convertBuffer);
                            }
                        break;
                        
                        /* **********************************************/
                        /* If the internal data type is CHAR_TYPE, too, */
                        /* then returns the minimum of two buffer sizes.*/
                        /* ******************************************** */
                        
                    case CHAR_TYPE:
                        if (external_size < pinfo->data_size)
                            {
                            length = external_size;
                            status = NL_CHAR_TYPE_CONVERT_FAIL;
                            }
                        else
                            {
                            length = internal_size;
                            }
                        hoblockmove ((VOIDPTR)bytePtr, (VOIDPTR)charPtr,
                                     length);
                        break;
                        }
                    }
                break;
                }
            }
        }
    /*free the memory space. */

#if NEED_TO_FREE
    for (i = 0; i < nCol; i++)
        {
        if (Var_List[i] != (VOIDPTR) NULL AND Var_List[i] != returnArray[i])
            {
            tc_free ((VOIDPTR)Var_List[i]);
            }
        }
    tc_free ((VOIDPTR)Var_List);
#endif    
    return status;
    }
/* END:     */
