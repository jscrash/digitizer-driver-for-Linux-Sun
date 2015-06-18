/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_CMPVRB.C*/
/*  *2    17-AUG-1990 22:06:09 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:12:26 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_CMPVRB.C*/
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

#include    "esi_mp.h"
#include    "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to allocate and initialize the compiled verb
    structure.  Size and initialization requirements are
    determined by the verb that is being initialized.

Prototype:
    publicdef INT mp_init_comp_verb(MD_COMPILED_VERB **comp_verb,
        MD_VERB_INFO *verb_info);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    comp_verb       -(MD_COMPILED_VERB **)
    verb_info       -(MD_VERB_INFO *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_init_comp_verb (MD_COMPILED_VERB **comp_verb, MD_VERB_INFO *verb_info)
#else
publicdef INT mp_init_comp_verb (comp_verb, verb_info)
MD_COMPILED_VERB **comp_verb;
MD_VERB_INFO *verb_info;
#endif
    {
    INT status;
    MD_COMPILED_VERB *temp_comp_verb;
    MD_KEY_INFO *temp_key_info;
    INT i;
    
    status = SUCCESS;
    
    /* * ALLOCATE THE VERB STRUCTURE * */
    /* ------------------------------- */
    
    temp_comp_verb = (MD_COMPILED_VERB *)am_allocate (AM_GLOBAL,
                                                      sizeof(MD_COMPILED_VERB));
    temp_comp_verb->nrows = verb_info->no_of_keywords;
    temp_comp_verb->key_code = (INT *)am_allocate (AM_GLOBAL,
                                                   (temp_comp_verb->nrows) *
                                                   sizeof(INT));
    temp_comp_verb->key_used = (INT *)am_allocate (AM_GLOBAL,
                                                   (temp_comp_verb->nrows) *
                                                   sizeof(INT));
    temp_comp_verb->parm_ptr = (MD_PARM_STRUCT **)am_allocate (AM_GLOBAL,
                                                               (temp_comp_verb->
                                                                nrows) *
                                                               sizeof(MD_PARM_STRUCT *)
                                                               );
    
    for (i = 0; i < verb_info->no_of_keywords; i++)
        {
        temp_key_info = &(verb_info->key_list[i]);
        temp_comp_verb->key_code[i] = temp_key_info->key_code;
        if (temp_key_info->key_req)
            {
            temp_comp_verb->key_used[i] = -1;
            }
        }
    *comp_verb = temp_comp_verb;
    
    return SUCCESS;
    }
/* END:     */
