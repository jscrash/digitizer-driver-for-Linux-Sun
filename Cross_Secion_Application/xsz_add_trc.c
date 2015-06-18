/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_TRC.C*/
/* *6    17-JUL-1991 16:21:43 JANTINA "(SPR 0) Fix for trace unit conversion"*/
/* *5    28-DEC-1990 10:07:50 MING "(SPR 6434) add checking on trace unit"*/
/* *4    26-OCT-1990 15:35:13 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *3    17-AUG-1990 22:33:10 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-SEP-1989 16:22:53 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:41:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_TRC.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
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

/* **************************************************************************       */
/*                                                                                  */
/*    MODULE: XS_PUT_LAYOUT                                                         */
/*                                                                                  */
/*    DESCRIPTION                                                                   */
/*         A module of routines of Cross Section. This module includes              */
/*         all routines which perform function on database.                         */
/*                                                                                  */
/*         This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS. */
/*                                                                                  */
/*    Author:   James Chang                                                         */
/*    ************************************************************************      */

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

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif


static CHAR *verb[1] = 
    {
    "LOGPLOT TRACE"
    };
static CHAR *buffer = (CHAR *)0; /* ??? When is this freed ???                  */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    A module of routines of Cross Section. This module includes                   */
/*    all routines which perform function on database.                              */
/*                                                                                  */
/*    This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS.      */
/*    Adds rows of records of VERB equals to LOGPLOT TRACE to the table             */
/*    named <PROJECT>.CROSS_SECTION_LAYOUTS.                                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xsz_add_trace(XS_STRUCT *pxs, CHAR layout_name[]);              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) pxs         -(XS_STRUCT *) A pointer to requested cross section structure.*/
/*    (I) layout_name -(CHAR []) The specified layout name.                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    SQL ERROR CODES                                                               */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xsz_add_trace (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_trace (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                             */
    CHAR name[26];                  /* the text of NAME column in table             */
    CHAR addStr[512];               /* the added string.                            */
    INT availLen = 0, bufferLen = 0;
    INT i = 0, totalTraces = 0;
    INT write_out = TRUE;           /* write out the buffer right away.             */
    BOOL not_write_yet = FALSE;     /* not write yet until longer than maximum      */
    LISTHEAD *pList;
    LP_TRACE_STRUCT *pTrace;
    UOM_STRUCTURE uom_data;
    /* *************************************************************************    */
    pList = &(pxs->plo->traces);
    
    /* inquire the total number of                                                  */
    /* existing traces.                                                             */
    
    totalTraces = llcount (pList);
    if (totalTraces <= 0)
        return SUCCESS;
    
    for (i = 0, llfirst (pList, &pTrace); i < totalTraces;
         llnext (pList, &pTrace), i++)
        {
        /* put the NAME.                                                            */
        
        sprintf (addStr, "NAME = %s", pTrace->name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the TRACK number.                                                    */
        
        sprintf (addStr, "TRACK = %d", pTrace->track_num);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the COLOR.                                                           */
        
        status = lu_index_to_text ("COLOR", name, (INDEX)pTrace->line.color);
        if (status)
            return status;
        sprintf (addStr, "COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the LINE STYLE.                                                      */
        
        status = lu_index_to_text ("LINE_STYLE", name, (INDEX)pTrace->line.style);
        if (status)
            return status;
        sprintf (addStr, "LINE STYLE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the LINE THICKNESS.                                                  */
        status = lu_index_to_text ("THICKNESS", name, (INDEX)pTrace->line.width);
        if (status)
            return status;
        sprintf (addStr, "LINE THICKNESS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the LOGUNITS                                                         */
        
        if (IS_STRING (pTrace->trace_units)) 
            {
            sprintf (addStr, "LOGUNITS = %s", pTrace->trace_units);
            xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                              XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                              &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
            }
        /* write out the left sale flag.                                            */
        
        switch (pTrace->left_flag)
            {
        case VALUE_SCALE:
            sprintf (addStr, "LEFT SCALE VALUE = %f", pTrace->left);
            break;
            
        case MIN_SCALE:
            strcpy (addStr, "LEFT SCALE MINIMUM");
            break;
            
        case MAX_SCALE:
            strcpy (addStr, "LEFT SCALE MAXIMUM");
            break;
            }
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* write out the right sale flag.                                           */
        
        switch (pTrace->right_flag)
            {
        case VALUE_SCALE:
            sprintf (addStr, "RIGHT SCALE VALUE = %f", pTrace->right);
            break;
            
        case MIN_SCALE:
            strcpy (addStr, "RIGHT SCALE MINIMUM");
            break;
            
        case MAX_SCALE:
            strcpy (addStr, "RIGHT SCALE MAXIMUM");
            break;
            }
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, write_out);
        
        }
    return status;
    
    }
/* END:                                                                             */
