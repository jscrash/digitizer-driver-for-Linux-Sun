/* DEC/CMS REPLACEMENT HISTORY, Element HO_SPAWN.C*/
/* *9    15-AUG-1991 16:51:18 JTM "(SPR 3474) Added flag to send mail even if successful completion."*/
/* *8    22-JUL-1991 15:52:34 LOEBL "(SPR 8479) Use octal instead of hex on the integer constant."*/
/* *7    19-JUL-1991 14:39:49 LOEBL "(SPR 8479) UNIX - chmod to 777 the spawned file."*/
/* *6    30-OCT-1990 13:54:38 GILLESPIE "(SPR 37) Fix for Sun"*/
/* *5    17-AUG-1990 21:53:24 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    13-JUN-1990 23:31:43 JULIAN "(SPR -1) unix includes esi_stat.h not stat.h"*/
/* *3     5-SEP-1989 15:51:26 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    15-JUL-1989 17:10:19 GILLESPIE "(SPR 44) ADD TEST FOR EMPTY STRING FOR NON-BATCH SUBMITS"*/
/* *1    19-JUN-1989 12:59:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_SPAWN.C*/
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

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifdef primos
#include "keys.ins.cc"
#endif

#if UNIX

#ifdef sun
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#endif
/* Function Description -----------------------------------------------------
Description:
    Start a background task running a given COMI or CPL file.

Prototype:
    publicdef INT ho_spawn (INT type, CHAR *filename, CHAR *args, INT  *user);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) type        -(INT) SPAWN_PHANTOM to create detached process; 
                        other for batch.
    (I) filename    -(CHAR *) Name of OS file containing commands.
    (I) args        -(CHAR *) Argument string for command line.
    (I) user        -(INT *) User number of phantom/detached process. 
                        Not used in VMS.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_spawn (INT type, CHAR *filename, CHAR *args, INT *user) 
#else
publicdef INT ho_spawn (type, filename, args, user)
INT type;
CHAR *filename;
CHAR *args;
INT *user;
#endif

#ifdef vms
    {
    INT status;
    CHAR cmd[511];
    BOOL wait_flag;
    CHAR submit[21];
    
    if (type EQUALS SPAWN_PHANTOM)
        {
        wait_flag = 0;
        sprintf (cmd, "@%s %s", filename, IS_EMPTY_STRING (args) ? "" : args);
        }
    else
        {
        wait_flag = 1;
        strcpy (submit, "SUBMIT");
        if (type EQUALS SPAWN_BATCH_NOLOG)
            {
            strcat (submit, "/NOLOG");
            }
        if (IS_EMPTY_STRING (args))
            {
            sprintf (cmd, "%s %s", submit, filename);
            }
        else
            {
            sprintf (cmd, "%s/PARAMETER=(%s) %s", submit, args, filename);
            }
        }
    status = ho_syscom_nw (cmd, wait_flag);
    return(status);
    }


#endif

#ifdef primos
    {
    INT status;
    SHORT cplflg, funit, userno, filelen, argslen, action, code;
    FILENAME xfilename, origin, filename_only;
    CHAR xargs[128];
    CHAR cmd[162];
    BOOL wait_flag, stayhere;
    CHAR *rindex (), *p;
    
    fortran phntm$();
    
    wait_flag = FALSE;
    stayhere = TRUE;
    funit = 6;
    userno = 0;
    cplflg = 0;
    status = SUCCESS;
    
    /* THIS IS TEMPORARY - just until we get ho_syscom working!!! */
    /*  type = SPAWN_PHANTOM;   - lets see if its ok now VSC:3/14/89 */
    
    strcpy (xfilename, filename);   /*  make a local copy of strings. */
    strcpy (xargs, args);
    
    ts_convert_char (xargs, ',', ' ');
    ts_convert_char (xargs, '\"', '\'');
    argslen = strlen (xargs);
    
    /* first, get p to point to the file name part of the pathname */
    if (type EQUALS SPAWN_PHANTOM)
        {
        if (p = rindex (xfilename, '>'))
            {
            stayhere = FALSE;
            *p++ = '\0';
            
            /* attach to directory, remembering where we are for later */
            status = ho_change_directory (xfilename, origin);
            if (status)
                return(status);
            }
        else
            {
            p = xfilename;
            }
        strcpy (filename_only, p);
        
        /* append '.CPL' if not there already */
        filelen = strlen (filename_only);
        if (ts_strcmpi (filename_only + filelen - 4, ".CPL") != 0)
            {
            strcat (filename_only, ".CPL");
            filelen += 4;
            }
        cplflg = 1;
        phntm$(cplflg, filename_only, filelen, funit, userno, code, xargs, argslen);
        }
    else
        {
        /* NOTE: the JOB command does not need the .CPL extension */
        filelen = strlen (xfilename);
        cplflg = 1;
        userno = 0;
        if (cplflg AND argslen)
            sprintf (cmd, "JOB %s -ARGS %s", xfilename, xargs);
        else
            sprintf (cmd, "JOB %s", xfilename);
        
        status = ho_syscom_nw (cmd, wait_flag);
        }
    if (!stayhere)
        {
        status = ho_change_directory (origin, NULL);    /* back to original dir */
        }
    /* if we have write access to the pointer return the phantom user number */
    if (am_check_pointer (user, "w") == SUCCESS)
        *user = userno;
    
    return(status);
    }


#endif

#if UNIX
    {
    CHAR *tempnam ();
    
    INT flags;
    INT mode;
    INT status;
    INT desc;
    INT nbytes;
    CHAR cmd[511];
    BOOL wait_flag;
    CHAR *fname;
    
    ts_convert_char (args, ',', ' ');
    sprintf (cmd, "%s %s", filename, IS_EMPTY_STRING (args) ? "" : args);
    nbytes = strlen (cmd) + 1;
    wait_flag = 0;
    
    /* make sure the file is executable */
/*    mode = S_IREAD | S_IWRITE | S_IEXEC;
      chmod (filename, mode);    
*/
    /* make sure the file is everything */
    chmod (filename, (int)0777);  

    
    if (type != SPAWN_PHANTOM)
        {
        flags = O_WRONLY | O_CREAT;
        mode = S_IREAD | S_IWRITE | S_IEXEC;
        fname = tempnam ("/tmp", NULL);
        desc = open (fname, flags, mode);
        write (desc, cmd, nbytes);
        close (desc);
        sprintf (cmd, "batch -mc %s", fname);
        }
    status = ho_syscom_nw (cmd, wait_flag);
    return(status);
    }


#endif
/* END:     */
