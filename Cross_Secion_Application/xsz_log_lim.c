/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_LOG_LIM.C*/
/*  4    27-FEB-1992 09:23:49 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *3    17-AUG-1990 22:36:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JAN-1990 08:52:20 GILLESPIE "(SPR 6001) Changes for new wells struct stuff"*/
/* *1    19-JUN-1989 13:43:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_LOG_LIM.C*/
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
    To determine the minimum and maximum depths to draw the  
    the tracks for a log. This depends upon a number of      
    parameters such as depth selection type, availability    
    log data and the requested interval.                     

Prototype:
    publicdef INT xsz_find_log_limits(LAYOUT_STRUCT *plo, DOUBLE *d_start, DOUBLE *d_end );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) plo         -(LAYOUT_STRUCT *) Pointer to a well's layout.
    (O) d_start     -(DOUBLE *) Starting depth for all tracks.
    (O) d_end       -(DOUBLE *) Ending depth for all tracks.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_find_log_limits (LAYOUT_STRUCT *plo, DOUBLE *d_start, DOUBLE *d_end)
#else
publicdef INT xsz_find_log_limits (plo, d_start, d_end)
LAYOUT_STRUCT *plo;
DOUBLE *d_start;
DOUBLE *d_end;
#endif
    {
    WELL_HDR_1 *pwell1 = (WELL_HDR_1 *)plo->pwell->detail.ptr;
    switch (plo->interval.method)
        {
        /* User asked for Surface to TD.            */
    case SEL_TOTAL_INTERVAL:
        *d_start = 0.0;
        *d_end = pwell1->td.driller.md;
        break;
        
        /* User asked for constant interval or an  */
        /* interval somehow specified by tops.     */
    default:
        *d_start = plo->interval.ltdm_data_top;
        *d_end = plo->interval.ltdm_data_base;
        
        /* if upper top was missing, d_start = 0.0 */
        /* if lower top was missing, d_end   = 0.0 */
        if (*d_end < 0.0001)
            {
            *d_end = pwell1->td.driller.md; /* Try to fill in d_end. */
            }
        break;
        }
    /* If d_end < the base of data retrieved,  */
    /* ltdm_data_base = 0.0 & td was missing   */
    /* or td is less than base of log data.    */
    /* All we can do is use base of data retv'd*/
    
    if (*d_end < plo->interval.true_data_base)
        {
        *d_end = plo->interval.true_data_base;
        }
    /* If d_end still = 0.0                    */
    /* Then ltdm_data_base requested bottom of */
    /* data (0.0) and no data were retrieved.  */
    return SUCCESS;
    }
/* END:     */
