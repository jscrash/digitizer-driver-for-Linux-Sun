/* DEC/CMS REPLACEMENT HISTORY, Element XS_WELL.C*/
/*  11    5-MAR-1992 13:18:05 JESSIE "(SPR 3942) XS show well list not in synch with traverse"*/
/* *10   14-JUN-1991 08:27:19 CHARLIE "(SPR 7807) add informative message, we hope"*/
/* *9    17-AUG-1990 22:46:25 VINCE "(SPR 5644) Code Cleanup"*/
/* *8    14-APR-1990 15:09:40 SCD "(SPR 1) Fix memory overwrite in too short declaration of CHAR temp[]."*/
/* *7    13-MAR-1990 16:21:30 WALTERS "(SPR 0) see below"*/
/* *6    12-MAR-1990 15:01:59 WALTERS "(SPR 0) Add to last change: Show well name, well short name, uwi in that order"*/
/* *5     6-MAR-1990 16:21:57 WALTERS "(SPR 0) Show well name instead of UWI"*/
/* *4    21-NOV-1989 09:51:11 GORDON "(SPR 00) Fix case in esi_we_msg.h line"*/
/* *3     1-NOV-1989 10:53:59 GILLESPIE "(SPR 30) Fix castings - remove hard-coded message text"*/
/* *2    19-SEP-1989 11:10:14 GORDON "(SPR 100) make xs_we_use_cur_well a no-op, as it's not called"*/
/* *1    19-JUN-1989 13:46:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_WELL.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_AS_SAVE_H

#include "esi_as_save.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_WE_MSG_H

#include "esi_we_msg.h"

#endif

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

/* File Description ---------------------------------------------------------
Overview:
    A module of routines to perform the operations on WELLS in the 
    XS_SECTION package.  Includes using the Current Global Well, Current
    Global List to be the Well list, Selects one of existing Well List to
    be the executed traverse, saves the execute traverse, shows the 
    information of executed Well List/traverse.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_we_dgz_traverse(XS_STRUCT *pxs);
    
    publicdef INT xs_we_rtr_traverse(XS_STRUCT *pxs);
    
    publicdef INT xs_we_use_cur_we_list(XS_STRUCT *pxs);
    
    publicdef INT xs_we_save_traverse(XS_STRUCT *pxs);
    
    publicdef INT xs_we_show_well_list(XS_STRUCT *pxs);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    To digitize wells to build up the traverse for operation.
    
Prototype:
    publicdef INT xs_we_dgz_traverse(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_we_dgz_traverse (XS_STRUCT *pxs)
#else
publicdef INT xs_we_dgz_traverse (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    size_t nbytes = 0;
    NLIST_HEADER new_list;
    SELECT_STRUCT temp_struct;
    MAP_STRUCTURE *mapdef;
    
    /* if there is no open current map,
       allow user to select a map.       */      
    status = mp_front_map (&mapdef);
    if (status != SUCCESS)
        {
        am_message (AM_ALERT, mg_message (XS_NO_MAP));
        return status;
        }
    status = gb_well_picking (&new_list);
    if (status < 0)
        {
        mp_set_current_map ((MAP_STRUCTURE *)0);
        return status;
        }
    status = nl_inq_nlist_user_struct (new_list, sizeof(SELECT_STRUCT), &temp_struct,
                                       &nbytes);
    if (status < 0)
        return status;
    
    temp_struct.connected_flag = TRUE;
    
    status = nl_set_nlist_user_struct (new_list, &temp_struct, nbytes);
    if (status < 0)
        return status;
    
    status = nl_free_nlist (pxs->well_list);
    status = nl_copy_nlist (new_list, &(pxs->well_list));
    status = nl_free_nlist (new_list);
    status = gb_hilite_temp (WELLS_DATA_TYPE, pxs->well_list);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To retrieve an existing Well List to be the current executed Well
    list.

Prototype:
    publicdef INT xs_we_rtr_traverse(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_we_rtr_traverse (XS_STRUCT *pxs)
#else
publicdef INT xs_we_rtr_traverse (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    NLIST_HEADER temp_well_list;
    
    /* retrieve the select list.    */
    /* If the returned status is not*/
    /* equals to SUCCESS, maybe is  */
    /* retrieves failed or user cancels*/
    /* the operation, returns.      */
    
    status = sl_choose_list (WELLS_DATA_TYPE, &temp_well_list);
    if (status)
        return status;
    
    /* otherwise frees the previous */
    /* well_list, and set the retrieved*/
    /* old to be the WELL_LIST.     */
    
    status = nl_free_nlist (pxs->well_list);
    status = nl_copy_nlist (temp_well_list, &(pxs->well_list));
    if (status)
        return status;
    nl_free_nlist (temp_well_list);
    pxs->changed_since_display = TRUE;
    xs_cs_menu_mgr (pxs);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To use the Current Global Well List to be the well List for operation.

Prototype:
    publicdef INT xs_we_use_cur_we_list(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_we_use_cur_we_list (XS_STRUCT *pxs)
#else
publicdef INT xs_we_use_cur_we_list (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    CHAR current_well_list[STMAX];
    
    /* inquire the current well list, if */
    /* there is no current well list,    */
    /* returns.              */
    
    qs_inq_c (QS_CUR_WELL_LIST, current_well_list, (INT *)0);
    if (strlen (current_well_list) <= 0)
        {
        am_message (AM_ALERT, mg_message (WE_EMPTY_LIST));
        return status;
        }
    /* free the existing one, if there is one*/
    status = nl_free_nlist (pxs->well_list);
    
    /* read the select list.     */
    
    status = sl_read_list (WELLS_DATA_TYPE, current_well_list, &(pxs->well_list));
    status = (status >= 0) ? SUCCESS : status;
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To save the executed traverse to be a Select List.

Prototype:
    publicdef INT xs_we_save_traverse(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_we_save_traverse (XS_STRUCT *pxs)
#else
publicdef INT xs_we_save_traverse (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    size_t nbytes = 0;
    INT num_wells = 0;
    INT class , id, item, value;
    INT overwrite = TRUE;
    AS_SAVE_WORKSPACE p;
    SELECT_STRUCT temp_struct;
    
    /* inquire the name used to save.  */
    
    tc_zeroes (&p, sizeof(AS_SAVE_WORKSPACE));
    p.cancel = TRUE;
    
    /* error checking....              */
    /* If there is no well in the current*/
    /* traverse.               */
    
    status = nl_inq_nlist_int (pxs->well_list, NL_NLIST_NPOINTS, &num_wells);
    if (status < SUCCESS OR num_wells <= 0)
        {
        am_message (AM_ALERT, mg_message (XS_NO_WELL_IN_TRAVERSE));
        return status;
        }
    strcpy (p.message, mg_message (XS_ENTER_TRAVERSE_NAME));
    asp_save (&p);
    if (p.status)
        return p.status;
    
    /* save the select list as the     */
    /* requested name.         */
    status = nl_inq_nlist_user_struct (pxs->well_list, sizeof(SELECT_STRUCT),
                                       (VOIDPTR) & temp_struct, &nbytes);
    if (status)
        goto rtnpt;
    
    strcpy (temp_struct.name, p.filename);
    temp_struct.permanent_flag = TRUE;
    status = nl_set_nlist_user_struct (pxs->well_list, &temp_struct,
                                       sizeof(SELECT_STRUCT));
    if (status)
        goto rtnpt;
    
    status = sl_save_list (pxs->well_list, overwrite);
    
rtnpt:
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To show the information of wells in the current excuted well list.

Prototype:
    publicdef INT xs_we_show_well_list(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_we_show_well_list (XS_STRUCT *pxs)
#else
publicdef INT xs_we_show_well_list (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;
    INT num_items, i;
    TCP tcp_uwi;
    TCP tcp_name;
    CHAR temp[STMAX];
    WELL_STRUCT *ww_hdr;
    WELL_HDR_1 *ww_hdr_1;
    
    /* makes a TCP for well List.  */
    
    sl_make_tcp (&tcp_uwi, pxs->well_list, 1);
    
    /* make a TCP for well names   */
    num_items = ts_tcp_len (tcp_uwi);
    tcp_name = ts_tcp_alloc (num_items);
    
    for (i = 0; i < num_items; i++)
        {
        we_get_well_header (tcp_uwi[i], WELL_HDR_1_DETAIL, &ww_hdr);
        ww_hdr_1 = (WELL_HDR_1 *)ww_hdr->detail.ptr;
        
        if (ARE_DIFFERENT (ww_hdr_1->short_name, ""))
            strcpy (temp, ww_hdr_1->short_name);
        else if (ARE_DIFFERENT (ww_hdr_1->name, ""))
            strcpy (temp, ww_hdr_1->name);
        else
            strcpy (temp, tcp_uwi[i]);
        
        if (ARE_DIFFERENT (ww_hdr_1->number, ""))
            sprintf (temp, "%s #%s", temp, ww_hdr_1->number);
        
        ts_tcp_append (&tcp_name, temp);
        }
    /* show wells info in the TCP. */
    
    status = we_show_wells_server (tcp_uwi, tcp_name);
    
    return status;
    }
/* END:     */
