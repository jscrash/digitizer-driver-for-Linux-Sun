/* DEC/CMS REPLACEMENT HISTORY, Element MD_FIND_PARM.C*/
/* *2    17-AUG-1990 21:56:53 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:06:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_FIND_PARM.C*/
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
    Function to find and store the next parameter in the supplied
    parameter list.

Prototype:
    publicdef BYTE *md_find_parm(char *parm_list,INT *status);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    parm_list       -(CHAR *) Parameter list.
    status          -(INT *) Return status.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BYTE *md_find_parm (char *parm_list, INT *status)
#else
publicdef BYTE *md_find_parm (parm_list, status)
char *parm_list;
INT *status;
#endif
    {
    
    PROGNAME ("MD_FIND_PARM");
    INT plen;                       /* LENGTH OF PARAMETER */
    CHAR *cptr;                     /* DUMMY STRING POINTER */
    MD_PARM_STRUCT *temp;           /* PARAMETER STRUCTURE */
    CHAR text[STMAX];
    /* ********************************************************************** */
    
    *status = SUCCESS;
    
    /* * FIND PARAMETER DELIMITER * */
    /* ---------------------------- */
    
    if ((cptr = md_next_delim (parm_list, status)) != NULL)
        {
        plen = cptr - parm_list + 1;
        }
    else
        {
        if (*status < 0)
            {
            goto finished;
            }
        plen = strlen (parm_list) + 1;
        }
    /* * FOUND A PARAMETER, CONTINUE PARSING * */
    /* --------------------------------------- */
    
    if (plen > 1)
        {
        /* CREATE PARAMETER STRUCTURE */
        
        temp = (MD_PARM_STRUCT *)am_allocate (AM_GLOBAL, sizeof(MD_PARM_STRUCT));
        
        /* CREATE PARAMETER STRING */
        
        temp->parmval.cval = (CHAR *)am_allocate (AM_GLOBAL, plen);
        strncpy (temp->parmval.cval, parm_list, plen - 1);
        temp->parmval.cval[plen - 1] = '\0';
        ts_trim (temp->parmval.cval);
        
        /* GET NEXT PARAMETER */
        if (cptr != NULL)
            {
            temp->nxtparm = (MD_PARM_STRUCT *)md_find_parm (++cptr, status);
            if (*status != SUCCESS)
                {
                goto error;
                }
            }
        }
    /* * NO PARAMETER FOUND * */
    /* ---------------------- */
    
    else
        {
        temp = 0;
        }
    goto finished;
    
    /* * ERROR * */
    /* --------- */
    
error:
    
    am_free (temp->parmval.cval);
    am_free (temp);
    temp = 0;
    
    /* * RETURN * */
    /* ---------- */
    
finished:
    
    return(BYTE *)temp;
    }
/* END:     */
