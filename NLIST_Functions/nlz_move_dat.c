/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_MOVE_DAT.C*/
/* *3    25-OCT-1990 12:19:13 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:53:28 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:56:25 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_MOVE_DAT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_MOVE_DATA.C*/
/* *3    18-SEP-1989 17:46:01 GILLESPIE "(SPR 100) Gulf mods"*/
/* *2    11-JUL-1989 12:11:16 GILLESPIE "(SPR 33) Fix some casting problems (?) - shoulda worked as it was..."*/
/* *1    19-JUN-1989 13:17:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_MOVE_DATA.C*/
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
#include "esi_limits.h"

/* Function Description -----------------------------------------------------
Description:
    To convert the data to be the right size.

Prototype:
    publicdef INT nlz_move_data(VOIDPTR pbyte, INT type, size_t size, DOUBLE dValue );
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pbyte           -(VOIDPTR)
    type            -(INT)
    size            -(size_t)
    dValue          -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_move_data (VOIDPTR pbyte, INT type, size_t size, DOUBLE dValue) 
#else
publicdef INT nlz_move_data (pbyte, type, size, dValue)
VOIDPTR pbyte;
INT type;
size_t size;
DOUBLE dValue;
#endif
    {
    switch (type)
        {
    case INT_TYPE:
        switch (size)
            {
        case sizeof(CHAR):
            if (dValue > CHAR_MAX)
                {
                *((CHAR *)pbyte) = CHAR_MAX;
                }
            else if (dValue < CHAR_MIN)
                {
                *((CHAR *)pbyte) = CHAR_MIN;
                }
            else
                {
                *((CHAR *)pbyte) = (CHAR)dValue;
                }
            break;
        case sizeof(SHORT):
            if (dValue > SHRT_MAX)
                {
                *((SHORT *)pbyte) = SHRT_MAX;
                }
            else if (dValue < SHRT_MIN)
                {
                *((SHORT *)pbyte) = SHRT_MIN;
                }
            else
                {
                *((SHORT *)pbyte) = (SHORT)dValue;
                }
            break;
        case sizeof(INT):
            if (dValue > LONG_MAX)
                {
                *((INT *)pbyte) = LONG_MAX;
                }
            else if (dValue < LONG_MIN)
                {
                *((INT *)pbyte) = LONG_MIN;
                }
            else
                {
                *((INT *)pbyte) = (INT)dValue;
                }
            break;
            }
        break;
    case FLOAT_TYPE:
        switch (size)
            {
        case sizeof(FLOAT):
            *((FLOAT *)pbyte) = (FLOAT)dValue;
            break;
        case sizeof(DOUBLE):
            *((DOUBLE *)pbyte) = dValue;
            break;
            }
        break;
    default:
        return FAIL;
        }
    return SUCCESS;
    }
/* END:     */
