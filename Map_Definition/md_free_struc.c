/* DEC/CMS REPLACEMENT HISTORY, Element MD_FREE_STRUC.C*/
/* *2    17-AUG-1990 21:57:18 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:06:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_FREE_STRUC.C*/
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
#include "esi_gl_defs.h"
#include "esi_md_defs.h"

/* Function Description -----------------------------------------------------
Description:
	Function to free the memory for the linked structure list
	beginning at the supplied verbstruc.

Prototype:
    publicdef INT md_free_structs (MD_VERB_STRUCT *verbstruc);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    verbstruc		-(MD_VERB_STRUCT *)

Return Value/Status:
    SUCCESS	- Successful completion.
    FAIL	- Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_free_structs (MD_VERB_STRUCT *verbstruc)   
#else
publicdef INT md_free_structs (verbstruc)   
MD_VERB_STRUCT *verbstruc;
#endif
    {
    
    PROGNAME ("MD_FREE_STRUCTS");
    INT status;
    MD_KEY_STRUCT *keystruc1;
    MD_KEY_STRUCT *keystruc2;
    MD_PARM_STRUCT *parmstruc1;
    MD_PARM_STRUCT *parmstruc2;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (verbstruc != NULL_VERB)
        {
        keystruc1 = verbstruc->keyptr;
        
        while (keystruc1 != NULL_KEY)
            {
            keystruc2 = keystruc1->nxtkey;
            parmstruc1 = keystruc1->parmptr;
            
            while (parmstruc1 != NULL_PARM)
                {
                parmstruc2 = parmstruc1->nxtparm;
                if (parmstruc1->parmtype EQUALS 1)
                    {
                    am_free (parmstruc1->parmval.cval);
                    }
                am_free (parmstruc1);
                parmstruc1 = parmstruc2;
                }
            am_free (keystruc1);
            keystruc1 = keystruc2;
            }
        am_free (verbstruc);
        }
    return status;
    }
/* END:	    */
