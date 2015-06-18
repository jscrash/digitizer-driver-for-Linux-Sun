/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_LAB.C*/
/* *4    26-JUL-1990 17:19:15 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:03:14 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:05:21 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DRW_LAB.C*/
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
#include "esi_wm.h"

#define RIGHT  3
#define CENTER 2
#define LEFT   1
#define NORMAL 0
#define BASE   NORMAL
#define TOP    1

/* Function Description -----------------------------------------------------
Description:
    Function to draw shot label and Z value arrays.

Prototype:
    publicdef INT so_draw_label_arr(SO_STRUCT *pso,FLOAT    *x,FLOAT    *y,FLOAT    *zvalue,
        FLOAT    *shot_point,BOOL     *special,BOOL     *lab,BOOL     *zval,INT len);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    x               -(FLOAT *)
    y               -(FLOAT *)
    zvalue          -(FLOAT *)
    shot_point      -(FLOAT *)
    special         -(BOOL *)
    lab             -(BOOL *)
    zval            -(BOOL *)
    len             -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_draw_label_arr (SO_STRUCT *pso, FLOAT *x, FLOAT *y, FLOAT *zvalue,
                                 FLOAT *shot_point, BOOL *special, BOOL *lab,
                                 BOOL *zval, INT len)
#else
publicdef INT so_draw_label_arr (pso, x, y, zvalue, shot_point, special, lab, zval,
                                 len)
SO_STRUCT *pso;
FLOAT *x, *y;
FLOAT *zvalue;
FLOAT *shot_point;
BOOL *special, *lab, *zval;
INT len;
#endif
    {
    INT status, i;
    CHAR label[40];
    DOUBLE base_x, base_y;
    status = SUCCESS;
    
    /* see if no draw */
    
    if ((!
        pso->seismic_generic.draw_shotpoint_label) OR pso->disp_opt.
        shot_label_paraperp == SO_NOPARA)
        {
        return status;
        }
    for (i = 0; i < len; i++)
        {
        if ((shot_point[i] != SO_NULL) AND lab[i])
            {
            
            /* format the string */
            
            sprintf ((char *)label, "%.0f", shot_point[i]);
            
            /* draw shot label text */
            base_x = x[i] + pso->seismic_generic.shotpoint_label.text_offset.x;
            base_y = y[i] + pso->seismic_generic.shotpoint_label.text_offset.y;
            
            /*  Set the text attributes         */
            
            if (i EQUALS 0)
                {
                
                /*  Text path                   */
                wm_gstxp (pso->seismic_generic.shotpoint_label.text_path);
                
                /*  Text Up Vector              */
                wm_gschup (pso->seismic_generic.shotpoint_label.text_up_x,
                           pso->seismic_generic.shotpoint_label.text_up_y);
                
                /*  Text height                 */
                wm_gschh (pso->seismic_generic.shotpoint_label.text_size);
                
                /*  Character Expansion Factor          */
                wm_gschxp (pso->seismic_generic.shotpoint_label.text_expansion_factor);
                
                /*  Text Font & Precision           */
                wm_gstxfp (pso->seismic_generic.shotpoint_label.text_font,
                           pso->seismic_generic.shotpoint_label.text_precision);
                
                /*  Text Alignment              */
                wm_gstxal (pso->seismic_generic.shotpoint_label.text_alignment[0],
                           pso->seismic_generic.shotpoint_label.text_alignment[1]);
                
                /*  Text Color                  */
                wm_gstxci (pso->seismic_generic.shotpoint_label.text_color);
                }
            wm_gtx (base_x, base_y, label);
            }
        }
    for (i = 0; i < len; i++)
        {
        /* draw z value text */
        
        if ((pso->zzzz OR (pso->seismic_generic.horizons_ok AND zvalue[i] !=
                           SO_NULL)) AND zval[i])
            {
            
            /* reverse text layout */
            base_x = x[i] + pso->seismic_generic.zvalue_label.text_offset.x;
            base_y = y[i] + pso->seismic_generic.zvalue_label.text_offset.y;
            
            /* format the string */
            
            if (pso->zzzz)
                {
                strcpy ((char *)label, "zzzz");
                }
            else
                {
                sprintf ((char *)label, "%.0f", zvalue[i]);
                }
            pso->seismic_generic.zvalue_label.text_string = label;
            
            /* draw it */
            if (i EQUALS 0)
                {
                
                /*  Text path                   */
                wm_gstxp (pso->seismic_generic.zvalue_label.text_path);
                
                /*  Text Up Vector              */
                wm_gschup (pso->seismic_generic.zvalue_label.text_up_x,
                           pso->seismic_generic.zvalue_label.text_up_y);
                
                /*  Text height                 */
                wm_gschh (pso->seismic_generic.zvalue_label.text_size);
                
                /*  Character Expansion Factor          */
                wm_gschxp (pso->seismic_generic.zvalue_label.text_expansion_factor);
                
                /*  Text Font & Precision           */
                wm_gstxfp (pso->seismic_generic.zvalue_label.text_font,
                           pso->seismic_generic.zvalue_label.text_precision);
                
                /*  Text Alignment              */
                wm_gstxal (pso->seismic_generic.zvalue_label.text_alignment[0],
                           pso->seismic_generic.zvalue_label.text_alignment[1]);
                
                /*  Text Color                  */
                wm_gstxci (pso->seismic_generic.zvalue_label.text_color);
                }
            wm_gtx (base_x, base_y, label);
            }
        }
    /* if special format (not the line default, reset */
    /*
        for( i = 0; i < len; i++)
        {
        if(special[i])
        {
            so_set_shotpoint_defaults(pso);
        }
        }
    */
    
    return status;
    }
/* END:     */
