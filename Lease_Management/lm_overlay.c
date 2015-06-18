/* DEC/CMS REPLACEMENT HISTORY, Element LM_OVERLAY.C */
/* *5    11-SEP-1991 13:41:06 JULIAN "(SPR 0) add comments" */
/* *4     9-AUG-1991 12:47:29 JULIAN "(SPR 3683) updated info messages" */
/* *3    24-JUL-1991 15:02:13 JTM "(SPR 8591) Corrected error message wording." */
/* *2    18-MAR-1991 18:09:08 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:55:52 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element LM_OVERLAY.C */

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

/*
 * FILE : LM_OVERLAY.C
 *
 * ROUTINES : LM_OVERLAY()
 *
 * DESCRIPTION : This module will parse and draw a maps Topological overlay.
 * 
 */

/* Include Files */
#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif
#ifndef ESI_OV_H
#include "esi_ov.h"		   /* overlay defintions */
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"                /* mapping structures */
#endif
#ifndef ESI_FM_H
#include "esi_fm.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_LM_H
#include "esi_lm.h"
#endif


INT lm_overlay(display_line)
    INT display_line;
{
    INT			status;
    MAP_STRUCTURE	*mapdef;
    MD_DISPLAY		*display;
    MD_COMPILED_MAPDEF	*compiled;
    MD_COMPILED_VERB	*verbstruc;
    MD_PARM_STRUCT	*parmstruc;
    LM_STRUCT		values;
    INT			row;
    INT			i;
    FM_FILE		name;
    FILENAME		os_name;
    CHAR		mode[2];
    CHAR		desc[241];

    status = SUCCESS;
    status = mp_get_current_map(&mapdef);


    /* * INITIALIZE GRAPHIC OBJECT STRUCTURE * */
    /* --------------------------------------- */
    tc_zeroes( &values, sizeof(LM_STRUCT));
    values.text_color = -1;
    values.text_height = -1.0;
    values.text_font = -1;
    values.text_precision = -1;
    values.line.color = -1;
    values.line.style = -1;
    values.line.width = -1.0;
    values.annotate = (CHAR *)0;
    values.color_fill_attribute = (CHAR *)0;
    values.color_fill_clut = (CHAR *)0;
 	
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];

    for (i=0; i<verbstruc->nrows; i++) {
	if (verbstruc->key_used[i] != 0) {
	    parmstruc = verbstruc->parm_ptr[i];
	    switch (verbstruc->key_code[i]) {
		case 1:			/* ANNOTATE */
		    values.annotate = parmstruc->parmval.cval;
		    break;
		case 2:			/* COLOR/FILL ATTRIBUTE */
		    values.color_fill_attribute = parmstruc->parmval.cval;
		    break;

		case 3:			/* COLOR/FILL CLUT */
		    values.color_fill_clut = parmstruc->parmval.cval;
		    break;

		case 4:			/* LINE COLOR */
		    values.line.color = parmstruc->parmval.ival;
                    break;
		case 5:			/* LINE STYLE */
		    values.line.style = parmstruc->parmval.ival;
		    break;

		case 6:			/* LINE width */
		    values.line.width = parmstruc->parmval.ival;
		    break;

		case 7:			/* SELECT BY PHRASE */
		    values.select_by_phrase = parmstruc->parmval.cval;
		    break;

		case 8:			/* SELECT LIST */
		    values.select_list = parmstruc->parmval.cval;
		    break;


		case 9:			/* SUPPRESS LINES */
		    values.suppress_lines = TRUE;
		    break;

		case 10:			/* SUPPRESS TEXT */
		    values.suppress_text = TRUE;
		    break;

		case 11:			/* TEXT COLOR */
		    values.text_color = parmstruc->parmval.ival;
		    break;

		case 12:			/* TEXT EXPANSION */
		    values.text_height = parmstruc->parmval.dbval;
		    break;

		case 13:			/* TEXT FONT */
		    values.text_font = parmstruc->parmval.ival;
		    break;

		case 14:			/* TEXT PRECISION */
		    values.text_precision = parmstruc->parmval.ival;
		    break;

		default:
		    break;
		}
	    }
	}

			     /*	check if option COLOR/FILL ATTRIBUTE and    */
			     /*	COLOR/FILL CLUT are either both empty or    */
			     /*	both exist				    */
    if(values.color_fill_attribute EQUALS (CHAR *)0 AND
       values.color_fill_clut != (CHAR *)0) 
      {
	am_message(AM_ALERT,"Please add the LEASE ATTRIBUTE option to the \
overlay or delete the COLOR/FILL DEF option from the overlay.");
	return SUCCESS;
      }
    if(values.color_fill_attribute != (CHAR *)0 AND
       values.color_fill_clut EQUALS (CHAR *)0)
      {
	am_message(AM_ALERT,"Please add the COLOR/FILL DEF option to the \
overlay or delete the LEASE ATTRIBUTE option from the overlay.");
	return SUCCESS;
      }

    status = ov_open_overlay(display_line,values.name);
    if (status EQUALS SUCCESS)
	{
	status = lm_display_overlay(&values);
	ov_close_overlay(display_line);
	}

    return status;
}
