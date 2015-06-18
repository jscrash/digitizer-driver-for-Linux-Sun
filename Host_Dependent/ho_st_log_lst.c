/* DEC/CMS REPLACEMENT HISTORY, Element HO_ST_LOG_LST.C*/
/* *4    17-AUG-1990 21:53:38 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    26-FEB-1990 18:59:02 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2     1-NOV-1989 10:12:31 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *1    19-JUN-1989 12:59:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ST_LOG_LST.C*/
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

#include "esi_gl_defs.h"
#include "esi_ho.h"
#include "esi_ts.h"
#include "esi_tc.h"
#include "esi_ho_err.h"

#ifndef vms

#include "esi_lognode.h"
publicdef struct lognode *lnmprocess;   /* ptr to beginning node */

#else

#pragma nostandard

#include "descrip.h"
#include "lnmdef.h"
#include "psldef.h"

#pragma standard
typedef struct item_struct
    {
    USHORT buflen;
    USHORT code;
    BYTE *bufadr;
    USHORT *retlenadr;
    } ITEM_STRUCT;

#endif

/* Function Description -----------------------------------------------------
Description:
    Creates a logical name list as specified in the table of    
    char. pointers.                                              

Prototype:
    publicdef INT ho_set_logical_list(CHAR logical_table[], CHAR logical_name[], 
        TCP tcp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) logical_table   -(CHAR *) The name of logical table.        
    (I) logical_name    -(CHAR *) The name of logical name.          
    (I) tcp             -(TCP) A list of pointers point to FILENAME.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    HO_ERR_VMS
    HO_NULL_LOG_NAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_set_logical_list (CHAR logical_table[], CHAR logical_name[],
                                   TCP tcp) 
#else
publicdef INT ho_set_logical_list (logical_table, logical_name, tcp)
CHAR logical_table[];
CHAR logical_name[];
TCP tcp;
#endif
    {
    INT status;
    INT num_items = 0;
    INT i;
    INT len;

#ifdef vms

#pragma nostandard
    $DESCRIPTOR (logical_table_desc, logical_table);
    $DESCRIPTOR (logical_name_desc, logical_name);
#pragma standard
    INT attribute = 0;
    ITEM_STRUCT *item_list;
    static CHAR default_table[] = "LNM$JOB";
    
    if (logical_name EQUALS NULL OR strlen (logical_name) EQUALS 0)
        {
        return HO_NULL_LOG_NAME;
        }
    logical_name_desc.dsc$w_length = strlen (logical_name);
    len = (logical_table IS_NOT_EQUAL_TO NULL) ? strlen (logical_table) : 0;
    
    if (len EQUALS 0)
        {
        logical_table_desc.dsc$a_pointer = default_table;
        logical_table_desc.dsc$w_length = strlen (default_table);
        }
    else
        {
        logical_table_desc.dsc$w_length = len;
        }
    /* creates the item list.               */
    num_items = ts_tcp_len (tcp);
    
    item_list = (ITEM_STRUCT *)tc_alloc (sizeof(ITEM_STRUCT) * (num_items + 1));
    
    for (i = 0; i < num_items; i++)
        {
        item_list[i].buflen = strlen (tcp[i]);
        item_list[i].bufadr = (BYTE *)tcp[i];
        item_list[i].retlenadr = 0;
        item_list[i].code = LNM$_STRING;
        }
    item_list[i].buflen = 0;
    item_list[i].bufadr = 0;
    item_list[i].retlenadr = 0;
    item_list[i].code = 0;
    
    status = VAX_ROUTINE (LIB$SET_LOGICAL (&logical_name_desc, 0, &logical_table_desc,
                                           &attribute, item_list));
    
    tc_free (item_list);
    if (status & 1)
        return SUCCESS;
    else
        return HO_ERR_VMS;
#else
    struct lognode *tlp;
    int size;
    CHAR *p, **qq, **pp;
    
    tlp = lnmprocess;
    /* get space for node */
    p = tc_zalloc (sizeof(struct lognode) + strlen (logical_name));
    if (!p)
        return FAIL;
    lnmprocess = (struct lognode *)p;
    lnmprocess->next = tlp;         /* make it first in the list */
    strcpy (lnmprocess->log_name, logical_name);
    num_items = ts_tcp_len (tcp);   /* determine size of tcp */
    size = sizeof(CHAR *) * (num_items + 1);
    lnmprocess->tcp = (CHAR **)tc_zalloc (size);    /* make space for our tcp */
    
    for (pp = lnmprocess->tcp; *tcp; ++pp, ++tcp)
        {
        size = strlen (*tcp) + 1;
        *pp = tc_zalloc (size);
        if (!*pp)
            return FAIL;
        strcpy (*pp, *tcp);
        }
    return SUCCESS;

#endif
    }
/* END:     */
