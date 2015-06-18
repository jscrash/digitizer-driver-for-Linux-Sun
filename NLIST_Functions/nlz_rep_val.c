/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_REP_VAL.C*/
/* *3    25-OCT-1990 12:19:57 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:54:29 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:56:59 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_REP_VAL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_REPVAL.C*/
/* *2    24-SEP-1989 22:27:29 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:17:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_REPVAL.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Returns the value of the current row as a double.

Prototype:
    publicdef INT nlz_replace_value (DATA_HEADER *pdata, COLUMN_HEADER *pinfo, 
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
publicdef INT nlz_replace_value (DATA_HEADER *pdata, COLUMN_HEADER *pinfo,
                                 UINT current_row, DOUBLE *px)
#else
publicdef INT nlz_replace_value (pdata, pinfo, current_row, px)
DATA_HEADER *pdata;
COLUMN_HEADER *pinfo;
UINT current_row;
DOUBLE *px;
#endif
    {
    BYTE *data_ptr = pdata->data + (current_row - 1) * pinfo->data_size;
    INT status = SUCCESS;
    
    if (pdata->inc_value)
        {
        return status;
        }
    switch (pinfo->data_type)
        {
    case INT_TYPE:
        switch (pinfo->data_size)
            {
        case sizeof(CHAR):
            *data_ptr = (BYTE)*px;
            break;
        case sizeof(SHORT):
            *(SHORT *)data_ptr = (SHORT)*px;
            break;
        case sizeof(INT):
            *(INT *)data_ptr = (INT)*px;
            break;
            }
        break;
    case FLOAT_TYPE:
        switch (pinfo->data_size)
            {
        case sizeof(FLOAT):
            *(FLOAT *)data_ptr = (FLOAT)*px;
            break;
        case sizeof(DOUBLE):
            *(DOUBLE *)data_ptr = *px;
            break;
            }
        break;
    default:
        status = NL_INVALID_DATA_TYPE;
        ;
        }
    return status;
    }
/* END:     */
