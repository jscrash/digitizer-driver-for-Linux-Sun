/* DEC/CMS REPLACEMENT HISTORY, Element GB_CRE_W_MPLST.C */
/* *2    30-JUL-1990 17:22:06 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:10:59 JULIAN "create well map list" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_CRE_W_MPLST.C */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_RPICK_WELL.C*/
/* *5     5-SEP-1989 15:08:45 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *4    11-AUG-1989 20:41:07 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *3    11-AUG-1989 12:42:44 GORDON "(SPR 105) use sizeof(SELECT_KEY_STRING), not 62"*/
/* *2    18-JUL-1989 14:48:41 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:52:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_RPICK_WELL.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                        Exploration Systems, Inc.
                        579 Market Street
                        San Francisco, CA  USA 94105
                        (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* * FILE DESCRIPTION *                                                                                                 */
/* --------------------                                                                                                 */
/* ************************************************************************

   NAME: GB_RPICK_WELL.PC

   AUTHOR: JOHN CONROY

   DESCRIPTION: Function to locate and return the name of a list of wells
                which fall within the supplied rectangle.  These may be
                only the visible wells or all wells currently on the map
                depending on the value of the supplied boolean parameter.

   ************************************************************************ */

/* * INCLUDE FILES *                                                                                                    */
/* -----------------                                                                                                    */

#include "esi_gb.h"
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif


/* * FUNCTION TITLE *                                                                                                   */
/* ------------------                                                                                                   */


publicdef INT gb_create_well_on_map_list (map,select_list, full)
/* * ARGUMENT LIST *                                                                                                    */
/* ------------------                                                                                                   */
MAP_STRUCTURE *map;
NLIST_HEADER *select_list;  /* Pointer to created select list */
BOOL full;
{
    DOUBLE xmin, xmax, ymin, ymax;

    xmin = map->lower_left_xy.x;
    xmax = map->upper_right_xy.x;
    ymin = map->lower_left_xy.y;
    ymax = map->upper_right_xy.y;

    return (gb_rect_pick_well (xmin, ymin, xmax, ymax,select_list, full));

}
