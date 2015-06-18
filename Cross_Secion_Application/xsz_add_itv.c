/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_ITV.C*/
/* *3    27-MAR-1991 13:28:15 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *2    17-AUG-1990 22:32:47 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:41:41 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ADD_ITV.C*/
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

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    A module of routines of Cross Section. This module includes
    all routines which perform function on database. 

    This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS.
    Adds rows of records of VERB equals to LOGPLOT INTERVAL to the table 
    named <PROJECT>.XS_VERB_KEYWORDS.

Prototype:
    publicdef INT xsz_add_interval(XS_STRUCT *pxs, CHAR layout_name[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) A pointer to requested cross section structure.
    (I) layout_name -(CHAR []) The specified layout name.
 
Return Value/Status:
    SUCCESS - Successful completion.
    SQL ERROR CODES

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_add_interval (XS_STRUCT *pxs, CHAR layout_name[])
#else
publicdef INT xsz_add_interval (pxs, layout_name)
CHAR layout_name[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                        */
    static CHAR *verb[2] = 
        {
        "LOGPLOT INTERVAL",
        "LOGPLOT THINNING"
        };
    CHAR *name;                     /* the text of NAME column in table  */
    CHAR addStr[512];               /* the added string.             */
    static CHAR *buffer;
    INT availLen = 0, bufferLen = 0, i = 0;
    INT not_write_yet = FALSE, write_out = TRUE;
    
    /* ************************************************************************* */
    
    /* Adds interval. */
    switch (pxs->plo->interval.method)
        {
    case SEL_TOTAL_INTERVAL:
        strcpy (addStr, "ALL");
        break;
        
    case SEL_BY_DEPTH:
        switch (pxs->plo->interval.specify_as)
            {
        case MEASURED_DEPTH:
            sprintf (addStr, "MEASURED = \(%f, %f\)", pxs->plo->interval.top,
                     pxs->plo->interval.base);
            break;
        case TVD:
            sprintf (addStr, "TVD = \(%f, %f\)", pxs->plo->interval.top,
                     pxs->plo->interval.base);
            break;
        case SUBSEA_ELEV:
            sprintf (addStr, "SUBSEA = \(%f, %f\)", pxs->plo->interval.top,
                     pxs->plo->interval.base);
            break;
        case TIME:
            sprintf (addStr, "TIME = \(%f, %f\)", pxs->plo->interval.top,
                     pxs->plo->interval.base);
            break;
            }
        break;
    case SEL_BY_TOP:
        sprintf (addStr, "TOP HORIZON = %s, BASE HORIZON = %s, \
OFFSET = \( %f, %f\)", pxs->plo->interval.top_select_phrase,
                 pxs->plo->interval.base_select_phrase, pxs->plo->interval.top_offset,
                 pxs->plo->interval.base_offset);
        break;
        
    case SEL_BY_ZONE:
        sprintf (addStr, "ZONE NAME = %s, OFFSET = \( %f, %f\)",
                 pxs->plo->interval.zone_name, pxs->plo->interval.ztop_offset,
                 pxs->plo->interval.zbase_offset);
        break;
        }

    /* Adds interval display units */
    strcat(addStr,", DISPLAY UNITS = ");
    switch (pxs->depth_interval_scale_units)
      {
      case XS_INCH_UNITS:
	strcat(addStr,"FEET");
	break;
      case XS_CENTIMETER_UNITS:
      default:
	strcat(addStr,"METERS");
	break;
      }

    /* else time units */
    xsz_build_buffer (layout_name, verb[0], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);

    /* Adds decimation.         */
    
    switch (pxs->plo->data_thinning)
        {
    case NO_THINNING:
        strcpy (addStr, "NONE");
        break;
        
    case DECIMATION:
        sprintf (addStr, "DECIMATE = %d", pxs->plo->decimate);
        break;
    case SLOPE_CHANGE:
        sprintf (addStr, "SLOPE = %f", pxs->plo->delta_slope);
        break;
        }
    xsz_build_buffer (layout_name, verb[1], buffer, bufferLen, availLen,
                      XS_BUF_INC_LENGTH, addStr, strlen (addStr), &buffer, &bufferLen,
                      &availLen, XS_MAX_BUF_LENGTH, write_out);
    
    return status;
    }
/* END:     */
