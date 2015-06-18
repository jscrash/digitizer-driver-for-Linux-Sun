/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DIM_WINDOW.C*/
/*  *2    17-AUG-1990 22:04:03 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:10:52 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DIM_WINDOW.C*/
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

#include    "esi_gl_defs.h"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

#define IDENT "mp_dim_window"

/* Function Description -----------------------------------------------------
Description:
    Routine to redraw a window outline in white to indicate window
    is no longer active. 

Prototype:
    publicdef INT mp_dim_window(CHAR *map_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) map_name    -(CHAR *) The global workspace name of the map.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_dim_window (CHAR *map_name)
#else
publicdef INT mp_dim_window (map_name)
CHAR *map_name;
#endif
    {
    
    /*  This routine has been stubbed until a way to process multi-priority   */
    /*  segments is available                                                 */
    
    return SUCCESS;
    }
/* END:     */
