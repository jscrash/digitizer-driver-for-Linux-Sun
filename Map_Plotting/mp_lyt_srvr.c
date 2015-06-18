/*  DEC/CMS REPLACEMENT HISTORY, Element MP_LYT_SRVR.C*/
/*  *3    18-OCT-1990 12:03:56 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*   1B1  12-OCT-1990 17:17:50 GILLESPIE "Merge Ernie Deltas"*/
/*  *2    17-AUG-1990 22:06:43 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:12:55 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_LYT_SRVR.C*/
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

#include "esi_mp.h"
#include "mp_menus.rh"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Service events from the layout menu.                                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mp_layout_server(INT id,INT item,BYTE *p);                      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    id              -(INT)                                                        */
/*    item            -(INT)                                                        */
/*    p               -(BYTE *)                                                     */
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
publicdef INT mp_layout_server (INT id, INT item, BYTE *p)
#else
publicdef INT mp_layout_server (id, item, p)
INT id;
INT item;
BYTE *p;
#endif
    {
    INT status = SUCCESS;
    
    /********************************************************************************/
    
    switch (item)
        {
    case MP_PLOT_LAYOUT:
        or_run_oracle_form ("MP_PLOT_COMPOSER");
        break;
        
    case MP_TITLE_BLOCK:
        or_run_oracle_form ("MP_TITLE_BLOCK");
        break;
        }
    return status;
    }
/* END:                                                                             */
