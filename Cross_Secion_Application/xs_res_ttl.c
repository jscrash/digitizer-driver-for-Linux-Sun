/* DEC/CMS REPLACEMENT HISTORY, Element XS_RES_TTL.C*/
/* *2    17-AUG-1990 22:43:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RES_TTL.C*/
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
    To determine the title block origin and text size.    
    The title block is positioned at the lower right hand 
    side of the x-section. Its origin is therefore = to   
    the bottom right hand corner of the x-section.  The   
    Title block has 5 lines of text, the bottom line con- 
    taining 2 strings side by side, which require a text  
    size = 1/2 that of the top 4 lines. The height of the 
    text is set = 2 * text width, which is computed from  
    the blk width/ no. chars(64).                         

Prototype:
    publicdef INT xs_resolve_title(XS_STRUCT *p_xsec);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_xsec          -(XS_STRUCT *) Link list LISTHEAD struct. 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_resolve_title (XS_STRUCT *p_xsec)
#else
publicdef INT xs_resolve_title (p_xsec)
XS_STRUCT *p_xsec;
#endif
    {
    DOUBLE tb_edge_space;
    
    /* set title block origin      */
    
    p_xsec->title.xorigin = p_xsec->drawing.xorigin + p_xsec->drawing.width;
    p_xsec->title.yorigin = p_xsec->drawing.yorigin;
    
    p_xsec->title.width = TITLE_BLK_WIDTH;  /* set blk width, height       */
    p_xsec->title.height = TITLE_BLK_HEIGHT;
    
    tb_edge_space = 0.5;            /* set space from tb to text   */
    
    /* check if user units in centimetres and convert if so                   */
    /*
    if (p_xsec->xs_scale_user_units == XS_CENTIMETER_UNITS)
        {
        p_xsec->title.width = TITLE_BLK_WIDTH * 2.54;  
        p_xsec->title.height = TITLE_BLK_HEIGHT * 2.54;
        tb_edge_space = tb_edge_space * 2.54;
        }
    */
    /* compute text dimensions, allow 1/4 inch space to edge of block         */
    p_xsec->title.text_width = (p_xsec->title.width - tb_edge_space) / 64;
    p_xsec->title.text_height = p_xsec->title.text_width * 2.0;
    
    return SUCCESS;
    }
/* END:     */
