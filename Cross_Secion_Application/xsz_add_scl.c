/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_SCL.C*/
/* *3    17-AUG-1990 22:32:59 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    14-SEP-1989 16:22:39 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:41:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_SCL.C*/
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
    Adds rows of records of VERB equals to LOGPLOT SCALE and 
    LOGPLOT SCALE BLOCK to the table named <PROJECT>.CROSS_SECTION_LAYOUTS.
        
Prototype:
    publicdef INT xsz_add_scales(XS_STRUCT *pxs, CHAR layout_name[]);

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
publicdef INT xsz_add_scales (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_scales (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    static CHAR *verb[2] = 
        {
        "LOGPLOT SCALE",
        "LOGPLOT SCALE BLOCK"
        };
    CHAR name[26];                  /* the text of NAME column in table  */
    CHAR addStr[512];               /* the added string.             */
    static CHAR *buffer;
    INT availLen = 0, bufferLen = 0;
    INT not_write_yet = FALSE, write_out = TRUE;
    /* ************************************************************************* */
    
    /* put the LOGPLOT SCALE             */
    
    switch (pxs->depth_display_as)
        {
    case MEASURED_DEPTH:
        sprintf (addStr, "MEASURED = %s", pxs->depth_scale_str);
        break;
        
    case TVD:
        sprintf (addStr, "TVD = %s", pxs->depth_scale_str);
        break;
        
    case TIME:
        sprintf (addStr, "TIME = %s", pxs->depth_scale_str);
        break;
        
    case SUBSEA_ELEV:
        sprintf (addStr, "SUBSEA_ELEV = %s", pxs->depth_scale_str);
        break;
        }
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);
    
    /* put the LOGPLOT SCALE             */
    
    /* checking the visibility.        */
    
    if (!pxs->plo->scale_block.visible)
        {
        strcpy (addStr, "NONE");
        xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                          XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer,
                          &bufferLen, &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
        }
    /* put the color.                    */
    
    status = lu_index_to_text ("COLOR", name, pxs->plo->scale_block.line.color);
    if (status)
        return status;
    sprintf (addStr, "COLOR = %s", name);
    xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, not_write_yet);
    
    /* put the thickness                 */
    status = lu_index_to_text ("THICKNESS", name,
                               (INT)pxs->plo->scale_block.line.width);
    if (status)
        return status;
    sprintf (addStr, "BORDER THICKNESS = %s", name);
    xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);
    return status;
    
    }
/* END:     */
