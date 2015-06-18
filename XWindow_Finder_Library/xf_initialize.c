/* DEC/CMS REPLACEMENT HISTORY, Element XF_INITIALIZE.C */
/* *22   23-JUL-1991 12:21:09 SANTIAGO "(SPR -1) fixed compilation problem on VMS" */
/* *21   16-JUL-1991 14:55:04 SANTIAGO "(SPR -1) fixed compilation noise under VMS" */
/* *20   10-JUL-1991 15:49:25 SANTIAGO "(SPR -1) added additional command line options for fonts & resources dumping" */
/* *19    8-JUL-1991 18:28:19 SANTIAGO "(SPR -1) added -dumpOnXerror command line option" */
/* *18    6-JUL-1991 14:12:12 SANTIAGO "(SPR -1) moved wiinit from xf_initialize to fi_init" */
/* *17    5-JUL-1991 19:52:02 SANTIAGO "(SPR -1) x_initialize is now called xf_initialize" */
/* *16    5-JUL-1991 16:52:58 SANTIAGO "(SPR 8554) fixed BadMatch error on color workstations" */
/* *15   28-JUN-1991 10:55:54 SANTIAGO "(SPR -1) fixed core dump on DECwindows version, wiv.current_Display" */
/* *14   27-JUN-1991 15:20:21 SANTIAGO "(SPR 8438) removed call to wiz_color_init, no longer needed" */
/* *13   27-JUN-1991 15:16:47 SANTIAGO "(SPR -1) client icon internal bitmap is fixed for Motif" */
/* *12   18-JUN-1991 13:03:55 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1 */
/*version" */
/* *11   22-MAY-1991 11:35:03 SANTIAGO "(SPR 7598) argc is now passed by reference, minor cosmetic changes" */
/* *10    3-MAY-1991 16:15:32 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *9    23-OCT-1990 14:01:30 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *8    17-AUG-1990 22:32:30 VINCE "(SPR 5644) Code Cleanup" */
/* *7    20-JUL-1990 12:03:11 KEE "(SPR -1) Put away Windows menu for temporary" */
/* *6    27-JUN-1990 17:34:10 KEE "(SPR -1) New wi data structure using 2-D array" */
/* *5    12-APR-1990 21:11:07 JULIAN "(SPR -1) changed ansiDwt to ansidwt" */
/* *4     2-MAR-1990 11:24:36 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X" */
/* *3     6-FEB-1990 00:11:00 JULIAN "(SPR -1) X Version (Kee)" */
/* *2    11-DEC-1989 14:22:43 JULIAN "(SPR 1) initial checkin of X initialize routine" */
/* *1    11-DEC-1989 14:19:10 JULIAN "X Windows initialization for XFinder" */
/* DEC/CMS REPLACEMENT HISTORY, Element XF_INITIALIZE.C */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAinsIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE   */
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

#ifdef USE_X

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_X_H
#include "esi_x.h"
#endif

/* #include "ansixlib.h" - is no longer needed, X11R4 provides for ANSI C Prototypes and C++ */
/* #include "ansixt.h" - is no longer needed, X11R4 provides for ANSI C Prototypes and C++ */
/* #include "ansixm.h" - is no longer needed, Motif 1.1 provides for ANSI C Prototypes and C++ */

#include "ansixt.h"
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef MOTIF
#define XtPointer caddr_t
#endif

/* ------------------------------------------------------------------------- */
/* Application Window Manager Icon.                                          */
/* ------------------------------------------------------------------------- */

#define finder_width 50
#define finder_height 50
static char finder_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
   0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xc0, 0xff,
   0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00,
   0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff,
   0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x03, 0x00,
   0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff,
   0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00,
   0x00, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00};

/* ------------------------------------------------------------------------- */
/* Application Command Line Options Table,                                   */
/* ------------------------------------------------------------------------- */

static XrmOptionDescRec appCommandLineOptionsTable[] =
{
/*  Command Line           Resource              Argument             Argument */
/*    Option               Specifier              Style                Value   */

  { "-smallPTextFont",   ".smallPTextFont",   XrmoptionSepArg,    (XtPointer) NULL    },
  { "-sptf",             ".smallPTextFont",   XrmoptionSepArg,    (XtPointer) NULL    },
  { "-normalPTextFont",  ".normalPTextFont",  XrmoptionSepArg,    (XtPointer) NULL    },
  { "-nptf",             ".normalPTextFont",  XrmoptionSepArg,    (XtPointer) NULL    },
  { "-smallPFont",       ".smallPFont",       XrmoptionSepArg,    (XtPointer) NULL    },
  { "-spf",              ".smallPFont",       XrmoptionSepArg,    (XtPointer) NULL    },
  { "-normalPFont",      ".normalPFont",      XrmoptionSepArg,    (XtPointer) NULL    },
  { "-npf",              ".normalPFont",      XrmoptionSepArg,    (XtPointer) NULL    },
  { "-smallCTextFont",   ".smallCTextFont",   XrmoptionSepArg,    (XtPointer) NULL    },
  { "-sctf",             ".smallCTextFont",   XrmoptionSepArg,    (XtPointer) NULL    },
  { "-normalCTextFont",  ".normalCTextFont",  XrmoptionSepArg,    (XtPointer) NULL    },
  { "-nctf",             ".normalCTextFont",  XrmoptionSepArg,    (XtPointer) NULL    },
  { "-smallCFont",       ".smallCFont",       XrmoptionSepArg,    (XtPointer) NULL    },
  { "-scf",              ".smallCFont",       XrmoptionSepArg,    (XtPointer) NULL    },
  { "-normalCFont",      ".normalCFont",      XrmoptionSepArg,    (XtPointer) NULL    },
  { "-ncf",              ".normalCFont",      XrmoptionSepArg,    (XtPointer) NULL    },
  { "-dumpOnXerror",     ".dumpOnXerror",     XrmoptionNoArg,     (XtPointer) "TRUE"  },
  { "-resources",        "*resources",        XrmoptionNoArg,     (XtPointer) "TRUE"  },
  { "-verbose",          "*verbose",          XrmoptionNoArg,     (XtPointer) "TRUE"  },
} ;

/* ------------------------------------------------------------------------- */
/* Application Fallback Resources                                            */
/* ------------------------------------------------------------------------- */

static String appFallbackResources[] =
{
  ".smallPTextFont:  -*-Helvetica-Bold-R-Normal--10-100-75-75-P-60-*-*",
  ".normalPTextFont: -*-Helvetica-Bold-R-Normal--12-120-75-75-P-70-*-*", 
  ".smallPFont:      -*-Helvetica-Medium-R-Normal--10-100-75-75-P-56-*-*",
  ".normalPFont:     -*-Helvetica-Medium-R-Normal--12-120-75-75-P-67-*-*",
  ".smallCTextFont:  -*-Courier-Bold-R-Normal--12-120-75-75-M-70-*-*",
  ".normalCTextFont: -*-Courier-Bold-R-Normal--14-140-75-75-M-90-*-*",
  ".smallCFont:      -*-Courier-Medium-R-Normal--12-120-75-75-M-70-*-*",
  ".normalCFont:     -*-Courier-Medium-R-Normal--14-140-75-75-M-90-*-*",
  ".dumpOnXerror:    false",
  ".resources:       false",
  ".verbose:         false",
  NULL
} ;

/* ------------------------------------------------------------------------- */
/* Application Resource Data                                                 */
/* ------------------------------------------------------------------------- */

typedef struct {

    Pixel   bg ;        /* default background color */
    Pixel   fg ;        /* default foreground color */

    Dimension width ;
    Dimension height ;
#ifdef MOTIF
    XmFontList smallPTextFont ;
    XmFontList normalPTextFont ;
    XmFontList smallPFont ;
    XmFontList normalPFont ;
    XmFontList smallCTextFont ;
    XmFontList normalCTextFont ;
    XmFontList smallCFont ;
    XmFontList normalCFont ;
#else
    DwtFontList smallPTextFont ;
    DwtFontList normalPTextFont ;
    DwtFontList smallPFont ;
    DwtFontList normalPFont ;
    DwtFontList smallCTextFont ;
    DwtFontList normalCTextFont ;
    DwtFontList smallCFont ;
    DwtFontList normalCFont ;
#endif
    Boolean dumpOnXerror ; /* flag to set core dump on X error */
    Boolean resources ;    /* flag to set dump resources mode */
    Boolean verbose ;      /* flag to set verbose mode */

} AppData, *AppDataPtr ;

/* ------------------------------------------------------------------------- */
/* Application Resource List                                                 */
/* ------------------------------------------------------------------------- */

void computeDefaultWidth() ;
void computeDefaultHeight() ;

#ifdef MOTIF
#define XmNsmallPTextFont  "smallPTextFont"
#define XmNnormalPTextFont "normalPTextFont"
#define XmNsmallPFont      "smallPFont"
#define XmNnormalPFont     "normalPFont"
#define XmNsmallCTextFont  "smallCTextFont"
#define XmNnormalCTextFont "normalCTextFont"
#define XmNsmallCFont      "smallCFont"
#define XmNnormalCFont     "normalCFont"
#define XmNdumpOnXerror    "dumpOnXerror"
#define XmCDumpOnXerror    "DumpOnXerror"
#define XmNresources       "resources"
#define XmCResources       "Resources"
#define XmNverbose         "verbose"
#define XmCVerbose         "Verbose"
#else
#define DwtNsmallPTextFont  "smallPTextFont"
#define DwtNnormalPTextFont "normalPTextFont"
#define DwtNsmallPFont      "smallPFont"
#define DwtNnormalPFont     "normalPFont"
#define DwtNsmallCTextFont  "smallCTextFont"
#define DwtNnormalCTextFont "normalCTextFont"
#define DwtNsmallCFont      "smallCFont"
#define DwtNnormalCFont     "normalCFont"
#define DwtNdumpOnXerror    "dumpOnXerror"
#define DwtCDumpOnXerror    "DumpOnXerror"
#define DwtNresources       "resources"
#define DwtCResources       "Resources"
#define DwtNverbose         "verbose"
#define DwtCVerbose         "Verbose"
#endif

static XtResource appResources[] =
{
  {
#ifdef MOTIF
    XmNbackground,
    XmCBackground,
    XmRPixel,
#else
    DwtNbackground,
    XtCBackground,
    XtRPixel,
#endif
    sizeof( Pixel ),
    XtOffset( AppDataPtr, bg ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    XtDefaultBackground
  },
  {
#ifdef MOTIF
    XmNforeground,
    XmCForeground,
    XmRPixel,
#else
    DwtNforeground,
    DwtCForeground,
    XtRPixel,
#endif
    sizeof( Pixel ),
    XtOffset( AppDataPtr, fg ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    XtDefaultForeground
  },
  {
#ifdef MOTIF
    XmNwidth,
    XmCWidth,
    XmRDimension,
#else
    DwtNwidth,
    XtCWidth,
    XtRDimension,
#endif
    sizeof( Dimension ),
    XtOffset( AppDataPtr, width ),
#ifdef MOTIF
    XmRCallProc,
#else
    XtRCallProc,
#endif
    (XtPointer) computeDefaultWidth
  },
  {
#ifdef MOTIF
    XmNheight,
    XmCHeight,
    XmRDimension,
#else
    DwtNheight,
    XtCHeight,
    XtRDimension,
#endif
    sizeof( Dimension ),
    XtOffset( AppDataPtr, height ),
#ifdef MOTIF
    XmRCallProc,
#else
    XtRCallProc,
#endif
    (XtPointer) computeDefaultHeight
  },
  {
#ifdef MOTIF
    XmNsmallPTextFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNsmallPTextFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, smallPTextFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Helvetica-Bold-R-Normal--10-100-75-75-P-60-*-*"
  },
  {
#ifdef MOTIF
    XmNnormalPTextFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNnormalPTextFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, normalPTextFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Helvetica-Bold-R-Normal--12-120-75-75-P-70-*-*"
  },
  {
#ifdef MOTIF
    XmNsmallPFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNsmallPFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, smallPFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Helvetica-Medium-R-Normal--10-100-75-75-P-56-*-*"
  },
  {
#ifdef MOTIF
    XmNnormalPFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNnormalPFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, normalPFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Helvetica-Medium-R-Normal--12-120-75-75-P-67-*-*"
  },
  {
#ifdef MOTIF
    XmNsmallCTextFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNsmallCTextFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, smallCTextFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Courier-Bold-R-Normal--12-120-75-75-M-70-*-*"
  },
  {
#ifdef MOTIF
    XmNnormalCTextFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNnormalCTextFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, normalCTextFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Courier-Bold-R-Normal--14-140-75-75-M-90-*-*"
  },
  {
#ifdef MOTIF
    XmNsmallCFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNsmallCFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, smallCFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Courier-Medium-R-Normal--12-120-75-75-M-70-*-*"
  },
  {
#ifdef MOTIF
    XmNnormalCFont,
    XmCFontList,
    XmRFontList,
    sizeof(XmFontList),
#else
    DwtNnormalCFont,
    DwtCFontList,
    DwtRFontList,
    sizeof(DwtFontList),
#endif
    XtOffset( AppDataPtr, normalCFont ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "-*-Courier-Medium-R-Normal--14-140-75-75-M-90-*-*"
  },
  {
#ifdef MOTIF
    XmNdumpOnXerror,
    XmCDumpOnXerror,
    XmRBoolean,
#else
    DwtNdumpOnXerror,
    DwtCDumpOnXerror,
    XtRBoolean,
#endif
    sizeof( Boolean ),
    XtOffset( AppDataPtr, dumpOnXerror ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "FALSE",
  },
  {
#ifdef MOTIF
    XmNresources,
    XmCResources,
    XmRBoolean,
#else
    DwtNresources,
    DwtCResources,
    XtRBoolean,
#endif
    sizeof( Boolean ),
    XtOffset( AppDataPtr, resources ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "FALSE",
  },
  {
#ifdef MOTIF
    XmNverbose,
    XmCVerbose,
    XmRBoolean,
#else
    DwtNverbose,
    DwtCVerbose,
    XtRBoolean,
#endif
    sizeof( Boolean ),
    XtOffset( AppDataPtr, verbose ),
#ifdef MOTIF
    XmRString,
#else
    XtRString,
#endif
    (XtPointer) "FALSE",
  },
} ;

/* ------------------------------------------------------------------------- */
/* Application Data Object                                                   */
/* ------------------------------------------------------------------------- */

AppData appData ;

/* ------------------------------------------------------------------------- */
/* Application Object                                                        */
/* ------------------------------------------------------------------------- */

typedef struct {

    XtAppContext defaultContext ;
    Display      *defaultDisplay ;
    int          defaultScreen ;
    Colormap     defaultColormap ;
    Visual       *defaultVisual ;
    int          displayDepth ;
    char         geometry[64] ;
    Pixmap       iconPixmap ;
    Window       iconWindow ;

    Widget       masterToplevel ;
    Widget       commandArea ;
    Widget       messageArea ;

} AppObject, *AppObjectPtr ;

AppObject Xfinder ;

/* ------------------------------------------------------------------------- */
/* XtResourceDefaultProc(s)                                                  */
/* ------------------------------------------------------------------------- */

void computeDefaultWidth( widget, offset, value )
Widget widget ;
int offset ;
XrmValue *value ;
{
    /* actually want to take the users input and adapt it if not correct */
    /* so how do get width, -xrm "*width: 100" or XStep.width ?          */

    static Dimension widthPtr = 100 ; /* Initially this, later % of width */

    widthPtr =
        ( WidthOfScreen(
            DefaultScreenOfDisplay(
                wiv.current_Display ) ) * 75 ) / 100 ;
    value->addr = (XtPointer) &widthPtr ;
    }

void computeDefaultHeight( widget, offset, value )
Widget widget ;
int offset ;
XrmValue *value ;
{
    /* actually want to take the users input and adapt it if not correct */
    /* so how do get height, -xrm "*height: 100" or XStep.height ?       */

    static Dimension heightPtr = 100 ; /* Initially this, later % of height */

    heightPtr =
        ( HeightOfScreen(
            DefaultScreenOfDisplay(
                wiv.current_Display ) ) * 10 ) / 275 ;
    value->addr = (XtPointer) &heightPtr ;
    }

/* ------------------------------------------------------------------------- */

static void abortOnXProtocolError( dpy, event )
Display *dpy ;
XErrorEvent *event ;
{
    char buffer[BUFSIZ] ;
    char mesg[BUFSIZ] ;
    char number[32] ;
    char *mtype = "XlibMessage" ;
    register _XExtension *ext = (_XExtension *)NULL ;
    char *couldNotMsg = "cound not find any error database text" ;

    (void) fprintf( stderr, "\n\nFinder: X Protocol Error, dumping core for debug.\n" ) ;
    XGetErrorText( dpy, event->error_code, buffer, BUFSIZ ) ;
    XGetErrorDatabaseText( dpy, mtype, "XError", "X Error", mesg, BUFSIZ ) ;
    (void) fprintf( stderr, "%s:  %s\n  ", mesg, buffer ) ;
    XGetErrorDatabaseText( dpy, mtype, "MajorCode", "Request Major code %d", mesg, BUFSIZ ) ;
    (void) fprintf( stderr, mesg, event->request_code ) ;
    if (event->request_code < 128)
      {
        sprintf( number, "%d", event->request_code ) ;
        XGetErrorDatabaseText( dpy, "XRequest", number, couldNotMsg, buffer, BUFSIZ ) ;
      }
    else
      {
#ifndef vms
        for (ext = dpy->ext_procs ;
             ext && (ext->codes.major_opcode != event->request_code) ;
             ext = ext->next)
           ;
        if (ext)
          {
            strcpy( buffer, ext->name ) ;
          }
        else
          {
            buffer[0] = '\0' ;
          }
#endif
      }
    (void) fprintf( stderr, " (%s)\n  ", buffer ) ;
    XGetErrorDatabaseText( dpy, mtype, "MinorCode", "Request Minor code %d", mesg, BUFSIZ ) ;
    (void) fprintf( stderr, mesg, event->minor_code ) ;
#ifndef vms
    if (ext)
      {
        sprintf( mesg, "%s.%d", ext->name, event->minor_code ) ;
        XGetErrorDatabaseText( dpy, "XRequest", mesg, couldNotMsg, buffer, BUFSIZ ) ;
        (void) fprintf( stderr, " (%s)", buffer ) ;
      }
#endif
    fputs("\n  ", stderr) ;
    XGetErrorDatabaseText( dpy, mtype, "ResourceID", "ResourceID 0x%x", mesg, BUFSIZ ) ;
    (void) fprintf( stderr, mesg, event->resourceid ) ;
    fputs("\n  ", stderr) ;
    XGetErrorDatabaseText( dpy, mtype, "ErrorSerial", "Error Serial #%d", mesg, BUFSIZ ) ;
    (void) fprintf( stderr, mesg, event->serial ) ;
    fputs("\n  ", stderr) ;
    XGetErrorDatabaseText( dpy, mtype, "CurrentSerial", "Current Serial #%d", mesg, BUFSIZ ) ;
    (void) fprintf( stderr, mesg, dpy->request ) ;
    fputs("\n", stderr) ;
    if (event->error_code == BadImplementation)
      {
        fprintf( stderr, "BadImplementation\n") ;
      }
    fflush( stderr ) ;
    abort() ;
}

/* ------------------------------------------------------------------------- */

static void dumpResources( client_name )
String client_name ;
{
    fprintf( stderr, 
"\n%s: understands all standard X Toolkit command-line options\n\
       plus the following;\n\n\
     account/password (-project projectname | -proj projectname )\n\
     [-workstation workstationname] [-w workstationname] [-test] [-t]\n\
     [-menu filename] [-m filename] [-x] [-checkwi] [-journal filename] [-j filename]\n\
     [-record filename] [-r filename] [-play filename] [-p filename]\n\
     [-plot plottername] [-no_onunits] [-mac] [-maconly] [-ac] [-accounting]\n\
     [-no_wcplot] [-crt crtname] [-c crtname]\n\
     [-smallPTextFont fontname] [-sptf fontname]\n\
     [-normalPTextFont fontname] [-nptf fontname]\n\
     [-smallPFont fontname] [-spf fontname]\n\
     [-normalPFont fontname] [-npf fontname]\n\
     [-smallCTextFont fontname] [-sctf fontname]\n\
     [-normalCTextFont fontname] [-nctf fontname]\n\
     [-smallCFont fontname] [-scf fontname]\n\
     [-normalCFont fontname] [-ncf fontname]\n\
     [-dumpOnXerror] [-resources] [-verbose]\n\n", client_name ) ;
}

/* ------------------------------------------------------------------------- */
#ifdef MOTIF /* ---------- BEGIN MOTIF BLOCK ---------- */

#include <X11/Shell.h>
publicdef INT xf_initialize( argc, argv )
int *argc;
char *argv[];
{
    CHAR applicationGeometry[80];
    Pixmap iconPixmap = (Pixmap) NULL ;
    Arg arglist[MAX_ARG];
    INT n = 0 ;
    INT status = 0 ;
    XmString CompoundString = (XmString) NULL ;
    char msgText[132] ;

    wiv.toplevel =
        XtAppInitialize(
	    &Xfinder.defaultContext,
	    "XFinder",
	    appCommandLineOptionsTable,
	    XtNumber( appCommandLineOptionsTable ),
	    argc,
	    argv,
	    appFallbackResources,
	    (ArgList) NULL,
	    0 ) ;

    wiv.current_Display = XtDisplayOfObject( wiv.toplevel ) ;

    XtGetApplicationResources(
        wiv.toplevel,
	&appData,
        appResources,
        XtNumber( appResources ),
        (ArgList) NULL,
        0 ) ;

    if (appData.resources)
      {
        dumpResources( argv[0] ) ;
	exit( 0 ) ;
      }

    sprintf( applicationGeometry, "%dx%d+%d+%d",
             appData.width,
             appData.height,
             0,
             0 ) ;

    n = 0 ;
    XtSetArg( arglist[n], XmNallowShellResize, FALSE               ) ;    n++ ;
    XtSetArg( arglist[n], XmNgeometry,         applicationGeometry ) ;    n++ ;
    XtSetValues( wiv.toplevel, arglist, 2 ) ;

    if (appData.dumpOnXerror)
      {
	(void) fprintf( stderr, "Finder: Synchronizing the X Display Server\n") ;
	(void) XSynchronize( wiv.current_Display, True ) ;
	(void) XSetErrorHandler( abortOnXProtocolError ) ;
      }

    wiv.mainw =
        XmCreateMainWindow(
            wiv.toplevel,
            "Main_Window",
            arglist,
            0 ) ;
    XtManageChild( wiv.mainw ) ;
    
    n = 0 ;
    XtSetArg( arglist[n], XmNorientation, XmHORIZONTAL ) ; n++ ;
    XtSetArg( arglist[n], XmNspacing,     15           ) ; n++ ;
    XtSetArg( arglist[n], XmNresizeWidth, FALSE        ) ; n++ ;
    wiv.mainmenubar =
        XmCreateMenuBar(
            wiv.mainw,
            "Main_Menu_Bar",
            arglist,
            n ) ;
    XtManageChild( wiv.mainmenubar ) ;
    
    wiv.Fontlist[SMALL_P_TEXT] = appData.smallPTextFont ;
    wiv.Fontlist[NORMAL_P_TEXT] = appData.normalPTextFont ;
    wiv.Fontlist[SMALL_P_FONT] = appData.smallPFont ;
    wiv.Fontlist[NORMAL_P_FONT] = appData.normalPFont ;
    wiv.Fontlist[SMALL_C_TEXT] = appData.smallCTextFont ;
    wiv.Fontlist[NORMAL_C_TEXT] = appData.normalCTextFont ;
    wiv.Fontlist[SMALL_C_FONT] = appData.smallCFont ;
    wiv.Fontlist[NORMAL_C_FONT] = appData.normalCFont ;

    wiv.num_main_dialog_menu = 4;   /* Use dynmaic allocation so that we don't      */
                                    /*   have to change include file "esi_wi_int.h" */
                                    /*   all the times                              */
    /* 4 : 2 for Window menu,                                                       */
/*       2 for help menu,                                                           */
/*   this number can be changed if more main                                        */
/*   dialogs or menus are needed                                                    */

    wiv.main_dialog_menu =
	(Widget *) tc_zalloc( sizeof(Widget) * wiv.num_main_dialog_menu ) ;
    
    n = 0 ;
    XtSetArg( arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[0] =
	XmCreatePulldownMenu(
	    wiv.mainmenubar,
	    "Main_Dialog_Menu_0",
	    arglist,
	    n ) ;

    n = 0 ;
    CompoundString = XmStringCreateSimple("Windows") ;
    XtSetArg( arglist[n], XmNlabelString, CompoundString              ) ; n++ ;
    XtSetArg( arglist[n], XmNsubMenuId,   wiv.main_dialog_menu[0]     ) ; n++ ;
    XtSetArg( arglist[n], XmNfontList,    wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[1] = 
	XmCreateCascadeButton(
	    wiv.mainmenubar,
	    "Main_Dialog_Menu_1",
	    arglist,
	    n ) ;
    XmStringFree( CompoundString ) ;
    XtManageChild( wiv.main_dialog_menu[1] ) ;

    n = 0 ;
    XtSetArg( arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[2] =
	XmCreatePulldownMenu(
	    wiv.mainmenubar,
	    "Main_Dialog_Menu_2",
	    arglist,
	    n ) ;

    n = 0 ;
    CompoundString = XmStringCreateSimple("Help") ;
    XtSetArg( arglist[n], XmNlabelString, CompoundString              ) ; n++ ;
    XtSetArg( arglist[n], XmNsubMenuId,   wiv.main_dialog_menu[2]     ) ; n++ ;
    XtSetArg( arglist[n], XmNfontList,    wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[3] = 
	XmCreateCascadeButton(
	    wiv.mainmenubar,
	    "Main_Dialog_Menu_3",
	    arglist,
	    n ) ;
    XmStringFree( CompoundString ) ;
/*  XtManageChild( wiv.main_dialog_menu[3] ) ; */

    n = 0 ;
    XtSetArg( arglist[n], XmNmenuHelpWidget, wiv.main_dialog_menu[3] ) ; n++ ;
    XtSetValues( wiv.mainmenubar, arglist, n ) ;

    XmMainWindowSetAreas(
        wiv.mainw,
        wiv.mainmenubar,
        (Widget)NULL,
        (Widget)NULL,
        (Widget)NULL,
        (Widget)NULL ) ;
    
    XtRealizeWidget( wiv.toplevel ) ;

    /* Set the Window Manager Icon for Finder */

    iconPixmap =
        XCreateBitmapFromData(
            wiv.current_Display,
            DefaultRootWindow( wiv.current_Display ),
            finder_bits,
            finder_width,
            finder_height ) ;

    if ( iconPixmap != (Pixmap) NULL )
      {
        XtSetArg( arglist[0], XmNiconPixmap, iconPixmap ) ;
        XtSetValues( wiv.toplevel, arglist, 1 ) ;
      }

    return( SUCCESS ) ;
    }


#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */

#include <X11/Vendor.h>

#if USE_PROTOTYPES
static Window get_parent (Window win);
#else 
static Window get_parent ();
#endif

publicdef INT xf_initialize( argc, argv )
int *argc ;
char *argv[] ;
{
    Arg arglist[MAX_ARG];
    INT n = 0 ;
    INT status = 0 ;
    Pixmap pixmap = (Pixmap) NULL ;
    Window cur_focus = (Window) NULL ;
    DwtCompString CompoundString = (DwtCompString) NULL ;
    
    wiv.toplevel =
        XtInitialize(
            "Finder",
            "XFinder",
	    appCommandLineOptionsTable,
	    XtNumber( appCommandLineOptionsTable ),
            argc,
            argv ) ;
    XtSetArg( arglist[0], XtNallowShellResize, TRUE ) ;
    XtSetValues( wiv.toplevel, arglist, 1 ) ;
    
    n = 0 ;
    XtSetArg( arglist[n], DwtNx,           0        ) ; n++ ;
    XtSetArg( arglist[n], DwtNy,           0        ) ; n++ ;
    XtSetArg( arglist[n], DwtNwidth,       1024 - 5 ) ; n++ ;
    XtSetArg( arglist[n], DwtNacceptFocus, TRUE     ) ; n++ ;
    wiv.mainw =
        DwtMainWindowCreate(
            wiv.toplevel,
            "Main_Window",
            arglist,
            n ) ;
    XtManageChild( wiv.mainw ) ;
    
    n = 0 ;
    XtSetArg( arglist[n], DwtNorientation, DwtOrientationHorizontal ) ; n++ ;
    XtSetArg( arglist[n], DwtNspacing,     15                       ) ; n++ ;
    wiv.mainmenubar =
        DwtMenuBarCreate(
            wiv.mainw,
            "Main_Menu_Bar",
            arglist,
            n ) ;
    XtManageChild( wiv.mainmenubar ) ;
    
    wiv.current_Display = XtDisplay( wiv.toplevel ) ;

    wiv.Fontlist[SMALL_P_TEXT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Helvetica-Bold-R-Normal--10-100-75-75-P-60-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[NORMAL_P_TEXT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Helvetica-Bold-R-Normal--12-120-75-75-P-70-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[SMALL_P_FONT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Helvetica-Medium-R-Normal--10-100-75-75-P-56-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[NORMAL_P_FONT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Helvetica-Medium-R-Normal--12-120-75-75-P-67-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[SMALL_C_TEXT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Courier-Bold-R-Normal--12-120-75-75-M-70-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[NORMAL_C_TEXT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Courier-Bold-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[SMALL_C_FONT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Courier-Medium-R-Normal--12-120-75-75-M-70-ISO8859-1" ),
            1 ) ;

    wiv.Fontlist[NORMAL_C_FONT] =
        DwtCreateFontList(
            XLoadQueryFont(
                wiv.current_Display,
                "-Adobe-Courier-Medium-R-Normal--14-140-75-75-M-90-ISO8859-1" ),
            1 ) ;
    
    /*------------------------------------------------------------------------------*/
    /* Use dynmaic allocation so that we don't                                      */
    /* have to change include file "esi_wi_int.h"                                   */
    /* all the times                                                                */
    /* 4 : 2 for Window menu,                                                       */
    /* 2 for help menu                                                              */
    /* this number can be changed if more main                                      */
    /* dialogs or menus are needed                                                  */
    /*------------------------------------------------------------------------------*/
   wiv.num_main_dialog_menu = 4;

   wiv.main_dialog_menu = (Widget *) tc_zalloc (sizeof(Widget) *
                                                 wiv.num_main_dialog_menu);
    
    n = 0 ;
    XtSetArg( arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[0] =
        DwtMenuPulldownCreate(
            wiv.mainmenubar,
            "Main_Dialog_Menu_0",
            arglist,
            n ) ;

    n = 0 ;
    CompoundString = DwtLatin1String("Windows") ;
    XtSetArg( arglist[n], DwtNlabel,     CompoundString              ) ; n++ ;
    XtSetArg( arglist[n], DwtNsubMenuId, wiv.main_dialog_menu[0]     ) ; n++ ;
    XtSetArg( arglist[n], DwtNfont,      wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[1] =
        DwtPullDownMenuEntryCreate(
            wiv.mainmenubar,
            "Main_Dialog_Menu_1",
            arglist,
            n ) ;
    XtFree( CompoundString ) ;

    /*    XtManageChild(wiv.main_dialog_menu[1]);                                   */
    /*      comment out for july, 1990 release - unstable                           */
    /* enable "Windows" menu                                                        */
    
    n = 0 ;
    XtSetArg( arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT] ) ; n++;
    wiv.main_dialog_menu[2] =
        DwtMenuPulldownCreate(
            wiv.mainmenubar,
            "Main_Dialog_Menu_2",
            arglist,
            0 ) ;

    n = 0 ;
    CompoundString = DwtLatin1String("Help") ;
    XtSetArg( arglist[n], DwtNlabel,     CompoundString              ) ; n++ ;
    XtSetArg (arglist[n], DwtNsubMenuId, wiv.main_dialog_menu[2]     ) ; n++ ;
    XtSetArg (arglist[n], DwtNfont,      wiv.Fontlist[NORMAL_P_TEXT] ) ; n++ ;
    wiv.main_dialog_menu[3] =
        DwtPullDownMenuEntryCreate(
            wiv.mainmenubar,
            "Main_Dialog_Menu_3",
            arglist,
            n ) ;
    XtFree( CompoundString ) ;
    /* XtManageChild(wiv.main_dialog_menu[3]);                                      */
    /* enable "Help" menu                                                           */
    
    XtRealizeWidget( wiv.toplevel ) ;

    pixmap =
        XCreateBitmapFromData(
            wiv.current_Display,
            XtWindow( wiv.toplevel ),
            finder_bits,
            finder_width,
            finder_height ) ;
    XtSetArg( arglist[0], XtNiconifyPixmap, pixmap ) ;
    XtSetValues( wiv.toplevel, arglist, 1 ) ;
    
    XGetInputFocus(
        wiv.current_Display,
        &cur_focus,
        &wiv.Decterm_win_revert_to ) ;

    wiv.Decterm_win = get_parent( get_parent( cur_focus ) ) ;
    
    return SUCCESS ;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static Window get_parent(Window win);                                     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    win             -(Window)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xf_initialize.c>                                                  */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static Window get_parent (Window win)
#else
static Window get_parent (win)
Window win;
#endif
    {
    Window parent, *child, root;
    int nchildren;
    
    if (XQueryTree (wiv.current_Display, win, &root, &parent, &child, &nchildren))
        {
        return(parent);
        }
    else
        {
        fprintf (stderr, "Xf_initialize - XQueryTree failed.");
        exit (1);
        }
    }

/* END:                                                                             */
#endif  /* ---------- END DECW BLOCK ---------- */
#endif
