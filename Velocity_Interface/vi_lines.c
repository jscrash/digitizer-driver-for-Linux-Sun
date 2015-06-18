/* DEC/CMS REPLACEMENT HISTORY, Element VI_LINES.C */
/* *2    12-JUN-1991 20:19:42 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:11:59 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_LINES.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1991
                   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                        Finder Graphics Systems, Inc.
                        201 Tamal Vista Blvd
                        Corte Madera, CA  USA 94925
                        (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/
/* Function Description -------------------------------------------------------
Description:
    Starting with seismic line select list name, return integer array
    of seismic line ids.

Prototype:
    publicdef INT vi_lines (PROJECT_NAME project, NLIST_ID list_name,
                            INT **lines, INT *lineknt)

Parameters:
    (I) project     -(PROJECT_NAME) Name of project containing data.
    (I) list_name   -(NLIST_ID) Seismic select list name containing
                                names of lines to be used.
    (O) lines       -(INT **)   Unique lineids corresponding to list.
                                Memory will be allocated here.  The
                                calling routine is responsible for
                                freeing memory via 'tc_free' upon
                                program completion.
    (O) lineknt     -(INT *)    Length of 'lines'.

Return Value/Status:
    SUCCESS.
    FAIL code.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#include "esi_vi.h"
#include "esi_sl.h"
#include "esi_nl.h"
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#if USE_PROTOTYPES
publicdef INT vi_lines (PROJECT_NAME project, NLIST_ID list_name,
                        INT **lines, INT *lineknt)
#else
publicdef INT vi_lines (project, list_name, lines, lineknt)
PROJECT_NAME project;
NLIST_ID     list_name;
INT        **lines;
INT         *lineknt;
#endif
{
    INT status;
    NLIST_HEADER select;
    INT nrows;
    CHAR **ptcp;
    INT i;
    INT *out;
    INT knt = 0;
    UINT nallocated;
    UINT ncols = 1;
    UINT Col_List[1];
    VOIDPTR Var_List[1];
    
    status = SUCCESS;
    *lineknt = 0;
    
    /* Get nlist select list from database */
    /* ----------------------------------- */
    status = sl_read_list (SEISMIC_DATA_TYPE, list_name, &select);
    if(status != SUCCESS)
        {
        return status;
        }
    
    /* Get list size */
    /* ------------- */
    status = nl_inq_nlist_int(select, NL_NLIST_NROWS, &nrows);
    if(status < SUCCESS || nrows EQUALS 0)
        {
        nl_free_nlist(select);
        return FAIL;
        }
    
    /* Set 1st row,table */
    /* ----------------- */
    nl_set_current_row(select, 1, 1);
    
    /* Allocate return array space */
    /* --------------------------- */
    *lines = (INT *)tc_alloc(nrows * sizeof(INT));
    out = *lines;
    
    /* Get line ids */
    /* ------------ */
    nallocated = nrows;
    Col_List[0] = 1;
    Var_List[0] = (VOIDPTR)out;
    status = nl_get_rows(select, nallocated, ncols, Col_List, Var_List,
                         lineknt);
    if(status > SUCCESS)
        {
        status = SUCCESS;
        }
    
    nl_free_nlist(select);
    return status;

} /* end of vi_lines */
