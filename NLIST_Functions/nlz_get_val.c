/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_VAL.C*/
/* *5    25-OCT-1990 12:18:41 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:52:58 VINCE "(SPR 5644) Header Standardization"*/
/* *3    24-MAR-1990 11:21:11 GILLESPIE "(SPR 1) move assignment of data_ptr after increment test"*/
/* *2    12-MAR-1990 19:04:15 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:56:05 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_VAL.C*/
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

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_get_value (DATA_HEADER *pdata, COLUMN_HEADER *pinfo, 
        UINT current_row, DOUBLE *px);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pdata           -(DATA_HEADER *)
    pinfo           -(COLUMN_HEADER *)
    current_row     -(UINT)
    px              -(DOUBLE *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_DATA_TYPE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_value (DATA_HEADER *pdata, COLUMN_HEADER *pinfo,
                             UINT current_row, DOUBLE *px) 
#else
publicdef INT nlz_get_value (pdata, pinfo, current_row, px)
DATA_HEADER *pdata;
COLUMN_HEADER *pinfo;
UINT current_row;
DOUBLE *px;
#endif
    {
    register BYTE *data_ptr;
    
    if (pdata->inc_value)
        {
        *px = pdata->start_value + (current_row - 1) * pdata->increment;
        return SUCCESS;
        }
    data_ptr = pdata->data + (current_row - 1) * pinfo->data_size;
    switch (pinfo->data_type)
        {
    case INT_TYPE:
        switch (pinfo->data_size)
            {
        case sizeof(CHAR):
            *px = (DOUBLE)*data_ptr;
            break;
        case sizeof(SHORT):
            *px = (DOUBLE)*(SHORT *)data_ptr;
            break;
        case sizeof(INT):
            *px = (DOUBLE)*(INT *)data_ptr;
            break;
            }
        break;
    case FLOAT_TYPE:
        switch (pinfo->data_size)
            {
        case sizeof(FLOAT):
            *px = (DOUBLE)*(FLOAT *)data_ptr;
            break;
        case sizeof(DOUBLE):
            *px = *(DOUBLE *)data_ptr;
            break;
            }
        break;
    default:
        return NL_INVALID_DATA_TYPE;
        }
    return SUCCESS;
    }
/* END:     */
