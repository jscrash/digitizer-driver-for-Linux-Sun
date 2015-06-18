/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRAW_LABEL.C*/
/* *4    26-JUL-1990 17:18:46 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:02:52 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:05:02 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRAW_LABEL.C*/
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

#include "esi_c_lib.h"
#include "esi_so.h"
#include "esi_am.h"

#define RIGHT  3
#define CENTER 2
#define LEFT   1
#define NORMAL 0
#define BASE   NORMAL
#define TOP    1

/* Function Description -----------------------------------------------------
Description:
    Function to draw shot label and Z value.

Prototype:
    publicdef INT so_draw_label(SO_STRUCT *pso,COORD x,COORD y,DOUBLE zvalue,SP shot_point,
        BOOL special,BOOL lab,BOOL zval);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    x               -(COORD)
    y               -(COORD)
    zvalue          -(DOUBLE)
    shot_point      -(SP)
    special         -(BOOL)
    lab             -(BOOL)
    zval            -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_label (SO_STRUCT *pso, COORD x, COORD y, DOUBLE zvalue,
                             SP shot_point, BOOL special, BOOL lab, BOOL zval)
#else
publicdef INT so_draw_label (pso, x, y, zvalue, shot_point, special, lab, zval)
SO_STRUCT *pso;
COORD x, y;
DOUBLE zvalue;
SP shot_point;
BOOL special, lab, zval;
#endif
    {
    INT status;
    CHAR label[40];
    
    status = SUCCESS;
    
    /* see if no draw */
    
    if ((!
        pso->seismic_generic.draw_shotpoint_label) OR pso->disp_opt.
        shot_label_paraperp == SO_NOPARA)
        {
        return status;
        }
    if ((shot_point != SO_NULL) AND lab)
        {
        
        /* format the string */
        
        sprintf ((char *)label, "%.0f", shot_point);
        
        /* draw shot label text */
        
        pso->seismic_generic.shotpoint_label.text_string = label;
        pso->seismic_generic.shotpoint_label.text_position.x = x;
        pso->seismic_generic.shotpoint_label.text_position.y = y;
        mm_draw_text (&pso->seismic_generic.shotpoint_label, FALSE);
        }
    /* draw z value text */
    
    if ((pso->zzzz OR (pso->seismic_generic.horizons_ok AND zvalue !=
                       SO_NULL)) AND zval)
        {
        
        /* reverse text layout */
        
        pso->seismic_generic.zvalue_label.text_position.x = x;
        pso->seismic_generic.zvalue_label.text_position.y = y;
        
        /* format the string */
        
        if (pso->zzzz)
            {
            strcpy ((char *)label, "zzzz");
            }
        else
            {
            sprintf ((char *)label, "%.0f", zvalue);
            }
        pso->seismic_generic.zvalue_label.text_string = label;
        
        /* draw it */
        
        mm_draw_text (&pso->seismic_generic.zvalue_label, FALSE);
        
        }
    /* if special format (not the line default, reset */
    /*
        if(special)
            {
            so_set_shotpoint_defaults(pso);
            }
    */
    return status;
    }
/* END:     */
