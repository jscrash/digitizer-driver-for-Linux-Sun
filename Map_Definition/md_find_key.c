/* DEC/CMS REPLACEMENT HISTORY, Element MD_FIND_KEY.C*/
/* *3    17-AUG-1990 21:56:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     5-APR-1990 16:16:10 JULIAN "(SPR 0) fix char allocation problem"*/
/* *1    19-JUN-1989 13:06:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_FIND_KEY.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_AM_DEFS_H

#include "esi_am_defs.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MD_DEFS_H

#include "esi_md_defs.h"

#endif

#ifndef ESI_MD_H

#include "esi_md.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to find the next keyword in the phrase and store
    it in a MD_KEY_STRUCT structure.

Prototype:
    publicdef BYTE *md_find_key(CHAR *phrase,INT *status);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    phrase          -(CHAR *) Overlay phrase.
    status          -(INT *) Return status.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_KEYWORD_TOO_LONG
    MD_UNBALANCED_PARENS
    
Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BYTE *md_find_key (CHAR *phrase, INT *status)
#else
publicdef BYTE *md_find_key (phrase, status)
CHAR *phrase;
INT *status;
#endif
    {
    
    PROGNAME ("MD_FIND_KEY");
    INT klen;                       /* KEYWORD LENGTH */
    CHAR *cptr1;                    /* DUMMY STRING POINTER */
    CHAR *cptr2;                    /* DUMMY STRING POINTER */
    CHAR *subphrs = NULL;           /* PARM LIST SUBPHRASE */
    MD_KEY_STRUCT *temp;            /* KEYWORD STRUCTURE TO RETURN */
    
    /* ********************************************************************** */
    
    *status = SUCCESS;
    
    temp = (MD_KEY_STRUCT *)0;
    
    /* * FIND KEY DELIMITER * */
    /* ---------------------- */
    
    if ((cptr1 = md_next_delim (phrase, status)) != NULL)
        {
        klen = cptr1 - phrase + 1;
        }
    else
        {
        if (*status < 0)
            {
            goto finished;
            }
        klen = strlen (phrase) + 1;
        }
    /* * FOUND A KEY, CONTINUE PARSING * */
    /* --------------------------------- */
    
    if (klen > 1)
        {
        /* CREATE KEY STRUCTURE */
        
        temp = (MD_KEY_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_KEY_STRUCT));
        temp->valid = FALSE;
        
        /* GET NEXT KEY ADDRESS */
        if (cptr1 != NULL)
            {
            temp->nxtkey = (MD_KEY_STRUCT *)md_find_key (++cptr1, status);
            if (*status < 0)
                {
                goto error;
                }
            }
        /* SUBPHRASE WITH PARAMETER LIST */
        
        subphrs = (CHAR *)am_allocate (AM_APPLICATION, klen);
        strncpy (subphrs, phrase, klen - 1);
        subphrs[klen - 1] = '\0';
        
        /* THERE IS A PARAMETER LIST */
        
        if ((cptr1 = strchr (subphrs, START_PARMS)) != NULL)
            {
            klen = cptr1 - subphrs + 1;
            cptr2 = cptr1 + 1;
            while (*cptr2 EQUALS ' ')
                {
                cptr2++;
                }
            /* STRIP OFF PARENTHESIS IF NEEDED */
            
            if (*cptr2 EQUALS OPEN_PAREN)
                {
                cptr1 = cptr2;
                if ((cptr2 = strrchr (subphrs, CLOSE_PAREN)) != NULL)
                    {
                    subphrs[cptr2 - subphrs] = '\0';
                    }
                else
                    {
                    *status = MD_UNBALANCED_PARENS;
                    goto error;
                    }
                }
            /* GET PARAMETERS */
            
            temp->parmptr = (MD_PARM_STRUCT *)md_find_parm (++cptr1, status);
            if (*status < 0)
                {
                goto error;
                }
            }
        /* CREATE AND STORE KEYWORD */
        
        strncpy (subphrs, phrase, klen - 1);
        subphrs[klen - 1] = '\0';
        ts_trim (subphrs);
        klen = strlen (subphrs) + 1;
        if (klen > 26)
            {
            goto too_long;
            }
        temp->keywrd = (CHAR *)am_allocate (AM_GLOBAL, klen);
        strcpy (temp->keywrd, subphrs);
        }
    /* * NO KEY FOUND * */
    /* ---------------- */
    
    else
        {
        temp = 0;
        }
    goto finished;
    
    
    /* * KEY TOO LONG * */
    /* ---------------- */
    
too_long:
    
    *status = MD_KEYWORD_TOO_LONG;
    
    /* * ERROR * */
    /* --------- */
    
error:
    
    am_free (temp);
    temp = 0;
    
    /* * RETURN * */
    /* ---------- */
    
finished:
    
    am_free (subphrs);
    return(BYTE *)temp;
    }
/* END:     */
