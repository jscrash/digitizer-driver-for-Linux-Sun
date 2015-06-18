/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT.C*/
/*  19   14-FEB-1992 09:42:57 LOEBL "(SPR 6299) SQL*FORMS30 conversion requirement"*/
/* *18    8-JUL-1991 13:45:03 SANTIAGO "(SPR -1) moved call to wiinit to before xf_initialize"*/
/* *17    6-JUL-1991 14:11:22 SANTIAGO "(SPR -1) moved wiinit from xf_initialize to fi_init"*/
/* *16    5-JUL-1991 19:31:50 SANTIAGO "(SPR -1) x_initialize is now called xf_initialize"*/
/* *15   22-MAY-1991 17:37:27 SANTIAGO "(SPR 8087) verified noonunits is fixed, minor cosmetic changes"*/
/* *14   22-MAY-1991 11:36:31 SANTIAGO "(SPR 7598) argc is now passed by reference to xf_initialize"*/
/* *13   27-NOV-1990 14:31:06 JESSIE "(SPR 6087) add -c option on finder command line"*/
/* *12   23-OCT-1990 13:51:40 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *11   24-SEP-1990 16:19:18 GILLESPIE "Beta Deltas"*/
/* *10   30-JUL-1990 17:21:38 JULIAN "(SPR 0) add lynx,lease select_list"*/
/* *9    10-JUL-1990 19:11:36 MING "(SPR -1) ADD NO_WCPLOT OPTION TO TURN OFF WISS"*/
/* *8     2-MAR-1990 11:13:22 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *7     6-FEB-1990 00:17:36 JULIAN "(SPR -1) X Version "*/
/* *6    18-DEC-1989 16:40:34 JULIAN "(SPR 5069) export facility mods"*/
/* *5    10-AUG-1989 14:52:25 GORDON "(SPR 105) change -m processing to call ts_tcp_append"*/
/* *4    28-JUL-1989 15:35:51 GORDON "(SPR 101) Add -menu, -x options, changed -m to -mac"*/
/* *3    26-JUN-1989 10:44:03 GILLESPIE "(SPR 20) Fix castings"*/
/* *2    20-JUN-1989 11:09:37 GILLESPIE "(SPR 20) Fix minor casting problem"*/
/* *1    19-JUN-1989 12:50:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT.C                                   */
/* *2    16-AUG-1990 11:39:20 EXTERMINATOR "(SPR 0) select list modification"       */
/* *1    13-AUG-1990 16:38:55 SYSTEM "Finder 6.1"                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT.C                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT.C                                   */
/* *10   30-JUL-1990 17:21:38 JULIAN "(SPR 0) add lynx,lease select_list"            */
/* *9    10-JUL-1990 19:11:36 MING "(SPR -1) ADD NO_WCPLOT OPTION TO TURN OFF WISS" */
/* *8     2-MAR-1990 11:13:22 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *7     6-FEB-1990 00:17:36 JULIAN "(SPR -1) X Version "                          */
/* *6    18-DEC-1989 16:40:34 JULIAN "(SPR 5069) export facility mods"               */
/* *5    10-AUG-1989 14:52:25 GORDON "(SPR 105) change -m processing to call ts_tcp_append" */
/* *4    28-JUL-1989 15:35:51 GORDON "(SPR 101) Add -menu, -x options, changed -m to -mac"  */
/* *3    26-JUN-1989 10:44:03 GILLESPIE "(SPR 20) Fix castings"                     */
/* *2    20-JUN-1989 11:09:37 GILLESPIE "(SPR 20) Fix minor casting problem"        */
/* *1    19-JUN-1989 12:50:02 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element FI_INIT.C                                   */

/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    FINDER_INITIALIZE                                                     */
/*                                                                                  */
/*   AUTHOR:  W. A. Jaeck, 23-Apr-86                                                */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    FINDER_INITIALIZE initializes the Finder system.                              */
/*                                                                                  */
/*    It ought to be called once, and only once.                                    */
/*                                                                                  */
/*************************************************************************          */

#include "esi_c_lib.h"
#include "esi_fi.h"

#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#ifndef ESI_FI_MSG_H
#include "esi_fi_msg.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_ORACLE_X
#include "esi_oracle.x"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_RS_H
#include "esi_rs.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_WI_INT_H
#include "esi_wi_int.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#define USE_DEFAULT  FALSE
static FILE *fp_error;

/* **********************************************************************           */
publicdef INT finder_initialize( argc, argv )
/* **********************************************************************           */
int argc ;
char *argv[] ;
    {
    INT status, temp_status;        /* Completion status                            */
    BOOL done;
    BOOL first_parameter = TRUE;
    CHAR plotter_name[128];
    CHAR **tcp;
    CHAR text[100];
    CONFIGURATION_WORKSPACE *c;
    
    /* **********************************************************************       */
    
    status = SUCCESS;
    plotter_name[0] = NUL;

    am_initialize() ;

    rsinit() ;

#ifdef USE_X
    /* must call wiinit before xf_initialize, otherwise menus get screwed up */

    status = wiinit (1,             /* workstation id ... always 1                  */
            0,                      /* graphics terminal type ... not used.         */
            2,                      /* Terminal type: 0=vt100, 1=MAC, 2=Xwindows    */
            0, "");                 /* no journal record or playback. or file       */
    
    if (status != SUCCESS)
        printf ("Warning: wiinit() returned %d    ..... continuing", status);

    xf_initialize( &argc, argv ) ;      /* initialize the X-windows system              */

#endif

    am_define_workspace(
        "CONFIGURATION",
        AM_GLOBAL,
        sizeof(CONFIGURATION_WORKSPACE),
        (VOIDPTR *)&c ) ;
    
    c->on_units = TRUE;
    c->wiss = TRUE;
    c->checkwi = FALSE;
    
    if (NOT oracle_login)
      {
        status = or_login( argc, argv ) ; /* log onto ORACLE                          */
      }

    while ((status == SUCCESS) AND (--argc) > 0)
      {
        ts_sto_lower( *++argv ) ;
        done = FALSE ;
        
        /****------------------------***                                            */
        /****   TEST MODE SELECTION  ***                                            */
        /****------------------------***                                            */

        if ( (strcmp("-test", *argv) == 0) OR (strcmp("-t", *argv) == 0) )
          {
            c->test = TRUE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** USER MENU ADD-ON       ***                                            */
        /****------------------------***                                            */

        if ( (!strcmp("-m", *argv)) OR (!strcmp("-menu", *argv)) )
          {
            if (argc - 1 > 0)
              {
                if ( *(argv + 1)[0] == '-')  /* check file name                     */
                  {
                    am_message( AM_STATUS, mg_message( FI_NO_MENU_FILE ) ) ;
                  }
                else
                  {
                    --argc ;
                    ts_tcp_append( &c->user_menu_tcp, *++argv ) ;
                  }
              }
            else
              {
                am_message( AM_STATUS, mg_message(FI_NO_MENU_FILE) ) ;
	      }
            done = TRUE ;
          }

        /****------------------------***                                            */
        /****   X-WINDOWS SWITCH     ***                                            */
        /****------------------------***                                            */

        if ( (strcmp("-x", *argv) == 0) )
          {
            c->use_x = TRUE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /****   CHECK WI SWITCH      ***                                            */
        /****------------------------***                                            */

        if ( (strcmp("-checkwi", *argv) == 0) )
          {
            c->checkwi = TRUE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /****   SELECT JOURNAL MODE  ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-journal", *argv) == 0) OR (strcmp("-j", *argv) == 0) ) )
          {
            if (--argc)
              {
                strcpy(c->journal_file, *++argv) ;
              }
            else
              {
                am_message( AM_STATUS, mg(FI_NO_JOURNAL_FILE) ) ;
                status = FAIL;
              }
            done = TRUE;
          }

        /****------------------------***                                            */
        /****   RECORD JOURNAL FILE  ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-record", *argv) == 0) OR (strcmp("-r", *argv) == 0) ) )
          {
            if (c->record_play_mode != 0)
              {
                am_message( AM_STATUS, mg(FI_CONFLICTING_OPTIONS) ) ;
                status = FAIL;
              }
            if (--argc)
              {
                c->record_play_mode = WI_JOURNAL;
                strcpy(c->record_play_file, *++argv);
              }
            else
              {
                am_message( AM_STATUS, mg(FI_NO_RECORD_PLAY_FILE) );
                status = FAIL;
              }
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** PLAYBACK JOURNAL FILE  ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-play", *argv) == 0) OR (strcmp("-p", *argv) == 0) ) )
          {
            if (c->record_play_mode != 0)
              {
                am_message( AM_STATUS, mg(FI_CONFLICTING_OPTIONS) );
                status = FAIL;
              }
            if (--argc)
              {
                c->record_play_mode = WI_PLAYBACK;
                strcpy(c->record_play_file, *++argv);
              }
            else
              {
                am_message(AM_STATUS, mg(FI_NO_RECORD_PLAY_FILE) );
                status = FAIL;
              }
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** Graphics Device Selection ***                                         */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-workstation", *argv) == 0) OR (strcmp("-w", *argv) == 0) ) )
          {
            if (--argc)
              {
                strcpy(c->workstation_name, *++argv);
                ts_sto_upper(c->workstation_name);
              }
            else
              {
                am_message(AM_STATUS, mg (FI_NO_WORKSTATION_NAME));
                status = FAIL;
              }
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** USER PLOTTER SELECTION ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strncmp("-plot", *argv, 5) == 0) ) )
          {
            if (--argc)
              {
                strcpy(plotter_name, *++argv);
                ts_sto_upper(plotter_name);
              }
            else
              {
                am_message(AM_STATUS,
                            "Plotter not specified. Use -PLOT <name of plotter>.");
                status = FAIL;
              }
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** USE NO ON-UNIT ALARMS  ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND (strcmp("-no_onunits", *argv) == 0) )
          {
            c->on_units = FALSE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /****   NO GRAPHICS DEVICE   ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-mac", *argv) == 0) OR (strcmp("-maconly", *argv) == 0)) )
          {
            c->maconly = TRUE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** FINDER USER ACCOUNTING ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-ac", *argv) == 0) OR (strcmp("-accounting", *argv) == 0)) )
          {
            am_static()->accounting = TRUE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /**** PROJECT NAME SELECTION ***                                            */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-project", *argv) == 0) OR ( strcmp("-proj", *argv) == 0)) )
          {
            if (--argc)
              {
                ++argv;
                qs_set_c( QS_PROJECT_NAME, ts_sto_upper (*argv), strlen (*argv));
              }
            else
              {
                am_message( AM_STATUS, mg(FI_NO_PROJECT_NAME) );
                status = FAIL;
              }
            done = TRUE;
          }

        /****-------------------------------------***                               */
        /**** Turn on/off WISS for window control ***                               */
        /****-------------------------------------***                               */

        if ( (!done) AND (strcmp("-no_wcplot", *argv) == 0) )
          {
            c->wiss = FALSE;
            done = TRUE;
          }

        /****------------------------***                                            */
        /****    check crt name      ***                                           */
        /****------------------------***                                            */

        if ( (!done) AND ( (strcmp("-crt", *argv) == 0) OR (strcmp("-c", *argv) == 0)) )
          {
            if (--argc)
              {
                strcpy(c->crt_name, *++argv);
                ts_sto_upper(c->crt_name);
              }
            else
              {
                am_message(AM_STATUS, mg(FI_NO_CRT_NAME) );
                status = FAIL;
              }
            done = TRUE;
          }

        /****------------------------***                                            */
        
        if (NOT done AND NOT first_parameter)
          {
            sprintf(text, mg(FI_ILLEGAL_OPTION), *argv);
            am_message(AM_STATUS, text);
          }
        first_parameter = FALSE;
      }

    if (!status)
      {
        if (c->journal_file[0])
          {
            status = am_message(AM_OPEN_JOURNAL_FILE, c->journal_file);
            if (status)
              {
                am_message (AM_STATUS, mg (FI_CANT_OPEN_JOURNAL));
              }
          }
      }

    if (!status)
      {
        status = finder_config();
      }

    if (status EQUALS SUCCESS)
      {
        wismode(WI_MENU_EVENT, WI_EVENT_MODE);
        wismode(WI_DIALOG_EVENT, WI_EVENT_MODE);
        wismode(WI_SELECT_EVENT, WI_EVENT_MODE);
      }
    else
      {
        c->maconly = TRUE;          /* Since we're going to abort, and graphics     */
                                    /* haven't been initialized yet, this will      */
                                    /* prevent an access violation in trying to     */
                                    /* terminate graphics.                          */
        am_message(AM_STATUS, mg_message(status));
      }

    if (plotter_name[0] != NUL)
      {
        status = ho_set_logical_name (NULL, "ESI$SELECTED_PLOTTER", plotter_name, 0,
                                      FALSE);
      }
#if USE_DEFAULT == TRUE
    else
      {
        /* 12/12/88 GS use a returned TCP instead of char string                    */
        if ((ho_translate_symbol ("ESI$DEFAULT_PLOTTER", &tcp)) == SUCCESS)
          {
            strcpy(plotter_name, tcp[0]);  /* use first in list                    */
            status = ho_set_logical_name (NULL, "ESI$SELECTED_PLOTTER", plotter_name,
                                          0, FALSE);
            ts_tcp_free(tcp);      /* must free space                              */
          }
      }
#endif
    return status;
}
