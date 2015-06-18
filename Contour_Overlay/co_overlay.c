/* DEC/CMS REPLACEMENT HISTORY, Element CO_OVERLAY.C*/
/*  11   11-MAR-1992 18:12:10 JTM "(SPR 0) checked changes for Don's additions"*/
/*  10   16-JAN-1992 12:04:20 JTM "(SPR 0) Contour Annotation code from Don V."*/
/* *9    11-MAR-1991 10:11:36 KEE "(SPR -1) Modify Charlie Purmoy's Trend and Residual base on QA new requirements"*/
/* *8    19-NOV-1990 14:51:03 JULIAN "(SPR 6171) add fault source option"*/
/* *7    23-OCT-1990 11:13:54 GILLESPIE "(SPR 1000) Merge from Ernie"*/
/*  1B1  23-OCT-1990 10:39:06 GILLESPIE "Merge from Ernie"*/
/* *6    15-SEP-1990 13:11:51 CHARLIE "(SPR -1) enhanced perspective options"*/
/* *5    14-AUG-1990 13:05:47 VINCE "(SPR 5644) Header Standardization"*/
/* *4    25-APR-1990 08:31:50 JULIAN "(SPR 0) remove line_color option from co_overlay"*/
/* *3    24-APR-1990 17:15:01 JULIAN "(SPR 0) fix line color bug being always black"*/
/* *2    26-OCT-1989 09:50:45 CONROY "(SPR 5051) New initialization of color options"*/
/* *1    19-JUN-1989 12:43:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_OVERLAY.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_OVERLAY.C                                */
/* *6    15-SEP-1990 13:11:51 CHARLIE "(SPR -1) enhanced perspective options"       */
/* *5    14-AUG-1990 13:05:47 VINCE "(SPR 5644) Header Standardization"             */
/* *4    25-APR-1990 08:31:50 JULIAN "(SPR 0) remove line_color option from co_overlay"  */
/* *3    24-APR-1990 17:15:01 JULIAN "(SPR 0) fix line color bug being always black" */
/* *2    26-OCT-1989 09:50:45 CONROY "(SPR 5051) New initialization of color options"   */
/* *1    19-JUN-1989 12:43:29 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element CO_OVERLAY.C                                */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_OV_H

#include "esi_ov.h"

#endif

#ifndef ESI_OV_DEFS_H

#include "esi_ov_defs.h"

#endif

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_CO_ERR_H

#include "esi_co_err.h"

#endif

#ifndef ESI_CO_MSG_H

#include "esi_co_msg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif 

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_MD_DBDEFS_H

#include "esi_md_dbdefs.h"

#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Function to process a "CONTOURS" overlay.                                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT co_overlay(INT display_line);                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) display_line    -(INT) Line to be displayed.                              */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion                                               */
/*    CO_CONTOUR_FILE_ERROR - If contours has NUL for a name.                       */
/*    Function returns status returned by co_post_contours, co_make_grid,           */
/*    co_make_contours, and ov_open_overlay.                                        */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT co_overlay (INT display_line)
#else
publicdef INT co_overlay (display_line)
INT display_line;
#endif
/* * VARIABLE LIST *                                                                */
/* -----------------                                                                */
    {
    
    PROGNAME ("CO_OVERLAY");
    CONTOURS contours;              /* "CONTOURS" ORGANIZING STRUCT                 */
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT row;
    INT i;
    INT trend_count = 0;
    INT residual_count = 0;
    INT status;                     /* RETURN STATUS                                */
    CHAR msg[80];
    static CHAR *type_string[] = 
        {
        "CONTOURS", "CONTOUR PERSPECTIVE", "CONTOUR KEY_PERSPECTIVE",
        "MESH PERSPECTIVE", "RIDGE VIEW", "TRACE VIEW", 0
        };
    
    /********************************************************************************/
    
    status = SUCCESS;
    
    /* * INITIALIZE STRUCTURES USED IN OVERLAY *                                    */
    /* -----------------------------------------                                    */
    
    tc_zeroes (&contours, sizeof(CONTOURS));
    contours.bold_line_color = -1;
    contours.fine_line_color = -1;
    contours.grid_type = SURFACE_TYPE;    

    /* * FILL IN ORGANIZING STRUCTURES *                                            */
    /* ---------------------------------                                            */
    
    mp_get_current_map (&mapdef);
    
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];
    
    for (i=display_line; i<display->last_row_used; i++)
      {
	if (display->verb_ndx[i] == -1)
	  break;
	else if (strcmp(display->keywords[i],"TREND ORDER") == 0)
	  {
	    trend_count++;
	  }
	else if (strcmp(display->keywords[i],"RESIDUAL ORDER") == 0)
	  {
	    residual_count++;
	  }
      }
    if ((trend_count + residual_count) > 1)
      {
	status = CO_TREND_RESIDUAL;
	goto finished;
      }

    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];
            
            switch (verbstruc->key_code[i])
                {
            case 1:                 /* BOLD LINE COLOR                              */
                contours.bold_line_color = parmstruc->parmval.ival;
                break;
                
            case 2:                 /* BOLD LINE INTERVAL                           */
                contours.bold_line_interval = parmstruc->parmval.dbval;
                break;
                
            case 3:                 /* BOUNDED BY DATA                              */
                contours.bounded_by_data = TRUE;
                break;
                
            case 4:
            case 26:                /* CONTOUR FILE                                 */
                contours.contour_file = parmstruc->parmval.cval;
                break;
                
            case 5:                 /* CONTOUR INTERVAL                             */
                contours.contour_interval = parmstruc->parmval.cval;
                break;
                
            case 6:                 /* CONTOUR MAXIMUM                              */
                contours.contour_maximum = parmstruc->parmval.cval;
                break;
                
            case 7:                 /* CONTOUR MINIMUM                              */
                contours.contour_minimum = parmstruc->parmval.cval;
                break;
                
            case 8:                 /* FAULTED                                      */
                contours.faulted = parmstruc->parmval.cval;
                break;
                
            case 9:                 /* FINE LINE COLOR                              */
                contours.fine_line_color = parmstruc->parmval.ival;
                break;
                
            case 10:
            case 27:                /* GRID FILE                                    */
                contours.grid_file = parmstruc->parmval.cval;
                break;
                
            case 11:                /* GRID INTERVAL                                */
                contours.grid_interval = parmstruc->parmval.cval;
                break;
                
            case 12:                /* GRID MAXIMUM                                 */
                contours.grid_maximum = parmstruc->parmval.cval;
                break;
                
            case 13:                /* GRID MINIMUM                                 */
                contours.grid_minimum = parmstruc->parmval.cval;
                break;
                
            case 14:                /* ISOPACH                                      */
                contours.isopach = TRUE;
                break;
                
            case 15:                /* LABEL INTERVAL                               */
                contours.label_interval = parmstruc->parmval.dbval;
                break;
                
            case 17:                /* NAME                                         */
                contours.name = parmstruc->parmval.cval;
                break;
                
            case 18:                /* SUPPRESS SMOOTHING                           */
                contours.suppress_smoothing = TRUE;
                break;
                
            case 19:                /* XYZ FILE                                     */
                contours.xyz_file = parmstruc->parmval.cval;
                break;
                
            case 20:                /* use file defaults                            */
                contours.use_file_defaults = TRUE;
                break;
                
            case 21:                /* VERT EXAG                                    */
                contours.vertical_exaggeration = parmstruc->parmval.dbval;
                break;
                
            case 22:                /* VIEW AZIM                                    */
                contours.view_azimuth = parmstruc->parmval.dbval;
                break;
                
            case 23:                /* VIEW DIST                                    */
                contours.view_distance = parmstruc->parmval.dbval;
                break;
                
            case 24:                /* VIEW INCL                                    */
                contours.view_inclination = parmstruc->parmval.dbval;
                break;
                
            case 25:                /* VIEW TYPE                                    */
                contours.view_type = parmstruc->parmval.ival;
                break;

	    case 28:			    /* FAULT SOURCE */
		contours.fault_source = parmstruc->parmval.cval;
		break;

            case 30:
		if (trend_count == 1)
		  {
		    contours.trend_or_resid_order = parmstruc->parmval.ival;
		    contours.grid_type = TREND_TYPE;
		  }
                break;

            case 31:
		if (residual_count == 1)
		  {
		    contours.trend_or_resid_order = parmstruc->parmval.ival;
		    contours.grid_type = RESIDUAL_TYPE;
		  }
                break;
                }
            }
        }

    /* * PRE-PROCESSING: MAKE GRID FILE *                                           */
    /* ----------------------------------                                           */
    
    if (contours.xyz_file != NULL)
        {
        sprintf (msg, mg_message (CO_GRID_FROM_XYZ), contours.xyz_file);
        am_message (AM_DIALOG, msg);
        status = co_make_grid (&contours);
        am_message (AM_DIALOG, NULL);
        if (status < 0)
            {
            goto finished;
            }
        }
    /* * PRE-PROCESSING: MAKE GRID FILE *                                           */
    /* ----------------------------------                                           */
    
    if (contours.grid_file != NULL)
        {
        sprintf (msg, mg_message (CO_GRID_INFO), type_string[contours.view_type],
                 contours.grid_file);
        am_message (AM_DIALOG, msg);
        status = co_make_contours (&contours);
        am_message (AM_DIALOG, NULL);
        if (status < 0)
            {
            goto finished;
            }
        }
    if (contours.contour_file EQUALS NULL)
        {
        status = CO_CONTOUR_FILE_ERROR;
        goto finished;
        }
    /* * EXECUTE THE OVERLAY *                                                      */
    /* -----------------------                                                      */
    /* OPEN THE OVERLAY                                                             */
    
    status = ov_open_overlay (display_line, contours.name);
    if (status < 0)
        {
        goto finished;
        }
    /* PLOT THE OVERLAY                                                             */
    
    sprintf (msg, mg_message (CO_CONTOUR_INFO), type_string[contours.view_type],
             contours.contour_file);
    am_message (AM_DIALOG, msg);
    status = co_post_contours (&contours);
    am_message (AM_DIALOG, NULL);
    
    /* CLOSE THE OVERLAY                                                            */
    
    ov_close_overlay (display_line);
    
    /* * RETURN *                                                                   */
    /* ----------                                                                   */
    
finished:
    
    return status;
    }
/* END:                                                                             */
