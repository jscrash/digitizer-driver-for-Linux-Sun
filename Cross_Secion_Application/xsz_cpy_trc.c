/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_CPY_TRC.C*/
/* *2    17-AUG-1990 22:33:42 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:42:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_CPY_TRC.C*/
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
    To selectively copy trace attributes from one trace      
    structure (usually the master layout) to another        
    (an individual well's trace structure).                 

Prototype:
    extern INT xsz_copy_trace_info(LP_TRACE_STRUCT *p_source, LP_TRACE_STRUCT *p_copy );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_source        -(LP_TRACE_STRUCT *) Pointer to the source LP_TRACE_STRUCT.
    p_copy          -(LP_TRACE_STRUCT *) Pointer to the target LP_TRACE_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xsz_copy_trace_info (LP_TRACE_STRUCT *p_source, LP_TRACE_STRUCT *p_copy)
#else
extern INT xsz_copy_trace_info (p_source, p_copy)
LP_TRACE_STRUCT *p_source;
LP_TRACE_STRUCT *p_copy;
#endif
    {
    p_copy->left = p_source->left;
    p_copy->right = p_source->right;
    p_copy->left_flag = p_source->left_flag;
    p_copy->right_flag = p_source->right_flag;
    p_copy->line = p_source->line;
    p_copy->visible = p_source->visible;
    p_copy->visible_scale_bar = p_source->visible_scale_bar;
    
    return SUCCESS;
    }
/* END:     */
