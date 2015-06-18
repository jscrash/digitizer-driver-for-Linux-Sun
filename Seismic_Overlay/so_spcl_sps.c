/* DEC/CMS REPLACEMENT HISTORY, Element SO_SPCL_SPS.C */
/* *2    27-JUL-1990 14:07:58 GILLESPIE "(SPR 1) Change alling arg to bu UINT *" */
/* *1     5-MAR-1990 17:03:16 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_SPCL_SPS.C */

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/***********************************************************************/
/*                                                                     */
/* NAME:     SO_CHECK_LABEL.C                                          */
/* AUTHOR:   STEVE SPENCE                                              */
/* FUNCTION: FUNCTION TO SEE IF A SHOTPOINT SHOULD BE LABELED          */
/*                                                                     */
/***********************************************************************/

#include "esi_so.h"
#include "esi_se.h"
#include "esi_nl.h"
#include "esi_ct.h"
#include "esi_tc.h"
#include "esi_math.h"

#if USE_PROTOTYPES
publicdef INT so_special_shot_points(SO_STRUCT *pso,CDP first_cdp,
                                     CDP last_cdp,CDP **sym_list, UINT *nsyms)
#else
publicdef INT so_special_shot_points(pso,first_cdp,last_cdp,sym_list,nsyms)

SO_STRUCT *pso;                     /* seismic overlay structure */
CDP first_cdp;
CDP last_cdp;
CDP **sym_list;
UINT *nsyms;
#endif
{

INT    status;
UINT i;
DOUBLE angle;
INT    paraperp;
UINT   Col_List[3];
VOIDPTR Var_List[3];
DOUBLE zvalue;
DOUBLE pnt_in[2],pnt_out[2];
DOUBLE zang;
CDP *temp_list;
SP sp;
BOOL zval;

/**************************************************************************/

*nsyms = 0;
*sym_list = (CDP *)0;

nl_inq_nlist_int(pso->disp_opt.shot_label_nlist,NL_NLIST_NPOINTS,(INT *)(&i));
if (i > 0)
    {
    if (pso->seismic_generic.draw_shotpoint_symbol)
	{
	*sym_list = (CDP *) tc_alloc((i + 2) * sizeof(CDP));
	}

    temp_list = (CDP *) tc_alloc(i * sizeof(CDP));
    Col_List[0]=1;
    Col_List[1]=2;
    Col_List[2]=3;
    Var_List[0] = (VOIDPTR) temp_list;
    Var_List[1] = (VOIDPTR) &angle;
    Var_List[2] = (VOIDPTR) &paraperp;
    nl_get_points(pso->disp_opt.shot_label_nlist,i,3,Col_List,Var_List,&i);

				/* FIND STARTING POINT */
    i = 0;
    while (temp_list[i++] < first_cdp);

				/* CHECK FIRST CDP */
    if (temp_list[i] != first_cdp)
	{
	if (pso->seismic_generic.draw_shotpoint_symbol)
	    {
	    *sym_list[(*nsyms)++] = first_cdp;
	    }

	if (pso->seismic_generic.draw_shotpoint_label)
	    {
	    se_sp_from_cdp(&(pso->current_line),first_cdp,&sp);
	    se_xy_from_cdp(&(pso->current_line),first_cdp,0.0,
						&(pnt_in[0]),&(pnt_in[1]));
	    ct_map_xy(pnt_in[0],pnt_in[1],&pnt_out[0],&pnt_out[1]);

	    zval = TRUE;
	    if (pso->zzzz)
		{
		zvalue = SO_ZZZZ;
		}
	    else if (pso->seismic_generic.horizons_ok)
		{
		so_zvalue(pso,first_cdp,first_cdp,pnt_in[0],pnt_in[1],&zvalue);
		}
	    else
		{
		zval = FALSE;
		}
	    so_draw_label(pso,pnt_out[0],pnt_out[1],zvalue,sp,TRUE,TRUE,zval);
	    }
	}

				/* DO LIST */
    while (temp_list[i] <= last_cdp)
	{
				/* paraperp = SO_NOPARA means skip it */
	if (paraperp EQUALS SO_NOPARA )
	    {
	    continue;
	    }

	if (pso->seismic_generic.draw_shotpoint_symbol)
	    {
	    *sym_list[(*nsyms)++] = temp_list[i];
	    }

	if (pso->seismic_generic.draw_shotpoint_label)
	    {
	    se_sp_from_cdp(&(pso->current_line),temp_list[i],&sp);
	    se_xy_from_cdp(&(pso->current_line),temp_list[i],0.0,
						&(pnt_in[0]),&(pnt_in[1]));
	    ct_map_xy(pnt_in[0],pnt_in[1],&pnt_out[0],&pnt_out[1]);

	    zval = TRUE;
	    if (pso->zzzz)
		{
		zvalue = SO_ZZZZ;
		}
	    else if (pso->seismic_generic.horizons_ok)
		{
		so_zvalue(pso,temp_list[i],temp_list[i],pnt_in[0],pnt_in[1],&zvalue);
		}
	    else
		{
		zval = FALSE;
		}
				/* SET DEFAULTS */
	    pso->seismic_generic.shotpoint_label = 
				    pso->disp_opt.shot_label_default;

				/* CALCULATE OVERRIDES */
	    so_angpp_to_text(&pso->seismic_generic.shotpoint_label,
							    angle,paraperp);
	    zang = angle / 57.29582;
	    angle = angle + 180.0;
	    if (angle >= 360.0)
		{
		angle -= 360.0;
		}
	    so_angpp_to_text(&pso->seismic_generic.zvalue_label,angle,paraperp);

	    pso->seismic_generic.shotpoint_label.text_offset.x =
		    1.5 * cos(zang) * 
			pso->seismic_generic.shotpoint_symbol.symbol_size;

	    pso->seismic_generic.shotpoint_label.text_offset.y =
		    1.5 * sin(zang) * 
			pso->seismic_generic.shotpoint_symbol.symbol_size;

	    pso->seismic_generic.zvalue_label.text_offset.x =
		    (- pso->seismic_generic.shotpoint_label.text_offset.x);
	    pso->seismic_generic.zvalue_label.text_offset.y =
		    (- pso->seismic_generic.shotpoint_label.text_offset.y);

	    so_draw_label(pso,pnt_out[0],pnt_out[1],zvalue,sp,TRUE,TRUE,zval);
	    }

	i++;
	}

				/* CHECK LAST CDP */

    if (temp_list[i] != last_cdp)
	{
	if (pso->seismic_generic.draw_shotpoint_symbol)
	    {
	    *sym_list[(*nsyms)++] = last_cdp;
	    }

	if (pso->seismic_generic.draw_shotpoint_label)
	    {
	    se_sp_from_cdp(&(pso->current_line),last_cdp,&sp);
	    se_xy_from_cdp(&(pso->current_line),last_cdp,0.0,
						&(pnt_in[0]),&(pnt_in[1]));
	    ct_map_xy(pnt_in[0],pnt_in[1],&pnt_out[0],&pnt_out[1]);

	    zval = TRUE;
	    if (pso->zzzz)
		{
		zvalue = SO_ZZZZ;
		}
	    else if (pso->seismic_generic.horizons_ok)
		{
		so_zvalue(pso,last_cdp,last_cdp,pnt_in[0],pnt_in[1],&zvalue);
		}
	    else
		{
		zval = FALSE;
		}
	    so_draw_label(pso,pnt_out[0],pnt_out[1],zvalue,sp,TRUE,TRUE,zval);
	    }
	}
    }
return SUCCESS;
}
