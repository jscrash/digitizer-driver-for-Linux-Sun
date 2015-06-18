/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_XY.C */
/* *4    17-AUG-1990 22:18:18 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:09:27 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:12:12 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:57:45 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_XY.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_se.h"

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#if USE_PROTOTYPES
static INT sez_sp_from_cdp_bounds (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
                                       SP *sp);
#else
static INT sez_sp_from_cdp_bounds ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT se_sp_from_xy (FULL_LINE_NAME *seis_line, COORD wx, COORD wy,
        SP *sp);

Private_Functions:
    static INT sez_sp_from_cdp_bounds (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
                                      SP *sp);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT se_sp_from_xy (FULL_LINE_NAME *seis_line, COORD wx, COORD wy,
        SP *sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) seis_line   -(FULL_LINE_NAME *) Structure composed of the line name and 
                        the survey name.
    (I) wx          -(COORD) -\
    (I) wy          -(COORD) Wx, wy x and y coord of a point.
    (O) sp          -(SP *) SP retrieve ( float SP not rounded ).

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_sp_from_xy (FULL_LINE_NAME *seis_line, COORD wx, COORD wy, SP *sp)
#else
publicdef INT se_sp_from_xy (seis_line, wx, wy, sp)
FULL_LINE_NAME *seis_line;
COORD wx, wy;
SP *sp;
#endif
    {
    INT status;
    CDP_FLOAT cdp_float;
    DOUBLE cdp_dist;
    COORD x_proj, y_proj;
    COORD x_end, y_end;
    CDP cdp_int, cdp_end;
    
    /* cdp_dist : inter cdp distance which can
       come from the trace files - if the distance
       is > 0 we take this distance as inter cdp
       distance if it is < 0 we take as inter_cdp
       distance the distance computed from the SP
       spacing we set it to negative in this case  */
    
    cdp_dist = -1.;
    
    /* look for the closest cdp on the line using  */
    /* the wx , wy coord                           */
    
    status = se_cdp_from_xy (seis_line, wx, wy, cdp_dist, &cdp_int, &cdp_float,
                             &x_proj, &y_proj, &cdp_end, &x_end, &y_end);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* call a routine returning the SP
       corresponding to the CDP if we are out of
       the SP bounds this routine returns the
       closest end                                 */
    
    status = sez_sp_from_cdp_bounds (seis_line, cdp_float, sp);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    For the line this function gives
    the sp corresponding to the given cdp.

Prototype:
    static INT sez_sp_from_cdp_bounds (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
        SP *sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    cdp             -(CDP_FLOAT)
    sp              -(SP *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <se_sp_xy.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sez_sp_from_cdp_bounds (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
                                       SP *sp)
#else
static INT sez_sp_from_cdp_bounds (seis_line, cdp, sp)
FULL_LINE_NAME *seis_line;
CDP_FLOAT cdp;
SP *sp;
#endif
    {
    INT status;
    SP sp1;
    LINE_HDR *p_hdr;
    CDP_FLOAT cdp_float;
    CDP_FLOAT mini_cdp, maxi_cdp;
    
    /* look for the pointer to the structure       */
    /* describing the header                       */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* For 3-d lines, the shot point and cdp number are considered to be the same
    */
    switch (p_hdr->line_type)
        {
    case X_3D:
    case Y_3D:
        *sp = cdp;
        return SUCCESS;
    default:
        break;
        }
    /* we now perform the computation using the    */
    /* structure informations                      */
    
    mini_cdp = MIN (p_hdr->init_cdp, p_hdr->final_cdp);
    maxi_cdp = MAX (p_hdr->init_cdp, p_hdr->final_cdp);
    
    if (cdp < mini_cdp)
        {
        cdp_float = mini_cdp;
        }
    else
        {
        if (cdp > maxi_cdp)
            {
            cdp_float = maxi_cdp;
            }
        else
            {
            cdp_float = cdp;
            }
        }
    /* retrieve the sp corresponding to the cdp    */
    
    status = se_sp_from_cdp_intern (p_hdr, cdp_float, &sp1);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    else
        {
        *sp = sp1;
        return SUCCESS;
        }
    }
/* END:     */
