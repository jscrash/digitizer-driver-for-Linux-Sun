/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHOWN_ID.C*/
/* *5    23-OCT-1990 13:53:54 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:26:49 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:27:13 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:32:15 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHOWN_ID.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHOWN_ID.C                               */
/* *4    27-JUN-1990 17:26:49 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:27:13 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:32:15 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:54 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHOWN_ID.C                               */
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
/*NAME:    WI_CHANGE_OWNER_ID                                                       */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Change the owner value for a Dialog or Menu                                   */
/*                                                                                  */
/*    MOD: 7/24/87 this call requires a handshake from the mac to insure the        */
/*         named resource has not been freed in the mac due to lo heap              */
/*         considerations.                                                          */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions needed here */

/************************************************************************************/
INT wi_change_owner_id (input_id, new_owner_id) /*************************************************************************/

INT input_id;                       /* %P Menu or Dialog ID from WI_RDF.            */
INT new_owner_id;                   /* %P The owning application instance ID.       */
    {
    PROGNAME ("wi_change_owner_id");
#define IDENT progname
    INT id, status;
    INPUT_RECORD inp_rec;

#ifdef USE_X
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    /* Retrieve from Tree                                                           */
    id = input_id;
    if (btfind (&wiv.input_tree, &id, &inp_rec))
        {
        /* Update entry in tree.                                                    */
        inp_rec.owner_id = new_owner_id;
        btrepcur (&wiv.input_tree, &id, &inp_rec);
        
        E_CHECK_WIDGET (input_id, 0);
        (E_DIALOG_MENU (input_id))->app_id = new_owner_id;
        
        RETURN (SUCCESS);
        }
    else
        {
        /* Report not defined                                                       */
        RETURN (WI_NOT_DEFINED);
        }
#else        /* ESITERM code */
    INT respcode;
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    /* Retrieve from Tree                                                           */
    id = input_id;
    if (btfind (&wiv.input_tree, &id, &inp_rec))
        {
        /* Update entry in tree.                                                    */
        inp_rec.owner_id = new_owner_id;
        btrepcur (&wiv.input_tree, &id, &inp_rec);
        wiz_set_echo (FALSE);
        SEND_3_VALUES (new_owner_id, input_id, MC_CHANGE_OWNER);
        
        /* get the response from the mac...is the rsrc still defined?               */
        wiz_gnum (&respcode);
        wiz_acknowledge ();
        wiz_set_echo (TRUE);
        if (respcode != input_id)
            {
            btdel (&wiv.input_tree, &id);
            RETURN (WI_NOT_DEFINED);
            }
        if (input_id > 999)
            {
            wi_set_dialog_title (input_id, inp_rec.owner_title);
            }
        RETURN (SUCCESS);
        }
    else
        {
        /* Report not defined                                                       */
        RETURN (WI_NOT_DEFINED);
        }
#endif 
    }
