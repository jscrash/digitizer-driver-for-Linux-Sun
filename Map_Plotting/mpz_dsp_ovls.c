/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_DSP_OVLS.C*/
/*   16   13-FEB-1992 16:32:14 JULIAN "(SPR 5770) added check to update MHL dlg box"*/
/*  *15    2-AUG-1991 16:55:19 MING "(SPR 0) add ov_status = SUCCESS in the non PRIOR_GKS plotfile overlay code"*/
/*  *14   16-JUL-1991 17:42:09 SANTIAGO "(SPR -1) commented out call to wm_update, unnecessary redraw"*/
/*  *13    7-JUN-1991 13:51:24 MING "(SPR 0) change #if PRIOR_GKS to #ifdef PRIOR_GKS"*/
/*  *12    7-JUN-1991 10:23:42 MING "(SPR 0) change SEISMIC TIME to SEISMIC DATA"*/
/*  *11    6-JUN-1991 15:17:00 MING "(SPR 0) add PLOT FILE overlay"*/
/*  *10    1-MAY-1991 11:27:55 KEE "(SPR -1) Change for display dls/nts overlays"*/
/*  *9    18-MAR-1991 17:10:08 JULIAN "(SPR 0) Lease Mapping init checkin"*/
/*  *8    18-MAR-1991 17:01:35 MING "(SPR -1) enable PERSPECTIVE overlay"*/
/*  *7    19-NOV-1990 10:34:08 JULIAN "(SPR 5356) dim LEASE,SEISMIC 3D,PERS... overlays"*/
/*  *6    17-AUG-1990 22:03:06 VINCE "(SPR 5644) Code Cleanup"*/
/*  *5    20-FEB-1990 10:08:15 JULIAN "(SPR 5095) culture overlay"*/
/*  *4    13-DEC-1989 14:32:00 CONROY "(SPR 5071) Mods for label plotting fix"*/
/*  *3    10-OCT-1989 16:53:47 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *2    26-JUN-1989 10:50:29 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:02 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_DSP_OVLS.C*/
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

#include    "esi_gl_defs.h"
#include    "esi_mp.h"
#include    "esi_gks.h"
#include    "esi_qs.h"
#include    "esi_am.h"
#include    "esi_md_basis.x"
#include    "esi_md_err.h"
#include    "esi_md_msg.h"

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Calls the individual overlay routines.

Prototype:
    publicdef INT mpz_display_overlays(INT start,INT end);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    start           -(INT)
    end             -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mpz_display_overlays (INT start, INT end)
#else
publicdef INT mpz_display_overlays (start, end)
INT start;
INT end;
#endif
    {
    INT status = SUCCESS;
    INT ov_status;
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT temp_row;
    INT i;
    CHAR msg[80];
    INT defmod, regmod;
    INT batch;
    
    /**********************************************************************/
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * LOOP ONCE FOR EACH OVERLAY * */
    /* ------------------------------ */
    
    qs_inq_i (QS_IF_BATCH, &batch);
    wm_inq_deferral (&defmod, &regmod);
    wm_set_deferral (GBNIL, GSUPPD);
    mp_store_workspace ();
    temp_row = -1;
    for (i = start; i < end; i++)
        {
        if (display->compiled_row[i] != temp_row)
            {
            temp_row = display->compiled_row[i];
            
            if (temp_row != -1)
                {
                mp_overlay_defaults (display->verb_ndx[i], temp_row);
                if (NOT batch)
                    {
                    mp_set_duom (i);
                    }
                if (compiled->verb_code[temp_row] != MP_GRID_OPS)
                    {
                    sprintf (msg, "%s %s", mg_message (MD_DISPLAYING_OVERLAY),
                             overlay_verbs[display->verb_ndx[i]].verb);
                    }
                else
                    {
                    sprintf (msg, "%s %s", mg_message (MD_COMPUTING_OVERLAY),
                             overlay_verbs[display->verb_ndx[i]].verb);
                    }
                am_message (AM_DIALOG, msg);
                
                switch (compiled->verb_code[temp_row])
                    {
                case MP_PERSPEC:
                case MP_CONTOURS:    /* Contours  */
                    ov_status = co_overlay (i);
                    break;
                    
                case MP_GRAT:       /* Graticule  */
                    ov_status = gr_overlay (i);
                    break;
                    
                case MP_GRA_OBJS:   /* Graphic Objects  */
                    ov_status = go_overlay (i);
                    break;
                    
                case MP_LANDGRID:   /* Land Grid  */
                    ov_status = jg_overlay (i);
                    break;

		case MP_LEASE:	    /* Lease */
/*		    am_message(AM_ALERT,"The LEASE mapping overlay \
is not implemented on this version of FINDER");
		    ov_status = SUCCESS;
*/
		    ov_status = lm_overlay (i);
		    break;

                    /* Seismic  */
                case MP_SEISMIC:
                case MP_SEISDPTH:
                case MP_SEISCHRN:
                case MP_SEISPACH:
                case MP_SEISDATA:
                case MP_MISTIE:
                    ov_status = so_overlay (i);
                    break;
                    
                    /* Seismic 3d */
                case MP_SEIS3D:
		    am_message(AM_ALERT,"The SEISMIC 3D mapping overlay \
is not implemented on this version of FINDER");
		    ov_status = SUCCESS;
/*
                    ov_status = s3_overlay (i);
*/
                    break;
                    
                    /* Wells  */
                case MP_WELLS:
                case MP_WELLDPTH:
                case MP_WELLPACH:
                    ov_status = wl_overlay (i);
                    break;
                    
                case MP_GRID_OPS:   /* Grid Operations  */
                    ov_status = ge_overlay (i);
                    break;
                    
                case MP_GRID_DSP:   /* Grid Display  */
                    ov_status = gd_overlay (i);
                    break;
                    
                case MP_XYZ_DSP:    /* XYZ File Display  */
                    ov_status = xd_overlay (i);
                    break;
                    
                case MP_TOPOGPHY:   /* Topologic Display  */
                    ov_status = to_overlay (i);
                    break;
                    
		case MP_DLS_GRID: /* DLS land grid overlay */
		    ov_status = dls_overlay(i);
		    break;

		case MP_NTS_GRID: /* NTS land grid overlay */
		    ov_status = nts_overlay(i);
		    break;
                    
                case MP_PLOTFILE:   /* Plotfile */
#ifdef PRIOR_GKS
		    ov_status = pf_overlay (i);
#else
		    am_message (AM_ALERT,"The PLOT FILE mapping overlay is not \
implemented on this version of FINDER");
                    ov_status = SUCCESS;
#endif
		    break;

                default:
                    break;
                    }
                if (ov_status != SUCCESS)
                    {
                    am_message (AM_ALERT, mg_message (ov_status));
                    status = FAIL;
                    }
                else
                    {
                    mapdef->display->overlay_displayed[i] = TRUE;
                    }
/* ------------------------------------------------------------ */
/* RJS - commented this out to remove unnecessary redraw of GKS */
/*                wm_update ();                                 */
/* ------------------------------------------------------------ */
                if (NOT batch)
                    {
                    mp_reset_duom (i);
                    }
                }
            }
        }
    am_message (AM_DIALOG, NULL);
    wm_set_deferral (defmod, regmod);
    
    return status;
    }
/* END:     */
