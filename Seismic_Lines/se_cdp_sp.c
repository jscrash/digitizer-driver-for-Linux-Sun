/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_SP.C */
/* *4    17-AUG-1990 22:15:47 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:07:37 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:03:43 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:42:58 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_SP.C */
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

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#define NDIM 3
#define NDIM2 2
static INT datatype[NDIM] = 
    {
    FLOAT_TYPE,
    FLOAT_TYPE,
    FLOAT_TYPE
    };



static size_t width[NDIM] = 
    {
    sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT)
    };



static INT units[NDIM] = 
    {
    0,
    0,
    0
    };



static VOIDPTR value_list[NDIM];
static UINT dimlist[NDIM] = 
    {
    1,
    2,
    3
    };



/* Function Description -----------------------------------------------------
Description:
    For the seis_line (structure storing
    line and survey name ) this function gives
    the cdp corresponding to the given sp.

Prototype:
    publicdef INT se_cdp_from_sp (FULL_LINE_NAME *seis_line, SP sp, CDP *cdp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    sp              -(SP)
    cdp             -(CDP *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_cdp_from_sp (FULL_LINE_NAME *seis_line, SP sp, CDP *cdp)
#else
publicdef INT se_cdp_from_sp (seis_line, sp, cdp)
FULL_LINE_NAME *seis_line;
SP sp;
CDP *cdp;
#endif
    {
    DOUBLE nb, cdp_flo;
    INT status;
    UINT tot_points, nbp;
    LINE_HDR *p_hdr;
    SP sp_num[2], sp_cur;
    CDP cdp_num[2];
    CDP_FLOAT imincd, imaxcd, cdp_cur;
    SP minsp, maxsp;
    BOOL found;
    INT k;
    DOUBLE xfact;
    
    /* test first if we are on a regular           */
    /* correspondance                              */
    /* look for the pointer to the structure       */
    /* describing the header                       */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* For 3-D lines, the cdp and shot point are considered to be the same */
    switch (p_hdr->line_type)
        {
    case X_3D:
    case Y_3D:
        *cdp = sp;
        return SUCCESS;
    default:
        break;
        }
    if (p_hdr->regular EQUALS REGULAR)
        {
        *cdp = p_hdr->init_cdp + (sp - p_hdr->init_sp) * p_hdr->cdp_per_sp;
        return SUCCESS;
        }
    /* we are not on a regular correspondance      */
    /* we are not in the case of a regular
   correspondance between SP CDP so we need to
   examine the nlist SHOT_LABELS               */
    /* test first if the nlist was read in memory
   - if not we go and read it we just go
   through the nlist to find the
   correspondance                              */
    
    if (p_hdr->shot_label EQUALS NULL)
        /* read the nlist                              */
        {
        status = se_read_shot_lab (p_hdr);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /* the nlist has now been read successfully
       inquire the total number of points in the
       nlist                                       */
    
    status = nl_inq_nlist_int (p_hdr->shot_label, NL_NLIST_NPOINTS,
                               (INT *) & tot_points);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        nl_free_nlist (p_hdr->shot_label);
        p_hdr->shot_label = NULL;
        return status;
        }
    /*  we need to go through the nlist to look
        for the sp for which we need the
        correspondance                             */
    
    /* set current point to first point of the     */
    /* nlist                                       */
    
    status = nl_set_current_point (p_hdr->shot_label, 1, 1);
    
    /* get the first point of the nlist            */
    
    value_list[0] = (VOIDPTR) & cdp_num[0];
    value_list[1] = (VOIDPTR) & sp_num[0];
    
    
    status = nl_get_points (p_hdr->shot_label, 1, NDIM2, dimlist, value_list, &nbp);
    
    /* sp_cur and cdp_cur are the current couple ( */
    /* cdp,sp)                                     */
    
    cdp_cur = cdp_num[0];
    sp_cur = sp_num[0];
    
    for (k = 1, found = FALSE; k < tot_points; k++)
        {
        status = nl_next_point (p_hdr->shot_label, NDIM2, dimlist, value_list);
        
        /* test if we are around the current sp if yes
           we stop looking and go for the
           interpolation                               */
        
        if (((sp - sp_num[0]) * (sp - sp_cur)) <= 0)
            {
            imincd = cdp_cur;
            imaxcd = cdp_num[0];
            minsp = sp_cur;
            maxsp = sp_num[0];
            found = TRUE;
            break;
            }
        else
            {
            cdp_cur = cdp_num[0];
            sp_cur = sp_num[0];
            }
        }
    /* end for                                     */
    /* if we found the SP the boolean found is
   true if it is false and we try to
   extrapolate the SP                          */
    
    if (!found)
        {
        /* if we didn't found the SP inside the
           correspondance file we try to extrapolate
           to get an estimation of the cdp
        
           first we look if the SP seems to be before
           the beginning of the correspondance         */
        
        status = nl_set_current_point (p_hdr->shot_label, 1, 1);
        
        /* read the two first points to look at the
           beginning of the nlist                      */
        
        status = nl_get_points (p_hdr->shot_label, 2, NDIM2, dimlist, value_list,
                                &nbp);
        cdp_cur = cdp_num[0];
        sp_cur = sp_num[0];
        
        if (((sp_num[0] - sp) * (sp_num[0] - sp_num[1])) <= 0.)
            {
            imincd = cdp_num[1];
            imaxcd = cdp_num[0];
            minsp = sp_num[1];
            maxsp = sp_num[0];
            found = TRUE;
            }
        /* if the SP cannot be extrapolated at the
           beginning of the nlist we try at the end    */
        
        if (NOT found)
            {
            
            /* read the two last points to look at the end
               of the nlist                                */
            
            
            status = nl_set_current_point (p_hdr->shot_label, tot_points - 1, 1);
            
            status = nl_get_points (p_hdr->shot_label, 2, NDIM2, dimlist, value_list,
                                    &nbp);
            cdp_cur = cdp_num[0];
            sp_cur = sp_num[0];
            
            if (((sp_num[1] - sp) * (sp_num[1] - sp_num[0])) <= 0.)
                {
                imincd = cdp_num[1];
                imaxcd = cdp_num[0];
                minsp = sp_num[1];
                maxsp = sp_num[0];
                found = TRUE;
                }
            if (!found)
                return FAIL;
            }
        }
    /* interpolation to get the value of the CDP   */
    
    xfact = (sp - minsp) / (maxsp - minsp);
    cdp_flo = imincd + xfact * (imaxcd - imincd);
    
    /* cdp is an integer number                    */
    
    nb = floor (cdp_flo);
    if (fabs (cdp_flo - nb) < 0.5)
        {
        *cdp = nb;
        }
    else
        {
        *cdp = nb + 1;
        }
    return SUCCESS;
    }
/* END:     */
