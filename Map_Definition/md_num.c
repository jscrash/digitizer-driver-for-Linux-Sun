/* DEC/CMS REPLACEMENT HISTORY, Element MD_NUM.C*/
/* *2    17-AUG-1990 21:57:31 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_NUM.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Function to do table lookup parameter validation on given
    parameter.

Prototype:
    publicdef INT md_num(MD_PARM_STRUCT *parmstruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_BAD_NUMBER_RANGE
    MD_WRONG_TYPE_PARAMETER
    MD_BAD_PARAMETER_VALUE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_num (MD_PARM_STRUCT *parmstruc, MD_KEYLIST *keylist)
#else
publicdef INT md_num (parmstruc, keylist)
MD_PARM_STRUCT *parmstruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_NUM");
    INT status;                     /* RETURN STATUS */
    CHAR clow[20];                  /* LOWER LIMIT AS CHAR (IF ANY) */
    CHAR chigh[20];                 /* UPPER LIMIT AS CHAR (IF ANY) */
    DOUBLE low;                     /* LOWER LIMIT AS FLOAT (IF ANY) */
    DOUBLE high;                    /* UPPER LIMIT AS FLOAT (IF ANY) */
    DOUBLE dummy;                   /* DUMMY HOLDING PARAMETER VALUE */
    INT klen;                       /* VALIDATION ARGUEMENT STRING LENGTH */
    INT clen;                       /* # OF CHARS TO "," IN VAL ARG */
    CHAR *cptr;                     /* DUMMY STRING POINTER */
    
    /* ********************************************************************** */
    
    
    /* * GET PARAMETER VALUE * */
    /* ----------------------- */
    
    cptr = parmstruc->parmval.cval;
    status = md_parm_value (cptr, &(parmstruc->parmtype), &(parmstruc->parmval));
    
    /* * TEST PARAMETER VALUE * */
    /* ------------------------ */
    
    if (status EQUALS SUCCESS)
        {
        
        /* PARAMETER IS A STRING */
        
        if (parmstruc->parmtype EQUALS 1)
            {
            status = MD_WRONG_TYPE_PARAMETER;
            }
        /* PARAMETER IS A NUMBER */
        
        else
            {
            am_free (cptr);
            if (parmstruc->parmtype EQUALS 2)
                {
                dummy = parmstruc->parmval.ival;
                }
            else
                {
                dummy = parmstruc->parmval.dbval;
                }
            /* LOOK FOR STORED RANGE */
            
            if ((klen = strlen (keylist->val_arg)) > 0)
                {
                
                /* PARSE STORED RANGE */
                
                if ((cptr = strchr (keylist->val_arg, SEPARATOR)) != NULL)
                    {
                    clen = cptr - keylist->val_arg;
                    strncpy (clow, keylist->val_arg, clen);
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
        }
    return status;
    }
/* END:     */
