/* DEC/CMS REPLACEMENT HISTORY, Element TG_SET_TAB_TP.C*/
/* *2    17-AUG-1990 22:27:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:32:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_SET_TAB_TP.C*/
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

/* 12/14/88 GS - removed all fortran calls & replaced with glue calls */

/*
 * File : TG_SET_TABLET_TYPE.C
 */

/* Include files */
#ifndef ESI_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_DEFS_H

#include "esi_qs.h"

#endif

/* Tablet types.  When adding tablet types be sure to modify the following   */
/*                Also, identical codes kept in SQL */
/*                table CODES.GRAPHICS_TABLET_TYPES */

#define NO_TABLET          0
#define GTCO_11X11         1
#define GTCO_11X17         2
#define GTCO_20X20         3
#define GTCO_36X48_UGRID   4
#define GTCO_42X60_UGRID   5
#define GTCO_36X48         6
#define GTCO_42X60         7
#define CALCOMP_2200       8
#define CALCOMP_2500       9
#define CALCOMP_9100      10
#define RTS      rawToScaled
#define STS      scaledToScreen
#define RTSC     rawToScreen
#define STR      screenToReg
#define TTU      tablet_to_usr

/* Function Description -----------------------------------------------------
Description:
    Set tablet type and tablet transforms for GKS.

Prototype:
    publicdef INT tg_set_tablet_type(INT tablet_type, FLOAT screenToReg[6]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) tablet_type -(INT) Tablet type from ESI.GKS.H.
    (O) screenToReg -(FLOAT []) Default tracking registration.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_set_tablet_type (INT tablet_type, FLOAT screenToReg[6])
#else
publicdef INT tg_set_tablet_type (tablet_type, screenToReg)
INT tablet_type;
FLOAT screenToReg[6];
#endif
    {
    
    INT ifRawIdent, ifScaledIdent, ifRegIdent;
    
    FLOAT rawToScaled[6], scaledToScreen[6], rawToScreen[6];
    
    INT tablet_to_usr[16];          /* values that tablet returns to rastertech 1/80 */
    INT button_list[16];
    /*------------------------------------------*/
    /* button values must be mapped back to the */
    /* original puck values. this array will    */
    /* contain these mappings.                  */
    /*------------------------------------------*/
    INT button_mapping[16];
    
    INT index;
    INT id = 1, flag;
    INT status = SUCCESS;
    
    /*---------------------------------------------*/
    /* Used to determine if the two RT corrections */
    /* are to be applied.                          */
    /*---------------------------------------------*/
    INT divby2, add1k;
    
    /*
      This routine has been modified to correct for two Raster Tech firmware
      bugs.  These bugs are:
    
        - an offset of 1000 is subtracted from the tablet values, for some
        firmware levels,
        - the incoming tablet values are divided by two, in some firmware
        levels.
    
      Two QS variables are set in the FI_CONFIG_WORKSTATION routine, and
      indicate if a correction for either or both of the above bugs is
      to be applied.  Only the large tablet reading command (READBU 1,2)
      sees impact from these bugs.
    
      In this code, only the GTCO 36X48 UGRID tablet will be corrected,
      as the other large tablets must be configured to work as UGRIDs,
      and we are not marketing 42X60 units.
    
      December 20,1988 by Anthony P. Lucido.
    */
    
    ifRawIdent = FALSE;
    ifScaledIdent = FALSE;
    
    switch (tablet_type)
        {
        
    case GTCO_11X11:
        
        RTS[0] = 1.0;
        RTS[2] = 0.0;
        RTS[4] = 0.0;
        RTS[1] = 0.0;
        RTS[3] = 1.0;
        RTS[5] = 0.0;
        
        ifRawIdent = TRUE;          /* activate ZRAWXF            */
        
        /* scale "X" by .8 and translate X by -128 for map registration */
        
        STS[0] = 1.0;
        STS[2] = 0.0;
        STS[4] = 0.0;
        STS[1] = 0.0;
        STS[3] = 1.0;
        STS[5] = 0.0;
        
        ifScaledIdent = TRUE;       /* activate ZSCAXF           */
        
        STR[0] = 1.0;
        STR[2] = 0.0;
        STR[4] = 0.0;
        STR[1] = 0.0;
        STR[3] = 1.0;
        STR[5] = 0.0;
        
        ifRegIdent = TRUE;
        
        for (index = 0; index < 16; index++)
            TTU[index] = index;
        
        break;
        
    case GTCO_11X17:
        
        RTS[0] = 1.0;
        RTS[2] = 0.0;
        RTS[4] = 0.0;
        RTS[1] = 0.0;
        RTS[3] = 1.0;
        RTS[5] = 0.0;
        
        ifRawIdent = FALSE;         /* activate ZRAWXF            */
        
        /* scale "X" by .8 and translate X by -128 for map registration */
        
        STS[0] = 0.80;
        STS[2] = 0.0;
        STS[4] = -128.0;
        STS[1] = 0.0;
        STS[3] = 1.0;
        STS[5] = 0.0;
        
        ifScaledIdent = FALSE;      /* activate ZSCAXF           */
        
        STR[0] = 1.25;
        STR[2] = 0.0;
        STR[4] = 0.0;
        STR[1] = 0.0;
        STR[3] = 1.0;
        STR[5] = 0.0;
        
        ifRegIdent = FALSE;
        
        for (index = 0; index < 16; index++)
            TTU[index] = index;
        
        break;
        
    case GTCO_20X20:
        
        RTS[0] = 1.0;
        RTS[2] = 0.0;
        RTS[4] = 0.0;
        RTS[1] = 0.0;
        RTS[3] = 1.0;
        RTS[5] = 0.0;
        
        ifRawIdent = FALSE;         /* activate ZRAWXF            */
        
        /* same as for 11x11 except:                                    */
        /* scale "X" by .8 and translate X by -128 for map registration */
        /* and then scale the whole thing by .55 for 20x20 tablet       */
        
        STS[0] = 0.44;
        STS[2] = 0.0;
        STS[4] = -355.0;
        STS[1] = 0.0;
        STS[3] = 0.55;
        STS[5] = -232.0;
        
        ifScaledIdent = FALSE;      /* activate ZSCAXF           */
        
        STR[0] = 2.272727;
        STR[2] = 0.0;
        STR[4] = 0.0;
        STR[1] = 0.0;
        STR[3] = 1.818182;
        STR[5] = 0.0;
        
        ifRegIdent = FALSE;
        
        for (index = 0; index < 16; index++)
            TTU[index] = index;
        
        break;
        
    case GTCO_36X48_UGRID:
        
        /*
          Get the QS variables values:
        */
        if ((qs_inq_i (QS_DIVIDE_BY_2, &divby2) != SUCCESS) ||
            (qs_inq_i (QS_ADD_1K, &add1k) != SUCCESS))
            status = FAIL;
        else
            {
            RTS[0] = 0.320316;
            RTS[2] = 0.0;
            RTS[4] = add1k ? -320.960950 :  - 0.64494;
            RTS[1] = 0.0;
            RTS[3] = 0.253890;
            RTS[5] = add1k ? -254.523343 :  - 0.633343;
            
            ifRawIdent = FALSE;
            
            STS[0] = 0.133099;
            STS[2] = 0.0;
            STS[4] = -597.408275;
            STS[1] = 0.0;
            STS[3] = 0.167992;
            STS[5] = -469.515734;
            
            ifScaledIdent = FALSE;
            
            STR[0] = divby2 ? 4.0 : 2.0;
            STR[2] = 0.0;
            STR[4] = -1.5;
            STR[1] = 0.0;
            STR[3] = divby2 ? 4.0 : 2.0;
            STR[5] = -0.4;
            
            ifRegIdent = FALSE;
            
            TTU[0] = 0;
            TTU[1] = 1;
            TTU[2] = 2;
            TTU[3] = 3;
            TTU[4] = 12;
            TTU[5] = 4;
            TTU[6] = 5;
            TTU[7] = 6;
            TTU[8] = 13;
            TTU[9] = 7;
            TTU[10] = 8;
            TTU[11] = 9;
            TTU[12] = 14;
            TTU[13] = 10;
            TTU[14] = 15;
            TTU[15] = 11;
            }
        break;
        
    case GTCO_42X60_UGRID:
        
        RTS[0] = 0.320316;
        RTS[2] = 0.0;
        RTS[4] = -320.960950;
        RTS[1] = 0.0;
        RTS[3] = 0.253890;
        RTS[5] = -254.523343;
        
        ifRawIdent = FALSE;
        
        STS[0] = 0.106468;
        STS[2] = 0.0;
        STS[4] = -605.930234;
        STS[1] = 0.0;
        STS[3] = 0.134323;
        STS[5] = -444.032328;
        
        ifScaledIdent = FALSE;
        
        STR[0] = 4.0;
        STR[2] = 0.0;
        STR[4] = -1.5;
        STR[1] = 0.0;
        STR[3] = 4.0;
        STR[5] = -0.4;
        
        ifRegIdent = FALSE;
        
        TTU[0] = 0;
        TTU[1] = 1;
        TTU[2] = 2;
        TTU[3] = 3;
        TTU[4] = 12;
        TTU[5] = 4;
        TTU[6] = 5;
        TTU[7] = 6;
        TTU[8] = 13;
        TTU[9] = 7;
        TTU[10] = 8;
        TTU[11] = 9;
        TTU[12] = 14;
        TTU[13] = 10;
        TTU[14] = 15;
        TTU[15] = 11;
        
        break;
        
    case CALCOMP_2200:
        
        RTS[0] = 0.320316;
        RTS[2] = 0.0;
        RTS[4] = -320.960950;
        RTS[1] = 0.0;
        RTS[3] = 0.253890;
        RTS[5] = -254.523343;
        
        ifRawIdent = FALSE;
        
        STS[0] = 0.399297;
        STS[2] = 0.0;
        STS[4] = -597.408275;
        STS[1] = 0.0;
        STS[3] = 0.500398;
        STS[5] = -469.515734;
        
        ifScaledIdent = FALSE;
        
        STR[0] = 4.0;
        STR[2] = 0.0;
        STR[4] = -1.5;
        STR[1] = 0.0;
        STR[3] = 4.0;
        STR[5] = -0.4;
        
        ifRegIdent = FALSE;
        
        /* tablet returns no button number */
        
        TTU[0] = 13;
        TTU[1] = 10;
        TTU[2] = 8;
        TTU[3] = 11;
        TTU[4] = 7;
        TTU[5] = 12;
        TTU[6] = 9;
        TTU[7] = 14;
        TTU[8] = 15;
        TTU[9] = 5;
        TTU[10] = 2;
        TTU[11] = 4;
        TTU[12] = 1;
        TTU[13] = 6;
        TTU[14] = 3;
        TTU[15] = 0;
        
        break;
        
    case CALCOMP_2500:
        
        RTS[0] = 0.320316;
        RTS[2] = 0.0;
        RTS[4] = -320.960950;
        RTS[1] = 0.0;
        RTS[3] = 0.253890;
        RTS[5] = -254.523343;
        
        ifRawIdent = FALSE;
        
        STS[0] = 0.399297;
        STS[2] = 0.0;
        STS[4] = -597.408275;
        STS[1] = 0.0;
        STS[3] = 0.500398;
        STS[5] = -469.515734;
        
        ifScaledIdent = FALSE;
        
        STR[0] = 4.0;
        STR[2] = 0.0;
        STR[4] = -1.5;
        STR[1] = 0.0;
        STR[3] = 4.0;
        STR[5] = -0.4;
        
        ifRegIdent = FALSE;
        
        /* tablet returns no button number */
        
        TTU[0] = 13;
        TTU[1] = 10;
        TTU[2] = 8;
        TTU[3] = 11;
        TTU[4] = 7;
        TTU[5] = 12;
        TTU[6] = 9;
        TTU[7] = 14;
        TTU[8] = 15;
        TTU[9] = 5;
        TTU[10] = 2;
        TTU[11] = 4;
        TTU[12] = 1;
        TTU[13] = 6;
        TTU[14] = 3;
        TTU[15] = 0;
        
        break;
        
    case CALCOMP_9100:
        
        RTS[0] = 0.320316;
        RTS[2] = 0.0;
        RTS[4] = -320.960950;
        RTS[1] = 0.0;
        RTS[3] = 0.253890;
        RTS[5] = -254.523343;
        
        ifRawIdent = FALSE;
        
        STS[0] = 0.106468;
        STS[2] = 0.0;
        STS[4] = -605.930234;
        STS[1] = 0.0;
        STS[3] = 0.134323;
        STS[5] = -444.032328;
        
        ifScaledIdent = FALSE;
        
        STR[0] = 4.0;
        STR[2] = 0.0;
        STR[4] = -1.5;
        STR[1] = 0.0;
        STR[3] = 4.0;
        STR[5] = -0.4;
        
        ifRegIdent = FALSE;
        
        /* tablet returns no button number */
        
        TTU[0] = 13;
        TTU[1] = 10;
        TTU[2] = 8;
        TTU[3] = 11;
        TTU[4] = 7;
        TTU[5] = 12;
        TTU[6] = 9;
        TTU[7] = 14;
        TTU[8] = 15;
        TTU[9] = 5;
        TTU[10] = 2;
        TTU[11] = 4;
        TTU[12] = 1;
        TTU[13] = 6;
        TTU[14] = 3;
        TTU[15] = 0;
        
        break;
        
    case NO_TABLET:
    case GTCO_36X48:
    case GTCO_42X60:
        
    default:
        
        RTS[0] = 1.0;
        RTS[2] = 0.0;
        RTS[4] = 0.0;
        RTS[1] = 0.0;
        RTS[3] = 1.0;
        RTS[5] = 0.0;
        
        ifRawIdent = TRUE;
        
        STS[0] = 1.0;
        STS[2] = 0.0;
        STS[4] = 0.0;
        STS[1] = 0.0;
        STS[3] = 1.0;
        STS[5] = 0.0;
        
        ifScaledIdent = TRUE;
        
        STR[0] = 1.0;
        STR[2] = 0.0;
        STR[4] = 0.0;
        STR[1] = 0.0;
        STR[3] = 1.0;
        STR[5] = 0.0;
        
        ifRegIdent = TRUE;
        
        for (index = 0; index < 16; index++)
            TTU[index] = index;
        
        break;
        
        }
    for (index = 0; index < 16; index++)
        button_mapping[index] = button_list[index] = index;
    
    for (index = 0; index < 16; index++)
        button_mapping[TTU[index]] = index;
    
    ugstbtyp_gc (tablet_type);
    
    ugsscaxf_gc (id, ifScaledIdent, STS);
    
    if (!(ifRawIdent))
        {
        
        if (!(ifScaledIdent))
            {
            ugcmtm (RTS, STS, RTSC);    /* 1/17/89 GS - call new C routine */
            ugsrawxf_gc (RTSC);
            }
        else
            ugsrawxf_gc (RTS);
        }
    rt_button_macros (button_list);
    ugsbutmap_gc (button_mapping);
    
    return status;
    }
/* END:     */
