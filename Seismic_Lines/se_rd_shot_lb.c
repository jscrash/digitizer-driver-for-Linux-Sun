/* DEC/CMS REPLACEMENT HISTORY, Element SE_RD_SHOT_LB.C */
/* *3    17-AUG-1990 22:17:45 VINCE "(SPR 5644) Code Cleanup" */
/* *2    29-JUN-1990 19:04:07 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush" */
/* *1     5-MAR-1990 17:00:54 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_RD_SHOT_LB.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"
#include "esi_se.h"

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This function reads the nlist shot_labels in memory.

Prototype:
    publicdef INT se_read_shot_lab (LINE_HDR *p_hdr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_hdr           -(LINE_HDR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_read_shot_lab (LINE_HDR *p_hdr)
#else
publicdef INT se_read_shot_lab (p_hdr)
LINE_HDR *p_hdr;
#endif
    {
    INT status;
    FILENAME nlist_file;
    FILENAME full_filename;
    NLIST_HEADER nlist;
    
    /* first we test that the nlist name is not
       null if it is null we don't need to try to
       read the nlist we return FAIL               */
    
    if (IS_STRING (p_hdr->shot_label_id))
        {
        
        /* the nlist file name is taken in the
           structure and se_bld_nlist_ids is used to
           prepend the logical used for seismic nlist  */
        
        strcpy (nlist_file, p_hdr->nlist_file);
        
        status = se_bld_nlist_ids (&p_hdr->line, (VOIDPTR)0, SEIS_SHOT_LABEL_DATA,
                                   p_hdr->shot_label_id, nlist_file, full_filename);
        
        /* read the nlist of shot labels               */
        
        if (status EQUALS SUCCESS)
            {
            status = nl_read_nlist (&p_hdr->shot_label, full_filename,
                                    p_hdr->shot_label_id);
            }
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            /* impossible to read the nlist and compute
               the correspondance sp from cdp              */
            
            return status;
            }
        }
    else                            /* no nlist name for shot labels               */
        {
        return FAIL;
        }
    /* set current point to first point in the     */
    /* nlist                                       */
    
    status = nl_set_current_row (p_hdr->shot_label, 1, 1);
    
    return SUCCESS;
    }
/* END:     */
