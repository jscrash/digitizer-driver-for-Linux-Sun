/* DEC/CMS REPLACEMENT HISTORY, Element XS_PREFS_2.C*/
/*  9     4-MAR-1992 08:49:23 JEFF "(SPR 6672) removed slope stuff from depth interval dialog"*/
/*  8    27-FEB-1992 09:29:19 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *7    24-JUL-1991 18:14:22 JESSIE "(SPR 0) fix Zone and Offsets crash XS case#3243"*/
/* *6    18-JUL-1991 17:02:51 JANTINA "(SPR 0)  Prompt added to DEPTH CONSTANT dialog to specify depth units"*/
/* *5    27-MAR-1991 13:30:18 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *4    17-AUG-1990 22:42:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    11-MAY-1990 14:59:42 MING "(SPR 5318) change LINE_STYLE_CODE to LINE_STYPE "*/
/* *2    27-SEP-1989 08:45:09 GORDON "(SPR 5001) Re-enable default params for track depthgrid in xs_depthgrid_cdlg_init"*/
/* *1    19-JUN-1989 13:45:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_PREFS_2.C*/
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

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_MD_DEFS_H

#include "esi_md_defs.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef XS_CDLGS_RH

#include "xs_cdlgs.rh"

#endif

#ifndef XS_MENUS_RH

#include "xs_menus.rh"

#endif

#ifndef ESI_GL_PROJECT_H

#include "esi_gl_project.h"

#endif

#if USE_PROTOTYPES
static INT xs_depth_scale_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_dataint_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_depthgrid_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_top2top_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_zonesel_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_depthconst_cdlg_server (INT id, INT item, XS_STRUCT *pxs);
static INT xs_depthgrid_cdlg_init (XS_STRUCT *pxs);
static INT xs_top2top_cdlg_init (XS_STRUCT *pxs);
static INT xs_zonesel_cdlg_init (XS_STRUCT *pxs);
static INT xs_depthconst_cdlg_init (XS_STRUCT *pxs);
#else
static INT xs_depth_scale_cdlg_server ();
static INT xs_dataint_cdlg_server ();
static INT xs_depthgrid_cdlg_server ();
static INT xs_top2top_cdlg_server ();
static INT xs_zonesel_cdlg_server ();
static INT xs_depthconst_cdlg_server ();
static INT xs_depthgrid_cdlg_init ();
static INT xs_top2top_cdlg_init ();
static INT xs_zonesel_cdlg_init ();
static INT xs_depthconst_cdlg_init ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_depth_scale_cdlg_init(XS_STRUCT *pxs);
    
    publicdef INT xs_depth_scale_cdlg_term(XS_STRUCT *pxs);
   
    publicdef INT xs_depthgrid_cdlg_term(XS_STRUCT *pxs);
    
    publicdef INT xs_dataint_cdlg_init(XS_STRUCT *pxs);
    
    publicdef INT xs_dataint_cdlg_term(XS_STRUCT *pxs);
    
    publicdef INT xs_depthconst_cdlg_term(XS_STRUCT *pxs);
    
    publicdef INT xs_top2top_cdlg_term(XS_STRUCT *pxs);
    
    publicdef INT xs_zonesel_cdlg_term(XS_STRUCT *pxs);

Private_Functions:
    static INT xs_depth_scale_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

    static INT xs_dataint_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

    static INT xs_depthgrid_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

    static INT xs_top2top_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

    static INT xs_zonesel_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

    static INT xs_depthconst_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

    static INT xs_depthgrid_cdlg_init(XS_STRUCT *pxs );

    static INT xs_top2top_cdlg_init(XS_STRUCT *pxs );

    static INT xs_zonesel_cdlg_init(XS_STRUCT *pxs );

    static INT xs_depthconst_cdlg_init(XS_STRUCT *pxs );

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Init dialog to get depth scale.

Prototype:
    publicdef INT xs_depth_scale_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_depth_scale_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_depth_scale_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_DEPTH_SCALE,
                                 xs_depth_scale_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, DEPTH_SCALE_MENU_ITEM, OFF);
    pxs->dpthscal_in_use = ON;
    pxs->dpthscal_cdlg_id = id;
    
    /* display the users depth scale string from   */
    /* the structure                   */
    
    wiscdt (id, XS_DEPTH_SCALE_STR, pxs->depth_scale_str);
    
    /* Make the dialog box visible.            */
    status = wiopcd (id);
    /* Hilight the scale string for changes.       */
    status = wi_activate (id, XS_DEPTH_SCALE_STR, ON);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Accept depth scale from user.

Prototype:
    static INT xs_depth_scale_cdlg_server(INT id, INT item, XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_depth_scale_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_depth_scale_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT unused;
    INT status, flag;
    CHAR text[81];
    
    PROGNAME ("xs_depth_scale_cdlg_server");
    
    switch (item)
        {
        
    case XS_DEPTH_SCALE_ACCEPT:
        
        wiqccdi (id, XS_DEPTH_SCALE_STR, &unused, text);
        ts_sto_upper (text);
        strcpy (pxs->depth_scale_str, text);
        
        status = xsz_depth_scale_calc (pxs);
        
        /* If bad UOM string, leave cdlg up        */
        if (status < SUCCESS)
            return(SUCCESS);
        
        pxs->changed_since_save = TRUE;
        status = xs_depth_scale_cdlg_term (pxs);
        break;
        
    case XS_DEPTH_SCALE_IGNORE:
        
        status = xs_depth_scale_cdlg_term (pxs);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Complete depth scale transactions.

Prototype:
    publicdef INT xs_depth_scale_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC structure.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_depth_scale_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_depth_scale_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_depth_scale_cdlg_term");
    
    pxs->dpthscal_in_use = OFF;
    am_release_resource (pxs->dpthscal_cdlg_id);
    
    /* always force them through the depth grid
       routine, whether accept or ignore on the
       previous dialog                 */
    
    status = xs_depthgrid_cdlg_init (pxs);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Display dialog to accept grid defs.

Prototype:
    static INT xs_depthgrid_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_depthgrid_cdlg_init (XS_STRUCT *pxs)
#else
static INT xs_depthgrid_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    CHAR text[128];
    TRACK_STRUCT *pdata;
    PROGNAME ("xs_depthgrid_cdlg_init");
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_DPTHGRID,
                                 xs_depthgrid_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, DEPTH_SCALE_MENU_ITEM, OFF);
    pxs->depthgrid_in_use = ON;
    pxs->depthgrid_cdlg_id = id;
    
    status = llcount (&(pxs->plo->tracks));
    if (status <= 0)
        {
        /* make something up               */
        pxs->xst_bold = LINES;
        pxs->xst_bold_intvl = 100;
        pxs->xst_medium = TICS;
        pxs->xst_medium_intvl = 50;
        pxs->xst_fine = NONE;
        pxs->xst_fine_intvl = 0;
        pxs->xst_annotation_intvl = 250;
        }
    else
        {
        status = llfirst (&(pxs->plo->tracks), &pdata);
        pxs->xst_bold = pdata->primary.type;
        pxs->xst_bold_intvl = pdata->primary.frequency;
        pxs->xst_medium = pdata->secondary.type;
        pxs->xst_medium_intvl = pdata->secondary.frequency;
        pxs->xst_fine = pdata->tertiary.type;
        pxs->xst_fine_intvl = pdata->tertiary.frequency;
        pxs->xst_annotation_intvl = pdata->annotation_interval;
        }
    switch (pxs->xst_bold)
        {
    case LINES:
        wiscdv (id, XS_DPTHGRID_BOLDLINES_BTN, ON);
        break;
        
    case TICS:
        wiscdv (id, XS_DPTHGRID_BOLDTICS_BTN, ON);
        break;
        
    case NONE:
        wiscdv (id, XS_DPTHGRID_BOLDNONE_BTN, ON);
        break;
        
    default:
        break;
        
        }
    switch (pxs->xst_medium)
        {
    case LINES:
        wiscdv (id, XS_DPTHGRID_MEDLINES_BTN, ON);
        break;
        
    case TICS:
        wiscdv (id, XS_DPTHGRID_MEDTICS_BTN, ON);
        break;
        
    case NONE:
        wiscdv (id, XS_DPTHGRID_MEDNONE_BTN, ON);
        break;
        
    default:
        break;
        
        }
    switch (pxs->xst_fine)
        {
    case LINES:
        wiscdv (id, XS_DPTHGRID_FINELINES_BTN, ON);
        break;
        
    case TICS:
        wiscdv (id, XS_DPTHGRID_FINETICS_BTN, ON);
        break;
        
    case NONE:
        wiscdv (id, XS_DPTHGRID_FINENONE_BTN, ON);
        break;
        
    default:
        break;
        
        }
    /* waiting on enhancements to cross section -  */
    /* so turn these buttons off           */
    wiencdi (id, XS_DPTHGRID_TVD_BTN, OFF);
    wiencdi (id, XS_DPTHGRID_SUBSEA_BTN, OFF);
    wiencdi (id, XS_DPTHGRID_TIME_BTN, OFF);
    /* FOR NOW                     */
    wiscdv (id, XS_DPTHGRID_MD_BTN, ON);
    /* ignore this section for now         */
#if 0
    switch (pxs->xst_tracktype)
        {
    case MEASURED_DEPTH:
        wiscdv (id, XS_DPTHGRID_MD_BTN, ON);
        break;
        
    case TVD:
        wiscdv (id, XS_DPTHGRID_TVD_BTN, ON);
        break;
        
    case SUBSEA_ELEV:
        wiscdv (id, XS_DPTHGRID_SUBSEA_BTN, ON);
        break;
        
    case TIME:
        wiscdv (id, XS_DPTHGRID_TIME_BTN, ON);
        break;
        
    default:
        break;
        
        }
#endif
    sprintf (text, "%.0lf", pxs->xst_bold_intvl);
    wiscdt (id, XS_DPTHGRID_BOLD_INTVL, text);
    sprintf (text, "%.0lf", pxs->xst_medium_intvl);
    wiscdt (id, XS_DPTHGRID_MED_INTVL, text);
    sprintf (text, "%.0lf", pxs->xst_fine_intvl);
    wiscdt (id, XS_DPTHGRID_FINE_INTVL, text);
    
    /* INitialize the unit of depth interval */
    if (pxs->depth_interval_scale_units EQUALS 0)
        {
        pxs->depth_interval_scale_units = pxs->depth_scale_user_units;
        }
    switch (pxs->depth_interval_scale_units)
        {
    case XS_CENTIMETER_UNITS:
        wiscdv (id, XS_DPTHGRID_CM_UNIT_BTN, ON);
        break;
    case XS_INCH_UNITS:
    default:
        wiscdv (id, XS_DPTHGRID_INCH_UNIT_BTN, ON);
        break;
        }
    status = wiopcd (id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to accept track grid defs from user.

Prototype:
    static INT xs_depthgrid_cdlg_server( INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Resource event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_depthgrid_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_depthgrid_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT unused;
    INT count, i;
    CHAR text[128];
    TRACK_STRUCT *pdata, *pnext;
    MD_PARM_STRUCT parm_struct;
    MD_KEYLIST key_list;
    PROGNAME ("xs_depthgrid_cdlg_server");
    
    switch (item)
        {
    case XS_DPTHGRID_ACCEPT:
        /* SET PDATA FOR NOW               */
        status = llfirst (&pxs->plo->tracks, &pdata);
        /* hope there is something out there       */
        wiqccdi (id, XS_DPTHGRID_INCH_UNIT_BTN, &unused, text);  
	/* INC/FEET radio button is on ? - KLY */
        pxs->depth_interval_scale_units = 
	  (unused == ON) ? XS_INCH_UNITS : XS_CENTIMETER_UNITS;
        wiqccdi (id, XS_DPTHGRID_BOLD_INTVL, &unused, text);
        sscanf (text, "%lf", &pdata->primary.frequency);
        wiqccdi (id, XS_DPTHGRID_MED_INTVL, &unused, text);
        sscanf (text, "%lf", &pdata->secondary.frequency);
        wiqccdi (id, XS_DPTHGRID_FINE_INTVL, &unused, text);
        sscanf (text, "%lf", &pdata->tertiary.frequency);
        
        /* save all buttons                */
        pdata->primary.type = pxs->xst_bold;
        pdata->secondary.type = pxs->xst_medium;
        
        /* Use solid lines if tertiary marks are
           tics, dotted if they are lines.         */
        
        if (pdata->tertiary.type != pxs->xst_fine)
            {
            pdata->tertiary.type = pxs->xst_fine;
            
            /* Set up for table look up.           */
            key_list.tran_flg = 'Y';
            strcpy (key_list.val_arg, "LINE_STYLE");
            
            if (pdata->tertiary.type == TICS)
                {
                parm_struct.parmval.cval = (CHAR *)am_allocate (AM_APPLICATION,
                                                                strlen ("SOLID") + 1);
                strcpy (parm_struct.parmval.cval, "SOLID");
                }
            else
                {
                parm_struct.parmval.cval = (CHAR *)am_allocate (AM_APPLICATION,
                                                                strlen ("DOTTED") + 1);
                strcpy (parm_struct.parmval.cval, "DOTTED");
                }
            /* Perform table look up.              */
            
            status = md_tlu (&parm_struct, &key_list);
            
            if (status == SUCCESS)
                pdata->tertiary.line.style = parm_struct.parmval.ival;
            
            else                    /* Use solid if failure.               */
                pdata->tertiary.line.style = 1;
            }
        /* Copy track marks from first track to all.   */
        count = llcount (&pxs->plo->tracks);
        for (i = 1; i < count; i++)
            {
            status = llnext (&pxs->plo->tracks, &pnext);
            status = xsz_copy_track_marks (pdata, pnext);
            }
        pxs->changed_since_save = TRUE;
        
        /* Take Down the CDLG.             */
        status = xs_depthgrid_cdlg_term (pxs);
        break;
        
    case XS_DPTHGRID_IGNORE:
        
        status = xs_depthgrid_cdlg_term (pxs);
        break;
        
    case XS_DPTHGRID_BOLDLINES_BTN: /*  save state of buttons on dialog     */
        pxs->xst_bold = LINES;
        status = wi_activate (id, XS_DPTHGRID_BOLD_INTVL, ON);
        break;
        
    case XS_DPTHGRID_BOLDTICS_BTN:
        pxs->xst_bold = TICS;
        status = wi_activate (id, XS_DPTHGRID_BOLD_INTVL, ON);
        break;
        
    case XS_DPTHGRID_BOLDNONE_BTN:
        pxs->xst_bold = NONE;
        break;
        
    case XS_DPTHGRID_MEDLINES_BTN:
        pxs->xst_medium = LINES;
        status = wi_activate (id, XS_DPTHGRID_MED_INTVL, ON);
        break;
        
    case XS_DPTHGRID_MEDTICS_BTN:
        pxs->xst_medium = TICS;
        status = wi_activate (id, XS_DPTHGRID_MED_INTVL, ON);
        break;
        
    case XS_DPTHGRID_MEDNONE_BTN:
        pxs->xst_medium = NONE;
        break;
        
    case XS_DPTHGRID_FINELINES_BTN:
        pxs->xst_fine = LINES;
        status = wi_activate (id, XS_DPTHGRID_FINE_INTVL, ON);
        break;
        
    case XS_DPTHGRID_FINETICS_BTN:
        pxs->xst_fine = TICS;
        status = wi_activate (id, XS_DPTHGRID_FINE_INTVL, ON);
        break;
        
    case XS_DPTHGRID_FINENONE_BTN:
        pxs->xst_fine = NONE;
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Remove dialog for entering depth grid.

Prototype:
    publicdef INT xs_depthgrid_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_depthgrid_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_depthgrid_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_depthgrid_cdlg_term");
    
    /* flip in-use flag, take down dialog, reset   */
    /* menus                       */
    pxs->depthgrid_in_use = OFF;
    am_release_resource (pxs->depthgrid_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_dataint_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, DEPTH_SCALE_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Init dialog to accept depth interval.

Prototype:
    publicdef INT xs_dataint_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_dataint_cdlg_init (XS_STRUCT *pxs)
#else
publicdef INT xs_dataint_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    CHAR text[128];
    PROGNAME ("xs_dataint_cdlg_init");
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_DATA_INTVL,
                                 xs_dataint_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, OFF);
    wiencdi (id, XS_DATAINT_SLOPE, OFF);
    pxs->dataint_in_use = ON;
    pxs->dataint_cdlg_id = id;
    
    /* set up buttons for data thinning algorithm  */
    pxs->ptemp->data_thinning = pxs->plo->data_thinning;
    switch (pxs->ptemp->data_thinning)
        {
    case NO_THINNING:
        wiscdv (id, XS_DATAINT_NO_THIN, ON);
        wiencdi (id, XS_DATAINT_DELSLOPE, OFF);
        wiencdi (id, XS_DATAINT_DELPNTS, OFF);
        break;
    case DECIMATION:
        wiscdv (id, XS_DATAINT_LINEAR, ON);
        wiencdi (id, XS_DATAINT_DELSLOPE, OFF);
        wiencdi (id, XS_DATAINT_DELPNTS, ON);
        break;
        
        /* * Temperary disable the function.       */
#if 0
    case SLOPE_CHANGE:
        wiscdv (id, XS_DATAINT_SLOPE, ON);
        wiencdi (id, XS_DATAINT_DELSLOPE, ON);
        wiencdi (id, XS_DATAINT_DELPNTS, OFF);
        break;
#endif
    case SLOPE_CHANGE:
        pxs->ptemp->data_thinning = NO_THINNING;
        wiscdv (id, XS_DATAINT_NO_THIN, ON);
        wiscdv (id, XS_DATAINT_SLOPE, OFF);
        wiencdi (id, XS_DATAINT_DELSLOPE, OFF);
        wiencdi (id, XS_DATAINT_SLOPE, OFF);
        wiencdi (id, XS_DATAINT_DELPNTS, OFF);
        break;
    default:
        break;
        }
    /* set up buttons for method of drawing cross  */
    /* section                     */
    pxs->ptemp->interval.method = pxs->plo->interval.method;
    switch (pxs->ptemp->interval.method)
        {
    case SEL_TOTAL_INTERVAL:
        wiscdv (id, XS_DATAINT_SURF2TD, ON);
        break;
    case SEL_BY_DEPTH:
        wiscdv (id, XS_DATAINT_DPTHCONST, ON);
        break;
    case SEL_BY_TOP:
        wiscdv (id, XS_DATAINT_GEOMARK, ON);
        break;
    case SEL_BY_ZONE:
        wiscdv (id, XS_DATAINT_ZONEOFF, ON);
        break;
    default:
        break;
        }
    /* insert initial text into edittext fields    */
    pxs->ptemp->decimate = pxs->plo->decimate;
    sprintf (text, "%d", pxs->ptemp->decimate);
    wiscdt (id, XS_DATAINT_DELPNTS, text);
    
    pxs->ptemp->delta_slope = pxs->plo->delta_slope;
    sprintf (text, "%.1lf", pxs->ptemp->delta_slope);
    wiscdt (id, XS_DATAINT_DELSLOPE, text);
    
    status = wiopcd (id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Server to accept depth interval.

Prototype:
    static INT xs_dataint_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id. 
    item            -(INT) Resource event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_dataint_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_dataint_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT unused;
    CHAR text[80];
    PROGNAME ("xs_dataint_cdlg_server");
    
    switch (item)
        {
        
    case XS_DATAINT_PROCEED:
        pxs->plo->data_thinning = pxs->ptemp->data_thinning;
        
        /* If change in interval method, set method
           and require data retrieval.         */
        
        if (pxs->plo->interval.method != pxs->ptemp->interval.method)
            {
            pxs->plo->interval.method = pxs->ptemp->interval.method;
            pxs->changed_since_display = TRUE;
            pxs->changed_since_save = TRUE;
            }
        if (pxs->plo->data_thinning == DECIMATION)
            {
            wiqccdi (id, XS_DATAINT_DELPNTS, &unused, text);
            sscanf (text, "%d", &pxs->plo->decimate);
            }
        else if (pxs->plo->data_thinning == SLOPE_CHANGE)
            {
            wiqccdi (id, XS_DATAINT_DELSLOPE, &unused, text);
            sscanf (text, "%lf", &pxs->plo->delta_slope);
            }
        status = xs_dataint_cdlg_term (pxs);
        status = xs_cs_menu_mgr (pxs);
        break;
        
    case XS_DATAINT_IGNORE:
        status = xs_dataint_cdlg_term (pxs);
        break;
        
    case XS_DATAINT_LINEAR:
        /* save button states              */
        pxs->ptemp->data_thinning = DECIMATION;
        wiencdi (id, XS_DATAINT_DELSLOPE, OFF);
        wiencdi (id, XS_DATAINT_DELPNTS, ON);
        status = wi_activate (id, XS_DATAINT_DELPNTS, ON);
        break;
        
    case XS_DATAINT_SLOPE:
        pxs->ptemp->data_thinning = SLOPE_CHANGE;
        wiencdi (id, XS_DATAINT_DELSLOPE, ON);
        wiencdi (id, XS_DATAINT_DELPNTS, OFF);
        status = wi_activate (id, XS_DATAINT_DELSLOPE, ON);
        break;
        
    case XS_DATAINT_NO_THIN:
        pxs->ptemp->data_thinning = NO_THINNING;
        wiencdi (id, XS_DATAINT_DELSLOPE, OFF);
        wiencdi (id, XS_DATAINT_DELPNTS, OFF);
        break;
        
    case XS_DATAINT_SURF2TD:
        pxs->ptemp->interval.method = SEL_TOTAL_INTERVAL;
        break;
        
    case XS_DATAINT_DPTHCONST:
        pxs->ptemp->interval.method = SEL_BY_DEPTH;
        break;
        
    case XS_DATAINT_GEOMARK:
        pxs->ptemp->interval.method = SEL_BY_TOP;
        break;
        
    case XS_DATAINT_ZONEOFF:
        pxs->ptemp->interval.method = SEL_BY_ZONE;
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Call subordinate dialogs.

Prototype:
    publicdef INT xs_dataint_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_dataint_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_dataint_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_dataint_cdlg_term");
    
    pxs->dataint_in_use = OFF;
    am_release_resource (pxs->dataint_cdlg_id);
    
    /* pick which way to go i.e. there is a 4 way  */
    /* junction here depending on interval method  */
    switch (pxs->plo->interval.method)
        {
        
    case SEL_TOTAL_INTERVAL:
        if (pxs->review_all_switch == ON)
            status = xs_trackdef_cdlg_init (pxs);
        else
            {
            wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, ON);
            wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
            }
        break;
        
    case SEL_BY_DEPTH:
        status = xs_depthconst_cdlg_init (pxs);
        break;
        
    case SEL_BY_TOP:
        status = xs_top2top_cdlg_init (pxs);
        break;
        
    case SEL_BY_ZONE:
        status = xs_zonesel_cdlg_init (pxs);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Init dialog to accept depth constant interval.

Prototype:
    static INT xs_depthconst_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_depthconst_cdlg_init (XS_STRUCT *pxs)
#else
static INT xs_depthconst_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    CHAR text[128];
    PROJECT_DEFAULT *proj;
    PROGNAME ("xs_depthconst_cdlg_init");
    
    status = am_open_workspace( "PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_DEPTH_CONST,
                                 xs_depthconst_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, OFF);
    pxs->depthconst_in_use = ON;
    pxs->depthconst_cdlg_id = id;
    
    /* remember how interval specified         */
    pxs->ptemp->interval.specify_as = pxs->plo->interval.specify_as;
    
    sprintf(text, "Enter depth constants in %s",proj->unit.vertical);
    status = wi_set_dialog_text(id,XS_DEPCONST_UNIT_PROMPT,text);

    /* put in edittext stuff               */
    sprintf (text, "%.1lf", pxs->plo->interval.top);
    wiscdt (id, XS_DEPCONST_TOP_VAL, text);
    
    sprintf (text, "%.1lf", pxs->plo->interval.base);
    wiscdt (id, XS_DEPCONST_BASE_VAL, text);
    
    /* for now, this is temporary until cross      */
    /* section is enhanced             */
    
    wiscdv (id, XS_DEPCONST_MD_BTN, ON);
    wiencdi (id, XS_DEPCONST_TVD_BTN, OFF);
    wiencdi (id, XS_DEPCONST_SUBSEA_BTN, OFF);
    
    status = wiopcd (id);
    
    status = wi_activate (id, XS_DEPCONST_TOP_VAL, ON);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Accept depth constant from user.

Prototype:
    static INT xs_depthconst_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_depthconst_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_depthconst_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT unused;
    INT status;
    CHAR text[128];
    PROGNAME ("xs_depthconst_cdlg_server");
    
    switch (item)
        {
        
    case XS_DEPCONST_ACCEPT:
        wiqccdi (id, XS_DEPCONST_TOP_VAL, &unused, text);
        sscanf (text, "%lf", &pxs->plo->interval.top);
        wiqccdi (id, XS_DEPCONST_BASE_VAL, &unused, text);
        sscanf (text, "%lf", &pxs->plo->interval.base);
        pxs->plo->interval.specify_as = pxs->ptemp->interval.specify_as;
        
        pxs->changed_since_display = TRUE;
        pxs->changed_since_save = TRUE;
        
        status = xs_depthconst_cdlg_term (pxs);
        status = xs_cs_menu_mgr (pxs);
        break;
        
    case XS_DEPCONST_IGNORE:
        
        status = xs_depthconst_cdlg_term (pxs);
        break;
        
    case XS_DEPCONST_MD_BTN:
        /* save button states              */
        pxs->ptemp->interval.specify_as = MEASURED_DEPTH;
        break;
        
    case XS_DEPCONST_TVD_BTN:
        pxs->ptemp->interval.specify_as = TVD;
        break;
        
    case XS_DEPCONST_SUBSEA_BTN:
        pxs->ptemp->interval.specify_as = SUBSEA_ELEV;
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Clean up depth constant dialogs.

Prototype:
    publicdef INT xs_depthconst_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_depthconst_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_depthconst_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_depthconst_cdlg_term");
    
    /* turn off flag, close dialog, reset menus    */
    pxs->depthconst_in_use = OFF;
    am_release_resource (pxs->depthconst_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_trackdef_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Init dialog to accept markers and offsets.

Prototype:
    static INT xs_top2top_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_top2top_cdlg_init (XS_STRUCT *pxs)
#else
static INT xs_top2top_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    INT id;
    CHAR text[128];
    PROGNAME ("xs_top2top_cdlg_init");
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_TOP2TOP,
                                 xs_top2top_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, OFF);
    pxs->top2top_in_use = ON;
    pxs->top2top_cdlg_id = id;
    
    /* fill in initial values              */
    wiscdt (id, XS_TOP2TOP_SELECTUPPER, pxs->plo->interval.top_select_phrase);
    sprintf (text, "%.1lf", pxs->plo->interval.top_offset);
    wiscdt (id, XS_TOP2TOP_OFFSETUPPER, text);
    
    wiscdt (id, XS_TOP2TOP_SELECTLOWER, pxs->plo->interval.base_select_phrase);
    sprintf (text, "%.1lf", pxs->plo->interval.base_offset);
    wiscdt (id, XS_TOP2TOP_OFFSETLOWER, text);
    
    status = wiopcd (id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Accept top depth range from user.

Prototype:
    static INT xs_top2top_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_top2top_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_top2top_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT unused;
    INT status;
    CHAR text[128];
    PROGNAME ("xs_top2top_cdlg_server");
    
    switch (item)
        {
        
    case XS_TOP2TOP_ACCEPT:
        wiqccdi (id, XS_TOP2TOP_SELECTUPPER, &unused, text);
        ts_sto_upper (text);
        strcpy (pxs->plo->interval.top_select_phrase, text);
        wiqccdi (id, XS_TOP2TOP_SELECTLOWER, &unused, text);
        ts_sto_upper (text);
        strcpy (pxs->plo->interval.base_select_phrase, text);
        wiqccdi (id, XS_TOP2TOP_OFFSETUPPER, &unused, text);
        sscanf (text, "%lf", &pxs->plo->interval.top_offset);
        wiqccdi (id, XS_TOP2TOP_OFFSETLOWER, &unused, text);
        sscanf (text, "%lf", &pxs->plo->interval.base_offset);
        
        pxs->changed_since_display = TRUE;
        pxs->changed_since_save = TRUE;
        
        status = xs_top2top_cdlg_term (pxs);
        status = xs_cs_menu_mgr (pxs);
        break;
        
    case XS_TOP2TOP_IGNORE:
        
        status = xs_top2top_cdlg_term (pxs);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Finish up top to top data interval selection.

Prototype:
    publicdef INT xs_top2top_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.


Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_top2top_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_top2top_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_top2top_cdlg_term");
    
    /* turn off in-use flag, take down dialog,     */
    /* reset menu                  */
    pxs->top2top_in_use = OFF;
    am_release_resource (pxs->top2top_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_trackdef_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Init dialog to accept depth zone interval.

Prototype:
    static INT xs_zonesel_cdlg_init(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_zonesel_cdlg_init (XS_STRUCT *pxs)
#else
static INT xs_zonesel_cdlg_init (pxs)
XS_STRUCT *pxs;
#endif
    {
    static INT max_selectable = 1;
    INT status;
    INT id;
    CHAR text[128];
    INT i;
    INT j;
    INT num_items;
    PROGNAME ("xs_zonesel_cdlg_init");
    
    status = am_define_resource (AM_DIALOG, XS_CDLGS_FILE, XS_ZONESEL,
                                 xs_zonesel_cdlg_server, &id, pxs);
    
    wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, OFF);
    wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, OFF);
    pxs->zonesel_in_use = ON;
    pxs->zonesel_cdlg_id = id;
    
    /* set up top and base zone offset values      */
    sprintf (text, "%.1lf", pxs->plo->interval.ztop_offset);
    wiscdt (id, XS_ZONESEL_TOPOFFSET, text);
    
    sprintf (text, "%.1lf", pxs->plo->interval.zbase_offset);
    wiscdt (id, XS_ZONESEL_BOTOFFSET, text);
    
    /* go get zone list tcp and then set selector  */
    status = xs_gen_zone_list (pxs);
    
    num_items = ts_tcp_len (pxs->zone_list_tcp);
    
    if (num_items <= 0)
        {
        status = am_message (AM_ALERT, mg_message (XS_NO_ZONE_AVAIL));
        status = am_release_resource (pxs->zonesel_cdlg_id);
        status = xs_dataint_cdlg_init (pxs);
        return SUCCESS;
        }
    wisselt (id, XS_ZONESEL_LIST, XS_ZONESEL_VSC, num_items, max_selectable,
             pxs->zone_list_tcp);
    
    /* now compare strings to reactivate the       */
    /* selector                    */
    j = 0;
    for (i = 0; i < num_items; i++)
        {
        if (strcmp (pxs->zone_list_tcp[i], pxs->plo->interval.zone_name) == 0)
            j = i + 1;
        }
    wi_set_default_sel (id, XS_ZONESEL_LIST, j);
    
    status = wiopcd (id);
    
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Accept depth zone from user.

Prototype:
    static INT xs_zonesel_cdlg_server(INT id, INT item, XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Resource id.
    item            -(INT) Event.
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xs_prefs_2.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_zonesel_cdlg_server (INT id, INT item, XS_STRUCT *pxs)
#else
static INT xs_zonesel_cdlg_server (id, item, pxs)
INT id;
INT item;
XS_STRUCT *pxs;
#endif
    {
    INT unused;
    INT status;
    CHAR text[128];
    INT selected[1];
    PROGNAME ("xs_zonesel_cdlg_server");
    
    switch (item)
        {
        
    case XS_ZONESEL_ACCEPT:
        wiqccdi (id, XS_ZONESEL_TOPOFFSET, &unused, text);
        sscanf (text, "%lf", &pxs->plo->interval.ztop_offset);
        wiqccdi (id, XS_ZONESEL_BOTOFFSET, &unused, text);
        sscanf (text, "%lf", &pxs->plo->interval.zbase_offset);
        wiqsel (id, XS_ZONESEL_LIST, &unused, selected);
	if (unused < 1)
	  {
	  wi_beep(1);
	  am_message (AM_ALERT, "There is no ZONE been selected.");
	  }
	else
	  {
        strcpy (pxs->plo->interval.zone_name, pxs->zone_list_tcp[selected[0] - 1]);
        
        pxs->changed_since_display = TRUE;
        pxs->changed_since_save = TRUE;
        
        status = xs_zonesel_cdlg_term (pxs);
        status = xs_cs_menu_mgr (pxs);
        break;
	  }
        
    case XS_ZONESEL_IGNORE:
        
        status = xs_zonesel_cdlg_term (pxs);
        break;
        
    default:
        break;
        
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Take down zone selection dialog.

Prototype:
    publicdef INT xs_zonesel_cdlg_term(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to XSEC struct.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_zonesel_cdlg_term (XS_STRUCT *pxs)
#else
publicdef INT xs_zonesel_cdlg_term (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    PROGNAME ("xs_zonesel_cdlg_term");
    
    /* free the tcp, reset in-use flag, close      */
    /* dialog, reset menu              */
    status = ts_tcp_free (pxs->zone_list_tcp);
    pxs->zone_list_tcp = (CHAR **)0;
    pxs->zonesel_in_use = OFF;
    am_release_resource (pxs->zonesel_cdlg_id);
    if (pxs->review_all_switch == ON)
        status = xs_trackdef_cdlg_init (pxs);
    else
        {
        wienmni (pxs->edit_menu_id, INTERVAL_MENU_ITEM, ON);
        wienmni (pxs->edit_menu_id, CHANGE_ALL_MENU_ITEM, ON);
        }
    return SUCCESS;
    
    }
/* END:     */
