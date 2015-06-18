/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_DATA.C*/
/* *6    27-MAR-1991 13:28:42 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *5    17-AUG-1990 22:34:45 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:51:27 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    18-SEP-1989 12:44:47 JULIAN "Gulf mods under SPR 100"*/
/* *2    27-JUL-1989 20:22:30 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_DATA.C*/
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

#include "esi_xs_parse.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to fill the value into XS structure branching by
    verb name.

Prototype:
    publicdef INT xsz_fill_data(CHAR *verb, MD_VERB_STRUCT *verbptr, XS_STRUCT *xsptr, 
        INT init);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    verb            -(CHAR *)           
    verbptr         -(MD_VERB_STRUCT *)
    xsptr           -(XS_STRUCT *)
    init            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_data (CHAR *verb, MD_VERB_STRUCT *verbptr, XS_STRUCT *xsptr,
                             INT init)
#else
publicdef INT xsz_fill_data (verb, verbptr, xsptr, init)
CHAR *verb;
MD_VERB_STRUCT *verbptr;
XS_STRUCT *xsptr;
INT init;
#endif
    {
    static BOOL first_trace = TRUE, first_track = TRUE, first_top = TRUE;
    INT status = SUCCESS;
    BOOL first_track_keyword = TRUE, first_trace_keyword = TRUE,
                               first_top_keyword = TRUE;
    BOOL last_track_keyword = FALSE, last_trace_keyword = FALSE,
                              last_top_keyword = FALSE;
    MD_KEY_STRUCT *keystr;
    /* ********************************************************************** */
    /* If init, allocates space for PLO*/
    if (init)
        {
        xsptr->plo = ALLOCATE (LAYOUT_STRUCT);
        first_trace = first_track = first_top = TRUE;
        }
    keystr = verbptr->keyptr;
    while (keystr IS_NOT_EQUAL_TO (MD_KEY_STRUCT *)NULL)
        {
        if (ARE_SAME (verb, "CROSS SECTION"))
            {
            status = xsz_fill_xs (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT HEADER"))
            {
            status = xsz_fill_lh (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT SCALE"))
            {
            status = xsz_fill_ls (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT SCALE BLOCK"))
            {
            status = xsz_fill_lsb (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT TRACE"))
            {
            status = xsz_fill_ltrace (keystr, first_trace, first_trace_keyword,
                                      last_trace_keyword, xsptr);
            first_trace_keyword = FALSE;
            first_trace = FALSE;
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT TRACK") OR
		 ARE_SAME (verb, "LOGPLOT LAYOUT"))
            {
            status = xsz_fill_ltrack (keystr, first_track, first_track_keyword,
                                      last_track_keyword, xsptr);
            first_track_keyword = FALSE;
            first_track = FALSE;
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT TOPS"))
            {
            status = xsz_fill_ltop (keystr, first_top, first_top_keyword,
                                    last_top_keyword, xsptr);
            first_top_keyword = FALSE;
            first_top = FALSE;
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT INTERVAL"))
            {
            status = xsz_fill_linterval (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT THINNING"))
            {
            status = xsz_fill_linterval (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        else if (ARE_SAME (verb, "LOGPLOT WELL SYMBOL"))
            {
            status = xsz_fill_lws (keystr, xsptr);
            if (status < SUCCESS)
                return status;
            }
        keystr = keystr->nxtkey;
        }
    /* If the verb is LOGPLOT TRACE OR    */
    /* LOGPLOT TRACK, or LOGPLOT TOPS     */
    /* insert it into the linked list.    */
    if (ARE_SAME (verb, "LOGPLOT TRACE"))
        {
        last_trace_keyword = TRUE;
        status = xsz_fill_ltrace (keystr, first_trace, first_trace_keyword,
                                  last_trace_keyword, xsptr);
        }
    else if (ARE_SAME (verb, "LOGPLOT TRACK"))
        {
        last_track_keyword = TRUE;
        status = xsz_fill_ltrack (keystr, first_track, first_track_keyword,
                                  last_track_keyword, xsptr);
        }
    else if (ARE_SAME (verb, "LOGPLOT TOPS"))
        {
        last_top_keyword = TRUE;
        status = xsz_fill_ltop (keystr, first_top, first_top_keyword, last_top_keyword,
                                xsptr);
        }
    return status;
    
    }
/* END:     */
