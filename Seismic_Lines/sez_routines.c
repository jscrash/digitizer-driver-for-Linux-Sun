/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_ROUTINES.C*/
/* *4    17-AUG-1990 22:15:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    18-JUN-1990 12:07:30 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *2    25-JUL-1989 20:01:16 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:24:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_ROUTINES.C*/
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
#include "esi_se_int.h"

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Utility routines for seismic surfaces tool box.
    Used to return in source_hori either the current oracle USER  
    if source_hor is NULL or source_hor if not.

Prototype:
    publicdef INT  sez_fillsource (CHAR *source_hor ,CHAR *source_hori ) ;

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    source_hor      -(CHAR *) Pointer to a character string containing the source
                        it can be NULL.
    source_hori     -(CHAR *) Pointer to a character string containing the source
                        returned. 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sez_fillsource (CHAR *source_hor, CHAR *source_hori)
#else
publicdef INT sez_fillsource (source_hor, source_hori)
CHAR *source_hor;
CHAR *source_hori;
#endif
    {
    /* if source_hor is NULL we take the user as source_hori
        if not we don't take source_hor as source    
    */
    
    if (IS_EMPTY_STRING (source_hor))
        {
        qs_inq_c (QS_FINDER_ACCOUNT, source_hori, (INT *)0);
        }
    else
        {
        strcpy (source_hori, source_hor);
        }
    return SUCCESS;
    }
/* END:     */
