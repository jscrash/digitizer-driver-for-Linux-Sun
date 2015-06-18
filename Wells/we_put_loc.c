/* DEC/CMS REPLACEMENT HISTORY, Element WE_PUT_LOC.C*/
/* *7    14-AUG-1990 13:39:25 VINCE "(SPR 5644) Header Standardization"*/
/* *6    14-FEB-1990 11:06:36 GORDON "(SPR 0) set the symbol code"*/
/* *5     9-FEB-1990 16:13:34 MING "(SPR 6007) change node id declaration to NODE_ID"*/
/* *4    17-JAN-1990 22:56:16 GILLESPIE "(SPR 6001) Changes for new well struct mods"*/
/* *3    27-NOV-1989 11:16:33 CONROY "(SPR 5063) Create short loc only struct"*/
/* *2    14-SEP-1989 16:21:25 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:35:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WE_PUT_LOC.C*/
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

#include "esi_c_lib.h"

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

#ifndef ESI_HT_H

#include "esi_ht.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to put the minimum set of information
    into the well_struct cache system.
    
    The top and bottom locations must be passed in already 
    in the project projection coordinates.

Prototype:
    publicdef INT we_put_location(UWI uwi,DOUBLE top_x,DOUBLE top_y,DOUBLE bot_x,
        DOUBLE bot_y,NODE_ID node_id,INT dev_flag,INT symbol);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uiw             -(UWI)
    top_x           -(DOUBLE)
    top_y           -(DOUBLE)
    bot_x           -(DOUBLE)
    bot_y           -(DOUBLE)
    node_id         -(NODE_ID)
    dev_flag        -(INT)
    symbol          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT we_put_location (UWI uwi, DOUBLE top_x, DOUBLE top_y, DOUBLE bot_x,
                               DOUBLE bot_y, NODE_ID node_id, INT dev_flag,
                               INT symbol) 
#else
publicdef INT we_put_location (uwi, top_x, top_y, bot_x, bot_y, node_id, dev_flag,
                               symbol)
UWI uwi;
DOUBLE top_x;
DOUBLE top_y;
DOUBLE bot_x;
DOUBLE bot_y;
NODE_ID node_id;
INT dev_flag;
INT symbol;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT status;
    WELL_STRUCT *pwell;
    HASH_TABLE_P SYSTEM_ANCHOR_POINT ();
    HASH_TABLE_P pwell_chain = SYSTEM_ANCHOR_POINT (1);
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * ONLY DO SOMETHING IF RECORD DOES NOT ALREADY EXIST * */
    /* ------------------------------------------------------ */
    
    if (ht_find (pwell_chain, (VOIDPTR)uwi, (VOIDPTR) & pwell) != SUCCESS)
        {
        pwell = ALLOCATE (WELL_STRUCT);
        
        strcpy (pwell->uwi, uwi);
        pwell->tophole_xy.node_id = node_id;
        pwell->bottomhole_xy.node_id = node_id;
        pwell->tophole_xy.x = top_x;
        pwell->tophole_xy.y = top_y;
        pwell->bottomhole_xy.x = bot_x;
        pwell->bottomhole_xy.y = bot_y;
        pwell->deviation.flag = dev_flag;
        pwell->symbol_code = symbol;
        
        /* If desired, the latitude and longitude of the tophole and bottomhole
            could be calculated and inserted into the structure as part of 
            the minimum set */
        
        ht_insert (pwell_chain, (VOIDPTR)pwell->uwi, (VOIDPTR) & pwell);
        }
    return status;
    }
/* END:     */
