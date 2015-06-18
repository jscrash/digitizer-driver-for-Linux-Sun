/* DEC/CMS REPLACEMENT HISTORY, Element MD_FREE_PARMS.C*/
/* *2    17-AUG-1990 21:57:14 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:06:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_FREE_PARMS.C*/
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
	Function to free the memory for the linked parameter 
	structure list beginning at the supplied parmstruc.

Prototype:
    publicdef INT md_free_parms (MD_PARM_STRUCT *parmstruc);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parmstruc		-(MD_PARM_STRUCT *)

Return Value/Status:
    SUCCESS	- Successful completion.
    FAIL	- Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT md_free_parms (MD_PARM_STRUCT *parmstruc)       
#else
publicdef INT md_free_parms (parmstruc)
MD_PARM_STRUCT *parmstruc;
#endif
    {
    
    PROGNAME ("MD_FREE_PARMS");
    INT status;
    MD_PARM_STRUCT *parmstruc1;
    MD_PARM_STRUCT *parmstruc2;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    parmstruc1 = parmstruc;
    
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
    return status;
    }
/* END:	    */
