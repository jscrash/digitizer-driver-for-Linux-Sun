/* DEC/CMS REPLACEMENT HISTORY, Element MD_PARM_VALU.C*/
/* *2    17-AUG-1990 21:57:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_PARM_VALU.C*/
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
#include "esi_md_defs.h"
#include "esi_am.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Function to determine the 'true' type and value of a
    parameter. "parm" should come in as a 'trimmed' string,
    no leading or trailing blanks.

            type = 1 if string
                 = 2 if integer
                 = 3 if real.

Prototype:
    publicdef INT md_parm_value(CHAR *parm,INT *parm_type,MD_PARMVAL_UNION *parm_value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parm            -(CHAR *) Supplied parameter string.
    parm_type       -(INT *) Returned parameter type.
    parm_value      -(MD_PARMVAL_UNION *) Returned parameter.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_parm_value (CHAR *parm, INT *parm_type, MD_PARMVAL_UNION *parm_value)
#else
publicdef INT md_parm_value (parm, parm_type, parm_value)
CHAR *parm;
INT *parm_type;
MD_PARMVAL_UNION *parm_value;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    INT i;                          /* DUMMY INDEX */
    BOOL parm_is_string;            /* FLAG FOR STRING */
    BOOL parm_is_real;              /* FLAG FOR DECIMAL FOUND */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    parm_is_string = FALSE;
    parm_is_real = FALSE;
    
    /* * IS STRING A NUMBER * */
    /* ---------------------- */
    
    if (ts_isfloat (parm) <= 0)
        {
        parm_is_string = TRUE;
        }
    /* * DETERMINE IF PARAMETER IS FLOAT OR INTEGER * */
    /* ---------------------------------------------- */
    
    else
        {
        i = 0;
        /* SKIP SIGN */
        
        if (parm[i] EQUALS '+' OR parm[i] EQUALS '-')
            {
            i++;
            }
        /* IF NOT A DIGIT, MUST BE REAL */
        
        while (parm[i])
            {
            if ((parm[i] < '0') OR (parm[i] > '9'))
                {
                parm_is_real = TRUE;
                break;
                }
            i++;
            }
        }
    /* * SET PARAMETER TYPE AND VALUE * */
    /* -------------------------------- */
    
    if (parm_is_string)             /* PARAMETER IS A STRING */
        {
        *parm_type = 1;
        (*parm_value).cval = parm;
        }
    else if (parm_is_real)          /* PARAMETER IS A REAL NUMBER */
        {
        *parm_type = 3;
        (*parm_value).dbval = atof (parm);
        }
    else                            /* PARAMETER IS AN INTEGER NUMBER */
        {
        *parm_type = 2;
        (*parm_value).ival = atol (parm);
        }
    return status;
    }
/* END:     */
