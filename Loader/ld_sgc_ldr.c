/* DEC/CMS REPLACEMENT HISTORY, Element LD_SGC_LDR.C*/
/* *5    17-DEC-1990 16:55:59 JESSIE "(SPR 1000) get rid of GRID, CONTOUR and PERSPECTIVE from the menu"*/
/* *4     9-NOV-1990 10:22:17 MING "(SPR -1) Bug fix - dialog orphaned the second reenter the menu"*/
/* *3    14-AUG-1990 13:32:42 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-JUN-1989 10:47:43 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:03:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_SGC_LDR.C*/
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

#include "esi_gl_defs.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_ld_s_g_c.h"
#include "ld_s_g_c_data.rh"

/* Function Description -----------------------------------------------------
Description:
    Main driver for Scatter/Grid/Contour data loader.

Prototype:
    publicdef INT ld_s_g_c_loader(INT id, INT item, BYTE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT)   \
    (I) item        -(INT)    \
    (I) p           -(BYTE *) -Standard AM interface.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_s_g_c_loader (INT id, INT item, BYTE *p) 
#else
publicdef INT ld_s_g_c_loader (id, item, p)
INT id;
INT item;
BYTE *p;
#endif
    {
    INT status;
    LD_SGC_IDS *rsrc_id;
    LD_SGC_LOAD_STRUCT *sgc_files;
    static INT type = 1;      /* type of radio btns chosen, default = SCATTER */   

    /*-------------------------------------------------------------------------*/
    
    /* * GET WORKSPACES * */
    /* ------------------ */
    
    if (am_open_workspace ("SGC_LOADER_ID", AM_GLOBAL,
                           (VOIDPTR *) & rsrc_id) != SUCCESS)
        {
        if ((status = am_define_workspace ("SGC_LOADER_ID", AM_GLOBAL,
                                           sizeof(LD_SGC_IDS),
                                           (VOIDPTR *) & rsrc_id)) != SUCCESS)
            {
            return status;
            }
        }
    if (am_open_workspace ("LD_SGC_FILES", AM_GLOBAL,
                           (VOIDPTR *) & sgc_files) != SUCCESS)
        {
        if ((status = am_define_workspace ("LD_SGC_FILES", AM_GLOBAL,
                                           sizeof(LD_SGC_LOAD_STRUCT),
                                           (VOIDPTR *) & sgc_files)) != SUCCESS)
            {
            return status;
            }
        }
    /* * PUT UP THE DIALOG * */
    /* --------------------- */
    
    if ((status = am_define_resource (AM_DIALOG, "LD_S_G_C_DATA", LD_S_G_C_HEAD,
                                      ld_s_g_c_srvr, &(rsrc_id->main),
                                      &type)) != SUCCESS)
            {
            return status;
            }
    /* * OPEN THE DIALOG * */
    /* ------------------- */

    /* --------------------------------------------------- */
    /* get rid of GRID, CONTOUR, PERSPECTIVE from the menu */
    /* and leave SCATTER only for the time being           */
    /* --------------------------------------------------- */
    wi_enable_dialog_item(rsrc_id->main, LD_GRID_DATA, OFF);
    wi_enable_dialog_item(rsrc_id->main, LD_CONTOUR_DATA, OFF);
    wi_enable_dialog_item(rsrc_id->main, LD_PERSPEC_DATA, OFF);

    /* ----------------------------------- */
    /* set default value for radio buttons */
    /* ----------------------------------- */
    wi_set_dialog_value (rsrc_id->main, LD_SCATTER_DATA, ON);

    wi_open_dialog (rsrc_id->main);
    
    return SUCCESS;
    }
/* END:     */
