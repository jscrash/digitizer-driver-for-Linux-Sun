/* DEC/CMS REPLACEMENT HISTORY, Element XS_BLK_SIZE.C*/
/* *3    21-AUG-1991 14:20:19 JESSIE "(SPR 8917) fixing system crashes from user delete Track"*/
/* *2    17-AUG-1990 22:37:32 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_BLK_SIZE.C*/
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

#include "esi_gl_defs.h"
#include "esi_xs_defs.h"
#include "esi_xs_err.h"
#include "esi_ll.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Retrieve the widths and starting locations of traks   
    from the layout structure, calculate the positions of 
    the scale bars (in y) and the resulting size of       
    the scale block. The scale bars are positioned ac-    
    cording to available room on the current line. If     
    there is not enough room the scale is positioned on   
    the line above. The flag (pos_flag) allows the user   
    to specify if he wants to allow scales to butt up ag- 
    ainst each other on the same line ie. the end of one  
    is at the same x-position as the start of an adjacent 
    scale (to allow or not to allow for scale label over- 
    lap).  Note that the scale bar y positions are rel-   
    ative to the bottom of the scale block. Must have     
    previously called XS_HDR_SIZE.                        

Prototype:
    publicdef INT xs_scale_blk_size (LAYOUT_STRUCT *p_layout);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) p_layout    -(LAYOUT_STRUCT *) Pointer to a logplot (layout structure);
                        Set up structure pointers.    

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_scale_blk_size (LAYOUT_STRUCT *p_layout)
#else
publicdef INT xs_scale_blk_size (p_layout)
LAYOUT_STRUCT *p_layout;
#endif
    {
    TRACK_STRUCT *p_track;
    LP_TRACE_STRUCT *p_trace;
    DOUBLE xmin[30], xmax[30];      /* x boundaries of track line  */
    DOUBLE xmin_trk, xmax_trk;      /* x boundaries of indiv.track */
    DOUBLE y_line = 0.0;            /* current track row           */
    DOUBLE y_line_max;              /* y max for sc.blk height     */
    DOUBLE y_base = 0.0;            /* y pos. of lower text line   */
    DOUBLE char_height;
    INT i, j, k, pos_flag = 1;
    INT no_traces;
    INT y_counter, loop_flag;
    SHORT flag;
    BOOL display_mg = FALSE;
    PROGNAME ("xs_scale_blk_size");
    
    no_traces = llcount (&p_layout->traces);    /* get number of traces        */
    
    if (no_traces > 0)
        {
        /* get first trace structure   */
        
        flag = llfirst (&p_layout->traces, &p_trace);
        
        /*get track data for 1st trace */
        
        flag = xsz_find_track (p_layout, p_trace->track_num, &p_track);

	/*************************************************************/
	/* return status checking for finding matched track or not   */
	/*************************************************************/
        if (flag == SUCCESS )
	  {
          xmin[0] = p_track->xorigin;
          xmax[0] = xmin[0] + p_track->width;
        char_height = p_layout->log_hdr.text_height;
        
        /* set well symbol coordinates */
        
        p_layout->log_hdr.well_symb.x = (p_layout->width / 2.0)
        -p_layout->log_hdr.well_symb.size;
        p_layout->log_hdr.well_symb.y = char_height;
        
        y_line = char_height * 3.5; /* set y pos. for 1st scale line*/
        y_base = y_line;
        /* allowing for well symbol     */
        p_trace->scale_yorigin = y_line;
	  }
        
        /*  loop on traces, calculating scale bar positions for each trace            */
        
        for (i = 1; i < no_traces; ++i)
            {
            /* get next trace and ass. track*/
            
            llnext (&p_layout->traces, &p_trace);
            flag = xsz_find_track (p_layout, p_trace->track_num, &p_track);
            
            if (flag == SUCCESS)
              {
                xmin_trk = p_track->xorigin;
                xmax_trk = xmin_trk + p_track->width;
                
        y_counter = 1;
        y_line_max = 0;
                y_line = y_base;
                /* loop on y lines, chk for room*/
                
                for (k = 0, loop_flag = 0; k < y_counter && loop_flag == 0; k++)
                    {
                    loop_flag = 1;
                    
                    if (xmin_trk > xmax[k]) /*sc. bar fits to the right of*/
                        {
                        xmax[k] = xmax_trk; /*xmax on the current y-line  */
                        }
                    else if (xmax_trk < xmin[k])    /*sc. bar fits to the left of*/
                        {
                        xmin[k] = xmin_trk; /*xmin on the current y-line */
                        }
                    /*scb butts up to adjacent trk  */
                    else if (xmin_trk == xmax[k] && pos_flag == 1)
                        {
                        xmax[k] = xmax_trk;
                        }
                    else if (xmax_trk == xmin[k] && pos_flag == 1)
                        {
                        xmin[k] = xmin_trk;
                        }
                    else
                        {
                        loop_flag = 0;
                        y_line = y_line + char_height * 2.5;
                        }
                    }
                if (loop_flag == 0) /* no room on current  */
                    {               /* line so increment one line  */
                    xmin[y_counter] = xmin_trk; /* up and start over  */
                    xmax[y_counter] = xmax_trk;
                    y_counter = y_counter + 1;
                    }
                p_trace->scale_yorigin = y_line;    /* set sc.bar y pos. in struct */
                if (y_line > y_line_max)    /* chk if gt ymax              */
                    {
                    y_line_max = y_line;
                    }
              }
            }
        /*end of trace loop            */
        /*allow spc above to top scblk */
        y_line = y_line_max + char_height * 2.5;
        p_layout->scale_block.height = y_line;  /*set height into scale struct */
        }
    else
        {
        p_layout->scale_block.height = 0.5;
        }
    return(SUCCESS);
    }
/* END:     */
