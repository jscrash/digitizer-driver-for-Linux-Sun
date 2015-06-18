/* DEC/CMS REPLACEMENT HISTORY, Element S3_PROC_OV.C*/
/* *4    15-OCT-1990 12:55:30 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  2B1  12-OCT-1990 17:49:25 GILLESPIE "Merge Ernie Deltas"*/
/* *3    17-AUG-1990 22:14:04 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JUN-1990 12:07:01 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)"*/
/* *1    19-JUN-1989 13:23:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element S3_PROC_OV.C*/
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

#include "esi_s3.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_OV_H

#include "esi_ov.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Responsible for determining if a select list or                     
    select by phrase exists (select lists are dubious right now),                   
    setting the current survey (if more than one), and looping until                 
    done.                                                                            

Prototype:
    publicdef INT s3_process_overlay(S3_STRUCT *ps3);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ps3             -(S3_STRUCT *) 3d seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT s3_process_overlay (S3_STRUCT *ps3)
#else
publicdef INT s3_process_overlay (ps3)
S3_STRUCT *ps3;
#endif
    {
    
    INT status;
    NLIST_HEADER slist;             /* temporary select list pointer                */
    NLIST_HEADER process_list;      /* final list to process                        */
    INT num_lines;                  /* number of lines to process                   */
    INT iline;                      /* iline in for loop                            */
    INT blocksize = 20;
    
    status = SUCCESS;
    slist = (NLIST_HEADER)0;
    
    /* open scatter file if necessary                                               */

#if 0
    /* temporarily eliminated until conversion to new file manager routines         */
    
    if (IS_STRING (ps3->seismic_generic.xyzfile))
        {
        if (ps3->seismic_generic.xyzfile_append)
            {
            status = ov_open_scatter (&ps3->seismic_generic.scatter,
                                      ps3->seismic_generic.xyzfile, "a");
            }
        else
            {
            status = ov_open_scatter (&ps3->seismic_generic.scatter,
                                      ps3->seismic_generic.xyzfile, "w");
            }
        if (status != SUCCESS)
            {
            return status;
            }
        else
            {
            ps3->seismic_generic.do_scatter = TRUE;
            }
        /* end status if                                                            */
        }
    /* end strlen if                                                                */
#endif
    
    /* if select list present, read it                                              */
    
    if (IS_STRING (ps3->generic.select_list))
        {
        status = sl_read_list (SEISMIC_DATA_TYPE, ps3->generic.select_list,
                               &ps3->select_list);
        
        /* check read status, save slist pointer to pass with sql statement         */
        
        if (status == SUCCESS)
            {
            slist = ps3->select_list;
            process_list = slist;
            }
        else
            {
            slist = (NLIST_HEADER)0;
            }
        }
    /* end strlen if                                                                */
    /* check select phrase                                                          */
    
    if (slist == (NLIST_HEADER)0 OR (slist !=
                                    (NLIST_HEADER)0 AND ps3->generic.select_by_phrase !=
                                    (CHAR *)0) OR (slist !=
                                                  (NLIST_HEADER)0 AND 
                                                  IS_STRING (ps3->generic.
                                                             select_by_phrase)))
        {
        
        /* get new select list                                                      */
        
        status = sl_select (slist, "SEIS_LINE_HDR", ps3->generic.select_by_phrase,
                            &process_list, &num_lines);
        }
    /*                                                                              */
    /* This code assumes that we are plotting all of the
lines in a given 3-D survey...
*/
    
    ss_set_cur_3d (ps3->s3_current_survey);
    
    ss_get_surv_3d (&ps3->grid_3d.lower_cdp, &ps3->grid_3d.upper_cdp,
                    &ps3->grid_3d.lower_line, &ps3->grid_3d.upper_line,
                    &ps3->grid_3d.x_azimuth, &ps3->grid_3d.y_azimuth,
                    &ps3->grid_3d.cdp_interval, &ps3->grid_3d.line_interval,
                    &ps3->grid_3d.x_origin, &ps3->grid_3d.y_origin,
                    ps3->grid_3d.xy_unit, &ps3->grid_3d.cdp_inc,
                    &ps3->grid_3d.line_inc);
    
    /* if bad status, no good data to work with                                     */
    
    if (status EQUALS SUCCESS)
        {
        status = s3_process_survey (ps3);
        
        /* close scatter file                                                       */

#if 0
        /* needs to conform to file manager                                         */
        
        if (ps3->seismic_generic.do_scatter)
            {
            ov_close_scatter (ps3->seismic_generic.scatter);
            }
#endif
        status = SUCCESS;
        }
    if (slist != (NLIST_HEADER)0 AND slist != ps3->select_list)
        {
        nl_free_nlist (slist);
        }
    return status;
    }
/* END:     */
