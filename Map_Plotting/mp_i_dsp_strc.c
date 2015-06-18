/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_DSP_STRC.C*/
/*  *5     6-NOV-1990 11:26:16 JULIAN "(SPR 5719) fix crashes when adding more than 27 overlays"*/
/*  *4    17-AUG-1990 22:06:17 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3     7-FEB-1990 15:13:53 WALTERS "(SPR 0) replace ts_tcp_alloc"*/
/*  *2     6-FEB-1990 15:39:40 WALTERS "(SPR 0) Fix ts_tcp_copy"*/
/*  *1    19-JUN-1989 13:12:34 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_DSP_STRC.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_md_defs.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to allocate and initialize the display structure.
    Size is determined by "rownum".

Prototype:
    publicdef INT mp_init_display_struct(MD_DISPLAY **display,INT rownum);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display         -(MD_DISPLAY **)
    rownum          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_init_display_struct (MD_DISPLAY **display, INT rownum)
#else
publicdef INT mp_init_display_struct (display, rownum)
MD_DISPLAY **display;
INT rownum;
#endif
    {
    
    PROGNAME ("MP_CREATE_MAP");
    INT status;
    static BOOL inited = FALSE;
    static INT neg_one[100];
    MD_DISPLAY *temp_display;
    INT i;
    INT j;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * INITIALIZE NEGATIVE ONE ARRAY * */
    /* --------------------------------- */
    
    if (NOT inited)
        {
        for (i = 0; i < 100; i++)
            {
            neg_one[i] = -1;
            }
        inited = TRUE;
        }
    /* * ALLOCATE NEW DISPLAY STRUCTURE * */
    /* ---------------------------------- */
    
    temp_display = (MD_DISPLAY *)am_allocate (AM_GLOBAL, sizeof(MD_DISPLAY));
    
    for (i = 100; i <= rownum; i += 100)
        ;
    
    temp_display->overlay_displayed = (BOOL *)am_allocate (AM_GLOBAL,
                                                           i * sizeof(BOOL));
    temp_display->verbs = (MD_DSP_VERB *)am_allocate (AM_GLOBAL,
                                                      i * sizeof(MD_DSP_VERB));
    temp_display->keywords = (MD_DSP_KEY *)am_allocate (AM_GLOBAL,
                                                        i * sizeof(MD_DSP_KEY));
    
    temp_display->parameters = ts_tcp_alloc (i - 1);
    temp_display->verb_ndx = (INT *)am_allocate (AM_GLOBAL, i * sizeof(INT));
    temp_display->key_ndx = (INT *)am_allocate (AM_GLOBAL, i * sizeof(INT));
    temp_display->compiled_row = (INT *)am_allocate (AM_GLOBAL, i * sizeof(INT));
    
    
    /* * INITIALIZE * */
    /* -------------- */
    
    temp_display->nrows = i - 1;
    tc_zeroes (temp_display->overlay_displayed, i * sizeof(BOOL));
    for (j = 0; j < i; j += 100)
        {
        hoblockmove (neg_one, &(temp_display->verb_ndx[j]), 100 * sizeof(INT));
        hoblockmove (neg_one, &(temp_display->key_ndx[j]), 100 * sizeof(INT));
        hoblockmove (neg_one, &(temp_display->compiled_row[j]), 100 * sizeof(INT));
        }
    *display = temp_display;
    
    return status;
    }
/* END:     */
