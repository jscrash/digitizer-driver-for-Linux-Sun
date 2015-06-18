/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_XS.C*/
/* *4    27-MAR-1991 13:28:26 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *3    17-AUG-1990 22:33:26 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-SEP-1989 16:23:15 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:41:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_XS.C*/
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
    named <PROJECT>.CROSS_SECTION_LAYOUT.
        
Prototype:
    publicdef INT xsz_add_cross_section(XS_STRUCT *pxs, CHAR layout_name[]);

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
publicdef INT xsz_add_cross_section (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_cross_section (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    static CHAR *verb[1] = 
        {
        "CROSS SECTION"
        };
    CHAR name[26];                  /* the text of NAME column in table  */
    CHAR addStr[512];               /* the added string.             */
    static CHAR *buffer;
    INT availLen = 0, bufferLen = 0;
    INT not_write_yet = FALSE, write_out = TRUE;
    /* ************************************************************************* */
    
    /* put the xs_type.          */
    
    status = lu_index_to_text ("CROSS_SECTION_TYPE", name, pxs->xs_type);
    if (status)
        return status;
    sprintf (addStr, "TYPE = %s", name);
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
    
    /* if the xs_type is equals to       */
    /* XS_STRATIGRAPHIC, add DATUM...    */
    
    if (pxs->xs_type EQUALS XS_STRATIGRAPHIC)
        {
        sprintf (addStr, "DATUM = %s", pxs->xs_datum_select);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        }
    /* put the xs_spacing.           */
    
    status = lu_index_to_text ("CROSS_SECTION_SPACING", name, pxs->xs_spacing);
    if (status)
        return status;
    sprintf (addStr, "WELL SPACING = %s", name);
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
    
    /* put the xs suppress vertical scale bar */

    sprintf (addStr, "SUPPRESS VBAR = %d", pxs->xs_suppress_vbar);
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
    
    /* if the xs_sacping is not CONST, */
    /* adds DISTANCE SCALE.            */
    
    if (pxs->xs_spacing IS_NOT_EQUAL_TO XS_CONSTANT_SPACED)
        {
        sprintf (addStr, "DISTANCE SCALE = %s", pxs->xs_scale_str);
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        }
    /* adds the TITLE...              */
    /* Wrap text in () to protect     */
    /* imbedded commas when parsed.   */
    
    sprintf (addStr, "TITLE = \( \(%s\), \(%s\), \(%s\), \(%s\), \(%s\), \(%s\) \)",
             pxs->title.line1, pxs->title.line2, pxs->title.line3, pxs->title.line4,
             pxs->title.line5, pxs->title.line6);
    
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);
    
    return status;
    
    }
/* END:     */
