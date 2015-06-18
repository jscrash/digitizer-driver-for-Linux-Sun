/* DEC/CMS REPLACEMENT HISTORY, Element HO_SYSTIME.C*/
/* *2    17-AUG-1990 21:53:58 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:59:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_SYSTIME.C*/
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

#include "esi_gl_defs.h"

#ifdef primos

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Returns system clock time for elapsed CPU and DISK, and WALL.                                                *

Prototype:
    publicdef void hosystime (FLOAT *cpu_time, FLOAT *disk_time, FLOAT *wall_time, 
        CHAR date[7]);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) cpu_time    -(FLOAT *) Cpu time.
    (O) disk_time   -(FLOAT *) Disk time.
    (O) wall_time   -(FLOAT *) Wall time.
    (O) date        -(CHAR []) Date.

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef void hosystime (FLOAT *cpu_time, FLOAT *disk_time, FLOAT *wall_time,
                          CHAR date[7]) 
#else
publicdef void hosystime (cpu_time, disk_time, wall_time, date)
FLOAT *cpu_time, *disk_time, *wall_time;
CHAR date[7];
#endif
    {
    struct 
        {
        char date[6];
        short time_min;
        short time_sec;
        short time_tic;
        short cpu_sec;
        short cpu_tic;
        short disk_sec;
        short disk_tic;
        short tic_sec;
        short user_number;
        char username[32];
        } timdat_info;
    short length = 28, i;
    float secs_per_tic;
    fortran void timdat ();
    
    timdat (timdat_info, length);
    
    for (i = 0; i < 6; i++)
        date[i] = timdat_info.date[i];
    date[6] = NIL;
    
    secs_per_tic = 1.0 / (float)timdat_info.tic_sec;
    
    *wall_time = (float)timdat_info.time_tic * secs_per_tic +
                 (float)timdat_info.time_sec + (float)timdat_info.time_min * 60.0;
    
    *cpu_time = (float)timdat_info.cpu_tic * secs_per_tic + (float)timdat_info.cpu_sec;
    
    *disk_time = (float)timdat_info.disk_tic * secs_per_tic +
                 (float)timdat_info.disk_sec;
    
    }


#endif
/* END:     */
