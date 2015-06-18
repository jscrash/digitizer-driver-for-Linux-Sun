/* DEC/CMS REPLACEMENT HISTORY, Element TG_REGISTER.C */
/* *3    17-AUG-1990 22:27:08 VINCE "(SPR 5644) Code Cleanup" */
/* *2    27-NOV-1989 14:49:56 JULIAN "(SPR 1) General tablet reg. user interface" */
/* *1    27-NOV-1989 14:48:22 JULIAN "General tablet registration user interface." */
/* DEC/CMS REPLACEMENT HISTORY, Element TG_REGISTER.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"
#include "esi_am.h"
#include "esi_ts.h"
#include "esi_tg_mat.h"
#include "esi_tb_defs.h"
#include "esi_tc.h"
#include "esi_wi.h"
#include "esi_wm.h"
#include "esi_wm_int.h"
#include "tg_register.rh"
#include "esi_tg_err.h"
#include "esi_tg_msg.h"
static VOID zz_set_ul ();
static VOID zz_set_ll ();
static VOID zz_set_lr ();
static INT zz_get_coord ();
static VOID zz_msg2 ();
static VOID zz_msg1 ();
static INT tg_reg_srvr ();
#define TG_LABEL_SIZE 12
#define TGARROWSTR "<--"
#define UL 1
#define LL 2
#define LR 3
static INT zz_set_curr ();
static INT zz_do_digitize ();
static INT uldone = FALSE, lldone = FALSE, lrdone = FALSE;

/* File Description ---------------------------------------------------------
Overview:
    Generalized registration dialog. 

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tg_register(INT curtnr,CHAR *xlabel,CHAR *ylabel,EXTENTS *wcsext);

Private_Functions:
    static INT zz_set_curr(INT id,INT cur);
    
    static INT zz_do_digitize(INT id,INT current,EXTENTS *wcsext,EXTENTS *tabext);
    
    static VOID zz_set_ul(INT id,INT onoff);
    
    static VOID zz_set_ll(INT id,INT onoff);
    
    static VOID zz_set_lr(INT id,INT onoff);
    
    static INT zz_get_coord(INT *x,INT *y,INT *b,INT done_btn);
    
    static VOID zz_msg2(INT id,CHAR *str);
    
    static VOID zz_msg1(INT id,CHAR *str);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Implements the generalized registration dialog user interface.
    This routine allows users register the tablet by requesting the
    world coordinates of three corners of the target media, and then
    digitizing the corresponding tablet points for the above three corners.
    After the points have been gathered, a transformation is created
    that maps the given tablet points to the given world coords.
 
    Note that after calling this routine, the calculated transformation
    is set into the window manager such that all input operations 
    requested while the transformation is selected, will be performed
    with the tablet.  To switch control back to the mouse (default)
    a different window manager transformation must be selected.
 
    Of course, to switch input back to the tablet, select the window
    manager transformation that was active during registration.
 
Prototype:
    publicdef INT tg_register(INT curtnr,CHAR *xlabel,CHAR *ylabel,EXTENTS *wcsext);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) curtnr      -(INT) The number (id) of the current WM tnr.
    (I) xlabel      -(CHAR *) X units string. (eg. 'X' or 'CDP'  etc.)
    (I) ylabel      -(CHAR *) Y units string. (eg. 'Y' or 'TIME' etc.)
    (O) extents     -(EXTENTS *) Extents struct. of users World Coords.

Return Value/Status:
    SUCCESS - Successful completion.
    CANCEL  -   Registration was cancelled.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_register (INT curtnr, CHAR *xlabel, CHAR *ylabel, EXTENTS *wcsext)
#else
publicdef INT tg_register (curtnr, xlabel, ylabel, wcsext)
INT curtnr;
CHAR *xlabel, *ylabel;
EXTENTS *wcsext;
#endif
    {
    size_t len = 0;
    static EXTENTS tabraw, *tabext = &tabraw;
    
    CHAR xlbl[14], ylbl[14], instr[128], text[255];
    
    INT event_class, event_id, event_item, status, id, alldone, pickbtn, current,
        tnrsave;
    POINT wcs_points[4];            /* users WCS control points to gpx_wmtregistration   */
    POINT tab_points[4];            /* users TABLET control points to gpx_wmtregistration   */
    INT appCur, winCur, tnrCur, groCur, segCur, picCur;
    
    wmoinqcur (&appCur, &winCur, &tnrsave, &groCur, &segCur, &picCur);
    wmtselect (curtnr);
    current = UL;
    uldone = FALSE;
    lldone = FALSE;
    lrdone = FALSE;
    tc_zeroes (tabext, (sizeof(EXTENTS)));  /* Clear out static struct. */
    pickbtn = 1;                    /* 1 key on puck used for digitizing. */
    alldone = FALSE;
    status = CANCEL;                /* SUCCESS only when all registered. */
                                    /* Check the x,y axis labels passed by caller.  */
    /* if none given use default strings "X","Y"    */
    if (xlabel == (CHAR *)0)
        strcpy (xlbl, " X ");
    else
        {
        len = MIN (TG_LABEL_SIZE, strlen (xlabel));
        strncpy (xlbl, xlabel, len);
        xlbl[len] = NUL;
        }
    if (ylabel == (CHAR *)0)
        strcpy (ylbl, " Y ");
    else
        {
        len = MIN (TG_LABEL_SIZE, strlen (ylabel));
        strncpy (ylbl, ylabel, len);
        ylbl[len] = NUL;
        }
    wmtregonoff (OFF);
    
    fi_push ();
    /* Create the control dialog box. */
    status = am_define_resource (AM_DIALOG, "TG_REGISTER", TG_REG_CDLG,
                                 NULL_FUNCTION_PTR, &id, NULL);
    
    /* Setup dialog items - Initial state upon dialog open. */
    wiencdi (id, TG_BTN_DONE, OFF);
    wiencdi (id, TG_BTN_BACKUP, OFF);
    wiencdi (id, TG_BTN_CANCEL, ON);
    zz_set_ul (ON);
    zz_set_ll (OFF);
    zz_set_lr (OFF);
    
    wiscdt (id, TG_ST_XLABEL, xlbl);
    wiscdt (id, TG_ST_YLABEL, ylbl);
    wiscdt (id, TG_ET_ULX, "");
    wiscdt (id, TG_ET_ULY, "");
    wiscdt (id, TG_ET_LLX, "");
    wiscdt (id, TG_ET_LLY, "");
    wiscdt (id, TG_ET_LRX, "");
    wiscdt (id, TG_ET_LRY, "");
    zz_msg1 (id, "");
    
    /* open the control dialog */
    wiopcd (id);
    
    current = UL;
    while (alldone == FALSE)
        {
        zz_set_curr (id, current);
        if ((uldone == TRUE) AND (lldone == TRUE) AND (lrdone == TRUE))
            wiencdi (id, TG_BTN_DONE, ON);
        
        if (current > UL)
            wiencdi (id, TG_BTN_BACKUP, ON);
        else
            wiencdi (id, TG_BTN_BACKUP, OFF);
        switch (current)
            {
        case UL:
            sprintf (text, "Enter the upper left %s,%s coordinate values", xlbl, ylbl);
            zz_msg2 (id, text);
            break;
            
        case LL:
            sprintf (text, "Enter the lower left %s,%s coordinate values", xlbl, ylbl);
            zz_msg2 (id, text);
            break;
            
        case LR:
            sprintf (text, "Enter the lower right %s,%s coordinate values", xlbl,
                     ylbl);
            zz_msg2 (id, text);
            break;
            }
        wi_request_event (0, &event_class, &event_id, &event_item);
        if (event_id != id)
            wibeep (2);
        else
            switch (event_item)
            {
        case TG_BTN_DIGITIZE:
            status = zz_do_digitize (id, current, wcsext, tabext);
            if (status == SUCCESS)
                {
                current = current + 1;
                if (current > LR)
                    current = UL;
                }
            break;
            
        case TG_BTN_BACKUP:
            if (current > UL)
                current--;
            zz_set_curr (id, current);
            break;
            
        case TG_BTN_CANCEL:
            status = CANCEL;
            break;
            
        case TG_BTN_DONE:
            status = SUCCESS;
            alldone = TRUE;
            break;
            
        default:
            break;
            }
        if (status == CANCEL)
            {
            wmtselect (tnrsave);
            break;
            }
        }
    /* end of while */
    
    if (alldone == TRUE)
        {
        wcs_points[0].x = wcsext->ul_x;
        wcs_points[0].y = wcsext->ul_y;
        wcs_points[1].x = wcsext->ll_x;
        wcs_points[1].y = wcsext->ll_y;
        wcs_points[2].x = wcsext->lr_x;
        wcs_points[2].y = wcsext->lr_y;
        
        tab_points[0].x = tabext->ul_x;
        tab_points[0].y = tabext->ul_y;
        tab_points[1].x = tabext->ll_x;
        tab_points[1].y = tabext->ll_y;
        tab_points[2].x = tabext->lr_x;
        tab_points[2].y = tabext->lr_y;
        gpx_wmtregistration (curtnr, wcs_points, tab_points, 3);
        }
    am_release_resource (id);
    fi_pop ();
    wmtregonoff (ON);
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_set_curr(INT id,INT cur);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    cur             -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_set_curr (INT id, INT cur)
#else
static INT zz_set_curr (id, cur)
INT id, cur;
#endif
    {
    switch (cur)
        {
    case UL:
        zz_set_ll (id, OFF);
        zz_set_lr (id, OFF);
        zz_set_ul (id, ON);
        break;
        
    case LL:
        zz_set_ll (id, ON);
        zz_set_lr (id, OFF);
        zz_set_ul (id, OFF);
        break;
        
    case LR:
        zz_set_ll (id, OFF);
        zz_set_lr (id, ON);
        zz_set_ul (id, OFF);
        break;
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_do_digitize(INT id,INT current,EXTENTS *wcsext,EXTENTS *tabext);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    current         -(INT)
    wcsext          -(EXTENTS *)
    tabext          -(EXTENTS *)

Return Value/Status:
    SUCCESS - Successful completion.
    TG_NO_WCS
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_do_digitize (INT id, INT current, EXTENTS *wcsext, EXTENTS *tabext)
#else
static INT zz_do_digitize (id, current, wcsext, tabext)
INT id, current;
EXTENTS *wcsext, *tabext;
#endif
    {
    
    INT status = SUCCESS;
    CHAR str1[64];
    CHAR str2[64];
    INT pickbtn = 1;
    INT unused, ix, iy, ibtn;
    CHAR text[256];
    
    switch (current)
        {
    case UL:
        wiqccdi (id, TG_ET_ULX, &unused, str1);
        wiqccdi (id, TG_ET_ULY, &unused, str2);
        if (strlen (str1) == 0 OR (strlen (str2) == 0))
            {
            zz_msg2 (id, "You must supply both Upper left world values first.");
            status = TG_NO_WCS;
            break;
            }
        else
            {
            sscanf (str1, "%lf", &wcsext->ul_x);
            sscanf (str2, "%lf", &wcsext->ul_y);
            }
        zz_msg2 (id, "Digitize the upper left registration point now...");
        zz_get_coord (&ix, &iy, &ibtn, pickbtn);
        zz_msg2 (id, "");
        tabext->ul_x = (DOUBLE)ix;
        tabext->ul_y = (DOUBLE)iy;
        uldone = TRUE;
        break;
        
    case LL:
        wiqccdi (id, TG_ET_LLX, &unused, str1);
        wiqccdi (id, TG_ET_LLY, &unused, str2);
        if (strlen (str1) == 0 OR (strlen (str2) == 0))
            {
            zz_msg2 (id, "You must supply both Upper left world values first.");
            status = TG_NO_WCS;
            break;
            }
        else
            {
            sscanf (str1, "%lf", &wcsext->ll_x);
            sscanf (str2, "%lf", &wcsext->ll_y);
            }
        zz_msg2 (id, "Digitize the lower left registration point now...");
        zz_get_coord (&ix, &iy, &ibtn, pickbtn);
        zz_msg2 (id, "");
        tabext->ll_x = (DOUBLE)ix;
        tabext->ll_y = (DOUBLE)iy;
        lldone = TRUE;
        break;
        
    case LR:
        wiqccdi (id, TG_ET_LRX, &unused, str1);
        wiqccdi (id, TG_ET_LRY, &unused, str2);
        if (strlen (str1) == 0 OR (strlen (str2) == 0))
            {
            zz_msg2 (id, "You must supply both Upper left world values first.");
            status = TG_NO_WCS;
            break;
            }
        else
            {
            sscanf (str1, "%lf", &wcsext->lr_x);
            sscanf (str2, "%lf", &wcsext->lr_y);
            }
        zz_msg2 (id, "Digitize the lower right registration point now...");
        zz_get_coord (&ix, &iy, &ibtn, pickbtn);
        zz_msg2 (id, "");
        tabext->lr_x = (DOUBLE)ix;
        tabext->lr_y = (DOUBLE)iy;
        lrdone = TRUE;
        break;
        }
    sprintf (text, "TABLET: X=%d, Y=%d, Button=%d\n", ix, iy, ibtn);
    zz_msg1 (id, text);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID zz_set_ul(INT id,INT onoff);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    onoff           -(INT)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID zz_set_ul (INT id, INT onoff)
#else
static VOID zz_set_ul (id, onoff)
INT id;
INT onoff;
#endif
    {
    
    wiencdi (id, TG_ET_ULX, onoff);
    wiencdi (id, TG_ET_ULY, onoff);
    wiencdi (id, TG_ST_KEY_UL, onoff);
    if (onoff)
        wiscdt (id, TG_ST_FLAG_UL, TGARROWSTR);
    else
        wiscdt (id, TG_ST_FLAG_UL, "");
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID zz_set_ll(INT id,INT onoff);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    onoff           -(INT)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID zz_set_ll (INT id, INT onoff)
#else
static VOID zz_set_ll (id, onoff)
INT id;
INT onoff;
#endif
    {
    
    wiencdi (id, TG_ET_LLX, onoff);
    wiencdi (id, TG_ET_LLY, onoff);
    wiencdi (id, TG_ST_KEY_LL, onoff);
    if (onoff)
        wiscdt (id, TG_ST_FLAG_LL, TGARROWSTR);
    else
        wiscdt (id, TG_ST_FLAG_LL, "");
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID zz_set_lr(INT id,INT onoff);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    onoff           -(INT)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID zz_set_lr (INT id, INT onoff)
#else
static VOID zz_set_lr (id, onoff)
INT id;
INT onoff;
#endif
    {
    
    wiencdi (id, TG_ET_LRX, onoff);
    wiencdi (id, TG_ET_LRY, onoff);
    wiencdi (id, TG_ST_KEY_LR, onoff);
    if (onoff)
        wiscdt (id, TG_ST_FLAG_LR, TGARROWSTR);
    else
        wiscdt (id, TG_ST_FLAG_LR, "");
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT zz_get_coord(INT *x,INT *y,INT *b,INT done_btn);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(INT *)
    y               -(INT *)
    b               -(INT *)
    done_btn        -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    CANCEL
    TB_NOT_INIT
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT zz_get_coord (INT *x, INT *y, INT *b, INT done_btn)
#else
static INT zz_get_coord (x, y, b, done_btn)
INT *x, *y, *b;
INT done_btn;
#endif
    {
    INT status = SUCCESS;
    INT cnt = 0;
    INT tx, ty, tb;
    
    tb = 0;
    tbh_clear_buffer ();
    while (cnt == 0 OR (tb != done_btn))
        {
        
        status = tbh_get_points (1, &tx, &ty, &tb, &cnt);
        
        if (status == TB_NOT_INIT OR (tb == 12))
            break;
        }
    if (tb == WM_BUTTON_CANCEL)
        status = CANCEL;
    else if (status != TB_NOT_INIT)
        {
        *x = tx;
        *y = ty;
        *b = tb;
        status = SUCCESS;
        wibeep (1);
        }
    else
        {
        *x = 0;
        *y = 0;
        *b = 0;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID zz_msg2(INT id,CHAR *str);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    str             -(CHAR *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID zz_msg2 (INT id, CHAR *str)
#else
static VOID zz_msg2 (id, str)
INT id;
CHAR *str;
#endif
    {
    
    wiscdt (id, TG_ST_MSG2, str);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID zz_msg1(INT id,CHAR *str);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    str             -(CHAR *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <tg_register.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID zz_msg1 (INT id, CHAR *str)
#else
static VOID zz_msg1 (id, str)
INT id;
CHAR *str;
#endif
    {
    
    wiscdt (id, TG_ST_MSG1, str);
    }
/* END:     */
