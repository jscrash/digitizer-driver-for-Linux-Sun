/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_PARENS.C*/
/* *2    17-AUG-1990 22:36:26 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_PARENS.C*/
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

#include "esi_gl_defs.h"

/* Function Description -----------------------------------------------------
Description:
    Unwrap parenthesis from cross section title strings  
    The title strings are wrapped when saved so that     
    imbedded commas are not interpreted by the parser    
    as delimiters.                                       
    "(TEXACO, INC.)"   will become  "TEXACO, INC."       
    The modifications are done in place.                 

Prototype:
    publicdef INT xsz_strip_parens(CHAR text[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) text        -(CHAR []) A character string.         

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_strip_parens (CHAR text[])
#else
publicdef INT xsz_strip_parens (text)
CHAR text[];
#endif
    {
    INT length, i;
    
    if (text[0] != '(')
        return SUCCESS;
    length = strlen (text) - 1;
    if (text[length] != ')')
        return SUCCESS;
    for (i = 1; i < length; i++)
        {
        text[i - 1] = text[i];
        }
    text[length - 1] = '\0';
    return SUCCESS;
    }
/* END:     */
