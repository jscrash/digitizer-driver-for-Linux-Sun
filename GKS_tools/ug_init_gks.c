/* DEC/CMS REPLACEMENT HISTORY, Element UG_INIT_GKS.C*/
/* *25   19-AUG-1991 18:14:45 KEE "(SPR -1) Put in the correct GKS open error log file error message"*/
/* *24   14-AUG-1991 12:01:09 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *23    6-AUG-1991 17:05:41 KEE "(SPR 8887) Fixing gks error log file locking problem, now use unique file name base on the*/
/*analyst name and the current time (case 3621)"*/
/* *22    3-MAY-1991 16:12:55 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *21   31-OCT-1990 09:31:29 GILLESPIE "(SPR 37) Sun compile fixes"*/
/* *20   23-OCT-1990 13:52:28 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *19   26-AUG-1990 14:36:37 GILLESPIE "(SPR 1) Change HOST != ULTRIXR to #ifndef mips"*/
/* *18   23-AUG-1990 16:15:57 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *17   10-JUL-1990 19:18:45 MING "(SPR -1) TURN OFF WISS IF NO_WCPLOT IS USED"*/
/* *16    3-MAY-1990 21:48:09 KEE "(SPR -1) multi window support."*/
/* *15    8-MAR-1990 10:39:53 KEE "(SPR 6016) Fixing gks_widget_ref local copy bug "*/
/* *14    5-MAR-1990 04:03:13 JULIAN "(SPR 6010) Define configuration Workspace for Ultrix."*/
/* *13    4-MAR-1990 12:33:31 JULIAN "(SPR 6010) Ultrix mods."*/
/* *12    2-MAR-1990 11:11:53 JULIAN "(SPR -1) Add X initialization (Kee)"*/
/* *11    2-MAR-1990 10:34:35 GILLESPIE "(SPR 6010) Fix up USE_X logic to be IFDEF only"*/
/* *10   28-FEB-1990 15:46:12 JULIAN "(SPR -1) Add element to WM_CURRENT struct, change MAIN_WKID & GKS_WISS_WKID def (Kee)"*/
/* *9     6-FEB-1990 00:18:10 JULIAN "(SPR -1) X Version "*/
/* *8     2-NOV-1989 15:22:26 JULIAN "(SPR 30) move ug_term_gks, make ugv static"*/
/* *7     1-NOV-1989 12:54:53 GILLESPIE "(SPR 1) Remove misc. toolbox init calls"*/
/* *6    10-OCT-1989 12:36:57 GORDON "(SPR 4999) Only send errors to file for programmers, create file name in line."*/
/* *5    26-SEP-1989 14:31:07 GORDON "(SPR 201) test for graphics_planes on VMS"*/
/* *4    14-AUG-1989 21:07:25 JULIAN "(SPR -1) added color indexes for colors 2-15"*/
/* *3    10-AUG-1989 13:39:36 GORDON "(SPR 105) GULF's ugoperrc changes"*/
/* *2    20-JUL-1989 11:03:12 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:35:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element UG_INIT_GKS.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright FINDER Graphics Systems, Inc. 1989                      */
/*                     Unpublished -- All rights reserved                           */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*   FINDER Graphics Systems, Inc.                                                  */
/*   201 Tamal Vista                                                                */
/*   Corte Madera, CA  USA 94925                                                    */
/*   (415) 927-0100                                                                 */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/*                                                                                  */
/* *  Name :          init_gks.c                                                    */
/* *  Description :   initializes gks and Raster/Onelib environments                */
/* *                                                                                */
/*                                                                                  */
/* START-CODE :                                                                     */
#include "esi_c_lib.h"
#include "esi_ug.h"        /* utility graphics header file      */
#include "esi_ug_err.h"    /* contains defines from ug message  */
#include "esi_fi_err.h"    /* contains defines from fi message  */
#include "esi_ugv.h"       /* structure for RT terminal settings */

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_WM_INT_H

#include "esi_wm_int.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_ERR_H

#include "esi_ho_err.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TG_H

#include "esi_tg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_CONF_WS_H

#include "esi_conf_ws.h"

#endif

#ifdef vms
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ==========*/
#ifndef ESI_GLUE_H
#include "esi_glue.h"
#endif
#include "ssdef.h"
#include "iodef.h"
#include "ttdef.h"
#include "tt2def.h"
#include "dvidef.h"
#include "esi_ho_lib.h"
int LIB$GETDVI ();
#endif  /* ========== END DEC GKS ==========*/
#endif

/* -------------------------------------------                                      */
/* Global static variables used in this module                                      */
/* -------------------------------------------                                      */

static UGV ugv;

#ifdef SUN_GKS_OR_PRIOR_GKS
    FILE *errfil = (FILE *)0;
#else
    INT errfil = 0;
#endif

/* -----------                                                                      */
/* X-dependent                                                                      */
/* -----------                                                                      */

#ifdef USE_X

#include "ansixt.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#define DECW_CMAP_ENTRY 256

#ifdef mips

#define GKS_CMAP_ENTRY  64

#else

#define GKS_CMAP_ENTRY  16

#endif
#endif

/* =====  UG_INIT_GKS =====                                                         */
#if USE_PROTOTYPES
publicdef INT ug_init_gks (WKS_ID *wksid)
#else
publicdef INT ug_init_gks (wksid)
WKS_ID *wksid;
#endif
    {
    WM_CURRENT *wmv_current = wm_static ();
    CONFIGURATION_WORKSPACE *c;
    
    INT status = SUCCESS;
#ifdef SUN_GKS_OR_PRIOR_GKS
    extern FILE *ugoperrc ();
    FILENAME uni_file_name;
#endif
    APPLICATION_ID wm_application = 1;  /* initial window manager application id    */
    GKS_REAL tracking_tabxf[6];
    GKS_INT devtype;                /* physical workstation type                    */
    GKS_INT wiss_conid = 0;
    GKS_INT ibuf = 0;
    GKS_INT app_id;
    GKS_INT tnr;
    BOOL isGpxType;                 /* The workstation is GPX.                      */
    INT channel_type, efn;          /* dma channel type, and event flag             */
    BOOL programmer = FALSE;
    INT tool_x, tool_y, tool_width, tool_height, msg_win_size, textsw;
    INT i;
    CHAR label[8];
    PATHNAME errfilname;
    ANALYST_NAME username;
#ifdef USE_X /* MGKS */
    GKS_INT error_status, max_op_ws, max_ac_ws, max_ws_seg;
#endif

#ifdef vms
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ==========*/
    INT option;
    INT mode[3];
    INT mask = 0xFFFFFFFF;
    INT mode_length = sizeof mode;
    unsigned short iosb[4];
    INT if_dma = 0;
    CHAR dvi_name[64];
    int func = DVI$_DEVNAM;
    unsigned short dvi_channel, dvi_len;
    struct dsc$descriptor_s dvi_name_dsc;
#endif  /* ========== END DEC GKS ==========*/
#endif
    
    /* Get the workstation type.                                                    */
    
    isGpxType = ug_if_gpx ();
    
    if (wksid == (WKS_ID *)NULL)
        {
        status = UG_NO_WK_STRUCT;
        return status;
        }
    qs_inq_i (QS_USER_SKILL, &programmer);  /* We'll need this later.               */
    
    /********************************************************************************/

#ifndef SUN_GKS_OR_PRIOR_GKS
    if (!isGpxType)
        {
#ifdef vms
        /* get current terminal characteristics                                     */
        /*then set those needed for ESITERM.                                        */
        /*These will be reset on image termination                                  */
        /* sense current                                                            */
        /* get device name. if dma, do no terminal settings                         */
        dvi_channel = wksid->channel;
        dvi_name_dsc.dsc$a_pointer = dvi_name;
        dvi_name_dsc.dsc$w_length = sizeof dvi_name;
        dvi_name_dsc.dsc$b_class = DSC$K_CLASS_S;
        dvi_name_dsc.dsc$b_dtype = DSC$K_DTYPE_T;
        status = LIB$GETDVI (&func, &dvi_channel, 0, 0, &dvi_name_dsc, &dvi_len);
        switch (status)
            {
        case SS$_NORMAL:
            status = SUCCESS;
            break;
        default:
            status = UG_VMS_ERROR;
            break;
            }
        if (status == SUCCESS)
            {
            /*     Note:  the device name begins with the "_" character.            */
            if ((dvi_name[1] == 'G') || (dvi_name[1] == 'g') || (dvi_name[1] == 'X') ||
                (dvi_name[1] == 'x'))
                {
                if_dma = 1;
                }
            }
        if (if_dma != 1)
            {
            /*  set terminal characteristics                                        */
            option = IO$_SENSEMODE;
            status = VAX_ROUTINE (sys$qiow (0, wksid->channel, option, iosb, 0, 0,
                                            mode, mode_length, 0, 0, 0, 0));
            if ((status == SS$_NORMAL) AND (iosb[0] == SS$_NORMAL))
                {
                status = SUCCESS;
                }
            else
                {
                status = HO_ERR_VMS;
                return status;
                }
            for (i = 0; i < 3; i++)
                {
                ugv.raster_terminal_setting[i] = mode[i];
                }
            mode[1] &= mask ^ TT$M_WRAP;    /* turn off wrap                        */
            mode[1] &= mask ^ TT$M_MECHTAB; /* turn off machine tabs                */
            mode[1] &= mask ^ TT$M_READSYNC;    /* turn off ctrlS/ctrlQ             */
            
            mode[1] |= TT$M_NOBRDCST;   /* turn off broadcast                       */
            mode[2] |= TT2$M_PASTHRU;   /* turn on pasthru                          */
            mode[2] |= TT2$M_DECCRT;    /* enable DECCRT (VT100)                    */
            
            option = IO$_SETMODE;
            status = VAX_ROUTINE (sys$qiow (0, wksid->channel, option, iosb, 0, 0,
                                            mode, mode_length, 0, 0, 0, 0));
            if ((status == SS$_NORMAL) AND (iosb[0] == SS$_NORMAL))
                status = SUCCESS;
            else
                {
                status = HO_ERR_VMS;
                return status;
                }
            }
#endif
        
        /*                                                                          */
        /* *  Get the channel_type and event flag.                                  */
        /* *  Initialize esi's gks modifications.                                   */
        /* *  Make an error file available and open it with a buffer length of 0    */
        /*                                                                          */
        
        status = ho_get_channel_type (wksid->channel, &channel_type, &efn);
        giemod_gc (wksid->channel, wksid->type, channel_type, efn);

#ifdef vms
        if (if_dma == 1)
            {
            /*  DMA case.                                                           */
            gkz_init_dr11_gc (wksid->channel);
            }
#endif

#ifdef primos
        if (channel_type > 0)       /* 0 = serial, 1 = dma                          */
            {
            /*  DMA case.                                                           */
            gkz_init_dr11_gc (wksid->channel);
            }
#endif
        }
    /* End of none-gpx block                                                        */
    /* We open the gks error log file only if it's                                  */
    /* a programmer. Otherwise, errfil is still 0                                   */
    /* and GKS should send errors to SYS$ERROR                                      */
    if (programmer)
        {
        qs_inq_c (QS_FINDER_ACCOUNT, username, &i);
        sprintf (errfilname, "GKS_%s.ERR", i ? username : "USER");
        ho_add_path ("ESI$ERRORS", errfilname, sizeof(FILENAME), errfilname);
        ugoperr_gc (&errfil, errfilname);   /* Open it                              */
#ifdef mips
        errfil = 0;
#endif
        }
#endif     /* End of non-SUN or non-PRIOR block */

#ifdef SUN_GKS_OR_PRIOR_GKS
    
    /* Open the error log file.                                                     */
    qs_inq_c (QS_FINDER_ACCOUNT, username, &i);
    ho_get_unique_filename (uni_file_name);
    sprintf (errfilname, "GKS_%s_%s.ERR", i ? username : "USER", uni_file_name);
    ho_add_path ("ESI$ERRORS", errfilname, sizeof(FILENAME), errfilname);
    errfil = ho_open (errfilname, "w", NULL);
    status = ho_lock (errfil, ON);
    if (errfil == (FILE *)NULL OR status == -1)
      {
	status = FI_GKS_ERRLOG_OPEN_FAIL;
      }
    printf ("GKS errors will be logged to %s\n", errfilname);
#ifdef USE_X
    errfil = (FILE *)0;
#else
    ibuf = -1;
    tool_x = 0;
    tool_y = 0;
    textsw = 0;
    strcpy (label, "FINDER");
    msg_win_size = 1;
    if (programmer)
        {
        tool_width = 400;
        tool_height = 400;
        }
    else
        {
        tool_width = 800;
        tool_height = 800;
        }
    wm_cfg_ws (tool_x, tool_y, tool_width, tool_height, textsw, label, msg_win_size);
#endif  /* USE_X */
#endif  /* SUN_GKS_OR_PRIOR_GKS */
    wm_gopks (errfil, ibuf);
    /* -----------                                                                  */
    /* X-dependent                                                                  */
    /* -----------                                                                  */

#ifdef USE_X
    wm_gqwkm (&error_status, &max_op_ws, &max_ac_ws, &max_ws_seg);
    if (error_status == SUCCESS)
        {
        wmv_current->ws_id = 0;
        max_ac_ws -= 3;             /* for WISS, metafile input and metafile output */
        wmv_current->max_ac_ws = max_ac_ws;
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
        wmv_current->max_ws = max_ac_ws;
#else  /* ========== END PRIOR GKS = BEGIN DEC GKS ========== */
        wmv_current->max_ws = ((DECW_CMAP_ENTRY / GKS_CMAP_ENTRY - 1 + 2) <
                               max_ac_ws) ?
                              (DECW_CMAP_ENTRY / GKS_CMAP_ENTRY - 1) : max_ac_ws;
        /* - 1 : last 16 colormap entries being reserved, can't be used             */
        /*   + 2 : one for WISS, one for metafile id                                */
#endif  /* ========== END DEC GKS ========== */
        wmv_current->gks_widget_array =
            (GKS_WIDGET_STRUCT *) tc_zalloc (sizeof(GKS_WIDGET_STRUCT) *
                                             (wmv_current->max_ws));
        }
    else
        {
        return(FAIL);
        }
#else /* End of X-dependent code */
    wmv_current->ws_id = 1;
    wmv_current->max_ws = 2;
    /*                                                                              */
    /* *  Open workstation with channel id and wkstation id and                     */
    /* *  wkstation type from external structure, wksid.                            */
    /* *  Then activate wkstation.                                                  */
    /* *  Then select normalized transformation                                     */
    /*                                                                              */
    
    if (isGpxType)
        {
        devtype = GWSDEF;
        }
    else
        {
        devtype = 3;
        }
    wksid->channel = GCONID;
    wm_gopwk (MAIN_WKID, wksid->channel, devtype);
    wm_gacwk (MAIN_WKID);
    
    /*                                                                              */
    /* * set window and viewport.                                                   */
    /*                                                                              */
    
    tnr = 1;
    wm_gswn (tnr, 0.0, 1000.0, 0.0, 1000.0);
    wm_gsvp (tnr, 0.0, 1.0, 0.0, 1.0);
    wm_gselnt (tnr);
    
    /* init color for VMS and SUN mac version                                       */
    wmz_init_color ();

#endif
    
    /*                                                                              */
    /* * Open WISS for window manager use.                                          */
    /*                                                                              */
    

#ifdef SUN_GKS_OR_PRIOR_GKS
    am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)&c);
    if (c->wiss == TRUE)
        {
        wm_gopwk (GKS_WISS_WKID, GCONID, 1);
        wm_gacwk (GKS_WISS_WKID);
        }
#ifndef USE_X
    wm_hold_updates ();             /* X doesn't create any MAIN_WKID at this point */
#endif

#else
    if (!isGpxType)
        {
        hog_gfnt (&wiss_conid);
        am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)&c);
        if (c->wiss == TRUE)
            {
            wm_gopwk (GKS_WISS_WKID, wiss_conid, GKS_WISS_WTYPE);
            wm_gacwk (GKS_WISS_WKID);
            }
        }
    else
        {
/*	#ifndef mips					 910319:sjs: use same as sun for mips */
        am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)&c);
        if (c->wiss == TRUE)
            {
            wm_gopwk (GKS_WISS_WKID, GCONID, GWSWIS);
            wm_gacwk (GKS_WISS_WKID);
            }
/*	#endif	 end of ifndef mips */
        }
#endif
    
    /*                                                                              */
    /* *  Open GKS related toolboxes.                                               */
    /*                                                                              */
    wminit ();
    wmaopen (wm_application);
    
    /*                                                                              */
    /* *  Set tablet attributes ( this used to be only for 180 models               */
    /* *   terminal type if it is a 180 workstation )                               */
    /*                                                                              */

#ifndef SUN_GKS_OR_PRIOR_GKS
    if (!isGpxType)
        {
        tg_set_tablet_type (wksid->tablet_type, tracking_tabxf);
        wmtregdefault (tracking_tabxf);
        wmtregmode (WM_REG_MODE_REL);
        }
#endif
    return status;
    }

/*************************************************************************/
/*                                                                                  */
/* *  Name : ug_term_gks.c                                                          */
/* *  Author : John Barton                                                          */
/* *  Description :                                                                 */
/* *       Terminate a GKS session by recieving structure containing :              */
/* *         a workstation channel #                                                */
/*                                                                                  */

/************************************************************************************/
#if USE_PROTOTYPES
publicdef INT ug_term_gks (INT channel)
#else
publicdef INT ug_term_gks (channel)
INT channel;
#endif
    {
    INT status = SUCCESS;
    INT mode[3];
    INT mode_length = sizeof mode;
    unsigned short iosb[4];
    INT i;
    CONFIGURATION_WORKSPACE *c;
#ifdef USE_X
    WM_CURRENT *wmv_current = wm_static ();
    INT j;
#endif

#ifdef vms
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ==========*/
    /* reset the Graphics Terminal line to its                                      */
    /* original settings (stored in UG_INIT_GKS)                                    */
    for (i = 0; i < 3; i++)
        {
        mode[i] = ugv.raster_terminal_setting[i];
        }
    status = VAX_ROUTINE (sys$qiow (0, channel, IO$_SETMODE, iosb, 0, 0, mode,
                                    mode_length, 0, 0, 0, 0));
#endif  /* ========== END DEC GKS ==========*/
#endif
    wmterm ();                      /* window manager initialize                    */

#ifdef USE_X
    for (i = 1, j = wmz_ngks_wk_created (); i <= j; i++)
        {
        if (wmv_current->gks_widget_array[i].widget != (Widget)NULL)
            {
            XtDestroyWidget (wmv_current->gks_widget_array[i].widget);
#ifndef PRIOR_GKS  /* ========== BEGIN DEC GKS ========== */
            XtDestroyWidget (wmv_current->gks_widget_array[i].child_widget);
            XtDestroyWidget (wmv_current->gks_widget_array[i].out_widget);
#endif  /* ========== END DEC GKS ========== */
            if (i == MAIN_WKID)
                wm_gdawk (i);
            wm_gclwk (i);
            }
        }
    tc_free (wmv_current->gks_widget_array);
#endif
    
#ifdef SUN_GKS_OR_PRIOR_GKS
    status = ho_lock (errfil, OFF);
    fclose (errfil);
#endif
    /* terminate WISS                                                               */
    am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)&c);
    if (c->wiss == TRUE)
        {
        wm_gdawk (GKS_WISS_WKID);   /* deactivate wiss                              */
        wm_gclwk (GKS_WISS_WKID);   /* close wiss                                   */
        }
    /* terminate applications                                                       */
#ifndef USE_X
    wm_gdawk (MAIN_WKID);           /* deactivate workstation                       */
    wm_gclwk (MAIN_WKID);           /* close workstation                            */
#endif
    wm_gclks ();                    /* close GKS                                    */
    
    return status;
    }
