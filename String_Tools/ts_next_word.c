/*  DEC/CMS REPLACEMENT HISTORY, Element TS_NEXT_WORD.C*/
/*  *3    17-AUG-1990 22:29:37 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-FEB-1990 19:01:04 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/*  *1    19-JUN-1989 13:33:38 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_NEXT_WORD.C*/
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

#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    TS_NEXT_WORD parses a null terminated character string and returns
    tokens, or words, from the string, to the string indicated by the
    token argument. The value of the function is a pointer to the next
    word in the line. Words are delimited by one or more spaces.
    Quoted strings are considered one word.

    The caller specifies a line to be parsed, and a character string
    where the next word in the line is to be placed. The function
    returns the address of the next word in the line. This address
    should be used in subsequent calls to ts_next_word so that all
    of the words can be extracted.

    CALLING SEQUENCE:

    next = ts_next_word(line, token);

    EXAMPLES:

     CHAR line[100];
     CHAR token[100];
     CHAR *next;

     strcpy(line,"THE BOY ATE THE \"UGLY DOG\"");
     next = line;
     while (next = ts_next_word(next,token))
          printf("%s\n",token);


     Will produce the following output:

     THE
     BOY
     ATE
     THE
     UGLY DOG

Prototype:
    publicdef CHAR *ts_next_word(CHAR *line, CHAR *token);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) line        -(CHAR *) input character string containing words to be 
                        parsed.
    (I) token       -(CHAR *) output character string to contain next word.

Return Value/Status:
    TS_NEXT_WORD returns a null pointer if no word could be found.
    Naturally, this will always occur after all words have been
    extracted from the character string.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_next_word (CHAR *line, CHAR *token) 
#else
publicdef CHAR *ts_next_word (line, token)
CHAR *line;
CHAR *token;
#endif
    {
    CHAR *l;                        /* Source line */
    CHAR *t;                        /* Returned token */
    CHAR c;                         /* Current token character */
    BOOL status;                    /* Complete token flag */
    BOOL quote;                     /* Quoted string being processed flag */
    
    /* ********************************************************************** */
    
    /*   Initialize address of returned token. */
    
    t = token;
    
    /*   If null string passed, then do
         nothing. */
    
    if (line == (CHAR *)0)
        l = (CHAR *)0;
    
    else
        {
        /*   Indicate not processing quoted string. */
        
        quote = FALSE;
        
        /*   Indicate still processing token. */
        
        status = TRUE;
        
        /*   Initialize address of next word. */
        
        l = line;
        
        /*   Skip leading spaces. */
        while ((*l != 0) && (*l == ' ') && (*l != '\n'))
            l++;
        
        /*   Process entire token. */
        
        while (status)
            {
            
            /*   Look for token terminators. */
            
            switch (c = *l++)
                {
                
                /*   \n or null terminates token. */
                
            case 0:
            case '\n':
                l = (CHAR *)NULL;
                status = FALSE;
                break;
                
                /*   Quote character. */
            case '\"':
                /*   If terminating quote, then done with
                     token. */
                
                if (quote)
                    status = FALSE;
                
                /*   If leading quote, indicate processing
                     quoted string. */
                
                else
                    quote = TRUE;
                break;
                
                /*   Blank. */
                
            case ' ':
                
                /*   If within quoted string, then blank
                     is part of token. */
                
                if (quote)
                    *t++ = c;
                
                /*   If not in quoted string, then blank
                     terminates token. */
                
                else
                    status = FALSE;
                break;
                
                /*   Not a token terminator. Copy character
                     into resultant token. */
                
            default:
                *t++ = c;
                break;
                }
            }
        }
    /*   Terminate resultant token. */
    
    *t = 0;
    
    /*   If end of line, return null pointer. */
    
    if (*token == 0)
        l = (CHAR *)NULL;
    
    /*   Return update line position for access
         to next token. */
    return l;
    }
/* END:     */
