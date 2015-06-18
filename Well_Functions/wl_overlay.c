/* DEC/CMS REPLACEMENT HISTORY, Element WL_OVERLAY.C*/
/* *13   30-JUL-1991 14:27:52 CHARLIE "(SPR 8688) symboltype=none becomes suppress symbol"*/
/* *12    9-JUL-1991 19:43:14 CHARLIE "(SPR 3109) engage suppress surface symbol"*/
/* *11    4-DEC-1990 14:13:10 GILLESPIE "(SPR 1) Convert several string parameters to upper case"*/
/* *10   14-AUG-1990 13:41:51 VINCE "(SPR 5644) Header Standardization"*/
/* *9     9-FEB-1990 16:24:53 MING "(SPR 6007) change node_id declaration to long int"*/
/* *8    23-JAN-1990 12:34:41 GILLESPIE "(SPR 6001) Change all [21] sizes to [24] for RISC machines"*/
/* *7    17-JAN-1990 17:18:48 GILLESPIE "(SPR 6001) changes for new well structures"*/
/* *6    17-JAN-1990 15:40:58 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *5    17-NOV-1989 14:13:28 GILLESPIE "(SPR 1) Change blocksize to 500; qualify the SOURCE column"*/
/* *4    24-OCT-1989 09:40:25 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *3    11-AUG-1989 20:47:36 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *2    27-JUL-1989 17:43:42 GILLESPIE "(SPR 1) fix minor casting problem"*/
/* *1    19-JUN-1989 13:40:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_OVERLAY.C*/
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
#include "esi_md_dbdefs.h"
#include "esi_ov.h"
#include "esi_wl.h"
#include "esi_wl_err.h"
#include "esi_wl_msg.h"
#include "esi_nl.h"
#include "esi_mp.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_am.h"
#include "esi_fm.h"
#include "esi_mg.h"
#include "esi_sl.h"

/* Function Description -----------------------------------------------------
Description:
    Function to process a 'WELLS' overlay.
    There are three nlists used in this overlay:
        1) Non deviated wells - "nodev_nlist" (4 dimensions),
        2) Deviated wells - "dev_nlist" (8 dimensions),
        3) UWIS - "well_nlist" (1 dimension).

        The dimensions of these Nlists hold the following info:

        DIM INFO
        --- -------------------
        1   UWI
        2   PLOT SYMBOL
        3   BASE X LOCATION (PROJECT PROJECTION)
        4   BASE Y LOCATION (PROJECT PROJECTION)
        5   TOP X LOCATION (PROJECT PROJECTION)
        6   TOP Y LOCATION (PROJECT PROJECTION)
        7   TOP POSITION (NAME)
        8   DEVIATION FILE NAME

Prototype:
    publicdef INT wl_overlay(INT display_line);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_overlay (INT display_line) 
#else
publicdef INT wl_overlay (display_line)
INT display_line;
#endif
    {
    
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT row;
    INT i;
    WELLS wells;
    INT num_well;
    NLIST_HEADER well_nlist = (NLIST_HEADER)0;
    NLIST_HEADER dev_nlist = (NLIST_HEADER)0;
    NLIST_HEADER nodev_nlist = (NLIST_HEADER)0;
    FM_FILE name;
    CHAR mode[2];
    FILENAME os_name;
    CHAR desc[241];
    INT status;
    PICKABLE_WELLS *picks;
    INT ncells;
    BOOL write_file = FALSE;
    
    static INT units[8] = 
        {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
        };
    static size_t d_size_1[1] = 
        {
        sizeof(SELECT_KEY_STRING)
        };
    static size_t d_size[7] = 
        {
        sizeof(UWI), sizeof(INT), sizeof(FLOAT), sizeof(FLOAT), sizeof(FLOAT),
        sizeof(FLOAT), sizeof(NODE_ID)
        };
    static INT d_type[7] = 
        {
        CHAR_TYPE,
        INT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        FLOAT_TYPE,
        INT_TYPE
        };
    static INT v_type[7] = 
        {
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST,
        VALUE_LIST
        };
    static UINT blocksize = 500;
    static UINT d_list1[1] = 
        {
        1
        };
    static UINT d_list4[4] = 
        {
        1,
        2,
        3,
        4
        };
    static UINT d_list8[7] = 
        {
        1,
        2,
        3,
        4,
        5,
        6,
        7
        };
    static CHAR *c_name1[2] = 
        {
        "UWI",
        NULL
        };
    static CHAR *c_name4[5] = 
        {
        "UWI",
        "PLOT_SYMBOL",
        "X",
        "Y",
        NULL
        };
    static CHAR *c_name8[8] = 
        {
        "UWI",
        "PLOT_SYMBOL",
        "BOTTOM_X",
        "BOTTOM_Y",
        "TOP_X",
        "TOP_Y",
        "TOP_POSITION",
        NULL
        };
    
    /***************************************************************************/
    
    if ((status = mp_get_current_map (&mapdef)) != SUCCESS)
        {
        goto finished;
        }
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];
    
    /* * INITIALIZE STRUCTURES USED IN OVERLAY * */
    /* ----------------------------------------- */
    
    tc_zeroes (&wells, sizeof(WELLS));
    if (compiled->verb_code[row] EQUALS MP_WELLDPTH)
        {
        wells.ov_type = 1;
        }
    else if (compiled->verb_code[row] EQUALS MP_WELLPACH)
        {
        wells.ov_type = 2;
        }
    /* * FILL IN ORGANIZING STRUCTURES * */
    /* --------------------------------- */
    
    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];
            
            switch (verbstruc->key_code[i])
                {
            case 1:                 /* ABOVE */
                wells.above = parmstruc->parmval.cval;
                break;
                
            case 2:                 /* ADD TO XYZ FILE */
                wells.add_to_xyz_file = parmstruc->parmval.cval;
                write_file = TRUE;
                break;
                
            case 3:                 /* BELOW */
                wells.below = parmstruc->parmval.cval;
                break;
                
            case 4:                 /* CENTERED */
                wells.centered = parmstruc->parmval.cval;
                break;
                
            case 5:                 /* LEFT */
                wells.left = parmstruc->parmval.cval;
                break;
                
            case 6:                 /* NAME */
                wells.name = parmstruc->parmval.cval;
                break;
                
            case 7:                 /* RIGHT */
                wells.right = parmstruc->parmval.cval;
                break;
                
            case 8:                 /* SELECT BY PHRASE */
                wells.select_by_phrase = parmstruc->parmval.cval;
                break;
                
            case 9:                 /* SELECT LIST */
                wells.select_list = ts_sto_upper(parmstruc->parmval.cval);
                break;
                
            case 10:                /* SUPPRESS SYMBOLS */
                wells.suppress_symbols = TRUE;
                break;
                
            case 11:                /* SUPPRESS TEXT */
                wells.suppress_text = TRUE;
                break;
                
            case 12:                /* SYMBOL COLOR */
                wells.symbol_color = parmstruc->parmval.ival;
                break;
                
            case 13:                /* SYMBOL SIZE */
                wells.symbol_size = parmstruc->parmval.dbval;
                break;
                
            case 14:                /* SYMBOL TYPE*/
                wells.symbol_type = parmstruc->parmval.ival;
		if (wells.symbol_type == 0)
			wells.suppress_symbols = TRUE;
                break;
                
            case 15:                /* TEXT COLOR */
                wells.text_color = parmstruc->parmval.ival;
                break;
                
            case 16:                /* TEXT SIZE */
                wells.text_size = parmstruc->parmval.dbval;
                break;
                
            case 17:                /* XYZ FILE */
                wells.xyz_file = parmstruc->parmval.cval;
                write_file = TRUE;
                break;
                
            case 18:                /* ZFIELD */
                wells.zfield = parmstruc->parmval.ival;
                break;
                
            case 19:                /* <BOTTOM> HORIZON */
                wells.bottom_horizon = ts_sto_upper(parmstruc->parmval.cval);
                break;
                
            case 20:                /* TOP HORIZON */
                wells.top_horizon = ts_sto_upper(parmstruc->parmval.cval);
                break;
                
            case 21:                /* LABEL POSITION */
                wells.label_position = parmstruc->parmval.ival;
                break;
                
            case 22:                /* SOURCE */
                wells.source = ts_sto_upper(parmstruc->parmval.cval);
                break;
                
            case 23:                /* FULL TRACE*/
                wells.full_trace = TRUE;
                break;
                
            case 24:                /* SUPPRESS SURFACE SYMBOLS */
                wells.suppress_surface_symbols = TRUE;
                break;
                }
            }
        }
    /* * CONSTRUCT LABEL SQL PHRASE IF NECESSARY AND CHECK ZFIELD * */
    /* ------------------------------------------------------------ */
    
    if (wells.ov_type)
        {
        wl_label_sql (&wells);
        if (write_file)
            {
            wells.zfield = wells.label_position;
            }
        }
    else
        {
        /* WELLS OVERLAY WITH NO ZFIELD SPECIFIED */
        
        if (write_file AND NOT wells.zfield)
            {
            /* IF ONE LABEL POSITION, USE IT */
            ncells = 0;
            if (wells.above != (CHAR *)0)
                {
                ncells++;
                i = 1;
                }
            if (wells.below != (CHAR *)0)
                {
                ncells++;
                i = 2;
                }
            if (wells.centered != (CHAR *)0)
                {
                ncells++;
                i = 3;
                }
            if (wells.left != (CHAR *)0)
                {
                ncells++;
                i = 4;
                }
            if (wells.right != (CHAR *)0)
                {
                ncells++;
                i = 5;
                }
            if (ncells != 1)
                {
                am_message (AM_ALERT, mg_message (WL_CANT_DETERMINE_ZFIELD));
                }
            else
                {
                wells.zfield = i;
                }
            }
        /* WELLS OVERLAY WITH NO FILE SPECIFIED */
        
        else if (NOT write_file AND wells.zfield)
            {
            wells.zfield = 0;
            am_message (AM_ALERT, mg_message (WL_NO_FILE_SPECIFIED));
            }
        }
    /* * INITIALIZE THE NLISTS * */
    /* ------------------------- */
    
    status = nl_init_nlist (&well_nlist, 1, units, d_size_1, d_type, v_type,
                            blocksize);
    if (status < 0)
        {
        goto finished;
        }
    status = nl_set_nlist_info (well_nlist, NL_NLIST_COLUMN_NAME, 1, d_list1,
                                (VOIDPTR)c_name1);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_table (well_nlist, (UINT *) & i);
    
    status = nl_init_nlist (&nodev_nlist, 4, units, d_size, d_type, v_type, blocksize);
    if (status < 0)
        {
        goto finished;
        }
    status = nl_set_nlist_info (nodev_nlist, NL_NLIST_COLUMN_NAME, 4, d_list4,
                                (VOIDPTR)c_name4);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_table (nodev_nlist, (UINT *) & i);
    
    status = nl_init_nlist (&dev_nlist, 7, units, d_size, d_type, v_type, blocksize);
    if (status < 0)
        {
        goto finished;
        }
    status = nl_set_nlist_info (dev_nlist, NL_NLIST_COLUMN_NAME, 7, d_list8,
                                (VOIDPTR)c_name8);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_table (dev_nlist, (UINT *) & i);
    
    /* * GET THE LIST OF WELLS * */
    /* ------------------------- */
    
    status = wl_get_wells (&wells, nodev_nlist, &dev_nlist, well_nlist, &num_well);
    /* tc_free(wells.select_by_phrase); */
    if (status < 0)
        {
        goto finished;
        }
    if (num_well <= 0)
        {
        status = WL_NO_WELLS_SELECTED;
        goto finished;
        }
    /* * OPEN SCATTER FILE IF NEEDED * */
    /* ------------------------------- */
    
    if (wells.zfield)
        {
        /* NEW SCATTER FILE */
        if (wells.xyz_file != NULL)
            {
            strcpy (name, wells.xyz_file);
            strcpy (mode, "w");
            strcpy (desc, "Created ");
            }
        /* EXISTING SCATTER FILE */
        
        else if (wells.add_to_xyz_file != NULL)
            {
            strcpy (name, wells.add_to_xyz_file);
            strcpy (mode, "a");
            strcpy (desc, "Appended ");
            }
        if ((status = ov_open_scatter (&wells.scatter, name, mode,
                                       os_name)) EQUALS SUCCESS)
            {
            status = ov_set_scatter_horizons (wells.scatter, wells.bottom_horizon,
                                              wells.top_horizon);
            }
        if (status != SUCCESS)
            {
            wells.zfield = 0;
            am_message (AM_ALERT, mg_message (status));
            }
        }
    /* * EXECUTE THE OVERLAY * */
    /* ----------------------- */
    
    /* OPEN THE OVERLAY */
    
    status = ov_open_overlay (display_line, wells.name);
    if (status < 0)
        {
        goto finished;
        }
    /* ALLOCATE PICKING STRUCTURE ARRAY */
    
    picks = (PICKABLE_WELLS *)(compiled->picks[row]);
    if (picks EQUALS (PICKABLE_WELLS *)NULL)
        {
        picks = (PICKABLE_WELLS *)am_allocate (AM_GLOBAL, sizeof(PICKABLE_WELLS));
        compiled->picks[row] = (BYTE *)picks;
        }
    /* PLOT THE OVERLAY */
    
    status = wl_post_wells (&wells, nodev_nlist, dev_nlist, picks);
    
    /* CLOSE THE OVERLAY */
    
    ov_close_overlay (display_line);
    
    /* CLOSE SCATTER FILE IF NEEDED */
    
    if (wells.zfield)
        {
        ov_close_scatter (wells.scatter, name, mode, os_name);
        
        sprintf (desc + strlen(desc), "in overlay \"%s\" in map \"%s\"",
	    display->verbs[display_line], mapdef->map_name);
        fm_add_history_record ("ESI$SCATTERS", name, "SCATTER", "", "", "", desc);
        }
    /* * RETURN * */
    /* ---------- */
    
finished:
    nl_free_nlist (nodev_nlist);
    nl_free_nlist (dev_nlist);
    nl_free_nlist (well_nlist);
    
    return status;
    }
/* END:     */
