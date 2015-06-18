/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_PLIST.C*/
/* *2    17-AUG-1990 21:59:25 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:08:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VER_PLIST.C*/
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
#include "esi_md.h"
#include "esi_md_defs.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Function to validate a PLIST parameter value.

Prototype:
    publicdef INT md_verify_plist(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **) Map def parameter structure.       

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_plist (MD_KEY_INFO *key_info,
                               CHAR parm_string[MD_MAX_PARM_STRING],
                               MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_plist (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    PROGNAME ("MD_VERIFY_PLIST");
    MD_PARM_STRUCT *temp_parm1 = NULL_PARM;
    MD_PARM_STRUCT *temp_parm2 = NULL_PARM;
    CHAR temp_string[MD_MAX_PARM_STRING];
    INT plen;                       /* LENGTH OF PARAMETER */
    CHAR *cptr;                     /* DUMMY STRING POINTER */
    INT status;                     /* RETURN STATUS */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * FIND PARAMETER DELIMITER * */
    /* ---------------------------- */
    
    if ((cptr = md_next_delim (parm_string, &status)) != NULL)
        {
        plen = cptr - parm_string;
        }
    else
        {
        if (status < 0)
            {
            goto finished;
            }
        plen = strlen (parm_string);
        }
    if (plen > 0)
        {
        strncpy (temp_string, parm_string, plen);
        temp_string[plen] = '\0';
        ts_snowhite (temp_string);
        status = md_verify_plu (key_info, temp_string, &temp_parm1);
        if (status EQUALS SUCCESS)
            {
            if (cptr != NULL)
                {
                status = md_verify_plist (key_info, ++cptr, &temp_parm2);
                temp_parm1->nxtparm = temp_parm2;
                if (status != SUCCESS)
                    {
                    am_free (temp_parm1);
                    temp_parm1 = NULL_PARM;
                    }
                }
            }
        }
finished:
    
    *parmstruc = temp_parm1;
    return status;
    }
/* END:     */
