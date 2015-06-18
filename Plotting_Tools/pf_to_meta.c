/* DEC/CMS REPLACEMENT HISTORY, Element PF_TO_META.C */
/* *11   16-AUG-1991 16:41:53 MING "(SPR 0) fix vdc_extent problem" */
/* *10   14-AUG-1991 12:00:52 KEE "(SPR -1) Put in File Lock/Unlock logic" */
/* *9    12-JUL-1991 10:18:43 MING "(SPR 0) add PRIOR GKS codes in pf_init_file" */
/* *8     3-MAY-1991 16:12:18 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *7    23-OCT-1990 13:52:17 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *6    17-AUG-1990 22:11:44 VINCE "(SPR 5644) Code Cleanup" */
/* *5    10-JUL-1990 18:12:27 MING "(SPR -1) fix bug" */
/* *4    10-JUL-1990 17:47:59 MING "(SPR -1) FIX BUG FOR SUN VERSION" */
/* *3    20-JUN-1990 16:14:36 MING "(SPR 0) change float to double" */
/* *2    20-JUN-1990 10:54:45 MING "(SPR 0) create" */
/* *1    20-JUN-1990 10:53:08 MING "regenrate segment routines" */
/* DEC/CMS REPLACEMENT HISTORY, Element PF_TO_META.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*          (415) 927-0100                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"
#include    "esi_pf.h"

#ifndef ESI_WM_H

#include    "esi_wm.h"

#endif

#ifndef ESI_WM_CLS_H

#include    "esi_wm_cls.h"

#endif

#ifndef ESI_GKS_H

#include    "esi_gks.h"

#endif

#ifndef ESI_LH_H

#include    "esi_lh.h"

#endif


#ifndef ESI_HO_H

#include    "esi_ho.h"

#endif

#ifndef ESI_HO_FILES_H

#include    "esi_ho_files.h"

#endif

#ifndef ESI_AM_H

#include    "esi_am.h"

#endif

#ifndef ESI_WI_H

#include    "esi_wi.h"

#endif

#ifndef ESI_WM_INT_H

#include    "esi_wm_int.h"

#endif

#ifndef ESI_FI_H

#include    "esi_fi.h"

#endif

#ifndef ESI_ER_H

#include "esi_er.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#include    "pl_cdlgs.rh"

#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
#include "sun_ansicgks.h"
#endif /* ========== END PRIOR GKS ========== */

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Routines for metafile output.                                                 */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT pf_file_init(INT *meta_ws,PATHNAME metafile);                   */
/*                                                                                  */
/*    publicdef INT pf_file_close(INT meta_ws,PATHNAME metafile,CHAR *units,DOUBLE width,   */
/*                                                                                  */
/*    publicdef INT pf_win_init(INT meta_ws,INT window_id,lh_listhead **seg_list_ptr);  */
/*                                                                                  */
/*    publicdef INT pf_win_close(INT window_id,lh_listhead *seg_list_ptr,INT meta_ws);  */
/*                                                                                  */
/*    publicdef INT pf_all_segs(lh_listhead *seg_list_ptr);                         */
/*                                                                                  */
/*    publicdef INT pf_one_seg(INT seg_id,lh_listhead *seg_list_ptr);               */
/*                                                                                  */
/*    publicdef INT pf_all_groups(lh_listhead *seg_list_ptr);                       */
/*                                                                                  */
/*    publicdef INT pf_one_group(INT gro_id,lh_listhead *seg_list_ptr);             */
/*                                                                                  */
/*    publicdef INT pf_win_to_meta(INT meta_ws,INT window_id);                      */
/*                                                                                  */
/*    publicdef INT pf_ovl_to_meta(INT meta_ws,INT window_id,INT tnr_id,DOUBLE xscale,  */
/*        DOUBLE yscale,DOUBLE xoffset,DOUBLE yoffset,INT subgroup_id,INT group_id);*/
/*                                                                                  */
/*Private_Functions:                                                                */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Initialize metafile.                                                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_file_init(INT *meta_ws,PATHNAME metafile);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    meta_ws         -(INT *)                                                      */
/*    metafile        -(PATHNAME)                                                   */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_file_init (INT *meta_ws, PATHNAME metafile)
#else
publicdef INT pf_file_init (meta_ws, metafile)
INT *meta_ws;
PATHNAME metafile;
#endif
    {
    
    INT metatype;
    INT status;
    CHAR *pdot;

#if UNIX
#ifndef PRIOR_GKS /* ========== BEGIN SUN GKS ========== */
    Gks_ws_config config;
#endif /* ========== END SUN GKS ========== */
#endif
    pdot = strchr (metafile, '.') + 1;
    
    printf ("pathname %s\n", metafile);
    
    *meta_ws = GKS_WISS_WKID + 1;

#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
    if (ARE_SAME (pdot, "CGMT"))
      {
      gopenws (*meta_ws, metafile, "cgmoct");
      }
    else if (ARE_SAME (pdot, "CGMC"))
      {
      gopenws (*meta_ws, metafile, "cgmoce");
      }
    else if (ARE_SAME (pdot, "CGMB"))
      {
      gopenws (*meta_ws, metafile, "cgmobe");
      }
    else if (ARE_SAME (pdot, "GKSM"))
      {
      gopenws (*meta_ws, metafile, "moasc");
      }
    else if (ARE_SAME (pdot, "GKSMT"))
      {
      gopenws (*meta_ws, metafile, "moasc");
      }
    else if (ARE_SAME (pdot, "GKSMB"))
      {
      gopenws (*meta_ws, metafile, "mobin");
      }
    else if (ARE_SAME (pdot, "PS"))
      {
      gopenws (*meta_ws, metafile, "postscript");
      }
    else if (ARE_SAME (pdot, "PSC"))
      {
      gopenws (*meta_ws, metafile, "postscriptc");
      }
    else if (ARE_SAME (pdot, "PSM"))
      {
      gopenws (*meta_ws, metafile, "postscriptm");
      }
    else
      {
      printf ("Metatype %s not supported", pdot);
      return FAIL;
      }
#else    /* ========== END PRIOR GKS = BEGIN NON PRIOR GKS ==========*/
#if UNIX /*========== BEGIN SUN GKS ========== */
    if (ARE_SAME (pdot, "CGMB"))
        {
        config = gks_ws_config_get_installed ("cgmo");
        if (config == (Gks_ws_config)NULL)
            {
            return FAIL;
            }
        if (gks_ws_config_set (config, GKS_CGMO_ENCODING, GKS_BINARY, 0) == FALSE)
            {
            return FAIL;
            }
        if (gks_ws_config_install (config) == FALSE)
            {
            return FAIL;
            }
        gks_ws_config_destroy (config);
        gopenws (*meta_ws, metafile, "cgmo");
        }
    else if (ARE_SAME (pdot, "CGMC"))
        {
        config = gks_ws_config_get_installed ("cgmo");
        if (config == (Gks_ws_config)NULL)
            {
            return FAIL;
            }
        if (gks_ws_config_set (config, GKS_CGMO_ENCODING, GKS_CHARACTER, 0) == FALSE)
            {
            return FAIL;
            }
        if (gks_ws_config_install (config) == FALSE)
            {
            return FAIL;
            }
        gks_ws_config_destroy (config);
        gopenws (*meta_ws, metafile, "cgmo");
        }
    else if (ARE_SAME (pdot, "CGMT"))
        {
        config = gks_ws_config_get_installed ("cgmo");
        if (config == (Gks_ws_config)NULL)
            {
            return FAIL;
            }
        if (gks_ws_config_set (config, GKS_CGMO_ENCODING, GKS_CLEAR_TEXT, 0) == FALSE)
            {
            return FAIL;
            }
        if (gks_ws_config_install (config) == FALSE)
            {
            return FAIL;
            }
        gks_ws_config_destroy (config);
        gopenws (*meta_ws, metafile, "cgmo");
        }
    else if (ARE_SAME (pdot, "GKSM"))
        {
        gopenws (*meta_ws, metafile, "moasc");
        }
    else if (ARE_SAME (pdot, "PS"))
        {
        gopenws (*meta_ws, metafile, "postscript");
        }
    else if (ARE_SAME (pdot, "HPGL"))
        {
        gopenws (*meta_ws, metafile, "hpgl");
        }
    else
        {
        printf ("Metatype %s not supported\n", pdot);
        return FAIL;
        }
#else   /* ========== END SUN GKS = BEGIN DECW GKS ========== */
    if (ARE_SAME (pdot, "CGMT"))
        {
        metatype = GKS$K_CGM_OUTPUT | GKS$M_CLEAR_TEXT_ENCODING;
        }
    else if (ARE_SAME (pdot, "CGMC"))
        {
        metatype = GKS$K_CGM_OUTPUT | GKS$M_CHARACTER_ENCODING;
        }
    else if (ARE_SAME (pdot, "GKSM"))
        {
        metatype = GKS$K_GKSM_OUTPUT;
        }
    else if (ARE_SAME (pdot, "PS"))
        {
        metatype = GKS$K_POSTSCRIPT;
        }
    else
        {
        printf ("Metatype %s not supported\n", pdot);
        return FAIL;
        }
    gks$open_ws (meta_ws, hoz_str_descpt (metafile), &metatype);
#endif  /* ========== END DECW GKS ========== */
#endif  /* ========== END NON PRIOR GKS ========== */
    wm_gacwk (*meta_ws);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Close metafile and initiate plotting job.                                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_file_close(INT meta_ws,PATHNAME metafile,CHAR *units,DOUBLE width,   */
/*        DOUBLE height);                                                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    meta_ws         -(INT)                                                        */
/*    metafile        -(PATHNAME)                                                   */
/*    units           -(CHAR *)                                                     */
/*    width           -(DOUBLE)                                                     */
/*    height          -(DOUBLE)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_file_close (INT meta_ws, PATHNAME metafile, CHAR *units, DOUBLE width,
                             DOUBLE height, DOUBLE ndc_width, DOUBLE ndc_height)
#else
publicdef INT pf_file_close (meta_ws, metafile, units, width, height, ndc_width, ndc_height)
INT meta_ws;
PATHNAME metafile;
CHAR *units;
DOUBLE width;
DOUBLE height;
DOUBLE ndc_width;
DOUBLE ndc_height;
#endif
    {
    
    INT status;
    CHAR *pdot;
    PATHNAME paramfile;
    FILE *fd;
    DOUBLE user_unit_to_mm;
    WM_CURRENT *wmv = wm_static();
    
    printf ("Plot will be %.2f by %.2f (%s)\n", width, height, units);
    user_unit_to_mm = 1.0;
    tu_unit_conversion (user_unit_to_mm, units, &user_unit_to_mm, "MM");
    strcpy (paramfile, metafile);
    pdot = strchr (paramfile, '.') + 1;
    strcpy (pdot, "PAR");
    fd = ho_open (paramfile, "w", NULL);
    status = ho_lock(fd, ON);
    
    if (fd == NULL_FILE_PTR || status != SUCCESS)
        {
        printf ("file %s cannot open\n", paramfile);
        return FAIL;
        }
    /* ming: 8/16/81 */
    /* the vdc_extent is not exact from 0-1 since we reszie    */
    /* the window depend on the aspect ratio of the map/x-sect */
    /* ------------------------------------------------------- */
    fprintf (fd, "vdc_extent(0.0,0.0,%lf,%lf)\n",ndc_width,ndc_height);
    fprintf (fd, "vdc_viewport(0.0,0.0,%lf,%lf)\n", width * user_unit_to_mm,
             height * user_unit_to_mm);
    status = ho_lock(fd, OFF);
    fclose (fd);
    
    wm_gdawk (meta_ws);
    wm_gclwk (meta_ws);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write colour table and scaling information to                                 */
/*    metafile and open window.                                                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_win_init(INT meta_ws,INT window_id,lh_listhead **seg_list_ptr);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    meta_ws         -(INT)                                                        */
/*    window_id       -(INT)                                                        */
/*    seg_list_ptr    -(LH_LISTHEAD **)                                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_win_init (INT meta_ws, INT window_id, lh_listhead **seg_list_ptr)
#else
publicdef INT pf_win_init (meta_ws, window_id, seg_list_ptr)
INT meta_ws;
INT window_id;
lh_listhead **seg_list_ptr;
#endif
    {
    INT status;
    INT i;
    CHAR colours[40];
    GKS_INT type, length, errind;
    GKS_REAL cr, cg, cb;
    BOOL signed_keys_flag;
    BOOL duplicate_keys_flag;
    BOOL c_string_keys_flag;
    BOOL system_list_flag;
    lh_listhead *header_pointer;
    CHAR scalefactors[36];
    CHAR trans[84];
    FLOAT vx_ll, vx_ur, vy_ll, vy_ur;
    FLOAT wx_ll, wx_ur, wy_ll, wy_ur;
    
    /********************************************************************************/
    
    if (window_id == 0)
        return FAIL;
    
    /* Write colour table to metafile                                               */

#if 0
    for (i = 0; i < 16; i++)
        {
        wm_gqcr (MAIN_WKID, i, GREALI, &errind, &cr, &cg, &cb);
        sprintf (colours, "%6d%10f%10f%10f", i, cr, cg, cb);
        type = 201;
        length = strlen (colours);
        wm_gwitm (meta_ws, type, 0, length, colours);
        }
    /* Write scale factors to metafile                                              */
    
    sprintf (scalefactors, "%10f%10f%10f%10f", xscale, yscale, xoffset, yoffset);
    type = 202;
    length = strlen (scalefactors);
    wm_gwitm (meta_ws, type, 0, length, scalefactors);
    
    /* Open window                                                                  */
    
    wmwopen (window_id);
    wmtinq (tnr_id, &vx_ll, &vx_ur, &vy_ll, &vy_ur, &wx_ll, &wx_ur, &wy_ll, &wy_ur);
    
    /* Write transformation parameters to metafile                                  */
    
    sprintf (trans, "%12.2f%12.2f%12.2f%12.2f%12.2f%12.2f%12.2f%12.2f", vx_ll, vx_ur,
             vy_ll, vy_ur, wx_ll, wx_ur, wy_ll, wy_ur);
    type = 203;
    length = strlen (trans);
    wm_gwitm (meta_ws, type, 0, length, trans);

#endif
    
    /* Initialize linked list that will hold segment ids that are to be             */
    /*   written to metafile                                                        */
    wmwopen (window_id);
    
    signed_keys_flag = TRUE;
    duplicate_keys_flag = FALSE;
    c_string_keys_flag = FALSE;
    system_list_flag = FALSE;
    status = lh_initialize (&header_pointer, 0, sizeof(INT), signed_keys_flag,
                            duplicate_keys_flag, c_string_keys_flag, system_list_flag,
                            sizeof(INT));
    
    *seg_list_ptr = header_pointer;
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write all segments in linked list to metafile                                 */
/*    close window.                                                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_win_close(INT window_id,lh_listhead *seg_list_ptr,INT meta_ws);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    window_id       -(INT)                                                        */
/*    seg_list_ptr    -(LH_LISTHEAD *)                                              */
/*    meta_ws         -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_win_close (INT window_id, lh_listhead *seg_list_ptr, INT meta_ws)
#else
publicdef INT pf_win_close (window_id, seg_list_ptr, meta_ws)
INT window_id;
lh_listhead *seg_list_ptr;
INT meta_ws;
#endif
    {
    INT count;
    INT i;
    INT seg_id;
    INT length;
    INT priority;
    INT status;
    
    count = lh_count (seg_list_ptr);
    if (count > 0)
        {
        for (i = 0; i < count; i++)
            {
            if (i EQUALS 0)
                lh_first (seg_list_ptr, &priority, &seg_id, &length);
            else
                lh_next (seg_list_ptr, &priority, &seg_id, &length);
            wm_gasgwk (meta_ws, seg_id);
            }
        }
    status = lh_free (seg_list_ptr);
    wmwclose (window_id);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write all segment ids in this window or group                                 */
/*    to linked list.                                                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_all_segs(lh_listhead *seg_list_ptr);                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    seg_list_ptr    -(LH_LISTHEAD *)                                              */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_all_segs (lh_listhead *seg_list_ptr)
#else
publicdef INT pf_all_segs (seg_list_ptr)
lh_listhead *seg_list_ptr;
#endif
    {
    INT status;
    INT count;
    LISTHEAD seg_list;
    INT j;
    INT seg_id;
    GKS_INT errind, vis, high, det;
    GKS_REAL sgpr;
    INT priority;
    INT  class ;
    CHAR db_name[WM_DBS_STR_SIZE];
    CHAR usr_name[WM_USR_STR_SIZE];
    GKS_REAL segtm[2][3];
    
    llinit (&seg_list, 0, sizeof(INT), 0);
    wmsinqlist (WM_CLASS_SEGMENT, &seg_list);
    count = llcount (&seg_list);
    if (count > 0)
        {
        for (j = 0; j < count; j++)
            {
            if (j EQUALS 0)
                llfirst (&seg_list, &seg_id);
            else
                llnext (&seg_list, &seg_id);
            wmoinq (seg_id, &class , db_name, usr_name);
            if (class  != WM_CLASS_BACKGROUND && class  != WM_CLASS_BORDER)
                {
                wm_gqsga (seg_id, &errind, segtm, &vis, &high, &sgpr, &det);
                if (vis == GVISI)
                    {
                    priority = (INT)(10000000 * sgpr);
                    status = lh_add (seg_list_ptr, &priority, &seg_id, sizeof(INT));
                    }
                }
            }
        }
    llfree (&seg_list);
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write the indicated segment id in this window                                 */
/*    or group to linked list.                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_one_seg(INT seg_id,lh_listhead *seg_list_ptr);               */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    seg_id          -(INT)                                                        */
/*    seg_list_ptr    -(LH_LISTHEAD *)                                              */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_one_seg (INT seg_id, lh_listhead *seg_list_ptr)
#else
publicdef INT pf_one_seg (seg_id, seg_list_ptr)
INT seg_id;
lh_listhead *seg_list_ptr;
#endif
    {
    INT status;
    GKS_INT errind, vis, high, det;
    GKS_REAL sgpr;
    INT priority;
    GKS_REAL segtm[2][3];
    
    wm_gqsga (seg_id, &errind, segtm, &vis, &high, &sgpr, &det);
    if (vis == GVISI)
        {
        priority = (INT)(10000000 * sgpr);
        status = lh_add (seg_list_ptr, &priority, &seg_id, sizeof(INT));
        }
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write all segments from all groups and sub-groups                             */
/*    in this window or group to linked list.                                       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_all_groups(lh_listhead *seg_list_ptr);                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    seg_list_ptr    -(LH_LISTHEAD *)                                              */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_all_groups (lh_listhead *seg_list_ptr)
#else
publicdef INT pf_all_groups (seg_list_ptr)
lh_listhead *seg_list_ptr;
#endif
    {
    LISTHEAD gro_list;
    INT count;
    INT i;
    INT gro_id;
    
    llinit (&gro_list, 0, sizeof(INT), 0);
    wmginqlist (WM_CLASS_GROUP, &gro_list);
    count = llcount (&gro_list);
    if (count > 0)
        {
        for (i = 0; i < count; i++)
            {
            if (i EQUALS 0)
                llfirst (&gro_list, &gro_id);
            else
                llnext (&gro_list, &gro_id);
            wmgopen (gro_id);
            pf_all_groups (seg_list_ptr);
            pf_all_segs (seg_list_ptr);
            wmgclose (gro_id);
            }
        }
    llfree (&gro_list);
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write to linked list all segments from the indicated                          */
/*    group and its sub-groups                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_one_group(INT gro_id,lh_listhead *seg_list_ptr);             */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    gro_id          -(INT)                                                        */
/*    seg_list_ptr    -(LH_LISTHEAD *)                                              */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_one_group (INT gro_id, lh_listhead *seg_list_ptr)
#else
publicdef INT pf_one_group (gro_id, seg_list_ptr)
INT gro_id;
lh_listhead *seg_list_ptr;
#endif
    {
    wmgopen (gro_id);
    pf_all_groups (seg_list_ptr);
    pf_all_segs (seg_list_ptr);
    wmgclose (gro_id);
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_win_to_meta(INT meta_ws,INT window_id);                      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    meta_ws         -(INT)                                                        */
/*    window_id       -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_win_to_meta (INT meta_ws, INT window_id)
#else
publicdef INT pf_win_to_meta (meta_ws, window_id)
INT meta_ws;
INT window_id;
#endif
    {
    INT status;
    lh_listhead *seg_list_ptr;
    
    /********************************************************************************/
    
    status = pf_win_init (meta_ws, window_id, &seg_list_ptr);
    
    status = pf_all_segs (seg_list_ptr);
    
    status = pf_all_groups (seg_list_ptr);
    
    status = pf_win_close (window_id, seg_list_ptr, meta_ws);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Write one group for a given window to a GKS metafile.                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT pf_ovl_to_meta(INT meta_ws,INT window_id,INT tnr_id,DOUBLE xscale,  */
/*        DOUBLE yscale,DOUBLE xoffset,DOUBLE yoffset,INT subgroup_id,INT group_id);*/
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    meat_ws         -(INT)                                                        */
/*    window_id       -(INT)                                                        */
/*    tnr_id          -(INT)                                                        */
/*    xscale          -(DOUBLE)                                                     */
/*    yscale          -(DOUBLE)                                                     */
/*    xoffset         -(DOUBLE)                                                     */
/*    yoffset         -(DOUBLE)                                                     */
/*    subgroup_id     -(INT)                                                        */
/*    group_id        -(INT)                                                        */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT pf_ovl_to_meta (INT meta_ws, INT window_id, INT tnr_id, DOUBLE xscale,
                              DOUBLE yscale, DOUBLE xoffset, DOUBLE yoffset,
                              INT subgroup_id, INT group_id)
#else
publicdef INT pf_ovl_to_meta (meta_ws, window_id, tnr_id, xscale, yscale, xoffset,
                              yoffset, subgroup_id, group_id)
INT meta_ws;
INT window_id;
INT tnr_id;
DOUBLE xscale;
DOUBLE yscale;
DOUBLE xoffset;
DOUBLE yoffset;
INT subgroup_id;
INT group_id;
#endif
    {
    INT status;
    lh_listhead *seg_list_ptr;
    
    /********************************************************************************/
    
    status = pf_win_init (meta_ws, window_id, &seg_list_ptr);
    
    if (subgroup_id == 0)
        status = pf_one_group (group_id, seg_list_ptr);
    else
        {
        wmgopen (group_id);
        pf_one_group (subgroup_id, seg_list_ptr);
        wmgclose (group_id);
        }
    status = pf_win_close (window_id, seg_list_ptr, meta_ws);
    
    return SUCCESS;
    }
/* END:                                                                             */
