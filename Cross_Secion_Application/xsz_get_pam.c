/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_GET_PAM.C*/
/* *5    31-OCT-1990 09:31:46 GILLESPIE "(SPR 37) Sun compile fixes"*/
/* *4    17-AUG-1990 22:35:55 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     2-NOV-1989 13:03:00 GILLESPIE "(SPR 31) Change for esi_xs_parse.h"*/
/* *2    19-SEP-1989 11:14:33 GORDON "(SPR 100) cast in call to strcpy"*/
/* *1    19-JUN-1989 13:42:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_GET_PAM.C*/
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

#include "esi_xs_parse.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To retrieve a layout information structure from an oracle database
    and make it to be parser language structure; To get out parameters from 
    the MD_KEY_STRUCT.

Prototype:
    publicdef INT xsz_get_parms(MD_KEY_STRUCT *keyStr, MD_PARM_STRUCT *parmStr, INT ith, 
        BYTE *variable, INT *status) ;

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keystr      -(MD_KEY_STRUCT *) A pointer points to MD_KEY_STRUCT.
    (I) parmstr     -(MD_PARM_STRUCT *) A pointer points to MD_PARM_STRUCT.
    (I) ith         -(INT) The ith parameter in the parameters list requested.
    (I) variable    -(BYTE *) The pointer points returned variable.
    (O) status      -(INT *) The returned status.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_get_parms (MD_KEY_STRUCT *keyStr, MD_PARM_STRUCT *parmStr, INT ith,
                             BYTE *variable, INT *status)
#else
publicdef INT xsz_get_parms (keyStr, parmStr, ith, variable, status)
MD_KEY_STRUCT *keyStr;
MD_PARM_STRUCT *parmStr;
INT ith;
BYTE *variable;
INT *status;
#endif
    {
    if (keyStr->parmptr EQUALS (MD_PARM_STRUCT *)0)
        *status = FAIL;
    else
        {
        parmStr = keyStr->parmptr;
        
        /* Looking for ith parameter.                */
        
        while (ith IS_NOT_EQUAL_TO 1 AND parmStr IS_NOT_EQUAL_TO (MD_PARM_STRUCT *)0)
            {
            ith--;
            parmStr = parmStr->nxtparm;
            }
        if (parmStr EQUALS (MD_PARM_STRUCT *)0)
            *status = FAIL;
        else
            {
            
            /* according to the parameter type,     */
            /* returns data.                */
            
            switch (parmStr->parmtype)
                {
            case 1:
                strcpy ((CHAR *)variable, parmStr->parmval.cval);
                break;
            case 2:
                *(INT *)variable = parmStr->parmval.ival;
                break;
            case 3:
                *(DOUBLE *)variable = parmStr->parmval.dbval;
                break;
                }
            *status = SUCCESS;
            }
        }
    }
/* END:     */
