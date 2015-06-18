/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_NUM.C*/
/* *2    17-AUG-1990 21:58:36 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_NUM.C*/
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
#include "esi_md_defs.h"
#include "esi_md_err.h"
#include "esi_am.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Function to do table lookup parameter validation on given
    parameter.

Prototype:
    publicdef INT md_verify_num(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **) Map def parameter structure.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_BAD_PARAMETER_VALUE
    MD_BAD_NUMBER_RANGE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_num (MD_KEY_INFO *key_info,
                             CHAR parm_string[MD_MAX_PARM_STRING],
                             MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_num (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    PROGNAME ("MD_VERIFY_NUM");
    INT status;                     /* RETURN STATUS */
    MD_PARM_STRUCT *temp_parm;
    INT i;
    CHAR clow[20];                  /* LOWER LIMIT AS CHAR (IF ANY) */
    CHAR chigh[20];                 /* UPPER LIMIT AS CHAR (IF ANY) */
    DOUBLE low;                     /* LOWER LIMIT AS FLOAT (IF ANY) */
    DOUBLE high;                    /* UPPER LIMIT AS FLOAT (IF ANY) */
    DOUBLE dummy;                   /* DUMMY HOLDING PARAMETER VALUE */
    INT klen;                       /* VALIDATION ARGUEMENT STRING LENGTH */
    INT clen;                       /* # OF CHARS TO "," IN VAL ARG */
    CHAR *cptr;                     /* DUMMY STRING POINTER */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * TEST PARAMETER VALUE IS NUMERIC * */
    /* ----------------------------------- */
    
    if (ts_isfloat (parm_string) > 0)
        {
        dummy = atof (parm_string);
        
        /* * TEST PARAMETER VALUE IS IN CORRECT RANGE * */
        /* -------------------------------------------- */
        
        if ((klen = strlen (key_info->val_arg)) > 0)
            {
            
            /* PARSE STORED RANGE */
            
            if ((cptr = strchr (key_info->val_arg, SEPARATOR)) != NULL)
                {
                clen = cptr - key_info->val_arg;
                strncpy (clow, key_info->val_arg, clen);
                clow[clen] = '\0';
                strcpy (chigh, ++cptr);
                
                /* TEST NUMBER AGAINST STORED RANGE */
                
                low = atof (clow);
                high = atof (chigh);
                if (dummy < low OR dummy > high)
                    {
                    status = MD_BAD_PARAMETER_VALUE;
                    }
                }
            else
                {
                status = MD_BAD_NUMBER_RANGE;
                }
            }
        }
    else
        {
        status = MD_BAD_PARAMETER_VALUE;
        }
    if (status EQUALS SUCCESS)
        {
        temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_PARM_STRUCT));
        temp_parm->parmtype = 3;
        temp_parm->parmval.dbval = dummy;
        temp_parm->nxtparm = NULL_PARM;
        *parmstruc = temp_parm;
        }
    return status;
    }
/* END:     */
