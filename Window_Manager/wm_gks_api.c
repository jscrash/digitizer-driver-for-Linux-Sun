/* DEC/CMS REPLACEMENT HISTORY, Element WM_GKS_API.C*/
/* *34   13-FEB-1992 14:18:39 MING "(SPR 0) comment out gqchxp on VMS size "*/
/* *33    2-JUL-1991 10:22:58 KEE "(SPR 7433)  Add wm_gschh with height less than or equal zero error testing, case 1708"*/
/* *32    3-MAY-1991 16:14:24 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *31    9-NOV-1990 10:24:11 MING "(SPR -1) get rid of fortran code on SUN and add MAX_PTS on wm_gpl as VMS does"*/
/* *30   30-OCT-1990 11:33:33 KEE "(SPR -1) Fix compile noise at sun"*/
/* *29   25-OCT-1990 18:02:38 MING "(SPR 6075) add more cases in wm_gqtxx to handle diff align in plotting"*/
/* *28   23-OCT-1990 13:58:30 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *27    9-AUG-1990 22:46:48 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *26   31-JUL-1990 16:42:09 GILLESPIE "(SPR 1) Add reference to esi_gks.h"*/
/* *25   27-JUL-1990 16:42:31 MING "(SPR 0) assign the correct error status to wm_gqtxx for the former changes"*/
/* *24   26-JUL-1990 14:57:30 MING "(SPR 0) a hack to solve wm_gqtxx for metafile, should be revised later"*/
/* *23   25-JUL-1990 15:28:29 KEE "(SPR -1) Fix compilation noise if (i2 == GKS$_SUCCESS)"*/
/* *22   10-JUL-1990 17:41:54 MING "(SPR -1) delete checking of previous setting of SUN GKS call which cause a problem "*/
/* *21   21-JUN-1990 16:22:42 MING "(SPR 0) add wm_gqsga"*/
/* *20   13-JUN-1990 22:42:52 JULIAN "(SPR -1) Added wm_gqchup & wm_gqchh for sun"*/
/* *19   12-JUN-1990 10:14:08 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit "*/
/* *18    1-JUN-1990 12:33:12 SCD "(SPR 1) Validity check workstation parameter in wm_guwk."*/
/* *17   17-MAY-1990 15:12:49 JULIAN "(SPR 0) remove er_log on bad font"*/
/* *16   17-MAY-1990 09:54:23 JULIAN "(SPR 0) add return status on wm_inq_deferral"*/
/* *15    3-MAY-1990 21:45:35 KEE "(SPR -1) multi window support."*/
/* *14    3-MAY-1990 21:37:53 KEE "(SPR -1) multi window support."*/
/* *13    4-MAR-1990 22:14:34 JULIAN "(SPR 6010)  Checkin ULTRIX (GKS3D) bindings."*/
/* *12    3-MAR-1990 17:13:43 GILLESPIE "(SPR 1) Change iinitialization of last... to NOT use HUGE_VAL"*/
/* *11    2-MAR-1990 11:08:00 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"*/
/* *10   28-FEB-1990 17:22:57 VINCE "(SPR -1) added sticky interface for some set routines."*/
/* *9    26-FEB-1990 19:01:27 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *8     6-FEB-1990 00:12:05 JULIAN "(SPR -1) X Version (Kee)"*/
/* *7    31-JAN-1990 14:26:39 GORDON "(SPR 0) Put back in wm_gpl VMS fix & tc_free's in Sun code."*/
/* *6    18-DEC-1989 11:53:50 CONROY "(SPR 5078) Mods for plotting GKS polymarkers"*/
/* *5    18-DEC-1989 09:57:53 GILLESPIE "(SPR 5077) Added MAX_PTS for VMS GPL implementation; freed up memory for Sun"*/
/* *4     4-AUG-1989 08:54:38 GORDON "(SPR 102) Smarten up wm_gsrgwk with wm_update type logic for gpx"*/
/* *3    18-JUL-1989 11:21:27 GORDON "(SPR 100) Merge in UNIX changes"*/
/* *2     1-JUL-1989 15:51:44 JULIAN "(SPR 5) added gks dollar bindings.."*/
/* *1    19-JUN-1989 13:40:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WM_GKS_API.C*/
/* * FILE DESCRIPTION *                                                             */
/* --------------------                                                             */
/* ************************************************************************         */
/*                                                                                  */
/*   NAME: WM_GKS_ROUTINES.C                                                        */
/*                                                                                  */
/*   AUTHOR: JOHN CONROY                                                            */
/*                                                                                  */
/*   DESCRIPTION: ESI "GKS" interface routines.                                     */
/*                                                                                  */
/*   ************************************************************************       */

/* * INCLUDE FILES *                                                                */
/* -----------------                                                                */

#include "esi_wm.h"
#include "esi_er.h"
#include "esi_qs.h"

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif
publicdef INT wm_gqchs ();
publicdef INT wm_ginch ();
publicdef INT wm_gschm ();
publicdef INT wm_gsmch ();
publicdef INT wm_grqch ();
publicdef INT wm_ginlc ();
publicdef INT wm_gqlcs ();
publicdef INT wm_gslcm ();
publicdef INT wm_gsmlc ();
publicdef INT wm_gsasf ();
publicdef INT wm_gacwk ();
publicdef INT wm_gasgwk ();
publicdef INT wm_gclks ();
publicdef INT wm_gclrwk ();
publicdef INT wm_gclsg ();
publicdef INT wm_gclwk ();
publicdef INT wm_gcrsg ();
publicdef INT wm_gdawk ();
publicdef INT wm_gdsg ();
publicdef INT wm_gevtm ();
publicdef INT wm_gfa ();
publicdef INT wm_ginsg ();
publicdef INT wm_gkzqbtn ();
publicdef INT wm_gopks ();
publicdef INT wm_gopwk ();
publicdef INT wm_gpl ();
publicdef INT wm_gpm ();
publicdef INT wm_gqcntn ();
publicdef INT wm_gqchh ();
publicdef INT wm_gqchup ();
publicdef INT wm_gqdsp ();
publicdef INT wm_gqfaci ();
publicdef INT wm_gqfais ();
publicdef INT wm_gqfasi ();
publicdef INT wm_gqln ();
publicdef INT wm_gqlwsc ();
publicdef INT wm_gqnt ();
publicdef INT wm_gqplci ();
publicdef INT wm_gqtxx ();
publicdef INT wm_gqwkc ();
publicdef INT wm_gqwkcl ();
publicdef INT wm_gqwkdu ();
publicdef INT wm_gqwkt ();
publicdef INT wm_grensg ();
publicdef INT wm_grqlc ();
publicdef INT wm_gqwkm ();
publicdef INT wm_grqpk ();
publicdef INT wm_grsgwk ();
publicdef INT wm_gschh ();
publicdef INT wm_gschsp ();
publicdef INT wm_gschup ();
publicdef INT wm_gschxp ();
publicdef INT wm_gsclip ();
publicdef INT wm_gscr ();
publicdef INT wm_gsds ();
publicdef INT wm_gsdtec ();
publicdef INT wm_gselnt ();
publicdef INT wm_gsfaci ();
publicdef INT wm_gsfais ();
publicdef INT wm_gsfasi ();
publicdef INT wm_gshlit ();
publicdef INT wm_gsln ();
publicdef INT wm_gslwsc ();
publicdef INT wm_gsmk ();
publicdef INT wm_gsmksc ();
publicdef INT wm_gspkid ();
publicdef INT wm_gsplci ();
publicdef INT wm_gspmci ();
publicdef INT wm_gssgp ();
publicdef INT wm_gssgt ();
publicdef INT wm_gstxal ();
publicdef INT wm_gstxci ();
publicdef INT wm_gstxfp ();
publicdef INT wm_gstxp ();
publicdef INT wm_gsvis ();
publicdef INT wm_gsvp ();
publicdef INT wm_gsvpip ();
publicdef INT wm_gswkvp ();
publicdef INT wm_gswkwn ();
publicdef INT wm_gswn ();
publicdef INT wm_gtx ();
publicdef INT wm_guwk ();
publicdef INT wm_gqsga ();
publicdef INT wm_gacwk ();
publicdef INT wm_gasgwk ();
publicdef INT wm_gclks ();
publicdef INT wm_gclrwk ();
publicdef INT wm_gclsg ();
publicdef INT wm_gclwk ();
publicdef INT wm_gcrsg ();
publicdef INT wm_gdawk ();
publicdef INT wm_gdsg ();
publicdef INT wm_gevtm ();
publicdef INT wm_gfa ();
publicdef INT wm_ginsg ();
publicdef INT wm_gkzqbtn ();
publicdef INT wm_gopks ();
publicdef INT wm_gopwk ();
publicdef INT wm_gpl ();
publicdef INT wm_gpm ();
publicdef INT wm_gqcntn ();
publicdef INT wm_gqdsp ();
publicdef INT wm_gqfaci ();
publicdef INT wm_gqfais ();
publicdef INT wm_gqfasi ();
publicdef INT wm_gqln ();
publicdef INT wm_gqlwsc ();
publicdef INT wm_gqnt ();
publicdef INT wm_gqplci ();
publicdef INT wm_gqtxx ();
publicdef INT wm_gqwkc ();
publicdef INT wm_gqwkcl ();
publicdef INT wm_gqwkdu ();
publicdef INT wm_gqwkt ();
publicdef INT wm_grensg ();
publicdef INT wm_grqlc ();
publicdef INT wm_gqwkm ();
publicdef INT wm_grqpk ();
publicdef INT wm_grsgwk ();
publicdef INT wm_gschh ();
publicdef INT wm_gschsp ();
publicdef INT wm_gschup ();
publicdef INT wm_gschxp ();
publicdef INT wm_gsclip ();
publicdef INT wm_gsds ();
publicdef INT wm_gsdtec ();
publicdef INT wm_gselnt ();
publicdef INT wm_gsfaci ();
publicdef INT wm_gsfais ();
publicdef INT wm_gsfasi ();
publicdef INT wm_gshlit ();
publicdef INT wm_gsln ();
publicdef INT wm_gslwsc ();
publicdef INT wm_gsmk ();
publicdef INT wm_gsmksc ();
publicdef INT wm_gspkid ();
publicdef INT wm_gsplci ();
publicdef INT wm_gspmci ();
publicdef INT wm_gssgp ();
publicdef INT wm_gssgt ();
publicdef INT wm_gstxal ();
publicdef INT wm_gstxci ();
publicdef INT wm_gstxfp ();
publicdef INT wm_gstxp ();
publicdef INT wm_gsvis ();
publicdef INT wm_gsvp ();
publicdef INT wm_gsvpip ();
publicdef INT wm_gswkvp ();
publicdef INT wm_gswkwn ();
publicdef INT wm_gswn ();
publicdef INT wm_gtx ();
publicdef INT wm_guwk ();
publicdef INT wm_gkzqbtn ();
publicdef INT wm_gacwk ();
publicdef INT wm_gasgwk ();
publicdef INT wm_gclks ();
publicdef INT wm_gclrwk ();
publicdef INT wm_gclsg ();
publicdef INT wm_gclwk ();
publicdef INT wm_gcrsg ();
publicdef INT wm_gdawk ();
publicdef INT wm_gdsg ();
publicdef INT wm_gevtm ();
publicdef INT wm_gfa ();
publicdef INT wm_ginsg ();
publicdef INT wm_gopks ();
publicdef INT wm_gopwk ();
publicdef INT wm_gpl ();
publicdef INT wm_gpm ();
publicdef INT wm_gqcntn ();
publicdef INT wm_gqchh ();
publicdef INT wm_gqchup ();
publicdef INT wm_gqdsp ();
publicdef INT wm_gqfaci ();
publicdef INT wm_gqfais ();
publicdef INT wm_gqfasi ();
publicdef INT wm_gqln ();
publicdef INT wm_gqlwsc ();
publicdef INT wm_gqnt ();
publicdef INT wm_gqplci ();
publicdef INT wm_gqtxx ();
publicdef INT wm_gqwkc ();
publicdef INT wm_gqwkcl ();
publicdef INT wm_gqwkdu ();
publicdef INT wm_gqwkt ();
publicdef INT wm_grensg ();
publicdef INT wm_grqpk ();
publicdef INT wm_grsgwk ();
publicdef INT wm_gsasf ();
publicdef INT wm_gschh ();
publicdef INT wm_gschsp ();
publicdef INT wm_gschup ();
publicdef INT wm_gschxp ();
publicdef INT wm_gsclip ();
publicdef INT wm_gscr ();
publicdef INT wm_gsds ();
publicdef INT wm_gsdtec ();
publicdef INT wm_gsdyns ();
publicdef INT wm_gselnt ();
publicdef INT wm_gsfaci ();
publicdef INT wm_gsfais ();
publicdef INT wm_gsfasi ();
publicdef INT wm_gshlit ();
publicdef INT wm_gsln ();
publicdef INT wm_gslwsc ();
publicdef INT wm_gsmk ();
publicdef INT wm_gsmksc ();
publicdef INT wm_gspkid ();
publicdef INT wm_gsplci ();
publicdef INT wm_gspmci ();
publicdef INT wm_gssgp ();
publicdef INT wm_gssgt ();
publicdef INT wm_gstxal ();
publicdef INT wm_gstxci ();
publicdef INT wm_gstxfp ();
publicdef INT wm_gstxp ();
publicdef INT wm_gsvis ();
publicdef INT wm_gsvp ();
publicdef INT wm_gsvpip ();
publicdef INT wm_gswkvp ();
publicdef INT wm_gswkwn ();
publicdef INT wm_gswn ();
publicdef INT wm_gtx ();
publicdef INT wm_guwk ();
publicdef INT wm_gqchs ();
publicdef INT wm_ginch ();
publicdef INT wm_gschm ();
publicdef INT wm_gsmch ();
publicdef INT wm_grqch ();
publicdef INT wm_gqlcs ();
publicdef INT wm_ginlc ();
publicdef INT wm_gslcm ();
publicdef INT wm_gsmlc ();
publicdef INT wm_gwait ();
publicdef INT wm_gflush ();
publicdef INT wm_ggtlc ();
publicdef INT wm_grqlc ();
publicdef INT wm_gqwkm ();
publicdef INT wm_cfg_ws ();
publicdef INT wm_gqsga ();
publicdef INT wm_ggtitm ();
publicdef INT wm_giitm ();
publicdef INT wm_grditm ();
publicdef INT wm_gwitm ();
publicdef INT wm_gqchw ();
publicdef INT wm_gqtxp ();
publicdef INT wm_gqtxal ();
publicdef INT wm_gqchxp ();
publicdef INT wm_gqchsp ();

typedef struct 
    {
    GKS_INT color;
    GKS_INT style;
    DOUBLE width;
    } LINEATT;

typedef struct 
    {
    GKS_INT path;
    GKS_INT color;
    GKS_INT font;
    GKS_INT prec;
    DOUBLE up_x, up_y;
    DOUBLE height;
    DOUBLE expansion;
    } TEXTATT;

typedef struct 
    {
    GKS_INT color;
    GKS_INT style;
    } FILLATT;

static struct 
    {
    LINEATT line;
    TEXTATT text;
    FILLATT fill;
    } last = 
    {
        {
        -1, -1, 0.0
        }, 
        {
        -1, -1, -1, -1, 0.0, 0.0, 0.0, 0.0
        }, 
        {
        -1, -1
        }
    };



#ifdef vms             /* Begin VMS code */
#ifndef PRIOR_GKS

#define MAX_PTS 4092

#define GKS_DOLLAR 1

#include "esi_ho_desc.h"

/* CHOICE DEVICE STUFF                                                              */
/* ************************                                                         */

publicdef INT wm_gqchs (wks_id, choice_dev, record_buflen, err_stat, input_mode,
                        echo_flag, initial_status, initial_choice, echo_type,
                        echo_area, record_size, datrec)
GKS_INT wks_id, choice_dev, record_buflen, *err_stat, *input_mode, *echo_flag,
*initial_status, *initial_choice, *echo_type, *record_size;
CHOICE_DATREC *datrec;
GKS_REAL echo_area[];
    {

#if GKS_DOLLAR == 1
    gks$inq_choice_state (&wks_id, &choice_dev, err_stat, input_mode, echo_flag,
                          initial_status, initial_choice, echo_type, echo_area, datrec,
                          &record_buflen, record_size);
#else
    gqchs (&wks_id, &choice_dev, &record_buflen, err_stat, input_mode, echo_flag,
           initial_status, initial_choice, echo_type, echo_area, record_size, datrec);
#endif
    return SUCCESS;
    
    }

/* ************************                                                         */
publicdef INT wm_ginch (wks_id, choice_dev, initial_status, initial_choice, echo_type,
                        echo_area, record_buflen, datrec)
GKS_INT wks_id;
GKS_INT choice_dev;
GKS_INT initial_status;
GKS_INT initial_choice;
GKS_INT echo_type;
GKS_REAL echo_area[];
GKS_INT record_buflen;
CHOICE_DATREC *datrec;
    {
#if GKS_DOLLAR == 1
    gks$init_choice (&wks_id, &choice_dev, &initial_status, &initial_choice,
                     &echo_type, echo_area, datrec, &record_buflen);

#else
    ginch (&wks_id, &choice_dev, &initial_status, &initial_choice, &echo_type,
           &echo_area[0], &echo_area[1], &echo_area[2], &echo_area[3], &record_buflen,
           datrec);
#endif
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschm (wks_id, choice_dev, mode, echoflag)
GKS_INT wks_id, choice_dev, mode, echoflag;
    {

#if GKS_DOLLAR == 1
    gks$set_choice_mode (&wks_id, &choice_dev, &mode, &echoflag);

#else
    gschm (&wks_id, &choice_dev, &mode, &echoflag);
    return SUCCESS;
#endif  
    }

/* ************************                                                         */

publicdef INT wm_gsmch (wks_id, choice_dev, status, choice)
GKS_INT wks_id, choice_dev, *status, *choice;
    {
#if GKS_DOLLAR == 1
    gks$sample_choice (&wks_id, &choice_dev, status, choice);

#else
    gsmch (&wks_id, &choice_dev, status, choice);
#endif    
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_grqch (wks_id, choice_dev, status, choice)
GKS_INT wks_id, choice_dev, *status, *choice;
    {
#if GKS_DOLLAR == 1
    gks$request_choice (&wks_id, &choice_dev, status, choice);

#else
    grqch (&wks_id, &choice_dev, status, choice);
#endif
    return SUCCESS;
    }
/* LOCATOR DEVICE STUFF                                                             */
/* ************************                                                         */

publicdef INT wm_ginlc (wks_id, locate_dev, world_x, world_y, xform, echo_type,
                        echo_area, record_buflen, locate_datrec)
GKS_INT wks_id, locate_dev;
DOUBLE world_x;
DOUBLE world_y;
GKS_INT xform, echo_type;
GKS_REAL echo_area[];
LOCATE_DATREC *locate_datrec;
GKS_INT record_buflen;
    {
    GKS_REAL wx = world_x;
    GKS_REAL wy = world_y;

#if GKS_DOLLAR == 1
    gks$init_locator (&wks_id, &locate_dev, &world_x, &world_y, &xform, &echo_type,
                      echo_area, locate_datrec, &record_buflen);

#else
    ginlc (&wks_id, &locate_dev, &wx, &wy, &xform, &echo_type, &echo_area[0],
           &echo_area[1], &echo_area[2], &echo_area[3], &record_buflen, locate_datrec);
#endif  
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqlcs (wks_id, locate_dev, type, record_buflen, status, mode,
                        echo_flag, xform, world_x, world_y, echo_type, echo_area,
                        record_size, datrec)
GKS_INT wks_id, locate_dev, type, record_buflen;
GKS_INT *status, *mode, *echo_flag, *xform;
GKS_REAL *world_x, *world_y, echo_area[];
GKS_INT *echo_type, *record_size;
LOCATE_DATREC *datrec;
    {

#if GKS_DOLLAR == 1
    gks$inq_locator_state (&wks_id, &locate_dev, &type, status, mode, echo_flag, xform,
                           world_x, world_y, echo_type, echo_area, datrec,
                           &record_buflen, record_size);

#else
    gqlcs (&wks_id, &locate_dev, &type, &record_buflen, status, mode, echo_flag, xform,
           world_x, world_y, echo_type, echo_area, record_size, datrec);
#endif  
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gslcm (wks_id, locate_dev, mode, echoflag)
GKS_INT wks_id, locate_dev, mode, echoflag;
    {
#if GKS_DOLLAR == 1
    gks$set_locator_mode (&wks_id, &locate_dev, &mode, &echoflag);

#else
    gslcm (&wks_id, &locate_dev, &mode, &echoflag);
#endif
    return SUCCESS;
    
    }

/* ************************                                                         */

publicdef INT wm_gsmlc (wks_id, locate_dev, xform, x, y)
GKS_INT wks_id, locate_dev, *xform;
GKS_REAL *x, *y;
    {
#if GKS_DOLLAR == 1
    gks$sample_locator (&wks_id, &locate_dev, xform, x, y);

#else
    gsmlc (&wks_id, &locate_dev, xform, x, y);
#endif
    return SUCCESS;
    
    }


/* ************************                                                         */

publicdef INT wm_gsasf (flags)
GKS_INT *flags;
    {
    gsasf (flags);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gacwk (i1)
GKS_INT i1;
    {
    gacwk (&i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gasgwk (i1, i2)
GKS_INT i1, i2;
    {
    gasgwk (&i1, &i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclks ()
    {
    gclks ();
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclrwk (i1, i2)
GKS_INT i1, i2;
    {
    gclrwk (&i1, &i2);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gclsg ()
    {
    gclsg ();
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclwk (i1)
GKS_INT i1;
    {
    gclwk (&i1);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gcrsg (i)
GKS_INT i;
    {
    gcrsg (&i);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gdawk (i1)
GKS_INT i1;
    {
    gdawk (&i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gdsg (segment)
GKS_INT segment;                    /* segment name to delete                       */
    {
    gdsg (&segment);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gevtm (r1, r2, r3, r4, r5, r6, r7, i1, ra1)
DOUBLE r1;
DOUBLE r2;
DOUBLE r3;
DOUBLE r4;
DOUBLE r5;
DOUBLE r6;
DOUBLE r7;
GKS_INT i1;
GKS_REAL *ra1;
    {
    GKS_REAL x1 = r1;
    GKS_REAL x2 = r2;
    GKS_REAL x3 = r3;
    GKS_REAL x4 = r4;
    GKS_REAL x5 = r5;
    GKS_REAL x6 = r6;
    GKS_REAL x7 = r7;
    
    gevtm (&x1, &x2, &x3, &x4, &x5, &x6, &x7, &i1, ra1);
    
    return SUCCESS;
    
    }

/* ************************                                                         */

publicdef INT wm_gfa (n, px, py)
GKS_INT n;
GKS_REAL *px, *py;
    {
    gfa (&n, px, py);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_ginsg (i1, r1)
GKS_INT i1;
GKS_REAL *r1;
    {
    ginsg (&i1, r1);
    }

/* ************************                                                         */

publicdef INT wm_gkzqbtn (buttonPtr)
INT *buttonPtr;
    {
    gkzqbtn (buttonPtr);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gopks (errfil, ibuf)
GKS_INT errfil;                     /* error file unit number                       */
GKS_INT ibuf;                       /* dummy parameter (not used)                   */
    {
    gopks (&errfil, &ibuf);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gopwk (i1, i2, i3)
GKS_INT i1, i2, i3;
    {
    gopwk (&i1, &i2, &i3);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gpl (n, px, py)
GKS_INT n;
GKS_REAL *px, *py;
    {
    GKS_INT i;
    GKS_INT npts = 0;
    
    /*  send out MAX_PTS points - GKS dependent                                     */
    /*  Be sure to overlap the last point of the                                    */
    /*  previous series and the first point of the                                  */
    /*  current series                                                              */
    
    for (i = 0; i < n - 1; npts--, i += npts, px += npts, py += npts)
        {
        npts = MIN (MAX_PTS, n - i);
        gpl (&npts, px, py);
        }
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gpm (n, px, py)
GKS_INT n;
GKS_REAL *px, *py;
    {
    gpm (&n, px, py);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gqcntn (i1, i2)
GKS_INT *i1;
GKS_INT *i2;
    {
    gqcntn (i1, i2);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gqchh (status, char_height)
GKS_INT *status;
GKS_REAL *char_height;
    {
    gqchh (status, char_height);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gqchup (status, up_x, up_y)
GKS_INT *status;
GKS_REAL *up_x;
GKS_REAL *up_y;
    {
    gqchup (status, up_x, up_y);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqdsp (wtype, errind, dcunit, rx, ry, lx, ly)
GKS_INT wtype;
GKS_INT *errind;
GKS_INT *dcunit;
GKS_REAL *rx, *ry;
GKS_INT *lx, *ly;
    {
    gqdsp (&wtype, errind, dcunit, rx, ry, lx, ly);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqfaci (statusPtr, intPtr)
GKS_INT *statusPtr;
COLOR_INDEX *intPtr;
    {
    gqfaci (statusPtr, intPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqfais (statusPtr, intPtr)
GKS_INT *statusPtr;
FILL_AREA_INTERIOR_STYLE *intPtr;
    {
    gqfais (statusPtr, intPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqfasi (statusPtr, intPtr)
GKS_INT *statusPtr;
FILL_AREA_STYLE_INDEX *intPtr;
    {
    gqfasi (statusPtr, intPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqln (statusPtr, linePtr)
GKS_INT *statusPtr;
LINE_STYLE_INDEX *linePtr;
    {
    gqln (statusPtr, linePtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqlwsc (statusPtr, scalePtr)
GKS_INT *statusPtr;
GKS_REAL *scalePtr;
    {
    gqlwsc (statusPtr, scalePtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqnt (tnr, statusPtr, windowPtr, viewPtr)
GKS_INT tnr;
GKS_INT *statusPtr;
GKS_REAL windowPtr[4];
GKS_REAL viewPtr[4];
    {
    gqnt (&tnr, statusPtr, windowPtr, viewPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqplci (statusPtr, colorPtr)
GKS_INT *statusPtr;
COLOR_INDEX *colorPtr;
    {
    gqplci (statusPtr, colorPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqtxx (wkid, px, py, str, errind, cpx, cpy, txexpx, txexpy)
GKS_INT wkid, *errind;
DOUBLE px, py;
GKS_REAL *cpx, *cpy, txexpx[4], txexpy[4];
CHAR *str;
    {
    GKS_REAL x = px, y = py;
    GKS_REAL chh, chxp, chsp, chw, chup_x, chup_y;
    GKS_INT txp, txal_h, txal_v;
    CHAR ws_type[40];
    int str_len;
    
    gqtxx (&wkid, &x, &y, hoz_str_descpt (str), errind, cpx, cpy, txexpx, txexpy);
    
    /* ------------------------------------------------------                       */
    /* This is a hack to solve the metafile problem                                 */
    /*   this call return error status 39 when the workstation is metafile          */
    /*   this break our plotting software.                                          */
    /*   to solve this just for the time being, we only consider                    */
    /*   text with text path right and text alignment normal (or left and base)     */
    /*   and calculate the x width for that text label                              */
    /*   otherwise return error status 39.                                          */
    /*   To really solve the problem, we should caluclate the text extent           */
    /*   considered different text attritude, text alignment, text path, up vector  */
    /*   .... and have a good default for the font precision.                       */
    /*                                                                              */
    /*   P.S. the character width from gqchw may not be the one that we want        */
    /*   it may be changed to some other default value                              */
    
    if (*errind == 39)
        {
        qs_inq_c (QS_WORKSTATION_TYPE, ws_type, (INT *)0);
        if (strcmp (ws_type, "METAFILE") != 0)
            {
            return *errind;
            }
        else
            {
            str_len = strlen (str);
            wm_gqchh (errind, &chh);
            chxp = 1.0;
#if 0
            /* ming: 02/12/92 */
            /* this function does not exist in the NOVA GKS which we use for */
            /* PL_BATCH_PLOT in VMS to produce neutral plot files            */
            /* It is OK now to set the expansion factor = 1.0 since in our   */
            /* codes do not change the expansion factor, but in the future   */
            /* if neutral plot file is not supported, this code should be    */
            /* reactivated                                                   */
            gqchxp (errind, &chxp);
#endif
            gqtxp (errind, &txp);
            gqchsp (errind, &chsp);
            wm_gqchup (errind, &chup_x, &chup_y);
            gqchw (errind, &chw);
            gqtxal (errind, &txal_h, &txal_v);
            if (chup_x == 0 && chup_y == 1 && txp == 0 && (txal_h == GAHNOR ||
                                                           txal_h == GALEFT) &&
                (txal_v == GAVNOR || txal_v == GABASE))
                {
                txexpx[0] = px;
                txexpx[1] = px + str_len * chw * chxp + (str_len * chsp * chh * chxp);
                txexpx[2] = txexpx[1];
                txexpx[3] = px;
                txexpy[0] = py;
                txexpy[1] = py;
                txexpy[2] = py + chh;
                txexpy[3] = txexpy[2];
                *cpx = txexpx[1];
                *cpy = py;
                }
	    else if (chup_x == 0 && chup_y == 1 && txp == 0 && 
		(txal_h == GACENT) && 
		(txal_v == GAVNOR || txal_v == GABASE))
	      {
		txexpx[0] = px - str_len/2*chw*chxp - (str_len/2*chsp*chh*chxp);
		txexpx[1] = px + str_len/2*chw*chxp + (str_len/2*chsp*chh*chxp);
		txexpx[2] = txexpx[1];
		txexpx[3] = px;
		txexpy[0] = py;
		txexpy[1] = py;
		txexpy[2] = py + chh;
		txexpy[3] = txexpy[2];	  
		*cpx = txexpx[1];
		*cpy = py;
	      }
	    else if (chup_x == 0 && chup_y == 1 && txp == 0 && 
		(txal_h == GACENT) && 
		(txal_v == GAHALF))
	      {
		txexpx[0] = px - str_len/2*chw*chxp - (str_len/2*chsp*chh*chxp);
		txexpx[1] = px + str_len/2*chw*chxp + (str_len/2*chsp*chh*chxp);
		txexpx[2] = txexpx[1];
		txexpx[3] = px;
		txexpy[0] = py - chh/2;
		txexpy[1] = py - chh/2;
		txexpy[2] = py + chh/2;
		txexpy[3] = txexpy[2];	  
		*cpx = txexpx[1];
		*cpy = py;
	      }
	    else if (chup_x == 0 && chup_y == 1 && txp == 0 && 
		(txal_h == GAHNOR || txal_h == GALEFT) && 
		(txal_v == GAHALF))
	      {
		txexpx[0] = px;
		txexpx[1] = px + str_len*chw*chxp + (str_len*chsp*chh*chxp);
		txexpx[2] = txexpx[1];
		txexpx[3] = px;
		txexpy[0] = py - chh/2;
		txexpy[1] = py - chh/2;
		txexpy[2] = py + chh/2;
		txexpy[3] = txexpy[2];	  
		*cpx = txexpx[1];
		*cpy = py;
	      }
            else
                {
                return(*errind = 39);
                }
            }
        }
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkc (i1, i2, i3, i4)
GKS_INT i1, *i2, *i3, *i4;
    {
    gqwkc (&i1, i2, i3, i4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkcl (wtype, statusPtr, wclassPtr)
GKS_INT wtype;
GKS_INT *statusPtr;
GKS_INT *wclassPtr;
    {
    gqwkcl (&wtype, statusPtr, wclassPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqwkdu (i1, i2, i3, i4, i5, i6)
GKS_INT i1, *i2, *i3, *i4, *i5, *i6;
    {
    if (ug_if_gpx ())
        {
        gqwkdu (&i1, i2, i3, i4, i5, i6);
        }
    if (*i2 == GKS$_SUCCESS)
        return SUCCESS;
    else
        return FAIL;
    }

/* ************************                                                         */

publicdef INT wm_gqwkt (i1, i2, i3, ra1, ra2, ra3, ra4)
GKS_INT i1;
GKS_INT *i2;
GKS_INT *i3;
GKS_REAL *ra1;
GKS_REAL *ra2;
GKS_REAL *ra3;
GKS_REAL *ra4;
    {
    gqwkt (&i1, i2, i3, ra1, ra2, ra3, ra4);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_grensg (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    grensg (&i1, &i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_grqlc (wkid, device, status, tnr, xpos, ypos)
GKS_INT wkid;
GKS_INT device;
GKS_INT *status;
GKS_INT *tnr;
GKS_REAL *xpos;
GKS_REAL *ypos;
    {
    grqlc (&wkid, &device, status, tnr, xpos, ypos);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gqwkm (errind, max_op, max_ac, max_seg)
GKS_INT *errind;
GKS_INT *max_op;
GKS_INT *max_ac;
GKS_INT *max_seg;
    {
    gqwkm (errind, max_op, max_ac, max_seg);
    return(SUCCESS);
    }

/* ************************************************************************         */

publicdef INT wm_grqpk (i1, i2, i3, i4, i5)
GKS_INT i1;
GKS_INT i2;
GKS_INT *i3;
GKS_INT *i4;
GKS_INT *i5;
    {
    grqpk (&i1, &i2, i3, i4, i5);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_grsgwk (wkid)
GKS_INT wkid;
    {
    INT defmod, regmod, ier, surface, newframe;
    
    if (!ug_if_gpx ())
        grsgwk (&wkid);             /* raster must wipe the surface.                */
    
    else
        {
        /* Check the current values for deferral mode & implicit                    */
        /* regeneration mode in the state list.                                     */
        
        gqwkdu (&wkid, &ier, &defmod, &regmod, &surface, &newframe);
        
        if (newframe == GYES)
            wm_guwk (wkid, GPERFO); /* Regenerate screen display                    */
        else
            wm_guwk (wkid, GPOSTP); /* release deferred output only.                */
        }
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschh (chh)
DOUBLE chh;
    {
    GKS_REAL x = chh;
    
    if (last.text.height == chh)
        return SUCCESS;
#ifdef USE_X
    if (x <= 0.0)
        {
        x = chh = 1.0;
        }
#endif
    gschh (&x);
    last.text.height = chh;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschsp (chsp)
DOUBLE chsp;
    {
    GKS_REAL x = chsp;
    gschsp (&x);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschup (chux, chuy)
DOUBLE chux, chuy;
    {
    GKS_REAL x = chux, y = chuy;
    if ((last.text.up_x == chux) && (last.text.up_y == chuy))
        return SUCCESS;
    gschup (&x, &y);
    last.text.up_x = chux;
    last.text.up_y = chuy;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschxp (chxp)
DOUBLE chxp;
    {
    GKS_REAL x = chxp;
    
    if (last.text.expansion == chxp)
        return SUCCESS;
#ifdef USE_X
    if (x > 4.0 OR x <= 0.0)
        {
        x = 1.0;                    /* bear right by default                        */
        }
#endif
    gschxp (&x);
    last.text.expansion = chxp;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsclip (i1)
GKS_INT i1;
    {
    gsclip (&i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gscr (id, ic, r, g, b)
GKS_INT id;
GKS_INT ic;
DOUBLE r;
DOUBLE g;
DOUBLE b;
    {
    GKS_REAL local_r = r;
    GKS_REAL local_g = g;
    GKS_REAL local_b = b;
    
    gscr (&id, &ic, &local_r, &local_g, &local_b);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsds (i1, i2, i3)
GKS_INT i1, i2, i3;
    {
    /* Raster driver doesn't support deferral. JSC Jan, 89                          */
    if (ug_if_gpx ())
        {
        if (i1 != 0)                /* MGKS                                         */
            gsds (&i1, &i2, &i3);
        }
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gsdtec (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    gsdtec (&i1, &i2);
    return SUCCESS;
    }

/* ************************                                                         */
/* Select normalization xform                                                       */

publicdef INT wm_gselnt (nt)
GKS_INT nt;
    {
    gselnt (&nt);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfaci (coli)
COLOR_INDEX coli;
    {
    if (last.fill.color == coli)
        return SUCCESS;
    gsfaci (&coli);
    last.fill.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfais (ints)
FILL_AREA_INTERIOR_STYLE ints;
    {
    if (last.fill.style == ints)
        return SUCCESS;
    gsfais (&ints);
    last.fill.style = ints;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfasi (ints)
FILL_AREA_STYLE_INDEX ints;
    {
    gsfasi (&ints);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gshlit (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    gshlit (&i1, &i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsln (ltype)
LINE_STYLE_INDEX ltype;
    {
    if (last.line.style == ltype)
        return SUCCESS;
    gsln (&ltype);
    last.line.style = ltype;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gslwsc (lwidth)
DOUBLE lwidth;
    {
    GKS_REAL x = lwidth;
    if (last.line.width == lwidth)
        return SUCCESS;
    gslwsc (&x);
    last.line.width = lwidth;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsmk (pmk)
GKS_INT pmk;
    {
    gsmk (&pmk);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsmksc (mksc)
DOUBLE mksc;
    {
    GKS_REAL x = mksc;
    gsmksc (&x);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gspkid (i1)
GKS_INT i1;
    {
    gspkid (&i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsplci (coli)
COLOR_INDEX coli;
    {
    if (coli == last.line.color)
        return SUCCESS;
    gsplci (&coli);
    last.line.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gspmci (coli)
COLOR_INDEX coli;
    {
    gspmci (&coli);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gssgp (segment, prior)
GKS_INT segment;                    /* segment name                                 */
DOUBLE prior;                       /* segment priority                             */
    {
    GKS_REAL pr = prior;
    gssgp (&segment, &pr);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gssgt (i1, ra1)
GKS_INT i1;
GKS_REAL *ra1;
    {
    gssgt (&i1, ra1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxal (txalh, txalv)
GKS_INT txalh, txalv;
    {
    gstxal (&txalh, &txalv);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxci (coli)
COLOR_INDEX coli;
    {
    if (last.text.color == coli)
        return SUCCESS;
    gstxci (&coli);
    last.text.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxfp (font, prec)
GKS_INT font, prec;
    {
    if ((last.text.font == font) && (last.text.prec == prec))
        return SUCCESS;
#ifdef USE_X
    if (font != 1 AND (font > -100))
        {
        font = 1;                   /* Default DEC GKS software font. SIMPLEX ROMAN */
        }
    if (prec < 0 OR prec > 2)
        {
        prec = 1;                   /* Default DEC GKS software precision. STRING   */
        }
#endif
    gstxfp (&font, &prec);
    last.text.font = font;
    last.text.prec = prec;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxp (txp)
GKS_INT txp;
    {
    if (last.text.path == txp)
        return SUCCESS;
#ifdef USE_X
    if (txp > 3 OR txp < 0)
        {
        txp = 1;                    /* bear right by default                        */
        }
#endif
    gstxp (&txp);
    last.text.path = txp;
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gsvis (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    gsvis (&i1, &i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsvp (tnr, x1, x2, x3, x4)
GKS_INT tnr;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gsvp (&tnr, &y1, &y2, &y3, &y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsvpip (i1, i2, i3)
GKS_INT i1, i2, i3;
    {
    gsvpip (&i1, &i2, &i3);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswkvp (ws, x1, x2, x3, x4)
GKS_INT ws;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gswkvp (&ws, &y1, &y2, &y3, &y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswkwn (ws, x1, x2, x3, x4)
GKS_INT ws;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    
    gswkwn (&ws, &y1, &y2, &y3, &y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswn (tnr, x1, x2, x3, x4)
GKS_INT tnr;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gswn (&tnr, &y1, &y2, &y3, &y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gtx (x0, y0, chars)
DOUBLE x0, y0;
CHAR *chars;
    {
    GKS_REAL x = x0, y = y0;
    gtx (&x, &y, hoz_str_descpt (chars));
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_guwk (i1, i2)
GKS_INT i1, i2;
    {
    if (i1 > 0)
        {
        guwk (&i1, &i2);
        }
    return SUCCESS;
    }

publicdef INT wm_gqsga (sgna, errind, segtm, vis, high, sgpr, det)
GKS_INT sgna;
GKS_INT *errind;
GKS_REAL segtm[2][3];
GKS_INT *vis;
GKS_INT *high;
GKS_REAL *sgpr;
GKS_INT *det;
    {
    gqsga (&sgna, errind, segtm, vis, high, sgpr, det);
    return SUCCESS;
    }

#endif /* ifndef PRIOR_GKS */

#endif /* vms */

/* ************************************************************************         */

#ifdef primos             /* Begin Primos code */

/* ************************                                                         */

publicdef INT wm_gacwk (i1)
GKS_INT i1;
    {
    gacwk (i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gasgwk (i1, i2)
GKS_INT i1, i2;
    {
    gasgwk (i1, i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclks ()
    {
    gclks ();
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclrwk (i1, i2)
GKS_INT i1, i2;
    {
    gclrwk (i1, i2);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gclsg ()
    {
    gclsg ();
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclwk (i1)
GKS_INT i1;
    {
    gclwk (i1);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gcrsg (i)
GKS_INT i;
    {
    gcrsg (i);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gdawk (i1)
GKS_INT i1;
    {
    gdawk (i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gdsg (segment)
GKS_INT segment;                    /* segment name to delete                       */
    {
    gdsg (segment);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gevtm (r1, r2, r3, r4, r5, r6, r7, i1, ra1)
DOUBLE r1;
DOUBLE r2;
DOUBLE r3;
DOUBLE r4;
DOUBLE r5;
DOUBLE r6;
DOUBLE r7;
GKS_INT i1;
GKS_REAL *ra1;
    {
    GKS_REAL x1 = r1;
    GKS_REAL x2 = r2;
    GKS_REAL x3 = r3;
    GKS_REAL x4 = r4;
    GKS_REAL x5 = r5;
    GKS_REAL x6 = r6;
    GKS_REAL x7 = r7;
    
    gevtm (x1, x2, x3, x4, x5, x6, x7, i1, (GKS_REAL[])ra1);
    
    return SUCCESS;
    
    }

/* ************************                                                         */

publicdef INT wm_gfa (n, px, py)
GKS_INT n;
GKS_REAL *px, *py;                  /* these are arrays                             */
    {
    gfa (n, (GKS_REAL[])px, (GKS_REAL[])py);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_ginsg (i1, r1)
GKS_INT i1;
GKS_REAL *r1;
    {
    ginsg (i1, (GKS_REAL[])r1);
    }

/* ************************                                                         */

publicdef INT wm_gkzqbtn (buttonPtr)
INT *buttonPtr;
    {
    gkzqbtn (*buttonPtr);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gopks (errfil, ibuf)
GKS_INT errfil;                     /* error file unit number                       */
GKS_INT ibuf;                       /* dummy parameter (not used)                   */
    {
    gopks (errfil, ibuf);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gopwk (i1, i2, i3)
GKS_INT i1, i2, i3;
    {
    gopwk (i1, i2, i3);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gpl (n, px, py)
GKS_INT n;
GKS_REAL *px, *py;                  /* these are arrays                             */
    {
    gpl (n, (GKS_REAL[])px, (GKS_REAL[])py);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gpm (n, px, py)
GKS_INT n;
GKS_REAL *px, *py;
    {
    gpm (n, (GKS_REAL[])px, (GKS_REAL[])py);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gqcntn (i1, i2)
GKS_INT *i1;
GKS_INT *i2;
    {
    gqcntn (*i1, *i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqdsp (wtype, errind, dcunit, rx, ry, lx, ly)
GKS_INT wtype;
GKS_INT *errind;
GKS_INT *dcunit;
GKS_REAL *rx, *ry;
GKS_INT *lx, *ly;
    {
    gqdsp (wtype, *errind, *dcunit, *rx, *ry, *lx, *ly);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqfaci (statusPtr, intPtr)
GKS_INT *statusPtr;
COLOR_INDEX *intPtr;
    {
    gqfaci (*statusPtr, *intPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqfais (statusPtr, intPtr)
GKS_INT *statusPtr;
FILL_AREA_INTERIOR_STYLE *intPtr;
    {
    gqfais (*statusPtr, *intPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqfasi (statusPtr, intPtr)
GKS_INT *statusPtr;
FILL_AREA_STYLE_INDEX *intPtr;
    {
    gqfasi (*statusPtr, *intPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqln (statusPtr, linePtr)
GKS_INT *statusPtr;
LINE_STYLE_INDEX *linePtr;
    {
    gqln (*statusPtr, *linePtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqlwsc (statusPtr, scalePtr)
GKS_INT *statusPtr;
GKS_REAL *scalePtr;
    {
    gqlwsc (*statusPtr, *scalePtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqnt (tnr, statusPtr, windowPtr, viewPtr)
GKS_INT tnr;
GKS_INT *statusPtr;
GKS_REAL windowPtr[4];
GKS_REAL viewPtr[4];
    {
    gqnt (tnr, *statusPtr, (GKS_REAL[])windowPtr, (GKS_REAL[])viewPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqplci (statusPtr, colorPtr)
GKS_INT *statusPtr;
COLOR_INDEX *colorPtr;
    {
    gqplci (*statusPtr, *colorPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqtxx (wkid, px, py, str, errind, cpx, cpy, txexpx, txexpy)
GKS_INT wkid, *errind;
DOUBLE px, py;
GKS_REAL *cpx, *cpy, txexpx[4], txexpy[4];
CHAR *str;
    {
    GKS_REAL x = px, y = py;
    gqtxx (wkid, x, y, (CHAR[])str, *errind, *cpx, *cpy, (GKS_REAL[])txexpx,
           (GKS_REAL[])txexpy, 0, 0, 0, strlen (str));
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkc (i1, i2, i3, i4)
GKS_INT i1, *i2, *i3, *i4;
    {
    gqwkc (i1, *i2, *i3, *i4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkcl (wtype, statusPtr, wclassPtr)
GKS_INT wtype;
GKS_INT *statusPtr;
GKS_INT *wclassPtr;
    {
    gqwkcl (wtype, *statusPtr, *wclassPtr);
    return *statusPtr;
    }

/* ************************                                                         */

publicdef INT wm_gqwkdu (i1, i2, i3, i4, i5, i6)
GKS_INT i1, *i2, *i3, *i4, *i5, *i6;
    {
    if (ug_if_gpx ())
        {
        gqwkdu (i1, *i2, *i3, *i4, *i5, *i6);
        }
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkt (i1, i2, i3, ra1, ra2, ra3, ra4)
GKS_INT i1;
GKS_INT *i2;
GKS_INT *i3;
GKS_REAL *ra1;
GKS_REAL *ra2;
GKS_REAL *ra3;
GKS_REAL *ra4;
    {
    gqwkt (i1, *i2, *i3, (GKS_REAL[])ra1, (GKS_REAL[])ra2, (GKS_REAL[])ra3,
           (GKS_REAL[])ra4);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_grensg (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    grensg (i1, i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_grqlc (wkid, device, status, tnr, xpos, ypos)
GKS_INT wkid;
GKS_INT device;
GKS_INT *status;
GKS_INT *tnr;
GKS_REAL *xpos;
GKS_REAL *ypos;
    {
    grqlc (wkid, device, *status, *tnr, *xpos, *ypos);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkm (errind, max_op, max_ac, max_seg)
GKS_INT *errind;
GKS_INT *max_op;
GKS_INT *max_ac;
GKS_INT *max_seg;
    {
    gqwkm (*errind, *max_op, *max_ac, *max_seg);
    return(SUCCESS);
    }

/* ************************************************************************         */

publicdef INT wm_grqpk (i1, i2, i3, i4, i5)
GKS_INT i1;
GKS_INT i2;
GKS_INT *i3;
GKS_INT *i4;
GKS_INT *i5;
    {
    grqpk (i1, i2, *i3, *i4, *i5);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_grsgwk (wkid)
GKS_INT wkid;
    {
    grsgwk (wkid);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschh (chh)
DOUBLE chh;
    {
    GKS_REAL x = chh;
    
    if (last.text.height == chh)
        return SUCCESS;
#ifdef USE_X
    if (x <= 0.0)
        {
        x = chh = 1.0;
        }
#endif
    gschh (x);
    last.text.height = chh;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschsp (chsp)
DOUBLE chsp;
    {
    GKS_REAL x = chsp;
    gschsp (x);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschup (chux, chuy)
DOUBLE chux, chuy;
    {
    GKS_REAL x = chux, y = chuy;
    if ((last.text.up_x == chux) && (last.text.up_y == chuy))
        return SUCCESS;
    gschup (x, y);
    last.text.up_x = chux;
    last.text.up_y = chuy;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschxp (chxp)
DOUBLE chxp;
    {
    GKS_REAL x = chxp;
    
    if (last.text.expansion == chxp)
        return SUCCESS;
#ifdef USE_X
    if (x > 4.0 OR x <= 0.0)
        {
        x = 1.0;                    /* bear right by default                        */
        }
#endif
    gschxp (x);
    last.text.expansion = chxp;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsclip (i1)
GKS_INT i1;
    {
    gsclip (i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsds (i1, i2, i3)
GKS_INT i1, i2, i3;
    {
    if (i1 != 0)                    /* MGKS                                         */
        gsds (i1, i2, i3);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gsdtec (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    gsdtec (i1, i2);
    return SUCCESS;
    }

/* *************************                                                        */

publicdef INT wm_gselnt (nt)
GKS_INT nt;
    {
    gselnt (nt);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfaci (coli)
COLOR_INDEX coli;
    {
    if (last.fill.color == coli)
        return SUCCESS;
    gsfaci (coli);
    last.fill.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfais (ints)
FILL_AREA_INTERIOR_STYLE ints;
    {
    if (last.fill.style == ints)
        return SUCCESS;
    gsfais (ints);
    last.fill.style = ints;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfasi (ints)
FILL_AREA_STYLE_INDEX ints;
    {
    gsfasi (ints);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gshlit (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    gshlit (i1, i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsln (ltype)
LINE_STYLE_INDEX ltype;
    {
    if (last.line.style == ltype)
        return SUCCESS;
    gsln (ltype);
    last.line.style = ltype;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gslwsc (lwidth)
DOUBLE lwidth;
    {
    GKS_REAL x = lwidth;
    if (last.line.width == lwidth)
        return SUCCESS;
    gslwsc (x);
    last.line.width = lwidth;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsmk (pmk)
GKS_INT pmk;
    {
    gsmk (pmk);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsmksc (mksc)
DOUBLE mksc;
    {
    GKS_REAL x = mksc;
    gsmksc (x);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gspkid (i1)
GKS_INT i1;
    {
    gspkid (i1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsplci (coli)
COLOR_INDEX coli;
    {
    if (coli == last.line.color)
        return SUCCESS;
    gsplci (coli);
    last.line.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gspmci (coli)
COLOR_INDEX coli;
    {
    gspmci (coli);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gssgp (segment, prior)
GKS_INT segment;                    /* segment name                                 */
GKS_REAL prior;                     /* segment priority                             */
    {
    GKS_REAL pr = prior;
    gssgp (segment, pr);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gssgt (i1, ra1)
GKS_INT i1;
GKS_REAL *ra1;
    {
    gssgt (i1, (GKS_REAL[])ra1);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxal (txalh, txalv)
GKS_INT txalh, txalv;
    {
    gstxal (txalh, txalv);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxci (coli)
COLOR_INDEX coli;
    {
    if (last.text.color == coli)
        return SUCCESS;
    gstxci (coli);
    last.text.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxfp (font, prec)
GKS_INT font, prec;
    {
    if ((last.text.font == font) && (last.text.prec == prec))
        return SUCCESS;
    gstxfp (font, prec);
    last.text.font = font;
    last.text.prec = prec;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxp (txp)
GKS_INT txp;
    {
    if (last.text.path == txp)
        return SUCCESS;
#ifdef USE_X
    if (txp > 3 OR txp < 0)
        {
        txp = 1;                    /* bear right by default                        */
        }
#endif
    gstxp (txp);
    last.text.path = txp;
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gsvis (i1, i2)
GKS_INT i1;
GKS_INT i2;
    {
    gsvis (i1, i2);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsvp (tnr, x1, x2, x3, x4)
GKS_INT tnr;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gsvp (tnr, y1, y2, y3, y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsvpip (i1, i2, i3)
GKS_INT i1, i2, i3;
    {
    gsvpip (i1, i2, i3);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswkvp (ws, x1, x2, x3, x4)
GKS_INT ws;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gswkvp (ws, y1, y2, y3, y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswkwn (ws, x1, x2, x3, x4)
GKS_INT ws;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gswkwn (ws, y1, y2, y3, y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswn (tnr, x1, x2, x3, x4)
GKS_INT tnr;
DOUBLE x1, x2, x3, x4;
    {
    GKS_REAL y1 = x1, y2 = x2, y3 = x3, y4 = x4;
    gswn (tnr, y1, y2, y3, y4);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gtx (x0, y0, chars)
DOUBLE x0, y0;
CHAR *chars;
    {
    gtx ((GKS_REAL)x0, (GKS_REAL)y0, (CHAR[])chars, 0, 0, strlen (chars));
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_guwk (i1, i2)
GKS_INT i1, i2;
    {
    guwk (i1, i2);
    return SUCCESS;
    }


#endif      /* end of Primos code */

#ifdef SUN_GKS_OR_PRIOR_GKS   /* ========== BEGIN SUN OR PRIOR GKS ========== */
#define MAX_PTS 512

#ifndef ESI_STDIO_H

#include "esi_stdio.h"

#endif

#ifndef SUN_AGKSTYPE_H
#include "sun_agkstype.h"
#endif
#ifndef SUN_AGKSFUNC_H
#include "sun_agksfunc.h"
#endif
#ifndef SUN_ANSILONG_H
#include "sun_ansilong.h"
#endif

#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */

#include "prior_escconst.h"

#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========= */

#include "sun_gkswsconfig.h"

#endif  /* ========== END SUN GKS ========== */
publicdef INT wm_gkzqbtn (buttonPtr)
INT *buttonPtr;
    {
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gacwk (ws)
GKS_INT ws;
    {
    gactivatews (ws);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gasgwk (ws, seg)
GKS_INT ws, seg;
    {
    gassocsegws (ws, seg);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclks ()
    {
    gclosegks ();
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclrwk (ws, clearflag)
GKS_INT ws, clearflag;
    {
    gclearws (ws, clearflag);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gclsg ()
    {
    gcloseseg ();
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gclwk (ws)
GKS_INT ws;
    {
    gclosews (ws);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gcrsg (seg)
GKS_INT seg;
    {
    gcreateseg (seg);
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gdawk (ws)
GKS_INT ws;
    {
    gdeactivatews (ws);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gdsg (seg)
GKS_INT seg;                        /* segment name to delete                       */
    {
    gdeleteseg (seg);
    return SUCCESS;
    }

/* ************************                                                         */
publicdef INT wm_gevtm (x0, y0, dx, dy, phi, fx, fy, sw, mout)
DOUBLE x0;
DOUBLE y0;
DOUBLE dx;
DOUBLE dy;
DOUBLE phi;
DOUBLE fx;
DOUBLE fy;
GKS_INT sw;
GKS_REAL *mout;
    {
    
    Gsegtran segtran;
    Gnpoint point;
    Gnpoint shift;
    Gfloat angle;
    Gwpoint scale;
    Gcsw coord;
    
    point.x = x0;
    point.y = y0;
    shift.x = dx;
    shift.y = dy;
    angle = phi;
    scale.x = fx;
    scale.y = fy;
    coord = sw;
    
    segtran = gevaltran (&point, &shift, angle, &scale, coord);
    mout[0] = segtran.m11;
    mout[1] = segtran.m12;
    mout[2] = segtran.m13;
    mout[3] = segtran.m21;
    mout[4] = segtran.m22;
    mout[5] = segtran.m23;
    
    return SUCCESS;
    
    }

/* ************************                                                         */
publicdef INT wm_gfa (n, pxa, pya)
GKS_INT n;
GKS_REAL *pxa, *pya;
    {
    Gwpoint *points;
    Gfloat *a, *b;
    Gwpoint *c;
    int i;
    points = (Gwpoint *) tc_zalloc (n * (sizeof(Gwpoint) + 1));
    for (i = 0, a = pxa, b = pya, c = points; i < n; i++, c++, a++, b++)
        {
        c->x = *a;
        c->y = *b;
        }
    gfillarea (n, points);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_ginsg (seg, tmat)
GKS_INT seg;
GKS_REAL *tmat;
    {
    Gsegtran transform;
    
    transform.m11 = tmat[0];
    transform.m12 = tmat[1];
    transform.m13 = tmat[2];
    transform.m21 = tmat[3];
    transform.m22 = tmat[4];
    transform.m23 = tmat[5];
    ginsertseg (seg, &transform);
    return SUCCESS;
    }


/* ************************                                                         */

publicdef INT wm_gopks (errfil, memory)
FILE *errfil;                       /* error file pointer                           */
Glong memory;                       /* bytes of memory available for buffer space (not used)    */
    {
    gopengks (errfil, memory);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gopwk (ws, con, wtype)
GKS_INT ws, con, wtype;
    {
    Gchar *conid;
    Gchar type[12];
    switch (wtype)
        {
    case 1:
        strcpy (type, "wiss");
        break;
    case 2:
        strcpy (type, "mi");
        break;
    case 3:
        strcpy (type, "moasc");
        break;
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
        /* PRIOR GKS - x11 for MIT X11, Open Windows, OSF/Motif (colour)            */
    case 1100:
        strcpy (type, "x11");
        break;
        /* PRIOR GKS - x11m for MIT X11, Open Windows, OSF/Motif (monochrome)       */
    case 1101:
        strcpy (type, "x11m");
        break;
#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========== */
    case 4:
        strcpy (type, "sun_tool");
        break;
    case 5:
        strcpy (type, "sun_canvas");
        break;
#endif  /* ========== END SUN GKS ========== */
    case 6:
        strcpy (type, "hpgl");
        break;
    case 7:
        strcpy (type, "postscript");
        break;
    case 8:
        strcpy (type, "cgmo");
        break;
    case 9:
        strcpy (type, "cgmi");
        break;
        }
    conid = NULL;
    gopenws (ws, conid, type);
    return SUCCESS;
    }

/* ************************                                                         */
publicdef INT wm_gpl (n, pxa, pya)
GKS_INT n;
GKS_REAL *pxa, *pya;
    {
    Gwpoint *point;
    Gwpoint *p;
    Gfloat *a, *b;
    Gwpoint *c;
    int i;
    int npts = 0;

    point = (Gwpoint *) tc_zalloc (n * (sizeof(Gwpoint) + 1));
    for (i = 0, a = pxa, b = pya, c = point; i < n; i++, c++, a++, b++)
        {
        c->x = *a;
        c->y = *b;
        }
    for (i = 0, p = point ; i < n - 1; npts--, i += npts, p += npts)
        {
        npts = MIN (MAX_PTS, n - i);
        gpolyline (npts, p);
        }
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gpm (n, pxa, pya)
GKS_INT n;
GKS_REAL *pxa, *pya;
    {
    Gwpoint *points;
    Gfloat *a, *b;
    Gwpoint *c;
    int i;
    points = (Gwpoint *) tc_zalloc (n * (sizeof(Gwpoint) + 1));
    for (i = 0, a = pxa, b = pya, c = points; i < n; i++, c++, a++, b++)
        {
        c->x = *a;
        c->y = *b;
        }
    gpolymarker (n, points);
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gqcntn (errind, ctnr)
GKS_INT *errind;
GKS_INT *ctnr;
    {
    *ctnr = ginqcurnormtrannum (errind);
    return SUCCESS;
    }
/* ************************                                                         */
publicdef INT wm_gqchh (status, char_height)
GKS_INT *status;
GKS_REAL *char_height;
    {
    *char_height = ginqcharheight (status);
    return SUCCESS;
    }

/* ************************                                                         */
publicdef INT wm_gqchup (status, up_x, up_y)
GKS_INT *status;
GKS_REAL *up_x;
GKS_REAL *up_y;
    {
    Gwpoint p;
    p = ginqcharup (status);
    *up_x = p.x;
    *up_y = p.y;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqdsp (wtype, errind, dcunit, rx, ry, lx, ly)
GKS_INT wtype;
GKS_INT *errind;
GKS_INT *dcunit;
GKS_REAL *rx, *ry;
GKS_INT *lx, *ly;
    {
    Gdspsize dspsize;
    Gchar type[12];
    switch (wtype)
        {
    case 1:
        strcpy (type, "wiss");
        break;
    case 2:
        strcpy (type, "mi");
        break;
    case 3:
        strcpy (type, "moasc");
        break;
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
        /* PRIOR GKS - x11 for MIT X11, Open Windows, OSF/Motif (colour)            */
    case 1100:
        strcpy (type, "x11");
        break;
        /* PRIOR GKS - x11m for MIT X11, Open Windows, OSF/Motif (monochrome)       */
    case 1101:
        strcpy (type, "x11m");
        break;
#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========== */
    case 4:
        strcpy (type, "sun_tool");
        break;
    case 5:
        strcpy (type, "sun_canvas");
        break;
#endif  /* ========== END SUN GKS ========== */
    case 6:
        strcpy (type, "hpgl");
        break;
    case 7:
        strcpy (type, "postscript");
        break;
    case 8:
        strcpy (type, "cgmo");
        break;
    case 9:
        strcpy (type, "cgmi");
        break;
        }
    dspsize = ginqmaxdisplaysize (type, errind);
    *dcunit = dspsize.units;
    *rx = dspsize.device.x;
    *ry = dspsize.device.y;
    *lx = dspsize.raster.x;
    *ly = dspsize.raster.y;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqfaci (errind, coli)
GKS_INT *errind;
COLOR_INDEX *coli;
    {
    Gcolour colour;
    colour = ginqfillcolour (errind);
    *coli = colour;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqfais (errind, ints)
GKS_INT *errind;
FILL_AREA_INTERIOR_STYLE *ints;
    {
    Gflinter flinter;
    
    flinter = ginqfillintstyle (errind);
    *ints = flinter;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqfasi (errind, styli)
GKS_INT *errind;
FILL_AREA_STYLE_INDEX *styli;
    {
    Gflstyle flstyle;
    flstyle = ginqfillstyleindex (errind);
    *styli = flstyle;
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gqln (errind, ltype)
GKS_INT *errind;
LINE_STYLE_INDEX *ltype;
    {
    Glntype intype;
    
    intype = ginqlinetype (errind);
    *ltype = intype;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqlwsc (errind, lwidth)
GKS_INT *errind;
GKS_REAL *lwidth;
    {
    Gscale scale;
    scale = ginqlinewidth (errind);
    *lwidth = scale;
    return SUCCESS;
    }

/* ************************                                                         */
publicdef INT wm_gqnt (tnr, errind, windowPtr, viewPtr)
GKS_INT tnr;
GKS_INT *errind;
GKS_REAL windowPtr[4];
GKS_REAL viewPtr[4];
    {
    Gwlimit wlimit;
    Gnlimit nlimit;
    wlimit = ginqwindow (tnr, errind);
    windowPtr[0] = wlimit.xmin;
    windowPtr[1] = wlimit.xmax;
    windowPtr[2] = wlimit.ymin;
    windowPtr[3] = wlimit.ymax;
    nlimit = ginqviewport (tnr, errind);
    viewPtr[0] = nlimit.xmin;
    viewPtr[1] = nlimit.xmax;
    viewPtr[2] = nlimit.ymin;
    viewPtr[3] = nlimit.ymax;
    
    return SUCCESS;
    }


/* ************************                                                         */

publicdef INT wm_gqplci (errind, coli)
GKS_INT *errind;
COLOR_INDEX *coli;
    {
    Gcolour colour;
    colour = ginqlinecolour (errind);
    *coli = colour;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqtxx (wkid, px, py, str, errind, cpx, cpy, txexpx, txexpy)
GKS_INT wkid, *errind;
DOUBLE px, py;
GKS_REAL *cpx, *cpy, txexpx[4], txexpy[4];
CHAR *str;
    {
    Gextent extent;
    Gwpoint position;
    GKS_REAL chh, chxp, chsp, chw, chup_x, chup_y;
    GKS_INT txp, txal_h, txal_v;
    CHAR ws_type[40];
    INT str_len;
    
    position.x = px;
    position.y = py;
    extent = ginqtextextent (wkid, position, str, errind);
    /* PRIOR_GKS for vms - informational compile error 
                         - but &position doesn't work. */

    if (*errind == SUCCESS)
        {
        *cpx = extent.concat.x;
        *cpy = extent.concat.y;
        txexpx[0] = extent.ll.x;
        txexpx[1] = extent.lr.x;
        txexpx[2] = extent.ur.x;
        txexpx[3] = extent.ul.x;
        txexpy[0] = extent.ll.y;
        txexpy[1] = extent.lr.y;
        txexpy[2] = extent.ur.y;
        txexpy[3] = extent.ul.y;
        return SUCCESS;
        }
    /* ------------------------------------------------------                       */
    /* The following is a hack to solve the metafile problem                        */
    /*   this call return error status 39 when the workstation is metafile          */
    /*   this break our plotting software.                                          */
    /*   to solve this just for the time being, we only consider                    */
    /*   text with text path right and text alignment normal (or left and base)     */
    /*   and calculate the x width for that text label                              */
    /*   otherwise return error status 39.                                          */
    /*   To really solve the problem, we should caluclate the text extent           */
    /*   considered different text attritude, text alignment, text path, up vector  */
    /*   .... and have a good default for the font precision.                       */
    /*                                                                              */
    /*   P.S. the character width from gqchw may not be the one that we want        */
    /*   it may be changed to some other default value                              */
    
    if (*errind == 39)
        {
        qs_inq_c (QS_WORKSTATION_TYPE, ws_type, (INT *)0);
        if (strcmp (ws_type, "METAFILE") != 0)
            {
            return *errind;
            }
        else
            {
            str_len = strlen (str);
            wm_gqchh (errind, &chh);
            wm_gqchxp (errind, &chxp);
            wm_gqtxp (errind, &txp);
            wm_gqchsp (errind, &chsp);
            wm_gqchup (errind, &chup_x, &chup_y);
            wm_gqchw (errind, &chw);
            wm_gqtxal (errind, &txal_h, &txal_v);
            if (chup_x == 0 && chup_y == 1 && txp == 0 && (txal_h == GAHNOR ||
                                                           txal_h == GALEFT) &&
                (txal_v == GAVNOR || txal_v == GABASE))
                {
                txexpx[0] = px;
                txexpx[1] = px + str_len * chw * chxp + (str_len * chsp * chh * chxp);
                txexpx[2] = txexpx[1];
                txexpx[3] = px;
                txexpy[0] = py;
                txexpy[1] = py;
                txexpy[2] = py + chh;
                txexpy[3] = txexpy[2];
                *cpx = txexpx[1];
                *cpy = py;
                }
	    else if (chup_x == 0 && chup_y == 1 && txp == 0 && 
		(txal_h == GACENT) && 
		(txal_v == GAVNOR || txal_v == GABASE))
	      {
		txexpx[0] = px - str_len/2*chw*chxp - (str_len/2*chsp*chh*chxp);
		txexpx[1] = px + str_len/2*chw*chxp + (str_len/2*chsp*chh*chxp);
		txexpx[2] = txexpx[1];
		txexpx[3] = px;
		txexpy[0] = py;
		txexpy[1] = py;
		txexpy[2] = py + chh;
		txexpy[3] = txexpy[2];	  
		*cpx = txexpx[1];
		*cpy = py;
	      }
	    else if (chup_x == 0 && chup_y == 1 && txp == 0 && 
		(txal_h == GACENT) && 
		(txal_v == GAHALF))
	      {
		txexpx[0] = px - str_len/2*chw*chxp - (str_len/2*chsp*chh*chxp);
		txexpx[1] = px + str_len/2*chw*chxp + (str_len/2*chsp*chh*chxp);
		txexpx[2] = txexpx[1];
		txexpx[3] = px;
		txexpy[0] = py - chh/2;
		txexpy[1] = py - chh/2;
		txexpy[2] = py + chh/2;
		txexpy[3] = txexpy[2];	  
		*cpx = txexpx[1];
		*cpy = py;
	      }
	    else if (chup_x == 0 && chup_y == 1 && txp == 0 && 
		(txal_h == GAHNOR || txal_h == GALEFT) && 
		(txal_v == GAHALF))
	      {
		txexpx[0] = px;
		txexpx[1] = px + str_len*chw*chxp + (str_len*chsp*chh*chxp);
		txexpx[2] = txexpx[1];
		txexpx[3] = px;
		txexpy[0] = py - chh/2;
		txexpy[1] = py - chh/2;
		txexpy[2] = py + chh/2;
		txexpy[3] = txexpy[2];	  
		*cpx = txexpx[1];
		*cpy = py;
	      }
            else
                {
                return(*errind = 39);
                }
            }
        }
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gqwkc (ws, errind, conid, wtype)
GKS_INT ws, *errind, *conid, *wtype;
    {
    Gwsct wsct;
    wsct = ginqwsconntype (ws, errind);
    if (wsct.conn == NULL)
        *conid = 0;
    
    if (strcmp (wsct.type, "wiss") == 0)
        *wtype = 1;
    else if (strcmp (wsct.type, "mi") == 0)
        *wtype = 2;
    else if (strcmp (wsct.type, "moasc") == 0)
        *wtype = 3;
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
    else if (strcmp (wsct.type, "x11") == 0)
        *wtype = 1100;
    else if (strcmp (wsct.type, "x11m") == 0)
        *wtype = 1101;
#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========== */
    else if (strcmp (wsct.type, "sun_tool") == 0)
        *wtype = 4;
    else if (strcmp (wsct.type, "sun_canvas") == 0)
        *wtype = 5;
#endif  /* ========== END SUN GKS ========== */
    else if (strcmp (wsct.type, "hpgl") == 0)
        *wtype = 6;
    else if (strcmp (wsct.type, "postscript") == 0)
        *wtype = 7;
    else if (strcmp (wsct.type, "cmgo") == 0)
        *wtype = 8;
    else if (strcmp (wsct.type, "cmgi") == 0)
        *wtype = 9;
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
    else
        *wtype = 1100;              /* PRIOR GKS - default is X11 colour workstation*/
#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========== */
    else
        *wtype = 4;
#endif  /* ========== END SUN GKS ========== */
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gqwkcl (wtype, errind, wclassPtr)
GKS_INT wtype;
GKS_INT *errind;
GKS_INT *wclassPtr;
    {
    Gwsclass wsclass;
    Gchar type[12];
    switch (wtype)
        {
    case 1:
        strcpy (type, "wiss");
        break;
    case 2:
        strcpy (type, "mi");
        break;
    case 3:
        strcpy (type, "moasc");
        break;
#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
        /* PRIOR GKS - x11 for MIT X11, Open Windows, OSF/Motif (colour)            */
    case 1100:
        strcpy (type, "x11");
        break;
        /* PRIOR GKS - x11m for MIT X11, Open Windows, OSF/Motif (monochrome)       */
    case 1101:
        strcpy (type, "x11m");
        break;
#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========== */
    case 4:
        strcpy (type, "sun_tool");
        break;
    case 5:
        strcpy (type, "sun_canvas");
        break;
#endif  /* ========== END SUN GKS ========== */
    case 6:
        strcpy (type, "hpgl");
        break;
    case 7:
        strcpy (type, "postscript");
        break;
    case 8:
        strcpy (type, "cgmo");
        break;
    case 9:
        strcpy (type, "cgmi");
        break;
        }
    wsclass = ginqwsclass (type, errind);
    *wclassPtr = wsclass;
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gqwkdu (ws, errind, defmod, regmod, dempty, nframe)
GKS_INT ws, *errind, *defmod, *regmod, *dempty, *nframe;
    {
    Gwsdus wsdus;
    wsdus = ginqwsdeferralupdate (ws, errind);
    *defmod = wsdus.defmode;
    *regmod = wsdus.irgmode;
    *dempty = wsdus.dspsurf;
    *nframe = wsdus.nframe;
    
    if (*errind == SUCCESS)         /* equivalent to VMS GKS$_SUCCESS               */
        return SUCCESS;
    else
        return FAIL;
    }

/* ************************                                                         */
publicdef INT wm_gqwkt (ws, errind, tus, rwindo, cwindo, rviewp, cviewp)
GKS_INT ws;
GKS_INT *errind;
GKS_INT *tus;
GKS_REAL rwindo[4];
GKS_REAL cwindo[4];
GKS_REAL rviewp[4];
GKS_REAL cviewp[4];
    {
    Gwsti wsti;
    
    wsti = ginqwstran (ws, errind);
    *tus = wsti.wstus;
    rwindo[0] = wsti.request.w.xmin;
    rwindo[1] = wsti.request.w.xmax;
    rwindo[2] = wsti.request.w.ymin;
    rwindo[3] = wsti.request.w.ymax;
    cwindo[0] = wsti.current.w.xmin;
    cwindo[1] = wsti.current.w.xmax;
    cwindo[2] = wsti.current.w.ymin;
    cwindo[3] = wsti.current.w.ymax;
    rviewp[0] = wsti.request.v.xmin;
    rviewp[1] = wsti.request.v.xmax;
    rviewp[2] = wsti.request.v.ymin;
    rviewp[3] = wsti.request.v.ymax;
    cviewp[0] = wsti.current.v.xmin;
    cviewp[1] = wsti.current.v.xmax;
    cviewp[2] = wsti.current.v.ymin;
    cviewp[3] = wsti.current.v.ymax;
    
    return SUCCESS;
    }


/* ************************************************************************         */

publicdef INT wm_grensg (old, new)
GKS_INT old;
GKS_INT new;
    {
    grenameseg (old, new);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_grqpk (ws, dev, stat, sgna, pkid)
GKS_INT ws;
GKS_INT dev;
GKS_INT *stat;
GKS_INT *sgna;
GKS_INT *pkid;
    {
    Gqpick qpick;
    qpick = greqpick (ws, dev);
    *stat = qpick.status;
    *sgna = qpick.seg;
    *pkid = qpick.pickid;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_grsgwk (ws)
GKS_INT ws;
    {
    gredrawsegws (ws);              /* But raster must wipe the surface.            */
    
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gsasf (flags)
GKS_INT flags[13];
    {
    Gasfs asf;
    asf.ln_type = flags[0];
    asf.ln_width = flags[1];
    asf.ln_colour = flags[2];
    asf.mk_type = flags[3];
    asf.mk_size = flags[4];
    asf.mk_colour = flags[5];
    asf.tx_fp = flags[6];
    asf.tx_exp = flags[7];
    asf.tx_space = flags[8];
    asf.tx_colour = flags[9];
    asf.fl_inter = flags[10];
    asf.fl_style = flags[11];
    asf.fl_colour = flags[12];
    gsetasf (&asf);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschh (chh)
DOUBLE chh;
    {
    Gchrht height = chh;
    
    gsetcharheight (height);
    last.text.height = chh;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschsp (chsp)
DOUBLE chsp;
    {
    Gchrsp spacing;
    spacing = chsp;
    gsetcharspace (spacing);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschup (chux, chuy)
DOUBLE chux, chuy;
    {
    Gwpoint charup;
    
    charup.x = chux;
    charup.y = chuy;
    gsetcharup (&charup);
    last.text.up_x = chux;
    last.text.up_y = chuy;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gschxp (chxp)
DOUBLE chxp;
    {
    Gchrexp exp = chxp;
    
    gsetcharexp (exp);
    last.text.expansion = chxp;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsclip (clsw)
GKS_INT clsw;
    {
    gsetclip (clsw);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gscr (ws, ci, cr, cg, cb)
GKS_INT ws;
GKS_INT ci;
DOUBLE cr;
DOUBLE cg;
DOUBLE cb;
    {
    GKS_REAL r = cr;
    GKS_REAL g = cg;
    GKS_REAL b = cb;
    Gcobundl rep;

    rep.red = r;
    rep.green = g;
    rep.blue = b;
    gsetcolourrep (ws, ci, &rep);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsds (ws, defmod, regmod)
GKS_INT ws, defmod, regmod;
    {
    if (ws != 0)                    /* MGKS                                         */
        gsetdeferral (ws, defmod, regmod);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gsdtec (seg, det)
GKS_INT seg;
GKS_INT det;
    {
    gsetdet (seg, det);
    return SUCCESS;
    }
/* ************************************************************************         */

publicdef INT wm_gsdyns (ws, state)
GKS_INT ws;
GKS_INT state;
    {

#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
    /* PRIOR GKS - no such function call in PRIOR GKS.                              */
    /*   Please refer to Appendix A of "Prior Gks C User's Manual"                  */
    
    Gescin esc_in;
    Gescout esc_out;
    Gint int_data[1];
    
    if (ws != 0)                    /* MGKS                                         */
        {
        switch (state)
            {
        case G_DYN_OPT_UNDRAW:
        case G_DYN_OPT_ERASE:
            gsetdeferral (ws, GASAP, GSUPPD);   /* implicit regeneration suppressed */
            break;
        case G_DYN_ON_UNDRAW:
        case G_DYN_ON_ERASE:
            gsetdeferral (ws, GASAP, GALLOW);   /* implicit regeneration allowed    */
            break;
        case G_DYN_OFF:
            break;
            }
        esc_in.esc_function = GESC_SIRGSTRAT;
        esc_in.esc_route = G_SPECIFIC_WS;
        esc_in.esc_ws = ws;
        esc_in.n_ints = 1;
        int_data[0] = state;
        esc_in.int_array = int_data;
        esc_in.n_reals = 0;
        esc_in.n_strings = 0;
        esc_out = gescape (NULL, &esc_in);
        /* escape fuction to set implicit regeneration strategy                     */
        }
#else  /* ========== END PRIOR GKS = BEGIN SUN GKS ========== */
    gsetdynamicseg (ws, state);
#endif  /* ========== END SUN GKS ========== */
    return SUCCESS;
    
    }

/* ************************                                                         */
/* Select normalization xform                                                       */

publicdef INT wm_gselnt (tnr)
GKS_INT tnr;
    {
    gselnormtran (tnr);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfaci (coli)
COLOR_INDEX coli;
    {
    gsetfillcolour (coli);
    last.fill.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfais (ints)
FILL_AREA_INTERIOR_STYLE ints;
    {
    gsetfillintstyle (ints);
    last.fill.style = ints;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsfasi (ints)
FILL_AREA_STYLE_INDEX ints;
    {
    gsetfillstyleindex (ints);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gshlit (seg, hil)
GKS_INT seg;
GKS_INT hil;
    {
    gsethighlight (seg, hil);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsln (ltype)
LINE_STYLE_INDEX ltype;
    {
    gsetlinetype (ltype);
    last.line.style = ltype;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gslwsc (lwidth)
DOUBLE lwidth;
    {
    Gscale x = lwidth;
    gsetlinewidth (x);
    last.line.width = lwidth;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsmk (pmk)
GKS_INT pmk;
    {
    gsetmarkertype (pmk);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsmksc (mksc)
DOUBLE mksc;
    {
    Gscale size = mksc;
    gsetmarkersize (size);
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gspkid (pkid)
GKS_INT pkid;
    {
    gsetpickid (pkid);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsplci (coli)
COLOR_INDEX coli;
    {
    gsetlinecolour (coli);
    last.line.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gspmci (coli)
COLOR_INDEX coli;
    {
    gsetmarkercolour (coli);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gssgp (segment, prior)
GKS_INT segment;                    /* segment name                                 */
DOUBLE prior;                       /* segment priority                             */
    {
    Gsegpri priority;
    priority = prior;
    gsetsegpri (segment, priority);
    return SUCCESS;
    }

/* ************************************************************************         */
publicdef INT wm_gssgt (seg, m)
GKS_INT seg;
GKS_REAL *m;
    {
    Gsegtran transform;
    transform.m11 = m[0];
    transform.m12 = m[1];
    transform.m13 = m[2];
    transform.m21 = m[3];
    transform.m22 = m[4];
    transform.m23 = m[5];
    gsetsegtran (seg, &transform);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxal (txalh, txalv)
GKS_INT txalh, txalv;
    {
    Gtxalign txalign;
    txalign.hor = txalh;
    txalign.ver = txalv;
    gsettextalign (&txalign);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxci (coli)
COLOR_INDEX coli;
    {
    gsettextcolour (coli);
    last.text.color = coli;
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gstxfp (font, prec)
GKS_INT font, prec;
    {
    Gtxfp txfp;
    
    if ((last.text.font == font) && (last.text.prec == prec))
        return SUCCESS;

#ifdef PRIOR_GKS  /* ========== BEGIN PRIOR GKS ========== */
    /* valid font range from -1 to -16 and -100                                     */
    if ((font < -17 OR font > -1) AND font != -100)
        {
        font = -3;                  /* Default PRIOR GKS software font. SIMPLEX ROMAN   */
        }
    if (prec < 0 OR prec > 2)
        {
        prec = 1;                   /* Default PRIOR GKS software precision. STRING */
        }
#endif  /* ========== END PRIOR GKS ========== */
    txfp.font = font;
    txfp.prec = prec;
    gsettextfontprec (&txfp);
    last.text.font = font;
    last.text.prec = prec;
    return SUCCESS;
    
    }

/* ************************                                                         */

publicdef INT wm_gstxp (txp)
GKS_INT txp;
    {
    gsettextpath (txp);
    last.text.path = txp;
    return SUCCESS;
    }

/* ************************************************************************         */

publicdef INT wm_gsvis (seg, vis)
GKS_INT seg;
GKS_INT vis;
    {
    gsetvis (seg, vis);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsvp (tnr, xmin, xmax, ymin, ymax)
GKS_INT tnr;
DOUBLE xmin, xmax, ymin, ymax;
    {
    Gnlimit viewport;
    viewport.xmin = xmin;
    viewport.xmax = xmax;
    viewport.ymin = ymin;
    viewport.ymax = ymax;
    gsetviewport (tnr, &viewport);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gsvpip (tnr, rtnr, relpri)
GKS_INT tnr, rtnr, relpri;
    {
    gsetviewportpri (tnr, rtnr, relpri);
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_gswkvp (ws, xmin, xmax, ymin, ymax)
GKS_INT ws;
DOUBLE xmin, xmax, ymin, ymax;
    {
    Gdlimit viewport;
    viewport.xmin = xmin;
    viewport.xmax = xmax;
    viewport.ymin = ymin;
    viewport.ymax = ymax;
    gsetwsviewport (ws, &viewport);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswkwn (ws, xmin, xmax, ymin, ymax)
GKS_INT ws;
DOUBLE xmin, xmax, ymin, ymax;
    {
    Gnlimit window;
    window.xmin = xmin;
    window.xmax = xmax;
    window.ymin = ymin;
    window.ymax = ymax;
    gsetwswindow (ws, &window);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gswn (tnr, xmin, xmax, ymin, ymax)
GKS_INT tnr;
DOUBLE xmin, xmax, ymin, ymax;
    {
    Gwlimit window;
    window.xmin = xmin;
    window.xmax = xmax;
    window.ymin = ymin;
    window.ymax = ymax;
    gsetwindow (tnr, &window);
    return SUCCESS;
    }

/* ************************                                                         */

publicdef INT wm_gtx (x0, y0, chars)
DOUBLE x0, y0;
CHAR *chars;
    {
    Gwpoint position;
    position.x = x0;
    position.y = y0;
    gtext (&position, chars);
    return SUCCESS;
    }
/* ************************                                                         */

publicdef INT wm_guwk (ws, regfl)
GKS_INT ws, regfl;
    {
    if (ws > 0)
        gupdatews (ws, regfl);
    return SUCCESS;
    }

/* ************************                                                         */

/*************************************************************************          */
/**   SUN:  C bindings for CHOICE DEVICE and LOCATOR DEVICE                         */
/**************************************************************************         */
publicdef INT wm_gqchs (wks_id, choice_dev, record_buflen, err_stat, input_mode,
                        echo_flag, initial_status, initial_choice, echo_type,
                        echo_area, record_size, datrec)
GKS_INT wks_id;
GKS_INT choice_dev;
GKS_INT record_buflen;
GKS_INT *err_stat;
GKS_INT *input_mode;
GKS_INT *echo_flag;
GKS_INT *initial_status;
GKS_INT *initial_choice;
GKS_INT *echo_type;
GKS_REAL echo_area[];
GKS_INT *record_size;
Gchoicerec *datrec;
    {
    Gchoicest state;
    state = ginqchoicestate (wks_id, choice_dev, err_stat);
    *input_mode = state.mode;
    *echo_flag = state.esw;
    *initial_choice = state.choice;
    *echo_type = state.pet;
    echo_area[0] = state.e_area.xmin;
    echo_area[1] = state.e_area.xmax;
    echo_area[2] = state.e_area.ymin;
    echo_area[3] = state.e_area.ymax;
    *datrec = state.record;
    return SUCCESS;
    }

publicdef INT wm_ginch (wks_id, choice_dev, initial_status, initial_choice, echo_type,
                        echo_area, record_buflen, datrec)
GKS_INT wks_id;
GKS_INT choice_dev;
GKS_INT initial_status;
GKS_INT initial_choice;
GKS_INT echo_type;
GKS_REAL echo_area[];
GKS_INT record_buflen;
Gchoicerec *datrec;
    {
    ginitchoice (wks_id, choice_dev, initial_choice, echo_type, echo_area, datrec);
    return SUCCESS;
    }

publicdef INT wm_gschm (wks_id, choice_dev, mode, echoflag)
GKS_INT wks_id;
GKS_INT choice_dev;
GKS_INT mode;
GKS_INT echoflag;
    {
    gsetchoicemode (wks_id, choice_dev, mode, echoflag);
    return SUCCESS;
    }

publicdef INT wm_gsmch (wks_id, choice_dev, status, choice)
GKS_INT wks_id;
GKS_INT choice_dev;
GKS_INT *status;
GKS_INT *choice;
    {
    Gint returned_choice;
    returned_choice = gsamplechoice (wks_id, choice_dev);
    *choice = returned_choice;
    return SUCCESS;
    }

publicdef INT wm_grqch (wks_id, choice_dev, status, choice)
GKS_INT wks_id;
GKS_INT choice_dev;
GKS_INT *status;
GKS_INT *choice;
    {
    Gqchoice returned_choice;
    returned_choice = greqchoice (wks_id, choice_dev);
    *status = returned_choice.status;
    *choice = returned_choice.choice;
    return SUCCESS;
    }

publicdef INT wm_gqlcs (wks_id, locate_dev, type, record_buflen, status, mode,
                        echo_flag, xform, world_x, world_y, echo_type, echo_area,
                        record_size, datrec)
GKS_INT wks_id;
GKS_INT locate_dev;
GKS_INT type;
GKS_INT record_buflen;
GKS_INT *status;
GKS_INT *mode;
GKS_INT *echo_flag;
GKS_INT *xform;
GKS_REAL *world_x;
GKS_REAL *world_y;
GKS_INT *echo_type;
GKS_REAL echo_area[];
GKS_INT *record_size;
Glocrec *datrec;
    {
    Glocst state;
    state = ginqlocstate (wks_id, locate_dev, type, status);
    *mode = state.mode;
    *echo_flag = state.esw;
    *xform = state.loc.transform;
    *world_x = state.loc.position.x;
    *world_y = state.loc.position.y;
    *echo_type = state.pet;
    echo_area[0] = state.e_area.xmin;
    echo_area[1] = state.e_area.xmax;
    echo_area[2] = state.e_area.ymin;
    echo_area[3] = state.e_area.ymax;
    *datrec = state.record;
    
    return SUCCESS;
    }

publicdef INT wm_ginlc (wks_id, locate_dev, world_x, world_y, xform, echo_type,
                        echo_area, record_buflen, locate_datrec)
GKS_INT wks_id;
GKS_INT locate_dev;
GKS_INT xform;
DOUBLE world_x;
DOUBLE world_y;
GKS_INT echo_type;
GKS_REAL echo_area[];
GKS_INT record_buflen;
Glocrec *locate_datrec;
    {
    Gloc *pinit;
    Gloc init;
    Gdlimit *parea;
    Gdlimit area;
    GKS_REAL wx = world_x;
    GKS_REAL wy = world_y;

    init.transform = xform;
    init.position.x = wx;
    init.position.y = wy;
    pinit = &init;
    area.xmin = echo_area[0];
    area.xmax = echo_area[1];
    area.ymin = echo_area[2];
    area.ymax = echo_area[3];
    parea = &area;
    ginitloc (wks_id, locate_dev, pinit, echo_type, parea, locate_datrec);
    return SUCCESS;
    }

publicdef INT wm_gslcm (wks_id, locate_dev, mode, echoflag)
GKS_INT wks_id;
GKS_INT locate_dev;
GKS_INT mode;
GKS_INT echoflag;
    {
    gsetlocmode (wks_id, locate_dev, mode, echoflag);
    return SUCCESS;
    }

publicdef INT wm_gsmlc (wks_id, locate_dev, xform, world_x, world_y)
GKS_INT wks_id;
GKS_INT locate_dev;
GKS_INT *xform;
GKS_REAL *world_x;
GKS_REAL *world_y;
    {
    Gloc locator;
    locator = gsampleloc (wks_id, locate_dev);
    *xform = locator.transform;
    *world_x = locator.position.x;
    *world_y = locator.position.y;
    return SUCCESS;
    }

publicdef INT wm_gwait (timeout, wks_id, icl, idnr)
GKS_REAL timeout;
GKS_INT *wks_id;
GKS_INT *icl;
GKS_INT *idnr;
    {
    Gevent event;
    event = gawaitevent (timeout);
    *wks_id = event.ws;
    *icl = event.class ;
    *idnr = event.dev;
    return SUCCESS;
    }

publicdef INT wm_gflush (wks_id, icl, idnr)
GKS_INT wks_id;
GKS_INT icl;
GKS_INT idnr;
    {
    gflushevents (wks_id, icl, idnr);
    return SUCCESS;
    }

publicdef INT wm_ggtlc (xform, world_x, world_y)
GKS_INT *xform;
GKS_REAL *world_x;
GKS_REAL *world_y;
    {
    Gloc locator;
    locator = ggetloc ();
    *xform = locator.transform;
    *world_x = locator.position.x;
    *world_y = locator.position.y;
    return SUCCESS;
    }

publicdef INT wm_grqlc (wks_id, locate_dev, status, xform, world_x, world_y)
GKS_INT wks_id;
GKS_INT locate_dev;
GKS_INT *status;
GKS_INT *xform;
GKS_REAL *world_x;
GKS_REAL *world_y;
    {
    Gqloc returned_loc;
    returned_loc = greqloc (wks_id, locate_dev);
    *status = returned_loc.status;
    *xform = returned_loc.loc.transform;
    *world_x = returned_loc.loc.position.x;
    *world_y = returned_loc.loc.position.y;
    return SUCCESS;
    }

publicdef INT wm_gqwkm (errind, max_op, max_ac, max_seg)
GKS_INT *errind;
GKS_INT *max_op;
GKS_INT *max_ac;
GKS_INT *max_seg;
    {
    Gwsmax wsmax;
    
    wsmax = ginqwsmaxnum (errind);
    *max_op = wsmax.open;
    *max_ac = wsmax.active;
    *max_seg = wsmax.assoc;
    
    return(SUCCESS);
    }

publicdef INT wm_cfg_ws (tool_x, tool_y, tool_width, tool_height, textsw, label,
                         msg_win_size)
GKS_INT tool_x;
GKS_INT tool_y;
GKS_INT tool_width;
GKS_INT tool_height;
GKS_INT msg_win_size;
GKS_INT textsw;
CHAR label[];
    {
#ifndef PRIOR_GKS  /* ========== BEGIN SUN GKS ========== */
    Gks_ws_config config;
    
    config = gks_ws_config_get_installed ("sun_tool");
    gks_ws_config_set (config, GKS_TOOL_X, tool_x, GKS_TOOL_Y, tool_y, GKS_TOOL_WIDTH,
                       tool_width, GKS_TOOL_HEIGHT, tool_height, GKS_TOOL_LABEL, label,
                       GKS_MSG_WIN_SIZE, msg_win_size, 0);
    gks_ws_config_install (config);
    gks_ws_config_destroy (config);
#endif  /* ========== END SUN GKS ========== */
    return SUCCESS;
    }

publicdef INT wm_gqsga (sgna, errind, segtm, vis, high, sgpr, det)
GKS_INT sgna;
GKS_INT *errind;
GKS_REAL segtm[2][3];
GKS_INT *vis;
GKS_INT *high;
GKS_REAL *sgpr;
GKS_INT *det;
    {
    Gsegtran segtran;
    
    segtran = ginqsegtran (sgna, errind);
    segtm[0][0] = segtran.m11;
    segtm[0][1] = segtran.m12;
    segtm[0][2] = segtran.m13;
    segtm[1][0] = segtran.m21;
    segtm[1][1] = segtran.m22;
    segtm[1][2] = segtran.m23;
    
    *vis = ginqvis (sgna, errind);
    *high = ginqhighlight (sgna, errind);
    *sgpr = ginqsegpri (sgna, errind);
    *det = ginqdet (sgna, errind);
    
    return SUCCESS;
    }

publicdef INT wm_ggtitm (wkid, type, length)
GKS_INT wkid;
GKS_INT *type;
GKS_INT *length;
    {
    Ggksmit item;
    
    item = ggetgksm (wkid);
    *type = item.type;
    *length = item.length;
    return SUCCESS;
    }

publicdef INT wm_giitm (type, len, dim, data)
GKS_INT type;
GKS_INT len;
GKS_INT dim;
CHAR *data;
    {
    Ggksmit item;
    
    item.type = type;
    item.length = len;
    ginterpret (&item, data);
    free (data);
    return SUCCESS;
    }

publicdef INT wm_grditm (wkid, len, dim, data)
GKS_INT wkid;
GKS_INT len;
GKS_INT dim;
CHAR *data;
    {
    data = greadgksm (wkid, len);
    return SUCCESS;
    }

publicdef INT wm_gwitm (wkid, type, len, dim, data)
GKS_INT wkid;
GKS_INT type;
GKS_INT len;
GKS_INT dim;
CHAR *data;
    {
    gwritegksm (wkid, type, len, data);
    return SUCCESS;
    }

publicdef INT wm_gqchw (errind, chw)
GKS_INT *errind;
GKS_REAL *chw;
    {
    *chw = ginqcharwidth (errind);
    return SUCCESS;
    }

publicdef INT wm_gqtxp (errind, txp)
GKS_INT *errind;
GKS_REAL *txp;
    {
    *txp = ginqtextpath (errind);
    return SUCCESS;
    }

publicdef INT wm_gqtxal (errind, txalh, txalv)
GKS_INT *errind;
GKS_INT *txalh;
GKS_INT *txalv;
    {
    Gtxalign txal;
    
    txal = ginqtextalign (errind);
    *txalh = txal.hor;
    *txalv = txal.ver;
    return SUCCESS;
    }

publicdef INT wm_gqchxp (errind, chxp)
GKS_INT *errind;
GKS_REAL *chxp;
    {
    *chxp = ginqcharexp (errind);
    return SUCCESS;
    }

publicdef INT wm_gqchsp (errind, chsp)
GKS_INT *errind;
GKS_REAL *chsp;
    {
    *chsp = ginqcharspace (errind);
    return SUCCESS;
    }


#endif  /* SUN */

