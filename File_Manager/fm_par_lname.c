/* DEC/CMS REPLACEMENT HISTORY, Element FM_PAR_LNAME.C*/
/* *3    14-AUG-1990 13:25:07 VINCE "(SPR 5644) Header Standardization"*/
/* *2     7-NOV-1989 13:23:43 GILLESPIE "(SPR 30) Use esi_fm_int.h include file"*/
/* *1    19-JUN-1989 12:50:51 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FM_PAR_LNAME.C*/
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
#include "esi_fm_int.h"
#include "esi_qs.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to parse the supplied long name into its
    constituent parts (creator and Finder filename).

Prototype:
    publicdef INT fm_parse_long_name(FM_LONG long_name,ANALYST_NAME creator,FM_FILE filename);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    long_name       -(FM_LONG)
    creator         -(ANALYST_NAME)
    filename        -(FM_FILE)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fm_parse_long_name (FM_LONG long_name, ANALYST_NAME creator,
                                  FM_FILE filename)
#else
publicdef INT fm_parse_long_name (long_name, creator, filename)
FM_LONG long_name;
ANALYST_NAME creator;
FM_FILE filename;
#endif
    {
    CHAR *ptr;
    INT i;
    
    /*****************************************************************************/
    
    /* * LONG NAME CONTAINS CREATOR NAME * */
    /* ----------------------------------- */
    
    if ((ptr = strchr (long_name, ':')) != (CHAR *)0)
        {
        i = ptr - long_name;
        strncpy (creator, long_name, i);
        creator[i] = '\0';
        
        ptr++;
        strcpy (filename, ts_snowhite (ptr));
        }
    /* * LONG NAME DOES NOT CONTAIN CREATOR NAME * */
    /* ------------------------------------------- */
    
    else
        {
        qs_inq_c (QS_FINDER_ACCOUNT, creator, (INT *)0);
        strcpy (filename, ts_snowhite (long_name));
        }
    ts_sto_upper (creator);
    return SUCCESS;
    }
/* END:     */
