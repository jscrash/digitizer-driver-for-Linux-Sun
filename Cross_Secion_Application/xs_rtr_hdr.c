/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_HDR.C*/
/* *7    27-MAR-1991 13:30:32 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *6    11-DEC-1990 11:35:30 MING "(SPR -1) skip empty header string"*/
/* *5    17-AUG-1990 22:43:12 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    25-JAN-1990 09:47:33 GILLESPIE "(SPR 6001) Free up strings from previous path; remove offshore items"*/
/* *3    17-JAN-1990 22:43:58 GILLESPIE "(SPR 6001) Changes for altered well structs"*/
/* *2    14-SEP-1989 16:24:34 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:45:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_HDR.C*/
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
#include "esi_xs_defs.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Create a linked list of all header copied_layout to be put on logPlot.

Prototype:
    publicdef INT xs_retrieve_hdr (LAYOUT_STRUCT *sample_layout, LAYOUT_STRUCT *copied_layout);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) sample_layout   -(LAYOUT_STRUCT *) The current layout structure.
    (I) copied_layout   -(LAYOUT_STRUCT *) A pointer to layout structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful Completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_retrieve_hdr (LAYOUT_STRUCT *sample_layout,
                               LAYOUT_STRUCT *copied_layout)
#else
publicdef INT xs_retrieve_hdr (sample_layout, copied_layout)
LAYOUT_STRUCT *sample_layout;
LAYOUT_STRUCT *copied_layout;
#endif
    {
    INT i, totalItems;
    CHAR temp[128];
    CHAR *ptr = (CHAR *)0;
#if 0
    WELL_STRUCT *well_header; /* for pwell2 for rigrel, platform_name and 
				 offshore block */
    WELL_HDR_2 *pwell2;
#endif
    
    /* error checking.                  */
    
    if (sample_layout EQUALS (LAYOUT_STRUCT *)
        NULL OR copied_layout EQUALS (LAYOUT_STRUCT *)NULL)
        return XS_INVALID_POINTER;
    
    copied_layout->log_hdr = sample_layout->log_hdr;
    
    totalItems = sample_layout->log_hdr.total_items;
    
    /* if there is no items, return.     */
    
    if (totalItems EQUALS 0)
        {
        copied_layout->log_hdr.items = (INT *)NULL;
        return SUCCESS;
        }
    /* copy all items in the items array*/
    /* into all layout in the linked list.*/
    
    copied_layout->log_hdr.items = (INT *)tc_alloc (sizeof(INT) * totalItems);
    hoblockmove (sample_layout->log_hdr.items, copied_layout->log_hdr.items,
                 sizeof(INT) * totalItems);
    
    /* if the hdr_items is not empty, free
                           it first, then initialize a new one.*/
    
    for (i = 1; i <= totalItems; i++)
        {
        llgetn (&(copied_layout->log_hdr.hdr_items), &ptr, i);
        tc_free (ptr);
        }
    llfree (&(copied_layout->log_hdr.hdr_items));
    
    if (llinit (&(copied_layout->log_hdr.hdr_items), 0, sizeof(CHAR *),
                LL_LIST_SYSTEM) IS_NOT_EQUAL_TO SUCCESS)
        {
        return FAIL;
        }
#if 0
    we_get_well_header(copied_layout->pwell->uwi,
		       WELL_HDR_2_DETAIL, &well_header);
    pwell2 = (WELL_HDR_2 *)well_header;
#endif

    for (i = 0; i < totalItems; i++)
        {
        WELL_HDR_1 *pwell1 = (WELL_HDR_1 *)copied_layout->pwell->detail.ptr;
        switch (sample_layout->log_hdr.items[i])
            {
        case XS_NAME_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("WELL NAME : ") + strlen(pwell1->name) + 2));
	    strcpy (ptr, "WELL NAME : ");
            strcat (ptr, pwell1->name);
            break;
            
        case XS_NUMBER_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("WELL NUMBER : ") + strlen(pwell1->number) + 2));
	    strcpy (ptr, "WELL NUMBER : ");
            strcat (ptr, pwell1->number);
            break;
            
        case XS_SHORT_NAME_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("SHORT NAME : ") + strlen(pwell1->short_name) + 2));
	    strcpy (ptr, "SHORT NAME : ");
            strcat (ptr, pwell1->short_name);
            break;
            
        case XS_OPERATOR_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("OPERATOR : ") + strlen(pwell1->operator) + 2));
	    strcpy (ptr, "OPERATOR : ");
            strcat (ptr, pwell1->operator);
            break;
            
        case XS_CLASS_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("CLASSIFICATION : ") + strlen(pwell1->class) + 2));
	    strcpy (ptr, "CLASSIFICATION : ");
            strcat (ptr, pwell1->class);
            break;
            
        case XS_STATUS_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("CURRENT STATUS : ") + strlen(pwell1->crstatus) + 2));
	    strcpy (ptr, "CURRENT STATUS : ");
            strcat (ptr, pwell1->crstatus);
            break;
            
        case XS_SPUD_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("START DATE : ") + strlen(pwell1->Spud_Date) + 2));
	    strcpy (ptr, "START DATE : ");
            strcat (ptr, pwell1->Spud_Date);
            break;
            
        case XS_COMP_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("COMPLETE DATE : ") + strlen(pwell1->Comp_Date) + 2));
	    strcpy (ptr, "COMPLETE DATE : ");
            strcat (ptr, pwell1->Comp_Date);
            break;
            
        case XS_OWNER_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("PRIMARY SOURCE : ") + strlen(pwell1->primary_source) + 2));
	    strcpy (ptr, "PRIMARY SOURCE : ");
            strcat (ptr, pwell1->primary_source);
            break;
            
        case XS_UWI_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("UWI : ") + strlen(copied_layout->pwell->uwi) + 2));
	    strcpy (ptr, "UWI : ");
            strcat (ptr, copied_layout->pwell->uwi);
            break;
            
        case XS_ELEVATION_ITEM:
            ptr = (CHAR *)tc_zalloc ((sprintf (temp, "%.2f %s", 
					       pwell1->elevation.value,
					       pwell1->elevation.ref)) +
				     (sizeof(CHAR) * (strlen("ELEVATION : ")
						      + 2)));
	    strcpy (ptr, "ELEVATION : ");
            strcat (ptr, temp);
            break;
            
        case XS_STATE_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("PROVINCE/STATE : ") + strlen(pwell1->tophole.state) + 2));
	    strcpy (ptr, "PROVINCE/STATE : ");
            strcat (ptr, pwell1->tophole.state);
            break;
            
        case XS_COUNTY_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("COUNTY : ") + strlen(pwell1->tophole.county) + 2));
	    strcpy (ptr, "COUNTY : ");
            strcat (ptr, pwell1->tophole.county);
            break;
            
        case XS_DRILLERS_TD_ITEM:
            ptr = (CHAR *)tc_zalloc ((sprintf (temp, "%.2f",
                                             pwell1->td.driller.md)) +
				     (sizeof(CHAR) * (strlen("DRILLER'S TD : ")
						      + 2)));
	    strcpy (ptr, "DRILLER'S TD : ");
            strcat (ptr, temp);
            break;
            
        case XS_LOG_TD_ITEM:
            ptr = (CHAR *)tc_zalloc ((sprintf (temp, "%.2f",
                                             pwell1->td.logger.md)) +
				     (sizeof(CHAR) * (strlen("LOGGER'S TD : ")
						      + 2)));
	    strcpy (ptr, "LOGGER'S TD : ");
            strcat (ptr, temp);
            break;
            
        case XS_WATER_DEPTH_ITEM:
            ptr = (CHAR *)tc_zalloc ((sprintf (temp, "%.2f",
                                             pwell1->offshore.water_depth)) +
				     (sizeof(CHAR) * (strlen("WATER DEPTH : ")
						      + 2)));
	    strcpy (ptr, "WATER DEPTH : ");
            strcat (ptr, temp);
            break;
            
#if 0
        case XS_RIGREL_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("RIG RELEASED : ") + strlen(pwell2->rigrel) + 2));
	    strcpy (ptr, "RIG RELEASED : ");
            strcat (ptr, pwell2->rigrel);
            break;
            
        case XS_PLATFORM_NAME_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("PLATFORM NAME : ") + strlen(pwell2->offshore.platform_name) + 2));
	    strcpy (ptr, "PLATFORM NAME : ");
            strcat (ptr, pwell2->offshore.platform_name);
            break;

        case XS_OFFSHORE_BLOCK_ITEM:
	    ptr  = (CHAR *) tc_zalloc(sizeof(CHAR) *
                      (strlen("OFFSHORE BLOCK : ") + strlen(pwell2->offshore.offshore_block) + 2));
	    strcpy (ptr, "OFFSHORE BLOCK : ");
            strcat (ptr, pwell2->offshore.offshore_block);
            break;
#endif
            
        default:
            ptr = (CHAR *)tc_zalloc (1);
            break;
            }
        if (ptr != (CHAR *)0 AND strcmp (ptr,"") != 0)
            {
            llinsa (&(copied_layout->log_hdr.hdr_items), &ptr);
            }
        ptr = (CHAR *)0;
        }
    return SUCCESS;
    }
/* END:     */
