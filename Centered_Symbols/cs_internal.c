/* DEC/CMS REPLACEMENT HISTORY, Element CS_INTERNAL.C*/
/* *3    14-AUG-1991 16:36:53 JTM "(SPR 0) Cast GKS_REAL to DOUBLE for wm_gslwsc()."*/
/* *2    27-JUL-1989 13:42:54 GILLESPIE "(SPR 1) Fix monor cast problems"*/
/* *1    19-JUN-1989 12:44:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CS_INTERNAL.C*/

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

/* CS_INTERNAL.CC,  GUY>CS>SOURCE,  GUY H BINNS,  06/24/86
   Internal functions for CS_CENTERED_SYMBOLS
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : CS_INTERNAL - Internal functions for CS_CENTERED_SYMBOLS */

/* START-HISTORY :
   06/24/86 GUY      Standard symbol radius is now in NWC not NDC
   06/19/86 GUY      Typo
   06/13/86 GUY      First release
   06/10/86 GUY      First checkin of prototype
   06/04/86 GUY H BINNS Template constructed
   END-HISTORY */

/* START-CODE : */
#include "esi_c_lib.h"
#include "esi_cs.h"

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif


/* Group Tree */
typedef struct group_tree_key
{
   ENTRYNAME string ;
} GROUP_TREE_KEY ;

typedef struct group_tree_data
{
   TREEHEAD *treePtr ;
} GROUP_TREE_DATA ;

/* Symbol Tree */
typedef struct symbol_tree_key
{
   INT integer ;
} SYMBOL_TREE_KEY ;

typedef struct symbol_tree_data
{
   INT numPrim ;
   INT numPoints ;
   INT *primTypes ;
   INT *primNpts ;
   DOUBLE *primXArray ;
   DOUBLE *primYArray ;
} SYMBOL_TREE_DATA ;

static INT    csIfInit      = FALSE ;
static INT    csNumAllocate = 0 ;
static INT    csNumFree     = 0 ;
static DOUBLE csRadiusX     = 0.0 ;
static DOUBLE csRadiusY     = 0.0 ;

static GROUP_TREE_KEY  csGroupKey  = {0} ;
static GROUP_TREE_DATA csGroupData = {0} ;
static TREEHEAD        csGroupTree = (TREEHEAD)0 ;

static SYMBOL_TREE_KEY  csSymbolKey  = {0} ;
static SYMBOL_TREE_DATA csSymbolData = {0} ;
static TREEHEAD         *csSymbolTreePtr = (TREEHEAD *)0 ;

static GKS_REAL gks_px[CS_MAX_POINTS] = {0.0} ;
static GKS_REAL gks_py[CS_MAX_POINTS] = {0.0} ;

static GKS_INT  gpl_ltype  = 0 ;
static GKS_INT  gpl_coli   = 0 ;
static GKS_REAL gpl_lwidth = 0.0 ;
static GKS_INT  gfa_ints   = 0 ;
static GKS_INT  gfa_styli  = 0 ;
static GKS_INT  gfa_coli   = 0 ;

/* misc macros */
/* STRNCPY note - because of addressing, to[max-1] is the one to null */
/* the cs routines pass max which is the length of the string's allocation */
#define STRNCPY_TO_UPPER(to,from,max) \
        { \
           strncpy( to, from, max ) ; \
	   to[(max)-1] = 0; \
           ts_sto_upper( to ) ; \
        }

/* error macros */
#define IF_NOT_INIT_RETURN \
           if (!(csIfInit)) \
           { \
              return (CS_ERR_NOT_INIT) ; \
           }

#define IF_NOT_VALID_GROUP_RETURN(g) \
           if ( (strlen(g) <= 0) OR (strlen(g) > MAXENTRYLENGTH) ) \
           { \
              return (CS_ERR_INVALID_GROUP) ; \
           }

#define IF_NOT_VALID_SYMBOL_RETURN(s) \
           if (s <= 0) \
           { \
              return (CS_ERR_INVALID_SYMBOL) ; \
           }

#if USE_PROTOTYPES

/* Internal Routines */
static INT initGroupTree( TREEHEAD *treePtr );
static INT termGroupTree(TREEHEAD *treePtr ) ;
static INT setGroup(ENTRYNAME group) ;

static INT initSymbolTree(TREEHEAD *treePtr ) ;
static INT termSymbolTree(TREEHEAD *treePtr ) ;
static INT setSymbol(INT symbol) ;

static INT setScale(DOUBLE scale) ;
static INT setColor(INT color) ;
static INT restoreColor(void) ;
static INT drawSymbolData(SYMBOL_TREE_DATA *symDataPtr,
    DOUBLE wx, DOUBLE wy);
static INT loadGksPoints(DOUBLE wx, DOUBLE wy, INT n,
    DOUBLE fx[], DOUBLE fy[] );
static CHAR *zzAllocate(INT numbytes) ;
static INT zzFree(VOIDPTR ptr) ;

#else

static INT initGroupTree();
static INT termGroupTree() ;
static INT setGroup() ;

static INT initSymbolTree() ;
static INT termSymbolTree() ;
static INT setSymbol() ;

static INT setScale() ;
static INT setColor() ;
static INT restoreColor() ;
static INT drawSymbolData() ;
static INT loadGksPoints() ;
static CHAR *zzAllocate() ;
static INT zzFree() ;

#endif

/* === CSZ_INIT === */

publicdef INT csz_Init()

{
   INT status = SUCCESS ;

   csGroupKey.string[0] = NUL ;
   csSymbolKey.integer = 0 ;

   if (!(csIfInit))
   {
      status = initGroupTree( &csGroupTree ) ;
      if (status == SUCCESS)
      {
         csIfInit = TRUE ;
      }
   }
   else
   {
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* === CSZ_TERM === */

publicdef INT csz_Term()

{
   INT status = SUCCESS ;

   if (csIfInit)
   {
      status = termGroupTree( &csGroupTree ) ;
   }
   else
   {
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* === CSZ_QUERYGROUP === */

publicdef INT csz_QueryGroup( group, numSymbolsPtr )

ENTRYNAME group ;
INT *numSymbolsPtr ;

{

   INT status = SUCCESS ;
   ENTRYNAME groupName ;

   *numSymbolsPtr = 0 ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status == SUCCESS)
   {
      *numSymbolsPtr = btcount( csSymbolTreePtr ) ;
   }

   return ( status ) ;
}

/* === CSZ_ADDGROUP === */

publicdef INT csz_AddGroup( group )

ENTRYNAME group ;

{

   INT status = SUCCESS ;
   GROUP_TREE_KEY groKey ;
   GROUP_TREE_DATA groData ;
   ENTRYNAME groupName ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status != SUCCESS)
   {
      csSymbolTreePtr = (TREEHEAD *)zzAllocate( sizeof(TREEHEAD) ) ;
      status = initSymbolTree( csSymbolTreePtr ) ;
      if (status == SUCCESS)
      {
         strncpy( groKey.string, groupName, MAXENTRYLENGTH ) ;
         groKey.string[MAXENTRYLENGTH-1] = 0;
         groData.treePtr = csSymbolTreePtr ;
         btadd( &csGroupTree, &groKey, &groData ) ;
         status = setGroup( groupName ) ;
      }
   }

   return ( status ) ;
}

/* === CSZ_DELETEGROUP === */

publicdef INT csz_DeleteGroup( group )

ENTRYNAME group ;

{

   INT status = SUCCESS ;
   ENTRYNAME groupName ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status == SUCCESS)
   {
      status = termSymbolTree( csSymbolTreePtr ) ;
      zzFree( (VOIDPTR)csSymbolTreePtr ) ;
      if (status == SUCCESS)
      {
         btdel( &csGroupTree, &csGroupKey ) ;
         csGroupKey.string[0] = NUL ;
      }
   }
   else
   {
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* === CSZ_QUERYSYMBOL === */

publicdef INT csz_QuerySymbol( group, symbol, numPrimPtr, numPointsPtr )

ENTRYNAME group ;
INT symbol ;
INT *numPrimPtr, *numPointsPtr ;

{

   INT status = SUCCESS ;
   ENTRYNAME groupName ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;
   IF_NOT_VALID_SYMBOL_RETURN( symbol ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status == SUCCESS)
   {
      status = setSymbol( symbol ) ;
   }
   if (status == SUCCESS)
   {
      *numPrimPtr = csSymbolData.numPrim ;
      *numPointsPtr = csSymbolData.numPoints ;
   }

   return ( status ) ;
}

/* === CSZ_ADDSYMBOL === */

publicdef INT csz_AddSymbol( group, symbol, numPrim, numPoints,
                          typesArray, nptsArray, xArray, yArray )

ENTRYNAME group ;
INT symbol, numPrim, numPoints ;
INT typesArray[], nptsArray[] ;
DOUBLE xArray[], yArray[] ;

{

   INT status = SUCCESS ;
   INT i ;
   SYMBOL_TREE_KEY symKey ;
   SYMBOL_TREE_DATA symData ;
   ENTRYNAME groupName ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;
   IF_NOT_VALID_SYMBOL_RETURN( symbol ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status == SUCCESS)
   {
      status = setSymbol( symbol ) ;
      if (status == SUCCESS )
      {
         csz_DeleteSymbol( groupName, symbol ) ;
      }

      if ( (numPrim > 0 && numPrim <= CS_MAX_PRIMITIVES) &&
           (numPoints > 0 && numPoints <= CS_MAX_POINTS) )
      {
         symKey.integer = symbol ;
         symData.numPrim = numPrim ;
         symData.numPoints = numPoints ;
         symData.primTypes = (INT *) zzAllocate( (numPrim * sizeof(INT)) ) ;
         symData.primNpts = (INT *) zzAllocate( (numPrim * sizeof(INT)) ) ;
         symData.primXArray = (DOUBLE *) zzAllocate( (numPoints * sizeof(DOUBLE)) ) ;
         symData.primYArray = (DOUBLE *) zzAllocate( (numPoints * sizeof(DOUBLE)) ) ;
         for (i=0;i<numPrim;i++)
         {
            symData.primTypes[i] = typesArray[i] ;
            symData.primNpts[i] = nptsArray[i] ;
         }
         for (i=0;i<numPoints;i++)
         {
            symData.primXArray[i] = xArray[i] ;
            symData.primYArray[i] = yArray[i] ;
         }
         btadd( csSymbolTreePtr, &symKey, &symData ) ;
         status = setSymbol( symbol ) ;
      }
      else
      {
         status = CS_ERR_INVALID_PARAMETER ;
      }
   }

   return ( status ) ;
}

/* === CSZ_DELETESYMBOL === */

publicdef INT csz_DeleteSymbol( group, symbol )

ENTRYNAME group ;
INT symbol ;

{

   INT status = SUCCESS ;
   ENTRYNAME groupName ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;
   IF_NOT_VALID_SYMBOL_RETURN( symbol ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status == SUCCESS)
   {
      status = setSymbol( symbol ) ;
   }
   if (status == SUCCESS)
   {
      btdel( csSymbolTreePtr, &csSymbolKey ) ;
      csSymbolKey.integer = 0 ;
   }

   return ( status ) ;
}

/* === CSZ_DRAWSYMBOLS === */

publicdef INT csz_DrawSymbols( group, symbol, scale, color, npts, px, py )

ENTRYNAME group ;
INT symbol ;
DOUBLE scale ;
INT color, npts ;
DOUBLE px[], py[] ;

{

   INT status = SUCCESS ;
   INT i ;
   ENTRYNAME groupName ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;
   IF_NOT_VALID_SYMBOL_RETURN( symbol ) ;

   STRNCPY_TO_UPPER( groupName, group, MAXENTRYLENGTH ) ;

   status = setGroup( groupName ) ;
   if (status == SUCCESS)
   {
      status = setSymbol( symbol ) ;
   }
   if (status == SUCCESS)
   {
      if (npts > 0)
      {
         setScale( scale ) ;
         setColor( color ) ;
         for (i=0;i<npts;i++)
         {
            status = drawSymbolData( &csSymbolData, px[i], py[i] ) ;
            if (status != SUCCESS)
            {
               break ;
            }
         }
         restoreColor() ;
      }
      else
      {
         status = CS_ERR_INVALID_PARAMETER ;
      }
   }

   return ( status ) ;
}


/* === CSZ_SCALETORADII === */

publicdef INT csz_ScaleToRadii( scale, xRadiusPtr, yRadiusPtr )

DOUBLE scale ;
DOUBLE *xRadiusPtr, *yRadiusPtr ;

{

   INT status = SUCCESS ;
   DOUBLE ndc2nwcX, ndc2nwcY ;
   DOUBLE nwc2wcsX, nwc2wcsY ;
   INT app, win, tnr, gro, seg, pic, color ;
   FLOAT ndcMinX, ndcMaxX, ndcMinY, ndcMaxY ;
   FLOAT nwcMinX, nwcMaxX, nwcMinY, nwcMaxY ;
   FLOAT wcsMinX, wcsMaxX, wcsMinY, wcsMaxY ;

   wmoinqcur( &app, &win, &tnr, &gro, &seg, &pic ) ;
   if (win)
   {
      wmwinq( win, &ndcMinX, &ndcMaxX, &ndcMinY, &ndcMaxY, &color ) ;
      wmtinq( tnr, &nwcMinX, &nwcMaxX, &nwcMinY, &nwcMaxY,
                   &wcsMinX, &wcsMaxX, &wcsMinY, &wcsMaxY ) ;
      ndc2nwcX = 1.0 / (ndcMaxX - ndcMinX) ;
      ndc2nwcY = 1.0 / (ndcMaxY - ndcMinY) ;
      nwc2wcsX = (wcsMaxX - wcsMinX) / (nwcMaxX - nwcMinX) ;
      nwc2wcsY = (wcsMaxY - wcsMinY) / (nwcMaxY - nwcMinY) ;

/* Use the the nwc of the smallest side in ndc */
      if (ndc2nwcX < ndc2nwcY)
      {
         *xRadiusPtr = scale * CS_NWC_RADIUS * nwc2wcsX ;
         *yRadiusPtr = scale * (CS_NWC_RADIUS * (ndc2nwcY / ndc2nwcX)) * nwc2wcsY ;
      }
      else
      {
         *xRadiusPtr = scale * (CS_NWC_RADIUS * (ndc2nwcX / ndc2nwcY)) * nwc2wcsX ;
         *yRadiusPtr = scale * CS_NWC_RADIUS * nwc2wcsY ;
      }
      status = SUCCESS ;
   }
   else
   {
      status = CS_ERR_WM_ERROR ;
   }

   return ( status ) ;
}

/* === SETGROUP === */

static INT setGroup( group )

ENTRYNAME group ;

{

   INT status = SUCCESS ;
   GROUP_TREE_KEY groKey ;
   GROUP_TREE_DATA groData ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( group ) ;

   if ( strncmp( group, csGroupKey.string, MAXENTRYLENGTH ) )
   {
      strncpy( groKey.string, group, MAXENTRYLENGTH ) ;
      groKey.string[MAXENTRYLENGTH-1] = 0;
      if ( btfind( &csGroupTree, &groKey, &groData ) )
      {
         csGroupKey = groKey ;
         csGroupData = groData ;
         csSymbolTreePtr = groData.treePtr ;
         csSymbolKey.integer = 0 ;
         status = SUCCESS ;
      }
      else
      {
         status = CS_ERR_GROUP_DOES_NOT_EXIST ;
      }
   }
   else
   {
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* === SETSYMBOL === */

static INT setSymbol( symbol )

INT symbol ;

{

   INT status = SUCCESS ;
   SYMBOL_TREE_KEY symKey ;
   SYMBOL_TREE_DATA symData ;

   IF_NOT_INIT_RETURN ;
   IF_NOT_VALID_GROUP_RETURN( csGroupKey.string ) ;
   IF_NOT_VALID_SYMBOL_RETURN( symbol ) ;

   if (symbol != csSymbolKey.integer)
   {
      symKey.integer = symbol ;
      if ( btfind( csSymbolTreePtr, &symKey, &symData ) )
      {
         csSymbolKey = symKey ;
         csSymbolData = symData ;
         status = SUCCESS ;
      }
      else
      {
         status = CS_ERR_SYMBOL_DOES_NOT_EXIST ;
      }
   }
   else
   {
      status = SUCCESS ;
   }

   return ( status ) ;
}

/* === INITGROUPTREE === */

static INT initGroupTree( treePtr )

TREEHEAD *treePtr ;

{

   INT status = SUCCESS ;
   INT type, keySize, dataSize, flags ;

   type = 0 ;
   keySize = sizeof( GROUP_TREE_KEY ) ;
   dataSize = sizeof( GROUP_TREE_DATA ) ;
   flags = BT_KEY_IS_C_STRING | BT_TREE_SYSTEM ;
   btinit( treePtr, type, keySize, dataSize, flags ) ;
   status = SUCCESS ;

   return ( status ) ;
}

/* === TERMGROUPTREE === */

static INT termGroupTree( treePtr )

TREEHEAD *treePtr ;

{

   INT status = SUCCESS ;
   INT i, numItems ;
   GROUP_TREE_KEY groKey ;
   GROUP_TREE_DATA groData ;

   numItems = btcount( treePtr ) ;
   if (numItems > 0)
   {
      for (i=0;i<numItems;i++)
      {
         if (i == 0)
         {
            btfirst( treePtr, &groKey, &groData ) ;
         }
         else
         {
            btnext( treePtr, &groKey, &groData ) ;
         }
         csSymbolTreePtr = groData.treePtr ;
         status = termSymbolTree( csSymbolTreePtr ) ;
         zzFree( (VOIDPTR)csSymbolTreePtr ) ;
         if (status != SUCCESS)
         {
            break ;
         }
      }
   }
   else
   {
      status = SUCCESS ;
   }
   btfree( treePtr ) ;

   return ( status ) ;
}

/* === INITSYMBOLTREE === */

static INT initSymbolTree( treePtr )

TREEHEAD *treePtr ;

{

   INT status = SUCCESS ;
   INT type, keySize, dataSize, flags ;

   type = 0 ;
   keySize = sizeof( SYMBOL_TREE_KEY ) ;
   dataSize = sizeof( SYMBOL_TREE_DATA ) ;
   flags = BT_TREE_SYSTEM ;
   btinit( treePtr, type, keySize, dataSize, flags ) ;

   status = SUCCESS ;
   return ( status ) ;
}

/* === TERMSYMBOLTREE === */

static INT termSymbolTree( treePtr )

TREEHEAD *treePtr ;

{

   INT status = SUCCESS ;
   INT i, numItems ;
   SYMBOL_TREE_KEY symKey ;
   SYMBOL_TREE_DATA symData ;

   numItems = btcount( treePtr ) ;
   if (numItems > 0)
   {
      for (i=0;i<numItems;i++)
      {
         if (i == 0)
         {
            btfirst( treePtr, &symKey, &symData ) ;
         }
         else
         {
            btnext( treePtr, &symKey, &symData ) ;
         }
         zzFree( (VOIDPTR)symData.primTypes ) ;
         zzFree( (VOIDPTR)symData.primNpts ) ;
         zzFree( (VOIDPTR)symData.primXArray ) ;
         zzFree( (VOIDPTR)symData.primYArray ) ;
      }
   }
   btfree( treePtr ) ;
   status = SUCCESS ;

   return ( status ) ;
}

/* === LOADGKSPOINTS === */

static INT loadGksPoints( wx, wy, n, fx, fy )

DOUBLE wx, wy ;
INT n ;
DOUBLE fx[], fy[] ;

{
   INT status = SUCCESS ;
   INT i ;

   if (n > 0)
   {
      for (i=0;i<n;i++)
      {
         gks_px[i] = wx + (fx[i] * csRadiusX) ;
         gks_py[i] = wy + (fy[i] * csRadiusY) ;
      }
      status = SUCCESS ;
   }
   else
   {
      status = CS_ERR_INVALID_PARAMETER ;
   }

   return ( status ) ;
}

/* === ZZALLOCATE === */

static CHAR *zzAllocate( numBytes )

INT numBytes ;

{

   INT status = SUCCESS ;

   csNumAllocate++ ;
   return ( (CHAR *)tc_alloc( numBytes ) ) ;
}

/* === ZZFREE === */

static INT zzFree( charPtr )

VOIDPTR charPtr ;

{

   if (charPtr != NULL)
   {
      csNumFree-- ;
      tc_free( charPtr ) ;
   }
}

/* === DRAWSYMBOLDATA === */

static INT drawSymbolData( symDataPtr, wx, wy )

SYMBOL_TREE_DATA *symDataPtr ;
DOUBLE wx, wy ;

{

   INT i, primCount, pointCount ;
   GKS_INT npts ;
   GKS_REAL *xPtr, *yPtr ;

   primCount = symDataPtr->numPrim ;
   pointCount = symDataPtr->numPoints ;
   if ( (primCount > 0) && (pointCount > 0) && (pointCount <= CS_MAX_POINTS) )
   {
      loadGksPoints( wx, wy, pointCount, symDataPtr->primXArray,
                                         symDataPtr->primYArray ) ;
      xPtr = gks_px ;
      yPtr = gks_py ;
      for (i=0;i<primCount;i++)
      {
         npts = symDataPtr->primNpts[i] ;
         if (npts > 0)
         {
            switch (symDataPtr->primTypes[i])
            {
               case CS_POLYLINE:
               {
                  wm_gpl( npts, xPtr, yPtr ) ;
                  break ;
               }
               case CS_FILLED_AREA:
               {
                  wm_gfa( npts, xPtr, yPtr ) ;
                  break ;
               }
            }
            xPtr += npts ;
            yPtr += npts ;
         }
      }
   }
   return SUCCESS ;

}

/* === SETSCALE === */

static INT setScale( scale )

DOUBLE scale ;

{

   INT status = SUCCESS ;
   DOUBLE xRadius, yRadius ;

   status = csz_ScaleToRadii( scale, &xRadius, &yRadius ) ;
   if (status == SUCCESS)
   {
      csRadiusX = xRadius ;
      csRadiusY = yRadius ;
   }

   return ( status ) ;
}

/* === SETCOLOR === */

static INT setColor( color )

INT color ;

{

   INT status = SUCCESS ;
   GKS_INT errind, ltype, coli, ints, styli ;
   GKS_REAL lwidth ;

   wm_gqln( &errind, &gpl_ltype ) ;
   wm_gqlwsc( &errind, &gpl_lwidth ) ;
   wm_gqplci( &errind, &gpl_coli ) ;
   wm_gqfais( &errind, &gfa_ints ) ;
   wm_gqfasi( &errind, &gfa_styli ) ;
   wm_gqfaci( &errind, &gfa_coli ) ;

   ltype  = 1     ; wm_gsln( ltype ) ;
   lwidth = 1.0   ; wm_gslwsc( (DOUBLE) lwidth ) ;
   coli   = color ; wm_gsplci( coli ) ;
   ints   = SOLID ; wm_gsfais( ints ) ;
   styli  = 1     ; wm_gsfasi( styli ) ;
   coli   = color ; wm_gsfaci( coli ) ;

   status = SUCCESS ;

   return ( status ) ;
}

/* === RESTORECOLOR === */

static INT restoreColor()

{

   INT status = SUCCESS ;

   wm_gsln( gpl_ltype ) ;
   wm_gslwsc( (DOUBLE) gpl_lwidth ) ;
   wm_gsplci( gpl_coli ) ;
   wm_gsfais( gfa_ints ) ;
   wm_gsfasi( gfa_styli ) ;
   wm_gsfaci( gfa_coli ) ;

   status = SUCCESS ;

   return ( status ) ;
}

/* END-CODE */
