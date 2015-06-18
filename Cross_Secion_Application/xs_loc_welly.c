/* DEC/CMS REPLACEMENT HISTORY, Element XS_LOC_WELLY.C*/
/* *4    17-AUG-1990 22:40:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     7-MAR-1990 18:13:56 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *2    26-JUN-1989 11:06:34 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:44:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_LOC_WELLY.C*/
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

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#define BORDER 1.0

/* Function Description -----------------------------------------------------
Description:
    To correctly position the layouts in the Y direction  
    on the cross section. The Y position of the layout    
    is computed by determining the deepest and shallowest 
    log data relative to the datum (if present). The lay- 
    outs containing the shallowest and deepest data are   
    then positioned at the top and bottom of the x-sec.   
    The bottom of the x-section is assumed to be at y=0.  
    Assuming the bottom left corner of the layout to be   
    to be the layout origin then the y-origin of the deep 
    -est layout is at y=0, and the top of the header blk  
    of the shallowest layout is at y=ymax. We assume that 
    the height of the text blocks are all equal. Requires 
    the log max,min depth intervals relative to datum.    

    Depth UOM conversions are done in this module.        

Prototype:
    publicdef INT xs_struct_loc_welly(XS_STRUCT *p_xsec);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_xsec          -(XS_STRUCT *) Pointer to x-section struct.

Return Value / Status :
    SUCCESS-Successful completion
    FAIL-Unsuccessful completion

Scope :
    PUBLIC

Limitations / Assumptions :
-----------------------------------------------------------------------------*/ 

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_struct_loc_welly (XS_STRUCT *p_xsec)
#else
publicdef INT xs_struct_loc_welly (p_xsec)
XS_STRUCT *p_xsec;
#endif
    {
    LAYOUT_STRUCT *p_layout;
    TRACK_STRUCT *p_trk;
    PROJECT_DEFAULT *proj;
    DOUBLE conv;
    DOUBLE max_text_height;
    DOUBLE upper_z_limit;
    DOUBLE lower_z_limit;
    DOUBLE z_scale;
    DOUBLE track_yorigin;
    INT num_layouts;
    INT num_trks;
    INT j;
    INT k;
    INT flag;
    
    /* Compute conversion factor from default */
    /* log sample units to requested units.   */
    
    flag = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *) & proj);
    flag = tu_unit_conversion (1.0, proj->unit.vertical, &conv,
                               p_xsec->depth_scale_map_units);
    
    /* Depth_scale is in default depth units  */
    /* per paper inch.                        */
    z_scale = p_xsec->depth_scale;
    
    /* Loop on layouts, find upper & lower    */
    /* limits of log data in default units.   */
    
    num_layouts = llcount (&p_xsec->drawing.log_plots);
    max_text_height = 0.0;
    
    if (p_xsec->xs_type == XS_NO_DATUM)
        {
        upper_z_limit = 1000000.0;
        lower_z_limit = -1000000.0;
        
        for (j = 0; j < num_layouts; j++)
            {
            flag = (j == 0) ? llfirst (&p_xsec->drawing.log_plots,
                                       &p_layout) : llnext (&p_xsec->drawing.log_plots,
                                                            &p_layout);
            
            max_text_height = MAX (max_text_height, p_layout->text_block.height);
            
            /* Find upper & lower track limits   */
            /* in measured depth, default units. */
            
            upper_z_limit = MIN (upper_z_limit, p_layout->interval.true_rel_top);
            lower_z_limit = MAX (lower_z_limit, p_layout->interval.true_rel_base);
            
            /* Convert true_data to display units*/
            /* for depth marks & annotation.     */
            /* (Feet to meters, etc.)            */
            
            p_layout->interval.display_top = p_layout->interval.track_top * conv;
            p_layout->interval.display_base = p_layout->interval.track_base * conv;
            }
        /* calculate x section height as limits of bottom of lowest log to top of     */
        /* highest log + height of log header, scale block and text block.            */
        
        p_xsec->drawing.height = (lower_z_limit - upper_z_limit) / z_scale +
                                 p_layout->log_hdr.height +
                                 p_layout->scale_block.height + max_text_height +
                                 (2.0 * BORDER);
        }
    else
        {
        upper_z_limit = -1000000.0;
        lower_z_limit = 1000000.0;
        
        for (j = 0; j < num_layouts; j++)
            {
            flag = (j == 0) ? llfirst (&p_xsec->drawing.log_plots,
                                       &p_layout) : llnext (&p_xsec->drawing.log_plots,
                                                            &p_layout);
            
            max_text_height = MAX (max_text_height, p_layout->text_block.height);
            
            /* Find upper & lower track limits   */
            /* in relative elev., default units. */
            
            upper_z_limit = MAX (upper_z_limit, p_layout->interval.true_rel_top);
            lower_z_limit = MIN (lower_z_limit, p_layout->interval.true_rel_base);
            
            /* Convert true_data to display units*/
            /* for depth marks & annotation.     */
            /* (Feet to meters, etc.)            */
            
            p_layout->interval.display_top = p_layout->interval.track_top * conv;
            p_layout->interval.display_base = p_layout->interval.track_base * conv;
            }
        /* calculate x section height as limits of bottom of lowest log to top of     */
        /* highest log + height of log header, scale block and text block.            */
        
        p_xsec->drawing.height = (upper_z_limit - lower_z_limit) / z_scale +
                                 p_layout->log_hdr.height +
                                 p_layout->scale_block.height + max_text_height +
                                 (2.0 * BORDER);
        }
    /* Set V scale bar limits.    */
    /* (Using requested units.)   */
    
    p_xsec->drawing.sc_upper_limit = upper_z_limit * conv;
    p_xsec->drawing.sc_lower_limit = lower_z_limit * conv;
    
    /* Loop to determine y-origins*/
    /* Assume bottom of display   */
    /* is at Y=0 + BORDER.        */
    
    p_xsec->drawing.sc_bar_yorigin = 9999999.9; /* Initialize for MIN fn.     */
    p_xsec->drawing.sc_bar_length = -999999.9;  /* Initialize for MAX fn.     */
    
    for (j = 0; j < num_layouts; j++)
        {                           /*  Get a logplot        */
        
        flag = (j == 0) ? llfirst (&p_xsec->drawing.log_plots,
                                   &p_layout) : llnext (&p_xsec->drawing.log_plots,
                                                        &p_layout);
        
        /*  Compute track y origin  */
        
        track_yorigin = BORDER + ABS (lower_z_limit - p_layout->interval.true_rel_base)
        / z_scale;
        
        /*  Subtract text height to */
        /*  compute logplot yorigin */
        
        p_layout->yorigin = track_yorigin - p_layout->text_block.height;
        
        /*  Set text block yorigin  */
        /*  to logplot yorigin.     */
        
        p_layout->text_block.yorigin = p_layout->yorigin;
        
        /* Loop & set track yorigins*/
        
        num_trks = llcount (&p_layout->tracks);
        for (k = 0; k < num_trks; k++)
            {
            flag = (k == 0) ? llfirst (&p_layout->tracks,
                                       &p_trk) : llnext (&p_layout->tracks, &p_trk);
            p_trk->yorigin = track_yorigin;
            }
        /* Set scale block y origin */
        /* at the top of the tracks */
        
        p_layout->scale_block.yorigin = track_yorigin + p_trk->length;
        
        /* Log header y origin is   */
        /* above scale block.       */
        
        p_layout->log_hdr.yorigin = p_layout->scale_block.yorigin +
                                    p_layout->scale_block.height;
        
        /* Find the length of the   */
        /* Cross Section scale bar. */
        
        p_xsec->drawing.sc_bar_length = MAX (p_xsec->drawing.sc_bar_length,
                                             p_layout->scale_block.yorigin - BORDER);
        
        /* Set y origin of scale    */
        /* bar to the lowest track  */
        /* extent.                  */
        
        p_xsec->drawing.sc_bar_yorigin = MIN (p_xsec->drawing.sc_bar_yorigin,
                                              p_layout->yorigin);
        }
    return(SUCCESS);
    }
/* END:     */
