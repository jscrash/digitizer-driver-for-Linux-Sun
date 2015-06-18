/* DEC/CMS REPLACEMENT HISTORY, Element GE_OVERLAY.C*/
/* *2    14-AUG-1990 13:27:53 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:52:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GE_OVERLAY.C*/
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
#include "esi_mp.h"
#include "esi_ee.h"
#include "esi_ts.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    To do a Grid operation for a list of parameters.

Prototype:
    publicdef INT ge_overlay(INT display_line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ge_overlay (INT display_line)
#else
publicdef INT ge_overlay (display_line)
INT display_line;
#endif
    {
    INT status = SUCCESS;
    MAP_STRUCTURE *mapdef;
    MD_COMPILED_VERB *vstruct;
    MD_PARM_STRUCT *vpstruct;
    INT i;
    CHAR *exp_in, *exp_out, *value_out;
    
    mp_get_current_map (&mapdef);
    vstruct = mapdef->compiled->verb_ptr[mapdef->display->compiled_row[display_line]];
    
    for (i = 0; i < vstruct->nrows; i++)
        {
        if (vstruct->key_used[i] != 0)
            {
            switch (vstruct->key_code[i])
                {
            case 1:                 /* EXPRESSION */
                vpstruct = vstruct->parm_ptr[i];
                break;
                
            default:
                vpstruct = NULL_PARM;
                break;
                }
            }
        }
    /* go througth all keys to operate*/
    /* all expressions.              */
    
    while (vpstruct IS_NOT_EQUAL_TO NULL_PARM)
        {
        /* operate this expression.      */
        exp_in = STRING_ALLOCATE (vpstruct->parmval.cval);
        strcpy (exp_in, vpstruct->parmval.cval);
        /*    exp_in = ts_noblanks( exp_in );*/
        status = ee_parser (exp_in, &exp_out);
        tc_free (exp_in);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            break;
            }
        status = ee_evaluator (exp_out, &value_out);
        tc_free (exp_out);
        
        vpstruct = vpstruct->nxtparm;
        }
    return status;
    }
/* END:     */
