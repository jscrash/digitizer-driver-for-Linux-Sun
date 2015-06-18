/* DEC/CMS REPLACEMENT HISTORY, Element XS_GR_PLOT.C*/
/* *9     6-JUN-1991 16:38:21 MING "(SPR 0) new plot file mechanism"*/
/* *8     7-DEC-1990 09:23:58 MING "(SPR -1) fix scaling in neutral plot file"*/
/* *7    15-OCT-1990 15:18:56 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  5B1  12-OCT-1990 18:07:53 GILLESPIE "Merge Ernie Deltas"*/
/* *6    17-AUG-1990 22:40:01 VINCE "(SPR 5644) Code Cleanup"*/
/* *5     9-JUL-1990 09:44:15 MING "(SPR -1) produce CGM"*/
/* *4    20-JUN-1990 16:01:28 JULIAN "(SPR 0) change size from plot size to drawing size"*/
/* *3     6-JUN-1990 20:11:27 MING "(SPR 0) change tmpnam to ho_get_unqiue_filename"*/
/* *2     6-OCT-1989 14:52:09 GORDON "(SPR 4999) Use ANSI func tmpnam() to get temp file names"*/
/* *1    19-JUN-1989 13:44:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_GR_PLOT.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"

#ifndef ESI_XS_GR_H
#include "esi_xs_gr.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_PF_H
#include "esi_pf.h"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_PF_H
#include "esi_pf.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#define DEBUG_PLOT  FALSE
XS_STRUCT *xs_obj_load ();

#if USE_PROTOTYPES
static VOID msg (CHAR *string); /* debug-active print routine                   */
#else
static VOID msg ();             /* debug-active print routine                   */
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    For a detailed description, go to the end of this file.                       */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_gr_plot_section(XS_STRUCT  *pxs);                            */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static VOID msg(CHAR *string);                                            */
/*                                                                                  */
/*    static INT zz_read_draw();                                                */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This program, called directly from the xs_main menu servers,                  */
/*    is responsible for any special processing req'd to spawn                      */
/*    a batch/plotting version of the cross section.  Its tasks                     */
/*    include:                                                                      */
/*                                                                                  */
/*    -  Preparing a DCL script to submit.                                          */
/*    -  Calling tools that dump an image of the XS_STRUCT.                         */
/*    -  Preparing a 'unique' file name for the dump file.                          */
/*                                                                                  */
/*    USAGE:  xs_gr_plot_section(pxs);                                              */
/*            XS_STRUCT  *pxs;                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_gr_plot_section(XS_STRUCT  *pxs);                            */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*    CANCEL                                                                        */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_gr_plot_section (XS_STRUCT *pxs)
#else
publicdef INT xs_gr_plot_section (pxs)
XS_STRUCT *pxs;
#endif
    {
    PATHNAME dump_file_name, plot_file_name;
    PATHNAME cmd_file_name;
    PROJECT_NAME project;
    ANALYST_NAME f_account;
    ORACLE_NAME f_password;
    ORACLE_NAME f_database;
    CHAR login_phrase[255];
    CHAR args[512];
    INT status;
    INT front_window;
    INT len;
    PDS *pinfo;
    FILENAME selected_plotter;
    TCP tcp;
    DOUBLE max_size;
    DOUBLE user_unit_to_inch;
    CHAR layout[40];
    PLOT_LAYOUT pl;
    PL_BASE_WINDOW bw;
    PF_INFO pf_info;

    
    pinfo = (PDS *) TALLOC (sizeof(PDS));
    
    /* check entry conditions                                                       */
    if (pxs == (XS_STRUCT *)0)
        {
        return FAIL;
        }
    len = 1;
    if (len == 0)
        return FAIL;
    
    /* Build dump file name                                                         */
    
    ho_get_unique_filename (dump_file_name);
    strcat (dump_file_name, ".DAT");
    ho_add_path ("ESI$SCRATCH", dump_file_name, sizeof(PATHNAME), dump_file_name);
    ho_translate_filename (dump_file_name, dump_file_name);
    
    /* construct the arguments.                                                     */
    
    ho_add_path ("ESI$BIN", "xs_launch_plots", sizeof(FILENAME), cmd_file_name);
    
    /* --------------------                                                         */
    /* get plot information                                                         */
    /* 1 == XSECT                                                                   */
    /* --------------------                                                         */
    qs_inq_c (QS_FINDER_ACCOUNT, f_account, &len);
    strcpy (pf_info.analyst,f_account);
    strcpy (pf_info.app,"XSECT");
    strcpy (pf_info.finder_path,"ESI$PLOTS");
    strcpy (pf_info.filename,pxs->layout_name);
    pf_info.xsize = pxs->drawing.width;
    pf_info.ysize = pxs->drawing.height;

    if ((status = pf_get_plot_info (pinfo,layout,&pf_info, PL_XSECT)) == FAIL)
        {
        if (pinfo != (PDS *)0)
            {
            tc_free ((VOIDPTR)pinfo);
            }
        return FAIL;
        }
    /* Build neutral/metafile file name, including path                             */
    
    /* inquire the finder account and password and database node.                   */
    qs_inq_c (QS_PROJECT_NAME, project, &len);
    qs_inq_c (QS_FINDER_PASSWORD, f_password, &len);
    qs_inq_c (QS_DATABASE, f_database, &len);
    
    max_size = MAX (pf_info.xsize, pf_info.ysize);
/* ------------------------------------------------------------- */
/* 0.01 small offset to make sure the plot size is not less than */
/* the original one due to rounding                              */
/* ------------------------------------------------------------- */
    max_size + 0.01;

    sprintf (login_phrase, "%s/%s@%s", f_account, f_password, f_database);
    sprintf (args, "\"%s\",%s,\"%s@%s@%s\",%.2f,\"%s\",\"%s@%s\",\"%2d@%1d\",\"%s@%s\"", 
	     login_phrase,
             project, 
	     pf_info.filename,
	     pf_info.meta_type,
	     dump_file_name, 
	     max_size,
	     pf_info.layout,
	     pinfo->driver, 
	     pinfo->queue,
	     pf_info.copies,
	     pf_info.save,
	     pf_info.expired_date,
	     pf_info.remark);
    
    /* dump the xs_struct                                                           */
    
    status = xs_obj_dump (pxs, dump_file_name);
    if (status == FAIL)
        {
        msg ("The pxs dump procedure failed in some way....Aborting plot");

#if NEED_TO_FREE
        if (pinfo != (PDS *)0)
            tc_free (pinfo);
#endif
        return FAIL;
        }
    /* 12/14/88 GS - don't call ho_syscom, be consistent with other plotting rts    */
    status = ho_spawn (SPAWN_BATCH, cmd_file_name, args, (INT *)0);

#if NEED_TO_FREE
    if (pinfo != (PDS *)0)
        tc_free (pinfo);
#endif
    return status;
    }

/*********************************************                                      */
/**                                                                                 */
/** Print strings if debug mode set, else just                                      */
/** return immediatly.                                                              */
/**                                                                                 */
/*********************************************                                      */
static VOID msg (str)
CHAR *str;
    {
#if DEBUG_PLOT == TRUE
    printf ("%s\n", str);
#endif
    return;
    }
/* END:                                                                             */

#if 0
/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This is a test stub for use in the debugger.                                  */
/*                                                                                  */
/*    Set a break point right after the strcpy function, then                       */
/*    'dep/az filename = "jsc$plot:PXS_xx.DAT" where xx are replaced                */
/*    by two digits from a real file spec.                                          */
/*    the file will be read if possible, then the cross section                     */
/*    will be drawn.                                                                */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT zz_read_draw();                                                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT zz_read_draw ()
#else
static INT zz_read_draw ()
#endif
    {
    CHAR filename[128];
    XS_STRUCT *pxs;
    INT status;
    
    strcpy (filename, "JSC$PLOT:PXS_25.DAT");
    
    pxs = xs_obj_load (filename);
    
    if (pxs == (XS_STRUCT *)0)
        {
        msg ("Failed to read it back in.");
        return FAIL;
        }
    status = xs_gr_draw_section (pxs, (CHAR *)0);
    
    return SUCCESS;
    }
/* END:                                                                             */
#endif

/********************************************************************               */
/**                                                                                 */
/**                    Functional Description                                       */
/**                                                                                 */
/**********************************************************************             */
/**                                                                                 */
/**   When the 'Plot Crossection' menu pick is activated, the following             */
/**   actions are performed by the main (interactive) application:                  */
/**                                                                                 */
/**   The Cross Section Application first dumps a copy if its                       */
/**   structure (XS_STRUCT) to a file. The filename is of the                       */
/**   form 'PXS_xx.DAT' where xx is replaced with two digits                        */
/**   from the most dynamic elements in CPU_TIME, in an attempt                     */
/**   to make the filename unique.                                                  */
/**                                                                                 */
/**   Next, the DCL script ESI$BIN:XS_LAUNCH_PLOTS.COM is                           */
/**   submitted as a batch job and the single argument FILENAME                     */
/**   is passed to it.                                                              */
/**                                                                                 */
/**   It is this DCL script that does the actual driving of                         */
/**   XS_PLOTTER.  The scripts responsibilities include:                            */
/**                                                                                 */
/**      a.  Expanding the filename argument into a full                            */
/**          path name.  E.g., the filename PXS_34.DAT                              */
/**          might expand into USERDISK:[USERS.DAN]PXS_34.DAT;1                     */
/**                                                                                 */
/**      b.  Checking the validity of the expanded file spec                        */
/**          and aborting if, for example, the file does'nt                         */
/**          exist or it is unreachable.                                            */
/**                                                                                 */
/**      c.  Deleting the filename after it has been read.                          */
/**                                                                                 */
/**      d.  If requested by an argument switch, generating                         */
/**          and printing a report file for the attempted                           */
/**          operation.                                                             */
/**                                                                                 */
/**      e.  Creating an 'execution symbol' and using it to                         */
/**          run XS_PLOTTER.  The execution symbol is of the                        */
/**          typical form:  BOOT_PLOT == "$ESI$BIN:XS_PLOTTER"                      */
/**                                                                                 */
/**                                                                                 */
/** SUPPORT NOTES:                                                                  */
/**   The DCL file ESI$BIN:XS_LAUNCH_PLOTS.COM is expected to exist.                */
/**   It must be located in ESI$BIN: along with XS_PLOTTER.exe                      */
/**                                                                                 */
/**                                                                                 */
/**        --- GENERAL OVERVIEW OF BOTH LOAD & DUMP ---                             */
/**                                                                                 */
/**   The two routines used to dump & load the data structure are                   */
/**   xs_obj_dump & xs_obj_load respectively.  These two routines                   */
/**   are by nature, VERY data structure dependent.  This is mainly                 */
/**   a result of the time frame in which they were created, and                    */
/**   it is likely that they will be improved in the future....                     */
/**                                                                                 */
/**   For now however, changes to the data-structure definition                     */
/**   in esi_xs_defs.h can, and usually will, require matching                      */
/**   alterations to the load & dump routines.  Familiarity with                    */
/**   these routines is a must if sweeping XS_STRUCT mods are to                    */
/**   be affected.                                                                  */
/**                                                                                 */
/**   For now, however, some obeying some simple rules simplifies                   */
/**   the process of making safe, load/dump transparent data-structure              */
/**   modifications:                                                                */
/**                                                                                 */
/**   1) You may add as much immediate storage to a structure def.                  */
/**      as you want.  Load/Dump check sizeof(XS_STRUCT) to know                    */
/**      how much to read/write.                                                    */
/**                                                                                 */
/**   2) NLIST_HEADERS added to the structure will not be loaded                    */
/**      or saved.  The handling of nlists in-the-raw can be                        */
/**      a messy thing.  The recommended approach is (when                          */
/**      adding code to dump them) to write out at the current                      */
/**      file pos, the name of the nlist, prefixed by its length.                   */
/**      When adding code to the load routine, just read in the                     */
/**      nlist name and call nl_read_nlist().  Simple. Effective.                   */
/**      Slow.                                                                      */
/**                                                                                 */
/**   3) Adding linked lists or btrees requires that you add code to                */
/**      the load/dump routines to properly dump & re-create these                  */
/**      entities.  This is done by replacing the LISTHEAD or TREEHEAD              */
/**      itself with a node count and then looping out that many                    */
/**      nodes to the file.  A LISTHEAD is a structure.  A TREEHEAD                 */
/**      is a (BYTE *).  For both, the following method is used to                  */
/**      store the node count into or over the list/tree head:                      */
/**                                                                                 */
/**    NOTE: A copy of the struct containing these lists is made, all               */
/**          operations are performed on this copy struct so as to not              */
/**          disturb the active pointers in the original.                           */
/**                                                                                 */
/**        unsigned  *pintu;                                                        */
/**        LISTHEAD  linklist;                                                      */
/**                                                                                 */
/**        pintu = &copy_struct.linklist;                                           */
/**                                                                                 */
/**    \* replace the first four bytes of LISTHEAD with a four     *\               */
/**    \* byte unsigned containing the number of list nodes within *\               */
/**        *pintu = llcount(&orig_struct.linklist);                                 */
/**                                                                                 */
/**        following this, the actual data from the list is stored.                 */
/**                                                                                 */
/**   4) Adding a pointer to allocated storage also requires                        */
/**      adding code to the load/dump modules to appropriatly                       */
/**      this storage.  Again, the pointer is replaced with the                     */
/**      length of the object that is points to.  Then immediatly                   */
/**      following this comes that # of bytes representing the target.              */
/**                                                                                 */
/**   5) Deleting storage that is immediatly contained is O.K.                      */
/**                                                                                 */
/**                                                                                 */
/** Julian Carlisle   July 1, 1987                                                  */
/** James Chang       july 28, 1987                                                 */
/********************************************************************               */
