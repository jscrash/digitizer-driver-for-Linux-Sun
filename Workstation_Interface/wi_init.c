/* DEC/CMS REPLACEMENT HISTORY, Element WI_INIT.C*/
/* *10    9-AUG-1990 22:56:04 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *9     9-AUG-1990 22:17:59 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *8    27-JUN-1990 17:29:35 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *7     2-MAR-1990 12:07:35 JULIAN "(SPR 6012) fix if USE_X == 0 to be ifndef USE_X"*/
/* *6     2-MAR-1990 11:31:18 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *5     5-FEB-1990 23:37:20 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *4    21-SEP-1989 13:57:44 VINCE "(SPR -1) added lookup of mac_device for vms systems"*/
/* *3     6-SEP-1989 09:59:37 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    20-JUL-1989 11:09:00 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:37:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_INIT.C*/
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

NAME:    WI_INITIALIZE

DESCRIPTION:

   Initialize Workstation Interface and MAC
   Input:
          wkid = (INT) GKS workstation ID.
         gtype = (INT) type of graphic workstation, 0 = non-graphic.
         ttype = (INT) type of alphanumeric workstation, 0 = dumb.
       jp_mode = (INT) JOURNAL, PLAYBACK or 0
       jp_name = (CHAR *) journal file name

*************************************************************************/

#include "esi_wi_int.h"
#ifndef ESI_RS_H
#include "esi_rs.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifdef vms
#include "iodef.h"
#include "ttdef.h"
#include "tt2def.h"
#include "ssdef.h"
#endif

#ifdef sun
#include <termio.h>
#include "esi_c_lib.h"
#include "esi_qs.h"
publicdef struct termio sun_term_buf, sun_term_save;
#endif


#define MAC_MINIMUM_VERSION 4054

#ifdef vms
static INT jtrmchn;
#endif

publicdef WIV_STRUCT wiv;

/************************************************************************/
publicdef INT wi_initialize (wkid, gtype, ttype, jp_mode, jp_name)
/************************************************************************/

INT wkid;
INT gtype;                    /* OBSOLETE */
INT ttype;                    /* Wkstn terminal type */
INT jp_mode;
CHAR *jp_name;
    {
    PROGNAME ("WI_INITIALIZE");
    INT version;
    INT dontcare;
    INT chantype;
    INT *num;
    INT chars;
    INT charat, stat;
    CHAR stresp[STMAX];
    INT i;
#ifdef sun
    CHAR mac_device[31];
#endif
#ifdef vms
    INT option;
    INT mode[3];
    INT mask = 0xFFFFFFFF;
    INT mode_length = sizeof mode;
    INT status;
    INT iosb[2];
    long ctrl_mask = 0x02100000;    /* LIB$M_CLI_CTRLT | LIB$M_CLI_CTRLY; */
    long prev_mask;
#endif

    /************************************************************************/

#define IDENT progname

    tc_zeroes (&wiv, sizeof wiv);    /* zero out the wiv structure */

    wiv.ittype = ttype;
    wiv.iwkid = wkid;
    wiv.ifwiinit = TRUE;
    wiv.stock_select_cdlgid = -1;
    wiv.current.owner_id = WI_NO_OWNER_ID;
    strcpy (wiv.current.owner_tag, "ORPHAN");
    strcpy (wiv.mn, "WI");
#ifdef USE_X
    wiv.dialog_menu = (DIALOG_MENU *) tc_zalloc (sizeof(DIALOG_MENU) * 100);
#endif

    /* Initialize tree for use by WI */
    btinit (&wiv.input_tree, 0, sizeof(INT), sizeof(INPUT_RECORD),
            BT_TREE_SYSTEM);

#ifndef USE_X      /* ESITERM code */
    for (i = 0; i < NUM_EVENT_TYPES; i++)
        {
        wiv.event[i].event_code = -1;
        wiv.event[i].mode = WI_REQUEST_MODE;
        }
    wiv.event[8].event_code = RQMN_CODE;
    wiv.event[9].event_code = RQCD_CODE;
    wiv.event[10].event_code = RQWS_CODE;
    wiv.event[11].event_code = FMEN_CODE;

    wiv.macon = MACON;
    wiv.macoff = MACOFF;

#ifdef vms

    /* get current terminal characteristics
        then set those needed for ESITERM.
        These will be reset on image termination */

    if ((status = hoopenio (&jtrmchn, "TT:")) != SUCCESS)
        {
        RETURN (status);
        }

    /* disable the CLI's ctrl-Y AST */
    /* save current terminal characteristics */

    option = IO$_SENSEMODE;
    status = VAX_ROUTINE (SYS$QIOW (0, jtrmchn, option, iosb, 0, 0, mode,
                                    mode_length, 0, 0, 0, 0));
    for (i = 0; i < 3; i++)
        {
        wiv.terminal_setting[i] = mode[i];
        }

    /* install the Finder term-characteristics */

    mode[1] &= mask ^ TT$M_WRAP;    /* turn off wrap */
    mode[1] &= mask ^ TT$M_MECHTAB;    /* turn off machine tabs */
    mode[1] |= TT$M_NOBRDCST;    /* turn off broadcast */
    mode[1] |= TT$M_READSYNC;    /* turn on ctrlS/ctrlQ */
    mode[2] &= mask ^ TT2$M_PASTHRU;    /* turn off pasthru */
    mode[2] |= TT2$M_DECCRT;    /* enable DECCRT (VT100) */

    option = IO$_SETMODE;
    status = VAX_ROUTINE (SYS$QIOW (0, jtrmchn, option, iosb, 0, 0, mode,
                                    mode_length, 0, 0, 0, 0));

    /* remove following if installing the CTL-Y AST and modify
       WIZ_RESTORE_TERMINAL such that it does not open it on entry.  */

    hoclosio (jtrmchn);
#endif
#ifdef sun
    if (ioctl (0, TCGETA, &sun_term_buf) == -1)
        {
        perror ("wi_init: ioctl to get term parameters failed");
        }
    sun_term_save = sun_term_buf;
    /*      sun_term_buf.c_iflag &= ~(INLCR | ICRNL | IUCLC | ISTRIP);  */
    sun_term_buf.c_iflag &= ~(IUCLC | ISTRIP);
    sun_term_buf.c_iflag |= IXON;
    /*      sun_term_buf.c_oflag |=  ONLCR;   */
    sun_term_buf.c_lflag &= ~(ICANON | ECHOCTL);
    sun_term_buf.c_cflag &= ~PARENB;
    sun_term_buf.c_cflag |= CS8;
    sun_term_buf.c_cflag &= ~B9600;
    sun_term_buf.c_cflag |= B19200;
    sun_term_buf.c_cc[VMIN] = 1;
    sun_term_buf.c_cc[VTIME] = 0;

    qs_inq_c (QS_WORKSTATION_MAC_DEVICE, mac_device, (INT *)0);
    if (strlen (mac_device))
        {
        freopen (mac_device, "r", stdin);
        freopen (mac_device, "w", stdout);
        }

    if (ioctl (0, TCSETA, &sun_term_buf) == -1)
        {
        perror ("wi_init: ioctl to set term parameters failed");
        }
    setbuf (&_iob[1], NULL);    /* set no buffering */
#endif

    /* Send initialization command to MAC */
    SEND_1OPCODE (MC_INIT);

    /* Request version number. */
    if ((jp_mode != WI_JOURNAL) && (jp_mode != WI_PLAYBACK))
        {
        if (ttype != TTYPE_VT100)
            {
            SEND_1_VALUE (MC_QUERY_VERSION);
            wiz_set_echo (FALSE);
            if (WAIT_RESPONSE (RQVR_CODE))
                {
                /* read response, force 1st decimal place to zero to cancel out test versions */
                wiz_gets (stresp);
                chars = strlen (stresp);
                for (charat = 0; charat < chars; charat++)
                    {
                    if ((stresp[charat] == '4') || (stresp[charat] == '5'))
                        {
                        chars = charat;
                        }

                    }
                if (charat != strlen (stresp))
                    {
                    stresp[charat] = '0';
                    }
                num = &version;
                sscanf (stresp, "%d", num);

                /* wizgnum(&version); */

                /*   If versions are not compatable, tell
                     user. */

                wiz_acknowledge ();

                if (version < MAC_MINIMUM_VERSION)
                    {
                    wi_beep (1);
                    wi_message (mg (WI_NOT_COMPATABLE_1));
                    wi_message (mg (WI_NOT_COMPATABLE_2));
                    sprintf (wiv.stl, mg (WI_RECOMMEND_VERSION),
                             MAC_MINIMUM_VERSION - 4000);
                    wi_message (mg (WI_TELL_SYS_ADMIN));
                    SEND_LINEOUT (wiv.stl);
                    }
                }
            wiz_set_echo (TRUE);
            }
        else
            {
            /*   Tell user that MAC wont answer version
                 request, and to try using ESITERM. */
            wi_message (mg (WI_CANT_GET_VERSION));
            wi_message (mg (WI_USE_ESITERM));
            }
        }
    /* Open module for stock interactions. */
    rsmdopen (1);

    /* set the desired journal/playback mode, if any */

    if ((jp_mode == WI_JOURNAL) || (jp_mode == WI_PLAYBACK))
        {
        wiv.jpmode = jp_mode;
        strcpy (wiv.jfilename, jp_name);

        /* jp, Initialize journal/playback system based on above... */

        if ((stat = wiz_jp_initialize ()) != SUCCESS)
            {
            RETURN (stat);
            }
        }
    else
        {
        wiv.jpmode = 0;
        }

    wiz_jp_name (IDENT);

    /* if in PLAYBACK, tell the Mac to shut up. */
    if (wiv.jpmode == WI_PLAYBACK)
        {
        SEND_2_VALUES (1, MC_SHUTUP);
        }
#endif

    RETURN (SUCCESS);
    }

/******************************************************************************/


publicdef WIV_STRUCT *wiz_static ()
    {
    return &wiv;
    }



