/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_NUM.C*/
/* *3    17-AUG-1990 21:59:16 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    11-AUG-1989 17:45:00 JULIAN "(SPR -1) temp_parm2 may be used before set"*/
/* *1    19-JUN-1989 13:07:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_NUM.C*/
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
#include "esi_gl_defs.h"
#include "esi_am_defs.h"
#include "esi_am.h"
#include "esi_md_defs.h"
#include "esi_md.h"
#include "esi_md_err.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Function to verify a list of numbers.

Prototype:
    publicdef INT md_verify_numlist(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_numlist (MD_KEY_INFO *key_info,
                                 CHAR parm_string[MD_MAX_PARM_STRING],
                                 MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_numlist (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    INT status;
    INT plen;                       /* LENGTH OF PARAMETER */
    CHAR *cptr;                     /* DUMMY STRING POINTER */
    CHAR *ctemp;                    /* DUMMY STRING POINTER */
    MD_PARM_STRUCT *temp_parm1;     /* PARAMETER STRUCTURE */
    MD_PARM_STRUCT *temp_parm2 = (MD_PARM_STRUCT *)0;
    /* PARAMETER STRUCTURE */
    CHAR temp_string[MD_MAX_PARM_STRING];
    BOOL loop = TRUE;
    BOOL first = TRUE;
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * FIND PARAMETER DELIMITER * */
    /* ---------------------------- */
    
    ctemp = parm_string;
    while (loop)
        {
        if ((cptr = md_next_delim (ctemp, &status)) != NULL)
            {
            plen = cptr - ctemp;
            }
        else
            {
            if (status < 0)
                {
                break;
                }
            loop = FALSE;
            plen = strlen (ctemp);
            }
        strncpy (temp_string, ctemp, plen);
        temp_string[plen] = '\0';
        ctemp = cptr + 1;
        
        status = md_verify_num (key_info, temp_string, &temp_parm1);
        if (status != SUCCESS)
            {
            if (NOT first)
                {
                md_free_parms (*parmstruc);
                }
            break;
            }
        if (first)
            {
            *parmstruc = temp_parm1;
            first = FALSE;
            }
        else
            {
            temp_parm2->nxtparm = temp_parm1;
            }
        temp_parm2 = temp_parm1;
        
        }
    return status;
    }
/* END:     */
