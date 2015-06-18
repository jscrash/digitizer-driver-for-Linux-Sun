/* DEC/CMS REPLACEMENT HISTORY, Element XS_CRT_DRW.C*/
/*  5    27-FEB-1992 09:25:41 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *4    18-JUL-1991 17:04:39 JANTINA "(SPR 0) Reverse top and base if top > base"*/
/* *3    17-AUG-1990 22:37:57 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    17-JAN-1990 22:17:41 GILLESPIE "(SPR 6001) Change include file"*/
/* *1    19-JUN-1989 13:43:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_CRT_DRW.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_XS_MSG_H

#include "esi_xs_msg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Supervises the building of the drawing data linked list.

Prototype:
    publicdef INT xs_create_drawing(XS_STRUCT *pstruct);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pstruct     -(XS_STRUCT *) Pointer to xs_struct.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_create_drawing (XS_STRUCT *pstruct)
#else
publicdef INT xs_create_drawing (pstruct)
XS_STRUCT *pstruct;
#endif
    {
    PROGNAME ("xs_create_drawing");
    INT status = SUCCESS;
    INT i;
    CHAR **uwiList;
    CHAR buff[128];
    WELL_STRUCT *pWell;
    LAYOUT_STRUCT *data;
    DOUBLE tmp;
    
    /* error checking.                    */
    
    if (pstruct EQUALS (XS_STRUCT *)NULL OR pstruct->plo EQUALS (LAYOUT_STRUCT *)
        NULL OR pstruct->well_list EQUALS NULL)
        return XS_INVALID_POINTER;
    
    /* retrieve the list of UWIs.        */
    
    status = sl_make_tcp (&uwiList, pstruct->well_list, 1);
    if (status)
        return status;
    /* initialize the LOG_PLOTS linked list*/
    
    llinit (&(pstruct->drawing.log_plots), 0, sizeof(LAYOUT_STRUCT *), LL_LIST_SYSTEM);
    
    /* go througth all UWIs....         */
    
    for (i = 0; uwiList[i] IS_NOT_EQUAL_TO NULL; i++)
        {
        /* first, retrieves the WELL_STRUCT*/
        
        status = we_get_well_header (uwiList[i], WELL_HDR_1_DETAIL, &pWell);
        /* Changes made 12 May 88 by JGG */
        /* Well not fond for some reason - skip it for now, but we should
           alert the user that this well not found */
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            sprintf (buff, mg_message (XS_BAD_UWI), uwiList[i]);
            am_message (AM_STATUS, buff);
            continue;
            }
        /* second, allocate a LAYOUT_STRUCT,*/
        
        data = (LAYOUT_STRUCT *)tc_zalloc (sizeof(LAYOUT_STRUCT));
        
        /* Copy data from plo to this well. */
        
        status = xsz_copy_layout (pstruct->plo, data);
        data->pwell = pWell;
        
        /* inserts into the LOG_PLOTS.      */
        
        llinsa (&(pstruct->drawing.log_plots), &data);
        
        /* retrieve the interval range.      */
        
        xsz_determine_intvl (data);

/* Problems will result if top > base */
/* Should warn user of this but for now, just switch */

	if (data->interval.ltdm_data_top > data->interval.ltdm_data_base)
	{
	    tmp = data->interval.ltdm_data_top;
	    data->interval.ltdm_data_top = data->interval.ltdm_data_base;
	    data->interval.ltdm_data_base = tmp;
	}
        
        /* third, retrieves the header data.*/
        
        status = xs_retrieve_hdr (pstruct->plo, data);
        if (status)
            return status;
        
        /* fourth, retrieve creates the track*/
        
        status = xs_create_tracks (pstruct->plo, data);
        if (status)
            return status;
        
        /* fifth, retireve the trace.       */
        /* there is no returned error checking*/
        /* because if there is a error happen*/
        /* the requested trace would be NULL,*/
        /* but we still have to retrieve     */
        /* others.                           */
        
        xs_retrieve_traces (pstruct->plo, data);
        
        }
    am_message (AM_DIALOG, "");
    status = xs_rtr_tops_when_redraw (pstruct);
    return status;
    }
/* END:     */
