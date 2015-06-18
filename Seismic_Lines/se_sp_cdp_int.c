/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_CDP_INT.C */
/*  5    12-MAR-1992 10:36:32 JANTINA "(SPR 0) changed type of first column in nlist to INT" */
/* *4    17-AUG-1990 22:18:08 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:09:10 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:10:59 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:56:32 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_CDP_INT.C */
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



static VOIDPTR Var_List[NDIM];
static UINT Col_List[NDIM] = 
    {
    1,
    2,
    3
    };



/* Function Description -----------------------------------------------------
Description:
    This function gives the sp corresponding to
    the given cdp using the informations of the
    structure p_hdr.

Prototype:
    publicdef INT se_sp_from_cdp_intern (LINE_HDR *p_hdr, CDP_FLOAT cdp, SP *sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_hdr           -(LINE_HDR *)
    cdp             -(CDP_FLOAT)
    sp              -(SP *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_sp_from_cdp_intern (LINE_HDR *p_hdr, CDP_FLOAT cdp, SP *sp)
#else
publicdef INT se_sp_from_cdp_intern (p_hdr, cdp, sp)
LINE_HDR *p_hdr;
CDP_FLOAT cdp;
SP *sp;
#endif
    {
    INT status;
    DOUBLE nb, sp1;
    
    UINT tot_rows, nbp;
    SP sp_num[2], sp_cur;
    CDP cdp_num[2];
    DOUBLE mini_cdp, maxi_cdp, imincd, imaxcd, cdp_cur;
    SP mini_sp, maxi_sp, minsp, maxsp;
    UINT icurp, icurst;
    INT k;
    DOUBLE xfact;
    
    /* test first if we are on a regular           */
    /* correspondance                              */
    
    if (p_hdr->regular EQUALS REGULAR)
        {
        *sp = p_hdr->init_sp + (cdp - p_hdr->init_cdp) / p_hdr->cdp_per_sp;
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
        /* read the nlist */
        {
        status = se_read_shot_lab (p_hdr);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /* the nlist has now been read successfully    */
    /* inquire the total number of rows in the   */
    /* nlist                                       */
    
    status = nl_inq_nlist_int (p_hdr->shot_label, NL_NLIST_NROWS, (INT *) & tot_rows);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        nl_free_nlist (p_hdr->shot_label);
        p_hdr->shot_label = NULL;
        return status;
        }
    /* look first if we are outside the range of
       cdps if we so we extrapolate                */
    
    if (p_hdr->init_cdp < p_hdr->final_cdp)
        {
        mini_cdp = p_hdr->init_cdp;
        maxi_cdp = p_hdr->final_cdp;
        mini_sp = p_hdr->init_sp;
        maxi_sp = p_hdr->final_sp;
        }
    else
        {
        mini_cdp = p_hdr->final_cdp;
        maxi_cdp = p_hdr->init_cdp;
        mini_sp = p_hdr->final_sp;
        maxi_sp = p_hdr->init_sp;
        }
    if (cdp < mini_cdp)
        {
        /* cdp before the minimum cdp                  */
        *sp = mini_sp + (cdp - mini_cdp) / p_hdr->cdp_per_sp;
        return SUCCESS;
        }
    else
        {
        if (cdp > maxi_cdp)
            {
            /* cdp after the maximum cdp                   */
            *sp = maxi_sp + (cdp - maxi_cdp) / p_hdr->cdp_per_sp;
            return SUCCESS;
            }
        }
    /*  we come here where the input cdp is
        "inside the nlist" correspondance cdp-sp
        get the current row of the nlist         */
    
    Var_List[0] = (VOIDPTR) & cdp_num[0];
    Var_List[1] = (VOIDPTR) & sp_num[0];
    
    status = nl_inq_current_row (p_hdr->shot_label, &icurp, &icurst);
    
    status = nl_get_rows (p_hdr->shot_label, 1, NDIM2, Col_List, Var_List, &nbp);
    
    /* sp_cur and cdp_cur are the current couple ( */
    /* cdp,sp)                                     */
    
    cdp_cur = cdp_num[0];
    sp_cur = sp_num[0];
    
    /*  the nlist is always ordered by increasing
        cdp we look where the cuurent row is
        relatively to the input cdp and depending
        on that we go backwards or forwards in the
        nlist to find the correspondance           */
    
    if (cdp_cur EQUALS cdp)
        {
        *sp = sp_cur;
        return SUCCESS;
        }
    else if (cdp_cur > cdp)
        {
        
        /* looking backwards in the nlist              */
        
        for (k = icurp; k > 0; k--)
            {
            status = nl_previous_row (p_hdr->shot_label, NDIM2, Col_List, Var_List);
            
            /* test if we are around the current CDP if
               yes we stop looking and go for the
               interpolation                               */
            
            if (cdp >= cdp_num[0])
                {
                imincd = cdp_num[0];
                imaxcd = cdp_cur;
                minsp = sp_num[0];
                maxsp = sp_cur;
                break;
                }
            else
                {
                cdp_cur = cdp_num[0];
                sp_cur = sp_num[0];
                }
            }
        /* end for                                     */
        }
    else
        {
        /* looking forwards in the nlist               */
        
        for (k = icurp; k < tot_rows; k++)
            {
            status = nl_next_row (p_hdr->shot_label, NDIM2, Col_List, Var_List);
            
            /* test if we are around the current CDP if
               yes we stop looking and go for the
               interpolation                               */
            
            if (cdp <= cdp_num[0])
                {
                imaxcd = cdp_num[0];
                imincd = cdp_cur;
                maxsp = sp_num[0];
                minsp = sp_cur;
                break;
                }
            else
                {
                cdp_cur = cdp_num[0];
                sp_cur = sp_num[0];
                }
            }
        /* end for */
        }
    /* interpolation to get the value of the sp    */
    
    xfact = (cdp - imincd) / (imaxcd - imincd);
    *sp = minsp + xfact * (maxsp - minsp);
    
    return SUCCESS;
    }
/* END:     */
