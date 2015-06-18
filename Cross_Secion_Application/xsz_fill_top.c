/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_TOP.C*/
/* *6    17-AUG-1990 22:35:18 VINCE "(SPR 5644) Code Cleanup"*/
/* *5     2-NOV-1989 12:52:06 GILLESPIE "(SPR 31) Change include file organization"*/
/* *4    14-SEP-1989 16:23:30 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *3    11-AUG-1989 17:24:58 JULIAN "(SPR -1) parmStr may be used before set"*/
/* *2    27-JUL-1989 20:23:13 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_TOP.C*/
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

#include "esi_xs_parse.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    To fill the data in the TOP_INFO structure.

Prototype:
    publicdef INT xsz_fill_ltop(MD_KEY_STRUCT *keyStr, INT init, INT talloc, INT insert, 
        XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keystr      -(MD_KEY_STRUCT *) The pointer to the MD_KEY_STRUCT.
    (I) init        -(INT) The flag of initializing the TOP_TABLE linked list.
    (I) talloc      -(INT) The flag of allocating a new pointer of trace.
    (I) insert      -(INT) To insert the pointer to TOP_INFO into the
                        linked list.
    (O) pxs         -(XS_STRUCT *) The pointer to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_ltop (MD_KEY_STRUCT *keyStr, INT init, INT talloc, INT insert,
                             XS_STRUCT *pxs)
#else
publicdef INT xsz_fill_ltop (keyStr, init, talloc, insert, pxs)
MD_KEY_STRUCT *keyStr;
INT init;
INT talloc;
INT insert;
XS_STRUCT *pxs;
#endif
    {
    static TOP_INFO *top_info;
    TOP_INFO *temp_top_info;
    LP_TRACE_STRUCT *tempPtr;
    MD_PARM_STRUCT *parmStr = (MD_PARM_STRUCT *)0;
    INT i;
    INT status = SUCCESS;
    /*-------------------------------------*/
    /* if there are some tops in the top_  */
    /* table, should not re-initialize     */
    /* the TOP_TABLE, otherwise initialize */
    /* it.                                 */
    /*-------------------------------------*/
    INT init_top_table = 0;
    
    INT old_top = FALSE;
    INT total_tops = 0;
    INT ithick = 0;
    
    /* If INSERT is TRUE, inserts the     */
    /* previous pointer into the linked   */
    /* list. Then returns.            */
    
    if (insert)
        {
        /* check the existance of requested   */
        /* top.                   */
        
        total_tops = llcount (&(pxs->drawing.top_table));
        old_top = FALSE;
        if (total_tops > 0)
            {
            for (i = 0, llfirst (&(pxs->drawing.top_table), &temp_top_info);
                 i < total_tops AND (!old_top); i++)
                {
                if (ARE_SAME (temp_top_info->top, top_info->top))
                    old_top = TRUE;
                else
                    llnext (&(pxs->drawing.top_table), &temp_top_info);
                }
            }
        /* if it does nont exist, initi-  */
        /* alize the UWI B-tree, then     */
        /* appends to the TOP_TABLE.      */
        
        if (!old_top)
            {
            status = btinit (&(top_info->uwis), 0, sizeof(UWI), sizeof(TOP_STRUCT),
                             BT_KEY_IS_C_STRING | BT_TREE_SYSTEM);
            if (status < 0)
                return status;
            
            status = llappnd (&(pxs->drawing.top_table), &top_info);
            if (status < 0)
                return status;
            else
                return SUCCESS;
            }
        /* otherwise, replace the top */
        /* color, line style, thickness.*/
        else
            {
            temp_top_info->line = top_info->line;
            temp_top_info->seg_id = 0;
            tc_free ((VOIDPTR)top_info);
            return SUCCESS;
            }
        }
    /* allocates a new pointer.      */
    if (talloc)
        {
        
        top_info = (TOP_INFO *)tc_zalloc (sizeof(TOP_INFO) + 1);
        
        /* set the color, line_stype,        */
        /* thickness to be the default.      */
        
        lu_text_to_index ("COLOR", (INDEX *)&top_info->line.color, "NORMAL");
        lu_text_to_index ("LINE_STYLE", (INDEX *)&top_info->line.style, "SOLID");
        top_info->line.width = 1.0;
        }
    /* initialize the linked list.        */
    if (init)
        {
        /* if there are existing tops, do not */
        /* re-initialize the TOP_TABLE.       */
        
        init_top_table = llcount (&(pxs->drawing.top_table));
        if (init_top_table <= 0)
            {
            status = llinit (&(pxs->drawing.top_table), 0, sizeof(TOP_INFO *),
                             LL_LIST_SYSTEM);
            
            if (status < 0)
                return status;
            }
        }
    /* fill TOP NAME.             */
    
    if (ARE_SAME (keyStr->keywrd, "NAME"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)top_info->top, &status);
        return status;
        }
    /* fill TRACE COLOR           */
    
    if (ARE_SAME (keyStr->keywrd, "COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (top_info->line.color), &status);
        return status;
        }
    /* fill TOP LINE THICKNESS        */
    
    if (ARE_SAME (keyStr->keywrd, "LINE THICKNESS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & ithick, &status);
        top_info->line.width = (DOUBLE)ithick;
        return status;
        }
    /* fill TOP LINE STYLE.      */
    
    if (ARE_SAME (keyStr->keywrd, "LINE STYLE"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (top_info->line.style),
                       &status);
        return status;
        }
    /* fill TOP VISIBILITY.     */
    
    if (ARE_SAME (keyStr->keywrd, "VISIBLE"))
        {
        top_info->visible = TRUE;
        }
    return status;
    
    }
/* END:     */
