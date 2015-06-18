/* DEC/CMS REPLACEMENT HISTORY, Element WL_PAR_WHERE.C*/
/* *3    23-AUG-1990 17:26:39 JESSIE "(SPR 1) fix include files"*/
/* *2    14-AUG-1990 13:42:20 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:40:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_PAR_WHERE.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_WL_H

#include "esi_wl.h"

#endif

#ifndef ESI_WL_ERR_H

#include "esi_wl_err.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to find the supplied special keys in the supplied
    where clause and return there query values.
    Allows for any of the following possible formats:

            KEY = "VALUE"
            KEY="VALUE"
            KEY= "VALUE"
            KEY ="VALUE"

Prototype:
    publicdef INT wl_parse_where(CHAR *where_cls,CHAR special_key[3][13],INT num_in,
        INT *num_found);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) where_cls   -(CHAR *) Where clause to analyse.
    (M) special_key -(CHAR [][]) In: Special keys to search for.
                        Out: there repsective query values.
    (I) num_in      -(INT) Number of keys in array.
    (I) num_found   -(INT *) Number of keys found.

Return Value/Status:
    SUCCESS - Successful completion.
    WL_BAD_SQL_STATEMENT
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_parse_where (CHAR *where_cls, CHAR special_key[3][13], INT num_in,
                              INT *num_found) 
#else
publicdef INT wl_parse_where (where_cls, special_key, num_in, num_found)
CHAR *where_cls;
CHAR special_key[3][13];
INT num_in;
INT *num_found;
#endif
    {
    
    PROGNAME ("WL_PARSE_WHERE");
    INT status;                     /* RETURN STATUS */
    CHAR temp_key[3][13];           /* TEMP ARRAY TO HOLD QUERY VALUE */
    BOOL key_found[3];              /* KEY FOUND FLAG */
    CHAR *word;                     /* PARSE WORD */
    CHAR test_key[13];              /* WORD TO TEST AS A KEY */
    CHAR temp_val[13];              /* PARSED VALUE */
    INT word_len;                   /* LENGTH OF WORD */
    INT eq_location;                /* LOCATION OF = IN WORD */
    CHAR *cptr;                     /* DUMMY CHAR POINTER */
    INT i;                          /* DUMMY INDEX */
    
    /* ********************************************************************** */
    
    key_found[0] = FALSE;
    key_found[1] = FALSE;
    key_found[2] = FALSE;
    
    status = SUCCESS;
    
    word = (CHAR *)am_allocate (AM_APPLICATION, strlen (where_cls) + 1);
    where_cls = ts_next_word (where_cls, word);
    while (where_cls != (CHAR *)NULL)
        {
        
        /* * ONLY ANALYZE IF WORD != AND, ELSE JUST GET NEXT WORD AND LOOP * */
        /* ----------------------------------------------------------------- */
        
        if (strcmp (word, "AND") != 0)
            {
            word_len = strlen (word);
            
            /* * FORMAT: KEY="VALUE" OR FORMAT: KEY= "VALUE" * */
            /* ----------------------------------------------- */
            
            if ((cptr = strrchr (word, '=')) != (CHAR *)NULL)
                {
                eq_location = cptr - word;
                
                /* FORMAT: KEY= "VALUE" */
                
                if (eq_location + 1 EQUALS word_len)
                    {
                    cptr = word + eq_location + 1;
                    ts_next_word (cptr, temp_val);
                    }
                /* FORMAT: KEY="VALUE" */
                
                else
                    {
                    where_cls = ts_next_word (where_cls, temp_val);
                    }
                strncpy (test_key, word, eq_location);
                test_key[eq_location] = '\0';
                }
            /* * FORMAT: KEY = "VALUE" OR FORMAT: KEY ="VALUE" * */
            /* ------------------------------------------------- */
            
            else
                {
                strcpy (test_key, word);
                where_cls = ts_next_word (where_cls, word);
                
                /* FORMAT: KEY = "VALUE" */
                
                if (strlen (word) EQUALS 1)
                    {
                    where_cls = ts_next_word (where_cls, temp_val);
                    }
                /* FORMAT: KEY ="VALUE" */
                
                else
                    {
                    strcpy (temp_val, (word + 1));
                    }
                }
            /* * HAVE KEY AND VALUE CHECK IF KEY MATCHES ANY IN SUPPLIED LIST * */
            /* ---------------------------------------------------------------- */
            
            for (i = 0; i < num_in; i++)
                {
                if (strcmp (test_key, special_key[i]) EQUALS 0)
                    {
                    
                    /* MULTIPLE OCCURANCE OF KEY */
                    
                    if (key_found[i])
                        {
                        status = WL_BAD_SQL_STATEMENT;
                        goto finished;
                        }
                    /* SET VALUE */
                    
                    strcpy (temp_key[i], temp_val);
                    key_found[i] = TRUE;
                    goto found;
                    }
                }
            /* * KEY NOT IN SEARCH LIST * */
            /* -------------------------- */
            
            status = WL_BAD_SQL_STATEMENT;
            goto finished;
            }
        /* * GET NEXT WORD AND LOOP * */
        /* -------------------------- */
        
    found:
        where_cls = ts_next_word (where_cls, word);
        }
    /* * CHECK THAT FOUND ALL NECESSARY KEYS * */
    /* --------------------------------------- */
    
    for (i = 0; i < num_in - 1; i++)
        {
        if (NOT key_found[i])
            {
            status = WL_BAD_SQL_STATEMENT;
            goto finished;
            }
        }
    /* * DETERMINE HOW MANY KEYS FOUND * */
    /* --------------------------------- */
    
    if (key_found[num_in - 1])
        {
        *num_found = num_in;
        }
    else
        {
        *num_found = num_in - 1;
        }
    /* * COPY OVER THE FOUND KEYS * */
    /* ---------------------------- */
    
    for (i = 0; i < num_in; i++)
        {
        strcpy (special_key[i], temp_key[i]);
        }
    /* * RETURN * */
    /* ---------- */
    
finished:
    am_free (word);
    return status;
    }
/* END:     */
