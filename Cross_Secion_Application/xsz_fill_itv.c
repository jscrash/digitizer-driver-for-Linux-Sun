/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_ITV.C*/
/* *6    27-MAR-1991 13:28:48 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *5    17-AUG-1990 22:34:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     2-NOV-1989 12:51:33 GILLESPIE "(SPR 31) Change include file organization"*/
/* *3    11-AUG-1989 17:03:04 JULIAN "(SPR -1) parmStr maybe used before set, function xsz_fill_linterval has return(e) and*/
/*return;"*/
/* *2    27-JUL-1989 19:28:28 JULIAN "(SPR -1) cleaned up casting errors"*/
/* *1    19-JUN-1989 13:42:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_FILL_ITV.C*/
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

#include "esi_xs_parse.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To retrieve a layout information structure from an oracle databse
    and make it to be parser language structure; To fill the data in the 
    LOG_INTERVAL structure.

Prototype:
    publicdef INT xsz_fill_linterval(MD_KEY_STRUCT *keyStr, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) keystr      -(MD_KEY_STRUCT *) The pointer to the MD_KEY_STRUCT.
    (O) pxs         -(XS_STRUCT *) The pointer to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_fill_linterval (MD_KEY_STRUCT *keyStr, XS_STRUCT *pxs)
#else
publicdef INT xsz_fill_linterval (keyStr, pxs)
MD_KEY_STRUCT *keyStr;
XS_STRUCT *pxs;
#endif
    {
    LOG_INTERVAL *intervalPtr;
    MD_PARM_STRUCT *parmStr = (MD_PARM_STRUCT *)0;
    INT status = SUCCESS;
    CHAR units[30];

    /* ************************************************************************* */
    /* get the THINNING info.        */
    
    if (ARE_SAME (keyStr->keywrd, "NONE"))
        {
        pxs->plo->data_thinning = NO_THINNING;
        return status;
        }
    if (ARE_SAME (keyStr->keywrd, "DECIMATE"))
        {
        pxs->plo->data_thinning = DECIMATION;
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (pxs->plo->decimate), &status);
        return status;
        }
    if (ARE_SAME (keyStr->keywrd, "SLOPE"))
        {
        pxs->plo->data_thinning = SLOPE_CHANGE;
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (pxs->plo->delta_slope),
                       &status);
        return status;
        }
    intervalPtr = &(pxs->plo->interval);
    /* get the SEL_TOTAL_INTERVAL    */
    
    if (ARE_SAME (keyStr->keywrd, "ALL"))
        {
        intervalPtr->method = SEL_TOTAL_INTERVAL;
        return status;
        }
    /* get the ZONE NAME            */
    
    if (ARE_SAME (keyStr->keywrd, "ZONE NAME"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)intervalPtr->zone_name, &status);
        intervalPtr->method = SEL_BY_ZONE;
        return status;
        }
    /* get the TOP HORIZON.        */
    
    if (ARE_SAME (keyStr->keywrd, "TOP HORIZON"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)intervalPtr->top_select_phrase,
                       &status);
        intervalPtr->method = SEL_BY_TOP;
        return status;
        }
    /* get the BASE HORIZON        */
    
    if (ARE_SAME (keyStr->keywrd, "BASE HORIZON"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)intervalPtr->base_select_phrase,
                       &status);
        intervalPtr->method = SEL_BY_TOP;
        return status;
        }
    /* get the OFFSET.              */
    
    if (ARE_SAME (keyStr->keywrd, "OFFSET"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (intervalPtr->top_offset),
                       &status);
        xsz_get_parms (keyStr, parmStr, 2, (VOIDPTR) & (intervalPtr->base_offset),
                       &status);
        intervalPtr->ztop_offset = intervalPtr->top_offset;
        intervalPtr->zbase_offset = intervalPtr->base_offset;
        return status;
        }
    /* get the MEASURED.              */
    
    if (ARE_SAME (keyStr->keywrd, "MEASURED"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (intervalPtr->top), &status);
        xsz_get_parms (keyStr, parmStr, 2, (VOIDPTR) & (intervalPtr->base), &status);
        intervalPtr->specify_as = MEASURED_DEPTH;
        intervalPtr->method = SEL_BY_DEPTH;
        return status;
        }
    /* get the TVD                   */
    
    if (ARE_SAME (keyStr->keywrd, "TVD"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (intervalPtr->top), &status);
        xsz_get_parms (keyStr, parmStr, 2, (VOIDPTR) & (intervalPtr->base), &status);
        intervalPtr->specify_as = TVD;
        intervalPtr->method = SEL_BY_DEPTH;
        return status;
        }
    /* get the SUBSEA_ELEV*/
    
    if (ARE_SAME (keyStr->keywrd, "SUBSEA_ELEV"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (intervalPtr->top), &status);
        xsz_get_parms (keyStr, parmStr, 2, (VOIDPTR) & (intervalPtr->base), &status);
        intervalPtr->specify_as = SUBSEA_ELEV;
        intervalPtr->method = SEL_BY_DEPTH;
        return status;
        }
    /* get the TIME.        */
    
    if (ARE_SAME (keyStr->keywrd, "TIME"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR) & (intervalPtr->top), &status);
        xsz_get_parms (keyStr, parmStr, 2, (VOIDPTR) & (intervalPtr->base), &status);
        intervalPtr->specify_as = TIME;
        intervalPtr->method = SEL_BY_DEPTH;
        return status;
        }

    /* get the INTERVAL DISPLAY UNITS.        */
    if (ARE_SAME (keyStr->keywrd, "DISPLAY UNITS"))
        {
        xsz_get_parms (keyStr, parmStr, 1, (VOIDPTR)units, &status);
	if (strcmp(units, "METERS") == 0)
	  {
	    pxs->depth_interval_scale_units = XS_CENTIMETER_UNITS;
	  }
	else if (strcmp(units, "FEET") == 0)
	  {
	    pxs->depth_interval_scale_units = XS_INCH_UNITS;
	  }
	/* else time units */
        return status;
        }

    return status;
    
    }
/* END:     */

