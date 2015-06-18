/* DEC/CMS REPLACEMENT HISTORY, Element WM_ATTACH.C*/
/* *2    18-JUL-1990 18:39:54 JESSIE "(SPR 1) change include files"*/
/* *1    19-JUN-1989 13:40:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_ATTACH.C*/

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

/* WM.ATTACH.CC,  GUY>WM>SOURCE,  GUY H BINNS,  06/02/86
   Direct attach function to any level of wm graphics structure
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : WM - Direct attach function to any level of wm graphics structure */

/* START-HISTORY :
   06/02/86 GUY      First checkin
   06/02/86 GUY H BINNS Template constructed
   END-HISTORY */

/* START-CODE : */
#include "esi_ly.h"

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_ERR_H
#include "esi_wm_err.h"
#endif

static VOID zz_up() ;
static VOID zz_down() ;
static VOID zz_down_group() ;

/* === WMOATTACH === */

INT wmoattach( app, win, tnr, gro, statusPtr )

INT app, win, tnr, gro ;
INT *statusPtr ;

{  PROGNAME("wmoattach") ;

   INT exists ;
   INT appCur, winCur, tnrCur, groCur, segCur, picCur ;
   INT appOld, winOld, tnrOld, groOld, segOld, picOld ;
   INT returned_status =SUCCESS;

   wmoexists( app, win, tnr, gro, &exists ) ;
   if (exists)
   {
      wmoinqcur( &appCur, &winCur, &tnrCur, &groCur, &segCur, &picCur ) ;
      if ((app != appCur) OR (win != winCur) OR (tnr != tnrCur) OR (gro != groCur) )
      {
         zz_up() ;
         zz_down( app, win, tnr, gro ) ;
         
/* Make current workstation screen match this window's setting */
      
      	wm_attach_window_screen( win );
      	
         *statusPtr = SUCCESS ;
      }
      else
      {
         *statusPtr = SUCCESS ;
      }
	}
   else
   {
      *statusPtr = FAIL ;
      returned_status = WM_APP_NOT_EXIST;
   }
   return (returned_status );
}

/* === ZZ_UP === */

static VOID zz_up()

{  PROGNAME("zz_up") ;

   INT appCur, winCur, tnrCur, groCur, segCur, picCur ;

   wmoinqcur( &appCur, &winCur, &tnrCur, &groCur, &segCur, &picCur ) ;
   if (picCur)
   {
      wmpclose( picCur ) ;
   }
   if (segCur)
   {
      wmsclose( segCur ) ;
   }
   if (groCur)
   {
      do
      {
         wmgclose( groCur ) ;
         wmoinqcur( &appCur, &winCur, &tnrCur, &groCur, &segCur, &picCur ) ;
      } while (groCur) ;
   }
   if (winCur)
   {
      wmwclose( winCur ) ;
   }
   if (appCur)
   {
      wmaclose( appCur ) ;
   }
}

/* === ZZ_DOWN === */

static VOID zz_down( appTo, winTo, tnrTo, groTo )

INT appTo, winTo, tnrTo, groTo ;

{  PROGNAME("wmoattach") ;

   if (appTo)
   {
      wmaopen( appTo ) ;
   }
   if (winTo)
   {
      wmwopen( winTo ) ;
   }
   if (tnrTo)
   {
      wmtselect( tnrTo ) ;
   }
   if (groTo)
   {
      zz_down_group( groTo ) ;
   }
}

/* === zz_down_group === */

static VOID zz_down_group( gro )

INT gro ;

{  PROGNAME("zz_down_group") ;

   INT appOld, winOld, tnrOld, groOld, segOld, picOld ;

   if (gro)
   {
      wmoinqold( gro, &appOld, &winOld, &tnrOld, &groOld, &segOld, &picOld ) ;
      zz_down_group( groOld ) ;
      wmgopen( gro ) ;
   }
}

/* === WMOEXISTS === */

INT wmoexists( app, win, tnr, gro, ifExistsPtr )

INT app, win, tnr, gro ;
INT *ifExistsPtr ;
{  PROGNAME("wmoexists") ;

   INT appOld, winOld, tnrOld, groOld, segOld, picOld ;
   INT returned_status = SUCCESS;

   *ifExistsPtr = TRUE ;

   if (!(app))
   {
      *ifExistsPtr = FALSE ;
      returned_status = WM_APP_NOT_EXIST;
   }

   if (win)
   {
      wmoinqold( win, &appOld, &winOld, &tnrOld, &groOld, &segOld, &picOld ) ;
      if (appOld != app)
      {
         *ifExistsPtr = FALSE ;
      returned_status = WM_WINDOW_NOT_EXIST;
      }
   }

   if (tnr)
   {
      wmoinqold( tnr, &appOld, &winOld, &tnrOld, &groOld, &segOld, &picOld ) ;
      if (winOld != win)
      {
         *ifExistsPtr = FALSE ;
         returned_status = WM_XFORM_NOT_EXIST;
      }
   }

   if (gro)
   {
      wmoinqold( gro, &appOld, &winOld, &tnrOld, &groOld, &segOld, &picOld ) ;
      if (winOld != win)
      {
         *ifExistsPtr = FALSE ;
         returned_status = WM_GROUP_NOT_EXIST;
      }
   }
   return returned_status;
}

/* === WMOWHERE === */

INT wmowhere( appPtr, winPtr, tnrPtr, groPtr, statusPtr )

INT *appPtr, *winPtr, *tnrPtr, *groPtr ;
INT *statusPtr ;

{  PROGNAME("wmowhere") ;

   INT seg, pic ;

   *statusPtr = wmoinqcur( appPtr, winPtr, tnrPtr, groPtr, &seg, &pic ) ;
   return *statusPtr;
}

/* END-CODE */
