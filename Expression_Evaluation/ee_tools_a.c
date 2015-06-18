/* DEC/CMS REPLACEMENT HISTORY, Element EE_TOOLS_A.C*/
/* *6    14-AUG-1991 14:01:54 JULIAN "(SPR 1679) added more cases for unary operator"*/
/* *5     2-AUG-1991 16:58:27 MING "(SPR 0) fix expression syntax with 2 adjacent constants"*/
/* *4    14-AUG-1990 13:11:43 VINCE "(SPR 5644) Header Standardization"*/
/* *3     6-JUN-1990 20:07:19 MING "(SPR 0) chang tmpnam to ho_get_unique_filename"*/
/* *2    10-OCT-1989 13:00:16 GORDON "(SPR 4999) Call tmpnam, not ho_get_unique_filename"*/
/* *1    19-JUN-1989 12:49:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element EE_TOOLS_A.C*/
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

#include "esi_ctype.h"
#include "esi_c_lib.h"
#include "esi_ee.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_EE_ERR_H

#include "esi_ee_err.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

#if USE_PROTOTYPES
static INT ee_icp (CHAR *operator);
static INT ee_isp (CHAR *operator);
static CHAR *ee_next_token (CHAR *expression, CHAR **token, INT *type,
                                INT *status);
static INT ee_pop (CHAR **stack, INT *top, CHAR **token);
static INT ee_push (CHAR **stack, INT *top, CHAR *token);

#else
static INT ee_icp ();
static INT ee_isp ();
static CHAR *ee_next_token ();
static INT ee_pop ();
static INT ee_push ();

#endif
static CHAR op_string[] = 
    {
    "()^*/+-="
    };

/* allowed operators */

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ee_evaluator(CHAR *exp_in, CHAR **value);

    publicdef INT ee_parser(CHAR *exp_in, CHAR **exp_out);

Private_Functions:
    static INT ee_icp(CHAR *operator);
    
    static INT ee_isp(CHAR *operator);
    
    static CHAR *ee_next_token(CHAR *expression, CHAR **token, INT *type,
       INT *status);

    static INT ee_pop(CHAR **stack, INT *top, CHAR **token);
    
    static INT ee_push(CHAR **stack, INT *top, CHAR *token);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ee_parser(CHAR *exp_in, CHAR **exp_out);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) exp_in      -(CHAR *) Input expression.
    (O) exp_out     -(CHAR **) Expression allocated and returned.

Return Value/Status:
    SUCCESS - Successful completion.
    EE_INVALID_INPUT
    EE_UNMATCHED_PAREN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ee_parser (CHAR *exp_in, CHAR **exp_out)
#else
publicdef INT ee_parser (exp_in, exp_out)
CHAR *exp_in;
CHAR **exp_out;
#endif
    {
    CHAR *stack[MAX_DEPTH + 1];     /* tcp of operands on stack */
    INT top;                        /* top of stack counter */
    CHAR *e;                        /* remainder of expression from next_token */
    CHAR last_token[80];
    CHAR temp_str[80];
    CHAR *x;                        /* token holder */
    CHAR *y;                        /* token holder */
    CHAR *ptr;
    INT type,prev_type;             /* type of token - operator, operand, const */
    INT status;
    INT first;                      /* flag for checking the unary operator */
    
    /* allocate expression as input length + extra */
    /* (separators)                                */
    ptr = *exp_out = (CHAR *)tc_alloc ((strlen (exp_in) + 100));
    ptr[0] = 0;
    stack[0] = (CHAR *)0;
    top = 0;
    
    /* get assignment (result) variable (file      */
    /* name)                                       */
    e = ee_next_token (exp_in, &x, &type, &status);
    if (type != VARIABLE_TYPE)
        return EE_INVALID_INPUT;
    
    /* if (x == a valid file name) continue        */
    sprintf (*exp_out, "%s%s ", *exp_out, x);
    tc_free (x);
    
    /* get assignment operand and put it on the    */
    /* stack this algorithm requires something on  */
    /* the stack with an isp = -1 to work          */
    e = ee_next_token (e, &x, &type, &status);
    if (strcmp (x, "=") != 0)
        {
        tc_free (x);
        return EE_INVALID_INPUT;
        }
    ee_push (stack, &top, x);
    /* copy token to last_token - used to find     */
    /* unary -                                     */
    strcpy (last_token, x);
    tc_free (x);
    
    /* now get the first token following '=' and   */
    /* start loop until no_more_tokens             */
    e = ee_next_token (e, &x, &type, &status);
    if (status != SUCCESS)
        {
        tc_free (x);
        return status;
        }

    first = TRUE;    /* flag for checking the unary operator, if it's first,
			and it hit - sign, it means - sign is a unary operator */
		 
    /* while there are more tokens                 */
    while (x != (CHAR *)0)
        {
        if ((strcmp(x,"-") == 0 AND 
	    ((strcmp(last_token,"(") == 0) OR (strcmp(last_token,"=") == 0)))
	    OR
	    (strcmp(x,"-") == 0 AND (prev_type == OPERATOR_TYPE OR first)))
            {
            /* this should be unary - get another token    */
            /* since that will be the constant part        */
            e = ee_next_token (e, &x, &type, &status);
            if (type != CONSTANT_TYPE)
                return EE_INVALID_INPUT;
            sprintf (temp_str, "-%s", x);
            tc_free (x);
            x = STRING_ALLOCATE (temp_str);
            strcpy (x, temp_str);
            }
        if (type == VARIABLE_TYPE || type == CONSTANT_TYPE)
            {
            /* write it with separator                     */
            sprintf (*exp_out, "%s%s ", *exp_out, x);
            }
        else if (strcmp (x, ")") == 0)
            {
            /* pop off stack until matching '(' found.     */
            /* return if error                             */
            while (strcmp (stack[top], "(") != 0)
                {
                status = ee_pop (stack, &top, &y);
                if (y == (CHAR *)0)
                    {
                    return(EE_UNMATCHED_PAREN);
                    }
                /* add operands to output                      */
                sprintf (*exp_out, "%s%s ", *exp_out, y);
                tc_free (y);
                }
            /* get rid of '('                              */
            status = ee_pop (stack, &top, &y);
            tc_free (y);
            }
        else
            {
            /* now pop operators until one of lower        */
            /* priority found                              */
            while (ee_isp (stack[top]) >= ee_icp (x))
                {
                ee_pop (stack, &top, &y);
                sprintf (*exp_out, "%s%s ", *exp_out, y);
                tc_free (y);
                }
            /* push the current operator back on the stack */
            ee_push (stack, &top, x);
            }
        /* continue loop with next token               */
        strcpy (last_token, x);
	prev_type = type;
        tc_free (x);

        e = ee_next_token (e, &x, &type, &status);
        if (status != SUCCESS)
            {
            tc_free (x);
            return status;
            }
	if(first) first = FALSE;
        }
    /* no more tokens - pop operators until stack  */
    /* empty                                       */
    
    while (top > 0)
        {
        ee_pop (stack, &top, &y);
        sprintf (*exp_out, "%s%s ", *exp_out, y);
        tc_free (y);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static CHAR *ee_next_token(CHAR *expression, CHAR **token, INT *type, INT *status);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) expression  -(CHAR *) Input expression.
    (O) token       -(CHAR **) Token returned.
    (O) type        -(INT *) Type of token operator, etc.
    (O) status      -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns the value of returned_string.
    
Scope:
    PRIVATE to <ee_tools_a.c>

Limitations/Assumptions:
    This should be redone to allow better error 
    checking return a token then return the     
    pointer to the remainder of the string.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static CHAR *ee_next_token (CHAR *expression, CHAR **token, INT *type, INT *status)
#else
static CHAR *ee_next_token (expression, token, type, status)
CHAR *expression;
CHAR **token;
INT *type;
INT *status;
#endif
    {
    INT position;
    CHAR *returned_string;          /* whats left after token extraction           */
    CHAR *ptr;
    CHAR first_char[2];
    
    /* remove leading blanks */
    expression = ts_snowhite(expression);

    /* see if there are more tokens in string      */
    if (expression[0] == 0)
        {
        *token = (CHAR *)NULL;
        returned_string = NULL;
        *status = SUCCESS;
        return returned_string;
        }
    /* check to see if first character is an       */
    /* operator                                    */
    first_char[0] = expression[0];
    first_char[1] = '\0';
    if (strpbrk (op_string, first_char) != NULL)
        {
        
        /* return string of operator                   */
        *token = (CHAR *)tc_alloc (sizeof(CHAR) * 2);
        strcpy (*token, first_char);
        *type = OPERATOR_TYPE;
        *status = SUCCESS;
        returned_string = expression + 1;
        return returned_string;
        }
    /* get the next token - based on next operator */
    /* or end of string                            */
    position = strcspn (expression, op_string);
    ptr = *token = (CHAR *)tc_alloc (sizeof(CHAR) * (position + 1));
    strncpy (*token, expression, position);
    ptr[position] = '\0';
    /* check to see if it is operand or constant.
       although for the moment i dont know why we
       want to do that here - it's in the design   */
    
    if (ts_isfloat (*token) > 0)
        {
        *type = CONSTANT_TYPE;
        *status = SUCCESS;
        }
    else
        {
        *type = VARIABLE_TYPE;
        *status = SUCCESS;
        }
    /* point to remainder of string after token    */
    returned_string = expression + position;
    
    return returned_string;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Increment top of stack and put token in stack.

Prototype:
    static INT ee_push(CHAR *stack[], INT *ptop, CHAR *token);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    stack           -(CHAR *[])
    ptop            -(INT *)
    token           -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ee_tools_a.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ee_push (CHAR *stack[], INT *ptop, CHAR *token)
#else
static INT ee_push (stack, ptop, token)
CHAR *stack[];
INT *ptop;
CHAR *token;
#endif
    {
    
    (*ptop)++;
    if (*ptop > MAX_DEPTH)
        return EE_STACK_FULL;
    stack[*ptop] = STRING_ALLOCATE (token);
    strcpy ((char *)stack[*ptop], (char *)token);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT ee_pop(CHAR *stack[], INT *ptop, CHAR **token);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    stack           -(CHAR *[])
    ptop            -(INT *)
    token           -(CHAR **)

Return Value/Status:
    SUCCESS - Successful completion.
    EE_STACK_EMPTY
    
Scope:
    PRIVATE to <ee_tools_a.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ee_pop (CHAR *stack[], INT *ptop, CHAR **token)
#else
static INT ee_pop (stack, ptop, token)
CHAR *stack[];
INT *ptop;
CHAR **token;
#endif
    {
    
    if (*ptop == 0)
        {
        *token = NULL;
        return EE_STACK_EMPTY;
        }
    else
        {
        
        /* pull top of stack off and decrement top of  */
        /* stack                                       */
        *token = stack[*ptop];
        (*ptop)--;
        return SUCCESS;
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT ee_isp(CHAR *operator);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    operator        -(CHAR *)

Return Value/Status:
    Function returns the value of in_stack_prio[ strcspn(op_string, operator) ].
    
Scope:
    PRIVATE to <ee_tools_a.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ee_isp (CHAR *operator)
#else
static INT ee_isp (operator)
CHAR *operator;
#endif
    {
    static INT in_stack_prio[] = 
        {
        0,
        0,
        3,
        2,
        2,
        1,
        1,
        -1
        };
    INT value;
    
    /* determine in-stack-priority of  operator    */
    value = in_stack_prio[strcspn (op_string, operator)];
    return value;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT ee_icp(CHAR *operator);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    operator        -(CHAR *)

Return Value/Status:
    Function returns the value of in_coming_prio[ strcspn(op_string, operator) ].
    
Scope:
    PRIVATE to <ee_tools_a.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ee_icp (CHAR *operator)
#else
static INT ee_icp (operator)
CHAR *operator;
#endif
    {
    static INT in_coming_prio[] = 
        {
        4,
        0,
        4,
        2,
        2,
        1,
        1,
        0
        };
    INT value;
    
    /* determine incoming priority of operator     */
    value = in_coming_prio[strcspn (op_string, operator)];
    return value;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ee_evaluator(CHAR *exp_in, CHAR **value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) exp_in      -(CHAR *)
    (O) value       -(CHAR **)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ee_evaluator (CHAR *exp_in, CHAR **value)
#else
publicdef INT ee_evaluator (exp_in, value)
CHAR *exp_in;
CHAR **value;
#endif
    {
    INT status;
    CHAR token[80];
    CHAR *x;                        /* token holders */
    CHAR *y;
    CHAR *z;
    CHAR *e;
    CHAR *stack[MAX_DEPTH];
    INT type;
    INT top;
    INT ii;
    INT loop;
    FILENAME result;
    FILENAME temp_nlist_filename;
    FILENAME grid_filename;
    CHAR description[241];
    static CHAR directory[] = "ESI$GRIDS";
    DOUBLE temp_val, left_val, right_val;
    
    /* create the file name for the temporary nlist file */
    sprintf (temp_nlist_filename, "TEMP_NLIST_%d", ho_user_number ());
    ho_add_path ("ESI$SCRATCH", temp_nlist_filename, sizeof(FILENAME),
                 temp_nlist_filename);
    
    stack[0] = (CHAR *)0;
    top = 0;
    /* the operands are now separated by blanks so */
    /* ts_next_word pulls them off                 */
    e = ts_next_word (exp_in, token);
    if (ts_isfloat (token) >= 0)
        {
        type = CONSTANT_TYPE;
        }
    else if (isalpha (token[0]))
        {
        type = VARIABLE_TYPE;
        }
    else
        {
        type = OPERATOR_TYPE;
        }
    /* while there are more operands               */
    while (strlen (token) > 0)
        {
        if (type == VARIABLE_TYPE || type == CONSTANT_TYPE)
            {
            status = ee_push (stack, &top, token);
            }
        else
            {
            /* get both operands that go along with the    */
            /* operator found order IS dependent if using  */
            /* division or exponentiation                  */
            ee_pop (stack, &top, &z);
            ee_pop (stack, &top, &y);
            /* use a temp grid unless token is =           */
            
            if (strcmp (token, "=") == SUCCESS)
                {
                strcpy (result, y);
                }
            else
                ho_get_unique_filename (result);    /* let operating system make a temp name for the nlist id */
            
           if (ts_isfloat (y) >  0 && ts_isfloat (z))
	       {
	       sscanf (y, "%lf", &left_val);
	       sscanf (z, "%lf", &right_val);
	       switch (token[0])
	           {
               case '+':
	           temp_val = left_val + right_val;
	           break;
               case '-':
                   temp_val = left_val - right_val;
                   break;
               case '*':
                   temp_val = left_val * right_val;
                   break;
               case '/':
                   temp_val = left_val / right_val;
                   break;
               case '^':
                   temp_val = left_val;
                   for (loop = (INT)(right_val - 1.0); loop > 0; --loop)
                       {
                       temp_val = temp_val * left_val;
                       }
                   break;
               default:
                   printf ("invalid operator in evaluator\n");
		   break;
		   }
               sprintf (token,"%f",temp_val);
               status = ee_push (stack, &top, token);
	       }
            else
	       {
               status = ee_perform_operation (result, token, y, z, temp_nlist_filename);
               tc_free (y);
               tc_free (z);
               if (status IS_NOT_EQUAL_TO SUCCESS)
                   {
                   return status;
                   }
               ee_push (stack, &top, result);
	       }
            }
        e = ts_next_word (e, token);
        if (ts_isfloat (token) >= 0)
            {
            type = CONSTANT_TYPE;
            }
        else if (isalpha (token[0]))
            {
            type = VARIABLE_TYPE;
            }
        else
            {
            type = OPERATOR_TYPE;
            }
        }
    ee_pop (stack, &top, value);
    
    /* now clean up temporary grids  */
    nl_delete_nlist_file (temp_nlist_filename);
    
    return SUCCESS;
    }
/* END:     */
