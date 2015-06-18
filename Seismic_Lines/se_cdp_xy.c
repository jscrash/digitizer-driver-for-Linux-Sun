/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_XY.C */
/* *5    30-OCT-1990 11:53:37 GILLESPIE "(SPR 37) Fix Sun side of code" */
/* *4    17-AUG-1990 22:16:05 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:08:02 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:05:16 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:45:23 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_XY.C */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_se.h"

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

/* flag used when computing closest point to a                                      */
/* segment                                                                          */
typedef enum 
    {
    IN_SEGMENT, BEGIN_SEGMENT, END_SEGMENT
    } SEIS_SEGMENT_FLAG;

#if USE_PROTOTYPES
static INT sez_close_point (FLOAT *px, FLOAT *py, FLOAT *p_cdp, UINT nb, COORD x,
                                COORD y, INT *range, CDP_FLOAT *cdp_found,
                                COORD *close_x, COORD *close_y, BOOL *extreme);
static INT sez_cdp_extrap (LINE_HDR *p_hdr, COORD x, COORD y, INT flag,
                               DOUBLE cdp_dist, CDP_FLOAT *cdp, COORD *x_proj,
                               COORD *y_proj);
static INT sez_ortho_xy (COORD x, COORD y, COORD x1, COORD y1, COORD x2, COORD y2,
                             COORD *closex, COORD *closey, DOUBLE *dx, DOUBLE *dy,
                             SEIS_SEGMENT_FLAG *flag);
#else
static INT sez_close_point ();
static INT sez_cdp_extrap ();
static INT sez_ortho_xy ();
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT sez_close_point (FLOAT *px, FLOAT *py, FLOAT *p_cdp, UINT nb,  */
/*                                COORD x, COORD y, INT *range,                     */
/*                                CDP_FLOAT *cdp_found, COORD *close_x,             */
/*                                COORD *close_y, BOOL *extreme);                   */
/*    static INT sez_cdp_extrap (LINE_HDR *p_hdr, COORD x, COORD y, INT flag,   */
/*                               DOUBLE cdp_dist, CDP_FLOAT *cdp, COORD *x_proj,    */
/*                               COORD *y_proj);                                    */
/*    static INT sez_ortho_xy (COORD x, COORD y, COORD x1, COORD y1,            */
/*                             COORD x2, COORD y2, COORD *closex,                   */
/*                             COORD *closey, DOUBLE *dx, DOUBLE *dy,               */
/*                             SEIS_SEGMENT_FLAG *flag);                            */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT se_cdp_from_xy (FULL_LINE_NAME *seis_line, COORD x, COORD y,    */
/*        DOUBLE cdp_dist, CDP *cdp_int, CDP_FLOAT *cdp_float,                      */
/*        COORD *x_proj, COORD *y_proj, CDP *cdp_end, COORD *x_end, COORD *y_end);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) seis_line   -(FULL_LINE_NAME *) Structure storing line_name + survey.     */
/*    (I) x           -(COORD) -\                                                   */
/*    (I) y           -(COORD) Coordinates.                                         */
/*    (I) cdp_dist    -(DOUBLE) Inter cdp distance which can come from the trace files -    */
/*                        if the distance is > 0 we take this distance as inter cdp */
/*                        distance if it is < 0 we take as inter_cdp                */
/*                        distance the distance computed from the SP spacing.       */
/*    (O) cdp_int     -(CDP *) Closest integer cdp on the line.                     */
/*    (O) cdp_float   -(CDP_FLOAT *) Closest float cdp on the line.                 */
/*    (O) cdp_end     -(CDP *) Closest end cdp on the line.                         */
/*                        if the projection of the cdp is out of the line linename if the   */
/*                        projection is inside the line cdp_end = cdp_int.          */
/*    (O) x_proj      -(COORD *) -\                                                 */
/*    (O) y_proj      -(COORD *) Coordinates of the true projection on the line.    */
/*    (O) x_end       -(COORD *) -\                                                 */
/*    (O) y_end       -(COORD *) Coordinates of cdp_end.                            */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT se_cdp_from_xy (FULL_LINE_NAME *seis_line, COORD x, COORD y,
                              DOUBLE cdp_dist, CDP *cdp_int, CDP_FLOAT *cdp_float,
                              COORD *x_proj, COORD *y_proj, CDP *cdp_end, COORD *x_end,
                              COORD *y_end)
#else
publicdef INT se_cdp_from_xy (seis_line, x, y, cdp_dist, cdp_int, cdp_float, x_proj,
                              y_proj, cdp_end, x_end, y_end)
FULL_LINE_NAME *seis_line;
COORD x, y;
DOUBLE cdp_dist;
CDP *cdp_int;
CDP_FLOAT *cdp_float;
COORD *x_proj, *y_proj;
CDP *cdp_end;
COORD *x_end, *y_end;
#endif
    {
    INT status, nb, range, deb, nb_inc, i, j;
    COORD x1, y1, x2, y2;
    CDP cdp_num1, cdp_num2;
    DOUBLE fact;
    FLOAT *px, *py, *p_cdp;         /* these REALLY want to be 4 byte floats        */
    LINE_HDR *p_hdr;
    BOOL extreme;
    DOUBLE rect_inter[4];
    DOUBLE cur_dx, cur_dy, cur_dist, min_dist, l_side;
    CDP_FLOAT cdp;
    COORD close_x, close_y;
    
    /* look for the pointer to the structure                                        */
    /*   describing the header                                                      */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    switch (p_hdr->line_type)
        {
    case X_3D:
    case Y_3D:
        /* Handle 3-D lines                                                         */
        if ((status = ss_set_cur_3d (seis_line->survey)) EQUALS SUCCESS)
            {
            status = ss_cdpli_xy (x, y, cdp_float, &p_hdr->line_number);
            *cdp_int = *cdp_float > 0 ? *cdp_float + 0.5 :  * cdp_float - 0.5;
            
            /* The following are 'temporary' patches jgg 15-jun-90                  */
            *x_proj = x;
            *y_proj = y;
            *cdp_end = *cdp_float;
            *x_end = x;
            *y_end = y;
            }
        return status;
    default:
        break;
        }
    /* now test that the nlist shot_xy has been                                     */
    /*   read or not                                                                */
    
    if (p_hdr->nb_points EQUALS NUL)
        {
        /* nlist not read so we try to read it                                      */
        
        if ((status = sef_read_optim (p_hdr)))
            {
            return status;
            }
        }
    /* we try to optimize the reseach for the                                       */
    /*   closest point pertaining to the line by                                    */
    /*   clipping the line by a rectangle in which                                  */
    /*   we are sure that the closest ppoint is for                                 */
    /*   this purpose we first compute a few                                        */
    /*   distances between the point x y and a set                                  */
    /*   of N_DEC points in the array of xy                                         */
    /*   composing the line which are reparted along                                */
    /*   the line the shortest distance will                                        */
    /*   determine a square around the point xy                                     */
    /*   which will be used to clip the sp,x,y array                                */
    
    min_dist = HUGE_VAL;
    
    nb_inc = p_hdr->nb_points / N_DEC;
    
    for (i = 1, px = p_hdr->array_x, py = p_hdr->array_y; i <= N_DEC;
         i++, px += nb_inc, py += nb_inc)
        {
        cur_dx = x - *px;
        cur_dy = y - *py;
        cur_dist = cur_dx * cur_dx + cur_dy * cur_dy;
        
        if (cur_dist < min_dist)
            {
            min_dist = cur_dist;
            }
        }
    /* look for distance to the last point of the                                   */
    /* line                                                                         */
    
    px = p_hdr->array_x + p_hdr->nb_points - 1;
    py = p_hdr->array_y + p_hdr->nb_points - 1;
    
    cur_dx = x - *px;
    cur_dy = y - *py;
    cur_dist = cur_dx * cur_dx + cur_dy * cur_dy;
    
    if (cur_dist < min_dist)
        {
        min_dist = cur_dist;
        }
    /* l_side is the length of the half side of                                     */
    /*   the rectangle used for clipping                                            */
    
    l_side = sqrt (min_dist) + TOLERANCE;
    
    /* we now clip the x y list to reduce the                                       */
    /*   number of operations the line is clipped by                                */
    /*   the rectangle rect_inter with xmax = x +                                   */
    /*   l_side ; xmin = x - l_side ; ymin = y -                                    */
    /*   l_side ; ymax = y + l_side ;                                               */
    
    rect_inter[0] = x - l_side;
    rect_inter[1] = x + l_side;
    rect_inter[2] = y - l_side;
    rect_inter[3] = y + l_side;
    
    status = sez_clip_array (p_hdr->array_x, p_hdr->array_y, p_hdr->nb_points,
                             rect_inter, &deb, &nb);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* routine used to compute the closest point                                    */
    /*   to x y pertaining to the line clipped by                                   */
    /*   the rectangle                                                              */
    
    px = p_hdr->array_x + deb - 1;
    py = p_hdr->array_y + deb - 1;
    p_cdp = p_hdr->array_cdp + deb - 1;
    
    status = sez_close_point (px, py, p_cdp, nb, x, y, &range, &cdp, &close_x,
                              &close_y, &extreme);
    
    /* if the closest point is the first or the                                     */
    /*   last of the line we need to extrapolate the                                */
    /*   line to find the cdp out of the line                                       */
    /*   coordinates which means that we don't have                                 */
    /*   coordinates for these cdp 'we need to                                      */
    /*   invent ' these coordinates                                                 */
    /*                                                                              */
    /*   we are on the first point if deb + range =                                 */
    /*   2 and extreme is true we are on the last                                   */
    /*   point if deb + range = p_hdr -> nb_points +                                */
    /*   1 and extreme is true                                                      */
    
    range += deb;
    *cdp_int = cdp;
    *cdp_float = cdp;
    *cdp_end = *cdp_int;
    *x_proj = close_x;
    *y_proj = close_y;
    
    /* attention the coordinates of x_end and                                       */
    /*   y_end are approximated                                                     */
    
    *x_end = *x_proj;
    *y_end = *y_proj;
    
    if (extreme AND range EQUALS 2)
        {
        /*      extrapolate before the first point of the                           */
        /*      line                                                                */
        
        status = sez_cdp_extrap (p_hdr, x, y, BEGIN_LIST, cdp_dist, &cdp, x_proj,
                                 y_proj);
        *cdp_float = cdp;
        p_cdp = p_hdr->array_cdp;
        *cdp_end = (INT)*p_cdp;
        *x_end = p_hdr->array_x[0];
        *y_end = p_hdr->array_y[0];
        *cdp_int = (INT)cdp;
        return status;
        }
    else
        {
        if (extreme AND range EQUALS p_hdr->nb_points + 1)
            {
            /*     extrapolate after the last point of the                          */
            /*     line                                                             */
            status = sez_cdp_extrap (p_hdr, x, y, END_LIST, cdp_dist, &cdp, x_proj,
                                     y_proj);
            *cdp_int = cdp;
            *cdp_float = cdp;
            p_cdp = p_hdr->array_cdp + (p_hdr->nb_points) - 1;
            *cdp_end = *p_cdp;
            *x_end = p_hdr->array_x[p_hdr->nb_points - 1];
            *y_end = p_hdr->array_y[p_hdr->nb_points - 1];
            
            return status;
            }
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT sez_close_point (FLOAT *px, FLOAT *py, FLOAT *p_cdp, UINT nb,  */
/*        COORD x, COORD y, INT *range, CDP_FLOAT *cdp_found,                       */
/*        COORD *close_x, COORD *close_y, BOOL *extreme);                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) x           -(COORD) -\                                                   */
/*    (I) y           -(COORD) Coordinates of a point px.                           */
/*    (I) px          -(FLOAT *) Float array of x coordinates of a line py.         */
/*    (I) py          -(FLOAT *) Float array of y coordinates of a line.            */
/*    (I) p_cdp       -(FLOAT *) Float array of cdp associated with xy              */
/*                        coordinate. Four byte float ptr.                          */
/*    (I) nb          -(UINT) Number of points in the array.                        */
/*    (O) range       -(INT *) The index of the closest                             */
/*                        point in the array cdp_found is the                       */
/*                        interpolated value of the cdp of the                      */
/*                        projection on the line of the closest point.              */
/*    (O) cdp_found   -(CDP_FLOAT *)                                                */
/*    (O) close_x     -(COORD *)                                                    */
/*    (O) close_y     -(COORD *)                                                    */
/*    (O) extreme     -(BOOL *) Is TRUE if the closest point is one                 */
/*                        of the ends of the array close_x , close_y                */
/*                        are coordinates.                                          */
/*                                                                                  */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <se_cdp_xy.c>                                                      */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT sez_close_point (FLOAT *px, FLOAT *py, FLOAT *p_cdp, UINT nb, COORD x,
                                COORD y, INT *range, CDP_FLOAT *cdp_found,
                                COORD *close_x, COORD *close_y, BOOL *extreme)
#else
static INT sez_close_point (px, py, p_cdp, nb, x, y, range, cdp_found, close_x,
                                close_y, extreme)
FLOAT *px, *py;
FLOAT *p_cdp;
UINT nb;
COORD x, y;
INT *range;
CDP_FLOAT *cdp_found;
COORD *close_x, *close_y;
BOOL *extreme;
#endif
    {
    
    /* computation is done in double precision                                      */
    FLOAT *p1_x, *p1_y, *p2_x, *p2_y;
    INT i, j;
    DOUBLE dist, dx, dy, cur_dist, min_dist, cur_x, cur_y, fact;
    SEIS_SEGMENT_FLAG flag, cur_flag;
    INT index;
    FLOAT *pcdp;
    DOUBLE x_close, y_close;
    
    /* initialise min_dist to a big value                                           */
    
    min_dist = HUGE_VAL;
    
    p1_x = px;
    p1_y = py;
    p2_x = px + 1;
    p2_y = py + 1;
    
    for (i = 1; i < nb; i++, p1_x++, p2_x++, p1_y++, p2_y++)
        {
        
        sez_ortho_xy (x, y, *p1_x, *p1_y, *p2_x, *p2_y, &cur_x, &cur_y, &dx, &dy,
                      &cur_flag);
        
        cur_dist = dx * dx + dy * dy;
        
        if (cur_dist < min_dist)
            {
            min_dist = cur_dist;
            flag = cur_flag;
            index = i;
            x_close = cur_x;
            y_close = cur_y;
            
            }
        }
    /* at the end of the routine we set the                                         */
    /*   boolean extreme if the closest point is one                                */
    /*   of the ends of the line this is if index =                                 */
    /*   1 and flag = BEGIN_SEGMENT or if index = nb                                */
    /*   - 1 and flag = END_SEGMENT                                                 */
    /*                                                                              */
    /*   we also compute the value of the cdp                                       */
    
    *close_x = x_close;
    *close_y = y_close;
    
    if (index EQUALS 1 AND flag EQUALS BEGIN_SEGMENT)
        {
        /* the closest point is the first of the array                              */
        *cdp_found = *p_cdp;
        *extreme = TRUE;
        *range = 1;
        }
    else
        {
        if (index EQUALS nb - 1 AND flag EQUALS END_SEGMENT)
            {
            /* the closest point is the last of the array                           */
            *cdp_found = *(p_cdp + nb - 1);
            *extreme = TRUE;
            *range = nb;
            }
        else
            {
            /* the closest point is not one of the ends of                          */
            /*   the segment we compute the interpolation of                        */
            /*   the cdp for this point we first test the                           */
            /*   flag to see if the point is on one end of a                        */
            /*   segment inside the line                                            */
            
            *extreme = FALSE;
            *range = index;
            
            if (flag EQUALS BEGIN_SEGMENT)
                {
                /* the nearest cdp is at the beginning of the                       */
                /*   segment                                                        */
                *cdp_found = *(p_cdp + index - 1);
                }
            else
                {
                if (flag EQUALS END_SEGMENT)
                    {
                    /* the nearest cdp is at the end of the                         */
                    /* segment                                                      */
                    
                    *cdp_found = *(p_cdp + index);
                    }
                else
                    {
                    /* the nearest cdp is in the middle of the                      */
                    /* segment                                                      */
                    
                    p1_x = px + index - 1;
                    p2_x = p1_x + 1;
                    p1_y = py + index - 1;
                    p2_y = p1_y + 1;
                    
                    sez_ortho_xy (x, y, *p1_x, *p1_y, *p2_x, *p2_y, &cur_x, &cur_y,
                                  &dx, &dy, &cur_flag);
                    
                    if (*p1_x IS_NOT_EQUAL_TO * p2_x)
                        {
                        fact = (cur_x - *p1_x) / (*p2_x - *p1_x);
                        }
                    else
                        {
                        fact = (cur_y - *p1_y) / (*p2_y - *p1_y);
                        }
                    pcdp = p_cdp + index - 1;
                    
                    *cdp_found = *pcdp + fact * (*(pcdp + 1) - *pcdp);
                    
                    return SUCCESS;
                    }
                }
            }
        }
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine used to compute an extrapolation                                      */
/*    of the cdp number when locating the cdp                                       */
/*    using the x , y coordinates.                                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT sez_cdp_extrap (LINE_HDR *p_hdr, COORD x, COORD y, INT flag,   */
/*        DOUBLE cdp_dist, CDP_FLOAT *cdp, COORD *x_proj, COORD *y_proj);           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    p_hdr           -(LINE_HDR *)                                                 */
/*    x               -(COORD)                                                      */
/*    y               -(COORD)                                                      */
/*    flag            -(INT)                                                        */
/*    cdp_dist        -(DOUBLE) Inter cdp distance which can                        */
/*                        come from the trace files - if the                        */
/*                        distance is > 0 we take this distance as                  */
/*                        inter cdp distance if it is < 0 we take as                */
/*                        inter_cdp distance the distance computed                  */
/*                        from the SP spacing.                                      */
/*    cdp             -(CDP_FLOAT *)                                                */
/*    (O) x_proj      -(COORD *) -\                                                 */
/*    (O) y_proj      -(COORD *) Coordinates of the                                 */
/*                        extrapolated cdp.                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <se_cdp_xy.c>                                                      */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT sez_cdp_extrap (LINE_HDR *p_hdr, COORD x, COORD y, INT flag,
                               DOUBLE cdp_dist, CDP_FLOAT *cdp, COORD *x_proj,
                               COORD *y_proj)
#else
static INT sez_cdp_extrap (p_hdr, x, y, flag, cdp_dist, cdp, x_proj, y_proj)
LINE_HDR *p_hdr;
COORD x, y;
INT flag;
DOUBLE cdp_dist;
CDP_FLOAT *cdp;
COORD *x_proj, *y_proj;
#endif
    {
    COORD x1, x2, y1, y2;
    COORD dif_cdp;
    DOUBLE dif_x, dif_y;
    DOUBLE nb_cdp;
    DOUBLE dis12, dis2;
    FLOAT *px, *py;
    COORD cur_x, cur_y;
    DOUBLE dx, dy;
    SEIS_SEGMENT_FLAG cur_flag;
    DOUBLE inter_cdp_dist;
    
    if (cdp_dist > 0)
        {
        inter_cdp_dist = cdp_dist;
        }
    else
        {
        inter_cdp_dist = ABS (p_hdr->sp_spacing / p_hdr->cdp_per_sp);
        }
    /* depending if we are at the beginning or at                                   */
    /*   the end of the line we fill x1 , x2 , y1 ,                                 */
    /*   y2                                                                         */
    
    if (flag EQUALS BEGIN_LIST)
        {
        x1 = *(p_hdr->array_x + 1);
        y1 = *(p_hdr->array_y + 1);
        x2 = *(p_hdr->array_x);
        y2 = *(p_hdr->array_y);
        }
    else
        {
        px = p_hdr->array_x + p_hdr->nb_points - 1;
        py = p_hdr->array_y + p_hdr->nb_points - 1;
        x1 = *(px - 1);
        y1 = *(py - 1);
        x2 = *px;
        y2 = *py;
        }
    /* we now call sez_ortho_xy to get the                                          */
    /*   coordinates of the projection of x , y on                                  */
    /*   the segment defined by x1,y1 x2,y2                                         */
    
    sez_ortho_xy (x, y, x1, y1, x2, y2, &cur_x, &cur_y, &dx, &dy, &cur_flag);
    
    dif_x = cur_x - x2;
    dif_y = cur_y - y2;
    dis12 = hypot (dif_x, dif_y);
    
    /* look for distance between cdp                                                */
    
    if (p_hdr->regular EQUALS REGULAR)
        {
        dis2 = inter_cdp_dist;
        }
    else
        {
        /* ATTENTION THIS PART NEEDS TO BE IMPLEMENTED                              */
        return FAIL;
        }
    /* using distances we compute the number of                                     */
    /* cdp                                                                          */
    
    nb_cdp = dis12 / dis2;
    
    if (flag EQUALS BEGIN_LIST)
        {
        *cdp = *(p_hdr->array_cdp) - nb_cdp;
        }
    else
        {
        *cdp = *(p_hdr->array_cdp + p_hdr->nb_points - 1) + nb_cdp;
        }
    *x_proj = cur_x;
    *y_proj = cur_y;
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT sez_ortho_xy (COORD x, COORD y, COORD x1, COORD y1, COORD x2, COORD y2,    */
/*        COORD *closex, COORD *closey, DOUBLE *dx, DOUBLE *dy, SEIS_SEGMENT_FLAG *flag);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*COORD x, y, x1, y1, x2, y2;                                                       */
/*COORD *closex, *closey;                                                           */
/*DOUBLE *dx, *dy;                                                                  */
/*SEIS_SEGMENT_FLAG *flag;                                                          */
/*    (I) x           -(COORD) -|                                                   */
/*    (I) y           -(COORD) -|                                                   */
/*    (I) x1          -(COORD) -|                                                   */
/*    (I) y1          -(COORD) -|                                                   */
/*    (I) x2          -(COORD) -\                                                   */
/*    (I) y2          -(COORD) Coord of a point x1,y1 coord                         */
/*                        of first point of a segment x2 ,y2 coord                  */
/*                        of second point of a segment.                             */
/*    (O) closex      -(COORD *) -\                                                 */
/*    (O) closey      -(COORD *) Coordinates of the point projection of x,y on the  */
/*                        segment (x1,y1) (x2,y2) it can be 'outside ofthe segment'.*/
/*    (O) dx          -(DOUBLE *) -\                                                */
/*    (O) dy          -(DOUBLE *) *dx = *closex - x *dy = *closey - y *flag         */
/*    (O) flag        -(SEIS_SEGMENT_FLAG *) Flag saying if the closest point is    */
/*                        inside the segment or one of the ends of                  */
/*                        the segment flag    = BEGIN_SEGMENT =                     */
/*                        END_SEGMENT = IN_SEGMENT.                                 */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <se_cdp_xy.c>                                                      */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT sez_ortho_xy (COORD x, COORD y, COORD x1, COORD y1, COORD x2, COORD y2,
                             COORD *closex, COORD *closey, DOUBLE *dx, DOUBLE *dy,
                             SEIS_SEGMENT_FLAG *flag)
#else
static INT sez_ortho_xy (x, y, x1, y1, x2, y2, closex, closey, dx, dy, flag)
COORD x, y, x1, y1, x2, y2;
COORD *closex, *closey;
DOUBLE *dx, *dy;
SEIS_SEGMENT_FLAG *flag;
#endif
    {
    
    DOUBLE xal, xal2, xal3, xproj, yproj;
    DOUBLE fact;
    
    if (ABS (y1 - y2) > EPSILON)
        {
        /* xal is the director coefficient of the                                   */
        /* perpendicular to the segment                                             */
        
        xal = (x1 - x2) / (y2 - y1);
        xal2 = xal * xal;
        *closex = (xal * (y1 - y) + xal2 * x + x1) / (1 + xal2);
        *closey = xal * (*closex - x) + y;
        
        fact = (y - y1) / (y2 - y1);
        }
    else                            /* the line is horizontal                       */
        {
        *closex = x;
        *closey = y1;
        fact = (x - x1) / (x2 - x1);
        }
    /* now verify if the projection is inside the                                   */
    /*   segment or not if not look for the closest                                 */
    /*   point and set the segment flag                                             */
    /* fact < 0 means that the point is outside                                     */
    /*   the segment closer to x1,y1 = 0 means that                                 */
    /*   the point = ( x1,y1) >0 and < 1 the point                                  */
    /*   is inside the segment = 1 the point is                                     */
    /*   equal to (x2,y2) > 1 outside the segment on                                */
    /*   the side of x2,y2                                                          */
    
    if (fact < EPSILON)
        {
        *flag = BEGIN_SEGMENT;
        *dx = x - x1;
        *dy = y - y1;
        }
    else
        {
        if (fact > 1 - EPSILON)
            {
            *flag = END_SEGMENT;
            *dx = x - x2;
            *dy = y - y2;
            }
        else
            {
            *flag = IN_SEGMENT;
            *dx = x - *closex;
            *dy = y - *closey;
            }
        }
    return SUCCESS;
    }
/* END:                                                                             */
