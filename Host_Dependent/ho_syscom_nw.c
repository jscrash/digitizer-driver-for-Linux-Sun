/* DEC/CMS REPLACEMENT HISTORY, Element HO_SYSCOM_NW.C*/
/* *4    17-AUG-1990 21:53:52 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    26-FEB-1990 18:59:09 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2     1-NOV-1989 10:12:41 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *1    19-JUN-1989 12:59:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_SYSCOM_NW.C*/
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
#include "esi_ho.h"
#include "esi_ho_err.h"

#ifdef primos

#ifndef  ESI_TS_H

#include "esi_ts.h"

#endif
#endif

#ifdef vms

#pragma nostandard

#include descrip
#include ssdef
#include jpidef
#include iodef

#pragma standard

#endif

/* Function Description -----------------------------------------------------
Description:
    Execute a system command from within a program.

Prototype:
    publicdef INT  ho_syscom_nw (CHAR *string, BOOL wait_flag);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) System command.
    (I) wait_flag   -(BOOL) 0 = wait for completion; 1 = do not wait.

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    Function also returns status from system call.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_syscom_nw (CHAR *string, BOOL wait_flag) 
#else
publicdef INT ho_syscom_nw (string, wait_flag)
CHAR *string;
BOOL wait_flag;
#endif
    {
#ifdef primos
    SHORT com_status;
    CHAR command[1026], dummy[10];
    SHORT invok_status, ttyon, ttyoff, zero, status;
    fortran cp$(), como$$();
    
    ts_cs_to_vcs (command, string);
    
    invok_status = 0;
    status = 0;
    zero = 0;
    ttyon = 2;
    ttyoff = 1;
    strcpy (dummy, "");
    
    /*  como$$ ( &ttyoff, dummy, &zero, &zero, &status); /* turn off TTY output */
    cp$(command, invok_status, com_status);
    /*  como$$ ( &ttyon, dummy, &zero, &zero, &status); /* turn on TTY output */
    
    if (com_status != 0)
        com_status = HO_ERR_VMS;
    
    return com_status;

#endif

#if UNIX
        {
        INT status;
        
        status = system (string);
        
        return status;
        }
#endif

#ifdef vms
    /*  Mailbox-driver host command processor
        22 July 88  J Gillespie
        The first time through, a VMS mailbox is created with the logical name
        ESI$SERVER_MBX.  VMS automatically creates the mailbox and assigns it
        a channel number.  Next, a new process is created using LIB$SPAWN.  The
        command procedure ESI$BIN:ESI_SERVER is initiated and passed two
        parameters: the logical name of the communications mailbox and the name
        of the current process (for later attaching).  After opening the mailbox,
        the server reattaches to the current (calling) process.  The process ID
        number of this spawned process is saved.
    
        For each call to HO_SYSCOM_NW, the command is written to the opened
        mailbox using SYS$QIOW.  Following successful write, the current process
        reattaches to the process previously spawned by using the saved process ID.
        The command procedure being executed by the server then reads the mailbox,
        executes the command and reattaches to the parent process.  This context
        switch occurs quickly.  Note that we pay the price for process
        initialization only once.  Also, any logical names defined at the
        process level remain active.
    
        One thing to remember is that process level logical names are not known
        to a spawned process' parent.  Use the JOB logical names to make this
        happen, as in DEFINE/JOB MYDIR DUA0:[MYDIR]...
    
        To make the detached process go away, send the command LOGOUT to it.
    
        If the detached process dies then we start over...
    */
    
    INT status;
    USHORT i;
    CHAR blank = ' ';
    static INT spawned_process_id = 0;  /* ID of detached process */
    static SHORT channel;       /* channel number to mailbox */
    short iosb[4];                  /* I/O status block */
    CHAR command_line[255];
    
    if (spawned_process_id EQUALS 0)
        {
        
        /*      --> --> -->  BEGIN FIRST-TIME INITIALIZATION  <-- <-- <--         */
        
        static CHAR command_file[] = "ESI$BIN:ESI_SERVER";
        static CHAR mailbox_logical_name[] = "ESI$SERVER_MBX";
        CHAR current_process_id[16];
        INT len;
#pragma nostandard
        $DESCRIPTOR (d, command_line);
        $DESCRIPTOR (m, mailbox_logical_name);
#pragma standard
        
        /* Create a mailbox                            */
        
        status = VAX_ROUTINE (SYS$CREMBX (0, &channel, sizeof command_line, 0, 0, 0,
                                          &m));
        if (NOT (status & 1))
            {
            return HO_ERR_VMS;
            }
        /* get the id of the current process           */
        
        status = hoz_getjpi (JPI$_PID, current_process_id, sizeof current_process_id,
                             0);
        if (NOT (status & 1))
            {
            return HO_ERR_VMS;
            }
        /* construct the command line to start the     */
        /* server                                      */
        
        d.dsc$w_length = sprintf (command_line, "@%s %s %s", command_file,
                                  mailbox_logical_name, current_process_id);
        
        /* create a process and run the special server */
        /* process file                                */
        
        status = VAX_ROUTINE (LIB$SPAWN (&d, 0, 0, 0, 0, &spawned_process_id));
        if (NOT (status & 1))
            {
            return HO_ERR_VMS;
            }
        /*         --> --> -->  END FIRST-TIME INITIALIZATION  <-- <-- <--         */
        }
    /******************************************************************************/
    /*                      Main Message Passing Routine                          */
    /******************************************************************************/
    
    if ((i = strlen (string)) > 0)
        {
        
        /* Copy the input string to the command line   */
        /* with blank pad                              */

#pragma nostandard
        VAX_ROUTINE (LIB$MOVC5 (&i, string, &blank, &sizeof command_line,
                                command_line));
#pragma standard
        
        /* Write the command to the mailbox            */
        
        status = VAX_ROUTINE (SYS$QIOW (0, channel, IO$_WRITEVBLK | IO$M_NOW, iosb, 0,
                                        0, command_line, sizeof command_line, 0, 0, 0,
                                        0));
        if (NOT (status & 1) OR NOT (iosb[0] & 1))
            {
            return HO_ERR_VMS;
            }
        /* reattach to the server process - the
        process will reattach to this process upon
        completion of the command                   */
        
        status = VAX_ROUTINE (LIB$ATTACH (&spawned_process_id));
        if (status EQUALS SS$_NORMAL)
            {
            status = SUCCESS;
            }
        else if (status EQUALS SS$_NONEXPR)
            {
            /* process has expired - start another         */
            
            spawned_process_id = 0;
            status = VAX_ROUTINE (SYS$DELMBX (channel));
            status = VAX_ROUTINE (SYS$DASSGN (channel));
            return ho_syscom_nw (string, wait_flag);
            }
        else
            {
            status = HO_ERR_VMS;
            }
        }
    return(status);
#endif
    }
/* END:     */
