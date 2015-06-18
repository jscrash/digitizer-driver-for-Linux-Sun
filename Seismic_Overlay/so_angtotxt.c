/* DEC/CMS REPLACEMENT HISTORY, Element SO_ANGTOTXT.C*/
/*  5B1   3-OCT-1990 11:50:55 GILLESPIE "Change file type to stream"*/
/* *6    20-AUG-1990 17:11:05 KEE "(SPR -1) fixing compile error at sun"*/
/* *5    26-JUL-1990 17:18:06 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *4    25-JUL-1990 20:02:15 VINCE "(SPR 5644) Header Standardization"*/
/* *3     6-SEP-1989 09:22:40 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    25-JUL-1989 20:04:19 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:25:41 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ANGTOTXT.C*/
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

#include "esi_math.h" 
#include "esi_so.h"
#include "esi_am.h"
#include "esi_gks.h"

#define LEFT   1
#define CENTER 2
#define RIGHT  3
#define TOP    1
#define HALF   3
#define BASE   4

/* Function Description -----------------------------------------------------
Description:
    Function to set text attributes.

Prototype:
    publicdef INT so_angpp_to_text(TEXT *ptext,DOUBLE angle,INT paraperp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ptext           -(TEXT  *)
    angle           -(DOUBLE)
    paraperp        -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_angpp_to_text (TEXT *ptext, DOUBLE angle, INT paraperp)
#else
publicdef INT so_angpp_to_text (ptext, angle, paraperp)
TEXT *ptext;
DOUBLE angle;
INT paraperp;
#endif
    {
    INT status;
    INT iquad;
    DOUBLE upang;
    
    status = SUCCESS;
    
    /* determine up vector */
    
    if (paraperp == SO_PERP)
        {
        upang = angle + 90.0;
        }
    else
        {
        upang = angle;
        }
    if (upang >= 360.0)
        {
        upang -= 360.0;
        }
    if (upang >= 180.0 AND paraperp == SO_PERP)
        {
        upang -= 180.0;
        }
    else if (upang > 180.0 AND paraperp == SO_PARA)
        {
        upang -= 180.0;
        }
    /* determine quad */
    
    iquad = angle / 90.0 + 1;
    
    if (paraperp == SO_PARA AND upang == 180.0)
        {
        iquad = 2;
        }
    upang /= 57.295827;
    
    ptext->text_path = 0;
    ptext->text_expansion_factor = 1.0;
    ptext->text_line_spacing = 0.0;
    ptext->text_precision = 1;
    if (ptext->text_font < SIMPLEX)
        {
        ptext->text_font = SIMPLEX;
        }
    ptext->text_up_x = cos (upang);
    ptext->text_up_y = sin (upang);
    
    if (paraperp == SO_PARA)
        {
        ptext->text_alignment[0] = CENTER;
        if (iquad < 3)
            {
            ptext->text_alignment[1] = BASE;
            }
        else
            {
            ptext->text_alignment[1] = TOP;
            }
        }
    else
        {
        ptext->text_alignment[1] = HALF;
        if ((iquad == 2) OR (iquad == 3))
            {
            ptext->text_alignment[0] = RIGHT;
            }
        else
            {
            ptext->text_alignment[0] = LEFT;
            }
        }
    return status;
    }
/* END:     */
