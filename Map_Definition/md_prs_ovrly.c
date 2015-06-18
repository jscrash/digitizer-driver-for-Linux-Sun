/* DEC/CMS REPLACEMENT HISTORY, Element MD_PRS_OVRLY.C*/
/* *3    30-OCT-1990 11:53:04 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *2    17-AUG-1990 21:57:52 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_PRS_OVRLY.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_PRS_OVRLY.C                              */
/* *2    17-AUG-1990 21:57:52 VINCE "(SPR 5644) Code Cleanup"                       */
/* *1    19-JUN-1989 13:07:11 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element MD_PRS_OVRLY.C                              */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

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

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function to start parsing the sentence sentence into the                      */
/*    sentence structures.  This function specifically finds the                    */
/*    verb in the sentence sentence, extracts it and places it                      */
/*    in a MD_VERB_STRUCT structure.  The function returns the                      */
/*    pointer to this structure as its value.                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef BYTE *md_parse_ovrly(CHAR *sentence,INT *status);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    sentence        -(CHAR *) Overlay sentence.                                   */
/*    status          -(INT *) Return status.                                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    MD_VERB_NOT_FOUND                                                             */
/*    MD_VERB_TOO_LONG                                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef BYTE *md_parse_ovrly (CHAR *sentence, INT *status)
#else
publicdef BYTE *md_parse_ovrly (sentence, status)
CHAR *sentence;
INT *status;
#endif
    {
    INT vlen;                       /* LENGTH OF VERB                               */
    CHAR *cptr;                     /* DUMMY STRING POINTER                         */
    MD_VERB_STRUCT *temp;           /* VERB STRUCTURE TO RETURN                     */
    
    /* **********************************************************************       */
    
    *status = SUCCESS;
    
    /* * FIND VERB DELIMITER                                                        */
    /*   -------------------                                                        */
    
    if ((cptr = strchr (sentence, SEPARATOR)) != NULL)
        {
        vlen = cptr - sentence + 1;
        }
    else
        {
        vlen = strlen (sentence) + 1;
        }
    /* * FOUND A VERB, CONTINUE PARSING *                                           */
    /* ----------------------------------                                           */
    
    if (vlen > 1 AND vlen < 27)
        /* CREATE VERB STRUCT AND STORE VERB                                        */
        {
        temp = (MD_VERB_STRUCT *) am_allocate (AM_GLOBAL, sizeof(MD_VERB_STRUCT));
        temp->overlay = sentence;
        temp->verb = (CHAR *) am_allocate (AM_GLOBAL, vlen);
        strncpy (temp->verb, sentence, vlen - 1);
        temp->verb[vlen - 1] = '\0';
        ts_trim (temp->verb);
        
        /* LOOK FOR KEYS                                                            */
        
        if (cptr != NULL)
            temp->keyptr = (MD_KEY_STRUCT *) md_find_key (++cptr, status);
        
        if (*status != SUCCESS)
            {
            am_free (temp->verb);
            am_free (temp);
            temp = 0;
            }
        }
    /* * VERB STRING TOO LONG *                                                     */
    /* ------------------------                                                     */
    
    else if (vlen > 26)
        {
        *status = MD_VERB_TOO_LONG;
        temp = 0;
        }
    /* * NO VERB FOUND *                                                            */
    /* -----------------                                                            */
    
    else
        {
        *status = MD_VERB_NOT_FOUND;
        temp = 0;
        }
    return(BYTE *)temp;
    }
/* END:                                                                             */
