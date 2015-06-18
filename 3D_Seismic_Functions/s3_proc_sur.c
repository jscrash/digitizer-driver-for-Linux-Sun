/* DEC/CMS REPLACEMENT HISTORY, Element S3_PROC_SUR.C*/
/* *3    17-AUG-1990 22:14:09 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JUN-1990 12:07:12 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *1    19-JUN-1989 13:23:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_PROC_SUR.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_PROC_SUR.C                               */
/* *1    19-JUN-1989 13:23:16 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element S3_PROC_SUR.C                               */
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

#include "esi_s3.h"

/* Function Description -----------------------------------------------------
Description:
    Creates the grid lines, draws the survey, posts the                 
    the horizons and faults, if necessary.                                            

Prototype:
    publicdef INT s3_process_survey(S3_STRUCT *ps3);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ps3             -(S3_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT s3_process_survey (S3_STRUCT *ps3)
#else
publicdef INT s3_process_survey (ps3)
S3_STRUCT *ps3;
#endif
    {
    INT status;
    
    /* get the grid (via calculations). line end-points and numbers are
       in the nlist */
    
    status = s3_calc_cdps_lines (&ps3->grid_3d, &ps3->cdp_nlist, &ps3->line_nlist);
    
    /* first process the line, cdp symbol and cdp value drawing                     */
    
    status = s3_draw_survey (ps3);
    
    /* next post the horizon data                                                   */
    /* THERE IS AN EXCLUSIVE-ON-LINES-DRAWN OPTION WHICH WILL CAUSE HORIZONS TO BE
   POSTED DURING S3_DRAW_SURVEY ABOVE. A SEPARATE DRAW-IT-WHERE-THERE-IS-DATA
   OPTION (OPPOSITE OF ABOVE OPTION) WILL REQUIRE A SEPARATE ENTITY TO DRAW IT
   HERE
*/
    /* this hasn't been written yet, so comment it out                              */
#if 0
    status = s3_post_all_horizon (ps3);
    
    if (status != SUCCESS)
        {
        ps3->seismic_generic.horizons_ok = FALSE;
        }
    else
        {
        ps3->seismic_generic.horizons_ok = TRUE;
        }
#endif
    
    /* fault posting is an undetermined point. horizons can be posted at faults
       but there is no documentation that the fault itself is labeled. that
       functionality would be done here */
    
    status = SUCCESS;
    return status;
    }
/* END:     */
