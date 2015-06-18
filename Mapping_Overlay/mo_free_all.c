/* DEC/CMS REPLACEMENT HISTORY, Element MO_FREE_ALL.C*/
/* *2    17-AUG-1990 22:01:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:09:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_FREE_ALL.C*/
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

#include "esi_mo.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Free up model structure, link lists, n-lists, etc, for   
    shutdown purposes.                                      

Prototype:
    publicdef INT mo_free(STRAT_MODEL_STRUCT *pModel);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pModel          -(STRAT_MODEL_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mo_free (STRAT_MODEL_STRUCT *pModel)
#else
publicdef INT mo_free (pModel)
STRAT_MODEL_STRUCT *pModel;
#endif
    {
    INT i;
    
    /* we really do not want to free the pcse->pwell because we get well header
       will give the pointer back if you try to do the same well again. */
    
    CSE_WELL *pcse;
    
    for (i = 0, llfirst (&pModel->well_list, &pcse); i < llcount (&pModel->well_list);
         i++)
        {
        
        tc_free (pcse);
        llnext (&pModel->well_list, &pcse);
        }
    llfree (&pModel->well_list);
    llfree (&pModel->uwi_list);
    
    nl_free_nlist (pModel->model.nlist);
    nl_free_nlist (pModel->velocity.nlist);
    
    tc_free (pModel);
    }
/* END:     */
