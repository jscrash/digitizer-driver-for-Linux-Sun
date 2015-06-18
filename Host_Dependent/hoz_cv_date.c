/* DEC/CMS REPLACEMENT HISTORY, Element HOZ_CV_DATE.C */
/* *7    14-AUG-1990 13:30:22 VINCE "(SPR 5644) Header Standardization" */
/* *6    21-MAR-1990 15:28:50 MING "(SPR 0) fix bug to force month to upper case" */
/* *5    14-FEB-1990 12:55:46 MING "(SPR 0) change date format YYMMDD_HHMISS to YYMMDD-HHMISS" */
/* *4    13-FEB-1990 10:50:50 MING "(SPR 0) add prototype" */
/* *3    13-FEB-1990 10:46:23 MING "(SPR 0) add comment" */
/* *2    12-FEB-1990 12:37:18 MING "(SPR 0) created" */
/* *1    12-FEB-1990 12:34:08 MING "change old date format DD-MON-YYYY HH:MI:SS to YYMMDD_HHMISS" */
/* DEC/CMS REPLACEMENT HISTORY, Element HOZ_CV_DATE.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_GL_DEF_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifdef PROTOTYPE
publicdef void hoz_cv_date (DATE_TIME, CHAR *);
#else
publicdef void hoz_cv_date ();
#endif

/* Function Description -----------------------------------------------------
Description:
    Change old date format : DD-MMM-YYYY HH:MI:SS\0
    to new format : YYMMDD-HHMISS\0

Prototype:
    publicdef VOID hoz_cv_date (DATE_TIME new_date, CHAR * old_date);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) new_date    -(DATE_TIME) New date format.
    (I) old_date    -(CHAR *) Old date format.

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID hoz_cv_date (DATE_TIME new_date, CHAR *old_date) 
#else
publicdef VOID hoz_cv_date (new_date, old_date)
DATE_TIME new_date;
CHAR *old_date;
#endif
    {
    CHAR month[4];
    CHAR local_month[4];
    INT int_month;
    
    static CHAR *months[] = 
        {
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN",
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC",
        0
        };
    
    strncpy ((CHAR *)local_month, (CHAR *) & old_date[3], 3);
    local_month[3] = '\0';
    ts_sto_upper (local_month);
    
    int_month = ts_switch (local_month, months) + 1;
    
    if (int_month == 0)
        {
        am_message (AM_ALERT, "month is not converted right");
        }
    sprintf ((char *)month, "%02d", int_month);
    
    sprintf (new_date, "%2.2s%2.2s%2.2s-%2.2s%2.2s%2.2s", &old_date[9], month,
             &old_date[0], &old_date[12], &old_date[15], &old_date[18]);
    }
/* END:     */
