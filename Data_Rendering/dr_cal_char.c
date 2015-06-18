/* DEC/CMS REPLACEMENT HISTORY, Element DR_CAL_CHAR.C*/
/* *3    31-JUL-1990 21:59:55 GILLESPIE "(SPR 1) Add esi_c_lib.h"*/
/* *2    12-JUN-1990 10:35:09 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *1    19-JUN-1989 12:46:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_CAL_CHAR.C*/

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

/******************************************************************************

NAME:           dr_calc_char_size

DESCRIPTION:    Convert from width-height format to height-expansion format


     Input:     dr_data 	- Standard data structure
                tx_width 	- Text width in WCS
                tx_height 	- Text heighth in WCS

     Output:    dr_data 	- Updated

     Status:
     
     TODO:		1) Separate the line spacing function into specific module.

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_ll.h"
#include    "esi_tg.h"

#if USE_PROTOTYPES
publicdef INT dr_calc_char_size(DR_DATA *dr_data, DOUBLE xmin, DOUBLE ymin,
                                DOUBLE xmax, DOUBLE ymax, INT *nl)
#else
publicdef INT dr_calc_char_size(dr_data, xmin, ymin, xmax, ymax, nl)
DR_DATA *dr_data;
DOUBLE xmin, ymin, xmax, ymax;
INT *nl;
#endif
    {
    INT count = 0;
    DOUBLE one_char_size, line_ht, approx_ht, dy;
    INT num_lines;
    CHAR str[512], txt[512], *s;
    LISTHEAD lhead;

    strcpy (txt, dr_data->current_text->text_string);
    llinit (&lhead, 0, sizeof(CHAR *), 0);
    s = strtok (txt, "\n");
    llappnd (&lhead, (VOIDPTR)&s);
    count++;
    while (s)
	{
	s = strtok (NULL, "\n");
	if (s)
	    {
	    llappnd (&lhead, (VOIDPTR)&s);
	    count++;
	    }
	}

    tg_fit_strings (&lhead, xmin, ymin, xmax, ymax, DR_DEFAULT_LINE_SPACING, 
		    &one_char_size);

    dr_data->current_text->text_line_spacing = DR_DEFAULT_LINE_SPACING * one_char_size;
    dr_data->current_text->text_size = one_char_size;
    dr_data->current_text->text_expansion_factor = 1.0;
    
/*	Update text defaults */

    dr_data->default_text.text_size = dr_data->current_text->text_size;
    dr_data->default_text.text_line_spacing = 
		dr_data->current_text->text_line_spacing;
    dr_data->default_text.text_expansion_factor = 1.0; 

    llfree (&lhead);
    *nl = count;
    return SUCCESS;
    }
