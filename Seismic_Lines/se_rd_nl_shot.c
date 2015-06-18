/* DEC/CMS REPLACEMENT HISTORY, Element SE_RD_NL_SHOT.C */
/* *5    17-AUG-1990 22:17:40 VINCE "(SPR 5644) Code Cleanup" */
/* *4    29-JUN-1990 19:03:54 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush" */
/* *3     2-NOV-1989 13:57:20 JULIAN "(SPR 30) completed last mod" */
/* *2     1-NOV-1989 14:07:55 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:49:05 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_RD_NL_SHOT.C */
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

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#if USE_PROTOTYPES
static INT sez_read_shot_xy_nl (LINE_HDR *p_hdr, NLIST_HEADER *nlist);
#else
static INT sez_read_shot_xy_nl ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT se_rd_nl_shot_xy (FULL_LINE_NAME *seis_line, NLIST_HEADER *nlist);

Private_Functions:
    static INT sez_read_shot_xy_nl (LINE_HDR *p_hdr, NLIST_HEADER *nlist);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This function is used to read the nlist
    shot_xy corresponding to the given line and
    returns a pointer to he nlist.

Prototype:
    publicdef INT se_rd_nl_shot_xy (FULL_LINE_NAME *seis_line, NLIST_HEADER *nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *) Seis_line is the id structure for the line.
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns n-List status  if the nlist cannot be read
    or is not existing.                          

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_rd_nl_shot_xy (FULL_LINE_NAME *seis_line, NLIST_HEADER *nlist)
#else
publicdef INT se_rd_nl_shot_xy (seis_line, nlist)
FULL_LINE_NAME *seis_line;
NLIST_HEADER *nlist;
#endif
    {
    
    INT status;
    LINE_HDR *p_hdr;
    
    /* retrieve the LINE_HDR corresponding to this
       line                                        */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    
    if (status EQUALS SUCCESS)
        {
        status = sez_read_shot_xy_nl (p_hdr, nlist);
        }
    return status;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function is used to read the nlist
    shot_xy corresponding to the given line and
    returns a pointer to the nlist.

Prototype:
    publicdef INT func (arg);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    p_hdr           -(LINE_HDR *)
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - If the nlist cannot be read or is not existing.

Scope:
    PRIVATE to <se_rd_nl_shot.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sez_read_shot_xy_nl (LINE_HDR *p_hdr, NLIST_HEADER *nlist)
#else
static INT sez_read_shot_xy_nl (p_hdr, nlist)
LINE_HDR *p_hdr;
NLIST_HEADER *nlist;
#endif
    {
    INT status = FAIL;
    FILENAME nlist_file;
    FILENAME full_filename;
    
    /* first we test that the nlist name is not
       null if it is null we don't need to try to
       read the nlist we return FAIL               */
    
    if (IS_STRING (p_hdr->shot_xy_id))
        {
        
        /* the nlist file name is taken in the
           structure and se_bld_nlist_ids is used to
           prepend the logical used for seismic nlist  */
        
        strcpy (nlist_file, p_hdr->nlist_file);
        
        status = se_bld_nlist_ids (&p_hdr->line, (VOIDPTR)0, SEIS_LOCATION_DATA,
                                   p_hdr->shot_xy_id, nlist_file, full_filename);
        
        if (status EQUALS SUCCESS)
            {
            status = nl_read_nlist (nlist, full_filename, p_hdr->shot_xy_id);
            }
        }
    return status;
    
    }
/* END:     */
