/* DEC/CMS REPLACEMENT HISTORY, Element CI_CMDINTERP.C*/
/* *2     5-SEP-1989 10:14:39 GORDON "(SPR 200) JGG's Gulf changes (August trip)"*/
/* *1    19-JUN-1989 12:01:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CI_CMDINTERP.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


/* %F This module provides a reverse polish command interpreter
   for easy testing of code.  It can also be used as a hex, octal,
   decimal or floating point calculator.  With some extensions
   it could even become a prototyping language similar to FORTH.
   It is also useful for providing an easily customized and
   extensible command driven environment.  The Finder
   Centered Symbol System is one example.  Access from
   this language to 'C' or FORTRAN is accomplished by writing an "glue"
   routine that pops parameters off of the stacks and  then calls
   the appropriate routine. Since the entire system is written
   in 'C', we should expect none of the problems that have plagued
   INFO to 'C' glue routines.

   Commands are added to the vocabulary with ci_add_command().
   Strings can be interpreted using ci_intrepret().
   A conversational mode can be entered by calling ci_converse().
   Files containing commands can be loaded by calling ci_load().

   This system makes heavy use of the integer, double and
   pointer stacks in ps_param_stacks.  Direct reference to these
   stack variables are used so be sure to statically link these
   two modules.

   AUTHOR: Phil Burk

*/

#include "esi_c_lib.h"
#include "esi_ci.h"
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_PS_X
#include "esi_ps.x"
#endif

#ifndef ESI_CI_ERR_H
#include "esi_ci_err.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#define DEBUG FALSE

#define BLANK ' '

/* Declare command vocabulary structure. */
#define CI_MAX_NAME_LEN 24
#define CI_MAX_COMMANDS 64
static INT vocab_pointer;
static struct ci_vocab_struct
{
    CHAR name[CI_MAX_NAME_LEN];
    SHORT if_primary;
    SHORT if_immediate;
    VOID (*comm_func)();
    LONG body;
} ci_vocabulary[CI_MAX_COMMANDS] = { 0 };

/* Dictionary is in an allocated memory chunk. */
#define CI_DICT_MIN (INT)(1000)
static BYTE *ci_dict_base;
static INT ci_dict_pointer;
static INT ci_dict_size;

/* Declare TIB and TOKEN area. */
#define CI_MAX_TIB_NESTS 16
#define CI_TOKEN_MAX STMAX
#define CI_TIB_MAX STMAX
static struct tib_stack_struct
{
    CHAR string[CI_TOKEN_MAX];
    CHAR *cpos;
    FILE *read_from;
}   tib_stack[CI_MAX_TIB_NESTS];
static INT ci_tib_stack_pointer;

static CHAR ci_token[CI_TOKEN_MAX];
static CHAR ci_tib[CI_TIB_MAX];
static CHAR *ci_tib_ptr, *ci_token_ptr;
static FILE *ci_current_file;     /* File currently being compiled. */

/* Define offsets into dictionary for user variables. */
static LONG ci_number_base_offset;   /* 10 for decimal, 16 for hex, etc. */
static LONG ci_state_offset;

/* Declare rectangular array of characters for storing literal
   'C' strings.  This is to support c" */
#define CI_NUM_CSTRINGS 16
static CHAR ci_cstrings[CI_NUM_CSTRINGS][STMAX];
static INT ci_cstring_next;

/* #define CI_MSG(str) { printf("%s\n", str); } */
#define CI_MSG(str) { wimsg(str); }
#define CI_BYE 2000

/* These routines will normally only be called from CI */
#if USE_PROTOTYPES

static VOID ci_include(void);
static VOID ci_base(void);
static INT ci_converse(void);
static VOID ci_octal(void);
static VOID ci_decimal(void);
static VOID ci_hex(void);
static VOID ci_fetch(void);
static VOID ci_store(void);
static VOID ci_number(void);
static VOID ci_word(void);
static VOID ci_execute(void);
static VOID ci_search(void);
static VOID ci_vlist(void);
static VOID ci_dots(void);
static VOID ci_hello(void);
static VOID ci_dot_quote(void);
static VOID ci_c_quote(void);
static VOID ci_puts(void);
static VOID ci_dup(void);
static VOID ci_over(void);
static VOID ci_dot(void);
static VOID ci_plus(void);
static VOID ci_minus(void);
static VOID ci_swap(void);
static VOID ci_rot(void);
static VOID ci_drop(void);

static VOID ci_fdup(void);
static VOID ci_fdot(void);
static VOID ci_fplus(void);
static VOID ci_fminus(void);
static VOID ci_fswap(void);
static VOID ci_fover(void);
static VOID ci_frot(void);
static VOID ci_fdrop(void);

static VOID ci_help(void);
static INT ci_getline(CHAR *);
static INT ci_string_to_int(CHAR *string, INT base, INT *num_ptr);
static INT ci_digit_to_int(INT digit, INT *num_ptr);
static INT ci_interpret(CHAR *string);

#else

static VOID ci_include();
static INT ci_converse();
static VOID ci_base();
static VOID ci_octal();
static VOID ci_decimal();
static VOID ci_hex();
static VOID ci_fetch();
static VOID ci_store();
static VOID ci_number();
static VOID ci_word();
static VOID ci_execute();
static VOID ci_search();
static VOID ci_vlist();
static VOID ci_dots();
static VOID ci_hello();
static VOID ci_dot_quote();
static VOID ci_c_quote();
static VOID ci_puts();
static VOID ci_dup();
static VOID ci_over();
static VOID ci_dot();
static VOID ci_plus();
static VOID ci_minus();
static VOID ci_swap();
static VOID ci_rot();
static VOID ci_drop();

static VOID ci_fdup();
static VOID ci_fdot();
static VOID ci_fplus();
static VOID ci_fminus();
static VOID ci_fswap();
static VOID ci_fover();
static VOID ci_frot();
static VOID ci_fdrop();

static VOID ci_help();
static INT ci_getline();
static INT ci_string_to_int();
static INT ci_digit_to_int();
static INT ci_interpret();

#endif

/* ---------------------------------------------------- */
/* The following CI functions provide an interactive debugging
environment - they have been placed in a conditional compilation
section to prevent unnecessary code from being part of the
Finder product JGG 30 Aug 89 */

#if DEBUG
publicdef ci_test()
{
    ps_init();
    ci_init();
    ci_converse();
    ci_term();
    ps_term();
}
/* ---------------------------------------------------- */
static VOID ci_vlist()
/* %S Print list of available commands. */
{
    INT i;

    CI_MSG("Available commands:");
    for(i=0; i<vocab_pointer; i++)
    {
	CI_MSG(ci_vocabulary[i].name);
    }
}


/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
static VOID ci_hello()
/* %S Say "hello". Useful for debugging. */
{
    CI_MSG("Hello out there!!");
}

/* ---------------------------------------------------- */
static VOID ci_dot_quote()
/* %S Print the following text. */
{
   INT ic;
   ic = '"';
   PS_LONG_PUSH(ic);
   ci_word();
   CI_MSG(ci_token);
}
/* ---------------------------------------------------- */
static VOID ci_puts()
/* %S Print the string on top of the pointer stack. */
/* %P ( <string> -p- , store and leave address ) */
{
    CI_MSG(PS_POINTER_POP);
}

/* ---------------------------------------------------- */
static VOID ci_dots()
/* %S Show the contents of the three stacks. */
/* %P ( -- )
*/
{
    CI_MSG("Stack Dump -----------");
    ps_long_view();
    ps_double_view();
    ps_pointer_view();
}

/* ---------------------------------------------------- */
/* -------- MISCELLANEOUS FUNCTIONS ------------------- */
/* ---------------------------------------------------- */
static VOID ci_help()
/* Display useful information for user. */
{
    CI_MSG("This is a reverse polish command interpreter similar to FORTH.");
    CI_MSG("Multiple commands may be entered on one line.");
    CI_MSG("To see a list of commands, enter ' VLIST '.");
    CI_MSG("To load a file, enter ' INCLUDE filename '.");
    CI_MSG("To add two integers, enter ' 23 45 + . '.");
    CI_MSG("To add two floats, enter ' 23.197 45.221 f+ f. '.");
    CI_MSG("To exit, enter ' bye '.");
}

#endif	/*********** END OF DEBUGGING SECTION ***********/

/* ---------------------------------------------------- */
static VOID ci_number()
/* %S Convert a token to a floating point or an integer number. */
/* %P ( token -p- )
      ( -i- FALSE ) or ( num -i- 1 ) or ( fnum -f- , -i- 2 )
*/
{
    CHAR *string;
    INT ival;
    LONG num_base;

    string = (CHAR *)PS_POINTER_POP;

/* Get base of number. */
    PS_LONG_PUSH(ci_number_base_offset);
    ci_fetch();
    num_base = PS_LONG_POP;

/* Check for presence of decimal point. */
    if((strchr((char *)string, '.') EQUALS NULL)
       && (ci_string_to_int(string, num_base, &ival) EQUALS SUCCESS))
    {
	PS_LONG_PUSH(ival);
	PS_LONG_PUSH(1);
    }
    else
    {
	if(ts_isfloat(string) > 0)
	{
	    PS_DOUBLE_PUSH(atof((char *)string));
	    PS_LONG_PUSH(2);
	}
	else
	{
	    PS_LONG_PUSH(0);
	}
    }
}
/* ---------------------------------------------------- */
static INT ci_string_to_int(string, base, num_ptr)
/* %S Convert an ascii string to an integer if valid. */
/* %P Returns SUCCESS or FAIL. */
CHAR *string;  /* %P String containing number left justified. */
INT base;      /* %P Base of the number system, eg. 10 or 16 */
INT *num_ptr;  /* %P Result. */
{
    INT sign, accum;
    LONG ival;

    if(IS_EMPTY_STRING(string))
    {
	return(CI_STRING_TO_INT_FAIL);
    }

/* Check for minus sign. */
    if(string[0] EQUALS '-')
    {
	sign = -1;
	string++;
    }
    else
    {
	sign = 1;
    }

/* Convert characters in base until none left. */
    accum = 0;
    while(*string != 0)
    {
	if(ci_digit_to_int(*string, &ival) EQUALS SUCCESS)
	{
	    if(ival < base)
	    {
		accum = accum * base + ival;
	    }
	    else
	    {
		return(CI_STRING_TO_INT_FAIL);
	    }
	}
	else
	{
	    return(CI_STRING_TO_INT_FAIL);
	}
	string++;
    }

/* Apply sign. */
    *num_ptr = accum * sign;
    return(SUCCESS);
}
/* ---------------------------------------------------- */
static INT ci_digit_to_int(digit, num_ptr)
/* %S Convert a single digit to a numeric value. */
/* %P Returns SUCCESS or FAIL. */
INT digit;   /* %P ASCII digit, can be 0-9, A-Z. */
INT *num_ptr; /* %P Value of digit. */
{
    if((digit >= '0') && (digit <= '9'))
    {
	*num_ptr = digit - '0';
	return(SUCCESS);
    }

    if((digit >= 'a') && (digit <= 'z'))
    {
	*num_ptr = digit - 'a' + 10;
	return(SUCCESS);
    }

    if((digit >= 'A') && (digit <= 'Z'))
    {
	*num_ptr = digit - 'A' + 10;
	return(SUCCESS);
    }

    return(CI_DIGIT_TO_INT_FAIL);
}

/* ---------------------------------------------------- */
static INT ci_converse()
/* %S Carry out an interpretive session. */
{
    CHAR linein[STMAX];

/* Header. */
    if(ci_current_file EQUALS (FILE *) 0)
    {
	CI_MSG("Command Interp V1.3");
    }
    do
    {
	ps_check_stacks();
	if(ci_getline(linein) != SUCCESS)
	{
	    return(SUCCESS);
	}
    }
    while(ci_interpret(linein) != CI_BYE);
    return(SUCCESS);
}
/* ---------------------------------------------------- */
static INT ci_getline(linein)
/* %S Get line from current input device. */
CHAR *linein;
{
     CHAR *cp;

     if (ci_current_file == (FILE *)0)
     {
	printf("\n%d:%d:%d> ",
	       ps_long_many(),
	       ps_double_many(),
	       ps_pointer_many());
	gets((char *)linein);
     }
     else
     {
	 cp = fgets((char *)linein, CI_TIB_MAX, ci_current_file);
	 if (cp EQUALS NULL)
	 {
	     fclose(ci_current_file);
	     ci_tib_stack_pointer++;
	     ci_current_file = tib_stack[ci_tib_stack_pointer].read_from;
	     ci_tib_ptr = tib_stack[ci_tib_stack_pointer].cpos;
	     strcpy((char *)ci_tib,
		(char *)&tib_stack[ci_tib_stack_pointer].string[0]);
	     return(CI_GETLINE_FAIL);
	 }
     }
     return(SUCCESS);
}

/* ---------------------------------------------------- */

static INT ci_interpret(string)
CHAR *string;
/* %S Interpret the commands in a string. */
{

/* Copy string into TIB. */
    if(strlen((char *)string) <= sizeof ci_tib)
    {
	strcpy((char *)ci_tib, (char *)string);
	ci_tib_ptr = ci_tib;
    }
    else
    {
	return(CI_STRING_TOO_BIG); /* %Q */
    }

/* Process words in TIB */
    PS_LONG_PUSH(BLANK);
    ci_word();
    while(IS_STRING((char *)ci_token))
    {
/* Scan vocabulary. */
#if DEBUG
	CI_MSG(ci_token);
#endif
	PS_POINTER_PUSH((BYTE *)ci_token);
	ci_search();

/* Execute if found. */
	if(PS_LONG_POP EQUALS TRUE)
	{
/* Check for termination. */
	    if(PS_LONG_TOS EQUALS 0)
	    {
#if DEBUG
    CI_MSG("CI_INTERPRET ,  BYE detected.");
#endif
		return(CI_BYE);
	    }
	    else
	    {
		ci_execute();
	    }
	 }
	 else /* NOT FOUND */
	 {
/* Try to convert to a number. */
	     PS_POINTER_PUSH((BYTE *)ci_token);
	     ci_number();
	     if(PS_LONG_POP EQUALS FALSE)
	     {
		 return(CI_COMMAND_NOT_RECOGNIZED);
	     }
	}
	 PS_LONG_PUSH(BLANK);
	 ci_word();
     }
}

/* ---------------------------------------------------- */
publicdef INT ci_init()
/* %S Initialize Command Interpretation System. */
{

/* Declare initial dictionary space. */
    ci_dict_base = (BYTE *)tc_alloc(CI_DICT_MIN);
    if(ci_dict_base EQUALS (BYTE *)NULL)
    {
	return(CI_BAD_ALLOCATION);
    }
    ci_dict_pointer = 0;
    ci_dict_size = CI_DICT_MIN;
    ci_tib_stack_pointer = 0;
    ci_current_file = (FILE *) 0; /* Interactive first. */
    ci_cstring_next = 0;

/* Set globals */
    vocab_pointer = 0;

/* Set offsets for some user variables in dictionary. */
    ci_number_base_offset = ci_dict_pointer;
    ci_dict_pointer += sizeof(INT);
    ci_state_offset = ci_dict_pointer;
    ci_dict_pointer += sizeof(INT);

/* Set values of user variables. */
    ps_long_push(10);   /* Decimal. */
    ps_long_push(ci_number_base_offset);
    ci_store();
    ps_long_push(FALSE);
    ps_long_push(ci_state_offset);
    ci_store();

/* Build kernal. */
    ci_add_command("bye", (VOID (*)()) 0);
    ci_add_command("include", ci_include);
    ci_add_command("word", ci_word);
    ci_add_command("search", ci_search);
    ci_add_command("execute", ci_execute);
    ci_add_command("base", ci_base);
    ci_add_command("octal", ci_octal);
    ci_add_command("decimal", ci_decimal);
    ci_add_command("hex", ci_hex);
    ci_add_command("@", ci_fetch);
    ci_add_command("!", ci_store);
    ci_add_command("c\"", ci_c_quote);

    ci_add_command(".", ci_dot);
    ci_add_command("+", ci_plus);
    ci_add_command("-", ci_minus);
    ci_add_command("dup", ci_dup);
    ci_add_command("swap", ci_swap);
    ci_add_command("over", ci_over);
    ci_add_command("rot", ci_rot);
    ci_add_command("drop", ci_drop);

    ci_add_command("f.", ci_fdot);
    ci_add_command("f+", ci_fplus);
    ci_add_command("f-", ci_fminus);
    ci_add_command("fdup", ci_fdup);
    ci_add_command("fswap", ci_fswap);
    ci_add_command("fover", ci_fover);
    ci_add_command("frot", ci_frot);
    ci_add_command("fdrop", ci_fdrop);

#if DEBUG
    ci_add_command("puts", ci_puts);
    ci_add_command(".\"", ci_dot_quote);
    ci_add_command("hello", ci_hello);
    ci_add_command(".s", ci_dots);
    ci_add_command("help", ci_help);
    ci_add_command("vlist", ci_vlist);
#endif

    return(SUCCESS);
}
/* ---------------------------------------------------- */
publicdef INT ci_term()
{
    return(SUCCESS);
}
/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
static VOID ci_word()
/* %S Scan TIB for delimited word, store in token. */
/* %P ( delimiter -i- )
*/
{
    CHAR delimiter;

/* Get delimiter off stack. */
    delimiter = (CHAR) PS_LONG_POP;

/* Remove leading delimiters. */
    while(*ci_tib_ptr EQUALS delimiter)
    {
	ci_tib_ptr++;
    }

/* Copy into token. */
    ci_token_ptr = ci_token;
    while((*ci_tib_ptr != delimiter) && (*ci_tib_ptr != 0)
	  && (*ci_tib_ptr != '\n'))
    {
	*ci_token_ptr++ = *ci_tib_ptr++;
    }
    *ci_token_ptr = 0;
    if(*ci_tib_ptr EQUALS delimiter)
    {
	ci_tib_ptr++;  /* Move past delimiter. */
    }
}
/* ---------------------------------------------------- */
static VOID ci_search()
/* %S Search for a token in the vocabulary. */
/* %P ( token -p- )
      ( -i- index if_found)
*/
{
    LONG i;
    CHAR *token_ptr;

/* Scan vocabulary for match. */
    token_ptr = ts_sto_lower((CHAR *)PS_POINTER_POP);
    for(i = vocab_pointer-1; i >= 0; i--)
    {
	if(ARE_SAME((char *)ci_vocabulary[i].name, (char *)token_ptr))
	{
	    PS_LONG_PUSH(i);
	    PS_LONG_PUSH((LONG)TRUE);
	    return;
	}
    }

/* Not found! */
    PS_LONG_PUSH(FALSE);
    return;
}

/* ---------------------------------------------------- */
static VOID ci_execute()
/* %S Execute a command based on the command index. */
/* %P ( index -i- )
*/
{
    (*(ci_vocabulary[PS_LONG_POP].comm_func))();
}

/* ---------------------------------------------------- */
INT ci_add_command(comname, func_ptr)
/* %S Add a command to the vocabulary. */
/* %P Return token_index. */
CHAR comname[];   /* %P Name of command. */
VOID (*func_ptr)();
{
    if(vocab_pointer < CI_MAX_COMMANDS)
    {
	strcpy((char *)ci_vocabulary[vocab_pointer].name, comname);
	ci_vocabulary[vocab_pointer].comm_func = func_ptr;
	ci_vocabulary[vocab_pointer].if_primary = TRUE;
	ci_vocabulary[vocab_pointer].if_immediate = FALSE;
	ci_vocabulary[vocab_pointer].body = 0;
	vocab_pointer++;
	return(vocab_pointer-1);
    }
    else
    {
	return(CI_VOCAB_OVERFLOW);
    }
}

/* ---------------------------------------------------- */
static INT ci_open_source(filename)
/* %S Include source from file in input stream. */
FILENAME filename;
{
    FILE *fptr;

    fptr = ho_open((char *)filename, "r", NULL );
    if (fptr EQUALS (FILE *)0)
    {
	return(CI_FILE_NOT_FOUND);
    }

/* Push current input environment onto tib stack. */
/* %Q Needs overflow check. */
    tib_stack[ci_tib_stack_pointer].read_from = ci_current_file;
    ci_current_file = fptr;
    tib_stack[ci_tib_stack_pointer].cpos = ci_tib_ptr;
    strcpy((char *)&tib_stack[ci_tib_stack_pointer].string[0], (char *)ci_tib);
    ci_tib_stack_pointer--;
    return(SUCCESS);
}

/* ---------------------------------------------------- */
publicdef INT ci_load(filename)
/* Read source from a file. */
FILENAME filename;
{
    if(ci_open_source(filename) == SUCCESS)
    {
	ci_converse();
	return(SUCCESS);
    }
    else
    {
	return(CI_LOAD_FILE_FAILURE);
    }
}

/* ---------------------------------------------------- */
static VOID ci_include()
/* %S Load file whose name follows, eg. " include file1" */
{
    PS_LONG_PUSH(BLANK);
    ci_word();
    ci_load(ci_token);
}

/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
static VOID ci_base()
/* %S Place offset of number base variable on integer stack. */
/* %P ( -i- base ) */
{
    PS_LONG_PUSH(ci_number_base_offset);
}

/* ---------------------------------------------------- */
static VOID ci_decimal()
/* %S Set numeric base to decimal. */
/* %P ( -- ) */
{
    PS_LONG_PUSH(10);
    ci_base();
    ci_store();
}


/* ---------------------------------------------------- */
static VOID ci_hex()
/* %S Set numeric base to decimal. */
/* %P ( -- ) */
{
    PS_LONG_PUSH(16);
    ci_base();
    ci_store();
}

/* ---------------------------------------------------- */
static VOID ci_octal()
/* %S Set numeric base to octal. */
/* %P ( -- ) */
{
    PS_LONG_PUSH(8);
    ci_base();
    ci_store();
}

/* ---------------------------------------------------- */
static VOID ci_fetch()
/* %S Fetch integer value from address. */
/* %P ( addr -i- value ) */
{
    LONG i;
    LONG offset;

    offset = PS_LONG_POP;
    i = *((LONG *) (ci_dict_base + offset));
    PS_LONG_PUSH(i);
}

/* ---------------------------------------------------- */
static VOID ci_store()
/* %S Store integer value at address. */
/* %P ( value addr -i-  ) */
{
    LONG i, offset;
    offset = PS_LONG_POP;
    i = PS_LONG_POP;
    *((LONG *) (ci_dict_base + offset)) = i;
}

/* ---------------------------------------------------- */
static VOID ci_abs_store()
/* %S Store integer value at address. */
/* %P ( addr -p- , value -i- ) */
{
    *((LONG *) PS_POINTER_POP) = PS_LONG_POP;
}

/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
/* ---------------------------------------------------- */
static VOID ci_c_quote()
/* %S Generate address for the following text. */
{
   INT ic;
   ic = '"';
   PS_LONG_PUSH(ic);
   ci_word();
   strcpy(&ci_cstrings[ci_cstring_next][0], ci_token);
   PS_POINTER_PUSH((BYTE *) &ci_cstrings[ci_cstring_next][0]);
   ci_cstring_next++;
   if (ci_cstring_next >= CI_NUM_CSTRINGS)
   {
       ci_cstring_next = 0;
   }
}

/* ===== INTEGER STACK SUPPORT ======= */
/* ---------------------------------------------------- */
static VOID ci_dup()
/* %S Duplicate top of long stack. */
/* %P ( n -i- n n )
*/
{
    PS_LONG_PUSH(PS_LONG_TOS);
}

/* ---------------------------------------------------- */
static VOID ci_plus()
/* %S Add top two values on integer stack. */
/* %P ( a b -i- a+b )
*/
{
    LONG ia, ib;
    ib = PS_LONG_POP;
    ia = PS_LONG_POP;
    PS_LONG_PUSH(ia + ib);
}

/* ---------------------------------------------------- */
static VOID ci_dot()
/* %S Print top value on Integer Stack. */
/* %P ( num -i- )
*/
{
    LONG num_base;
    ci_base();
    ci_fetch();
    num_base = PS_LONG_POP;

    switch(num_base)
    {
    case 16:
	printf("%x ", PS_LONG_POP);   /* %Q */
	break;

    case 8:
	printf("%o ", PS_LONG_POP);   /* %Q */
	break;

    default:
	printf("%d ", PS_LONG_POP);   /* %Q */
    }
}
/* ---------------------------------------------------- */
static VOID ci_minus()
/* %S Subtract top two values on integer stack. */
/* %P ( a b -i- a-b )
*/
{
   LONG ia, ib;
   ib = PS_LONG_POP;
   ia = PS_LONG_POP;
   PS_LONG_PUSH(ia - ib);
}
/* ---------------------------------------------------- */
static VOID ci_swap()
/* Swap top two values on integer stack. */
/* %P ( a b -i- b a )
*/
{
    LONG i1, i2;
    i1 = PS_LONG_POP;
    i2 = PS_LONG_POP;
    PS_LONG_PUSH(i1);
    PS_LONG_PUSH(i2);
}
/* ---------------------------------------------------- */
static VOID ci_over()
/* Get second item on integer stack. */
/* %P ( a b -i- a b a )
*/
{
    LONG ia, ib;
    ib = PS_LONG_POP;
    ia = PS_LONG_TOS;
    PS_LONG_PUSH(ib);
    PS_LONG_PUSH(ia);
}

/* ---------------------------------------------------- */
static VOID ci_rot()
/* %S Rotate top two values on integer stack. */
/* %P ( a b c -i- b c a )
*/
{
    LONG ia, ib, ic;
    ic = PS_LONG_POP;
    ib = PS_LONG_POP;
    ia = PS_LONG_POP;
    PS_LONG_PUSH(ib);
    PS_LONG_PUSH(ic);
    PS_LONG_PUSH(ia);
}

/* ---------------------------------------------------- */
static VOID ci_drop()
/* %S Drop top of integer stack. */
/* %P ( num -i- ) */
{
    PS_LONG_POP;
}

/* ===== FLOATING POINT STACK SUPPORT ======= */
/* ---------------------------------------------------- */
static VOID ci_fdup()
/* %S Duplicate top of long stack. */
/* %P ( n -i- n n )
*/
{
    PS_DOUBLE_PUSH(PS_DOUBLE_TOS);
}

/* ---------------------------------------------------- */
static VOID ci_fplus()
/* %S Add top two values on floating point stack. */
/* %P ( a b -i- a+b )
*/
{
    DOUBLE fa, fb;
    fb = PS_DOUBLE_POP;
    fa = PS_DOUBLE_POP;
    PS_DOUBLE_PUSH(fa + fb);
}

/* ---------------------------------------------------- */
static VOID ci_fdot()
/* %S Print top value on floating point Stack. */
/* %P ( num -i- )
*/
{
    printf("%f ", PS_DOUBLE_POP);   /* %Q */
}
/* ---------------------------------------------------- */
static VOID ci_fminus()
/* %S Subtract top two values on floating point stack. */
/* %P ( a b -i- a-b )
*/
{
   DOUBLE fa, fb;
   fb = PS_DOUBLE_POP;
   fa = PS_DOUBLE_POP;
   PS_DOUBLE_PUSH(fa - fb);
}
/* ---------------------------------------------------- */
static VOID ci_fswap()
/* Swap top two values on floating point stack. */
/* %P ( a b -i- b a )
*/
{
    DOUBLE fa, fb;
    fb = PS_DOUBLE_POP;
    fa = PS_DOUBLE_POP;
    PS_DOUBLE_PUSH(fb);
    PS_DOUBLE_PUSH(fa);
}
/* ---------------------------------------------------- */
static VOID ci_fover()
/* Get second item on float stack. */
/* %P ( a b -i- a b a )
*/
{
    DOUBLE fa, fb;
    fb = PS_DOUBLE_POP;
    fa = PS_DOUBLE_TOS;
    PS_DOUBLE_PUSH(fb);
    PS_DOUBLE_PUSH(fa);
}

/* ---------------------------------------------------- */
static VOID ci_frot()
/* %S Rotate top two values on floating point stack. */
/* %P ( a b c -i- b c a )
*/
{
    DOUBLE fa, fb, fc;
    fc = PS_DOUBLE_POP;
    fb = PS_DOUBLE_POP;
    fa = PS_DOUBLE_POP;
    PS_DOUBLE_PUSH(fb);
    PS_DOUBLE_PUSH(fc);
    PS_DOUBLE_PUSH(fa);
}

/* ---------------------------------------------------- */
static VOID ci_fdrop()
/* %S Drop top of floating point stack. */
/* %P ( num -i- ) */
{
    PS_DOUBLE_POP;
}

/* END-CODE */
                                                                                   
