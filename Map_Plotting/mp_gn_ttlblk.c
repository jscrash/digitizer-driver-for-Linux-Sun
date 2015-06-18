/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GN_TTLBLK.C*/
/*  *5    17-AUG-1990 22:05:50 VINCE "(SPR 5644) Code Cleanup"*/
/*  *4     1-JUN-1990 12:16:59 SCD "(SPR 1) Remove extraneous carriage return from time/date string."*/
/*  *3    27-OCT-1989 11:23:02 GILLESPIE "(SPR 1) Add esi_time.h include file - change data type of i to time_t"*/
/*  *2    26-JUN-1989 10:56:11 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:07 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GN_TTLBLK.C*/
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

#include "esi_mp.h"
#include "esi_am.h"
#include "esi_qs.h"
#include "esi_ts.h"
#include "esi_time.h"
#include "md_mapdef.rh"

/* Function Description -----------------------------------------------------
Description:
    Function to generate a default TITLE BLOCK
    base on the map definition information.

Prototype:
    publicdef INT mp_gen_title_block(MAP_STRUCTURE *map);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    map             -(MAP_STRUCTURE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_gen_title_block (MAP_STRUCTURE *map)
#else
publicdef INT mp_gen_title_block (map)
MAP_STRUCTURE *map;
#endif
    {
    INT status = SUCCESS;
    PROJECT_NAME project;
    time_t i;
    static INT def_size[25] = 
        {
        1,
        1,
        1,
        4,
        5,
        5,
        5,
        5,
        3,
        3,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1
        };
    RESOURCE_ID *cdlg_id;
    CHAR text[101];
    
    /****************************************************************************/
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    
    /* * INITIALIZE TITLE BLOCK PARAMETERS */
    /* ----------------------------------- */
    
    hoblockmove (def_size, map->title_size, 25 * sizeof(INT));
    tc_zeroes (map->title_text, 2525);
    strcpy (map->title_block_name, map->map_name);
    
    /* * INSERT DEFAULT TEXT STRINGS * */
    /* ------------------------------- */
    
    strcpy (map->title_text[5], map->map_name);
    qs_inq_c (QS_PROJECT_NAME, project, (INT *)0);
    sprintf (map->title_text[7], "PROJECT: %s", project);
    sprintf (map->title_text[8], "PROJECTION: %s", map->map_projection);
    
    sprintf (map->title_text[9], "SCALE: %-.2f %s", map->map_scale,
             map->map_scale_uom);
    if (cdlg_id->map_setup_id != 0)
        {
        wi_query_dialog_item (cdlg_id->map_setup_id, MD_MAP_SCALE, &i, text);
        if (strlen (text))
            {
            sprintf (map->title_text[9], "SCALE: %s", text);
            }
        }
    strcpy (map->title_text[24], ts_snowhite (ctime (&i)));
    
    /* COMPANY NAME AND ADDRESS */
    /* NOT AVAILABLE YET */
    
    /* CONTOUR INFORMATION */
    /* NOT AVAILABLE YET */
    
    
    return status;
    }
/* END:     */
