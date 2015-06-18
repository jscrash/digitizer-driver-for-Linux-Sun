/* DEC/CMS REPLACEMENT HISTORY, Element FI_CONFIG.C*/
/* *4    14-AUG-1990 13:23:27 VINCE "(SPR 5644) Header Standardization"*/
/* *3    18-DEC-1989 16:40:29 JULIAN "(SPR 5069) export facility mods"*/
/* *2    11-AUG-1989 09:31:48 JULIAN "(SPR -1) fixing constant in conditional context"*/
/* *1    19-JUN-1989 12:49:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_CONFIG.C*/
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
#include "esi_fi.h"

/* Function Description -----------------------------------------------------
Description:
    FINDER_CONFIG is the driver for the configuration step of Finder
    initialization. Three phases of configuration are done:

    (1) Analyst configuration. This phase performs configuration processing
         related to who the analyst is.

    (2) Terminal configuration. This phase performs configuration processing
         related to which terminal Finder was run from.

    (3) Workstation configuration. This phase perform configuration
         processing related to what graphics terminal is to be used for
         the session.

Prototype:
    publicdef INT finder_config();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    See below.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT finder_config () 
#else
publicdef INT finder_config ()
#endif
    {
    INT status;                     /* Completion status */
    
    /* ********************************************************************** */
    
    if ((status = finder_config_terminal ()) || (status = finder_config_analyst ()) ||
                  (status = finder_config_project (FALSE)) ||
                  (status = finder_config_workstation ()))
        ;
    
    return status;
    }
/* END:     */
