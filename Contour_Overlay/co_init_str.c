/* DEC/CMS REPLACEMENT HISTORY, Element CO_INIT_STR.C*/
/* *4    14-AUG-1990 11:43:12 VINCE "(SPR 5644) Header Standardization"*/
/* *3    25-APR-1990 08:40:52 JULIAN "(SPR 0) remove line_color option from co_overlay"*/
/* *2    18-SEP-1989 12:42:34 JULIAN "Gulf mods under SPR 100"*/
/* *1    19-JUN-1989 12:43:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_INIT_STR.C*/
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
#include "esi_co.h"

#ifndef ESI_OV_DEFS_H

#include "esi_ov_defs.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to initialize the structures needed for the
    "CONTOURS" overlay.

Prototype:
    publicdef INT co_init_structs(CONTOURS *contours, OV_GENERIC *generic);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contours overlay structure.
    (I) generic     -(OV_GENERIC *) Generic information structure.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_init_structs (CONTOURS *contours, OV_GENERIC *generic) 
#else
publicdef INT co_init_structs (contours, generic)
CONTOURS *contours;
OV_GENERIC *generic;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;                     /* RETURN STATUS */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * INIT CONTOURS OVERLAY STRUCTURE * */
    /* ----------------------------------- */
    
    contours->grid_interval = 0;
    contours->grid_minimum = 0;
    contours->grid_maximum = 0;
    contours->contour_minimum = 0;
    contours->contour_maximum = 0;
    contours->contour_interval = 0;
    contours->xyz_file = 0;
    contours->grid_file = 0;
    contours->contour_file = 0;
    contours->faulted = 0;
    contours->label_interval = 0;
    contours->bold_line_interval = 0;
    contours->bold_line_color = -1;
    contours->fine_line_color = -1;
    contours->suppress_smoothing = FALSE;
    contours->bounded_by_data = FALSE;
    contours->use_file_defaults = FALSE;
    contours->isopach = FALSE;
    
    /* Perspective initializations */
    
    contours->vertical_exaggeration = 1.0;
    contours->view_azimuth = 305.;
    contours->view_distance = 10.0;
    contours->view_inclination = 20.0;
    contours->view_type = 0;        /* MAP */
    
    /* * INIT GENERIC STRUCTURE * */
    /* -------------------------- */
    
    generic->unit_of_measure = 0;
    generic->select_list = 0;
    generic->select_by_phrase = 0;
    generic->best_sources = 0;
    generic->name = 0;
    generic->replace = FALSE;
    generic->delete = FALSE;
    generic->dim = FALSE;
    
    return status;
    }
/* END:     */
