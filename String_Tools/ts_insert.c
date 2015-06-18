/*  DEC/CMS REPLACEMENT HISTORY, Element TS_INSERT.C*/
/*  *3    17-AUG-1990 22:29:20 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    21-MAY-1990 12:16:57 VINCE "(SPR 1) initial checkin"*/
/*  *1    21-MAY-1990 12:02:52 VINCE "insert a string within another string"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_INSERT.C*/
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

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    This routine will shift characters over and insert the insert_string
    at the beginning of the subject_string (see ts_subs for a 
    usage example).
    
Prototype:
    publicdef CHAR *ts_insert (CHAR *subject, CHAR *insert);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) subject     -(CHAR *) string to insert the insert substring.
    (I) insert      -(CHAR *) string to be inserted into subject string.

Return Value/Status:
    Returns pointer to subject string.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_insert (CHAR *subject, CHAR *insert) 
#else
publicdef CHAR *ts_insert (subject, insert)
CHAR *subject;
CHAR *insert;
#endif
    {
    INT insert_len;
    
    insert_len = strlen (insert);
    if (insert_len == 0)
        return subject;
    
    ts_shift (subject, insert_len);
    strncpy (subject, insert, insert_len);
    
    return subject;
    }
/* END:     */
