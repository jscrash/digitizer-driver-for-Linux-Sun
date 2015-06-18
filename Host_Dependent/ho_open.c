/* DEC/CMS REPLACEMENT HISTORY, Element HO_OPEN.C*/
/* *6    14-AUG-1991 11:57:24 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *5    17-AUG-1990 21:52:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    26-FEB-1990 18:58:59 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *3    24-SEP-1989 23:39:16 GILLESPIE "(SPR 102) Add lsc support"*/
/* *2    18-SEP-1989 12:43:24 JULIAN "Gulf mods under SPR 100"*/
/* *1    19-JUN-1989 12:59:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_OPEN.C*/
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
#include "esi_ho_files.h"
#include "esi_ts.h"

#if UNIX

#include "esi_c_lib.h"

#endif
static CHAR *ansi_table[16] =   /* table of ANSI access modes */
    {
    "r",                            /* text read            */
    "w",                            /* text write           */
    "a",                            /* text append          */
    "rb",                           /* binary read          */
    "wb",                           /* binary write         */
    "ab",                           /* binary append        */
    "r+",                           /* text read/update     */
    "w+",                           /* text write/update    */
    "a+",                           /* text append/update   */
    "rb+",                          /* binary read/update   */
    "wb+",                          /* binary write/update  */
    "ab+",                          /* binary append/update */
    "r+b",                          /* same as rb+          */
    "w+b",                          /* same as wb+          */
    "a+b",                          /* same as ab+          */
    NULL                            /* empty string ends the list */
    };



/* Function Description -----------------------------------------------------
Description:
    Host dependent routine to call ANSI C-library fopen().

Prototype:
    publicdef FILE *ho_open (FILENAME file_name, CHAR *mode, CHAR *share_mode);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) file_name       -(FILENAME) File to open.
    (I) mode            -(CHAR *) Modde file is to be opened with (r/w ...)
    (I) share_mode      -(CHAR *) VMS only.

Return Value/Status:
    Function returns the pointer to the open file.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef FILE *ho_open (FILENAME file_name, CHAR *mode, CHAR *share_mode) 
#else
publicdef FILE *ho_open (file_name, mode, share_mode)
FILENAME file_name;
CHAR *mode;
CHAR *share_mode;
#endif
    {
    INT i;
    FILE *file_pointer;
    CHAR *xmode;

#ifdef THINK_C
    return fopen (file_name, mode);
#endif  /* end of LSC */

#ifdef applec
    return fopen (file_name, mode);
#endif  /* end of MPW */

#if UNIX
    INT status;
    
    static CHAR *sun_table[16] = 
        {
        "r",
        "w",
        "a",
        "r",
        "w",
        "a",
        "r+",
        "w+",
        "a+",
        "r+",
        "w+",
        "a+",
        "r+",
        "w+",
        "a+",
        NULL
        };
    
    for (i = 0; ansi_table[i] != NULL; i++) /* locate MODE in the */
        {                           /* ANSI mode string table */
        if (ARE_SAME (mode, ansi_table[i]))
            break;                  /* match found, so stop searching */
        }
    /* translate ANSI mode to SUN mode */
    if (ansi_table[i] == NULL)      /* END-of-TABLE reached ??? */
        xmode = sun_table[0];       /* YES - so just use "r".   */
    else
        xmode = sun_table[i];       /* a match was found, so use it. */
    
    file_pointer = fopen (file_name, xmode);    /* open the file */
    
    /* If the file was opened for writing, then we
       must use a heavy-duty network lock to REALLY lock the file.  If  
       this lock is blocked by other locks, the process will sleep until
       the lock may be applied. */
    
    if (file_pointer == (FILE *)0)
        return(FILE *)0;
    
#if 0 /* comment out - we decide to put locking/unlocking in the application */
#include <fcntl.h>
    struct flock lock_param;
    if (ARE_DIFFERENT (xmode, "r") && ARE_DIFFERENT (xmode, "rb"))
        {
        lock_param.l_type = F_WRLCK;
        lock_param.l_whence = 0;
        lock_param.l_start = 0;
        lock_param.l_len = 0;
        status = fcntl (fileno (file_pointer), F_SETLKW, &lock_param);
        }
#endif
#endif      /* UNIX */

#ifdef vms
    /*  The VMS run-time library does funny things
        with FORTRAN files, e.g.  translating the
        first character as some combination of new
        page, no cr, etc.  The VMS-only option
        switch suppresses this behavior         */
    
    if (share_mode != NULL)
        file_pointer = fopen (file_name, mode, share_mode);
    else
        file_pointer = fopen (file_name, mode, "ctx=nocvt");
#endif

#ifdef primos
    static CHAR *prim_table[16] =/* table of PRIME C access modes, */
        {                           /* a conversion for ANSI_TABLE to PRIMOS. */
        "r",
        "w",
        "wa",
        "i",
        "o",
        "oa",
        "i+",
        "o+",
        "oa+",
        "i+",
        "o+",
        "oa+",
        "i+",
        "o+",
        "oa+",
        NULL                        /* empty string ends the list */
        };
    
    /* look thru ANSI-modes for a match */
    for (i = 0; ansi_table[i] != NULL; i++) /* locate MODE in the */
        {                           /* ANSI mode string table */
        if (strcmp (mode, ansi_table[i]) == 0)
            break;                  /* match found, so stop searching */
        }
    /* translate ANSI mode to PRIMOS mode */
    if (ansi_table[i] == NULL)      /* END-of-TABLE reached ??? */
        xmode = prime_table[0];     /* YES - so just use "r".   */
    else
        xmode = prime_table[i];     /* a match was found, so use it. */
    
    file_pointer = fopen (file_name, xmode);    /* open the file */
#endif      /* PRIMOS */
    return file_pointer;
    }                               /* end of HO_OPEN */
/* END:     */
