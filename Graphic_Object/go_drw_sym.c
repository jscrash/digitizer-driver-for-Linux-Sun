/* DEC/CMS REPLACEMENT HISTORY, Element GO_DRW_SYM.C*/
/* *2    18-JUL-1990 11:58:23 GILLESPIE "(SPR 1) Change calling args to ct_get_symbol"*/
/* *1    19-JUN-1989 12:53:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GO_DRW_SYM.C*/

/*
 * FILE: GO_DRAW_SYMBOL.C
 */

/* include files */
#include "esi_go.h"
#include "esi_mm.h"
#include "esi_ct.h"
#include "esi_ov.h"
#include "esi_wm.h"
#include "esi_wm_cls.h"

/************************************************************/

#if USE_PROTOTYPES
publicdef INT go_draw_symbol(GO_STRUCT *values, INT element_id)
#else
publicdef INT go_draw_symbol(values,element_id)
GO_STRUCT *values;
INT element_id;
#endif
{
INT status;
SYMBOL symbol;
INT segment;

/***************************************************************************/

status = ct_get_symbol( &symbol, element_id);
if ( status EQUALS SUCCESS )
    {
    if (NOT values->suppress_editing )
	{
	ov_new_element(&segment, WM_CLASS_SYMBOL, "", element_id );
	}		

    mm_draw_symbol(&symbol);

    if (NOT values->suppress_editing)
	{
	wmsclose(segment);
	}
    }

return status; 
}
