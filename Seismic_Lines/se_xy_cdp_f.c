/* DEC/CMS REPLACEMENT HISTORY, Element SE_XY_CDP_F.C */
/*  7    28-FEB-1992 10:28:28 CHEW "(SPR 5912) Fixed divide zero error." */
/* *6     4-DEC-1990 11:10:20 KEE "(SPR -1) fixing y2 = *(p_hdr->array_x) bug to be y2 = *(p_hdr->array_y)" */
/* *5    17-AUG-1990 22:18:39 VINCE "(SPR 5644) Code Cleanup" */
/* *4    18-JUN-1990 12:09:48 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *3    29-MAR-1990 16:43:02 MING "(SPR 5173) GULF bug fix" */
/* *2     1-NOV-1989 14:14:51 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 14:01:09 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_XY_CDP_F.C */
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
static INT sez_extrap_xy (INT flag, LINE_HDR *p_hdr, CDP_FLOAT cdp,
                              DOUBLE cdp_dist, COORD *x, COORD *y);
#else
static INT sez_extrap_xy ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT se_xy_from_cdp_float (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
        DOUBLE cdp_dist, COORD *x, COORD *y);

Private_Functions:
    static INT sez_extrap_xy (INT flag, LINE_HDR *p_hdr, CDP_FLOAT cdp,
                              DOUBLE cdp_dist, COORD *x, COORD *y);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    For the seis_line (structure storing
    line_name + survey ) this function returns
    the x and y coordinates of the cdp.

Prototype:
    publicdef INT se_xy_from_cdp_float (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
        DOUBLE cdp_dist, COORD *x, COORD *y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    cdp             -(CDP_FLOAT)
    cdp_dist        -(DOUBLE)
    x               -(COORD *)
    y               -(COORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_xy_from_cdp_float (FULL_LINE_NAME *seis_line, CDP_FLOAT cdp,
                                    DOUBLE cdp_dist, COORD *x, COORD *y)
#else
publicdef INT se_xy_from_cdp_float (seis_line, cdp, cdp_dist, x, y)
FULL_LINE_NAME *seis_line;
CDP_FLOAT cdp;
DOUBLE cdp_dist;
COORD *x, *y;
#endif
    {
    INT status, j;
    COORD x1, y1, x2, y2;
    CDP_FLOAT cdp_num1, cdp_num2;
    DOUBLE fact;
    FLOAT *p_cdp;                   /* this REALLY needs to be a float (4 bytes) */
    LINE_HDR *p_hdr;
    CDP_FLOAT low_cdp_float, high_cdp_float;
    
    /* look if the line is already here and if it
       is we verify that the nlist optim_xy has
       been read if not we read the nlist          */
    
    /* look for the pointer to the structure
       describing the header                       */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    switch (p_hdr->line_type)
        {
    case X_3D:
    case Y_3D:
        if ((status = ss_set_cur_3d (seis_line->survey)) IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        return ss_xy_cdpli (cdp, p_hdr->line_number, x, y);
    default:
        break;
        }
    if (p_hdr->nb_points EQUALS NO_POSITION_DATA)   /* GULF bug fix */
        {
        return FAIL;
        }
    /* now test that the nlist shot_xy has been
       read or not                                 */
    
    if (p_hdr->nb_points EQUALS NUL)
        {
        /* nlist not read so we try to read it         */
        
        if ((status = sef_read_optim (p_hdr)) IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /* computation of the coordinate x y
       corresponding to CDP we assume that the
       nlist is ordered by increasing cdp we test
       first if the cdp is inside the bounds of
       the nlist if not that means that we need to
       extrapolate to find the coordinate - if it
       is inside the bounds we begin from the
       current point and look for the interval
       where the cdp is and then perform an
       interpolation to have the exact coordinates */
    
    low_cdp_float = p_hdr->low_cdp_coord;
    
    if (cdp < low_cdp_float)
        {
        /* we extrapolate before the first cdp which   */
        /* has some coordinates                        */
        
        return sez_extrap_xy ((INT)BEGIN_LIST, p_hdr, cdp, cdp_dist, x, y);
        }
    else
        {
        high_cdp_float = p_hdr->up_cdp_coord;
        
        if (cdp > high_cdp_float)
            
            /* we extrapolate after the last cdp which has */
            /* some coordinates                            */
            {
            return(sez_extrap_xy ((INT)END_LIST, p_hdr, cdp, cdp_dist, x, y));
            }
        else                        /* we now go through the nlist to find the cdp */
            /* we are interested in                        */
            {
            /* first look current point corresponding to
               index p_hdr cur_index to decide if we go up
               or down in the nlist                        */
            
            p_cdp = p_hdr->array_cdp + p_hdr->cur_index;
            
            if (cdp >= *p_cdp)
                {
                /* we go up in the nlist until finding and the
                   interval where we are going to interpolate  */
                
                for (j = p_hdr->cur_index; j < p_hdr->nb_points AND cdp > *p_cdp;
                     j++, p_cdp++)
                    ;
                if (j > 0)
                    j--;
                
                }
            else
                {
                /* we go down in the nlist until finding and
                   the interval where we are going to
                   interpolate                                 */
                
                for (j = p_hdr->cur_index; j >= 0 AND cdp < *p_cdp; j--, p_cdp--)
                    ;
                }
            cdp_num1 = *(p_hdr->array_cdp + j);
            cdp_num2 = *(p_hdr->array_cdp + j + 1);
            x1 = *(p_hdr->array_x + j);
            x2 = *(p_hdr->array_x + j + 1);
            y1 = *(p_hdr->array_y + j);
            y2 = *(p_hdr->array_y + j + 1);
            p_hdr->cur_index = j;
            
            fact = (cdp - cdp_num1) / (cdp_num2 - cdp_num1);
            *x = x1 + fact * (x2 - x1);
            *y = y1 + fact * (y2 - y1);
            return SUCCESS;
            }
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Routine used to extrapolate the coordinates
    for the cdp which have not their
    coordinates in the list cdp,x, y.

Prototype:
    static INT sez_extrap_xy (INT flag, LINE_HDR *p_hdr, CDP_FLOAT cdp,
        DOUBLE cdp_dist, COORD *x, COORD *y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    flag            -(INT)
    p_hdr           -(LINE_HDR *)
    cdp             -(CDP_FLOAT)
    cdp_dist        -(DOUBLE)   Inter cdp distance which can
                        come from the trace files - if the distance
                        is > 0 we take this distance as inter cdp
                        distance if it is < 0 we take as inter_cdp
                        distance the distance computed from the SP
                        spacing.
    x               -(COORD *)
    y               -(COORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <se_xy_cdp_f.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sez_extrap_xy (INT flag, LINE_HDR *p_hdr, CDP_FLOAT cdp,
                              DOUBLE cdp_dist, COORD *x, COORD *y)
#else
static INT sez_extrap_xy (flag, p_hdr, cdp, cdp_dist, x, y)
INT flag;
LINE_HDR *p_hdr;
CDP_FLOAT cdp;
DOUBLE cdp_dist;
COORD *x, *y;
#endif
    {
    COORD x1, x2, y1, y2;
    CDP_FLOAT dif_cdp;
    DOUBLE dis1, dis2, dis12, dif_x, dif_y;
    FLOAT *px, *py;                 /* 4 byte float!! */
    DOUBLE inter_cdp_dist;
    
    if (cdp_dist > 0.0)
        {
        inter_cdp_dist = cdp_dist;
        }
    else
        {
        inter_cdp_dist = ABS (p_hdr->sp_spacing / p_hdr->cdp_per_sp);
        }
    /* depending if we are at the beginning or at
       the end of the line we fill x1 , x2 , y1 ,
       y2                                          */
    
    if (flag EQUALS BEGIN_LIST)
        {
        x1 = *(p_hdr->array_x + 1);
        y1 = *(p_hdr->array_y + 1);
        x2 = *(p_hdr->array_x);
        y2 = *(p_hdr->array_y);
        dif_cdp = p_hdr->low_cdp_coord - cdp;
        }
    else
        {
        px = p_hdr->array_x + p_hdr->nb_points - 1;
        py = p_hdr->array_y + p_hdr->nb_points - 1;
        x1 = *(px - 1);
        y1 = *(py - 1);
        x2 = *px;
        y2 = *py;
        dif_cdp = cdp - p_hdr->up_cdp_coord;
        }
    dif_x = x2 - x1;
    dif_y = y2 - y1;
    dis12 = hypot (dif_x, dif_y);
    
    /* look for distance between cdp               */
    
    if (p_hdr->regular EQUALS REGULAR)
        {
        dis2 = dif_cdp * inter_cdp_dist;
        }
    else
        {                           /* ATTENTION THIS PART NEEDS TO BE IMPLEMENTED */
        
        return FAIL;
        }
    dis1 = dis2 + dis12;
    
    if (dis12 > EPSILON)
        {
        *x = (FLOAT)((dis1 * dif_x + x2 * dis12) / dis12);
        *y = (FLOAT)((dis1 * dif_y + y2 * dis12) / dis12);
        return SUCCESS;
        }
    else
        {
        return FAIL;
        }
    }
/* END:     */
