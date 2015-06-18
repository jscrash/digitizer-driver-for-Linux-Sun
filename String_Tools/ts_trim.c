/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TRIM.C*/
/*  *2    17-AUG-1990 22:31:04 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:34:31 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TRIM.C*/
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
#include "esi_ts.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Similar to ts_snowhite, but left shifts the string so that
    the pointer is not changed.
    
Prototype:
    publicdef INT ts_trim(CHAR *cstr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) cstr        -(CHAR *) string to trim.

Return Value/Status:
    SUCCESS - Completion of string trim.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_trim (CHAR *cstr) 
#else
publicdef INT ts_trim (cstr)
CHAR *cstr;
#endif
    {
    CHAR *temp_str;                 /* DUMMY STRING POINTER */
    CHAR *old_temp_str;             /* DUMMY STRING POINTER */
    INT status;                     /* RETURN STATUS */
    
    status = SUCCESS;
    
    /* * COPY TO TEMP STRING * */
    
    temp_str = old_temp_str = (CHAR *)tc_alloc (strlen ((char *)cstr) + 2);
    
    /* * SNOWHITE THE TEMP STRING * */
    
    temp_str = ts_snowhite (cstr);
    
    /* * COPY BACK * */
    
    strcpy ((char *)cstr, (char *)temp_str);
    tc_free ((BYTE *)old_temp_str);
    
    return status;
    }
/* END:     */
