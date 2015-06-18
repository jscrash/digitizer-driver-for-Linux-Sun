/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_DEF.C*/
/* *3    17-AUG-1990 22:28:38 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-JUN-1989 11:04:59 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:33:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_DEF.C*/
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

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#ifndef ESI_FI_INT_H
#include "esi_fi_int.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TM_MSG_H
#include "esi_tm_msg.h"
#endif

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#include "am_test.rh"
#include "esi_gks.h"

typedef struct 
    {
    INT update_current;
    INT update_default;
    INT saved_defmode;
    INT saved_regmode;
    INT id;
    INT cdlg_active;
    INT irg_pending;
    INT new_defmode;
    INT new_regmode;
    INT irg_flag;
    } TM_TEST_DEF_WS;
static INT tm_test_def_cdlg_init ();
static VOID msg ();
static INT tm_test_def_srvr ();

#if USE_PROTOTYPES
static tm_test_def_srvr(INT id,INT item,TM_TEST_DEF_WS *r);
static VOID msg(INT id,CHAR *str);
static INT tm_test_def_cdlg_init(TM_TEST_DEF_WS *r);
#else
static VOID msg ();
static INT tm_test_def_srvr ();
static INT tm_test_def_cdlg_init ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tm_test_deferral(BYTE *pointer);

Private_Functions:
    static tm_test_def_srvr(INT id,INT item,TM_TEST_DEF_WS *r);
    
    static VOID msg(INT id,CHAR *str);
    
    static INT tm_test_def_cdlg_init(TM_TEST_DEF_WS *r);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT tm_test_deferral(BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tm_test_deferral (BYTE *pointer)
#else
publicdef INT tm_test_deferral (pointer)
BYTE *pointer;
#endif
    {
    INT status;                     /* Completion status */
    TM_TEST_DEF_WS *r;
    /*  Declare program name. */
    
    am_declare ("TEST DEFERRAL");
    
    /*  Create local workspace. */
    status = am_define_workspace ("TM_TEST_DEF_WS", AM_APPLICATION,
                                  sizeof(TM_TEST_DEF_WS), (VOIDPTR *) & r);
    
    /*  Define application control dialog. */
    
    status = am_define_resource (AM_DIALOG, "AM_TEST", TM_DEF_CTRL, tm_test_def_srvr,
                                 &r->id, r);
    
    tm_test_def_cdlg_init (r);
    
    /*  Open dialog. */
    
    wi_open_dialog (r->id);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static tm_test_def_srvr(INT id,INT item,TM_TEST_DEF_WS *r);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    r               -(TM_TEST_DEF_WS *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <tm_test_def.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tm_test_def_srvr (INT id, INT item, TM_TEST_DEF_WS *r)
#else
static INT tm_test_def_srvr (id, item, r)
INT id;
INT item;
TM_TEST_DEF_WS *r;
#endif
    {
    INT status, changed, done;
    CHAR cbuf[256];
    
    status = SUCCESS;
    changed = FALSE;
    done = FALSE;
    
    switch (item)
        {
        
    case TM_DEF_RB_ASAP:
        r->new_defmode = GASAP;
        changed = TRUE;
        break;
        
    case TM_DEF_RB_BNIG:
        r->new_defmode = GBNIG;
        changed = TRUE;
        break;
        
    case TM_DEF_RB_BNIL:
        r->new_defmode = GBNIL;
        changed = TRUE;
        break;
        
    case TM_DEF_RB_ASTI:
        r->new_defmode = GASTI;
        changed = TRUE;
        break;
        
    case TM_DEF_RB_SUPPRESS:
        r->new_regmode = GSUPPD;
        changed = TRUE;
        break;
        
    case TM_DEF_RB_ALLOW:
        r->new_regmode = GALLOW;
        changed = TRUE;
        break;
        
    case TM_DEF_CHK_CURRENT:
        wiqccdi (r->id, TM_DEF_CHK_CURRENT, &r->update_current, cbuf);
        break;
        
    case TM_DEF_CHK_DEFAULT:
        wiqccdi (r->id, TM_DEF_CHK_DEFAULT, &r->update_default, cbuf);
        break;
        
    case TM_DEF_BTN_UPDATE:
        msg (r->id, "Displaying deferred output only. (GUWK w/POSTPONE)");
        wm_update ();
        break;
        
    case TM_DEF_BTN_REDRAW:
        msg (r->id, "Redisplaying all workstation segments. (GRSGWK)");
        wm_grsgwk (MAIN_WKID);
        break;
        
    case TM_DEF_BTN_CANCEL:
        msg (r->id, "Deferral modes not changed.");
        changed = FALSE;
        done = TRUE;
        r->cdlg_active = FALSE;
        break;
        
    case TM_DEF_BTN_DONE:
        
        r->cdlg_active = FALSE;
        done = TRUE;
        break;
        
        }
    if (changed)
        {
        if (r->update_current == TRUE)
            {
            wm_set_deferral (r->new_defmode, r->new_regmode);
            }
        if (r->update_default == TRUE)
            {
            wm_set_default_deferral (r->new_defmode, r->new_regmode);
            }
        if (r->update_current OR (r->update_default))
            msg (r->id, "Deferral modes updated.");
        }
    if (done)
        am_quit ();
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID msg(INT id,CHAR *str);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    str             -(CHAR *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <tm_test_def.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID msg (INT id, CHAR *str)
#else
static VOID msg (id, str)
INT id;
CHAR *str;
#endif
    {
    
    wiscdt (id, TM_DEF_EDT_MSG, str);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT tm_test_def_cdlg_init(TM_TEST_DEF_WS *r);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    r               -(TM_TEST_DEF_WS *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <tm_test_def.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tm_test_def_cdlg_init (TM_TEST_DEF_WS *r)
#else
static INT tm_test_def_cdlg_init (r)
TM_TEST_DEF_WS *r;
#endif
    {
    INT status = SUCCESS;
    INT def, reg, surface, newframe, err;
    
    CHAR text[255];
    
    r->update_current = FALSE;
    r->update_default = FALSE;
    
    wm_inq_deferral (&r->saved_defmode, &r->saved_regmode);
    r->new_defmode = r->saved_defmode;
    r->new_regmode = r->saved_regmode;
    
    switch (r->saved_defmode)
        {
    case GASAP:
        wiscdv (r->id, TM_DEF_RB_ASAP, 1);
        msg (r->id, "Current defer mode = ASAP");
        break;
    case GBNIG:
        wiscdv (r->id, TM_DEF_RB_BNIG, 1);
        msg (r->id, "Current defer mode = BNIG");
        break;
    case GBNIL:
        wiscdv (r->id, TM_DEF_RB_BNIL, 1);
        msg (r->id, "Current defer mode = BNIL");
        break;
    case GASTI:
        wiscdv (r->id, TM_DEF_RB_ASTI, 1);
        msg (r->id, "Current defer mode = ASTI");
        break;
    default:
        /* no deferral as yet. */
        wiscdv (r->id, TM_DEF_RB_ASAP, 0);
        wiscdv (r->id, TM_DEF_RB_BNIG, 0);
        wiscdv (r->id, TM_DEF_RB_BNIL, 0);
        wiscdv (r->id, TM_DEF_RB_ASTI, 0);
        }
    switch (r->saved_regmode)
        {
    case GALLOW:
        wiscdv (r->id, TM_DEF_RB_ALLOW, 1);
        break;
        
    case GSUPPD:
        wiscdv (r->id, TM_DEF_RB_SUPPRESS, 1);
        break;
    default:
        wiscdv (r->id, TM_DEF_RB_ALLOW, 0);
        wiscdv (r->id, TM_DEF_RB_SUPPRESS, 0);
        break;
        }
    wiscdv (r->id, TM_DEF_CHK_CURRENT, ((r->update_current) ? TRUE : FALSE));
    wiscdv (r->id, TM_DEF_CHK_DEFAULT, ((r->update_default) ? TRUE : FALSE));
    
    wm_gqwkdu (MAIN_WKID, &err, &def, &reg, &surface, &newframe);
    sprintf (text, "Defmode: %d   IRG_Flag: %d  Surface: %d  Newframe: %d\n", def, reg,
             surface, newframe);
    msg (r->id, text);
    
    return status;
    }
/* END:     */
