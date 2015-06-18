/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_TRK.C*/
/* *4    27-MAR-1991 13:28:20 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *3    17-AUG-1990 22:33:16 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-SEP-1989 16:23:08 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:41:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_TRK.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

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

    This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS.
    Adds rows of records of VERB equals to CROSS SECTION  to the table 
    named <PROJECT>.CROSS_SECTION_LAYOUTS.
        
Prototype:
    publicdef INT xsz_add_track(XS_STRUCT *pxs, CHAR layout_name[]);

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
publicdef INT xsz_add_track (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_track (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    static CHAR *verb[2] = 
        {
        "LOGPLOT TRACK",
	"LOGPLOT LAYOUT"
        };
    CHAR name[26];                  /* the text of NAME column in table  */
    CHAR addStr[512];               /* the added string.             */
    static CHAR *buffer;
    INT availLen = 0, bufferLen = 0;
    INT i = 0, totalTracks = 0;
    INT write_out = TRUE;           /* write out the buffer right away.       */
    INT not_write_yet = FALSE;      /* not write yet until longer than maximum*/
    LISTHEAD *pList;
    TRACK_STRUCT *pTrack;
    /* ************************************************************************* */
    pList = &(pxs->plo->tracks);
    
    /* inquire the total number of       */
    /* existing tracks.                  */
    
    totalTracks = llcount (pList);
    if (totalTracks <= 0)
        return SUCCESS;
    
    for (i = 0, llfirst (pList, &pTrack); i < totalTracks;
         llnext (pList, &pTrack), i++)
        {
        /* put the TYPE*/
        
        status = lu_index_to_text ("LOGPLOT_TRACK", name, pTrack->type);
        if (status)
            return status;
        sprintf (addStr, "TYPE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the TRACK number.            */
        
        sprintf (addStr, "NUMBER = %d", pTrack->number);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the COLOR.                   */
        
        status = lu_index_to_text ("COLOR", name, pTrack->line.color);
        if (status)
            return status;
        sprintf (addStr, "COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the line.widthTHICKNESS.                */
        status = lu_index_to_text ("THICKNESS", name, (INT)pTrack->line.width);
        if (status)
            return status;
        sprintf (addStr, "BORDER THICKNESS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the OFFSET(xorigin).          */
        
        sprintf (addStr, "OFFSET = %f", pTrack->xorigin);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the WIDTH.                   */
        
        sprintf (addStr, "WIDTH = %f", pTrack->width);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the divisions.              */
        
        sprintf (addStr, "DIVISIONS = %d", pTrack->divisions.number);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the DIVISION COLOR.        */
        
        status = lu_index_to_text ("COLOR", name, pTrack->divisions.line.color);
        if (status)
            return status;
        sprintf (addStr, "DIVISION COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the DIVISION LINE STYLE.     */
        
        status = lu_index_to_text ("LINE_STYLE", name, pTrack->divisions.line.style);
        if (status)
            return status;
        sprintf (addStr, "DIVISION LINE STYLE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the DIVISION LINE THICKNESS   */
        status = lu_index_to_text ("THICKNESS", name,
                                   (INT)pTrack->divisions.line.width);
        if (status)
            return status;
        sprintf (addStr, "DIVISION LINE THICKNESS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the DIVISION TYPE   */
        
        status = lu_index_to_text ("LOGPLOT_MARK", name, pTrack->divisions.type);
        if (status)
            return status;
        sprintf (addStr, "DIVISION TYPE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /********************************/
        /* put the BOLD(primary) MARKS. */
        /********************************/
        /* put the MARK COLOR.        */
        
        status = lu_index_to_text ("COLOR", name, pTrack->primary.line.color);
        if (status)
            return status;
        sprintf (addStr, "BOLD MARK COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARK STYLE.     */
        
        status = lu_index_to_text ("LINE_STYLE", name, pTrack->primary.line.style);
        if (status)
            return status;
        sprintf (addStr, "BOLD MARK LINE STYLE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARKS INTERVAL   */
        
        sprintf (addStr, "BOLD MARK INTERVAL = %f ", pTrack->primary.frequency);
        
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARK TYPE   */
        
        status = lu_index_to_text ("LOGPLOT_MARK", name, pTrack->primary.type);
        if (status)
            return status;
        sprintf (addStr, "BOLD MARKS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /**********************************/
        /* put the MEDIUM(secondary) MARKS*/
        /**********************************/
        /* put the MARK COLOR.        */
        
        status = lu_index_to_text ("COLOR", name, pTrack->secondary.line.color);
        if (status)
            return status;
        sprintf (addStr, "MEDIUM MARK COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARK STYLE.     */
        
        status = lu_index_to_text ("LINE_STYLE", name, pTrack->secondary.line.style);
        if (status)
            return status;
        sprintf (addStr, "MEDIUM MARK LINE STYLE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARKS INTERVAL   */
        
        sprintf (addStr, "MEDIUM MARK INTERVAL = %f ", pTrack->secondary.frequency);
        
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARK TYPE   */
        
        status = lu_index_to_text ("LOGPLOT_MARK", name, pTrack->secondary.type);
        if (status)
            return status;
        sprintf (addStr, "MEDIUM MARKS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /********************************/
        /* put the FINE(tertiary) MARKS.*/
        /********************************/
        /* put the MARK COLOR.        */
        
        status = lu_index_to_text ("COLOR", name, pTrack->tertiary.line.color);
        if (status)
            return status;
        sprintf (addStr, "FINE MARK COLOR = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARK STYLE.     */
        
        status = lu_index_to_text ("LINE_STYLE", name, pTrack->tertiary.line.style);
        if (status)
            return status;
        sprintf (addStr, "FINE MARK LINE STYLE = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARKS INTERVAL   */
        
        sprintf (addStr, "FINE MARK INTERVAL = %f ", pTrack->tertiary.frequency);
        
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        /* put the MARK TYPE   */
        
        status = lu_index_to_text ("LOGPLOT_MARK", name, pTrack->tertiary.type);
        if (status)
            return status;
        sprintf (addStr, "FINE MARKS = %s", name);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, write_out);
        
        }

	/* put the WIDTH SCALE UNITS */
	strcpy(addStr,"UNITS = ");
        switch (pxs->track_width_scale_units)
	  {
	  case XS_INCH_UNITS:
	    strcat(addStr,"INCHES");
	    break;
	  case XS_CENTIMETER_UNITS:
	  default:
	    strcat(addStr,"CENTIMETERS");
	    break;
	  }

        xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, write_out);

    return SUCCESS;
    }
/* END:     */





