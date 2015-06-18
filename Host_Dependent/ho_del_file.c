/* DEC/CMS REPLACEMENT HISTORY, Element HO_DEL_FILE.C*/
/* *5     7-DEC-1990 10:26:14 MING "(SPR -1) add a buffer for filename on VMS. when we pass in some constant string FINDER link*/
/*won't let you modify it "*/
/* *4     9-NOV-1990 14:12:02 GILLESPIE "(SPR 1) Add #pragmas to quite compiles on VMS"*/
/* *3    17-AUG-1990 21:50:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:28 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:57:33 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_DEL_FILE.C*/
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

#include "esi_ho_files.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifdef vms
#include "esi_ho_desc.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Delete a named operating system file.

Prototype:
    publicdef INT ho_delete_file(FILENAME file, INT  *status);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) file        -(FILENAME) Valid OS File Name Char string.
    (O) status      -(INT *) Address of long integer to contain return status.
                        long integer status code: 0 = SUCCESS; non-zero = failure.

Return Value/Status:
    Function returns long integer status code: 0 = SUCCESS; non-zero = failure.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_delete_file (FILENAME file, INT *status) 
#else
publicdef INT ho_delete_file (file, status)
FILENAME file;
INT *status;
#endif
    {
#if UNIX
#ifdef mips
    extern INT unlink ();           /* 3/23/89 Julian: Not just Ultrix(r)! */
    
    return *status = unlink (file);        /*      --Standard 4.2 bsd -- */
#else	/* mips */
#ifdef sun
    extern INT unlink ();           /* 3/23/89 Julian: Not just Ultrix(r)! */
    
    return *status = unlink (file);        /*      --Standard 4.2 bsd -- */
#else   /* if not sun or mips, then ANSI */
    extern INT remove (CHAR * filename);
    
    return *status = remove (file);        /* ANSI "delete" file function */
#endif  /* sun */
#endif	/* mips */

#else   /* if not UNIX */

#ifdef vms
    INT mode;
    CHAR *p;
    FILENAME filename;
    
#pragma nostandard
    $DESCRIPTOR (deleted_file_desc, file);
#pragma standard
    /* set the deleted mode.           */
    
    strcpy (filename,file);	
    p = strrchr (filename, ';');
    if (p == NULL)
        strcat (filename, ";");
    
    mode = 0777;
    *status = ho_change_protection (filename, mode);
    deleted_file_desc.dsc$w_length = strlen (filename);
    *status = VAX_ROUTINE (lib$delete_file (&deleted_file_desc, 0, 0, 0, 0, 0, 0, 0,
                                            0));
    if (*status == 1)
        {                           /*   VAX SUCCESS code is 1    */
        *status = SUCCESS;
        }
    return *status;
#endif      /* VMS */

#ifdef primos
    extern INT delete ();
    
    return *status = delete (file);
#endif      /* PRIMOS */
#endif      /* UNIX */
    }
/* END:     */
