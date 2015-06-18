/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_PROC.C */
/* *3    14-AUG-1990 13:09:55 VINCE "(SPR 5644) Header Standardization" */
/* *2    11-AUG-1989 20:33:57 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes" */
/* *1    11-AUG-1989 19:38:45 CONROY "SQL Macro Processor" */
/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_PROC.C */
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

/* Function Description -----------------------------------------------------
Description:
        Substitute into the given sql phrase the user supplied values for
        all substitution variables.
        
        If no substitution variables are present in the input sql phrase,
        the output phrase will be identical.
        
        This function operates a modal user interface.
 
    SUBSTITUTION VARIABLE SYNTAX:
    
        Because the Oracle-defined syntax is too limited to support the functionality
        required by FINDER, we have defined a richer syntax.  A DBA or user who is
        defining a new SQL "macro" phrase can still use the Oracle syntax to test
        his SQL phrase, then modify the Oracle variables to the FINDER syntax.
                
    
        Substitution variables in an SQL phrase are of the form:
        
            <<prompt string>>, or
            <<prompt string | operator string>>
            
        where
        
            prompt string   :== any character string excluding the > character, and
            operator string :== operator = operand1, operand2, ...
            operator        :== TLU = table_name,column_name (user selects one from list)
                            :== PROJ (substitute current project account name)
                            :== ANAL (substitute current analyst account name)
                        etc.
        
        Each identical occurence of a given prompt string in the phrase will be replaced
        by the user supplied response string (i.e., the user is prompted only once for each
        unique prompt string).

Prototype:
    extern INT db_sql_parms_processor( CHAR *old_phrase, CHAR **new_phrase );
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) old_phrase  -(CHAR *) Old phrase.
    (O) new_phrase  -(CHAR **) New phrase (allocated here).

Return Value/Status:
    SUCCESS - If request was honored,
    FAIL - If old_phrase is improperly formed,
    CANCEL - If user canceled the interaction.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT db_sql_parms_processor (CHAR *old_phrase, CHAR **new_phrase) 
#else
extern INT db_sql_parms_processor (old_phrase, new_phrase)
CHAR *old_phrase;
CHAR **new_phrase;
#endif
    {
    
    INT status = SUCCESS;
    INT nvars;
    SUB_VAR_STRUCT *sub_vars = 0, *cs = 0;
    
    status = db_sql_parms_parse (old_phrase, &nvars, &sub_vars);
    
    if (status == SUCCESS)
        {
        if (nvars <= 0)
            {
            /* no variables to be process, so alloc new
            string space and copy old to new */
            
            *new_phrase = tc_alloc (strlen (old_phrase) + 1);
            strcpy (*new_phrase, old_phrase);
            }
        else
            {
            /* interact with user to define values to substitute */
            
            status = db_sql_parms_user (nvars, sub_vars);
            if (status == SUCCESS)
                {
                /* create the new sql phrase, and substitute user values */
                
                status = db_sql_parms_substitute (old_phrase, new_phrase, nvars,
                                                  sub_vars);
                }
            /* free space in sub_vars list */
            
            for (cs = sub_vars; cs != (SUB_VAR_STRUCT *)0; cs = cs->next)
                {
                tc_free ((BYTE *)cs);
                }
            }
        }
    return status;
    }
/* END:     */
