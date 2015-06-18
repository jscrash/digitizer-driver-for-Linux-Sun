/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_APP.C */
/* *1    13-NOV-1990 11:27:06 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_APP.C */
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_APP.C */
/* *1     6-MAR-1990 08:29:50 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_APP.C */

/*
 * $Header: /j/finder/maint/RCS/tm_test_app.c,v 1.1 2008/02/09 09:23:37 julian Exp julian $
 * $Locker: julian $
 * $Date: 2008/02/09 09:23:37 $
 * $Log: tm_test_app.c,v $
 * Revision 1.1  2008/02/09 09:23:37  julian
 * update
 *
 * Revision 1.3  87/01/26  15:55:53  billj
 * Change entry point name to more popular "testit".
 * 
 * Revision 1.2  87/01/18  15:22:30  gillespie
 * VAXinated
 * 
 * Revision 1.1  86/12/12  17:54:09  edb
 * Initial revision
 * 
 */

static char RCSid[] = "$Header: /j/finder/maint/RCS/tm_test_app.c,v 1.1 2008/02/09 09:23:37 julian Exp julian $";


/* ************************************************************************

   NAME:    TM_TEST_APPLICATION

   AUTHOR:  W. A. Jaeck, 11-Aug-86

   DESCRIPTION:

    This test application is intended to be replaced by your test application
    in a standalone version of Finder.

   ************************************************************************ */

#include "esi_gl_defs.h"
#include "esi_am.h"
#include "esi_dt.h"
#include "esi_mg.h"
#include "esi_tm_msg.h"

/* ********************************************************************** */
INT testit(pointer)
/* ********************************************************************** */

BYTE *pointer;                    /* not used */
{
    PROGNAME("TESTIT");
/* ********************************************************************** */

    DT_ENTER(progname);

    am_declare(progname);
    am_message(AM_STATUS,mg(TM_TEST_APP_NONE_AVAIL));
    am_quit();

    DT_RETURN(progname);
    return SUCCESS;
}
