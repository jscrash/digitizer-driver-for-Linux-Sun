/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH_OWN.C*/
/* *5    23-OCT-1990 13:55:58 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:30:28 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:34:37 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:39:14 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH_OWN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH_OWN.C                               */
/* *4    27-JUN-1990 17:30:28 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:34:37 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:39:14 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:38:01 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_PUSH_OWN.C                               */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_PUSH_OWNER                                                            */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Push all menus and dialogs for a given owner.                                 */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

/************************************************************************************/
#define IDENT progname
INT wi_push_owner (owner_id)        /*************************************************************************/

INT owner_id;                       /* The owning application instance ID.          */
    {
    PROGNAME ("wi_push_owner");
    
    /********************************************************************************/

#ifdef USE_X
    INT i, status;
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    for (i = 1; i <= wiv.dialog_menu_count; i++)
        {
        E_CHECK_WIDGET (i, 0);
        if ((E_DIALOG_MENU (i))->app_id == owner_id)
            {                       /* if dialog/menu belong to owner, then close it*/
            wiz_unmanagechild_widget (i);
            RETURN (SUCCESS);
            }
        }
    RETURN (FAIL);

#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    SEND_2_VALUES (owner_id, MC_PUSH_OWNER);
    RETURN (SUCCESS);
#endif
    }
