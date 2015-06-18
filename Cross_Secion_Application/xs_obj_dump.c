/* DEC/CMS REPLACEMENT HISTORY, Element XS_OBJ_DUMP.C*/
/* *3    14-AUG-1991 12:02:27 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *2    17-AUG-1990 22:41:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_OBJ_DUMP.C*/
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

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#define DEBUG_DUMP  FALSE
#define SHOW_COUNT  FALSE
#define DO_PWELL FALSE
static VOID msg ();             /* debug-active print routine */

/*****************************************************************
* Data size specific file write macros.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

/* 2 bytes */
#define WRITE_SHORT(var) fwrite((CHAR *)(&(var)),sizeof(SHORT),1,fp)

/* 4 bytes */
#define WRITE_INT(var)   fwrite((CHAR *)(&(var)),sizeof(INT),  1,fp)

/**********************************************************************
*
* err_exit()  - Macro for error return.
*
***********************************************************************/
#define NULL_PXS    ((XS_STRUCT *)0)
#define err_exit()      if (tmpxs != NULL_PXS)  \
                tc_free(tmpxs);     \
            if (tmpls != (LAYOUT_STRUCT *)0) \
                tc_free(tmpls);   \
            if (tmptinfo != (TOP_INFO *)0) \
                tc_free(tmptinfo);  \
            if (fp != NULL_FILE_PTR)  \
                fclose(fp);       \
            return FAIL
        
#define perr_exit(str)  printf("%s\n",str);  \
                        err_exit();

/* File Description ---------------------------------------------------------
Overview:
    Creates a disk file image of the Cross Section Object.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_obj_dump(XS_STRUCT   *pxs,FILENAME filename);

Private_Functions:
    static VOID msg(CHAR *str);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Archive the contents of the Cross Section Object to a file.
    All members, sub-structures, linked lists, btrees etc. are
    analyzed and written out to the disk file.

    USAGE:  xs_obj_dump(pxs,filename);
        XS_STRUCT  *pxs;             
        CHAR  *filename;             

Prototype:
    publicdef INT xs_obj_dump(XS_STRUCT   *pxs,FILENAME filename);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XS object.
    filename        -(CHAR *) Name to create file under.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    A detailed description of the dumping format etc. may be found at
    the end of this file.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_obj_dump (XS_STRUCT *pxs, FILENAME filename)
#else
publicdef INT xs_obj_dump (pxs, filename)
XS_STRUCT *pxs;
FILENAME filename;
#endif
    {
    unsigned long *pintu;           /* Gen. purp. unsigned INT ptr  */
    static USHORT tmp_ushort;   /* Gen. purpose unsigned SHORT  */
    
    INT n, i,                       /* loop control vars.           */
        list_status,                /* ll & bt status return        */
        fstatus,                    /* file i/o status              */
        status;                     /* Gen. status for all others   */
    
    CHAR *strptr,                   /* general string pointer       */
        string[255];                /* gen. purpose string buffer   */
    
    XS_STRUCT *tmpxs = 0;           /* temp copy of pxs struct      */
    
    LAYOUT_STRUCT *pls,             /* layout pointer               */
        *tmpls = 0;                 /* temp layout ptr              */
    
    TRACK_STRUCT *ptrack,           /* track struct pointer         */
        *tmpt = 0;                  /* temp. of the same            */
    
    LP_TRACE_STRUCT *ptrace = 0;    /* Trace struct ptr            */
    
    TOP_INFO *ptinfo,               /* Top info struct ptr          */
        *tmptinfo = 0;              /* temp vers of same            */
    
    WELL_STRUCT *tmpwell;
    
    static TOP_STRUCT top;      /* top struct from linked list  */
    
    INT nlyts,                      /* # of layouts (log plots)     */
        nitems,                     /* # of log hdr items (strings) */
        ntrks,                      /* # of tracks in each layout   */
        ntrcs,                      /* # of traces in each layout   */
        ntops,                      /* # of tops in Cross Section   */
        nuwis,                      /* # of wells with each top     */
        nvis;
    static UWI uwikey;          /* BTKEY for ptinfo->uwis       */
    
    FILE *fp;                       /* File pointer to output file  */
    
    /**************** End Equates,  Start Code. *********************/
    
    /* check entry conditions, TRY to fail! */
    if (pxs == ((XS_STRUCT *)0) OR (strlen (filename) EQUALS 0))
        {
        sprintf (string, "Error:  Invalid struct. ptr or filename....Aborting.\n");
        msg (string);
        return FAIL;
        }
    /* open output file */
    fp = ho_open (filename, "w", NULL);
    status = ho_lock (fp, ON);
    if (fp == (FILE *)0 || status != SUCCESS)
        {
        sprintf (string, "Error opening file %s for writing.\n", filename);
        msg (string);
        return FAIL;
        }
    /* copy pxs into a temp XS_STRUCT we can alter w/out affecting the real one */
    tmpxs = (XS_STRUCT *)tc_zalloc (sizeof(XS_STRUCT));
    hoblockmove (pxs, tmpxs, sizeof(XS_STRUCT));
    
    /* insert the # of links in place of the LISTHEADS themseleves */
    pintu = (unsigned long *) & (tmpxs->drawing.log_plots);
    *pintu = nlyts = llcount (&pxs->drawing.log_plots);
    if (nlyts < 0)
        {
        *pintu = nlyts = 0;
        }
#if SHOW_COUNT == TRUE
    sprintf (string, "Dumping %d elements from log_plots.", nlyts);
    msg (string);
#endif
    pintu = (unsigned long *) & (tmpxs->drawing.top_table);
    ntops = llcount (&pxs->drawing.top_table);
    
    /* find how many of the tops are visible, we only store those */
    for (nvis = 0, n = 0; n < ntops; n++)
        {
        list_status = (n == 0) ? llfirst (&pxs->drawing.top_table,
                                          &ptinfo) : llnext (&pxs->drawing.top_table,
                                                             &ptinfo);
        
        if (ptinfo->visible)
            nvis++;
        }
    *pintu = nvis;
    
    if (ntops <= 0)
        {
        *pintu = ntops = 0;
        nuwis = 0;
        }
    /* the first WORD written shows the # of bytes to follow that make */
    /* up the main structure body (& soul?) */
    tmp_ushort = (USHORT)sizeof(XS_STRUCT);
    
    /* write out the length of the structure in bytes */
    if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
        {
        err_exit ();                /* Abort if nothing written (I/O Err.) */
        }
    /* and then the structure itself */
    if ((fstatus = fwrite ((CHAR *)tmpxs, tmp_ushort, 1, fp)) == 0)
        {
        err_exit ();                /* Abort if nothing written (I/O Err.) */
        }
    /* create a work version of the layout struct */
    tmpls = (LAYOUT_STRUCT *)tc_zalloc (sizeof(LAYOUT_STRUCT));
    
    /******************************************
    *                                         *
    * Dump the linked list of LAYOUT_STRUCT   *
    * pointers.                               *
    *                                         *
    *******************************************/
    msg ("Dumping Log_plot linked list...");
    for (n = 0; n < nlyts; n++)
        {
        list_status = (n == 0) ? llfirst (&pxs->drawing.log_plots,
                                          &pls) : llnext (&pxs->drawing.log_plots,
                                                          &pls);
        
        if (pls == (LAYOUT_STRUCT *)0)
            {
            perr_exit ("got null pls...Aborting");
            }
        /* write out the size of the next record (layout struct to follow) */
        tmp_ushort = (USHORT)sizeof(LAYOUT_STRUCT);
        
        /* write the size of the next record before the record (Pascal style) */
        if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
            {
            err_exit ();            /* Abort if nothing written (I/O Err.) */
            }
        /* make our 'work' version of the layout struct */
        hoblockmove (pls, tmpls, sizeof(LAYOUT_STRUCT));
        
        /* de-reference the pwell, replace it w/num bytes in datastructure */
        tmpls->pwell = (WELL_STRUCT *)sizeof(WELL_STRUCT);
        
        /* replace the various linked list LISTHEADS with the # of items in list */
        pintu = (unsigned long *) & (tmpls->log_hdr.hdr_items);
        *pintu = nitems = llcount (&pls->log_hdr.hdr_items);
        if (nitems < 0)
            {
            *pintu = nitems = 0;
            }
#if SHOW_COUNT == TRUE
        sprintf (string, "Dumping %d elements from items.", nitems);
        msg (string);
#endif
        pintu = (unsigned long *) & (tmpls->tracks);
        *pintu = ntrks = llcount (&pls->tracks);

#if SHOW_COUNT == TRUE
        sprintf (string, "Dumping %d elements from pls->tracks.", ntrks);
        msg (string);
#endif
        pintu = (unsigned long *) & (tmpls->traces);
        *pintu = ntrcs = llcount (&pls->traces);

#if SHOW_COUNT == TRUE
        sprintf (string, "Dumping %d elements from pls->traces.", ntrcs);
        msg (string);
#endif
        
        /* write out the actual LAYOUT structure contents */
        if ((fstatus = fwrite ((CHAR *)tmpls, tmp_ushort, 1, fp)) == 0)
            {
            err_exit ();            /* Abort if nothing written (I/O Err.) */
            }
        /*******************************************
        *                                          *
        * Dump the linked list of log header items *
        * Each item is a pointer to a char string. *
        *                                          *
        ********************************************/
        for (i = 0; i < nitems; i++)
            {
            list_status = (i == 0) ? llfirst (&pls->log_hdr.hdr_items,
                                              &strptr) :
                          llnext (&pls->log_hdr.hdr_items, &strptr);
            
            tmp_ushort = (USHORT)strlen (strptr);
            
            /* first the length count of this string */
            if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            /* and now the string */
            if ((fstatus = fwrite ((CHAR *)strptr, tmp_ushort, 1, fp)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            }
        /******************************************
        *                                         *
        * Dump the linked list of TRACK_STRUCT    *
        * pointers.                               *
        *                                         *
        *******************************************/
        tmp_ushort = (USHORT)sizeof(TRACK_STRUCT);
        for (i = 0; i < ntrks; i++)
            {
            list_status = (i == 0) ? llfirst (&pls->tracks,
                                              &ptrack) : llnext (&pls->tracks,
                                                                 &ptrack);
            
            if (ptrack == (TRACK_STRUCT *)0)
                {
                perr_exit ("got null track ptr");
                }
            /* put the size before the record (Pascal style) */
            if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            /* write out this track struct */
            if ((fstatus = fwrite ((CHAR *)ptrack, tmp_ushort, 1, fp)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            }
        /******************************************
        *                                         *
        * Dump the linked list of lp_trace_struct *
        * pointers.                               *
        *                                         *
        *******************************************/
        tmp_ushort = (USHORT)sizeof(LP_TRACE_STRUCT);
        for (i = 0; i < ntrcs; i++)
            {
            list_status = (i == 0) ? llfirst (&pls->traces,
                                              &ptrace) : llnext (&pls->traces,
                                                                 &ptrace);
            
            if (ptrace == (LP_TRACE_STRUCT *)0)
                {
                perr_exit ("got null ptrace");
                }
            /* again, precede the next record with its byte count */
            if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            /* write out the trace struct */
            if ((fstatus = fwrite ((CHAR *)ptrace, tmp_ushort, 1, fp)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            }
        tmp_ushort = (USHORT)sizeof(WELL_STRUCT);
        if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
            {
            err_exit ();            /* Abort if nothing written (I/O Err.) */
            }
        /* write out the WELL_STRUCT */
        if ((fstatus = fwrite ((CHAR *)pls->pwell, sizeof(WELL_STRUCT), 1, fp)) == 0)
            {
            err_exit ();            /* Abort if nothing written (I/O Err.) */
            }
        }
    /*********************************************
    *                                            *
    * In a manner similar to the way we archived *
    * the log_plots linked list, we now archive  *
    * the top_table linked list.                 *
    *                                            *
    *********************************************/
    msg ("Dumping Top_Table linked list...");
    
    /* make a 'work' version of the top_info struct */
    tmptinfo = (TOP_INFO *)tc_zalloc (sizeof(TOP_INFO));
    
    for (n = 0; n < ntops; n++)
        {
        list_status = (n == 0) ? llfirst (&pxs->drawing.top_table,
                                          &ptinfo) : llnext (&pxs->drawing.top_table,
                                                             &ptinfo);
        
        if (list_status < 0)
            {
            sprintf (string, "List_status from top_table = %d \n", list_status);
            msg (string);
            }
        if (ptinfo == ((TOP_INFO *)0) OR (list_status < 0))
            {
            perr_exit ("Ptinfo was null when retrieved from top_table...Aborting");
            }
        if (ptinfo->visible != TRUE)
            {
            continue;
            }
        /* first word (16 bits) of record describes */
        /* the length of each TOP_INFO record */
        tmp_ushort = (USHORT)sizeof(TOP_INFO);
        
        if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
            {
            err_exit ();            /* Abort if nothing written (I/O Err.) */
            }
        /* make a safe-to-fiddle-with version of this top_info struct */
        hoblockmove (ptinfo, tmptinfo, sizeof(TOP_INFO));
        
        nuwis = btcount (&ptinfo->uwis);
        if (nuwis < 0)
            {
            nuwis = 0;
            }
#if SHOW_COUNT == TRUE
        sprintf (string, "Dumping %d elements from ptinfo->uwis.", nuwis);
        msg (string);
#endif
        
        /* tmptinfo->uwis is a TREEHEAD.  Otherwise known as (BYTE *) */
        /* so cast the uwi count into a 4 byte unsigned integer and */
        /* store it into the 4 bytes that are the BYTE *.    */
        tmptinfo->uwis = (TREEHEAD)nuwis;
        
        /* write this top_info struct */
        if ((fstatus = fwrite ((CHAR *)tmptinfo, tmp_ushort, 1, fp)) == 0)
            {
            err_exit ();            /* Abort if nothing written (I/O Err.) */
            }
        /******************************************************************
        *
        * Dump the tops btree that lives in each top_info structs.
        * Each node in the tree is a TOP_STRUCT.
        *
        * for each uwis tree node within the current ptinfo, check the
        * validity of the node, and either write it out or write
        * out a marker stating that this node is invalid. (pwell pointer
        * might be NULL for instance ).
        *
        ******************************************************************/
        
        /* For each well....*/
        for (i = 0; i < nuwis; i++)
            {
            list_status = (i == 0) ? btfirst (&ptinfo->uwis, uwikey,
                                              &top) : btnext (&ptinfo->uwis, uwikey,
                                                              &top);
            
            if (list_status < 0)
                {
                perr_exit ("error accessing the uwis btree");
                }
            /* first part of each tnode record is the BTKEY (UWI)  */
            
            /* WRITE UWI */
            if ((fstatus = fwrite ((CHAR *)uwikey, sizeof(UWI), 1, fp)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            tmp_ushort = (USHORT)sizeof(TOP_STRUCT);
            
            /* WRITE TOP_STRUCT LENGTH */
            if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            /* WRITE CONTENTS OF TOP_STRUCT */
            if ((fstatus = fwrite ((CHAR *) & top, tmp_ushort, 1, fp)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            /* Validate the pWell pointer.  If the pwell is null, set tmp_ushort */
            /* to 0 otherwise set tmp_ushort to the length of a WELL_STRUCT */

#if DO_PWELL == TRUE
            tmp_ushort = (top.pWell == (WELL_STRUCT *)0) ? 0 : sizeof(WELL_STRUCT);
            if (top.Depth.md < (HUGE_DEPTH / 2.0) OR (top.pWell < 100000))
                {
                tmp_ushort = top.pWell = 0;
                }
            /* WRITE out pwell length/ valid flag.  When reading back, this will */
            /* be checked to determine whether or not to try to re-construct it */
            
            if ((fstatus = WRITE_SHORT (tmp_ushort)) == 0)
                {
                err_exit ();        /* Abort if nothing written (I/O Err.) */
                }
            /* if object is > 0 bytes, write it */
            if (tmp_ushort > 0 AND top.pWell > 100000)
                {
                /* replace the well_struct ptr with the actual size of its target */
                tmpwell = top.pWell;    /* make a tmp copy of the ptr */
                
                pintu = (unsigned long *)top.pWell;
                *pintu = sizeof(WELL_STRUCT);
                
                if ((fstatus = fwrite ((CHAR *)tmpwell, (*pintu), 1, fp)) == 0)
                    {
                    err_exit ();    /* Abort if nothing written (I/O Err.) */
                    }
                }
#endif
            }
        }
    msg ("Intermediate file created.");
    
    if (tmpxs != NULL_PXS)
        tc_free (tmpxs);
    
    if (tmpls != (LAYOUT_STRUCT *)0)
        tc_free (tmpls);
    
    if (tmptinfo != (TOP_INFO *)0)
        tc_free (tmptinfo);
    
    if (fp != (FILE *)0)
      {
	status = ho_lock (fp, OFF);
        fclose (fp);
      }
    
    return SUCCESS;
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
    PRIVATE to <xs_obj_dump.c>

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
#if DEBUG_DUMP == TRUE
    printf ("%s\n", str);
#endif
    return;
    }
/* END:     */

/* do not compile the documentation */
#if 0

/***********************************************************************
* Document:
*
*               DETAILS OF THE DUMPING FORMAT & PROCEDURE
*
************************************************************************
*
*  +++ Structure pointer de-referencing/value casting +++
*
*  Whenever the main structure that is being archived contains a
*  pointer to a sub-structure (which of course must be
*  saved as well) the following method is used:
*
*  (note: ALL pointers, regardless of type, are 4 bytes wide.)
*
*   a) take the sizeof the sub-structure and cast to unsigned INT.
*   b) store the unsigned-size into the sub-structure pointer.
*   c) save the main structure.
*   d) save the sub-structure....Same # of bytes you placed in pointer.
*
*  This operation occurs frequently in this program.
*
*
*  +++++++ Saving Linked lists +++++++
*
*  When the main structure being saved contains a LISTHEAD to a
*  linked list, the following action is taken:
*
*   a) Take the address of the LISTHEAD and place it in an INT pointer.
*   b) Store the # of list elements in the LISTHEAD via the pointer.
*   c) Save the main structure.
*   d) Immediatly following the main structure, store each element
*      from the list.  Note: First store the Size in bytes of the
*      list element, then the element itself.   Repeat for each element.
*
*
*  ++++++ Saving B-Trees ++++++
*
*  Btrees are handled similarly:  A TREEHEAD is actually a BYTE pointer.
*  therefore, like before, we store the # of tnodes (cast to unsigned INT)
*  in place of the BYTE *.
*
*
*  +++++++ Notes on Data Types used ++++++++
*
*  Whenever a 'byte count' is written to the file, (always just in front of
*  the data record it represents), it is cast to USHORT (unsigned short)
*  which is 16 bits wide.  This means that if you try to store records
*  longer than 65535 bytes, you will start getting bogus results.  This
*  should not be a problem, but keep it in mind if you are maintaining
*  this code.
*
*
*  +++++ Overview of Archival Procedure ++++++
*
*  Saving the XS_STRUCT to a disk file is done in several distinct steps.
*   1)  A copy is made of the entire structure & contents.  This is
*        so that we may manipulate/re-cast or de-reference any pointers
*        within the structure without having the changes effect the
*        caller.
*
*   2)  All linked list, & btree headers are converted, as described
*        above, into a count of the # of elements they contain.
*
*   3)  The size of the structure is written (as USHORT) to the file.
*
*   4)  The size is immediatly followed by the structure itself.
*
*   5)  In the order that they occur within the structure, the various
*       linked lists/btrees are written to the file.  Actually this
*       involves several steps:
*
*         5a) Loop through the list/tree and for each element:
*         5b) Store the size (# bytes) of this element.
*         5c) Store the element itself.
*
*   6)  After all lists/trees are resolved, attention is turned to any
*       pointers-to-structures that exist.  For each sub-struct. pointer:
*
*         6a) Store the size of the sub-structure, as a 4 byte unsigned INT,
*             into the pointer itself.
*         6b) Write the size (as USHORT) of the sub-structure to the file.
*         6c) Write out the sub-structure itself.
*
*
*   7)  Steps 1-6 are repeated as the main structure is archived.  The
*       operations occurr in a nested fashion.  All sub-members are
*       resolved, any sub-members THEY may have are resolved etc. etc.
*       before moving on to the next sub-member in the main struct.
*
*       (Yes, my explanation is confusing......Do not adjust your set.)
*
* *** Caution: ****
*   Some convention, vague or not, must be adhered to when determining
*   the order in which to dump various portions of the XS_STRUCT.  This
*   is obviously because the program that reads and re-dresses the struct
*   from the file, must also adhere to this convention, except in the
*   opposite order.
*
* Here is some important info that is repeated in several files
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
*
* Julian Carlisle    June  30, 1987
*************************************************************************/

#endif
