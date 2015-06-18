/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_TOP.C*/
/* *3    17-AUG-1990 22:33:04 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-SEP-1989 16:22:46 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:41:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_TOP.C*/
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
#include "esi_xs_defs.h"

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    A module of routines of Cross Section. This module includes
    all routines which perform function on database. 

    This module will access one table in data base, 
    <AREA>.CROSS_SECTION_LAYOUT
    Adds rows of records of VERB equals to LOGPLOT TOPS to the table 
    named <PROJECT>.CROSS_SECTION_LAYOUTS.
        
Prototype:
    publicdef INT xsz_add_top(XS_STRUCT *pxs, CHAR layout_name[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) A pointer to requested cross section structure.
    (I) layout_name -(CHAR []) The specified layout name.
 
Return Value/Status:
    SUCCESS - Successful completion.
    SQL ERROR CODES
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_add_top (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_top (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    static CHAR *verb[1] = 
        {
        "LOGPLOT TOPS"
        };
    CHAR name[26];                  /* the text of NAME column in table  */
    CHAR addStr[512];               /* the added string.             */
    static CHAR *buffer;
    INT availLen = 0, bufferLen = 0;
    INT i = 0, totalTops = 0;
    INT write_out = TRUE;           /* write out the buffer right away.       */
    INT not_write_yet = FALSE;      /* not write yet until longer than maximum*/
    LISTHEAD *pList;
    TOP_INFO *top_info;
    /* ************************************************************************* */
    pList = &(pxs->drawing.top_table);
    
    /* inquire the total number of       */
    /* existing traces.                  */
    
    totalTops = llcount (pList);
    if (totalTops <= 0)
        return SUCCESS;
    
    for (i = 0, llfirst (pList, &top_info); i < totalTops;
         llnext (pList, &top_info), i++)
        {
        /* put the NAME.                    */
        
        sprintf (addStr, "NAME = %s", top_info->top);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the COLOR.                   */
        
        status = lu_index_to_text ("COLOR", name, top_info->line.color);
        if (status)
            return status;
        sprintf (addStr, "COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the LINE STYLE.             */
        
        status = lu_index_to_text ("LINE_STYLE", name, top_info->line.style);
        if (status)
            return status;
        sprintf (addStr, "LINE STYLE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the visibility.               */
        if (top_info->visible)
            {
            strcpy (addStr, "VISIBLE");
            xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                              XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                              &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
            }
        /* put the LINE THICKNESS.           */
        status = lu_index_to_text ("THICKNESS", name, (INT)top_info->line.width);
        if (status)
            return status;
        sprintf (addStr, "LINE THICKNESS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, write_out);
        
        }
    return status;
    
    }
/* END:     */
