/* DEC/CMS REPLACEMENT HISTORY, Element XD_POST_SCA.C*/
/* *8    16-AUG-1991 16:19:21 JULIAN "(SPR 3775) added check for NO SYMBOL type"*/
/* *7    18-JUL-1991 15:20:27 MING "(SPR 0) print 2 demical points for label"*/
/* *6    17-DEC-1990 17:06:04 GILLESPIE "(SPR 6263) Numerous tweeks - biggest is to use cartography correctly"*/
/* *5    17-AUG-1990 22:32:13 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    19-SEP-1989 11:13:10 GORDON "(SPR 100) make scatter type NLIST_HEADER, not NLIST_HEADER*"*/
/* *3    14-SEP-1989 18:45:12 GORDON "(SPR -1) checkin from gulf"*/
/* *2    20-JUL-1989 13:02:01 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:41:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XD_POST_SCA.C*/
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
#include "esi_xd_defs.h"
#include "esi_wm_cls.h"
#include "esi_lu.h"
#include "esi_wm.h"
#include "esi_ct.h"
#include "esi_ov.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_co_err.h"
#include "esi_ov_err.h"

/* Function Description -----------------------------------------------------
Description:
    Function to post the supplied scatter file.

Prototype:
    publicdef INT xd_post_scatter(XYZ_DISPLAY *xyz_display,SYMBOL *symbol);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xyz_display     -(XYZ_DISPLAY *)
    symbol          -(SYMBOL *)

Return Value/Status:
    SUCCESS - Successful completion.
    CO_NO_SCATTER_DATA
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xd_post_scatter (XYZ_DISPLAY *xyz_display, SYMBOL *symbol)
#else
publicdef INT xd_post_scatter (xyz_display, symbol)
XYZ_DISPLAY *xyz_display;
SYMBOL *symbol;
#endif
    {
    
    INT status = SUCCESS;           /* RETURN STATUS */
    INT i;                          /* DUMMY INDEX */
    MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE */
    CHAR label[36];
    SEGMENT_ID seg_id;
    CHAR segname[100];
    NLIST_HEADER scatter;           /* SCATTER FILE */
    FLOAT *scat_x = (FLOAT *)0;     /* SCATTER FILE X COORDS */
    FLOAT *scat_y = (FLOAT *)0;     /* SCATTER FILE Y COORDS */
    FLOAT *scat_z = (FLOAT *)0;     /* SCATTER FILE Z COORDS */
    INT scat_npts;                  /* NUMBER OF SCATTER POINTS */
    FLOAT *sub_x = (FLOAT *)0;
    FLOAT *sub_y = (FLOAT *)0;
    FLOAT *px, *py;
    INT sub_npts;
    COORD x;
    COORD y;
    DOUBLE xoffset;
    DOUBLE yoffset;
    FILENAME os_name;
    PROJECTION_ID projection_id_in;
    PROJECTION_STRUCTURE *projection_in;
    PROJECTION_STRUCTURE *projection_out;
    OV_SCATTER_USER scatter_user;
    size_t size;
    BOOL need_to_convert,no_symbol;
    
    /* ********************************************************************** */
    /* * CHECK IF GOING TO DISPLAY ANYTHING * */
    /* -------------------------------------- */
    
    if (xyz_display->suppress_text AND xyz_display->suppress_symbols)
        {
        goto finished;
        }
    status = mp_get_current_map (&map);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }

    /* * GET SCATTER POINTS * */
    /* ---------------------- */
    
    status = ov_open_scatter (&scatter, xyz_display->xyz_file, "r", os_name);
    if (status < 0)
        {
        goto finished;
        }
    status = ov_read_scatter (scatter, &scat_x, &scat_y, &scat_z, &scat_npts);
    if (status < 0)
        {
        goto finished;
        }
    /* Each scatter file has its own projection (which was the map projection used
    when the scatter file was created. Now determine if additional coordinate conversions
    are required */
    
    status = nl_inq_nlist_user_struct (scatter, sizeof(OV_SCATTER_USER),
                                       (VOIDPTR)&scatter_user, (size_t *)&size);
    if (status != SUCCESS OR size != sizeof(OV_SCATTER_USER))
        {
        return OV_PROJECTION_MISMATCH;
        }
    need_to_convert = ARE_DIFFERENT (scatter_user.projection, map->map_projection);
    if (need_to_convert)
        {
        ct_get_projection (scatter_user.projection, &projection_in);
        ct_get_projection (map->map_projection, &projection_out);
        }
    ov_close_scatter (scatter, xyz_display->xyz_file, "r", os_name);
    if (scat_npts EQUALS 0)
        {
        status = CO_NO_SCATTER_DATA;
        goto finished;
        }
    /* * ALLOCATE SEGMENT * */
    /* -------------------- */
    
    wmsallocate (&seg_id);
    
    sprintf ((char *)segname, "XYZ: %s", xyz_display->xyz_file);
    wmoset ((WM_ID)seg_id, WM_CLASS_SEGMENT, (CHAR *)"", segname);

    /* check for NO_SYMBOL type */
    if (NOT xyz_display->suppress_symbols)
      {
	if(symbol->symbol_type <= 0)
	  no_symbol = TRUE;
	else
	  no_symbol = FALSE;
      }
    
    /* * SET SYMBOL CONSTANTS * */
    /* ------------------------ */

    if (NOT xyz_display->suppress_symbols AND NOT no_symbol)
        {
        wm_gspmci (symbol->symbol_color);
        wm_gsmk (symbol->symbol_type);
        x = symbol->symbol_size * map->wcs_to_inches * 4.0;
        wm_gsmksc (x);
        
        i = (scat_npts / xyz_display->sample_int) + 1;
        sub_x = (FLOAT *) TALLOC (i * sizeof(FLOAT));
        sub_y = (FLOAT *) TALLOC (i * sizeof(FLOAT));
        sub_npts = 0;
        }
    /* * SET TEXT CONSTANTS * */
    /* ---------------------- */
    
    if (NOT xyz_display->suppress_text)
        {
        GKS_INT i1, i2;
        
        wm_gstxci (xyz_display->text_color);
        wm_gschh (xyz_display->text_size);
        lu_text_to_index ("TEXT_PATH", (INDEX *)&i1, "RIGHT");
        wm_gstxp (i1);
        lu_text_to_index ("FONT", (INDEX *)&i1, "SIMPLEX");
        lu_text_to_index ("TEXT_PRECISION", (INDEX *)&i2, "STROKE");
        wm_gstxfp (i1, i2);
        wm_gschxp (1.0);
        wm_gschsp (0.0);
        wm_gschup (0.0, 1.0);
        
        switch (xyz_display->label_position)
            {
        case 1:                     /* ABOVE */
            xoffset = 0.0;
            yoffset = 0.75 * symbol->symbol_size;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "CENTER");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "BOTTOM");
            break;
            
        case 2:                     /* BELOW */
            xoffset = 0.0;
            yoffset = -0.75 * symbol->symbol_size;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "CENTER");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "TOP");
            break;
            
        case 3:                     /* CENTERED */
            xoffset = 0.0;
            yoffset = 0.0;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "CENTER");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "HALF");
            break;
            
        case 4:                     /* LEFT */
            xoffset = -0.75 * symbol->symbol_size;
            yoffset = 0.0;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "RIGHT");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "HALF");
            break;
            
        case 5:                     /* RIGHT */
            xoffset = 0.75 * symbol->symbol_size;
            yoffset = 0.0;
            lu_text_to_index ("TEXT_HORIZ_ALIGN", (INDEX *)&i1, "LEFT");
            lu_text_to_index ("TEXT_VERT_ALIGN", (INDEX *)&i2, "HALF");
            break;
            
            }
        wm_gstxal (i1, i2);
        }
    /* * POSTING LOOP * */
    /* ---------------- */
    
    for (i = 0; i < scat_npts; i += xyz_display->sample_int)
        {
        
        /* Window on the CURRENT MAP coordinates, not the stored coordinates */
        if (need_to_convert)
            {
            status = ct_transform_point ((COORD)scat_x[i], (COORD)scat_y[i],
                                         projection_in, &x, &y, projection_out);
            }
        else
            {
            x = scat_x[i];
            y = scat_y[i];
            }
        if (x >= map->lower_left_xy.x AND x <= map->upper_right_xy.x AND y >=
            map->lower_left_xy.y AND y <= map->upper_right_xy.y)
            {
            if (NOT xyz_display->suppress_symbols AND NOT no_symbol)
                {
                sub_x[sub_npts] = x;
                sub_y[sub_npts] = y;
                sub_npts++;
                }
            if (NOT xyz_display->suppress_text)
                {
                x += xoffset;
                y += yoffset;
                sprintf (label, "%.2f", scat_z[i]);
                wm_gtx (x, y, label);
                }
            }
        }
    if (NOT xyz_display->suppress_symbols AND NOT no_symbol)
        {
        wm_gpm (sub_npts, sub_x, sub_y);
        }
    wmsclose (seg_id);
    
finished:
    
    am_free (scat_x);
    am_free (scat_y);
    am_free (scat_z);
#if NEED_TO_FREE
    tc_free (sub_x);
    tc_free (sub_y);
#endif
    return status;
    }
/* END:     */
