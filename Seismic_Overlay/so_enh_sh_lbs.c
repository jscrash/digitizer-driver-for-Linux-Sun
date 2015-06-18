/* DEC/CMS REPLACEMENT HISTORY, Element SO_ENH_SH_LBS.C */
/* *2    24-SEP-1990 17:23:16 GILLESPIE "Beta Deltas" */
/* *1     5-AUG-1990 13:22:44 SCD "Compute precision posting angle for seismic labels" */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ENH_SH_LBS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ENH_SH_LBS.C*/
/* *2    31-AUG-1990 18:24:08 SCD "(SPR 5750) New function to compute precision posting angles"*/
/* *1    13-AUG-1990 17:11:48 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ENH_SH_LBS.C*/

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

#include "esi_gl_defs.h"
#include "esi_so.h"
#include "esi_se.h"
#include "esi_wm.h"
#include "esi_math.h"

publicdef INT so_enhanced_shot_label(pso,cdp,offset_x, offset_y)

SO_STRUCT *pso;
CDP cdp;
DOUBLE *offset_x;
DOUBLE *offset_y;

{
INT status;
DOUBLE prev_x, prev_y, next_x, next_y;
DOUBLE zang, slope, angle;
TEXT local_text;

/* we have to look to the previous point and the next point
	from the one we want to get the "local" slope */

se_xy_from_cdp(&(pso->current_line),cdp-1,0.0,&prev_x, &prev_y);
	
se_xy_from_cdp(&(pso->current_line),cdp+1,0.0,&next_x, &next_y);

/* we borrow from so_load_disp_opts the code to determine where to
	put the generic label, except this is for the local slope */

if (abs(next_x-prev_x) > 0.001)	/* not vertical */
	{
	slope = (next_y - prev_y) / ( next_x - prev_x);
	zang = atan(slope);
	}
else
	{
	zang = PI / 2.0;
	}
angle = zang * 57.29582 + 90.0;
if (angle < 0.0)		/* if upsidy downy */
	angle += 360.0;		/* turn 'er aroun' */

/* reset zang */
zang = angle/57.29582;

so_angpp_to_text(&local_text, angle, SO_PERP);

*offset_x = cos(zang) * pso->seismic_generic.shotpoint_symbol.symbol_size
		* 1.5;
*offset_y = sin(zang) * pso->seismic_generic.shotpoint_symbol.symbol_size
		* 1.5;

/* override the defaults */
wm_gschup(local_text.text_up_x, local_text.text_up_y);
wm_gstxal(local_text.text_alignment[0], local_text.text_alignment[1]);

return SUCCESS;
}

publicdef INT so_enhanced_data_label(pso,cdp,offset_x, offset_y)

SO_STRUCT *pso;
CDP cdp;
DOUBLE *offset_x;
DOUBLE *offset_y;

{

INT status;
DOUBLE prev_x, prev_y, next_x, next_y;
DOUBLE zang, slope, angle;
TEXT local_text;

/* we have to look to the previous point and the next point
	from the one we want to get the "local" slope */

se_xy_from_cdp(&(pso->current_line),cdp-1,0.0,&prev_x, &prev_y);
	
se_xy_from_cdp(&(pso->current_line),cdp+1,0.0,&next_x, &next_y);

/* we borrow from so_load_disp_opts the code to determine where to
	put the generic label, except this is for the local slope */

if (abs(next_x-prev_x) > 0.001)	/* not vertical */
	{
	slope = (next_y - prev_y) / ( next_x - prev_x);
	zang = atan(slope);
	}
else
	{
	zang = PI / 2.0;
	}
angle = zang * 57.29582 + 90.0;
if (angle < 0.0)		/* if upsidy downy */
	angle += 360.0;		/* turn 'er aroun' */

/* reset zang */
zang = angle/57.29582;

*offset_x = cos(zang) * pso->seismic_generic.shotpoint_symbol.symbol_size
		* -1.5;
*offset_y = sin(zang) * pso->seismic_generic.shotpoint_symbol.symbol_size
		* -1.5;

zang = angle + 180.0;
if (zang >= 360.0)
	zang -= 360.0;

so_angpp_to_text(&local_text, zang, SO_PERP);

/* override the defaults */
wm_gschup(local_text.text_up_x, local_text.text_up_y);
wm_gstxal(local_text.text_alignment[0], local_text.text_alignment[1]);

return SUCCESS;
}
