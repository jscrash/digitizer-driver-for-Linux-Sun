/* DEC/CMS REPLACEMENT HISTORY, Element SO_BLD_COM_NL.C*/
/* *9     7-OCT-1991 15:21:35 JULIAN "(SPR 4156) added check to ignore invisible lines"*/
/* *8    24-SEP-1990 17:19:07 GILLESPIE "Beta Deltas"*/
/* *7    26-JUL-1990 17:18:18 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *6    25-JUL-1990 20:02:22 VINCE "(SPR 5644) Header Standardization"*/
/* *5     2-NOV-1989 09:53:37 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *4    24-OCT-1989 09:39:05 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *3    11-AUG-1989 20:20:41 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *2    25-JUL-1989 20:04:27 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:25:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_BLD_COM_NL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_BLD_COM_NL.C*/
/* *7    26-JUL-1990 17:18:18 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *6    25-JUL-1990 20:02:22 VINCE "(SPR 5644) Header Standardization"*/
/* *5     2-NOV-1989 09:53:37 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *4    24-OCT-1989 09:39:05 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *3    11-AUG-1989 20:20:41 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *2    25-JUL-1989 20:04:27 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:25:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_BLD_COM_NL.C*/
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

#include "esi_sx.h"

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_MD_DEFS_H

#include "esi_md_defs.h"

#endif

#ifndef ESI_MD_DBDEFS_H

#include "esi_md_dbdefs.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif
/* Function Description -----------------------------------------------------
Description:
    Create nlist with all selectable seismic lines.

Prototype:
    publicdef INT  so_build_common_nlist(SO_STRUCT *pso,NLIST_HEADER *nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)
    nlist           -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_build_common_nlist (SO_STRUCT *pso, NLIST_HEADER *nlist)
#else
publicdef INT so_build_common_nlist (pso, nlist)
SO_STRUCT *pso;
NLIST_HEADER *nlist;
#endif
    {
    INT status;                     /* status */
    INT i, j;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    INT row, lines, valid_lines;
    UINT total_rows, table_used;
    BOOL cloned = FALSE;
    INT verb_ndx;
    PICKABLE_SEISMIC **pick_seis;
    SX_USER_STRUCT user_struct;
    GKS_INT errind,vis,high,det;
    GKS_REAL sgpr, segtm[2][3];
    
    /* assume SUCCESS */
    
    status = SUCCESS;
    verb_ndx = -1;
    display = pmap->display;
    compiled = pmap->compiled;
    
    for (i = 0; i < display->last_row_used; i++)
        {
        if (verb_ndx == display->verb_ndx[i])
            continue;
        verb_ndx = display->verb_ndx[i];
        if (verb_ndx == -1)
            continue;
    /* --------------------------------- */
    /* Index only the "seismic" overlays */
    /* --------------------------------- */
        if (!mp_is_seismic_overlay
                (compiled->verb_code[display->compiled_row[i]]) )
        	{
                continue;
        	}
        row = display->compiled_row[i];
        pick_seis = (PICKABLE_SEISMIC **)compiled->picks[row];

        if(pick_seis EQUALS (PICKABLE_SEISMIC **)0)
	  continue;

	/* check whether the pickable seismic is visible or not (turn off using
	   window control (case 4156) */
	wm_gqsga(*pick_seis[0]->segment_id,
		 &errind,
		 segtm,
		 &vis,
		 &high,
		 &sgpr,
		 &det);

	if(vis EQUALS GINVIS) 
	  continue;

        lines = pick_seis[0]->nused;
        
        /* read the seismic lines into the common static nlist */
        for (j = 0, valid_lines = 0; j < lines; j++)
            {
            status = se_rd_nl_shot_xy (&(pick_seis[0]->line_name[j]), &pso->cdp_xy);
            /* if nlist not cloned, do so */
            if (status == SUCCESS)
                {
                status = nl_inq_nlist_int (pso->cdp_xy, NL_NLIST_NROWS,
                                           (INT *) & total_rows);
                if ((status == SUCCESS) AND (total_rows > 0))
                    {
                    if (NOT cloned)
                        {
                        status = nl_clone_nlist (pso->cdp_xy, nlist);
                        cloned = TRUE;
                        }
                    }
                else
                    continue;
                /* copy seismic data from each table to a table in the common nlist */
                valid_lines++;
                status = nl_copy_table (pso->cdp_xy, 1, *nlist, &table_used);
                status = nl_free_nlist (pso->cdp_xy);
                user_struct.segment_id = pick_seis[0]->segment_id[j];
                user_struct.line_name = pick_seis[0]->line_name[j];
                status = nl_set_table_user_struct (*nlist, table_used,
                                                    (VOIDPTR) & user_struct,
                                                    sizeof(SX_USER_STRUCT));
                }
            }
        }
    SX_RETURN;
    }
/* END:     */
