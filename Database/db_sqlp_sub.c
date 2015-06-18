/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_SUB.C */
/* *5    12-JUL-1991 14:38:44 JULIAN "(SPR 1541) added survey to rel retrieval by line name" */
/* *4    23-AUG-1990 15:29:20 JULIAN "(SPR 0) fix compiling error" */
/* *3    14-AUG-1990 13:10:01 VINCE "(SPR 5644) Header Standardization" */
/* *2    11-AUG-1989 20:34:42 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes" */
/* *1    11-AUG-1989 19:39:44 CONROY "SQL Macro Processor" */
/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_SUB.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_db_sql_parms.h"
#include "esi_tc.h"
#include "esi_ts.h"
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
/* Function Description -----------------------------------------------------
Description:
    Return a new sql phrase where all substitution variables in the
    old sql phrase have been replaced by the user supplied values
    contained in the sub_vars structure list.
 
    INITIAL CONDITIONS:
    
    1.  well-formed sql phrase with well-formed substitution variables.

Prototype:
    publicdef INT db_sql_parms_substitute( CHAR *old_phrase, CHAR **new_phrase, 
                                        INT nvars, SUB_VAR_STRUCT *sub_vars);
                                        
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) old_phrase  -(CHAR *) The original sql phrase.
    (O) new_phrase  -(CHAR **) The new phrase with substitutions.
    (I) nvars       -(INT) The number of variables.
    (I) sub_vars    -(SUB_VAR_STRUCT *) Pointer to list of structures.

Return Value/Status:
    SUCCESS - If request was honored (including no substitutions required),
    FAIL - If one or more substitution variables was not well-formed.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT db_sql_parms_substitute (CHAR *old_phrase, CHAR **new_phrase, INT nvars,
                                       SUB_VAR_STRUCT *sub_vars) 
#else
publicdef INT db_sql_parms_substitute (old_phrase, new_phrase, nvars, sub_vars)
CHAR *old_phrase;
CHAR **new_phrase;
INT nvars;
SUB_VAR_STRUCT *sub_vars;
#endif
    {
    
    INT status = SUCCESS;
    INT l, num_subs = 0, max_len = 0;
    SUB_VAR_STRUCT *cs;
    CHAR *p, prompt_buff[STMAX + 4];
    CHAR *token,temp_text[256];
    
    /* handle boundary cases -- if no sub's req'd, just return copy of old_phrase */
    
    if (old_phrase == (CHAR *)0)
        return FAIL;
    if (nvars <= 0 || sub_vars == (SUB_VAR_STRUCT *)0)
        {
        *new_phrase = (CHAR *)tc_alloc (strlen (old_phrase) + 1);
        strcpy (*new_phrase, old_phrase);
        return SUCCESS;
        }
    /* 
        determine maximum size the new_phrase could be before allocating:
        
        1) find num_subs = count number of sub vars occuring in old_phrase,
        2) find max_len = maximum length response string
        3) heuristic max size is length of old_phrase + num_subs*max_len
    */
    
    for (p = old_phrase; ts_find_substring (p, "<<", &p) == SUCCESS;
                                                                          p += 2,
                                                                          num_subs++)
        ;
    
    for (cs = sub_vars; cs != (SUB_VAR_STRUCT *)0; cs = cs->next)
        {
        if ((l = strlen (cs->response)) > max_len)
            max_len = l;
        }
    /* allocate the new_phrase and copy the old_phrase */
    
    *new_phrase = (CHAR *)tc_alloc (strlen (old_phrase) + num_subs * max_len + 1);
    strcpy (*new_phrase, old_phrase);
    
    /* now do the substitutions */
    
    for (cs = sub_vars; cs != (SUB_VAR_STRUCT *)0; cs = cs->next)
        {
        /* substitute all occurrences of {entire sub variable string} */

	if(strchr(cs->response,'-'))
	  {
	  strcpy(temp_text,cs->response);
	  token = strtok(temp_text,"-");
	  status = ts_sub_substring (*new_phrase, cs->sub_variable, token);
	  token = strtok(NULL,"-");
	  strcpy(cs->response,token);
	}
	else
	  status = ts_sub_substring (*new_phrase, cs->sub_variable, cs->response);
        
        /* format prompt to look like it did in the original sql phrase */
        
        strcpy (prompt_buff, "<<");
        strcat (prompt_buff, cs->prompt);
        strcat (prompt_buff, ">>");
        
        /* then substitute all occurrences of {<<prompt string>>} */
        
        status = ts_sub_substring (*new_phrase, prompt_buff, cs->response);
        }
    return SUCCESS;
    }
/* END:     */
