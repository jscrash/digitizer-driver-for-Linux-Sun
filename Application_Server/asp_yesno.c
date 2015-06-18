/* DEC/CMS REPLACEMENT HISTORY, Element ASP_YESNO.C*/
/* *3    28-JUN-1990 15:39:19 KEE "(SPR -1) use am_release_resource(cdlg_id) instead of wi_close_dialog(cdlg_id)"*/
/* *2     8-DEC-1989 18:41:02 JULIAN "(SPR 100) Change am_release_resource to wi_close_dialog"*/
/* *1    19-JUN-1989 12:00:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element ASP_YESNO.C*/

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

/* ************************************************************************

NAME:    ASP_YESNO

AUTHOR:  W. A. Jaeck, 6-Jun-86

DESCRIPTION:

    ASP_YESNO is a black pipe server.

    Please refer to the description of the AS_YESNO server.
    
*************************************************************************/

#define YESNO_RSRC "WI_YESNO"

#include "esi_gl_defs.h"
#include "esi_wi.h"
#include "esi_am.h"
#include "esi_fi.h"
#include "wi_yesno.rh"
#include "esi_asp_yesno.h"

static INT cdlg_id;

/* ********************************************************************** */
publicdef INT asp_yesno(p)
/* ********************************************************************** */

ASP_YESNO_WORKSPACE *p;
{
    INT event_class;
    INT event_id;
    INT event_item;
    INT status;

/* ********************************************************************** */

    fi_push();

    am_define_resource(AM_DIALOG,YESNO_RSRC,2,NULL_FUNCTION_PTR,&cdlg_id,p);

    if (p->cancel)
         wiencdi(cdlg_id,YES_NO_CANCEL_CANCEL,1);
    else
         wiencdi(cdlg_id,YES_NO_CANCEL_CANCEL,0);

    wiscdt(cdlg_id,YES_NO_CANCEL_QUESTION,p->question);

    wiopcd(cdlg_id);

    for ( status = FAIL; status != SUCCESS; )
       {
       wi_request_event(0,&event_class,&event_id,&event_item);
       if (event_id == cdlg_id)            /* Make sure event is from correct cdlg */
          {
          status = SUCCESS;
          switch (event_item) {

             case YES_NO_CANCEL_YES:
                  p->status = SUCCESS;
                  p->answer = 1;
                  break;

             case YES_NO_CANCEL_NO:
                  p->status = SUCCESS;
                  p->answer = 0;
                  break;

             case YES_NO_CANCEL_CANCEL:
                  p->status = AS_ERR_CANCEL;
                  break;

             default:
                  status = FAIL;
                  break;
             }                         /*  End Switch      */
          }                         /*  End If-Then     */
       else
          {
          wibeep(1);
          }                         /*  End Else        */
       }                         /*  End For Loop    */
    am_release_resource(cdlg_id); 

    fi_pop();

    return p->status;
}
