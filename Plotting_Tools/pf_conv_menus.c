
/* -- pf_conv_menus.c -- */

/* This file was produced by the FINDER_MENU_MAKER program; and it  */
/* defines the FINDER_MENUS array of FINDER_MENU_STRUCT structures, */
/* the contents of which drive the Software Bus.                    */


#include "esi_fi_int.h"


/* EXTERNAL functions referenced by elements of array FINDER_MENUS. */

extern INT pf_mp_conv_util ();
extern INT pf_xs_conv_util ();
extern INT finder_quit ();


/* The following array describes the menu-items of FINDER, and will */
/* determine the operation of the Software Bus.                     */

publicdef FINDER_MENU_STRUCT finder_menus[6] = 
   {     /* start of array of "menu-item" structures */
      { 1, 0, 0, "Convert Plot File", MenuName, (INT (*)()) 0, 0, "", "", (CHAR *) 0},
      { 1, 0, 0, "Map", EventsFunction, pf_mp_conv_util, 0, "", "Map", (CHAR *) 0},
      { 1, 0, 0, "Cross Section     ", EventsFunction, pf_xs_conv_util, 0, "", "Cross Section", (CHAR *) 0},
      { 1, 0, 0, "(-", Undefined, (INT (*)()) 0, 0, "", "", (CHAR *) 0},
      { 1, 0, 0, "Quit", EventsFunction, finder_quit, 0, "", "Quit", (CHAR *) 0},
      { -1, 0, 0, "", Undefined, (INT (*)()) 0, 0, "", "", (CHAR *) 0}   /* an empty "menu-item" ends the list */
   };    /* end of array of "menu-item" structures */

