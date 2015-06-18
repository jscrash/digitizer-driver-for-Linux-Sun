/* DEC/CMS REPLACEMENT HISTORY, Element FIZ_ONUNIT.C*/
/* *6    14-AUG-1990 22:50:16 GILLESPIE "(SPR 5644) Fix problems with last checkin"*/
/* *5    14-AUG-1990 13:12:57 VINCE "(SPR 5644) Header Standardization"*/
/* *4     8-MAR-1990 11:52:32 WALTERS "(SPR 5113) Removed 'Dump file created' message for VMS"*/
/* *3    26-JUN-1989 10:43:11 GILLESPIE "(SPR 20) Fix castings"*/
/* *2    20-JUN-1989 11:15:10 GILLESPIE "(SPR 20) Fix minor casting problem"*/
/* *1    19-JUN-1989 12:49:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FIZ_ONUNIT.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_fi_int.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_ER_H

#include "esi_er.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#include "esi_fi_msg.h"

#ifdef primos
struct onunit_data
    {
    char space[44];
    short *sp;
    };

#endif

/* File Description ---------------------------------------------------------
Overview:
    On units utility.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef void fiz_onunit(CHAR *condition_string);
    
    publicdef VOID prime_onunit(struct onunit_data *p);
    
    publicdef FIV *fiz_static();
    
Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef FIV *fiz_static();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns &fiv.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef FIV *fiz_static () 
#else
publicdef FIV *fiz_static ()
#endif
    {
    static FIV fiv;
    return &fiv;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef VOID prime_onunit(struct onunit_data *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) p           -(struct onunit_data *)
    
Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#ifdef primos
#if USE_PROTOTYPES
publicdef VOID prime_onunit (struct onunit_data *p) 
#else
publicdef VOID prime_onunit (p)
struct onunit_data *p;
#endif
    {
    char *c, condition_name[33];
    
    c = (char *)(p->sp + 1);        /* extract name from Primos structure */
    strncpy (condition_name, c, *p->sp);
    condition_name[*p->sp] = '\0';
    
    fiz_onunit (condition_name);
    return;
    }
/* END:     */
#endif
/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef void fiz_onunit(CHAR *condition_string);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) condition_string    -(CHAR *)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef void fiz_onunit (CHAR *condition_string) 
#else
publicdef void fiz_onunit (condition_string)
CHAR *condition_string;
#endif
    {
    CHAR text[100];
    CHAR error_text[200];
    CHAR dump_file[100];
    INT i;
    FINDER_WORKSPACE *f;

#ifdef primos
    static BOOL inonunit = FALSE;
    
    if (inonunit)
        {
        printf ("YOU ARE IN AN ONUNIT LOOP");
        printf ("condition = %s\n", condition_string);
        ho_show_callers ();
        exit (EXIT_FAILURE);
        }
    inonunit = TRUE;
    
    printf ("Condition \"%s\" has been raised.\n", condition_string);
    ho_show_callers ();
    
    /*   Dump stack. */
    
    ho_dump (dump_file);
    
    /*   Log error. */
    
    sprintf (error_text, mg (FI_ON_UNIT_OCCURRED), condition_string);
    strcat (error_text, "\r\n    ");
    sprintf (text, mg (FI_DUMP_FILE_CREATED), dump_file);
    strcat (error_text, text);
    am_message (AM_STATUS, error_text);
    er_log (error_text);
#endif
    
    /*   Tell user that application will not
         continue. */
    
    am_message (AM_STATUS, mg (FI_ABORTING_APP));
    
    /*   Abort offending application. */
    
    am_abort ();
    
    am_open_workspace ("FINDER", AM_GLOBAL, (VOIDPTR *)(VOIDPTR *) & f);
    
    /*   Unwind pushed environment. */
    
    while (f->fi_push_level > 0)
        fi_pop ();
    
    longjmp (fiz_static ()->event_env, 0);
    }
/* END:     */
