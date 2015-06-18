/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSIOSRVR.C*/
/* *4     1-SEP-1989 14:39:01 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *3    11-AUG-1989 14:18:40 JULIAN "(SPR -1) statement not reached"*/
/* *2    25-JUL-1989 20:11:21 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:51 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSIOSRVR.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* PROGRAM:  SX_DIALOG_SIO_SRVR.C                                            */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: SERVER FOR SHOTPOINT FILTER TRIGGER DIALOG BOX                  */
/*                                                                           */
/*                                                                           */
/* MODIFIED BY J. R. BREEDLOVE 12/7/87 TO ELIMINATE MISLEADING ERROR         */
/* MESSAGE FROM DISPATCHER WHEN USER "PUSHES" THE CANCEL BUTTON              */
/*                                                                           */
/*****************************************************************************/
#include "esi_sx.h"
#include "sx_cdlgs.rh"
#include "sx_menus.rh"
#include "esi_wi.h"
#include "esi_gks.h"
#ifndef ESI_AM_H
#include "esi_am.h"
#endif

publicdef INT sx_dialog_sio_srvr(id,item,psx)
INT id;
INT item;
SX_STRUCT *psx;
{
    INT   status;
    CHAR  text[128];
    INT   type;
    INT   min,max;
    CHAR  cval;
    INT   ival;

    status = SUCCESS;    
    psx->default_filter = FALSE;

    /* determine type from id */

    if(id == psx->cdlg_id.dialog_sio_sym)
        {
        type = ITEM_LP_SET_SYMTRIG;
        ival = psx->pso->disp_opt.line.shot_symbol_value;
        }
    else if(id == psx->cdlg_id.dialog_sio_lab)
        {
        type = ITEM_LP_SET_LABTRIG;
        ival = psx->pso->disp_opt.line.shot_label_value;
        }
    else if(id == psx->cdlg_id.dialog_sio_zval)
        {
        type = ITEM_LP_SET_SHOTTRIG;
        ival = psx->pso->disp_opt.line.zvalue_value;
        }

    /* process dialog box event */

    switch (item)
        {
    
        case ITEM_SIO_OK:
        
        status = sx_dialog_sio_read(psx,id,type);        
        if(status == SUCCESS)
            {
            wienmni(psx->menu_id.lp,type,ON);
            am_release_resource(id);
            psx->nchanges ++;
            psx->need_to_save = TRUE;
            sx_menu_ls_setup(psx);

/* reset psx id so we know it is not active */

            if(id == psx->cdlg_id.dialog_sio_sym)
                {
                psx->cdlg_id.dialog_sio_sym = 0;
/* this code was added to be compatible with label - spr 3461 */
                wm_grsgwk(MAIN_WKID);
                so_draw_seismic_line(psx->pso);
/* end added code */
                }
            else if(id == psx->cdlg_id.dialog_sio_lab)
                {
                psx->cdlg_id.dialog_sio_lab = 0;
                wm_grsgwk(MAIN_WKID);
                so_draw_seismic_line(psx->pso);
                }
            else if(id == psx->cdlg_id.dialog_sio_zval)
                {
                psx->cdlg_id.dialog_sio_zval = 0;
/* this code was added to be compatible with label - spr 3461 */
                wm_grsgwk(MAIN_WKID);
                so_draw_seismic_line(psx->pso);
/* end added code */
                }
            }

        SX_RETURN;

/*      case ITEM_SIO_HELP: JRB 12/9/87
 *      SX_RETURN; JRB 12/9/87
 *      break;   JRB 12/9/87
added close comment 12/18/87 CEP */

        case ITEM_SIO_CANCEL:
        am_release_resource(id);
        wienmni(psx->menu_id.lp,type,ON);

    /* reset psx id so we know it is not active */

        if(id == psx->cdlg_id.dialog_sio_sym)
            {
            psx->cdlg_id.dialog_sio_sym = 0;
            }
        else if(id == psx->cdlg_id.dialog_sio_lab)
            {
            psx->cdlg_id.dialog_sio_lab = 0;
            }
        else if(id == psx->cdlg_id.dialog_sio_zval)
            {
            psx->cdlg_id.dialog_sio_zval = 0;
            }
/*      status = FAIL;  JRB 12/7/87   */
        SX_RETURN;

        case ITEM_SIO_EVERY:

        cval = 'N';
        ival = 10;
        break;

        case ITEM_SIO_NUMBERS:

        cval = 'E';
        break;

        case ITEM_SIO_NUMBER2:
        cval = 'M';
        break;

        case ITEM_SIO_ALL:
        cval = 'N';
        ival = 1;
        break;

        case ITEM_SIO_NONE:
        cval = 'N';
        ival = 0;
        break;

        default: break;
        }

    /* set appropriate trigger value */

    switch (type)
        {

        case ITEM_LP_SET_SYMTRIG:
        psx->pso->disp_opt.line.shot_symbol_trigger[0] = cval;
        psx->pso->disp_opt.line.shot_symbol_value = ival;
        break;
    
        case ITEM_LP_SET_LABTRIG:
        psx->pso->disp_opt.line.shot_label_trigger[0] = cval;
        psx->pso->disp_opt.line.shot_label_value = ival;
        break;
    
        case ITEM_LP_SET_SHOTTRIG:
        psx->pso->disp_opt.line.zvalue_trigger[0] = cval;
        psx->pso->disp_opt.line.zvalue_value = ival;
        break;
        }

    /* reset box */

    sx_dialog_sio_setup(psx,id,FALSE);
    
    SX_RETURN;
}
