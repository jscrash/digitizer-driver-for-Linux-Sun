/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_PARSE.C */
/* *5    30-OCT-1990 11:52:55 GILLESPIE "(SPR 37) Fix Sun side of code" */
/* *4    14-AUG-1990 13:09:49 VINCE "(SPR 5644) Header Standardization" */
/* *3    20-SEP-1989 14:52:51 GORDON "(SPR 200) Get in sync" */
/* *2    11-AUG-1989 20:32:42 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes" */
/* *1    11-AUG-1989 19:37:54 CONROY "SQL Macro Processor" */
/* DEC/CMS REPLACEMENT HISTORY, Element DB_SQLP_PARSE.C */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_db_sql_parms.h"
#include "esi_tc.h"
#include "esi_ts.h"

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Parse the given sql phrase to build a  returned list of substitution          */
/*    variable structures.  This list of structures contains all the info           */
/*    necessary to manage the user interaction to determine the values              */
/*    which will be substituted into the sql phrase.                                */
/*                                                                                  */
/*    INITIAL CONDITIONS:                                                           */
/*                                                                                  */
/*    1.  well-formed sql phrase with well-formed substitution variables.           */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    extern INT db_sql_parms_parse(CHAR *sql_phrase, INT *nvars,                   */
/*                              SUB_VAR_STRUCT **sub_vars );                        */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT db_zz_parse_variable( CHAR *sub_variable, INT *nvars,          */
/*                                    SUB_VAR_STRUCT **sub_vars );                  */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Uniqueness is imposed on the structure list at two levels by treating         */
/*                                                                                  */
/*                sub_vars->sub_variable  as the primary key, and                   */
/*                sub_vars->prompt        as the secondary key                      */
/*                                                                                  */
/*    I.e., a candidate substitution variable will only be added to the list        */
/*    if there is no preceeding occurence of the complete prompt | operator string, */
/*    or of the prompt string.                                                      */
/*    See above.                                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT db_zz_parse_variable( CHAR *sub_variable, INT *nvars,          */
/*                                    SUB_VAR_STRUCT **sub_vars );                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) sql_phrase  -(CHAR *) The phrase to parse.                                */
/*    (O) nvars       -(INT *) The number of variables found.                       */
/*    (O) sub_vars    -(SUB_VAR_STRUCT  **) Pointer to list of structures built.    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion                                               */
/*    FAIL    if one or more substitution variables was not well-formed.            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <db_sql_parms_parse>                                               */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT db_zz_parse_variable (CHAR *sub_variable, INT *nvars,
                                     SUB_VAR_STRUCT **sub_vars)
#else                                   
static INT db_zz_parse_variable (sub_variable, nvars, sub_vars)
CHAR *sub_variable;
INT *nvars;
SUB_VAR_STRUCT **sub_vars;
#endif                                    
    {
    INT status = SUCCESS;
    INT found_operator = FALSE, found;
    INT ssize = 0;
    CHAR *start_prompt, *end_prompt;
    CHAR *start_operator, *end_operator;
    CHAR prompt[STMAX], prompt_operator[STMAX];
    SUB_VAR_STRUCT *ns, *cs;
    
    strcpy (prompt_operator, "");
    
    /* parse out the prompt string (skip the leading "<<")                          */
    
    start_prompt = sub_variable + 2;
    end_prompt = ts_find_char (start_prompt, '|');
    
    if (*end_prompt == NUL)
        {
        end_prompt = (ts_find_char (start_prompt, '>') - 1);
        }
    else
        {
        found_operator = TRUE;
        start_operator = end_prompt + 1;
        end_operator = (ts_find_char (start_operator, '>') - 1);
        end_prompt--;
        }
    /* move the prompt and operator (if any) substrings to clean strings            */
    
    ts_copy_substring (prompt, start_prompt, end_prompt);
    if (found_operator)
        {
        ts_copy_substring (prompt_operator, start_operator, end_operator);
        }
    /* lookup the complete string                                                   */
    
    for (found = FALSE, cs = *sub_vars;
         !found && *nvars > 0 && cs != (SUB_VAR_STRUCT *)0; cs = cs->next)
        {
        if (strcmp (sub_variable, cs->sub_variable) == 0)
            found = TRUE;
        }
    if (!found)
        {
        /* complete string not seen yet, so lookup the prompt string                */
        for (found = FALSE, cs = *sub_vars;
             !found && *nvars > 0 && cs != (SUB_VAR_STRUCT *)0; cs = cs->next)
            {
            if (strcmp (prompt, cs->prompt) == 0)
                found = TRUE;
            }
        if (!found)
            {
            /* this is a new substitution variable, so create and add to list       */
            
            ns = (SUB_VAR_STRUCT *) tc_alloc (sizeof(SUB_VAR_STRUCT));
            if (ns == (SUB_VAR_STRUCT *)0)
                return FAIL;
            ns->next = (SUB_VAR_STRUCT *)0;
            
            if (*nvars == 0)
                {
                /* set return pointer to first structure                            */
                *sub_vars = ns;
                }
            else
                {
                /* point last structure to this new one                             */
                for (cs = *sub_vars; cs->next != (SUB_VAR_STRUCT *)0; cs = cs->next)
                    ;
                
                cs->next = ns;
                }
            (*nvars)++;
            strcpy (ns->sub_variable, sub_variable);
            strcpy (ns->prompt, prompt);
            strcpy (ns->prompt_operator, prompt_operator);
            strcpy (ns->response, "");
            }
        }
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    See above.                                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    extern INT db_sql_parms_parse(CHAR *sql_phrase, INT *nvars,                   */
/*                              SUB_VAR_STRUCT **sub_vars );                        */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) sql_phrase  -(CHAR *) The phrase to parse.                                */
/*    (O) nvars       -(INT *) The number of variables found.                       */
/*    (O) sub_vars    -(SUB_VAR_STRUCT  **) Pointer to list of structures built.    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion                                               */
/*    Function returns status of db_zz_parse_variable.                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
extern INT db_sql_parms_parse (CHAR *sql_phrase, INT *nvars, SUB_VAR_STRUCT **sub_vars)
#else
extern INT db_sql_parms_parse (sql_phrase, nvars, sub_vars)
CHAR *sql_phrase;
INT *nvars;
SUB_VAR_STRUCT **sub_vars;
#endif
    {
    
    INT status = SUCCESS;
    CHAR sub_variable[STMAX];       /* current scratch copy of the <<...>> string   */
    CHAR *start;                    /* points to 1st char of the <<...>> string     */
    CHAR *end;                      /* points to last char of the <<...>> string    */
    CHAR *start_prompt;             /* points to 1st char of current prompt string  */
    CHAR *end_prompt;               /* points to last char of current prompt string */
    CHAR *p;                        /* current char position in sql phrase          */
    SUB_VAR_STRUCT *ns = 0;         /* new structure                                */
    SUB_VAR_STRUCT *cs = 0;         /* current structure                            */
    INT variable_open = FALSE;      /* currently processing inside  <<...>>         */
    INT operator_open = FALSE;      /* currently processing inside |...>>           */
    
    *nvars = 0;
    *sub_vars = (SUB_VAR_STRUCT *)0;
    
    if (sql_phrase == (CHAR *)0)
        return SUCCESS;
    p = sql_phrase;
    while (status == SUCCESS)
        {
        start = p = ts_find_char (p, '<');
        if (*p != NUL)
            {
            /* validate intro of "<<"                                               */
            
            if (*(++p) != '<')
                continue;
            p = ts_find_char (p, '>');
            
            /* validate termination of ">>"                                         */
            
            if ((*p == NUL) || (*(++p) != '>'))
                continue;
            end = p;
            
            /* copy the well-formed variable string                                 */
            
            ts_copy_substring (sub_variable, start, end);
            
            /* now crack the complete string and update structure list as needed    */
            
            status = db_zz_parse_variable (sub_variable, nvars, sub_vars);
            }
        else
            break;
        
        }
    return status;
    }
/* END:                                                                             */
