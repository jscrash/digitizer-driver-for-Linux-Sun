/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_TRK.C*/
/* *6    27-MAR-1991 13:28:53 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *5    17-AUG-1990 22:35:32 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:52:16 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:25:29 JULIAN "(SPR -1) parmStr may be used before set"*/
/* *2    27-JUL-1989 20:23:30 JULIAN "(SPR -1) clean up castings... found some bugs too!"*/
/* *1    19-JUN-1989 13:42:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_TRK.C*/
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

/* Function Description -----------------------------------------------------
Description:
    To retrieve a layout information structure from an oracle database
    and make it to be parser language structure; To fill the data in the 
    TRACK_STRUCT structure.

Prototype:
    publicdef INT xsz_fill_ltrack(MD_KEY_STRUCT *keyStr, INT init, INT talloc, INT insert,
        XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keystr      -(MD_KEY_STRUCT *) The pointer to the MD_KEY_STRUCT.
    (I) init        -(INT) The flag of initializing the track linked list.
    (I) talloc      -(INT) the flag of allocating a new pointer.
    (I) insert      -(INT) To insert the pointer to TRACK_STRUCT to the
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
publicdef INT xsz_fill_ltrack (MD_KEY_STRUCT *keyStr, INT init, INT talloc, INT insert,
                               XS_STRUCT *pxs)
#else
publicdef INT xsz_fill_ltrack (keyStr, init, talloc, insert, pxs)
MD_KEY_STRUCT *keyStr;
INT init;
INT talloc;
INT insert;
XS_STRUCT *pxs;
#endif
    {
    LAYOUT_STRUCT *layout;
    static TRACK_STRUCT *newTrackPtr;
    TRACK_STRUCT *tempPtr;
    MD_PARM_STRUCT *parmStr = (MD_PARM_STRUCT *)0;
    INT status = SUCCESS;
    INT ithick = 0;
    CHAR units[30];
    
    layout = pxs->plo;
    
    /* If INSERT is TRUE, inserts the     */
    /* previous pointer to the linked list*/
    /* then returns.              */
    
    if (insert)
        {
        status = llinsa (&(layout->tracks), &newTrackPtr);
        if (status < 0)
            return status;
        else
            return SUCCESS;
        }
    /* initialize the linked list.        */
    if (init)
        {
        status = llinit (&(layout->tracks), 0, sizeof(TRACK_STRUCT *), LL_LIST_SYSTEM);
        if (status < 0)
            return status;
        }
    /* allocates a new pointer of track.  */
    if (talloc)
        {
        newTrackPtr = (TRACK_STRUCT *)tc_zalloc (sizeof(TRACK_STRUCT) + 1);
        newTrackPtr->visible = TRUE;
        }
    /* fill track NUMBER.             */
    
    if (ARE_SAME (keyStr->keywrd, "NUMBER"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->number), &status);
        return status;
        }
    /* fill track COLOR           */
    
    if (ARE_SAME (keyStr->keywrd, "COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->line.color), &status);
        return status;
        }
    if (ARE_SAME (keyStr->keywrd, "TYPE"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->type), &status);
        return status;
        }
    /* fill track BORDER THICKNESS        */
    
    if (ARE_SAME (keyStr->keywrd, "BORDER THICKNESS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & ithick, &status);
        newTrackPtr->line.width = (FLOAT)ithick;
        return status;
        }
    /* fill track INVISIBLE.              */
    
    if (ARE_SAME (keyStr->keywrd, "INVISIBLE"))
        {
        newTrackPtr->visible = FALSE;
        }
    /* fill track OFFSET              */
    
    if (ARE_SAME (keyStr->keywrd, "OFFSET"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->xorigin),
                       &status);
        return status;
        }
    /* fill track WIDTH       */
    
    if (ARE_SAME (keyStr->keywrd, "WIDTH"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->width), &status);
        return status;
        }
    /* fill track DIVISION COLOR          */
    
    if (ARE_SAME (keyStr->keywrd, "DIVISION COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->divisions.line.color),
                       &status);
        return status;
        }
    /* fill track DIVISION LINE STYLE    */
    
    if (ARE_SAME (keyStr->keywrd, "DIVISION LINE STYLE"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->divisions.line.style), &status);
        return status;
        }
    /* fill track DIVISION LINE THICKNESS*/
    
    if (ARE_SAME (keyStr->keywrd, "DIVISION LINE THICKNESS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & ithick, &status);
        newTrackPtr->divisions.line.width = (DOUBLE)ithick;
        return status;
        }
    /* fill track TYPE        */
    
    if (ARE_SAME (keyStr->keywrd, "DIVISION TYPE"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->divisions.type),
                       &status);
        return status;
        }
    /* fill track DIVISIONS       */
    
    if (ARE_SAME (keyStr->keywrd, "DIVISIONS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->divisions.number),
                       &status);
        return status;
        }
    /**************************************/
    /* fill the primary MARKS.            */
    /**************************************/
    /* fill the BOLD MARKS.               */
    
    if (ARE_SAME (keyStr->keywrd, "BOLD MARKS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->primary.type),
                       &status);
        return status;
        }
    /* fill the BOLD MARKS COLOR.         */
    
    if (ARE_SAME (keyStr->keywrd, "BOLD MARK COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->primary.line.color),
                       &status);
        return status;
        }
    /* fill the BOLD MARKS INTERVAL       */
    
    if (ARE_SAME (keyStr->keywrd, "BOLD MARK INTERVAL"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->primary.frequency), &status);
        return status;
        }
    /* fill the BOLD MARKS LINE STYLE.    */
    
    if (ARE_SAME (keyStr->keywrd, "BOLD MARK LINE STYLE"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->primary.line.style), &status);
        return status;
        }
    /**************************************/
    /* fill the secondary MARKS.          */
    /**************************************/
    /* fill the MEDIUM MARKS.             */
    
    if (ARE_SAME (keyStr->keywrd, "MEDIUM MARKS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->secondary.type),
                       &status);
        return status;
        }
    /* fill the MEDIUM MARKS COLOR.         */
    
    if (ARE_SAME (keyStr->keywrd, "MEDIUM MARK COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->secondary.line.color),
                       &status);
        return status;
        }
    /* fill the MEDIUM MARKS INTERVAL       */
    
    if (ARE_SAME (keyStr->keywrd, "MEDIUM MARK INTERVAL"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->secondary.frequency), &status);
        return status;
        }
    /* fill the MEDIUM MARKS LINE STYLE.    */
    
    if (ARE_SAME (keyStr->keywrd, "MEDIUM MARK LINE STYLE"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->secondary.line.style), &status);
        return status;
        }
    /**************************************/
    /* fill the tertiary MARKS.           */
    /**************************************/
    /* fill the FINE MARKS.               */
    
    if (ARE_SAME (keyStr->keywrd, "FINE MARKS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->tertiary.type),
                       &status);
        return status;
        }
    /* fill the FINE MARKS COLOR.         */
    
    if (ARE_SAME (keyStr->keywrd, "FINE MARK COLOR"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (newTrackPtr->tertiary.line.color),
                       &status);
        return status;
        }
    /* fill the FINE MARKS INTERVAL       */
    
    if (ARE_SAME (keyStr->keywrd, "FINE MARK INTERVAL"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->tertiary.frequency), &status);
        return status;
        }
    /* fill the FINE MARKS LINE STYLE.    */
    
    if (ARE_SAME (keyStr->keywrd, "FINE MARK LINE STYLE"))
        {
        xsz_get_parms (keyStr, parmStr, 1,
                       (VOIDPTR) & (newTrackPtr->tertiary.line.style), &status);
        return status;
        }

    /* fill track WIDTH SCALE UNITS      */
    if (ARE_SAME (keyStr->keywrd, "UNITS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)units, &status);
	if (strcmp(units, "CENTIMETERS") == 0)
	  {
	    pxs->track_width_scale_units = XS_CENTIMETER_UNITS;
	  }
	else if (strcmp(units, "INCHES") == 0)
	  {
	    pxs->track_width_scale_units = XS_INCH_UNITS;
	  }
        return status;
        }

    return status;
    
    }
/* END:     */
