/* DEC/CMS REPLACEMENT HISTORY, Element GEN_MENU.C */
/* *1    13-NOV-1990 11:25:54 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element GEN_MENU.C */
/* DEC/CMS REPLACEMENT HISTORY, Element GEN_MENU.C */
/* *1     6-MAR-1990 08:28:28 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element GEN_MENU.C */
#ifdef RCSID
static char *RCSid = 
   "$Header: /j/finder/maint/RCS/gen_menu.c,v 1.1 2008/02/09 09:23:37 julian Exp julian $ gen_menu.c ";
#endif RCSID


/* -- output -- */

/* This file was produced by the FINDER_MENU_MAKER program; and it  */
/* defines the FINDER_MENUS array of FINDER_MENU_STRUCT structures, */
/* the contents of which drive the Software Bus.                    */


#include "esi_fi_int.h"


/* EXTERNAL functions referenced by elements of array FINDER_MENUS. */

extern INT mp_mapping ();
extern INT xs_main ();
extern INT finder_quit ();
extern INT interpret_main ();
extern INT inter_main ();
extern INT syn_synth_ ();
extern INT mo_strat_model ();
extern INT mo_array_ ();
extern INT or_select_form_app ();
extern INT or_select_report_app ();
extern INT or_sql_processor ();
extern INT lg_digitize_main ();
extern INT lg_edit_main ();
extern INT gb_configure_analyst ();
extern INT gb_host_processor  ();
extern INT db_dba_func ();
extern INT ukooa_main ();
extern INT tl_tobin_main ();
extern INT ld_lg_ascii ();


/* The following array describes the menu-items of FINDER, and will */
/* determine the operation of the Software Bus.                     */

privatedef FINDER_MENU_STRUCT finder_menus[35] = 
   {     /* start of array of "menu-item" structures */
      { 1, 0, 0, "Finder", MenuName, (INT (*)()) 0, 0, "", "" },
      { 1, 0, 0, "Mapping", EventsFunction, mp_mapping, 0, "", "Mapping" },
      { 1, 0, 0, "Geologic Interpretation", EventsFunction, xs_main, 0, "", "Cross Section" },
      { 1, 0, 0, "Seismic Interpretation", NextMenu, (INT (*)()) 0, 2, "", "" },
      { 1, 0, 0, "Seismic Modeling", NextMenu, (INT (*)()) 0, 3, "", "" },
      { 1, 0, 0, "Finder Database", NextMenu, (INT (*)()) 0, 4, "", "" },
      { 1, 0, 0, "Utilities", NextMenu, (INT (*)()) 0, 5, "", "" },
      { 1, 0, 0, "Quit Finder", EventsFunction, finder_quit, 0, "", "Quit Finder" },
      { 2, 0, 0, "Seismic", MenuName, (INT (*)()) 0, 0, "", "" },
      { 2, 0, 0, "Start Interpret", EventsFunction, interpret_main, 0, "", "Interpret" },
      { 2, 0, 0, "Calculate Line Intersections", EventsFunction, inter_main, 0, "", "Line Intersection" },
      { 2, 0, 0, "((Section Operations", Undefined, (INT (*)()) 0, 0, "", "" },
      { 3, 0, 0, "Modeling", MenuName, (INT (*)()) 0, 0, "", "" },
      { 3, 0, 0, "Synthetic Seismograms", NoEventsFunction, syn_synth_, 0, "", "Synthetic Seismograms" },
      { 3, 0, 0, "Stratigraphic Modeling", EventsFunction, mo_strat_model, 0, "", "Stratigraphic Modeling" },
      { 3, 0, 0, "Geophone Responses", NoEventsFunction, mo_array_, 0, "", "Geophone Array Response" },
      { 4, 0, 0, "Database", MenuName, (INT (*)()) 0, 0, "", "" },
      { 4, 0, 0, "ORACLE Forms", EventsFunction, or_select_form_app, 0, "", "Select ORACLE Form" },
      { 4, 0, 0, "Reports", EventsFunction, or_select_report_app, 0, "", "Select ORACLE Report" },
      { 4, 0, 0, "(-", Undefined, (INT (*)()) 0, 0, "", "" },
      { 4, 0, 0, "SQL Processor", EventsFunction, or_sql_processor, 0, "", "SQL Processor" },
      { 5, 0, 0, "Utilities", MenuName, (INT (*)()) 0, 0, "", "" },
      { 5, 0, 0, "Loaders", NextMenu, (INT (*)()) 0, 6, "", "" },
      { 5, 0, 0, "Log Digitizer", EventsFunction, lg_digitize_main, 0, "", "Log Trace Digitizer" },
      { 5, 0, 0, "Log Trace Editing", EventsFunction, lg_edit_main, 0, "", "Log Editing" },
      { 5, 0, 0, "Configure Analyst", EventsFunction, gb_configure_analyst, 0, "", "Configure Analyst" },
      { 5, 0, 0, "Host Command Processor", EventsFunction, gb_host_processor , 0, "", "Host Commands" },
      { 5, 0, 0, "DBA Utilities", EventsFunction, db_dba_func, 0, "", "DBA Functions" },
      { 6, 0, 0, "Loaders", MenuName, (INT (*)()) 0, 0, "", "" },
      { 6, 0, 0, "UKOOA", EventsFunction, ukooa_main, 0, "", "UKOOA Loader" },
      { 6, 0, 0, "Tobin Graphic Objects", EventsFunction, tl_tobin_main, 0, "", "Tobin Loader" },
      { 6, 0, 0, "Well Log Trace", EventsFunction, ld_lg_ascii, 0, "", "Log Trace Loader" },
      { 6, 0, 0, "(Scatter", Undefined, (INT (*)()) 0, 0, "", "" },
      { 0, 0, 0, "", Undefined, (INT (*)()) 0, 0, "", "" }   /* an empty "menu-item" ends the list */
   };    /* end of array of "menu-item" structures */


/* The following function will provide its callers with a pointer */
/* to the FINDER_MENUS array.                                     */

publicdef FINDER_MENU_STRUCT *fiz_finder_menus ()

   {
   return finder_menus;
   }

