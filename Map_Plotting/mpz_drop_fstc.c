/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_DROP_FSTC.C*/
/*  *2    17-AUG-1990 22:02:58 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:09:58 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_DROP_FSTC.C*/
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
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to add the supplied name to the
    supplied map file structure.

Prototype:
    publicdef INT mpz_drop_from_file_struct(CHAR *name,MP_FILES *file_struct);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)
    file_struct     -(MP_FILES *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mpz_drop_from_file_struct (CHAR *name, MP_FILES *file_struct)
#else
publicdef INT mpz_drop_from_file_struct (name, file_struct)
CHAR *name;
MP_FILES *file_struct;
#endif
    {
    
    INT i, j, k;
    
    /* ********************************************************************** */
    
    /* * SEARCH TCP FOR FILE NAME * */
    /* ---------------------------- */
    
    for (i = 0; i < file_struct->n_names; i++)
        {
        if (strcmp (name, file_struct->name[i]) EQUALS 0)
            {
            break;
            }
        }
    /* * FOUND IT * */
    /* ------------ */
    
    if (i < file_struct->n_names)
        {
        (file_struct->n_references[i])--;
        
        /* DELETE IT FROM TCP */
        
        if (file_struct->n_references[i] EQUALS 0)
            {
            tc_free (file_struct->name[i]);
            for (j = i + 1; j < file_struct->n_names; j++)
                {
                k = j - 1;
                file_struct->name[k] = file_struct->name[j];
                file_struct->n_references[k] = file_struct->n_references[j];
                }
            (file_struct->n_names)--;
            file_struct->name[file_struct->n_names] = NULL;
            }
        }
    return SUCCESS;
    }
/* END:     */
