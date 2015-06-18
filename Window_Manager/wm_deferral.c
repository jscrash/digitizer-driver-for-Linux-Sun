/* DEC/CMS REPLACEMENT HISTORY, Element WM_DEFERRAL.C*/
/* *10    3-MAY-1991 16:14:17 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *9    23-OCT-1990 13:58:23 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *8    23-AUG-1990 17:27:06 JESSIE "(SPR 1) fix include files"*/
/* *7    14-AUG-1990 13:44:03 VINCE "(SPR 5644) Header Standardization"*/
/* *6    21-MAY-1990 08:06:14 SCD "(SPR 1) Ensure we don't set deferral when no workstation is active."*/
/* *5    17-MAY-1990 09:54:03 JULIAN "(SPR 0) add return status on wm_inq_deferral"*/
/* *4     2-MAR-1990 11:07:51 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"*/
/* *3     6-FEB-1990 00:11:57 JULIAN "(SPR -1) X Version (Kee)"*/
/* *2    20-JUL-1989 11:06:27 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:40:46 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_DEFERRAL.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_gl_defs.h"
#include "esi_gks.h"

#ifdef USE_X

#include "esi_wi_int.x"

#endif
/*                                                                                  */
/**   GKS DEFERRAL CONTROL                                                          */
/**                                                                                 */
/** Julian Carlisle  Jan 25, 1989                                                   */
/*                                                                                  */
publicdef INT wm_update ();
publicdef INT wm_set_default_deferral ();
publicdef INT wm_inq_deferral ();
publicdef INT wm_set_deferral ();

static INT default_defmod = GASAP;
static INT default_regmod = GSUPPD;

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    GKS Deferral control.                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT wm_set_default_deferral(INT defmod,INT regmod);                 */
/*                                                                                  */
/*    publicdef INT wm_set_default_deferral(INT defmod,INT regmod);                 */
/*                                                                                  */
/*    publicdef INT wm_set_deferral(INT defmod,INT regmod);                         */
/*                                                                                  */
/*    publicdef INT wm_inq_deferral(INT *defmod,INT *regmod);                       */
/*                                                                                  */
/*    publicdef INT wm_update();                                                    */
/*                                                                                  */
/*    publicdef INT wm_make_current();                                              */
/*                                                                                  */
/*    publicdef INT wm_hold_updates();                                              */
/*                                                                                  */
/*Private_Functions:                                                                */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Tell GKS not to refresh the screen implicitly after each segment.             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_hold_updates();                                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_hold_updates ()
#else
publicdef INT wm_hold_updates ()
#endif
    {
    INT deferral_mode, irg_mode;
    
    deferral_mode = default_defmod;
    irg_mode = default_regmod;
    
    if (ug_if_gpx () == TRUE)
        wm_set_deferral (default_defmod, default_regmod);
#ifdef SUN_GKS_OR_PRIOR_GKS
    wm_gsdyns (MAIN_WKID, G_DYN_OFF);
#endif
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Tell GKS to send all deferred changes to the screen. And stop deferring.      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_make_current();                                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_make_current (void)
#else
publicdef INT wm_make_current ()
#endif
    {
    INT defmod, regmod, update_mode;
    
    if (ug_if_gpx () == TRUE)
        {
        wm_update ();
        wm_set_deferral (default_defmod, default_regmod);
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Update deferred changes.                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_update();                                                    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_update (void)
#else
publicdef INT wm_update ()
#endif
    {
    INT defmod, regmod, ier, surface, newframe;
    
    if (ug_if_gpx () == TRUE)
        {
        /* Check the current values for deferral mode & implicit                    */
        /* regeneration mode in the state list.                                     */
        wm_gqwkdu (MAIN_WKID, &ier, &defmod, &regmod, &surface, &newframe);
        
        if (newframe == GYES)
            wm_guwk (MAIN_WKID, GPERFO);    /* Regenerate screen display            */
        else
            wm_guwk (MAIN_WKID, GPOSTP);    /* release deferred output only.        */
        }
#ifdef USE_X
    wiz_XFlush (wiv.current_Display);
    /* flush X output to server, this is incase                                     */
#endif              /* we have'nt been near XtMainloop in a while */
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Inquire the current deferral mode & segment regen mode.                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_inq_deferral(INT *defmod,INT *regmod);                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    defmod          -(INT *)                                                      */
/*    regmod          -(INT *)                                                      */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_inq_deferral (INT *defmod, INT *regmod)
#else
publicdef INT wm_inq_deferral (defmod, regmod)
INT *defmod, *regmod;
#endif
    {
    GKS_INT ier, dum1, dum2;
    INT status = SUCCESS;
    
    if (ug_if_gpx () == TRUE)
        status = wm_gqwkdu (MAIN_WKID, &ier, defmod, regmod, &dum1, &dum2);
    
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Set the deferral & implicit segment regeneration modes.                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_set_deferral(INT defmod,INT regmod);                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    defmod          -(INT)                                                        */
/*    regmod          -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_set_deferral (INT defmod, INT regmod)
#else
publicdef INT wm_set_deferral (defmod, regmod)
INT defmod, regmod;
#endif
    {
    INT old_defmod, old_regmod;
    INT wk_id = MAIN_WKID;
    INT status = SUCCESS;
    
    if (ug_if_gpx () == TRUE && wk_id > 0)
        {
        status = wm_inq_deferral (&old_defmod, &old_regmod);
        if ((status == SUCCESS) && (old_defmod != defmod || (old_regmod != regmod)))
            wm_gsds (wk_id, defmod, regmod);
        }
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wm_set_default_deferral(INT defmod,INT regmod);                 */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    defmod          -(INT)                                                        */
/*    regmod          -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT wm_set_default_deferral (INT defmod, INT regmod)
#else
publicdef INT wm_set_default_deferral (defmod, regmod)
INT defmod, regmod;
#endif
    {
    INT old_defmod, old_regmod;
    
    if (ug_if_gpx () == TRUE)
        {
        default_defmod = defmod;
        default_regmod = regmod;
        wm_set_deferral (defmod, regmod);
        }
    return SUCCESS;
    }
/* END:                                                                             */
