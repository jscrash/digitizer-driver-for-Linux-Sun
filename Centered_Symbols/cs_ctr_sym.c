/* DEC/CMS REPLACEMENT HISTORY, Element CS_CTR_SYM.C*/
/* *2     1-NOV-1989 13:06:15 GILLESPIE "(SPR 1) Add initialization for ci (moved from ug_init_gks)"*/
/* *1    19-JUN-1989 12:44:46 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CS_CTR_SYM.C*/

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


/* CS_CENTERED_SYMBOLS.CC,  GUY>CS>SOURCE,  GUY H BINNS,  07/07/86
   Centered Symbol Drawing System
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : CS_CENTERED_SYMBOLS - Centered Symbol Drawing System */

/* START-HISTORY :
   07/07/86 GUY      scale miss-typed in cs_draw_symbol()
   06/24/86 GUY      Standard symbol radius is now in NWC not NDC
   06/13/86 GUY      First release
   06/10/86 GUY      First checkin of prototype
   06/04/86 GUY H BINNS Template constructed
   END-HISTORY */

/* START-CODE : */
#include "esi_math.h"
#include "esi_c_lib.h"
#include "esi_gks.h"
#include "esi_ci.h"
#include "esi_cs.h"
#include "esi_sdl.h"
#include "esi_ho_files.h"

/* CS state variables */
static INT       csInit   = 0 ;   /* if initialized   */
static ENTRYNAME csGroup  = {0} ; /* current group    */
static INT       csSymbol = 0 ;   /* current code     */
static DOUBLE    csScale  = 0.0 ; /* current scale    */
static INT       csColor  = 0 ;   /* current color    */
static INT       csStatus = 0 ;   /* global status    */

#define AUTO_INIT \
if (!(csInit)) \
{ \
   csStatus = cs_init() ; \
   if (csStatus IS_NOT_EQUAL_TO SUCCESS) \
   { \
      return ( csStatus ) ; \
   } \
}

/* === CS_INIT === */

publicdef INT cs_init()
{

   INT status ;

   strncpy( csGroup, CS_DEFAULT_GROUP, MAXENTRYLENGTH ) ;
   csGroup[MAXENTRYLENGTH-1] = 0;
/* csGroup  = CS_DEFAULT_GROUP ; */
   csSymbol = CS_DEFAULT_SYMBOL ;
   csScale  = CS_DEFAULT_SCALE ;
   csColor  = CS_DEFAULT_COLOR ;

   if (!(csInit))
   {
      ci_init();	/* initialize the command interpreter */
      status = csz_Init() ;
      if (status EQUALS SUCCESS)
      {
         status = sdl_init() ;
      }
      if (status EQUALS SUCCESS)
      {
         status = sdl_InitPolymarkerGroup() ;
      }
      if (status EQUALS SUCCESS)
      {
         csInit = TRUE ;
      }
   }
   else
   {
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* === CS_TERM === */

publicdef INT cs_term()

{
   INT status ;

   if (csInit)
   {
      status = csz_Term() ;
      csInit = FALSE ;
   }
   else
   {
      status = SUCCESS ;
   }
   return ( status ) ;
}

/* === CS_SET_GROUP === */

publicdef INT cs_set_group( group )
ENTRYNAME group ;
{

   INT status ;
   INT numSymbols ;

   AUTO_INIT ;

   status = csz_QueryGroup( group, &numSymbols ) ;
   if ( status IS_NOT_EQUAL_TO SUCCESS )
   {
      status = sdl_load( group ) ;
      status = csz_QueryGroup( group, &numSymbols ) ;
   }
   if ( status EQUALS SUCCESS )
   {
      strncpy( csGroup, group, MAXENTRYLENGTH ) ;
      csGroup[MAXENTRYLENGTH-1] = 0;
   }

   return ( status ) ;
}

/* === CS_QUERY_GROUP === */

publicdef INT cs_query_group( group )
ENTRYNAME group ;

{
   INT status ;

   AUTO_INIT ;

   strncpy( group, csGroup, MAXENTRYLENGTH ) ;
   group[MAXENTRYLENGTH-1] = 0;
   status = SUCCESS ;

   return ( status ) ;
}

/* === CS_SET_CODE === */

publicdef INT cs_set_code( code )

INT code ;

{

   INT status ;
   INT numPrim, numPoints ;

   AUTO_INIT ;

   status = csz_QuerySymbol( csGroup, code, &numPrim, &numPoints ) ;
   if ( status EQUALS SUCCESS )
   {
      csSymbol = code ;
   }

   return ( status ) ;
}

/* === CS_QUERY_CODE === */

publicdef INT cs_query_code( codePtr )

INT *codePtr ;

{
   INT status ;

   AUTO_INIT ;

   *codePtr = csSymbol ;
   status = SUCCESS ;

   return ( status ) ;
}

/* === CS_SET_SCALE_FACTOR === */

publicdef INT cs_set_scale_factor( scaleFactor )

DOUBLE scaleFactor ;

{
   INT status ;

   AUTO_INIT ;

   if (scaleFactor > 0.0)
   {
      csScale = scaleFactor ;
      status = SUCCESS ;
   }
   else
   {
      status = CS_ERR_INVALID_PARAMETER ;
   }

   return ( status ) ;
}

/* === CS_QUERY_SCALE_FACTOR === */

publicdef INT cs_query_scale_factor( scaleFactorPtr )

DOUBLE *scaleFactorPtr ;

{

   INT status ;

   AUTO_INIT ;

   *scaleFactorPtr = csScale ;
   status = SUCCESS ;

   return ( status ) ;
}

/* === CS_SET_HEIGHT === */

publicdef INT cs_set_height( height )

DOUBLE height ;

{

   INT status ;
   DOUBLE xRadius, yRadius, scale ;

   AUTO_INIT ;

   if (height > 0.0)
   {
      status = csz_ScaleToRadii( 1.0, &xRadius, &yRadius ) ;
      if (status EQUALS SUCCESS)
      {
         scale =  height / (yRadius * 2.0) ;
         status = cs_set_scale_factor( scale ) ;
      }
   }
   else
   {
      status = CS_ERR_INVALID_PARAMETER ;
   }

   return ( status ) ;
}

/* === CS_QUERY_HEIGHT === */

publicdef INT cs_query_height( heightPtr )

DOUBLE *heightPtr ;

{
   INT status ;
   DOUBLE xRadius, yRadius, scale ;

   AUTO_INIT ;

   status = cs_query_scale_factor( &scale ) ;
   if (status EQUALS SUCCESS)
   {
      status = csz_ScaleToRadii( scale, &xRadius, &yRadius ) ;
      if (status EQUALS SUCCESS)
      {
         *heightPtr = (2.0 * yRadius ) ;
      }
   }

   return ( status ) ;
}

/* === CS_SET_COLOR === */

publicdef INT cs_set_color( color )

INT color ;

{
   INT status ;

   AUTO_INIT ;

   if (color >= 0)
   {
      csColor = color ;
      status = SUCCESS ;
   }
   else
   {
      status = CS_ERR_INVALID_PARAMETER ;
   }

   return ( status ) ;
}

/* === CS_QUERY_COLOR === */

publicdef INT cs_query_color( colorPtr )

INT *colorPtr ;

{

   INT status ;

   AUTO_INIT ;

   *colorPtr = csColor ;
   status = SUCCESS ;

   return ( status ) ;
}

/* === CS_DRAW_ONE === */

publicdef INT cs_draw_one( xCenter, yCenter )

DOUBLE xCenter, yCenter ;

{

   INT status ;
   DOUBLE px[1], py[1] ;

   AUTO_INIT ;

   px[0] = xCenter ;
   py[0] = yCenter ;
   status = cs_draw_many( 1, px, py ) ;

   return ( status ) ;
}

/* === CS_DRAW_MANY === */

publicdef INT cs_draw_many(numSym, xCenters, yCenters )

INT numSym ;
DOUBLE xCenters[], yCenters[] ;

{

   INT status ;
   INT i ;

   AUTO_INIT ;

   if (numSym > 0)
   {
      status = csz_DrawSymbols( csGroup, csSymbol, csScale, csColor,
                                numSym, xCenters, yCenters ) ;
   }
   else
   {
      status = CS_ERR_INVALID_PARAMETER ;
   }

   return ( status ) ;
}

/* === CS_DRAW_SYMBOL === */

publicdef INT cs_draw_symbol( group, code, scale, color, xCenter, yCenter )

ENTRYNAME group ;
INT code, color ;
DOUBLE scale, xCenter, yCenter ;

{
   INT status ;

   status = cs_set_group( group ) ;
   if (status EQUALS SUCCESS)
   {
      status = cs_set_code( code ) ;
   }
   if (status EQUALS SUCCESS)
   {
      status = cs_set_scale_factor( scale ) ;
   }
   if (status EQUALS SUCCESS)
   {
      status = cs_set_color( color ) ;
   }
   if (status EQUALS SUCCESS)
   {
      status = cs_draw_one( xCenter, yCenter ) ;
   }

   return ( status ) ;
}

/* === CS_QUERY_GROUP_SIZE === */

publicdef INT cs_query_group_size( group, sizePtr )

ENTRYNAME group ;
INT *sizePtr ;

{
   INT status ;

   AUTO_INIT ;

   status = csz_QueryGroup( group, sizePtr ) ;
   if ( status IS_NOT_EQUAL_TO SUCCESS )
   {
      status = sdl_load( group ) ;
      status = csz_QueryGroup( group, sizePtr ) ;
   }

   return ( status ) ;
}

/* === CS_DISPLAY_GROUP === */

publicdef INT cs_display_group( group, xmin, ymin, xmax, ymax )

ENTRYNAME group ;
DOUBLE xmin, ymin, xmax, ymax ;

{
   INT status ;
   INT numSymbols, symbol, numCol, numRow, col, row ;
   DOUBLE dx, dy, wx, wy, height ;

   AUTO_INIT ;

   status = cs_query_group_size( group, &numSymbols ) ;
   if (status EQUALS SUCCESS)
   {
      status = cs_set_group( group ) ;
   }
   if (status EQUALS SUCCESS)
   {
      numCol = ( (INT)sqrt( (DOUBLE)(numSymbols - 1) ) ) + 1 ;
      numRow = numCol ;
      dx = (xmax - xmin) / (DOUBLE)numCol ;
      dy = (ymax - ymin) / (DOUBLE)numRow ;

/* %? hope it is a square coord system */
      height = MIN( dx, dy ) * 0.8 ;
      cs_set_height( height ) ;

      wy = ymax - (dy / 2.0) ;
      for (row=0;row<numRow;row++)
      {
         wx = xmin + (dx / 2.0) ;
         for (col=0;col<numCol;col++)
         {
            symbol = (row * numCol) + col + 1 ;
            status = cs_set_code( symbol ) ;
            if (status EQUALS SUCCESS)
            {
               status = cs_draw_one( wx, wy ) ;
            }
            if (status IS_NOT_EQUAL_TO SUCCESS)
            {
               break ;
            }
            wx += dx ;
         }
         if (status IS_NOT_EQUAL_TO SUCCESS)
         {
            break ;
         }
         wy -= dy ;
      }
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* END-CODE */
