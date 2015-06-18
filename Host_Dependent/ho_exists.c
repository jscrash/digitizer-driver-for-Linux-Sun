/* DEC/CMS REPLACEMENT HISTORY, Element HO_EXISTS.C*/
/* *4    17-AUG-1990 21:51:13 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    26-FEB-1990 18:58:31 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2     5-SEP-1989 15:49:43 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *1    19-JUN-1989 12:57:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_EXISTS.C*/
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

#ifdef vms
#include "esi_stat.h"
#endif

#if UNIX
#include "esi_stat.h"
#endif

/* Function Description -----------------------------------------------------
Description:
   Verifies the existence of a file system object.

Prototype:
    publicdef INT hoexists (FILENAME pathname);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pathname    -(FILENAME) File system object to verify.

Return Value/Status:
    See below.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoexists (FILENAME pathname) 
#else
publicdef INT hoexists (pathname)
FILENAME pathname;
#endif
    {
    INT does_exist = FALSE;
    FILENAME testname;
    static char **e;

#ifdef primos
    static char *exts[] = 
        {
        "",
        ".RB",
        ".RUN",
        ".CPL",
        ".FRM",
        ".SQL",
        ".SDL",
        NULL
        };
    
    for (e = exts; (!does_exist) && *e; e++)
        {
        sprintf (testname, "%s%s", pathname, *e);
        does_exist = fexists (testname);
        }
#endif

#ifdef vms
    static char *exts[] = 
        {
        "",
        ".RB",
        ".EXE",
        ".COM",
        ".FRM",
        ".SQL",
        ".SDL",
        NULL
        };
    struct stat buffer;
    INT return_value;
    INT stat ();
    
    for (e = exts; (!does_exist) && *e; e++)
        {
        sprintf (testname, "%s%s", pathname, *e);
        return_value = stat (testname, &buffer);
        if (return_value != -1)
            does_exist = TRUE;
        }
#endif

#if UNIX
    static char *exts[] = 
        {
        "",
        ".rb",
        ".exe",
        ".com",
        ".frm",
        ".sql",
        ".sdl",
        ".RB",
        ".EXE",
        ".COM",
        ".FRM",
        ".SQL",
        ".SDL",
        ".Rb",
        ".Exe",
        ".Com",
        ".Frm",
        ".Sql",
        ".Sdl",
        NULL
        };
    FILENAME fullpath;
    struct stat buffer;
    INT return_value;
/*    INT stat (); */
    
    for (e = exts; (!does_exist) && *e; e++)
        {
        sprintf (testname, "%s%s", pathname, *e);
        ho_translate_filename (testname, fullpath); /* expand any symbols */
        return_value = stat (fullpath, &buffer);
        if (return_value != -1)
            does_exist = TRUE;
        }
#endif
    return(does_exist);
    }
/* END:     */
