/* DEC/CMS REPLACEMENT HISTORY, Element PS_PARM_STKS.C*/
/* *4    17-AUG-1990 22:12:11 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     1-NOV-1989 13:06:59 GILLESPIE "(SPR 1) Remove ps_init - make static variables self-initialize"*/
/* *2    19-SEP-1989 16:30:13 GILLESPIE "(SPR 102) Fix casting problems"*/
/* *1    19-JUN-1989 13:22:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element PS_PARM_STKS.C*/
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
#include "esi_ps.h"
#include "esi_ps_err.h"
publicdef INT ps_long_sp = -1;
publicdef INT ps_double_sp = -1;
publicdef INT ps_pointer_sp = -1;
publicdef LONG ps_long_stack[STACK_SIZE];
publicdef DOUBLE ps_double_stack[STACK_SIZE];
publicdef VOIDPTR ps_pointer_stack[STACK_SIZE];

static CHAR astring[STMAX];

/* Error detection and reporting macros. */
#define PS_CHECK_OVERFLOW(sp) { \
    if (sp >= STACK_SIZE) \
    { \
    ps_reset(); \
    return(PS_STACK_OVERFLOW); \
    } \
}
        
#define PS_CHECK_UNDERFLOW(sp) { \
    if (sp < -1) \
    { \
    ps_reset(); \
    } \
}

#define PS_CHECK_EMPTY(sp) { \
    if (sp < -1) \
    { \
    ps_reset(); \
    return(PS_STACK_EMPTY); \
    } \
}


/* Declare internal routines. */
static VOID ps_string_out ();

#ifdef TEST

/* File Description ---------------------------------------------------------
Overview:
    This module provides LIFO data structures (stacks) for
    'C's long, double, and character pointer data types.
    These stacks can be used for storing state, passing parameters,
    supporting command interpreters, etc.
    The stacks are implemented as arrays with the index increasing
    as the stack grows.   Overflows and underflows are detected and
    reported.
    The stack pointers point to the next empty location.

    I cannot determine if the file ESI_PS.H is REALLY meant to
    allow other routines to push and pop things collectively, or
    whether each routine was expected perform the action through this
    service.  The entire notion probably ought to be tossed, since
    it is difficult to follow at best, impossible to follow at worst.
    Called by CI_CMDINTERP and SDL... modules
    JGG 18 nov 87

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ps_test(void);
    
    publicdef INT ps_term(void);
    
    publicdef INT ps_check_stacks(void);
    
    publicdef INT ps_reset(void);
    
    publicdef INT ps_long_push(LONG some_value);
    
    publicdef LONG ps_long_pop(void);
    
    publicdef LONG ps_long_tos(void);
    
    publicdef INT ps_long_many(void);
    
    publicdef INT ps_long_view(void);
    
    publicdef INT ps_double_push(DOUBLE some_value);
    
    publicdef DOUBLE  ps_double_pop(void);
    
    publicdef DOUBLE  ps_double_tos(void);
    
    publicdef INT ps_double_many(void);
    
    publicdef INT ps_double_print(void);
    
    publicdef INT ps_double_view(void);
    
    publicdef INT ps_pointer_push(VOIDPTR some_value);
    
    publicdef VOIDPTR ps_pointer_pop(void);
    
    publicdef VOIDPTR ps_pointer_tos(void);
    
    publicdef INT ps_pointer_many(void);
    
    publicdef INT ps_pointer_print(void);
    
    publicdef INT ps_pointer_view(void);

Private_Functions:
    static VOID ps_string_out(CHAR *string);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Test the Parameter Stack code.  

Prototype:
    publicdef INT ps_test(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_test (void)
#else
publicdef INT ps_test ()
#endif
    {
    INT lval1, lval2, lval;
    DOUBLE dval, dval1, dval2;
    BYTE *pval;
    INT many, i;
    
    printf ("Test the parameter stack;1\n");
    
    /* Push some values. */
    lval1 = 111;
    lval2 = 222;
    ps_long_push (lval1);
    ps_long_push (lval2);
    
    /* Check count. */
    many = ps_long_many ();
    printf ("LONG MANY = %d\n", many);
    
    /* View the stack. */
    ps_long_view ();
    
    /* Pop the values. */
    for (i = 0; i < many; i++)
        {
        lval = ps_long_pop ();
        sprintf (astring, " lval = %d", lval);
        ps_string_out (astring);
        }
    /* Test DOUBLE stack. */
    /* Push some values. */
    dval1 = 111.111;
    dval2 = 1.234567;
    ps_double_push (dval1);
    ps_double_push (dval2);
    
    /* Check count. */
    many = ps_double_many ();
    printf ("DOUBLE MANY = %d\n", many);
    
    /* View the stack. */
    ps_double_view ();
    
    /* Pop the values. */
    for (i = 0; i < many; i++)
        {
        dval = ps_double_pop ();
        sprintf (astring, " dval = %f", dval);
        ps_string_out (astring);
        }
    /* Test pointer stack. */
    /* Push some values. */
    lval1 = 111;
    lval2 = 222;
    ps_pointer_push ((VOIDPTR) & lval1);
    ps_pointer_push ((VOIDPTR) & lval2);
    
    /* Check count. */
    many = ps_pointer_many ();
    printf ("LONG MANY = %d\n", many);
    
    /* View the stack. */
    ps_pointer_view ();
    
    /* Pop the values. */
    for (i = 0; i < many; i++)
        {
        pval = ps_pointer_pop ();
        sprintf (astring, " pval = %p", pval);
        ps_string_out (astring);
        }
    ps_term ();
    return(SUCCESS);
    }
/* END:     */

#endif

/* Function Description -----------------------------------------------------
Description:
    Terminate and cleanup the parameter stacks.  

Prototype:
    publicdef INT ps_term(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_term (void)
#else
publicdef INT ps_term ()
#endif
    {
    ps_check_stacks ();
    ps_reset ();
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Check for over or underflow on stacks.  

Prototype:
    publicdef INT ps_check_stacks();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_check_stacks (void)
#else
publicdef INT ps_check_stacks ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_long_sp);
    PS_CHECK_OVERFLOW (ps_long_sp);
    PS_CHECK_UNDERFLOW (ps_double_sp);
    PS_CHECK_OVERFLOW (ps_double_sp);
    PS_CHECK_UNDERFLOW (ps_pointer_sp);
    PS_CHECK_OVERFLOW (ps_pointer_sp);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Reset stack pointers.  

Prototype:
    publicdef INT ps_reset(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_reset (void)
#else
publicdef INT ps_reset ()
#endif
    {
    ps_long_sp = -1;
    ps_double_sp = -1;
    ps_pointer_sp = -1;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Push value onto long stack.  

Prototype:
    publicdef INT ps_long_push(LONG some_value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    some_value      -(LONG) Any long value. 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_long_push (LONG some_value)
#else
publicdef INT ps_long_push (some_value)
LONG some_value;
#endif
    {
    PS_CHECK_OVERFLOW (ps_long_sp);
    
    PS_LONG_PUSH (some_value);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    POP value off of long stack.  

Prototype:
    publicdef LONG ps_long_pop(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    RETURNS the top of long stack. 

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef LONG ps_long_pop (void)
#else
publicdef LONG ps_long_pop ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_long_sp);
    
    return(PS_LONG_POP);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Return value on top of long stack.  

Prototype:
    publicdef LONG ps_long_tos(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of PS_LONG_TOS.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef LONG ps_long_tos (void)
#else
publicdef LONG ps_long_tos ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_long_sp);
    
    return(PS_LONG_TOS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Return count of how many values are on long stack.  

Prototype:
    publicdef INT ps_long_many(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of ps_long_sp + 1.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_long_many (void)
#else
publicdef INT ps_long_many ()
#endif
    {
    return(ps_long_sp + 1);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Print the top of the long stack.  

Prototype:
    publicdef INT ps_long_print(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_long_print (void)
#else
publicdef INT ps_long_print ()
#endif
    {
    PS_CHECK_EMPTY (ps_long_sp);
    
    sprintf (astring, "%d", ps_long_pop ());
    ps_string_out (astring);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    View the entire long stack.  

Prototype:
    publicdef INT ps_long_view(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_long_view (void)
#else
publicdef INT ps_long_view ()
#endif
    {
    INT many, i;
    
    ps_string_out ("LONG Stack:");
    
    /* Get count of stack. */
    if ((many = ps_long_many ()) EQUALS 0)
        {
        ps_string_out ("EMPTY!");
        }
    else
        {
        for (i = many - 1; i >= 0; --i)
            {
            sprintf (astring, "%d", ps_long_stack[i]);
            ps_string_out (astring);
            }
        }
    return(SUCCESS);
    }
/* END:     */

/* Stack operators for DOUBLE stack.
   * These are created by copying the code for LONG and changeing
   * LONG to DOUBLE
*/

/* Function Description -----------------------------------------------------
Description:
    Push value onto double stack. 

Prototype:
    publicdef INT ps_double_push(DOUBLE some_value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    some_value      -(DOUBLE) Any double value. 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_double_push (DOUBLE some_value)
#else
publicdef INT ps_double_push (some_value)
DOUBLE some_value;
#endif
    {
    PS_CHECK_OVERFLOW (ps_double_sp);
    
    PS_DOUBLE_PUSH (some_value);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    POP value off of double stack.  

Prototype:
    publicdef DOUBLE  ps_double_pop(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    RETURNS the top of double stack.  

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE ps_double_pop (void)
#else
publicdef DOUBLE ps_double_pop ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_double_sp);
    
    return(PS_DOUBLE_POP);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Return value on top of double stack.  

Prototype:
    publicdef DOUBLE  ps_double_tos(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of PS_DOUBLE_TOS.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE ps_double_tos (void)
#else
publicdef DOUBLE ps_double_tos ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_double_sp);
    
    return(PS_DOUBLE_TOS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Return count of how many values are on double stack.  

Prototype:
    publicdef INT ps_double_many(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of ps_double_sp + 1.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_double_many (void)
#else
publicdef INT ps_double_many ()
#endif
    {
    return(ps_double_sp + 1);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Print the top of the double stack.  

Prototype:
    publicdef INT ps_double_print(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_double_print(void)
#else
publicdef INT ps_double_print()
#endif
    {
    PS_CHECK_EMPTY (ps_double_sp);
    
    sprintf (astring, "%f", ps_double_pop ());
    ps_string_out (astring);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    View the entire double stack.  

Prototype:
    publicdef INT ps_double_view(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_double_view (void)
#else
publicdef INT ps_double_view ()
#endif
    {
    INT many, i;
    
    ps_string_out ("DOUBLE Stack:");
    
    /* Get count of stack. */
    if ((many = ps_double_many ()) EQUALS 0)
        {
        ps_string_out ("EMPTY!");
        }
    else
        {
        for (i = many - 1; i >= 0; --i)
            {
            sprintf (astring, "%f", ps_double_stack[i]);
            ps_string_out (astring);
            }
        }
    return(SUCCESS);
    }
/* END:     */

/* Stack operators for POINTER stack.
   * These are created by copying the code for LONG and changeing
   * LONG to POINTER
*/

/* Function Description -----------------------------------------------------
Description:
    Push value onto pointer stack.  

Prototype:
    publicdef INT ps_pointer_push(VOIDPTR some_value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    some_value      -(VOIDPTR) Any pointer value. 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_pointer_push (VOIDPTR some_value)
#else
publicdef INT ps_pointer_push (some_value)
VOIDPTR some_value;
#endif
    {
    PS_CHECK_OVERFLOW (ps_pointer_sp);
    
    PS_POINTER_PUSH (some_value);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    POP value off of pointer stack.  

Prototype:
    publicdef VOIDPTR ps_pointer_pop(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    RETURNS the top of pointer stack.  

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOIDPTR ps_pointer_pop (void)
#else
publicdef VOIDPTR ps_pointer_pop ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_pointer_sp);
    
    return(PS_POINTER_POP);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Return value on top of pointer stack.  

Prototype:
    publicdef VOIDPTR ps_pointer_tos(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of PS_POINTER_TOS.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOIDPTR ps_pointer_tos (void)
#else
publicdef VOIDPTR ps_pointer_tos ()
#endif
    {
    PS_CHECK_UNDERFLOW (ps_pointer_sp);
    
    return(PS_POINTER_TOS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Return count of how many values are on pointer stack.  

Prototype:
    publicdef INT ps_pointer_many(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the value of ps_pointer_sp + 1.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_pointer_many (void)
#else
publicdef INT ps_pointer_many ()
#endif
    {
    return(ps_pointer_sp + 1);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Print the top of the pointer stack.  

Prototype:
    publicdef INT ps_pointer_print(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_pointer_print (void)
#else
publicdef INT ps_pointer_print ()
#endif
    {
    PS_CHECK_EMPTY (ps_pointer_sp);
    
    sprintf (astring, "%f", ps_pointer_pop ());
    ps_string_out (astring);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    View the entire pointer stack.  

Prototype:
    publicdef INT ps_pointer_view(void);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ps_pointer_view (void)
#else
publicdef INT ps_pointer_view ()
#endif
    {
    INT many, i;
    
    ps_string_out ("POINTER Stack:");
    
    /* Get count of stack. */
    if ((many = ps_pointer_many ()) EQUALS 0)
        {
        ps_string_out ("EMPTY!");
        }
    else
        {
        for (i = many - 1; i >= 0; --i)
            {
            sprintf (astring, "%p", ps_pointer_stack[i]);
            ps_string_out (astring);
            }
        }
    return(SUCCESS);
    }
/* END:     */

/* Internal routines. */

/* Function Description -----------------------------------------------------
Description:
    Print a string.  

Prototype:
    static VOID ps_string_out(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    string          -(CHAR *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <ps_parm_stks.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID ps_string_out (CHAR *string)
#else
static VOID ps_string_out (string)
CHAR *string;
#endif
    {
    printf ("%s\n", string);        /* %Q */
    }
/* END:     */
