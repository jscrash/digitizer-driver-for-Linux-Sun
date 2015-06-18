/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MN_STAT.C*/
/*  *5     4-JAN-1991 10:45:31 VINCE "(SPR 6373) changed the init of the statics to ON to match the real state of the menus"*/
/*  *4    15-OCT-1990 13:44:20 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*   2B1  12-OCT-1990 17:20:25 GILLESPIE "Merge Ernie Deltas"*/
/*  *3    17-AUG-1990 22:07:13 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:57:28 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:22 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MN_STAT.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
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

#include    "esi_mp.h"
#include    "esi_wi.h"
#include    "esi_am.h"
#include    "mp_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Updates menu status.

Prototype:
    publicdef INT mp_menu_status();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.

Return Value/Status:
    SUCCESS - Successful completion.

Scope:
    PUBLIC

Limitations/Assumptions:

----------------------------------------------------------------------------- */

/* BEGIN: */
#if USE_PROTOTYPES
publicdef INT mp_menu_status ()
#else
publicdef INT mp_menu_status ()
#endif
    {
    INT i, j, k;
    INT status;
    MENU_ID *menu_id;

    /* -------------------------------------------------------------------- */
    /* since a menu is fully ON when it is first defined, initialize these  */
    /* statics to ON.						    */
    /* -------------------------------------------------------------------- */

    static BOOL old_menu_status[5] = 
        {
	ON, ON, ON, ON, ON
	};

    static BOOL old_menu_item_status[5][7] = 
        {
            { ON, ON, ON, ON, ON, ON, ON },
            { ON, ON, ON, ON, ON, ON, ON },
            { ON, ON, ON, ON, ON, ON, ON },
            { ON, ON, ON, ON, ON, ON, ON },
            { ON, ON, ON, ON, ON, ON, ON }
        };
    
    status = am_open_workspace ("MENU", AM_APPLICATION, (VOIDPTR *)&menu_id);
    if (status != SUCCESS)
        {
        am_message (AM_ALERT, "Trouble opening the menu workspace");
        return FAIL;
        }

    /*  Adjust menu visibility  */
    
    for (i = 0; i < 5; i++)
        {
        switch (i)
            {
        case 0:	/* ----- Mapping ----- */
            if (menu_id->mapping_id == 0)
                break;

            if (menu_id->menu_status[i] != old_menu_status[i])
                {
                wi_enable_menu (menu_id->mapping_id, menu_id->menu_status[i]);
                old_menu_status[i] = menu_id->menu_status[i];
                }

            for (j = 0; j < 6; j++)
                {
                if (old_menu_item_status[i][j] != 
		    menu_id->menu_item_status[i][j])
                    {
                    wi_enable_menu_items (menu_id->mapping_id, 6,
                                          menu_id->menu_item_status[i]);
                    for (k = 0; k < 6; k++)
                        old_menu_item_status[i][k] =
			    menu_id->menu_item_status[i][k];
                    break;
                    }
                }
            break;

        case 1:	/* ----- Make Map ----- */
            if (menu_id->make_map_id == 0)
                break;

            if (menu_id->menu_status[i] != old_menu_status[i])
                {
                wi_enable_menu (menu_id->make_map_id, menu_id->menu_status[i]);
                old_menu_status[i] = menu_id->menu_status[i];
                }

            for (j = 0; j < 7; j++)
                {
                if (old_menu_item_status[i][j] !=
		    menu_id->menu_item_status[i][j])
                    {
                    wi_enable_menu_items (menu_id->make_map_id, 7,
                                          menu_id->menu_item_status[i]);
                    for (k = 0; k < 7; k++)
                        old_menu_item_status[i][k] =
			    menu_id->menu_item_status[i][k];
                    break;
                    }
                }
            break;

        case 2:	/* ----- Options ----- */
            if (menu_id->display_options_id == 0)
                break;

            if (menu_id->menu_status[i] != old_menu_status[i])
                {
                wi_enable_menu (menu_id->display_options_id,
				menu_id->menu_status[i]);
                old_menu_status[i] = menu_id->menu_status[i];
                }

            for (j = 0; j < 6; j++)
                {
                if (old_menu_item_status[i][j] !=
		    menu_id->menu_item_status[i][j])
                    {
                    wi_enable_menu_items (menu_id->display_options_id, 6,
                                          menu_id->menu_item_status[i]);
                    for (k = 0; k < 6; k++)
                        old_menu_item_status[i][k] =
			    menu_id->menu_item_status[i][k];
                    break;
                    }
                }
            break;

        case 3:	/* ----- Layout ----- */
            if (menu_id->layout_id == 0)
                break;

            if (menu_id->menu_status[i] != old_menu_status[i])
                {
                wi_enable_menu (menu_id->layout_id, menu_id->menu_status[i]);
                old_menu_status[i] = menu_id->menu_status[i];
                }

            for (j = 0; j < 2; j++)
                {
                if (old_menu_item_status[i][j] !=
		    menu_id->menu_item_status[i][j])
                    {
                    wi_enable_menu_items (menu_id->layout_id, 2,
                                          menu_id->menu_item_status[i]);
                    for (k = 0; k < 5; k++)
                        old_menu_item_status[i][k] =
			    menu_id->menu_item_status[i][k];
                    break;
                    }
                }
            break;

        default: /* ----- default ----- */
            break;
            }
        }

    return SUCCESS;
}
/* END:  */
