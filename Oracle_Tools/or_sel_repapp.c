/* DEC/CMS REPLACEMENT HISTORY, Element OR_SEL_REPAPP.C*/
/* *4    25-JUL-1990 19:58:06 VINCE "(SPR 5644) Header Standardization"*/
/* *3     3-DEC-1989 14:12:50 JULIAN "(SPR 100) Make default data type WELLS"*/
/* *2    26-JUN-1989 11:02:00 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:21:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OR_SEL_REPAPP.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef OR_REPORTS_H

#include "or_reports.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT or_select_report_app(CHAR  *not_used);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    not_used        -(CHAR  *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT or_select_report_app (CHAR *not_used)
#else
publicdef INT or_select_report_app (not_used)
CHAR *not_used;
#endif
    {
    PROGNAME ("OR_SELECT_REPORT_APP");
    INT status;
    REPORT_STRUCT *pScript;
    
    am_declare ("Report Selection");
    
    status = am_open_workspace ("OR_REPORT", AM_GLOBAL, (VOIDPTR *) & pScript);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        status = am_define_workspace ("OR_REPORT", AM_GLOBAL, sizeof(REPORT_STRUCT),
                                      (VOIDPTR *) & pScript);
        pScript->control_list_required = FALSE;
        pScript->run_interactive = TRUE;
        pScript->print = FALSE;
        pScript->save = FALSE;
        strcpy (pScript->data_type, "WELLS");
        }
    status = or_select_script (pScript);
    
    return status;
    }
/* END:     */
