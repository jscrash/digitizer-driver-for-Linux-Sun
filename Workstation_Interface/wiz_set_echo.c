/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_SET_ECHO.C*/
/* *4     9-AUG-1990 22:55:59 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *3     9-AUG-1990 22:17:50 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *2     5-FEB-1990 23:30:19 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_SET_ECHO.C*/

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

NAME:    WIZ_SET_ECHO

DESCRIPTION:

    Set the state of the host echoing to the terminal.

    Input:    state, TRUE = echo, FALSE - do not echo.

    NOTE: this function temporarily nulled due to problems with VMS
          interface.

*************************************************************************/

#include "esi_wi_int.h"

#ifdef vms
#include "iodef.h"
#include "ttdef.h"
#include "tt2def.h"
#endif
#ifdef sun
#include <termio.h>
#endif
/************************************************************************/
publicdef INT wiz_set_echo(state)
/************************************************************************/
INT state;
{
}
#ifdef sun
extern struct termio sun_term_buf; 
#endif

#if 0
    PROGNAME("wiz_set_echo");
#define IDENT progname

#ifdef vms
    INT option;
    INT mode[3];
    INT mask = 0xFFFFFFFF;
    INT mode_length = sizeof mode;
    INT jtrmchn;
    INT status;
    INT iosb[2];
    INT i;
#endif

/************************************************************************/


    if ((state != FALSE) && (state != TRUE))  {
       RETURN(state);
    }

#ifdef vms
    if ((status = hoopenio(&jtrmchn, "TT:")) != SUCCESS)  {
            RETURN(status);
    }

                              /* sense current terminal characteristics */

    option = IO$_SENSEMODE;
    status = VAX_ROUTINE(SYS$QIOW(0,jtrmchn,option,iosb,0,0,
                     mode, mode_length,0,0,0,0));

    if (state == FALSE)  {
       mode[1] |= TT$M_NOECHO;			/* turn on NO_ECHO */
    }
    else {
        mode[1] &= mask ^ TT$M_NOECHO;		/* turn off NO_ECHO */
    }

    option = IO$_SETMODE;    
    status = VAX_ROUTINE(SYS$QIOW(0,jtrmchn,option,iosb,0,0,
                     mode, mode_length,0,0,0,0));
    status = hoclosio(jtrmchn);

#endif
#ifdef sun
  	if (state == TRUE)
	{
        sun_term_buf.c_lflag |= ECHO;
        sun_term_buf.c_lflag &= ~ECHOCRT;
	}
	else
	{
        sun_term_buf.c_lflag &= ~ECHO;
 	}	
        if (ioctl(0,TCSETA, &sun_term_buf) == -1)
                {
                perror ("wi_set_echo: ioctl to set term parameters failed");
                }
#endif


    RETURN(status);
}

#endif
