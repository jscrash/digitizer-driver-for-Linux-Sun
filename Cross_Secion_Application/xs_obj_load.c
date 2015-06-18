/* DEC/CMS REPLACEMENT HISTORY, Element XS_OBJ_LOAD.C*/
/* *3    17-AUG-1990 22:41:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JAN-1990 08:52:36 GILLESPIE "(SPR 6001) Changes for new wells struct stuff"*/
/* *1    19-JUN-1989 13:44:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_OBJ_LOAD.C*/
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

#include "esi_xs_gr.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#define DO_PWELL   FALSE
#define DEBUG_LOAD  TRUE

#if USE_PROTOTYPES
static VOID msg (char *string); /* debug-active print routine */
#else
static VOID msg ();             /* debug-active print routine */
#endif

#define NULL_PXS  ((XS_STRUCT *) 0)

/**********************************************************************
*
* err_exit()  - Macro for error return.
*
*  First free any allocated memory, then close
*  any files that are open, finally, return
*  a null XS_STRUCT pointer.
*
***********************************************************************/
#define err_exit()      if (pxs != NULL_PXS)  \
                tc_free(pxs);         \
            if (pt != (TRACK_STRUCT *)0) \
                tc_free(pt);  \
            if (pls != (LAYOUT_STRUCT *)0) \
                tc_free(pls);  \
            if (ptrace != (LP_TRACE_STRUCT *)0) \
                tc_free(ptrace);  \
            if (fp != (FILE *)0)  \
               fclose(fp);           \
            return (NULL_PXS)
        
#define RS(var)  fread(((CHAR *)(&var)), sizeof (USHORT), 1, fp)

/***********************************************************************/

/* File Description ---------------------------------------------------------
Overview:
    Read file and re-construct pxs.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef XS_STRUCT *xs_obj_load(CHAR *filename);

Private_Functions:
    static VOID msg(char *str);
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Xs_obj_load reads the given file in the opposite order
    that xs_obj_dump wrote it.  It re-allocates the XS_STRUCT
    and returns a pointer to it.

Prototype:
    publicdef XS_STRUCT *xs_obj_load(CHAR *filename);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(CHAR *) VMS filename for dump file.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    FUNCTIONAL DETAIL........See comment section at end of this file.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef XS_STRUCT *xs_obj_load (CHAR *filename)
#else
publicdef XS_STRUCT *xs_obj_load (filename)
CHAR *filename;
#endif
    {
    unsigned *pintu;                /* Gen. purp. unsigned INT ptr  */
    static USHORT tmp_ushort;   /* Gen. purpose unsigned SHORT  */
    
    INT n, i,                       /* loop control vars.           */
        list_status,                /* ll & bt status return        */
        f_stat,                     /* file i/o status              */
        status;                     /* Gen. status for all others   */
    
    CHAR *strptr,                   /* general string pointer       */
        string[255];                /* gen. purpose string buffer   */
    
    XS_STRUCT *pxs = 0;             /* master copy of XS_STRUCT ptr */
    
    LAYOUT_STRUCT *pls = 0;         /* layout pointer               */
    
    TRACK_STRUCT *pt = 0;           /* track struct pointer         */
    
    LP_TRACE_STRUCT *ptrace = 0;    /* Trace struct ptr            */
    
    TOP_INFO *ptinfo = 0;           /* Top info struct ptr          */
    
    TOP_STRUCT top;                 /* top struct from linked list  */
    
    USHORT nlyts,                   /* # of layouts (log plots)     */
        nitems,                     /* # of log hdr items (strings) */
        ntracks,                    /* # of tracks in each layout   */
        ntraces,                    /* # of traces in each layout   */
        ntops,                      /* # of tops in Cross Section   */
        nuwis;                      /* # of wells with each top     */
    
    UWI uwikey;                     /* BTKEY for ptinfo->uwis       */
    
    FILE *fp = 0;                   /* File pointer to output file  */
    
    /**************** End Equates,  Start Code. *********************/
    
    /* check entry conditions, TRY to fail! */
    if (filename EQUALS ((CHAR *)0))
        {
        sprintf (string, "Error:  Invalid filename....Aborting.\n");
        msg (string);
        err_exit ();
        }
    /* open input file */
    if ((fp = ho_open (filename, "r", NULL)) EQUALS (FILE *)0)
        {
        sprintf (string, "Error opening file %s for reading.\n", filename);
        msg (string);
        err_exit ();
        }
    printf ("Loading from %s \n", filename);
    
    pxs = ALLOCATE (XS_STRUCT);
    
    /* 1st read - get length of xs struct but ignore since already known */
    if ((f_stat = RS (tmp_ushort)) EQUALS 0)
        {
        msg ("Failed on first read.... aborting.");
        err_exit ();
        }
    if ((f_stat = fread (pxs, sizeof(XS_STRUCT), 1, fp)) EQUALS 0)
        {
        msg ("Failed trying to read main template. Aborting...");
        err_exit ();
        }
    pintu = (unsigned *) & (pxs->drawing.log_plots);
    nlyts = *pintu;
    list_status = llinit (&pxs->drawing.log_plots, 0, sizeof(LAYOUT_STRUCT *), 0);
    
    pintu = (unsigned *) & (pxs->drawing.top_table);
    ntops = *pintu;
    list_status = llinit (&pxs->drawing.top_table, 0, sizeof(TOP_INFO *), 0);

#if DEBUG_LOAD EQUALS TRUE
    printf ("LOAD: # layouts = %d\n", nlyts);
    printf ("LOAD: # ntops = %d\n", ntops);
#endif
    
    /* loop nlyts times to load up the log_plots linked list */
    for (n = 0; n < nlyts; n++)
        {
        
        f_stat = RS (tmp_ushort);
        pls = ALLOCATE (LAYOUT_STRUCT);
        
        if ((f_stat = fread (pls, sizeof(LAYOUT_STRUCT), 1, fp)) EQUALS 0)
            {
            msg ("Error reading one of the LAYOUT templates.. Aborting.");
            err_exit ();
            }
        pintu = (unsigned *) & (pls->log_hdr.hdr_items);
        nitems = *pintu;
        llinit (&(pls->log_hdr.hdr_items), 0, sizeof(CHAR *), 0);
        
        pintu = (unsigned *) & (pls->tracks);
        ntracks = *pintu;
        llinit (&(pls->tracks), 0, sizeof(TRACK_STRUCT *), 0);
        
        pintu = (unsigned *) & (pls->traces);
        ntraces = *pintu;
        llinit (&(pls->traces), 0, sizeof(LP_TRACE_STRUCT *), 0);
        
        /* Here is the first of the 3 linked list sub loops */
        /* this one loads the hdr_items list of string pointers */
        for (i = 0; i < nitems; i++)
            {
            f_stat = RS (tmp_ushort);
            /* JGG 28-June-88 loader seems to be putting out empty strings -
                skip read of data item when this occurs */
            if (tmp_ushort > 0)
                {
                strptr = (CHAR *)tc_zalloc (sizeof(CHAR) * (tmp_ushort + 1));
                
                if ((f_stat = fread (strptr, tmp_ushort, 1, fp)) EQUALS 0)
                    {
                    msg ("Error while reading hdr item string... Aborting.");
                    err_exit ();
                    }
                list_status = llappnd (&pls->log_hdr.hdr_items, &strptr);
                }
            }
        /* Subloop 2 of 3, this one is to reload the pls->tracks list */
        for (i = 0; i < ntracks; i++)
            {
            f_stat = RS (tmp_ushort);
            pt = ALLOCATE (TRACK_STRUCT);
            
            if ((f_stat = fread (pt, sizeof(TRACK_STRUCT), 1, fp)) EQUALS 0)
                {
                msg ("Error while reading track struct... Aborting.");
                err_exit ();
                }
            list_status = llappnd (&pls->tracks, &pt);
            }
        /* Subloop 3 of 3, load the traces list */
        for (i = 0; i < ntraces; i++)
            {
            f_stat = RS (tmp_ushort);
            ptrace = ALLOCATE (LP_TRACE_STRUCT);
            
            if ((f_stat = fread (ptrace, sizeof(LP_TRACE_STRUCT), 1, fp)) EQUALS 0)
                {
                msg ("Error while reading trace struct... Aborting.");
                err_exit ();
                }
            status = lg_ltdm_get_trace (&(ptrace->trace_struct));
            if (status EQUALS SUCCESS)
                {
                list_status = llappnd (&pls->traces, &ptrace);
                }
            else
                {
                sprintf (string, "Error %s reading log trace - continuing",
                         mg_message (status));
                msg (string);
                }
            }
        /* finished with subloops... This is the last thing to do */
        /* for each layout... Load the pwell struct */
        pls->pwell = ALLOCATE (WELL_STRUCT);
        f_stat = RS (tmp_ushort);
        
        if ((f_stat = fread (pls->pwell, sizeof(WELL_STRUCT), 1, fp)) EQUALS 0)
            {
            msg ("Error while reading layout well struct... Aborting.");
            err_exit ();
            }
        list_status = llappnd (&pxs->drawing.log_plots, &pls);
        }
    /* Now loop ntops times and restore the top_table list */
    for (n = 0; n < ntops; n++)
        {
        f_stat = RS (tmp_ushort);
        ptinfo = ALLOCATE (TOP_INFO);
        
        if ((f_stat = fread (ptinfo, sizeof(TOP_INFO), 1, fp)) EQUALS 0)
            {
            msg ("Error while reading top info struct... Aborting.");
            err_exit ();
            }
        pintu = (unsigned *) & ptinfo->uwis;
        nuwis = *pintu;
        
        list_status = btinit (&ptinfo->uwis, 0, sizeof(UWI), sizeof(TOP_INFO),
                              BT_KEY_IS_C_STRING);
        
        /* this is the subloop that reads in nuwis TOP structs */
        for (i = 0; i < nuwis; i++)
            {
            if ((f_stat = fread (uwikey, sizeof(UWI), 1, fp)) EQUALS 0)
                {
                msg ("Error while reading uwi key for TOPS... Aborting.");
                err_exit ();
                }
            f_stat = RS (tmp_ushort);   /* get len of top_struct */
            
            if ((f_stat = fread ((CHAR *)(&top), sizeof(TOP_STRUCT), 1, fp)) EQUALS 0)
                {
                msg ("Error while reading top struct... Aborting.");
                err_exit ();
                }
            list_status = btadd (&ptinfo->uwis, uwikey, &top);
            }
        list_status = llappnd (&pxs->drawing.top_table, &ptinfo);
        }
    msg ("### Finished reading in cross section definition.");
    return pxs;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Print strings for debugging.

    This routine is only active if DEBUG_DUMP
    is defined as TRUE.  Otherwise, it returns
    immediately.


Prototype:
    static VOID msg(CHAR *str);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    str             -(CHAR *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <xs_obj_load.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID msg (CHAR *str)
#else
static VOID msg (str)
CHAR *str;
#endif
    {
#if DEBUG_LOAD EQUALS TRUE
    printf ("%s\n", str);
#endif
    return;
    }
/* END:     */

/********************************************************************
*
*                         Documentation Block
*
*********************************************************************
*
*        --- GENERAL OVERVIEW OF BOTH LOAD & DUMP ---
*
*   The two routines used to dump & load the data structure are
*   xs_obj_dump & xs_obj_load respectively.  These two routines
*   are by nature, VERY data structure dependent.  This is mainly
*   a result of the time frame in which they were created, and
*   it is likely that they will be improved in the future....
*
*   For now however, changes to the data-structure definition
*   in esi_xs_defs.h can, and usually will, require matching
*   alterations to the load & dump routines.  Familiarity with
*   these routines is a must if sweeping XS_STRUCT mods are to
*   be affected.
*
*   For now, however, some obeying some simple rules simplifies
*   the process of making safe, load/dump transparent data-structure
*   modifications:
*
*   1) You may add as much immediate storage to a structure def.
*      as you want.  Load/Dump check sizeof(XS_STRUCT) to know
*      how much to read/write.
*
*   2) NLIST_HEADERS added to the structure will not be loaded
*      or saved.  The handling of nlists in-the-raw can be
*      a messy thing.  The recommended approach is (when
*      adding code to dump them) to write out at the current
*      file pos, the name of the nlist, prefixed by its length.
*      When adding code to the load routine, just read in the
*      nlist name and call nl_read_nlist().  Simple. Effective.
*      Slow.
*
*   3) Adding linked lists or btrees requires that you add code to
*      the load/dump routines to properly dump & re-create these
*      entities.  This is done by replacing the LISTHEAD or TREEHEAD
*      itself with a node count and then looping out that many
*      nodes to the file.  A LISTHEAD is a structure.  A TREEHEAD
*      is a (BYTE *).  For both, the following method is used to
*      store the node count into or over the list/tree head:
*
*    NOTE: A copy of the struct containing these lists is made, all
*          operations are performed on this copy struct so as to not
*          disturb the active pointers in the original.
*
*        unsigned  *pintu;
*        LISTHEAD  linklist;
*
*        pintu = &copy_struct.linklist;
*
*    \* replace the first four bytes of LISTHEAD with a four     *\
*    \* byte unsigned containing the number of list nodes within *\
*        *pintu = llcount(&orig_struct.linklist);
*
*        following this, the actual data from the list is stored.
*
*   4) Adding a pointer to allocated storage also requires
*      adding code to the load/dump modules to appropriatly
*      this storage.  Again, the pointer is replaced with the
*      length of the object that is points to.  Then immediatly
*      following this comes that # of bytes representing the target.
*
*   5) Deleting storage that is immediatly contained is O.K.
*
***********************************************************************/
