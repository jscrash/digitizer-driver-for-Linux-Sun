/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_CPY_TRK_M.C*/
/* *2    17-AUG-1990 22:33:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:42:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_CPY_TRK_M.C*/
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

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    extern INT xsz_copy_track_marks(TRACK_STRUCT *p_source, TRACK_STRUCT *p_copy );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_source        -(TRACK_STRUCT *)   
    p_copy          -(TRACK_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xsz_copy_track_marks (TRACK_STRUCT *p_source, TRACK_STRUCT *p_copy)
#else
extern INT xsz_copy_track_marks (p_source, p_copy)
TRACK_STRUCT *p_source;
TRACK_STRUCT *p_copy;
#endif
    {
    PROGNAME ("xsz_copy_track_marks");
    
    hoblockmove (&p_source->primary, &p_copy->primary, sizeof(MARKS));
    hoblockmove (&p_source->secondary, &p_copy->secondary, sizeof(MARKS));
    hoblockmove (&p_source->tertiary, &p_copy->tertiary, sizeof(MARKS));
    
    return SUCCESS;
    }
/* END:     */
