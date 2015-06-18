/* DEC/CMS REPLACEMENT HISTORY, Element TO_OVERLAY.C */
/* *4     6-MAY-1991 16:49:09 JULIAN "(SPR 0) add select list in culture overlay for data mover" */
/* *3    14-AUG-1990 16:45:44 GILLESPIE "(SPR 2001) Use LINE_GC" */
/* *2    16-MAR-1990 09:14:05 JULIAN "(SPR 5095) culture" */
/* *1    16-MAR-1990 08:54:11 JULIAN "culture overlay" */
/* DEC/CMS REPLACEMENT HISTORY, Element TO_OVERLAY.C */
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
 * FILE : TO_OVERLAY.C
 *
 * ROUTINES : TO_OVERLAY()
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
#ifndef ESI_TO_H
#include "esi_to.h"
#endif

INT to_overlay(display_line)
    INT display_line;
{
    INT			status;
    MAP_STRUCTURE	*mapdef;
    MD_DISPLAY		*display;
    MD_COMPILED_MAPDEF	*compiled;
    MD_COMPILED_VERB	*verbstruc;
    MD_PARM_STRUCT	*parmstruc;
    TO_STRUCT		values;
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

    tc_zeroes( &values, sizeof(TO_STRUCT));
    values.text_color = -1;
    values.text_height = -1.0;
    values.text_font = -1;
    values.text_spacing = -1.0;
    values.line.color = -1;
    values.line.style = -1;
    values.line.width = -1.0;
    values.suppress_lines = FALSE;
    values.suppress_text = FALSE;
 	
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];

    for (i=0; i<verbstruc->nrows; i++) {
	if (verbstruc->key_used[i] != 0) {
	    parmstruc = verbstruc->parm_ptr[i];
	    switch (verbstruc->key_code[i]) {
		case 1:			/* ADD TO XYZ FILE */
		    values.add_to_xyz_file = parmstruc->parmval.cval;
		    break;
		case 3:			/* LINE COLOR */
		    values.line.color = (COLOR_INDEX)parmstruc->parmval.ival;
		    break;

		case 4:			/* LINE STYLE */
		    values.line.style = (INDEX)parmstruc->parmval.ival;
		    break;

		case 5:			/* LINE width */
		    values.line.width = (DOUBLE)parmstruc->parmval.ival;
		    break;

		case 8:			/* NAME */
		    values.name = parmstruc->parmval.cval;
		    break;

		case 9:			/* SELECT BY PHRASE */
		    values.select_by_phrase = parmstruc->parmval.cval;
		    values.select_by_phrase = STRING_ALLOCATE(parmstruc->parmval.cval);
		    strcpy((char *)values.select_by_phrase,(char *)parmstruc->parmval.cval);
		    ov_chk_where(&values.select_by_phrase);		    
		    break;

		case 10:                 /* SELECT LIST */
                    values.select_list = parmstruc->parmval.cval;
                    break;


		case 12:			/* SUPPRESS LINES */
		    values.suppress_lines = TRUE;
		    break;

		case 13:			/* SUPPRESS TEXT */
		    values.suppress_text = TRUE;
		    break;

		case 14:			/* TEXT COLOR */
		    values.text_color = parmstruc->parmval.ival;
		    break;

		case 15:			/* TEXT EXPANSION */
		    values.text_height = parmstruc->parmval.dbval;
		    break;

		case 16:			/* TEXT FONT */
		    values.text_font = parmstruc->parmval.ival;
		    break;

		case 17:			/* TEXT PRECISION */
		    values.text_spacing = parmstruc->parmval.dbval;
		    break;

		case 20:			/* TYPE */
		    values.object_type = parmstruc->parmval.cval;
		    break;

		default:
		    break;
		}
	    }
	}


    /* * SET UP FOR SCATTER FILE GENERATION * */
    /* -------------------------------------- */

    if ((values.xyz_file	!= (CHAR *)NULL) OR  
	(values.add_to_xyz_file != (CHAR *)NULL)    ) {
	if (values.xyz_file EQUALS (CHAR *)NULL) {
	    strcpy(name,values.add_to_xyz_file);
	    strcpy(mode,"a");
	    strcpy(desc,"Appended ");
	}
	else {
	    strcpy(name,values.xyz_file);
	    strcpy(mode,"w");
	    strcpy(desc,"Created ");
	}

	status = ov_open_scatter(&(values.scatter),name,mode,os_name);
	if (status != SUCCESS) return status;

    }


    /* * CALL THE POSTING ROUTINE * */
    /* ---------------------------- */

    status = ov_open_overlay(display_line,values.name);
    if (status EQUALS SUCCESS) {
    	
	status = to_post_grov(&values);
	ov_close_overlay(display_line);
    }

    /* * CLOSE SCATTER FILE * */
    /* ---------------------- */

    if (values.scatter != (NLIST_HEADER)0) {
	ov_close_scatter(values.scatter,name,mode,os_name);

	strcat(desc,"in overlay \"");
	strcat(desc,display->verbs[display_line]);
	strcat(desc,"\" in map \"");
	strcat(desc,mapdef->map_name);
	strcat(desc,"\"");
	fm_add_history_record("ESI$SCATTERS",name,"SCATTER","","","",desc);
    }

    return status;
}
