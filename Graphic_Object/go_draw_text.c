/* DEC/CMS REPLACEMENT HISTORY, Element GO_DRAW_TEXT.C*/
/* *8    12-JUN-1991 11:42:20 KEE "(SPR 7094) Fix bug of display graphic object overlay, case 1308"*/
/* *7    30-JAN-1991 10:38:23 MING "(SPR -1) fix the former change that screw up user defined options in mapdef"*/
/* *6     4-JAN-1991 14:03:32 JESSIE "(SPR 6397) get text high from TEXT struc, if it is < 0 then get it from default"*/
/* *5    18-JUL-1990 11:54:50 GILLESPIE "(SPR 1) Change call to ct_get_text"*/
/* *4    12-JUN-1990 10:20:45 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit"*/
/* *3    13-FEB-1990 11:11:29 GORDON "(SPR 0) fix placement of call to tc_free"*/
/* *2     1-FEB-1990 11:33:39 GORDON "(SPR 0) Add call to free the text which was allocated by ct_get_text"*/
/* *1    19-JUN-1989 12:53:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GO_DRAW_TEXT.C*/

/*
 * FILE: GO_DRAW_TEXT.C
 */

/* include files */
#include "esi_go.h"
#include "esi_mm.h"
#include "esi_ov.h"
#include "esi_ct.h"
#include "esi_tc.h"
#include "esi_wm_cls.h"

/************************************************************/

#if USE_PROTOTYPES
publicdef INT go_draw_text(GO_STRUCT *values, INT element_id,
			   BOOL clip_required, INT *segment_id)
#else
publicdef INT go_draw_text(values,element_id,clip_required, segment_id)
GO_STRUCT *values;
INT element_id;
BOOL clip_required;
INT *segment_id;
#endif
{
INT status;
TEXT text;
INT segment;

/***************************************************************************/

status = ct_get_text( &text, element_id);
if (status EQUALS SUCCESS)
    {
    if (values->text_color != -1 )
	{
	text.text_color = values->text_color;
	}
    if ( values->text_size != 300.0 || text.text_size <= 0.0)
        {
	text.text_size = values->text_size;
	}
    if ( values->text_font != -999)
        {
	text.text_font = values->text_font;
	}
    if (values->text_precision >= 0 )
	{
	text.text_precision = values->text_precision;
	}
    if ( values->text_expansion_factor >= 0.0 )
	{
	text.text_expansion_factor = values->text_expansion_factor;
	}

    if (NOT values->suppress_editing)
	{
	ov_new_element(&segment,WM_CLASS_TEXT,"",element_id );
	*segment_id = segment;
	}							

    mm_draw_text(&text,clip_required);

    if (NOT values->suppress_editing)
	{
	wmsclose(segment);
	}
    tc_free( (VOIDPTR)text.text_string );
    }
    

    return status;
}
