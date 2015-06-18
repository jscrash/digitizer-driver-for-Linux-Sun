/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPSTUP.C*/
/* *6    18-JUN-1991 16:10:34 KEE "(SPR -1) comment out call to wiscdv = wi_set_dialog_value(cdlgid, cditemid, ON) where cditemid*/
/*= pushbutton.  wiscdv does not support pushbutton. case 2983"*/
/* *5    17-AUG-1990 22:20:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    26-MAR-1990 12:00:06 KEE "(SPR -1) Fixing bug of new (sx_cdlgs-131) dialog "*/
/* *3    23-MAR-1990 23:54:18 KEE "(SPR -1) change sx application for new dialog (Select the Best Label Position)"*/
/* *2    25-JUL-1989 20:11:14 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPSTUP.C*/
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

#include "esi_sx.h"
#include "sx_cdlgs.rh"
#include "esi_wi.h"

/* Function Description -----------------------------------------------------
Description:
    Control radio buttons on clp dialog.

Prototype:
    publicdef INT sx_dialog_clp_setup(SX_CLP_STRUCT *pclp,INT id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pclp            -(SX_CLP_STRUCT *)
    id              -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_dialog_clp_setup (SX_CLP_STRUCT *pclp, INT id)
#else
publicdef INT sx_dialog_clp_setup (pclp, id)
SX_CLP_STRUCT *pclp;
INT id;
#endif
    {
    INT status;
    INT iang;
    INT enable_control1;
    INT enable_control2;
    INT enable_control3;
    
    /* assume success */
    
    status = SUCCESS;
    
    /* set up suppress button */
    
    if (pclp->paraperp == SO_NOPARA)
        {
        wiscdv (id, ITEM_CLP_SUPPRESS, ON);
        }
    else
        {
        wiscdv (id, ITEM_CLP_SUPPRESS, OFF);
        }
    /* get even 45 degree integer angle */
    
    iang = pclp->angle + 0.0001;
    enable_control1 = OFF;
    enable_control2 = OFF;
    enable_control3 = OFF;
    
    /* set up for each valid angle */
    
    switch (iang)
        {
        
    case 0:
/*        wiscdv (id, ITEM_CLP_CENTERED_RIGHT, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = ON;
        enable_control2 = ON;
        enable_control3 = OFF;
        wiscdv (id, ITEM_CLP_HORIZONTAL, ON);
        break;
        
    case 45:
/*        wiscdv (id, ITEM_CLP_ABOVE_RIGHT, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = OFF;
        enable_control2 = OFF;
        enable_control3 = ON;
        wiscdv (id, ITEM_CLP_ANGLED, ON);
        break;
        
    case 90:
/*        wiscdv (id, ITEM_CLP_ABOVE_CENTERED, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = ON;
        enable_control2 = ON;
        enable_control3 = OFF;
        wiscdv (id, ITEM_CLP_VERTICAL, ON);
        break;
        
    case 135:
/*        wiscdv (id, ITEM_CLP_ABOVE_LEFT, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = OFF;
        enable_control2 = OFF;
        enable_control3 = ON;
        wiscdv (id, ITEM_CLP_ANGLED, ON);
        break;
        
    case 180:
/*        wiscdv (id, ITEM_CLP_CENTERED_LEFT, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = ON;
        enable_control2 = ON;
        enable_control3 = OFF;
        wiscdv (id, ITEM_CLP_HORIZONTAL, ON);
        break;
        
    case 225:
/*        wiscdv (id, ITEM_CLP_BELOW_LEFT, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = OFF;
        enable_control2 = OFF;
        enable_control3 = ON;
        wiscdv (id, ITEM_CLP_ANGLED, ON);
        break;
        
    case 270:
/*        wiscdv (id, ITEM_CLP_BELOW_CENTERED, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = ON;
        enable_control2 = ON;
        enable_control3 = OFF;
        wiscdv (id, ITEM_CLP_VERTICAL, ON);
        break;
        
    case 315:
/*        wiscdv (id, ITEM_CLP_BELOW_RIGHT, ON);*/
        pclp->paraperp = SO_PERP;
        enable_control1 = OFF;
        enable_control2 = OFF;
        enable_control3 = ON;
        wiscdv (id, ITEM_CLP_ANGLED, ON);
        break;
        }
    /* enable / diable hor, vert, ang buttons */
    
    wiencdi (id, ITEM_CLP_HORIZONTAL, enable_control1);
    wiencdi (id, ITEM_CLP_VERTICAL, enable_control2);
    wiencdi (id, ITEM_CLP_ANGLED, enable_control3);
    
    SX_RETURN;
    }
/* END:     */
