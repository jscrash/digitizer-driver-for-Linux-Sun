/* DEC/CMS REPLACEMENT HISTORY, Element SX_CHK_DLGITM.C*/
/* *4    17-AUG-1990 22:20:04 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-SEP-1989 18:33:34 GORDON "(SPR -1) checkin from gulf"*/
/* *2    25-JUL-1989 20:10:10 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_CHK_DLGITM.C*/
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
#include "esi_sx.h"
#include "esi_ts.h"
#include "esi_wi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Read and verify individual numeric dialog items.

Prototype:
    publicdef INT sx_check_dialog_item(INT id,INT item,INT message,INT data_type,BYTE *min,
        BYTE *max,BYTE *value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Dialog id.
    item            -(INT) Dialog item.
    message         -(INT) Mg message number code.
    data-type       -(INT) INT_TYPE or FLOAT_TYPE.
    min             -(BYTE *) Pointer to min in range value.
    max             -(BYTE *) Pointer to max in range value.
    value           -(BYTE *) Returned value if SUCCESS.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_check_dialog_item (INT id, INT item, INT message, INT data_type,
                                    BYTE *min, BYTE *max, BYTE *value)
#else
publicdef INT sx_check_dialog_item (id, item, message, data_type, min, max, value)
INT id;
INT item;
INT message;
INT data_type;
BYTE *min;
BYTE *max;
BYTE *value;
#endif
    {
    INT status;
    CHAR text[256];
    INT dummy;
    INT imin, imax, ival;
    DOUBLE fmin, fmax, fval;
    CHAR text2[128];
    
    /* query item */
    
    wiqccdi (id, item, &dummy, text);
    
    /* assume SUCCESS */
    
    status = SUCCESS;
    
    /* check numeric */
    
    if (ts_isfloat (text) <= 0)
        {
        strcpy (text2, mg_message (SX_NOT_NUMERIC));
        sprintf (text, "%s %s", mg_message (message), text2);
        am_message (AM_ALERT, text);
        status = FAIL;
        SX_RETURN;
        }
    /* set value */
    
    if (data_type == INT_TYPE)
        {
        *(INT *)value = ival = atoi (text);
        }
    else
        {
        *(DOUBLE *)value = fval = atof (text);
        }
    /* check range */
    
    if (min == (BYTE *)0 OR max == (BYTE *)0)
        {
        SX_RETURN;
        }
    if (data_type == INT_TYPE)
        {
        imin = *(INT *)min;
        imax = *(INT *)max;
        if (ival < imin OR ival > imax)
            {
            strcpy (text2, mg_message (SX_TO));
            sprintf (text, "%s %d %s %d", mg_message (SX_VALID_RANGE), imin, text2,
                     imax);
            am_message (AM_ALERT, text);
            status = FAIL;
            }
        SX_RETURN;
        }
    /* float type */
    
    fmin = *(DOUBLE *)min;
    fmax = *(DOUBLE *)max;
    if (fval < fmin OR fval > fmax)
        {
        strcpy (text2, mg_message (SX_TO));
        sprintf (text, "%s %f %s %f", mg_message (SX_VALID_RANGE), fmin, text2, fmax);
        am_message (AM_ALERT, text);
        status = FAIL;
        }
    SX_RETURN;
    }
/* END:     */
