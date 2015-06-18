/* DEC/CMS REPLACEMENT HISTORY, Element CT_UOM_FACT_DEG.C */
/* *2     6-MAY-1991 16:45:14 JULIAN "(SPR 0) add uom conversion from and to degrees type" */
/* *1     6-MAY-1991 16:38:50 JULIAN "data mover function" */
/* DEC/CMS REPLACEMENT HISTORY, Element CT_UOM_FACT_DEG.C */
/* DEC/CMS REPLACEMENT HISTORY, Element CT_UOM_FACT.C*/
/* *4    14-AUG-1990 13:08:00 VINCE "(SPR 5644) Header Standardization"*/
/* *3     7-MAR-1990 18:12:17 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *2    10-OCT-1989 09:46:00 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    19-JUN-1989 12:45:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_UOM_FACT.C*/
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

#include "esi_ct.h"
#include "esi_math.h"
/* Function Description -----------------------------------------------------
Description:
    Compute the UOM conversion factor between the map projections.
    
Prototype:
    publicdef INT ct_get_uom_factor_degrees
    
Return Value/Status:
    SUCCESS - Successful completion
    Function returns status of tu_conversion_factor.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_get_uom_factor_degrees (DOUBLE node_x, DOUBLE node_y,
					 PROJECTION_STRUCTURE *from_data,
					 DOUBLE *text_size,
					 PROJECTION_STRUCTURE *to_data)
#else
publicdef INT ct_get_uom_factor_degrees(node_x,node_y,from_data,text_size,
					to_data)
DOUBLE node_x,node_y;
PROJECTION_STRUCTURE *from_data;
DOUBLE *text_size;
PROJECTION_STRUCTURE *to_data;
#endif
    {
    INT status;
    DOUBLE dx,dy;
    DOUBLE x1,x2,y1,y2;
    DOUBLE in_text_size;

    in_text_size = *text_size;

    status = ct_transform_point(node_x,node_y,from_data,&x1,&y1,to_data)
             OR
	     ct_transform_point(node_x,node_y+in_text_size,from_data,
				&x2,&y2,to_data);

    if(status != SUCCESS) return status;
    dx = ABS(x1-x2);
    dy = ABS(y1-y2);

    if(dx EQUALS 0) 
      in_text_size = dy;
    else if (dy EQUALS 0)
      in_text_size = dx;
    else
      in_text_size = sqrt((dx*dx) + (dy*dy));

    *text_size = in_text_size;

    return status;
    }
/* END:     */




