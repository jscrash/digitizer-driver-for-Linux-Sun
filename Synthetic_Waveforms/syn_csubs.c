/* DEC/CMS REPLACEMENT HISTORY, Element SYN_CSUBS.C*/
/* *8    30-OCT-1990 11:55:20 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *7    17-AUG-1990 22:23:05 VINCE "(SPR 5644) Code Cleanup"*/
/* *6     7-MAR-1990 18:12:26 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *5    13-OCT-1989 15:47:23 GORDON "(SPR 5017) Enhance KB dialog, fix main_text"*/
/* *4    22-SEP-1989 16:02:42 GILLESPIE "(SPR 101) Remove obsolete functions; fix casting/var referernce problems"*/
/* *3    11-AUG-1989 18:09:07 JULIAN "(SPR -1) cdlgid may be used before set"*/
/* *2    11-AUG-1989 14:37:33 JULIAN "(SPR -1) statement not reached"*/
/* *1    19-JUN-1989 13:30:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SYN_CSUBS.C*/
/*****************************************************************************/
/*******************************************************************************/
/**/
/*                Copyright Exploration Systems, Inc. 1989*/
/*                    Unpublished -- All rights reserved*/
/**/
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY*/
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE*/
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE*/
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF*/
/**/
/*                            Exploration Systems*/
/*                            c/o Finder Graphics Systems*/
/*                            201 Tamal Vista Blvd.*/
/*                            Corte Madera, CA  94925*/
/*                            (415) 927-0100*/
/**/
/*(The above notice does not constitute an admission of publication or*/
/*unrestricted dissemination of the work)*/
/**/
/*******************************************************************************/

#include "esi_c_lib.h"
#include "esi_rs.h"
#include "esi_syn_syn.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "esi_am.h"
#include "esi_ho_files.h"
#include "esi_lu.h"
#include "esi_tu.h"
#include "syn.rh"
static FILENAME SYN_RSRC = 
    {
    "SYN"
    };

static INT syn_main_dialog_id;

/* File Description ---------------------------------------------------------*/
/*Overview:*/
/**/
/*Prototypes and one line descriptions of each function in the file.*/
/*Public_Functions:*/
/*    publicdef INT syncdlg(INT rsrc_id,INT *cdlg_id);*/
/*    */
/*    publicdef INT synmenu(INT rsrc_id,INT *menu_id);*/
/*    */
/*    publicdef VOID synacous_dialog (INT  rerun,INT  *icancel,SYNTH *psyn);*/
/*    */
/*    publicdef VOID synaltden_dialog (INT rerun,INT *icancel,INT fdccnl_avail,SYNTH *psyn);*/
/*    */
/*    publicdef INT synfaust_dialog (INT rerun ,INT  *icancel, SYNTH  *psyn);*/
/*    */
/*    publicdef INT syngeneral_dialog (INT rerun,INT *icancel,DOUBLE avgvel,INT maxdpt,*/
/*        BOOL ifeet,SYNTH *psyn);*/
/*    */
/*    publicdef INT synkelly_dialog (INT rerun,INT  *icancel ,SYNTH *psyn);*/
/*    */
/*    publicdef INT synstraight_dialog (INT rerun ,INT  *icancel, SYNTH *psyn);*/
/*    */
/*    publicdef INT syn_main_dialog();*/
/*    */
/*    publicdef INT syn_main_text(CHAR *string);*/
/**/
/*Private_Functions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------*/
/*Description:*/
/**/
/*Prototype:*/
/*    publicdef INT syncdlg(INT rsrc_id,INT *cdlg_id);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rsrc_id         -(INT)*/
/*    cdlg_id         -(INT *)*/
/**/
/*Return Value/Status:*/
/*    Function returns status from am_define_resource.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT syncdlg (INT rsrc_id, INT *cdlg_id)
#else
publicdef INT syncdlg (rsrc_id, cdlg_id)
INT rsrc_id;
INT *cdlg_id;
#endif
    {
    return am_define_resource (AM_DIALOG, SYN_RSRC, rsrc_id, NULL_FUNCTION_PTR,
                               cdlg_id, NULL);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/**/
/*Prototype:*/
/*    publicdef INT synmenu(INT rsrc_id,INT *menu_id);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rsrc_id         -(INT)*/
/*    menu_id         -(INT *)*/
/**/
/*Return Value/Status:*/
/*    Function returns status from am_define_resource.*/
/**/
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT synmenu (INT rsrc_id, INT *menu_id)
#else
publicdef INT synmenu (rsrc_id, menu_id)
INT rsrc_id;
INT *menu_id;
#endif
    {
    return am_define_resource (AM_MENU, SYN_RSRC, rsrc_id, NULL_FUNCTION_PTR, menu_id,
                               NULL);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/*    Acoustic dialog box.*/
/**/
/*Prototype:*/
/*    publicdef VOID synacous_dialog (INT  rerun,INT  *icancel,SYNTH *psyn);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rerun           -(INT)*/
/*    icancel         -(INT  *)*/
/*    psyn            -(SYNTH *)*/
/**/
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID synacous_dialog (INT rerun, INT *icancel, SYNTH *psyn)
#else
publicdef VOID synacous_dialog (rerun, icancel, psyn)
INT rerun;
INT *icancel;
SYNTH *psyn;
#endif
    {
    static CHAR helptext[] = 
        {
        "SYNTHETICS/ACOUS"
        };
    static INT id;
    INT return_ok;
    INT nerrors;
    INT last1;
    INT last2;
    INT numitems;
    INT cditem;
    INT cdlgid = 0;
    INT cdvalue;
    INT calc_acous;
    INT acous_min_freq;
    INT acous_max_freq;
    CHAR string[STMAX];
    /**/
    /*    start of routine*/
    /**/
    if (rerun == 0)
        {
        syncdlg (ACOUS, &id);
        psyn->id_acous = id;
        }
    wi_open_dialog (id);
    wiscdv (id, ACOUS_CALC, psyn->acous);
    sprintf (string, "%d", psyn->acous_min_freq);
    wiscdt (id, ACOUS_MIN_FREQ, string);
    sprintf (string, "%d", psyn->acous_max_freq);
    wiscdt (id, ACOUS_MAX_FREQ, string);
    calc_acous = psyn->acous;
    return_ok = 0;
    *icancel = 0;
    while (return_ok == 0)
        {
        cditem = -1;
        nerrors = 0;
        while (cditem != ACOUS_OK)
            {
            switch (cditem)
                {
            case ACOUS_CANCEL:
                wi_close_dialog (cdlgid);
                *icancel = 1;
                return;
            case ACOUS_CALC:
                if (calc_acous == 0)
                    {
                    calc_acous = 1;
                    }
                else
                    {
                    calc_acous = 0;
                    }
                break;
                }
            wirqcdi (&cdlgid, &cditem, &cdvalue, string);
            }
        if (calc_acous == 1)
            {
            wiqccdi (cdlgid, ACOUS_MIN_FREQ, &acous_min_freq, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                last1 = 1;
                wialert (1, helptext, "The Minimum Freq Entered is Non-Numeric", " ");
                }
            if (last1 == 0)
                sscanf (string, "%d", &acous_min_freq);
            if (acous_min_freq < 1 AND last1 == 0)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext,
                         "The Minimum Freq Entered is Less than or Equal to 0", " ");
                }
            wiqccdi (cdlgid, ACOUS_MAX_FREQ, &acous_max_freq, string);
            last2 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                last2 = 1;
                wialert (1, helptext, "The Maximum Freq Entered is Non-Numeric", " ");
                }
            if (last2 == 0)
                sscanf (string, "%d", &acous_max_freq);
            if ((acous_max_freq <= acous_min_freq) AND last2 == 0)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext,
                         "Maximum Freq is Less than or Equal to Minimum Freq", " ");
                }
            if (acous_max_freq > 250 AND last2 == 0)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext, "Maximum Freq exceeds limit of 250", " ");
                }
            if (nerrors == 0)
                return_ok = 1;
            }
        if (calc_acous == 0)
            return_ok = 1;
        }
    psyn->acous = calc_acous;
    if (calc_acous == 1)
        {
        psyn->acous_min_freq = acous_min_freq;
        psyn->acous_max_freq = acous_max_freq;
        }
    wi_close_dialog (cdlgid);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/*    Alternate density dialog box.*/
/**/
/*Prototype:*/
/*    publicdef VOID synaltden_dialog (INT rerun,INT *icancel,INT fdccnl_avail,SYNTH *psyn);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    fdccnl_avail    -(INT)*/
/*    icancel         -(INT *)*/
/*    rerun           -(INT)*/
/*    psyn            -(SYNTH *)*/
/**/
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID synaltden_dialog (INT rerun, INT *icancel, INT fdccnl_avail,
                                 SYNTH *psyn)
#else
publicdef VOID synaltden_dialog (rerun, icancel, fdccnl_avail, psyn)
INT fdccnl_avail;
INT *icancel;
INT rerun;
SYNTH *psyn;
#endif
    {
    static INT id;
    static CHAR helptext[] = 
        {
        "SYNTHETICS/ALTDEN"
        };
    INT cditem;
    INT cdlgid = 0;
    INT cdvalue;
    INT numitems;
    INT return_ok;
    INT nerrors;
    INT last1;
    INT calc_den_vel;
    INT calc_den_fdccnl;
    FLOAT altden_fluid;
    FLOAT altden_matrix;
    CHAR string[STMAX];
    /**/
    /*    start of routine*/
    /**/
    if (rerun == 0)
        {
        syncdlg (ALTDEN, &id);
        psyn->id_altden = id;
        }
    wi_open_dialog (id);
    if (psyn->altden_calc_den_vel == 0 && psyn->altden_calc_den_fdccnl == 0)
        {
        wiscdv (id, ALTDEN_NO_CALC_DEN, ON);
        }
    if (psyn->altden_calc_den_vel != 0)
        {
        wiscdv (id, ALTDEN_CALC_DEN_VEL, ON);
        }
    if (psyn->altden_calc_den_fdccnl != 0)
        {
        wiscdv (id, ALTDEN_CALC_DEN_FDCCNL, ON);
        }
    sprintf (string, "%4.2f", psyn->altden_matrix);
    wiscdt (id, ALTDEN_MATRIX, string);
    sprintf (string, "%4.2f", psyn->altden_fluid);
    wiscdt (id, ALTDEN_FLUID, string);
    if (fdccnl_avail == 0)
        {
        wiencdi (id, ALTDEN_CALC_DEN_FDCCNL, 0);
        wiencdi (id, ALTDEN_MATRIX, 0);
        wiencdi (id, ALTDEN_FLUID, 0);
        }
    return_ok = 0;
    calc_den_vel = psyn->altden_calc_den_vel;
    calc_den_fdccnl = psyn->altden_calc_den_fdccnl;
    altden_fluid = psyn->altden_fluid;
    altden_matrix = psyn->altden_matrix;
    while (return_ok == 0)
        {
        cditem = -1;
        nerrors = 0;
        *icancel = 0;
        while (cditem != ALTDEN_OK)
            {
            switch (cditem)
                {
            case ALTDEN_CANCEL:
                wi_close_dialog (cdlgid);
                *icancel = 1;
                return;
            case ALTDEN_NO_CALC_DEN:
                calc_den_vel = 0;
                calc_den_fdccnl = 0;
                break;
            case ALTDEN_CALC_DEN_VEL:
                calc_den_vel = 1;
                calc_den_fdccnl = 0;
                break;
            case ALTDEN_CALC_DEN_FDCCNL:
                calc_den_vel = 0;
                calc_den_fdccnl = 1;
                break;
                }
            wirqcdi (&cdlgid, &cditem, &cdvalue, string);
            }
        if (calc_den_fdccnl == 1)
            {
            wiqccdi (cdlgid, ALTDEN_FLUID, &cdvalue, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext, "Fluid Density not Numeric.", " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%f", &altden_fluid);
            if ((altden_fluid <= 0.5 || altden_fluid > 3.0) && last1 == 0)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext,
                         "Fluid Density not in Permissable Range of 0.5 to 3.0", " ");
                }
            wiqccdi (cdlgid, ALTDEN_MATRIX, &cdvalue, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext, "Matrix Density not Numeric.", " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%f", &altden_matrix);
            if ((altden_matrix <= 1.0 OR altden_matrix > 4.0) AND last1 == 0)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext,
                         "Matrix Density not in Permissable Range of 0.5 to 3.0", " ");
                }
            }
        if (nerrors == 0)
            return_ok = 1;
        }
    psyn->altden_matrix = altden_matrix;
    psyn->altden_fluid = altden_fluid;
    psyn->altden_calc_den_vel = calc_den_vel;
    psyn->altden_calc_den_fdccnl = calc_den_fdccnl;
    wi_close_dialog (cdlgid);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/*    Faust dialog box.*/
/**/
/*Prototype:*/
/*    publicdef INT synfaust_dialog (INT rerun ,INT  *icancel, SYNTH  *psyn);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rerun           -(INT)*/
/*    psyn            -(SYNTH *)*/
/*    incancel        -(INT *)*/
/**/
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT synfaust_dialog (INT rerun, INT *icancel, SYNTH *psyn)
#else
publicdef INT synfaust_dialog (rerun, icancel, psyn)
INT rerun;
SYNTH *psyn;
INT *icancel;
#endif
    {
    static INT id;
    INT nerrors;
    INT numitems;
    INT last1;
    INT return_ok;
    INT cditem;
    INT cdlgid = 0;
    INT cdvalue;
    INT calc_vel;
    FLOAT faust_constant;
    CHAR string[STMAX];
    /**/
    /*    start of routine*/
    /**/
    psyn->faust = ON;
    if (rerun == 0)
        {
        syncdlg (FAUST, &id);
        psyn->id_faust = id;
        }
    wi_open_dialog (id);
    wiscdv (id, FAUST_CALC_VEL, psyn->faust);
    sprintf (string, "%6.1f", psyn->faust_constant);
    wiscdt (id, FAUST_CONSTANT, string);
    return_ok = 0;
    calc_vel = psyn->faust;
    while (return_ok == 0)
        {
        nerrors = 0;
        *icancel = 0;
        cditem = -1;
        while (cditem != FAUST_OK)
            {
            switch (cditem)
                {
            case FAUST_CANCEL:
                wi_close_dialog (cdlgid);
                *icancel = 1;
                return;
            case FAUST_CALC_VEL:
                if (calc_vel == 0)
                    calc_vel = 1;
                else
                    calc_vel = 0;
                break;
                }
            wirqcdi (&cdlgid, &cditem, &cdvalue, string);
            }
        if (calc_vel == 1)
            {
            wiqccdi (cdlgid, FAUST_CONSTANT, &cdvalue, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, "SYNTHETICS FAUST", "Faust Constant is Not Numeric.", " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%f", &faust_constant);
            if (last1 == 0 AND (faust_constant <= 100 OR faust_constant > 20000))
                {
                wialert (1, "SYNTHETICS FAUST",
                         "Faust Constant out of Permissable Range of 100 to 20000",
                         " ");
                nerrors = nerrors + 1;
                }
            }
        if (nerrors == 0)
            return_ok = 1;
        }
    psyn->faust_constant = faust_constant;
    psyn->faust = calc_vel;
    wi_close_dialog (cdlgid);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/*    General dialog box.*/
/**/
/*Prototype:*/
/*    publicdef INT syngeneral_dialog (INT rerun,INT *icancel,DOUBLE avgvel,INT maxdpt,*/
/*        BOOL ifeet,SYNTH *psyn);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rerun           -(INT)*/
/*    icancel         -(INT *)*/
/*    avgvel          -(DOUBLE)*/
/*    maxdpt          -(INT)*/
/*    ifeet           -(BOOL)*/
/*    psyn            -(SYNTH *)*/
/**/
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT syngeneral_dialog (INT rerun, INT *icancel, DOUBLE avgvel, INT maxdpt,
                                 BOOL ifeet, SYNTH *psyn)
#else
publicdef INT syngeneral_dialog (rerun, icancel, avgvel, maxdpt, ifeet, psyn)
INT rerun;
INT *icancel;
DOUBLE avgvel;
INT maxdpt;
BOOL ifeet;
SYNTH *psyn;
#endif
    {
    static CHAR helptext[] = 
        {
        "SYNTHETICS/GENERAL"
        };
    static INT id;
    INT numitems;
    INT cditem;
    INT cdlgid = 0;
    INT cdvalue;
    INT calc_multi;
    FLOAT multi_coef;
    INT calc_agc;
    INT agc_length;
    INT td_print;
    INT nfilt;
    INT datum;
    FLOAT veloc;
    CHAR string[STMAX];
    INT nerrors;
    INT last1;
    INT return_ok;
    /**/
    /*    start of routine*/
    /**/
    if (rerun == 0)
        {
        syncdlg (GENERAL, &id);
        psyn->id_general = id;
        }
    wiscdv (id, GENERAL_MULTI, psyn->general_multi);
    sprintf (string, "%.3f", psyn->general_multi_coef);
    wiscdt (id, GENERAL_MULTI_COEF, string);
    wiscdv (id, GENERAL_AGC, psyn->general_agc);
    sprintf (string, "%d", psyn->general_agc_length);
    wiscdt (id, GENERAL_AGC_LENGTH, string);
    wiscdv (id, GENERAL_PRINT_TD, psyn->general_td_print);
    sprintf (string, "%d", psyn->general_nfilt);
    wiscdt (id, GENERAL_NFILT, string);
    if (psyn->iekb == -999)
        {
        wiencdi (id, GENERAL_DATUM, 0);
        }
    else
        {
        wiencdi (id, GENERAL_DATUM, 1);
        sprintf (string, "%d", psyn->general_datum);
        wiscdt (id, GENERAL_DATUM, string);
        }
    if (ifeet)
        {
        sprintf (string, "%6.0f", psyn->general_veloc);
        wiscdt (id, GENERAL_VELOC, string);
        wiscdt (id, GENERAL_AVGVEL_PROMPT,
                "Average velocity (f/s) for the 1st 50 ft of log is");
        sprintf (string, "%6.0f", avgvel);
        wiscdt (id, GENERAL_AVGVEL, string);
        }
    else
        {
        sprintf (string, "%6.0f", psyn->general_veloc / 3.28028);
        wiscdt (id, GENERAL_VELOC, string);
        wiscdt (id, GENERAL_AVGVEL_PROMPT,
                "Average velocity (m/s) for the 1st 16 m of log is");
        sprintf (string, "%6.0f", avgvel / 3.28028);
        wiscdt (id, GENERAL_AVGVEL, string);
        }
    return_ok = 0;
    calc_multi = psyn->general_multi;
    calc_agc = psyn->general_agc;
    td_print = psyn->general_td_print;
    multi_coef = psyn->general_multi_coef;
    agc_length = psyn->general_agc_length;
    datum = psyn->general_datum;
    wi_open_dialog (id);
    while (return_ok == 0)
        {
        nerrors = 0;
        *icancel = 0;
        cditem = -1;
        while (cditem != GENERAL_OK)
            {
            switch (cditem)
                {
            case GENERAL_CANCEL:
                wi_close_dialog (cdlgid);
                *icancel = 1;
                return;
            case GENERAL_AGC:
                if (calc_agc == 0)
                    calc_agc = 1;
                else
                    calc_agc = 0;
                break;
            case GENERAL_MULTI:
                if (calc_multi == 0)
                    calc_multi = 1;
                else
                    calc_multi = 0;
                break;
            case GENERAL_PRINT_TD:
                if (td_print == 0)
                    td_print = 1;
                else
                    td_print = 0;
                break;
                }
            wirqcdi (&cdlgid, &cditem, &cdvalue, string);
            }
        if (calc_multi == 1)
            {
            wiqccdi (cdlgid, GENERAL_MULTI_COEF, &cdvalue, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext, "Surface Reflexion Coefficient is Not Numeric",
                         " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%f", &multi_coef);
            if ((multi_coef < 0. OR multi_coef > 1.) AND last1 == 0)
                {
                wialert (1, helptext,
                 "Surface Reflexion Coefficient out of Permissable Range of 0. to 1.",
                         " ");
                nerrors = nerrors + 1;
                }
            }
        if (calc_agc == 1)
            {
            wiqccdi (cdlgid, GENERAL_AGC_LENGTH, &agc_length, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext, "Length of AGC Window is Not Numeric", " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%d", &agc_length);
            if ((agc_length < 10 OR agc_length > 1000) AND last1 == 0)
                {
                wialert (1, helptext,
                         "Length of Window out of Permissable Range of 10 to 1000",
                         " ");
                nerrors = nerrors + 1;
                }
            }
        wiqccdi (cdlgid, GENERAL_NFILT, &nfilt, string);
        last1 = 0;
        if (ts_isfloat (string) < 1)
            {
            nerrors = nerrors + 1;
            wialert (1, helptext, "Number of Filters is Not Numeric", " ");
            last1 = 1;
            }
        if (last1 == 0)
            sscanf (string, "%d", &nfilt);
        if ((nfilt < 0 OR nfilt > 10) AND last1 == 0)
            {
            wialert (1, helptext,
                     "Number of Filters out of Permissable Range of 0 to 10", " ");
            nerrors = nerrors + 1;
            }
        if (psyn->iekb != -999)
            {
            wiqccdi (cdlgid, GENERAL_DATUM, &datum, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, helptext, "Datum is Not Numeric", " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%d", &datum);
            if (datum > psyn->iekb AND last1 == 0)
                {
                sprintf (string, "Datum Greater than EKB of %5d", psyn->iekb);
                wialert (1, helptext, string, " ");
                nerrors = nerrors + 1;
                }
            if (datum < psyn->iekb - maxdpt + 100 AND last1 == 0)
                {
                sprintf (string, "Datum must be greater than %d",
                         psyn->iekb - maxdpt + 100);
                wialert (1, helptext, string, " ");
                nerrors = nerrors + 1;
                }
            }
        wiqccdi (cdlgid, GENERAL_VELOC, &cdvalue, string);
        last1 = 0;
        if (ts_isfloat (string) < 1)
            {
            nerrors = nerrors + 1;
            wialert (1, helptext, "Surface Velocity is Not Numeric", " ");
            last1 = 1;
            }
        if (last1 == 0)
            {
            sscanf (string, "%f", &veloc);
            if (ifeet)
                {
                if (veloc < 4000.0 OR veloc > 20000.0)
                    {
                    wialert (1, helptext,
                             "Surface velocity must be between 4000 to 20000 ft/sec.",
                             " ");
                    nerrors = nerrors + 1;
                    }
                else
                    {
                    }
                }
            else                    /*   Metric units of measure.   */
                {
                if (veloc < 1200.0 OR veloc > 6000.0)
                    {
                    wialert (1, helptext,
                             "Surface velocity must be between 1200 to 6000 m/sec.",
                             " ");
                    nerrors = nerrors + 1;
                    }
                else
                    {
                    }
                }
            }
        if (nerrors == 0)
            return_ok = 1;
        }
    psyn->general_multi = calc_multi;
    psyn->general_multi_coef = multi_coef;
    psyn->general_agc = calc_agc;
    psyn->general_agc_length = agc_length;
    psyn->general_td_print = td_print;
    psyn->general_nfilt = nfilt;
    psyn->general_datum = datum;
    if (NOT ifeet)
        {
        veloc *= 3.28028;
        }
    psyn->general_veloc = veloc;
    wi_close_dialog (cdlgid);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/*    Kelly dialog box.*/
/**/
/*Prototype:*/
/*    publicdef INT synkelly_dialog (INT rerun,INT  *icancel ,SYNTH *psyn);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rerun           -(INT)*/
/*    icancel         -(INT *)*/
/*    psyn            -(SYNTH *)*/
/**/
/*Return Value/Status:*/
/*    SUCCESS - Successful completion.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT synkelly_dialog (INT rerun, INT *icancel, SYNTH *psyn)
#else
publicdef INT synkelly_dialog (rerun, icancel, psyn)
INT rerun;
INT *icancel;
SYNTH *psyn;
#endif
    {
    static INT id;
    INT nerrors;
    INT last1;
    INT return_ok;
    INT numitems;
    INT cditem;
    INT cdlgid = 0;
    INT cdvalue;
    INT kelly_estimated;
    INT kelly_elevation;
    DOUBLE kelly_elevation_float;
    CHAR string[STMAX];
    BOOL inMeters;
    UOM_NAME defaultUom;            /* Project default vertical UOM */
    UOM_NAME requestedUom;          /* UOM user enters in this cdlg */
    
    if (rerun == 0)
        {
        syncdlg (KELLY, &id);
        psyn->id_kelly = id;
        }
    lu_project_default ("VERTICAL_UOM", defaultUom, CHAR_TYPE, sizeof(UOM_NAME));
    wi_open_dialog (id);
    if (ARE_SAME (defaultUom, "METERS"))
        {
        wi_set_dialog_value (id, KELLY_METERS, TRUE);
        inMeters = TRUE;
        }
    else
        {
        wi_set_dialog_value (id, KELLY_FEET, TRUE);
        inMeters = FALSE;
        }
    wiscdv (id, KELLY_ESTIMATED, psyn->kelly_estimated);
    if (psyn->kelly_elevation == -999)
        string[0] = NUL;
    else
        sprintf (string, "%d", psyn->kelly_elevation);
    
    wiscdt (id, KELLY_ELEVATION, string);
    return_ok = 0;
    *icancel = 0;
    kelly_estimated = psyn->kelly_estimated;
    while (return_ok == 0)
        {
        nerrors = 0;
        cditem = -1;
        while (cditem != KELLY_OK)
            {
            switch (cditem)
                {
            case KELLY_CANCEL:
                wi_close_dialog (cdlgid);
                *icancel = 1;
                return SUCCESS;
            case KELLY_ESTIMATED:
                kelly_estimated = kelly_estimated ? OFF : ON;
                break;
            case KELLY_FEET:
                inMeters = FALSE;
                break;
            case KELLY_METERS:
                inMeters = TRUE;
                break;
                }
            wirqcdi (&cdlgid, &cditem, &cdvalue, string);
            }
        if (kelly_estimated EQUALS ON)
            {
            wiqccdi (cdlgid, KELLY_ELEVATION, &kelly_elevation, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                last1 = 1;
                wialert (1, "SYNTHETICS KELLY", "Estimated E. K. B. is not numeric.",
                         " ");
                }
            if (last1 == 0)
                sscanf (string, "%d", &kelly_elevation);
            if (last1 == 0 AND (kelly_elevation < -50 OR kelly_elevation > 25000))
                {
                nerrors = nerrors + 1;
                wialert (1, "SYNTHETICS KELLY",
                        "Estimated E. K. B. out of Permissable Range of -50 to 25000",
                         " ");
                }
            }
        if (nerrors EQUALS 0)
            return_ok = 1;
        }
    psyn->kelly_estimated = kelly_estimated;
    if (kelly_estimated EQUALS ON)
        {                           /* Synthetics expects kelly_elevation to be in default veritcal UOM */
        if (inMeters)
            strcpy (requestedUom, "METERS");
        else
            strcpy (requestedUom, "FEET");
        kelly_elevation_float = kelly_elevation;
        /* Convert user's entered value to project default vertical UOM */
        tu_unit_conversion (kelly_elevation_float, requestedUom,
                            &kelly_elevation_float, defaultUom);
        kelly_elevation = kelly_elevation_float;
        psyn->kelly_elevation = kelly_elevation;
        }
    wi_close_dialog (cdlgid);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/*    Straight dialog box.*/
/**/
/*Prototype:*/
/*    publicdef INT synstraight_dialog (INT rerun ,INT  *icancel, SYNTH *psyn);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    rerun           -(INT)*/
/*    psyn            -(SYNTH *)*/
/*    icancel         -(INT *)*/
/**/
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT synstraight_dialog (INT rerun, INT *icancel, SYNTH *psyn)
#else
publicdef INT synstraight_dialog (rerun, icancel, psyn)
INT rerun;
SYNTH *psyn;
INT *icancel;
#endif
    {
    static INT id;
    INT nerrors;
    INT numitems;
    INT last1;
    INT return_ok;
    INT cditem;
    INT cdlgid = 0;
    INT cdvalue;
    INT straight_use;
    FLOAT straight_vel;
    CHAR string[STMAX];
    /**/
    /*    start of routine*/
    /**/
    if (rerun == 0)
        {
        syncdlg (STRAIGHT, &id);
        psyn->id_straight = id;
        }
    wi_open_dialog (id);
    if (psyn->straight_use == 1)
        wiscdv (id, STRAIGHT_USE, ON);
    else
        wiscdv (id, STRAIGHT_EXIT, ON);
    sprintf (string, "%f", psyn->straight_vel);
    wiscdt (id, STRAIGHT_VEL, string);
    straight_use = 0;
    return_ok = 0;
    *icancel = 0;
    straight_use = psyn->straight_use;
    while (return_ok == 0)
        {
        nerrors = 0;
        cditem = -1;
        while (cditem != STRAIGHT_OK)
            {
            switch (cditem)
                {
            case STRAIGHT_CANCEL:
                wi_close_dialog (cdlgid);
                *icancel = 1;
                return;
            case STRAIGHT_USE:
                straight_use = 1;
                break;
            case STRAIGHT_EXIT:
                straight_use = 0;
                break;
                }
            wirqcdi (&cdlgid, &cditem, &cdvalue, string);
            }
        if (straight_use == 1)
            {
            wiqccdi (cdlgid, STRAIGHT_VEL, &cdvalue, string);
            last1 = 0;
            if (ts_isfloat (string) < 1)
                {
                nerrors = nerrors + 1;
                wialert (1, "SYNTHETICS STRAIGHT", "Straight Velocity is Not Numeric.",
                         " ");
                last1 = 1;
                }
            if (last1 == 0)
                sscanf (string, "%f", &straight_vel);
            if ((straight_vel < 4000. OR straight_vel > 24000.) AND last1 == 0)
                {
                wialert (1, "SYNTHETICS STRAIGHT",
                   "Straight Line Velocity out of Permissable Range of 4000 to 24000",
                         " ");
                nerrors = nerrors + 1;
                }
            }
        if (nerrors == 0)
            return_ok = 1;
        }
    psyn->straight_vel = straight_vel;
    psyn->straight_use = straight_use;
    wi_close_dialog (cdlgid);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/**/
/*Prototype:*/
/*    publicdef INT syn_main_dialog();*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    None.*/
/*    */
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*    */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT syn_main_dialog (void)
#else
publicdef INT syn_main_dialog ()
#endif
    {
    am_declare ("Synthetics");
    syncdlg (SYN_MAIN_DIALOG, &syn_main_dialog_id);
    wiscdttl (syn_main_dialog_id, "Messages");
    wi_open_dialog (syn_main_dialog_id);
    }
/* END:     */

/* Function Description -----------------------------------------------------*/
/*Description:*/
/**/
/*Prototype:*/
/*    publicdef INT syn_main_text(CHAR *string);*/
/**/
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)*/
/*    string          -(CHAR *)*/
/**/
/*Return Value/Status:*/
/*    None.*/
/*    */
/*Scope:*/
/*    PUBLIC*/
/*   */
/*Limitations/Assumptions:*/
/**/
/*-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT syn_main_text (CHAR *string)
#else
publicdef INT syn_main_text (string)
CHAR *string;
#endif
    {
    if (IS_EMPTY_STRING (string))
        wi_close_dialog (syn_main_dialog_id);
    else
        wiscdt (syn_main_dialog_id, SYN_MAIN_TEXT, string);
    }
/* END:     */
