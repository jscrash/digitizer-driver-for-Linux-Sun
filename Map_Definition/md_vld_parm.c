/* DEC/CMS REPLACEMENT HISTORY, Element MD_VLD_PARM.C*/
/* *2    17-AUG-1990 21:59:50 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:08:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VLD_PARM.C*/
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
#include "esi_md.h"
#include "esi_md_err.h"

/* Function Description -----------------------------------------------------
Description:
    Function to validate the parameters of the given keyword.

Prototype:
    publicdef INT md_vld_parm(MD_KEY_STRUCT *keystruc,MD_KEYLIST *keylist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc       -(MD_PARM_STRUCT *) Map def parameter structure.
    keylist         -(MD_KEYLIST *) Keyword attributes.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_UNRECOGNIZED_VALID_CODE
    MD_WRONG_NUM_PARAMETER
    MD_MISSING_REQ_PARAMETER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_vld_parm (MD_KEY_STRUCT *keystruc, MD_KEYLIST *keylist)
#else
publicdef INT md_vld_parm (keystruc, keylist)
MD_KEY_STRUCT *keystruc;
MD_KEYLIST *keylist;
#endif
    {
    
    PROGNAME ("MD_VLD_PARM");
    MD_PARM_STRUCT *parmstruc;      /* MAP DEF PARAMETER STRUCTURE */
    CHAR dflt[30];                  /* DUMMY FOR DEFAULT VALUE */
    CHAR *cptr;                     /* DUMMY CHAR POINTER */
    INT status;                     /* RETURN STATUS */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    dflt[0] = '\0';
    parmstruc = keystruc->parmptr;
    
    /* * NO PARAMETER WITH GIVEN KEY * */
    /* ------------------------------- */
    
    if (parmstruc EQUALS NULL_PARM)
        {
        /* PARAMETER REQUIRED */
        
        if (keylist->parm_rqd EQUALS 'Y')
            {
            status = MD_MISSING_REQ_PARAMETER;
            }
        /* GET PARAMETER DEFAULT */
        
        else if (strlen (keylist->parm_dflt) != 0)
            {
            strcpy (dflt, keylist->parm_dflt);
            }
        /* GET KEYWORD DEFAULT */
        
        else if (strlen (keylist->key_dflt) != 0)
            {
            strcpy (dflt, keylist->key_dflt);
            }
        /* INSERT DEFAULT AND CHECK */
        
        if (strlen (dflt) != 0)
            {
            keystruc->parmptr = (MD_PARM_STRUCT *)md_find_parm (dflt, &status);
            if (status EQUALS SUCCESS)
                {
                status = md_vld_parm (keystruc, keylist);
                }
            }
        }
    /* * MULTIPLE PARAMETERS WITH KEYWORD * */
    /* ------------------------------------ */
    
    else if (parmstruc->nxtparm != NULL_PARM)
        {
        
        /* NO VALIDATION NECCESSARY */
        
        if (keylist->val_code[0] EQUALS '\0')
            {
            while ((parmstruc != NULL_PARM) AND (status EQUALS SUCCESS))
                {
                cptr = parmstruc->parmval.cval;
                status = md_parm_value (parmstruc->parmval.cval,
                                        &(parmstruc->parmtype), &(parmstruc->parmval));
                if (parmstruc->parmtype != 1)
                    {
                    am_free (cptr);
                    }
                parmstruc = parmstruc->nxtparm;
                }
            }
        /* TABLE LOOK-UP LIST */
        
        else if (strcmp (keylist->val_code, "TLU.LIST") EQUALS 0)
            {
            while ((parmstruc != NULL_PARM) AND (status EQUALS SUCCESS))
                {
                status = md_tlu (parmstruc, keylist);
                parmstruc = parmstruc->nxtparm;
                }
            }
        /* NUMBER LIST */
        
        else if (strcmp (keylist->val_code, "NUM.LIST") EQUALS 0)
            {
            while ((parmstruc != NULL_PARM) AND (status EQUALS SUCCESS))
                {
                status = md_num (parmstruc, keylist);
                parmstruc = parmstruc->nxtparm;
                }
            }
        /* PROJECT TABLE LOOK-UP LIST */
        
        else if (strcmp (keylist->val_code, "PLU.LIST") EQUALS 0)
            {
            while ((parmstruc != NULL_PARM) AND (status EQUALS SUCCESS))
                {
                status = md_plu (parmstruc, keylist);
                parmstruc = parmstruc->nxtparm;
                }
            }
        else
            {
            status = MD_WRONG_NUM_PARAMETER;
            }
        }
    /* * SINGLE PARAMETER WITH KEYWORD * */
    /* --------------------------------- */
    
    else
        {
        
        /* NO VALIDATION NECCESSARY */
        
        if (keylist->val_code[0] EQUALS '\0')
            {
            cptr = parmstruc->parmval.cval;
            status = md_parm_value (parmstruc->parmval.cval, &(parmstruc->parmtype),
                                    &(parmstruc->parmval));
            if (parmstruc->parmtype != 1)
                {
                am_free (cptr);
                }
            }
        /* TABLE LOOK_UP */
        
        else if ((strcmp (keylist->val_code,
                          "TLU.LIST") EQUALS 0) OR (strcmp (keylist->val_code,
                                                         "TLU") EQUALS 0))
            {
            status = md_tlu (parmstruc, keylist);
            }
        /* NUMBER */
        
        else if ((strcmp (keylist->val_code,
                          "NUM.LIST") EQUALS 0) OR (strcmp (keylist->val_code,
                                                         "NUM") EQUALS 0))
            {
            status = md_num (parmstruc, keylist);
            }
        /* LIST OF ALTERNATIVES */
        
        else if (strcmp (keylist->val_code, "ALT") EQUALS 0)
            {
            status = md_alt (parmstruc, keystruc);
            }
        /* PROJECT TABLE LOOK UP */
        
        else if ((strcmp (keylist->val_code,
                          "PLU.LIST") EQUALS 0) OR (strcmp (keylist->val_code,
                                                         "PLU") EQUALS 0))
            {
            status = md_plu (parmstruc, keylist);
            }
        /* SAVED LIST EXISTS */
        
        else if (strcmp (keylist->val_code, "SLE") EQUALS 0)
            {
            status = md_sle (parmstruc, keylist);
            }
        /* LEGAL FILE NAME */
        
        else if (strcmp (keylist->val_code, "LFN") EQUALS 0)
            {
            status = md_lfn (parmstruc, keylist);
            }
        /* FILE NAME EXISTS */
        
        else if (strcmp (keylist->val_code, "FNE") EQUALS 0)
            {
            status = md_fne (parmstruc, keylist);
            }
        /* DICTIONARY LOOK UP */
        
        else if (strcmp (keylist->val_code, "DLU") EQUALS 0)
            {
            status = md_dlu (parmstruc, keylist);
            }
        /* DISPLAY UNITS OF MEASURE */
        
        else if (strcmp (keylist->val_code, "DUOM") EQUALS 0)
            {
            status = md_duom (parmstruc, keylist);
            }
        /* UNITS OF MEASURE */
        
        else if (strcmp (keylist->val_code, "UOM") EQUALS 0)
            {
            status = md_uom (parmstruc, keylist);
            }
        /* DATE */
        
        else if (strcmp (keylist->val_code, "DATE") EQUALS 0)
            {
            status = md_date (parmstruc, keylist);
            }
        /* SUBROUTINE */
        
        else if (strcmp (keylist->val_code, "SUBR") EQUALS 0)
            {
            status = md_subr (parmstruc, keylist);
            }
        /* EXPRESSION */
        
        else if (strcmp (keylist->val_code, "EXPR") EQUALS 0)
            {
            status = md_expr (parmstruc, keylist);
            }
        /* SQL */
        
        else if (strcmp (keylist->val_code, "SQL") EQUALS 0)
            {
            status = md_sql (parmstruc, keylist);
            }
        else
            {
            status = MD_UNRECOGNIZED_VALID_CODE;
            }
        }
    return status;
    }
/* END:     */
