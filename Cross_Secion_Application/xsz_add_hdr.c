/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_HDR.C*/
/* *3    17-AUG-1990 22:32:41 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-SEP-1989 16:22:33 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:41:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_HDR.C*/
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

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    A module of routines of Cross Section. This module includes
    all routines which perform function on database. 

    This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS.
    Adds rows of records of VERB equals to LOGPLOT HEADER to the table 
    named <PROJECT>.CORSS_SECTION_LAYOUTS.
        
Prototype:
    publicdef INT xsz_add_header(XS_STRUCT *pxs, CHAR layout_name[]);

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
publicdef INT xsz_add_header (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_header (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    static CHAR *verb[2] = 
        {
        "LOGPLOT HEADER",
        "LOGPLOT WELL SYMBOL"
        };
    CHAR name[26];                  /* the text of NAME column in table  */
    CHAR addStr[512];               /* the added string.             */
    static CHAR *buffer;
    INT availLen = 0, bufferLen = 0, i = 0;
    INT not_write_yet = FALSE, write_out = TRUE;
    
    /* ************************************************************************* */
    
    /* put the thickness of border of     */
    /* header.                            */
    status = lu_index_to_text ("THICKNESS", name, (INT)pxs->plo->log_hdr.line.width);
    if (status != SUCCESS)
        return status;
    sprintf (addStr, "BORDER THICKNESS = %s", name);
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
    
    /* write out the items.              */
    for (i = 0; i < pxs->plo->log_hdr.total_items; i++)
        {
        if (i EQUALS 0)
            {
            status = lu_index_to_text ("LOGPLOT_HEADER_ITEM", name,
                                       pxs->plo->log_hdr.items[0]);
            
            sprintf (addStr, "ITEM = \(%s", name);
            }
        else
            {
            strcat (addStr, ", ");
            status = lu_index_to_text ("LOGPLOT_HEADER_ITEM", name,
                                       pxs->plo->log_hdr.items[i]);
            strcat (addStr, name);
            }
        if (i EQUALS pxs->plo->log_hdr.total_items - 1)
            {
            strcat (addStr, "\)");
            }
        }
    if (i IS_NOT_EQUAL_TO 0)
        {
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        }
    /* checking the visibility of border */
    
    if (!pxs->plo->log_hdr.visible_border)
        {
        strcpy (addStr, "NO BORDER");
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        }
    /* checking the visibility of header */
    
    if (!pxs->plo->log_hdr.visible)
        {
        strcpy (addStr, "NONE");
        xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        }
    /* put the angle of header.      */
    
    sprintf (addStr, "ANGLE = %f", pxs->plo->log_hdr.angle);
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
    
    /* put the color of header.          */
    
    status = lu_index_to_text ("COLOR", name, pxs->plo->log_hdr.line.color);
    if (status)
        return status;
    sprintf (addStr, "COLOR = %s", name);
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);
    
    /**************************************/
    /* write out LOGPLOT WELL SYMBOL.     */
    /**************************************/
    /* write the NONE.                    */
    
    if (!pxs->plo->log_hdr.well_symb.visible)
        {
        strcpy (addStr, "NONE");
        xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        
        }
    /* write out the COLOR.               */
    
    status = lu_index_to_text ("COLOR", name, pxs->plo->log_hdr.well_symb.color);
    if (status)
        return status;
    sprintf (addStr, "COLOR = %s", name);
    xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);
    
    return status;
    
    }
/* END:     */
