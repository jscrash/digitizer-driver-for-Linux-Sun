/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_REST_TERM.C*/
/* *5     9-AUG-1990 22:55:53 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *4     9-AUG-1990 22:17:40 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *3     5-FEB-1990 23:29:41 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *2    21-SEP-1989 13:58:39 VINCE "(SPR -1) added lookup of mac_device for vms systems"*/
/* *1    19-JUN-1989 13:36:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_REST_TERM.C*/

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*************************************************************************

NAME:    WI_RESTORE_TERMINAL

DESCRIPTION:

      This routine serves to restore the system terminal characteristics
   to that which existed when Finder was invoked.  It accepts 1 parameter,
   which, if non-zero, will exit to the operating system.
........ disabled
   This behavior
   is used when the user 'aborts' Finder by pressing the system 'Interrupt'
   key (^Y for VMS, ^P for PRIMOS).
........
   This function is also called from
   WI_TERMINATE when terminating Finder normally, in which case it should
   not exit, but return to the routines which are controlling the termination.

*************************************************************************/

#include "esi_c_lib.h"
#include "esi_wi_int.x"		/* external definitions needed here */
#include "esi_rs.h"
#include "esi_qs.h"

#ifdef vms
#include "iodef.h"
#include "ttdef.h"
#include "tt2def.h"
#endif

#ifdef sun
#include <termio.h>
#endif

/************************************************************************/
publicdef INT wiz_restore_terminal(exitflag)
INT exitflag;
/************************************************************************/

    {
#ifdef vms
    INT option;
    INT mode[3];
    INT mask = 0xFFFFFFFF;
    INT mode_length = sizeof mode;
    INT jtrmchn;
    INT status;
    INT iosb[2];
    INT i;
    long ctrl_mask = 0x02100000;    /* LIB$M_CLI_CTRLT | LIB$M_CLI_CTRLY; */
    long prev_mask;		    /* just needed for LIB$ENABLE_CTRL call */
    char mac_device[31];
#endif

#ifdef sun
    extern struct termio sun_term_buf, sun_term_save;
#endif

/************************************************************************/

#ifdef vms
                         /* re-install terminal characteristics
                            that were saved during WI_INITIALIZE... */

/* remove this call and the 'hoclosio' in WI_INIT.C if installing
   a CTL-Y AST...  */

    qs_inq_c (QS_WORKSTATION_MAC_DEVICE, mac_device, (INT*)0);
    if (strlen(mac_device) == 0) strcpy (mac_device, "TT:");

    if ((status = hoopenio(&jtrmchn, mac_device)) != SUCCESS)  
        {
        return status;
        }

    for (i = 0; i < 3; i++)
        {
	mode[i] = wiv.terminal_setting[i];
        }

    option = IO$_SETMODE;    
    status = VAX_ROUTINE(SYS$QIOW(0,jtrmchn,option,iosb,0,0,
                     mode, mode_length,0,0,0,0));

    status = hoclosio(jtrmchn);

                         /* Cleanup any journal/playback in progress... */

    wiz_jp_terminate();

                         /* Return to OS if desired... */

    if (exitflag) exit(EXIT_SUCCESS);
#endif

#ifdef sun
    if (ioctl(0,TCSETAF, &sun_term_save) == -1)
       {
       perror ("wiz_restore_terminal: ioctl to set term parameters failed");
       }
#endif

    return SUCCESS;
    }
