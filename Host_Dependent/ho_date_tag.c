/* DEC/CMS REPLACEMENT HISTORY, Element HO_DATE_TAG.C*/
/* *3    17-AUG-1990 21:50:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     6-FEB-1990 10:46:36 MING "(SPR 0) change the date tag to a new format 'YYMMDD_HHMISS'"*/
/* *1    19-JUN-1989 12:57:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_DATE_TAG.C*/
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
#include "esi_ho.h"

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#include "esi_time.h"

/* Function Description -----------------------------------------------------
Description:
    This routine returns a date/time character string which can be used as
    a suffix for file names such as for Phantom command files or COMO file.

    Format of string is YYMMDD_HHMISS\0                            

Prototype:
    publicdef CHAR *ho_date_tag (CHAR *buffer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) buffer      -(CHAR *) Pointer to buffer for output string (CST) or 0.
                        If input pointer is 0 an internal buffer will be used.  
                        Routine fills buffer with date/time string and returns 
                        pointer to first byte of buffer.

Return Value/Status:
    Returns a date/time suffix for file names.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ho_date_tag (CHAR *buffer) 
#else
publicdef CHAR *ho_date_tag (buffer)
CHAR *buffer;
#endif
    {
    time_t current_time;
    CHAR *lp, full_date[33], local_month[4];
    CHAR month[3], day[3], year[3], hour[3], minute[3], second[3];
    
    static CHAR local_buffer[14];
    
    static CHAR *months[] = 
        {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
        0
        };
    
    if (buffer)
        lp = buffer;
    else
        lp = local_buffer;
    
    time (&current_time);
    strcpy ((char *)full_date, ctime (&current_time));

#if UNIX
    strncpy ((char *)local_month, (char *) & full_date[4], 3);
    local_month[3] = 0;
    strncpy ((char *)day, (char *) & full_date[8], 2);
    day[2] = 0;
    strncpy ((char *)year, (char *) & full_date[22], 2);
    year[2] = 0;
    strncpy ((char *)hour, (char *) & full_date[11], 2);
    hour[2] = 0;
    strncpy ((char *)minute, (char *) & full_date[14], 2);
    minute[2] = 0;
    strncpy ((char *)second, (char *) & full_date[17], 2);
    second[2] = 0;
#else

#ifdef primos
    strncpy ((char *)local_month, (char *) & full_date[3], 3);
    local_month[3] = 0;
    strncpy ((char *)day, (char *) & full_date[0], 2);
    day[2] = 0;
    strncpy ((char *)year, (char *) & full_date[7], 2);
    year[2] = 0;
    strncpy ((char *)hour, (char *) & full_date[10], 2);
    hour[2] = 0;
    strncpy ((char *)minute, (char *) & full_date[13], 2);
    minute[2] = 0;
    strncpy ((char *)second, (char *) & full_date[16], 2);
    second[2] = 0;
#endif      /* PRIMOS */

#ifdef vms
    strncpy ((char *)local_month, (char *) & full_date[4], 3);
    local_month[3] = 0;
    strncpy ((char *)day, (char *) & full_date[8], 2);
    day[2] = 0;
    strncpy ((char *)year, (char *) & full_date[22], 2);
    year[2] = 0;
    strncpy ((char *)hour, (char *) & full_date[11], 2);
    hour[2] = 0;
    strncpy ((char *)minute, (char *) & full_date[14], 2);
    minute[2] = 0;
    strncpy ((char *)second, (char *) & full_date[17], 2);
    second[2] = 0;
#endif      /* VMS */
#endif      /* UNIX */
    
    /*  translate character month to integer month (ie. "JAN" ==> "01")  */
    local_month[3] = 0;
    sprintf ((char *)month, "%02d", (ts_switch (local_month, months) + 1));
    
    /*  make sure day and hour are zero filled.  */
    if (day[0] == ' ')
        day[0] = '0';
    if (hour[0] == ' ')
        hour[0] = '0';
    
    day[2] = 0;
    year[2] = 0;
    hour[2] = 0;
    minute[2] = 0;
    second[2] = 0;
    
    sprintf (lp, "%s%s%s_%s%s%s", year, month, day, hour, minute, second);
    lp[13] = 0;
    
    return lp;
    }
/* END:     */
