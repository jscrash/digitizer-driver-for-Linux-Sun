/* DEC/CMS REPLACEMENT HISTORY, Element XS_HDR_SIZE.C*/
/* *2    17-AUG-1990 22:40:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_HDR_SIZE.C*/
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

#ifndef ESI_XS_ERR_H

#include "esi_xs_err.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Compute the height of the log plot header block.      
    Use the width of the layout (computed from the        
    track widths), and allow for a fixed no. of chars.    
    The height will be a function of the character size   
    and the number of lines of text (from the link-list   
    of header items) plus the well symbol size. Char-     
    acters are assumed to be square.                      

Prototype:
    extern INT xs_hdr_size(LAYOUT_STRUCT   *p_layout);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_layout        -(LAYOUT_STRUCT *) Pointer to a logplot layout.  It must 
                        already have widths and link list of header text.                 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
extern INT xs_hdr_size (LAYOUT_STRUCT *p_layout)
#else
extern INT xs_hdr_size (p_layout)
LAYOUT_STRUCT *p_layout;
#endif
    {
    INT i, j;
    INT no_hdr_items;
    DOUBLE hdr_width, char_size;
    DOUBLE hdr_height, well_symbol_size;
    SHORT flag;
    PROGNAME ("xs_hdr_size");
    
    hdr_width = p_layout->log_hdr.width;    /* get width of log header     */
    char_size = hdr_width / LOG_HDR_MAX_CHARS;  /* compute character size      */
    well_symbol_size = char_size * 2.0; /* set symbol size=2*char size */
    
    no_hdr_items = p_layout->log_hdr.total_items;
    
    /*set hdr height according to no. text lines plus spaces (1/2 size) between   */
    
    hdr_height = no_hdr_items * char_size + well_symbol_size + (no_hdr_items + 2) *
                 (char_size * 0.5); /*spaces  */
    
    /*set header height in structs */
    
    p_layout->log_hdr.text_height = char_size;
    p_layout->log_hdr.well_symb.size = well_symbol_size;
    p_layout->log_hdr.height = hdr_height;
    
    return(SUCCESS);
    }
/* END:     */
