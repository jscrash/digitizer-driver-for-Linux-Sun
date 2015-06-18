/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_TRC.C*/
/* *6    26-OCT-1990 15:35:19 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *5    17-AUG-1990 22:35:25 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:52:11 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:25:13 JULIAN "(SPR -1) parmStr may be used before set"*/
/* *2    27-JUL-1989 20:23:21 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_TRC.C*/
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

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To retrieve a layout information structure from an oracle database            */
/*    and make it to be parser language structure; To fill the data in the          */
/*    LP_TRACE_STRUCT structure.                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xsz_fill_ltrace(MD_KEY_STRUCT *keyStr, INT init, INT talloc, INT insert,    */
/*        XS_STRUCT *pxs);                                                          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) keystr      -(MD_KEY_STRUCT *) The pointer to the MD_KEY_STRUCT.          */
/*    (I) init        -(INT) The flag of initializing the TRACE linked list.        */
/*    (I) talloc      -(INT) The flag of allocating a new pointer of trace.         */
/*    (I) insert      -(INT) To insert the pointer to TRACE_STRUCT into the         */
/*                        linked list.                                              */
/*    (O) pxs         -(XS_STRUCT *) The pointer to the XS_STRUCT.                  */
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
publicdef INT xsz_fill_ltrace (MD_KEY_STRUCT *keyStr, INT init, INT talloc, INT insert,
                               XS_STRUCT *pxs)
#else
publicdef INT xsz_fill_ltrace (keyStr, init, talloc, insert, pxs)
MD_KEY_STRUCT *keyStr;
INT init;
INT talloc;
INT insert;
XS_STRUCT *pxs;
#endif
    {
    LAYOUT_STRUCT *layout;
    static LP_TRACE_STRUCT *newTracePtr = (LP_TRACE_STRUCT *)0;
    MD_PARM_STRUCT *parmStr = (MD_PARM_STRUCT *)0;
    INT status = SUCCESS;
    INT ithick = 0;
    DOUBLE d_temp;                  /* a DOUBLE for returns from XSZ_GET_PARMS      */
    layout = pxs->plo;
    
    /* If INSERT is TRUE, inserts the                                               */
    /* previous pointer into the linked                                             */
    /* list. Then returns.                                                          */
    
    if (insert)
        {
        if (newTracePtr != (LP_TRACE_STRUCT *)0)
            {
            newTracePtr->visible = TRUE;
            status = llinsa (&(layout->traces), &newTracePtr);
            if (status < 0)
                return status;
            else
                return SUCCESS;
            }
        else
            {
            return FAIL;
            }
        }
    /* allocates a new pointer.                                                     */
    if (talloc)
        newTracePtr = (LP_TRACE_STRUCT *) tc_zalloc (sizeof(LP_TRACE_STRUCT) + 1);
    
    /* initialize the linked list.                                                  */
    if (init)
        {
        status = llinit (&(layout->traces), 0, sizeof(LP_TRACE_STRUCT *),
                         LL_LIST_SYSTEM);
        if (status < 0)
            return status;
        layout->last_trace_number = newTracePtr->number = 1;
        }
    else if (talloc)
        {
        newTracePtr->number = llcount (&(layout->traces));
        newTracePtr->number += 1;
        layout->last_trace_number = newTracePtr->number;
        }
    /* fill TRACE NAME.                                                             */
    
    if (ARE_SAME (keyStr->keywrd, "NAME"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)newTracePtr->name, &status);
        return status;
        }
    /* fill TRACK NUMBER.                                                           */
    
    if (ARE_SAME (keyStr->keywrd, "TRACK"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)&(newTracePtr->track_num),
                       &status);
        return status;
        }
    /* fill TRACE COLOR                                                             */
    
    if (ARE_SAME (keyStr->keywrd, "COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)&(newTracePtr->line.color),
                       &status);
        return status;
        }
    /* fill TRACE LINE THICKNESS                                                    */
    
    if (ARE_SAME (keyStr->keywrd, "LINE THICKNESS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)&ithick, &status);
        newTracePtr->line.width = (DOUBLE)ithick;
        return status;
        }
    /* fill TRACE LINE STYLE.                                                       */
    
    if (ARE_SAME (keyStr->keywrd, "LINE STYLE"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)&(newTracePtr->line.style),
                       &status);
        return status;
        }
    /* fill TRACE LEFT SCALE VALUE                                                  */
    
    if (ARE_SAME (keyStr->keywrd, "LEFT SCALE VALUE"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)&d_temp, &status);
        newTracePtr->left = (DOUBLE)d_temp;
        return status;
        }
    /* fill TRACE LEFT SCALE MINIMUM                                                */
    
    if (ARE_SAME (keyStr->keywrd, "LEFT SCALE MINIMUM"))
        {
        newTracePtr->left_flag = MIN_SCALE;
        return status;
        }
    /* fill TRACE LEFT SCALE MAXIMUM                                                */
    
    if (ARE_SAME (keyStr->keywrd, "LEFT SCALE MAXIMUM"))
        {
        newTracePtr->left_flag = MAX_SCALE;
        return status;
        }
    /* fill TRACE RIGHT SCALE VALUE                                                 */
    
    if (ARE_SAME (keyStr->keywrd, "RIGHT SCALE VALUE"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)&d_temp, &status);
        newTracePtr->right = (DOUBLE)d_temp;
        return status;
        }
    /* fill TRACE RIGHT SCALE MINIMUM                                               */
    
    if (ARE_SAME (keyStr->keywrd, "RIGHT SCALE MINIMUM"))
        {
        newTracePtr->right_flag = MIN_SCALE;
        return status;
        }
    /* fill TRACE RIGHT SCALE MAXIMUM                                               */
    
    if (ARE_SAME (keyStr->keywrd, "RIGHT SCALE MAXIMUM"))
        {
        newTracePtr->right_flag = MAX_SCALE;
        return status;
        }
    /* fill LOG UNITS.                                                              */
    
    if (ARE_SAME (keyStr->keywrd, "LOGUNITS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)newTracePtr->trace_units, &status);
        return status;
        }
    return status;
    
    }
/* END:                                                                             */
