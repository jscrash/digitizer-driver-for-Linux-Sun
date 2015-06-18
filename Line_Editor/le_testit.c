/* DEC/CMS REPLACEMENT HISTORY, Element LE_TESTIT.C */
/* *1    13-NOV-1990 11:26:04 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element LE_TESTIT.C */
/* DEC/CMS REPLACEMENT HISTORY, Element LE_TESTIT.C */
/* *1     6-MAR-1990 08:28:35 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element LE_TESTIT.C */
#include "esi_le_nled.h"
#include "esi_wm.h"
#include "esi_am.h"

/*      =========   LE_TESTIT  ==========    */

/* Test of n-List editor - does VERY little work */

publicdef INT le_testit ( pointer ) 
VOIDPTR pointer ;

{
    INT status;
    UINT ndim = 2;
    UINT blocksize = 20;
    FLOAT nxmin = 0.25, nxmax = 0.75, nymin = 0.25, nymax = 0.75 ;
    FLOAT vxmin = 0.0, vxmax = 1.0, vymin = 0.0, vymax = 1.0 ;
    FLOAT wxmin = 40.0, wxmax = 110.0, wymin = 2000.0, wymax = 8900.0 ;
    WINDOW_ID win_id;
    TNR_ID tnr_id ;
    privatedef size_t DataSize[] = {sizeof (FLOAT), sizeof (FLOAT)};
    privatedef INT DataType[] = {FLOAT_TYPE, FLOAT_TYPE};
    privatedef INT ValueInc[] = {VALUE_LIST, VALUE_LIST};
    privatedef INT Units[] = {0, 0};
    LE_PARLIST le_struct;

    status = wmwallocate( &win_id, nxmin, nxmax, nymin, nymax, DARK_GREY ) ;
    status = wmtallocate( &tnr_id, vxmin, vxmax, vymin, vymax ,
                             wxmin, wxmax, wymin, wymax ) ;

    tc_zeroes(&le_struct, sizeof (LE_PARLIST));
    le_struct.draw_color = BLUE ;
    le_struct.erase_color = DARK_GREY ;
    le_struct.hilight_color = ORANGE ;
    le_struct.line_style   =   1 ;
    le_struct.line_width =  1.0 ;
    le_struct.initkeymap= TRUE;
    le_struct.keymap_first = TRUE;
    le_struct.key_swap = TRUE ;
    le_struct.only_app_mac_dialog = FALSE ;
    strcpy(le_struct.title, "n-List Editor");
    status  = nl_init_nlist( &le_struct.nlist, ndim, Units, DataSize, DataType, 
                               ValueInc, blocksize);

	 /*      call the nlist editor on an empty nlist   */ 

    status = le_nlist_editor( &le_struct);

    status = nl_free_nlist( le_struct.nlist ) ;

    status = wmwclose( win_id ) ;

    am_quit ();
}
