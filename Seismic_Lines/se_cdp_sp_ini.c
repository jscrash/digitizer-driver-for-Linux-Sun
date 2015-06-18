/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_SP_INI.C */
/* *3    17-AUG-1990 22:15:56 VINCE "(SPR 5644) Code Cleanup" */
/* *2    18-JUN-1990 12:07:57 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *1     5-MAR-1990 17:06:32 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_SP_INI.C */
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

#include "esi_se.h"

/* Function Description -----------------------------------------------------
Description:
    This routine is used to create a
    correspondance cdp / sp if there is already
    a correspondance existing we use this
    correspondance if not this routine returns
    the correspondance and the number of cpd
    be done out of the scope of this routine
    per sp to use if some correspondance has to.
      
    Honor = 1 if we honor the correspondance
    already existing between cdp and sp = 0
    means that we reset the correspondance.

Prototype:
    publicdef INT se_cdp_from_sp_init (FULL_LINE_NAME *seis_line, INT honor,
        SP sp, CDP *cdp, CDP *cdp_per_sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    honor           -(INT)
    sp              -(SP)
    cdp             -(CDP *)
    cdp_per_sp      -(CDP *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_cdp_from_sp_init (FULL_LINE_NAME *seis_line, INT honor, SP sp,
                                   CDP *cdp, CDP *cdp_per_sp)
#else
publicdef INT se_cdp_from_sp_init (seis_line, honor, sp, cdp, cdp_per_sp)
FULL_LINE_NAME *seis_line;
INT honor;
SP sp;
CDP *cdp;
CDP *cdp_per_sp;
#endif
    {
    INT status;
    LINE_HDR *p_hdr;
    
    /* look for the pointer to the structure       */
    /* describing the header                       */
    
    status = se_retrieve_header (seis_line, &p_hdr);
    
    /* if we don't find the information in the
       SEIS_LINE_HDR table this means that this is
       the first time we establish a
       correspondance between cdp and sp so we
       just fill the structure with defaults which
       enable to establish the correspondance if
       HONOR is false we restablish the
       correspondance                              */
    
    if (status OR!honor)
        {
        /* Establish a non-zero dummy line ID.  This
           is used by the UKOOA loader while it is
          loading a new line and before it has
           written anything into the database.  Since
           this value is passed back to the called
           routine, it should be used for all
           subsequent calls to the se... routines in
           this unit                                   */
        
        seis_line->id = -1;
        p_hdr->line = *seis_line;
        
        p_hdr->cdp_per_sp = DEF_CDP_PER_SP;
        p_hdr->init_cdp = DEF_INIT_CDP;
        p_hdr->init_sp = sp;
        
        /* nlist pointers are set to NULL because
           there are no nlist already created for this
           line                                        */
        
        p_hdr->regular = REGULAR;
        p_hdr->nb_points = 0;
        p_hdr->shot_label = NULL;
        p_hdr->array_cdp = (FLOAT *)NULL;
        p_hdr->array_x = (FLOAT *)NULL;
        p_hdr->array_y = (FLOAT *)NULL;
        }
    se_cdp_from_sp (seis_line, sp, cdp);
    *cdp_per_sp = p_hdr->cdp_per_sp;
    
    return SUCCESS;
    }
/* END:     */
