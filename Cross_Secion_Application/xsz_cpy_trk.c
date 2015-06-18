/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_CPY_TRK.C*/
/* *2    17-AUG-1990 22:33:46 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:42:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_CPY_TRK.C*/
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

#include "esi_xs_defs.h"

/* Function Description -----------------------------------------------------
Description:
    To copy track attributes from one track struct. to another. 

Prototype:
    publicdef INT xsz_copy_track_info(TRACK_STRUCT *p_source, TRACK_STRUCT *p_copy );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_source        -(TRACK_STRUCT *) Pointers to source track      
                        structures.  Must have been previously allocated.       
    p_copy          -(TRACK_STRUCT *) Pointer to target track    
                        structures.  Must have been previously allocated.      

Return Value/Status:
    Function returns status from xsz_copy_track_marks.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_copy_track_info (TRACK_STRUCT *p_source, TRACK_STRUCT *p_copy)
#else
publicdef INT xsz_copy_track_info (p_source, p_copy)
TRACK_STRUCT *p_source;
TRACK_STRUCT *p_copy;
#endif
    {
    INT status;
    
    p_copy->xorigin = p_source->xorigin;
    p_copy->width = p_source->width;
    p_copy->visible = p_source->visible;
    p_copy->line = p_source->line;
    
    p_copy->divisions.number = p_source->divisions.number;
    p_copy->divisions.start_value = p_source->divisions.start_value;
    p_copy->divisions.type = p_source->divisions.type;
    p_copy->divisions.line = p_source->divisions.line;
    
    status = xsz_copy_track_marks (p_source, p_copy);
    
    return status;
    }
/* END:     */
