/* DEC/CMS REPLACEMENT HISTORY, Element UG_PLOT_GKS.C*/
/* *27   17-OCT-1991 15:06:25 MING "(SPR 0) if meta_type == PS, do not call ugz_init_color"*/
/* *26   16-AUG-1991 16:39:58 MING "(SPR 0) return status if pf_open_pf fail"*/
/* *25   14-AUG-1991 12:01:20 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *24   25-JUL-1991 16:02:17 MING "(SPR 0) set color table "*/
/* *23    6-JUN-1991 17:17:54 MING "(SPR 0) new plot file managment"*/
/* *22    3-MAY-1991 16:13:06 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *21   22-MAR-1991 11:34:59 MING "(SPR -1) put ug_if_gpx for giemod_gc "*/
/* *20   14-JAN-1991 16:41:32 MING "(SPR -1) add wm_gscr on color index 0 for postscript ws on SUN code "*/
/* *19   31-OCT-1990 09:31:39 GILLESPIE "(SPR 37) Sun compile fixes"*/
/* *18   23-OCT-1990 13:52:38 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *17   23-AUG-1990 17:25:06 JESSIE "(SPR 1) fix include files"*/
/* *16   31-JUL-1990 21:53:59 GILLESPIE "(SPR 1) Add esi_c_lib.h; pretty print"*/
/* *15   10-JUL-1990 19:13:24 MING "(SPR -1) TURN OFF WISS IF NO_WCPLOTS IS USED"*/
/* *14    6-JUN-1990 15:46:44 MING "(SPR 0) fix bug for SUN version"*/
/* *13    1-JUN-1990 14:33:03 MING "(SPR 0) replace postscript ws from integer to symbol"*/
/* *12    1-JUN-1990 12:09:30 MING "(SPR 0) produce GKSM and Postscript also"*/
/* *11   31-MAY-1990 15:25:43 MING "(SPR 0) produce all kind of metafile"*/
/* *10   24-MAY-1990 19:21:33 MING "(SPR 0) PUT SUN INCLUDE FILE AT THE END"*/
/* *9    24-MAY-1990 19:03:01 MING "(SPR 0) ADD SUN CODE"*/
/* *8     8-MAY-1990 21:48:21 VINCE "(SPR 1) add setting of WM control structure"*/
/* *7     3-MAY-1990 09:35:44 MING "(SPR 0) change clear text encoding to char encoding"*/
/* *6    27-APR-1990 14:57:23 MING "(SPR 0) add is_neutral flag and metafile name pararmeter"*/
/* *5    26-FEB-1990 19:01:23 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *4     1-NOV-1989 13:18:50 GILLESPIE "(SPR 30) Remove bogus init calls for ci, tg, ps"*/
/* *3    12-OCT-1989 10:48:19 GORDON "(SPR 4999) New call interface to ugoperr_gc."*/
/* *2    19-SEP-1989 11:11:41 GORDON "(SPR 100) make tracking_tabxf GKS_REAL's, not INT's"*/
/* *1    19-JUN-1989 13:35:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element UG_PLOT_GKS.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Exploration Systems, Inc.                                             */
/*            579 Market Street                                                     */
/*            San Francisco, CA  USA 94105                                          */
/*            (415) 974-1600                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/*                                                                                  */
/* *  File        : UG_PLOT_GKS.C                                                   */
/* *                                                                                */
/* *  Routines    : ug_init_plot_gks.c                                              */
/* *                ug_term_plot_gks.c                                              */
/* *                                                                                */
/*                                                                                  */

/* include files                                                                    */
#include "esi_c_lib.h"

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_WM_INT_H

#include "esi_wm_int.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_UG_H

#include "esi_ug.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifdef PRIOR_GKS 
#include "sun_ansicgks.h"
#endif 

static INT ws = 1;
static INT wiss_ws = 2;

#ifdef SUN_GKS_OR_PRIOR_GKS
    FILE *errfile = (FILE *)0;
#else
    INT errfile;
#endif

/************************************************************************************/
#if USE_PROTOTYPES
publicdef INT ug_init_plot_gks (CHAR *channel_name,CHAR *meta_type)
#else
publicdef INT ug_init_plot_gks (channel_name,meta_type)
CHAR *channel_name;
CHAR *meta_type;
#endif
    {
    INT status = SUCCESS;
    INT i;
    INT app_id;
    INT wm_application = 1;         /* initial window manager application id        */
    INT ibuf = 0;
    INT wiss_conid = 0;
    INT devtype;                    /* physical workstation type                    */
    GKS_REAL tracking_tabxf[6];
    INT channel = 0;                /* channel id                                   */
    INT type;                       /* type not used                                */
    INT channel_type, efn;          /* dma channel type, and event flag             */
    INT tablet_type;
    BOOL batch;
    PATHNAME errfilename;
    ANALYST_NAME username;
    INT metatype;
    PATHNAME metafile;
    CHAR *mtype;
    CHAR msg[80];
    WM_CURRENT *wmv_current = wm_static ();

#ifdef SUN_GKS_OR_PRIOR_GKS
    extern FILE *ugoperrc ();
#endif

#ifdef SUN_GKS_OR_PRIOR_GKS
#ifndef PRIOR_GKS /* ========== BEGIN SUN GKS ========== */
    Gks_ws_config config;
#endif /* ========== END SUN GKS ========== */
#endif
    
    /*  set the workstation id in the window manager control structure.             */
    
    wmv_current->ws_id = 1;
    wmv_current->max_ws = 2;
    
    /*                                                                              */
    /* *  Get the channel_type and event flag.                                      */
    /* *  Initialize esi's gks modifications.                                       */
    /* *  Make an error file available and open it with a buffer length of 0        */
    /*                                                                              */
    channel = type = channel_type = efn = 0;
#ifdef SUN_GKS_OR_PRIOR_GKS
    errfile = (FILE *)0;
#else
    errfile = 0;
#endif
    
    if (!ug_if_gpx())
       {
       giemod_gc (channel, type, channel_type, efn);
       }
	   
    qs_inq_c (QS_FINDER_ACCOUNT, username, &i);
    sprintf (errfilename, "PLOT_GKS_%s.ERR", i ? username : "USER");
    ho_add_path ("ESI$ERRORS", errfilename, sizeof(FILENAME), errfilename);
#ifdef SUN_GKS_OR_PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
    errfile = ho_open (errfilename, "w", NULL);
    status = ho_lock (errfile, ON);
#ifdef USE_X
    errfile = (FILE *)0;
#endif
#else  /* ========== END PRIOR GKS = BEGIN DEC GKS ========= */
    ugoperr_gc (&errfile, errfilename);
#endif  /* ========== END DEC GKS ========== */
    wm_gopks (errfile, ibuf);
    
    /*                                                                              */
    /* *  Open workstation with channel id and wkstation id and                     */
    /* *  wkstation type from external structure, wksid.                            */
    /* *  Then activate wkstation.                                                  */
    /* *  Then select normalized transformation                                     */
    /*                                                                              */
    
    if (ARE_SAME (meta_type,"NPF"))
        {
        devtype = 3;
        wm_gopwk (ws, channel, devtype);
        wm_gacwk (ws);
        
        /* open GKS related toolboxes                                               */
        
        wminit ();
        wmaopen (wm_application);
        
        /* set tablet attributes (this is used to be only for 180 models            */
        /* * terminal type if it is a 180 workstation)                              */
        /*                                                                          */
        
        tablet_type = 0;
        tg_set_tablet_type (tablet_type, tracking_tabxf);
        wmtregdefault (tracking_tabxf);
        wmtregmode (WM_REG_MODE_REL);
        }
    else
        {
        status = pf_open_plotfile (ws,channel_name,meta_type,"w");
	if (status != SUCCESS)
	    {
	    return status;
	    }

        wm_gacwk (ws);
        
        /*                                                                          */
        /* *  Open GKS related toolboxes.                                           */
        /*                                                                          */
        wminit ();
        wmaopen (wm_application);
        }

    /* when the output workstation is postscript, somehow PRIOR GKS */
    /* regenerate existing segements in separate page when the set  */
    /* color representation function, gscr is being called. Here is */
    /* a fix to keep image in one page                              */

    if (ts_like (meta_type,"PS%",'%') == FALSE)
        {
        ugz_init_color (ws);
        }

    return status;
    }

/************************************************************************************/
#if USE_PROTOTYPES
publicdef INT ug_term_plot_gks ()
#else
publicdef INT ug_term_plot_gks ()
#endif
    {
    INT status = SUCCESS;
    INT channel;
    
    wmterm ();                      /* window manager initialize                    */
    
    wm_gdawk (ws);
    wm_gclwk (ws);
    
    /* terminate applications                                                       */
    wm_gclks ();                    /* close GKS                                    */

#ifdef SUN_GKS_OR_PRIOR_GKS
    status = ho_lock (errfile, OFF);
    fclose (errfile);
#endif
    
    return status;
    }



/************************************************************************************/

#if USE_PROTOTYPES
static ugz_init_color (GKS_INT ws)
#else
static ugz_init_color (ws)
GKS_INT ws;
#endif
    {
    wm_gscr( ws,  0, (GKS_REAL)0.0, (GKS_REAL)0.0, (GKS_REAL)0.0 ) ; /* black         */
    wm_gscr( ws,  1, (GKS_REAL)1.0, (GKS_REAL)1.0, (GKS_REAL)1.0 ) ; /* white         */
    wm_gscr( ws,  2, (GKS_REAL)1.0, (GKS_REAL)0.0,  (GKS_REAL)0.0  ) ; /* red         */
    wm_gscr( ws,  3, (GKS_REAL)0.0, (GKS_REAL)1.0,  (GKS_REAL)0.0  ) ; /* green       */
    wm_gscr( ws,  4, (GKS_REAL)0.0,  (GKS_REAL)0.0, (GKS_REAL)1.0  ) ; /* blue        */
    wm_gscr( ws, 5, (GKS_REAL)0.0, (GKS_REAL)1.0, (GKS_REAL)1.0  ) ; /* cyan          */
    wm_gscr( ws, 6, (GKS_REAL)1.0,  (GKS_REAL)0.0,  (GKS_REAL)1.0  ) ; /* magenta     */
    wm_gscr( ws, 7, (GKS_REAL)1.0,  (GKS_REAL)1.0,  (GKS_REAL)0.0  ) ; /* yellow      */
    wm_gscr( ws, 8, (GKS_REAL)1.0,  (GKS_REAL)0.54, (GKS_REAL)0.0  ) ; /* orange      */
    wm_gscr( ws, 9, (GKS_REAL)0.75, (GKS_REAL)0.75, (GKS_REAL)0.68 ) ; /* light_grey  */
    wm_gscr( ws, 10, (GKS_REAL)0.75, (GKS_REAL)0.85, (GKS_REAL)0.85 ) ; /* light_blue */
    wm_gscr( ws, 11, (GKS_REAL)0.25, (GKS_REAL)0.45, (GKS_REAL)0.55 ) ; /* blue_grey  */
    wm_gscr( ws, 12, (GKS_REAL)1.0,  (GKS_REAL)0.0,  (GKS_REAL)0.35 ) ; /* red_magenta*/
    wm_gscr( ws, 13, (GKS_REAL)0.5,  (GKS_REAL)0.5,  (GKS_REAL)0.5  ) ; /* grey       */
    wm_gscr( ws, 14, (GKS_REAL)0.65, (GKS_REAL)0.16, (GKS_REAL)0.16 ) ; /* brown      */
    wm_gscr( ws, 15, (GKS_REAL)0.80, (GKS_REAL)0.20, (GKS_REAL)0.60 ) ; /* violet     */
    }





