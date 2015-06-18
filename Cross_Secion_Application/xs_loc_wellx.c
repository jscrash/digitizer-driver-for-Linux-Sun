/* DEC/CMS REPLACEMENT HISTORY, Element XS_LOC_WELLX.C*/
/* *2    17-AUG-1990 22:40:26 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_LOC_WELLX.C*/
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

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#define BORDER 2.0
static INT xsz_constant_spaced ();
static INT xsz_center_spaced ();
static INT xsz_edge_spaced ();

/* File Description ---------------------------------------------------------
Overview:
    Well positioning routines.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_struct_loc_wellx(XS_STRUCT *pxs );

Private_Functions:
    static INT xsz_constant_spaced(XS_STRUCT *pxs);
    
    static INT xsz_edge_spaced(XS_STRUCT *pxs);
    
    static INT xsz_center_spaced(XS_STRUCT *pxs);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    To position wells horizontally on a cross section.      

    The width of the logplots must already be computed.      
    (They are assumed to all be the same width.)             

Prototype:
    publicdef INT xs_struct_loc_wellx(XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to cross section structure (pxs).               

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_struct_loc_wellx (XS_STRUCT *pxs)
#else
publicdef INT xs_struct_loc_wellx (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_struct_loc_wellx");
    
    switch (pxs->xs_spacing)
        {
    case XS_CONSTANT_SPACED:
        status = xsz_constant_spaced (pxs);
        break;
    case XS_CENTER_SPACED:
        status = xsz_center_spaced (pxs);
        break;
    case XS_EDGE_SPACED:
        status = xsz_edge_spaced (pxs);
        break;
    default:
        status = FAIL;
        break;
        }
    pxs->drawing.xorigin = 0.0;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To position wells with a constant spacing between.      

Prototype:
    static INT xsz_constant_spaced(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to cross section structure (pxs). 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_loc_wellx.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xsz_constant_spaced (XS_STRUCT *pxs)
#else
static INT xsz_constant_spaced (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT n_wells;                    /* Number of wells.              */
    INT i;                          /* Loop counter.                 */
    LAYOUT_STRUCT *p_layout;        /* Current well logplot struct.  */
    DOUBLE xloc;                    /* x location accumulator.       */
    DOUBLE width;                   /* Logplot width in inches.      */
    DOUBLE gap;                     /* Space between wells in inches.*/
    
    PROGNAME ("xsz_constant_spaced");
    
    xloc = BORDER;                  /* Set initial location.         */
    
    llfirst (&pxs->drawing.log_plots, &p_layout);
    width = p_layout->width;        /* Set logplot width.            */
    
    gap = width;                    /* Set constant gap.             */
    
    if (gap > BORDER)               /* Don't let it get too big.     */
        gap = BORDER;
    
    n_wells = llcount (&pxs->drawing.log_plots);
    for (i = 0; i < n_wells; i++)   /* Loop thru well list.          */
        {
        p_layout->xorigin = xloc;   /* Set the well x location.      */
        xloc = xloc + width + gap;  /* Increment the accumulator.    */
        llnext (&pxs->drawing.log_plots, &p_layout);    /* Get the next well.     */
        }
    xloc = xloc - gap;              /* Remove the last gap.          */
                                    /* Vertical scale bar should not */
                                    /* be drawn in a no datum sectn. */
    
    pxs->drawing.sc_bar_xorigin = xloc + (BORDER / 2.0);    /* Position vertical scale bar.  */
    
    pxs->drawing.width = xloc + BORDER; /* Set drawing width.            */
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To position wells spaced with the distance between logs  
    scaled to the distance between the wells.                

Prototype:
    static INT xsz_edge_spaced(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to cross section structure (pxs).

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_loc_wellx.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xsz_edge_spaced (XS_STRUCT *pxs)
#else
static INT xsz_edge_spaced (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT n_wells;                    /* Number of wells.              */
    INT i;                          /* Loop counter.                 */
    LAYOUT_STRUCT *p_layout;        /* Current well logplot struct.  */
    LAYOUT_STRUCT *p_last;          /* Previous well logplot struct. */
    DOUBLE xloc;                    /* x location accumulator.       */
    DOUBLE width;                   /* Logplot width in inches.      */
    DOUBLE gap;                     /* Dist. between wells in inches.*/
    PROGNAME ("xsz_edge_spaced");
    
    xloc = BORDER;                  /* Initialize x location.        */
    
    llfirst (&pxs->drawing.log_plots, &p_layout);
    p_layout->xorigin = xloc;       /* Set the first well x location.*/
    
    width = p_layout->width;        /* Initialize the logplot width. */
    
    n_wells = llcount (&pxs->drawing.log_plots);
    for (i = 1; i < n_wells; i++)   /* Loop thru well list.          */
        {
        p_last = p_layout;          /* Set last well to current well.*/
        
        llnext (&pxs->drawing.log_plots, &p_layout);    /* Get current well. */
        
        status = xsz_well_distance (pxs, p_last, p_layout, &gap);
        if (status < SUCCESS)
            {
            return status;
            }
        xloc = xloc + width + gap;  /* Compute loc'n of current well.*/
        p_layout->xorigin = xloc;   /* Set current well x location.  */
        }
    xloc = xloc + width;            /* Add logplot width to x loc'n. */
    
    pxs->drawing.sc_bar_xorigin = xloc + (BORDER * 1.02);   /* Position vertical scale bar.  */
    
    pxs->drawing.width = xloc + (BORDER * 2.0); /* Set drawing width.            */
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To position wells spaced with the distance from center  
    to center of the logs scaled to the distance between    
    the wells.                                               

Prototype:
    static INT xsz_center_spaced(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to cross section structure (pxs).

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xsz_center_spaced (XS_STRUCT *pxs)
#else
static INT xsz_center_spaced (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT n_wells;                    /* Number of wells.              */
    INT n_trks;                     /* Number of tracks.             */
    INT i;                          /* Well Loop counter.            */
    INT j;                          /* Track Loop counter.           */
    LAYOUT_STRUCT *p_layout;        /* Current well logplot struct.  */
    LAYOUT_STRUCT *p_last;          /* Previous well logplot struct. */
    TRACK_STRUCT *p_track;          /* Logplot track structure.      */
    DOUBLE xloc;                    /* x location accumulator.       */
    DOUBLE width;                   /* Logplot width in inches.      */
    DOUBLE width_plus;              /* Width + 10%                   */
    DOUBLE gap;                     /* Dist between centers in inches*/
    DOUBLE squeeze;                 /* Squeeze factor for close wells*/
    
    PROGNAME ("xsz_center_spaced");
    
    xloc = BORDER;                  /* Initialize x location.        */
    
    squeeze = 1.0;                  /* Initialize the squeeze factor.*/
    
    llfirst (&pxs->drawing.log_plots, &p_layout);
    p_layout->xorigin = xloc;       /* Set the first well x location.*/
    
    /*-------------------------------*/
    /* Initialize the logplot width. */
    /* Width for all wells should be */
    /* the same.                     */
    /*-------------------------------*/
    width = p_layout->width;
    
    width_plus = 1.1 * width;       /* Initialize width_plus.        */
                                    /* Loop thru well list.          */
    
    n_wells = llcount (&pxs->drawing.log_plots);
    for (i = 1; i < n_wells; i++)
        {
        p_last = p_layout;          /* Set last well to current well.*/
        
        llnext (&pxs->drawing.log_plots, &p_layout);    /* Get current well. */
        
        status = xsz_well_distance (pxs, p_last, p_layout, &gap);
        if (status < SUCCESS)
            {
            return status;
            }
        xloc = xloc + gap;          /* Compute loc'n of current well.*/
        p_layout->xorigin = xloc;   /* Set current well x location.  */
                                    /* If the wells overlap, compute */
                                    /* a squeeze factor, keeping the */
                                    /* greatest squeeze factor.      */
        
        if (p_last->xorigin + width_plus > xloc)
            squeeze = MIN (squeeze, gap / width_plus);
        }
    if (squeeze < 1.0)              /* If there were any overlaps,   */
        {                           /* Loop thru the wells again to  */
        width = width * squeeze;    /* correct the logplot widths.   */
        
        llfirst (&pxs->drawing.log_plots, &p_layout);
        for (i = 0; i < n_wells; i++)
            {
            p_layout->log_hdr.width = width;
            p_layout->scale_block.width = width;
            p_layout->text_block.width = width;
            p_layout->width = width;
            
            /* Loop thru tracks resetting    */
            /* x origin and width.           */
            
            n_trks = llcount (&p_layout->tracks);
            llfirst (&p_layout->tracks, &p_track);
            for (j = 0; j < n_trks; j++)
                {
                p_track->xorigin = p_track->xorigin * squeeze;
                p_track->width = p_track->width * squeeze;
                
                llnext (&p_layout->tracks, &p_track);
                }
            /* Get next well for loop.       */
            llnext (&pxs->drawing.log_plots, &p_layout);
            
            }
        /* (end of well loop)            */
        }
    /* (end of squeeze logic)        */
    
    xloc = xloc + width;            /* Add logplot width to x loc'n. */
    
    pxs->drawing.sc_bar_xorigin = xloc + (BORDER * 1.02);   /* Position vertical scale bar.  */
    
    pxs->drawing.width = xloc + (BORDER * 2.0); /* Set drawing width.            */
    
    return SUCCESS;
    }
/* END:     */
