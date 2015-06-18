/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_LD_LYT.C*/
/* *3    17-AUG-1990 22:36:13 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    20-JUL-1989 13:00:28 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:43:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_LD_LYT.C*/
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
#include "esi_xs_gr.h"

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#define DO_PWELL   FALSE
#define DEBUG_LOAD  TRUE
static VOID msg ();             /* debug-active print routine */


#define NULL_PXS  ((XS_STRUCT *) 0)

/**********************************************************************
*
* ERROR_EXIT  - Macro for error return.
*
*  First free any allocated memory, then close
*  any files that are open, finally, return
*  a null XS_STRUCT pointer.
*
***********************************************************************/
#define ERROR_EXIT      if (pxs != NULL_PXS)  \
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
        
#define RS(var)  ho_read(fp, ((CHAR *)(&var)), sizeof (USHORT))

/* File Description ---------------------------------------------------------
Overview:
    Read file and re-construct pxs.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef XS_STRUCT *xsz_load_bin_lyt(CHAR  *filename);

Private_Functions:
    static VOID msg(CHAR *str);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef XS_STRUCT *xsz_load_bin_lyt(CHAR  *filename);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(CHAR *) VMS filename for dump file.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    Function also returns the value of pxs.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef XS_STRUCT *xsz_load_bin_lyt (CHAR *filename)
#else
publicdef XS_STRUCT *xsz_load_bin_lyt (filename)
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
    
    XS_STRUCT *pxs;                 /* master copy of XS_STRUCT ptr */
    
    LAYOUT_STRUCT *pls;             /* layout pointer               */
    
    TRACK_STRUCT *pt;               /* track struct pointer         */
    
    LP_TRACE_STRUCT *ptrace;        /* Trace struct ptr            */
    
    TOP_INFO *ptinfo;               /* Top info struct ptr          */
    
    static TOP_STRUCT top;      /* top struct from linked list  */
    
    USHORT nlyts,                   /* # of layouts (log plots)     */
        nitems,                     /* # of log hdr items (strings) */
        ntracks,                    /* # of tracks in each layout   */
        ntraces,                    /* # of traces in each layout   */
        ntops,                      /* # of tops in Cross Section   */
        nuwis;                      /* # of wells with each top     */
    
    static UWI uwikey;          /* BTKEY for ptinfo->uwis       */
    
    FILE *fp;                       /* File pointer to output file  */
    
    /**************** End Equates,  Start Code. *********************/
    
    pxs = NULL_PXS;
    pt = (TRACK_STRUCT *)0;
    pls = (LAYOUT_STRUCT *)0;
    ptrace = (LP_TRACE_STRUCT *)0;
    fp = (FILE *)0;
    
    /* check entry conditions, TRY to fail! */
    if (filename == ((CHAR *)0))
        {
        sprintf (string, "Error:  Invalid filename....Aborting.\n");
        msg (string);
        ERROR_EXIT;
        }
    /* open input file */
    if ((fp = ho_open (filename, "r", NULL)) == (FILE *)0)
        {
        sprintf (string, "Error opening file %s for reading.\n", filename);
        msg (string);
        ERROR_EXIT;
        }
    /* 1st read - get the version of binary */
    if ((f_stat = RS (tmp_ushort)) == 0)
        {
        msg ("The binary version is not current..");
        ERROR_EXIT;
        }
    if (tmp_ushort IS_NOT_EQUAL_TO XS_STRUCT_VERSION)
        {
        ERROR_EXIT;
        }
    pxs = ALLOCATE (XS_STRUCT);
    
    /* 1st read - get length of xs struct but ignore since already known */
    if ((f_stat = RS (tmp_ushort)) == 0)
        {
        msg ("Failed on first read.... aborting.");
        ERROR_EXIT;
        }
    if ((f_stat = ho_read (fp, pxs, sizeof(XS_STRUCT))) == 0)
        {
        msg ("Failed trying to read main template. Aborting...");
        ERROR_EXIT;
        }
    /* To zero out the run time variables in the front of structure. */
    tc_zeroes (pxs, sizeof(INT) * 31);
    
    pxs->drawing.window_id = 0;
    nlyts = 1;
    
    pintu = (unsigned *) & (pxs->drawing.top_table);
    ntops = *pintu;
    list_status = llinit (&pxs->drawing.top_table, 0, sizeof(TOP_INFO *), 0);
#if DEBUG_LOAD == TRUE

#endif
    
    /* load the binary of pxs -> plo sample layout.              */
    
    f_stat = RS (tmp_ushort);
    pxs->plo = pls = ALLOCATE (LAYOUT_STRUCT);
    
    if ((f_stat = ho_read (fp, pls, sizeof(LAYOUT_STRUCT))) == 0)
        {
        msg ("Error reading one of the LAYOUT templates.. Aborting.");
        ERROR_EXIT;
        }
    pintu = (unsigned *) & (pls->tracks);
    ntracks = *pintu;
    llinit (&(pls->tracks), 0, sizeof(TRACK_STRUCT *), 0);
    
    pintu = (unsigned *) & (pls->traces);
    ntraces = *pintu;
    llinit (&(pls->traces), 0, sizeof(LP_TRACE_STRUCT *), 0);
    
    /* Here is the first of the 3 linked list sub loops */
    /* this one loads the hdr_items list of string pointers */
    
    tmp_ushort = sizeof(INT) * pls->log_hdr.total_items;
    pls->log_hdr.items = (INT *)tc_zalloc (sizeof(INT) * pls->log_hdr.total_items);
    if (tmp_ushort != 0)
        {
        if ((f_stat = ho_read (fp, pls->log_hdr.items, tmp_ushort)) == 0)
            {
            msg ("Error while reading hdr item string... Aborting.");
            ERROR_EXIT;
            }
        }
    /* Subloop 2 of 3, this one is to reload the pls->tracks list */
    for (i = 0; i < ntracks; i++)
        {
        f_stat = RS (tmp_ushort);
        pt = ALLOCATE (TRACK_STRUCT);
        
        if ((f_stat = ho_read (fp, pt, sizeof(TRACK_STRUCT))) == 0)
            {
            msg ("Error while reading track struct... Aborting.");
            ERROR_EXIT;
            }
        list_status = llappnd (&pls->tracks, &pt);
        }
    /* Subloop 3 of 3, load the traces list */
    for (i = 0; i < ntraces; i++)
        {
        f_stat = RS (tmp_ushort);
        ptrace = ALLOCATE (LP_TRACE_STRUCT);
        
        if ((f_stat = ho_read (fp, ptrace, sizeof(LP_TRACE_STRUCT))) == 0)
            {
            msg ("Error while reading trace struct... Aborting.");
            ERROR_EXIT;
            }
        list_status = llappnd (&pls->traces, &ptrace);
        }
    /* Now loop ntops times and restore the top_table list */
    for (n = 0; n < ntops; n++)
        {
        f_stat = RS (tmp_ushort);
        ptinfo = ALLOCATE (TOP_INFO);
        
        if ((f_stat = ho_read (fp, ptinfo, sizeof(TOP_INFO))) == 0)
            {
            msg ("Error while reading top info struct... Aborting.");
            ERROR_EXIT;
            }
        ptinfo->seg_id = 0;
        list_status = llappnd (&pxs->drawing.top_table, &ptinfo);
        status = btinit (&(ptinfo->uwis), 0, sizeof(UWI), sizeof(TOP_STRUCT),
                         BT_KEY_IS_C_STRING | BT_TREE_SYSTEM);
        
        }
    return pxs;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
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
    PRIVATE to <module_description>

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
#if DEBUG_LOAD == TRUE
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
