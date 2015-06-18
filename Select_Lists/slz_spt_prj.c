/* DEC/CMS REPLACEMENT HISTORY, Element SLZ_SPT_PRJ.C*/
/* *2    25-JUL-1990 20:08:01 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:25:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SLZ_SPT_PRJ.C*/
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

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to sp

Prototype:
    publicdef INT slz_spt_project_table(CHAR **table_name, CHAR **project);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    table_name      -(CHAR **) String to check.
    project         -(CHAR **) String to check.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT slz_spt_project_table (CHAR **table_name, CHAR **project)
#else
publicdef INT slz_spt_project_table (table_name, project)
CHAR **table_name, **project;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    CHAR *cptr_table, *cptr_project;    /* TEMP CHAR STRING POINTER */
    CHAR *word1, *word2;            /* WORD FROM TS_NEXT_WORD */
    INT slen1, slen2;               /* LENGTH OF STRING */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    slen1 = strlen (*table_name);
    word1 = (CHAR *)am_allocate (AM_APPLICATION, slen1 + 1);
    ts_sto_upper (*table_name);
    if ((cptr_table = strchr (*table_name, '.')) IS_NOT_EQUAL_TO 0)
        {
        
        word2 = (CHAR *)am_allocate (AM_APPLICATION, slen1 + 1);
        slen2 = strlen (cptr_table) - 1;
        slen1 = (cptr_table - *table_name);
        hoblockmove (*table_name, word1, slen1);
        word1[slen1] = '\0';
        cptr_table++;
        hoblockmove (cptr_table, word2, slen2);
        word2[slen2] = '\0';
        am_free (*table_name);
        *table_name = word2;
        *project = word1;
        }
    else
        am_free (word1);
    
    return status;
    }
/* END:     */
