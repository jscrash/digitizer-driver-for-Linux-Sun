/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_PLU.C*/
/* *2    17-AUG-1990 21:58:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_VERIFY_PLU.C*/
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

#ifndef ESI_MD_DEFS_H

#include "esi_md_defs.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to validate a PLU parameter value.

Prototype:
    publicdef INT md_verify_plu(MD_KEY_INFO *key_info,CHAR parm_string[MD_MAX_PARM_STRING],
        MD_PARM_STRUCT **parmstruc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_info        -(MD_KEY_INFO *)
    parm_string     -(CHAR [])
    parmstruc       -(MD_PARM_STRUCT **) Map def parameter structure.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_CANT_FIND_LOOKUP_TABLE
    MD_CANT_FIND_LOOKUP_VALUE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_verify_plu (MD_KEY_INFO *key_info,
                             CHAR parm_string[MD_MAX_PARM_STRING],
                             MD_PARM_STRUCT **parmstruc)
#else
publicdef INT md_verify_plu (key_info, parm_string, parmstruc)
MD_KEY_INFO *key_info;
CHAR parm_string[MD_MAX_PARM_STRING];
MD_PARM_STRUCT **parmstruc;
#endif
    {
    
    MD_PARM_STRUCT *temp_parm;
    static CHAR table[MD_ARG_LENGTH] = "";
    static CHAR **parmlist = (CHAR **)NULL;
    static INT no_parms;
    CHAR **templist;
    INT temp_no;
    INT i;
    INT status;                     /* RETURN STATUS */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (strcmp (key_info->val_arg, table) != 0)
        {
        status = mp_project_parm_list (key_info, &templist, &temp_no);
        if (status EQUALS SUCCESS)
            {
            strcpy (table, key_info->val_arg);
            ts_tcp_free (parmlist);
            parmlist = templist;
            no_parms = temp_no;
            }
        }
    if (status EQUALS SUCCESS)
        {
        
        /* * FIND LOOKUP VALUE * */
        /* --------------------- */
        
        for (i = 0; i < no_parms; i++)
            {
            if (strcmp (parm_string, parmlist[i]) EQUALS 0)
                {
                break;
                }
            }
        /* * MAKE PARMSTRUC * */
        /* ------------------ */
        
        if (i < no_parms)
            {
            temp_parm = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL,
                                                       sizeof(MD_PARM_STRUCT));
            temp_parm->parmtype = 1;
            temp_parm->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL,
                                                           strlen (parmlist[i]) + 1);
            strcpy (temp_parm->parmval.cval, parmlist[i]);
            temp_parm->nxtparm = NULL_PARM;
            *parmstruc = temp_parm;
            }
        else
            {
            status = MD_CANT_FIND_LOOKUP_VALUE;
            }
        }
    else
        {
        status = MD_CANT_FIND_LOOKUP_TABLE;
        }
    return status;
    }
/* END:     */
