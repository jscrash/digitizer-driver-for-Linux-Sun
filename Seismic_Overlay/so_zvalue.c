/* DEC/CMS REPLACEMENT HISTORY, Element SO_ZVALUE.C*/
/* *21    8-JAN-1992 15:55:20 JTM "(SPR 0) Change from BHP. Added #include 'esi_ct.h'."*/
/* *20   22-NOV-1991 15:19:39 MING "(SPR 5046) add ct_map_xy before writing x,y values to the scatter file"*/
/* *19    6-JUN-1991 17:52:58 MING "(SPR 0) change seismic time to seismic surface"*/
/*  14B2  3-OCT-1990 11:53:22 GILLESPIE "Change file type to stream"*/
/* *18   28-SEP-1990 10:43:24 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  14B1 24-SEP-1990 17:27:38 GILLESPIE "Beta Deltas"*/
/* *17   29-AUG-1990 11:38:06 GILLESPIE "(SPR 1) CP Deltas"*/
/* *16   27-AUG-1990 14:09:59 JULIAN "(SPR 0) bug fixes from ernie"*/
/* *15   20-AUG-1990 17:11:34 KEE "(SPR -1) fixing compile error at sun"*/
/* *14   26-JUL-1990 17:22:57 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *13   26-JUL-1990 11:36:20 VINCE "(SPR 5644) Fixed compile errors introduced with the header changes."*/
/* *12   25-JUL-1990 20:06:58 VINCE "(SPR 5644) Header Standardization"*/
/* *11   29-JUN-1990 19:04:38 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *10   23-FEB-1990 13:18:43 GILLESPIE "(SPR 1) Change horizon_nlist to nlist in seis_surface structure"*/
/* *9    31-OCT-1989 09:34:44 GILLESPIE "(SPR 1) Change castings to quiet compiler"*/
/* *8    20-SEP-1989 16:06:44 GORDON "(SPR 200) Get in sync with Gulf"*/
/* *7    19-SEP-1989 13:38:49 GILLESPIE "(SPR 100) GULF MODS"*/
/* *6    14-SEP-1989 18:31:15 GORDON "(SPR -1) checkin from gulf"*/
/* *5     6-SEP-1989 09:30:19 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *4    25-AUG-1989 17:15:18 CONROY "(SPR 9999) rework to make work"*/
/* *3    30-JUL-1989 00:19:39 GILLESPIE "(SPR 1) Fix some minor casting problems"*/
/* *2    25-JUL-1989 20:08:23 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ZVALUE.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ZVALUE.C                                 */
/* *17   29-AUG-1990 11:38:06 GILLESPIE "(SPR 1) CP Deltas"                         */
/* *16   27-AUG-1990 14:09:59 JULIAN "(SPR 0) bug fixes from ernie"                  */
/* *15   20-AUG-1990 17:11:34 KEE "(SPR -1) fixing compile error at sun"            */
/* *14   26-JUL-1990 17:22:57 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"   */
/* *13   26-JUL-1990 11:36:20 VINCE "(SPR 5644) Fixed compile errors introduced with the header changes."   */
/* *12   25-JUL-1990 20:06:58 VINCE "(SPR 5644) Header Standardization"             */
/* *11   29-JUN-1990 19:04:38 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"  */
/* *10   23-FEB-1990 13:18:43 GILLESPIE "(SPR 1) Change horizon_nlist to nlist in seis_surface structure"   */
/* *9    31-OCT-1989 09:34:44 GILLESPIE "(SPR 1) Change castings to quiet compiler" */
/* *8    20-SEP-1989 16:06:44 GORDON "(SPR 200) Get in sync with Gulf"              */
/* *7    19-SEP-1989 13:38:49 GILLESPIE "(SPR 100) GULF MODS"                       */
/* *6    14-SEP-1989 18:31:15 GORDON "(SPR -1) checkin from gulf"                   */
/* *5     6-SEP-1989 09:30:19 GORDON "(SPR 200) Gulf changes (August trip)"         */
/* *4    25-AUG-1989 17:15:18 CONROY "(SPR 9999) rework to make work"               */
/* *3    30-JUL-1989 00:19:39 GILLESPIE "(SPR 1) Fix some minor casting problems"   */
/* *2    25-JUL-1989 20:08:23 GILLESPIE "(SPR 50) Seismic system revamp"            */
/* *1    19-JUN-1989 13:27:38 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_ZVALUE.C                                 */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_SE_H
#include "esi_se.h"
#endif

#ifndef ESI_SO_H
#include "esi_so.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#if USE_PROTOTYPES
static INT soz_get_time (NLIST_HEADER nlist, CDP cdp, UINT *crnt_stroke,
                             CDP **cdp_array, DOUBLE **time_array, UINT *npoints,
                             DOUBLE *time);
#else
static INT soz_get_time ();
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Function to compute Z value for a cdp.                                        */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT so_zvalue(SO_STRUCT *pso,CDP cdp,CDP cdp2,                      */
/*        COORD x, COORD y, DOUBLE *zvalue);                                        */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT soz_get_time(NLIST_HEADER nlist, CDP cdp, UINT *crnt_stroke,   */
/*        CDP **cdp_array, DOUBLE **time_array, UINT *npoints, DOUBLE *time);       */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT so_zvalue(SO_STRUCT *pso,CDP cdp,CDP cdp2,                      */
/*        COORD x, COORD y, DOUBLE *zvalue);                                        */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pso             -(SO_STRUCT *) Seismic overlay structure.                     */
/*    cdp             -(CDP)                                                        */
/*    cdp2            -(CDP)                                                        */
/*    x               -(COORD)                                                      */
/*    y               -(COORD)                                                      */
/*    zvalue          -(DOUBLE *)                                                   */
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
publicdef INT so_zvalue (SO_STRUCT *pso, CDP cdp, CDP cdp2, COORD x, COORD y,
                         DOUBLE *zvalue)
#else
publicdef INT so_zvalue (pso, cdp, cdp2, x, y, zvalue)
SO_STRUCT *pso;
CDP cdp;
CDP cdp2;
COORD x, y;
DOUBLE *zvalue;
#endif
    {
    INT status;
    static SEIS_SURFACE_NAME horizon1 = "";
    static INT version1 = -1;
    static INT line_no1 = -1;
    static SEIS_SURFACE_NAME horizon2 = "";
    static INT version2 = -1;
    static INT line_no2 = -1;
    static SEIS_LINE_ID line = 0;
    static ANALYST_NAME source = "";
    
    static UINT crnt_stroke1 = 0;
    static UINT crnt_stroke2 = 0;
    
    static CDP *cdp1_array = (CDP *)0;
    static CDP *cdp2_array = (CDP *)0;
    static DOUBLE *time1_array = (DOUBLE *)0;
    static DOUBLE *time2_array = (DOUBLE *)0;
    static UINT npts1 = 0;
    static UINT npts2 = 0;
    
    DOUBLE time1;
    DOUBLE time2;
    NLIST_HEADER nlist1;
    NLIST_HEADER nlist2;
    CHAR *dum_hor1;
    CHAR *dum_hor2;
    INT dum_vers1;
    INT dum_vers2;
    INT dum_line1;
    INT dum_line2;
    BOOL get_two;
    BOOL check1, check2;
    
    /********************************************************************************/
    /* * INITIALIZE DUMMY VARIABLES *                                               */
    /* ------------------------------                                               */
    
    switch (pso->ov_type)
        {
    case SO_SEISMIC:
    case SO_SEISMIC_DEPTH:
    case SO_SEISMIC_DATA:
        nlist1 = pso->seismic_surface.nlist;
        nlist2 = (NLIST_HEADER)0;
        
        dum_hor1 = pso->seismic_surface.name;
        dum_hor2 = (CHAR *)0;
        
        dum_vers1 = pso->seismic_surface.version;
        dum_vers2 = -1;
        get_two = FALSE;
        break;
        
    case SO_SEISMIC_ISOCHRON:
    case SO_SEISMIC_ISOPACH:
    case SO_SEISMIC_MISTIE:
        nlist1 = pso->seismic_interval.top_horizon.nlist;
        nlist2 = pso->seismic_interval.base_horizon.nlist;
        
        dum_hor1 = pso->seismic_interval.top_horizon.name;
        dum_hor2 = pso->seismic_interval.base_horizon.name;
        
        dum_vers1 = pso->seismic_interval.top_horizon.version;
        dum_vers2 = pso->seismic_interval.base_horizon.version;
        get_two = TRUE;
        break;
        }
    /*  set the boolean statement different for                                     */
    /*  seismic mistie                                                              */
    if (pso->ov_type EQUALS SO_SEISMIC_MISTIE)
        {
        dum_line1 = pso->seismic_interval.top_horizon.line.id;
        dum_line2 = pso->seismic_interval.base_horizon.line.id;
        check1 = version1 != dum_vers1 OR line_no1 != dum_line1;
        check2 = version2 != dum_vers2 OR line_no2 != dum_line2;
        }
    else
        {
        check1 = version1 != dum_vers1;
        check2 = version2 != dum_vers2;
        }
    /* * RE-INIT EVERYTHING IF A NEW LINE OR NEW SOURCE *                           */
    /* --------------------------------------------------                           */
    if (line != pso->current_line.id OR (ARE_DIFFERENT (source,
                                                        pso->seismic_generic.source)))
        {
        
        /* LINE AND SOURCE                                                          */
        line = pso->current_line.id;
        strcpy ((char *)source, (char *)pso->seismic_generic.source);
        
        /* HORIZONS                                                                 */
        if (IS_STRING (dum_hor1))
            {
            strcpy ((char *)horizon1, (char *)dum_hor1);
            }
        else
            {
            strcpy ((char *)horizon1, "");
            }
        if (IS_STRING (dum_hor2))
            {
            strcpy ((char *)horizon2, (char *)dum_hor2);
            }
        else
            {
            strcpy ((char *)horizon2, "");
            }
        /* VERSIONS                                                                 */
        version1 = dum_vers1;
        version2 = dum_vers2;
        
        /* CURRENT STROKES                                                          */
        crnt_stroke1 = 0;
        crnt_stroke2 = 0;
        
        if (pso->ov_type EQUALS SO_SEISMIC_MISTIE)
            {
            line_no1 = dum_line1;
            line_no2 = dum_line2;
            }
        /* CDP ARRAYS                                                               */
        if (cdp1_array != (CDP *)0)
            {
            tc_free ((VOIDPTR)cdp1_array);
            cdp1_array = (CDP *)0;
            }
        if (cdp2_array != (CDP *)0)
            {
            tc_free ((VOIDPTR)cdp2_array);
            cdp2_array = (CDP *)0;
            }
        /* TIME ARRAYS                                                              */
        if (time1_array != (DOUBLE *)0)
            {
            tc_free ((VOIDPTR)time1_array);
            time1_array = (DOUBLE *)0;
            }
        if (time2_array != (DOUBLE *)0)
            {
            tc_free ((VOIDPTR)time2_array);
            time2_array = (DOUBLE *)0;
            }
        /* NPOINTS                                                                  */
        npts1 = 0;
        npts2 = 0;
        }
    /* * ELSE RE-INIT EACH HORIZON AS NEEDED *                                      */
    /* ---------------------------------------                                      */
    
    else
        {
        if (check1 OR (dum_hor1 EQUALS (CHAR *)0 AND IS_STRING (
                                                            horizon1)) 
            OR (dum_hor1 != (CHAR *)0 AND ARE_DIFFERENT (horizon1, dum_hor1)))
            {
            if (IS_STRING (dum_hor1))
                {
                strcpy ((char *)horizon1, (char *)dum_hor1);
                }
            else
                {
                strcpy ((char *)horizon1, "");
                }
            if (pso->ov_type EQUALS SO_SEISMIC_MISTIE)
                line_no1 = dum_line1;
            version1 = dum_vers1;
            crnt_stroke1 = 0;
            if (cdp1_array != (CDP *)0)
                {
                tc_free ((VOIDPTR)cdp1_array);
                cdp1_array = (CDP *)0;
                }
            if (time1_array != (DOUBLE *)0)
                {
                tc_free ((VOIDPTR)time1_array);
                time1_array = (DOUBLE *)0;
                }
            npts1 = 0;
            }
        if (check2 OR (dum_hor2 EQUALS (CHAR *)0 AND IS_STRING (
                                                            horizon2)) 
            OR (dum_hor2 != (CHAR *)0 AND ARE_DIFFERENT (horizon2, dum_hor2)))
            {
            if (IS_STRING (dum_hor2))
                {
                strcpy ((char *)horizon2, (char *)dum_hor2);
                }
            else
                {
                strcpy ((char *)horizon2, "");
                }
            if (pso->ov_type EQUALS SO_SEISMIC_MISTIE)
                line_no2 = dum_line2;
            version2 = dum_vers2;
            crnt_stroke2 = 0;
            if (cdp2_array != (CDP *)0)
                {
                tc_free ((VOIDPTR)cdp2_array);
                cdp2_array = (CDP *)0;
                }
            if (time2_array != (DOUBLE *)0)
                {
                tc_free ((VOIDPTR)time2_array);
                time2_array = (DOUBLE *)0;
                }
            npts2 = 0;
            }
        }
    /* * PROCESS TO GET ZVALUE *                                                    */
    /* -------------------------                                                    */
    
    *zvalue = SO_NULL;
    
    /* GET FIRST TIME                                                               */
    
    if (soz_get_time (nlist1, cdp, &crnt_stroke1, &cdp1_array, &time1_array, &npts1,
                      &time1) != SUCCESS)
        {
        return FAIL;
        }
    /* GET SECOND TIME, IF NEEDED                                                   */
    if (get_two)
        {
        if (soz_get_time (nlist2, cdp2, &crnt_stroke2, &cdp2_array, &time2_array,
                          &npts2, &time2) != SUCCESS)
            {
            return FAIL;
            }
        }
    /* * SET RETURN VALUE *                                                         */
    /* --------------------                                                         */
    
    switch (pso->ov_type)
        {
    case SO_SEISMIC_DATA:
	/* I am not sure if I need to break down into diff surface type or not */
        *zvalue = time1;
        status = SUCCESS;
        break;

    case SO_SEISMIC:
        *zvalue = time1;
        status = SUCCESS;
        break;
        
    case SO_SEISMIC_ISOCHRON:
    case SO_SEISMIC_MISTIE:
        *zvalue = time2 - time1;
        status = SUCCESS;
        break;
        
    case SO_SEISMIC_DEPTH:
        status = so_depth_convert (&pso->grid, x, y, pso->datum, time1, zvalue);
        break;
        
    case SO_SEISMIC_ISOPACH:
        status = so_depth_convert_isopach (&pso->grid, x, y, pso->datum, time1, time2,
                                           zvalue);
        break;
        }
    /* * WRITE TO SCATTER FILE IF DESIRED AND DATA OK *                             */
    /* ------------------------------------------------                             */
    
    if (*zvalue != SO_NULL AND pso->seismic_generic.do_scatter)
        {
        FLOAT temp_xfloat;
        FLOAT temp_yfloat;
        FLOAT temp_zfloat = *zvalue;
        DOUBLE xout;
        DOUBLE yout;

        /* ming:11/22/91:CASE 5046 */
        /* x, y should be transformed to map projection before writing */
        /* to the scatter file */
		   
        ct_map_xy (x, y, &xout, &yout);
        temp_xfloat = xout;
        temp_yfloat = yout;
        
        ov_write_scatter (pso->seismic_generic.scatter, &temp_xfloat, &temp_yfloat,
                          &temp_zfloat, 1);
        }
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT soz_get_time(NLIST_HEADER nlist, CDP cdp, UINT *crnt_stroke,   */
/*        CDP **cdp_array, DOUBLE **time_array, UINT *npoints, DOUBLE *time);       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    nlist           -(NLIST_HEADER)                                               */
/*    cdp             -(CDP)                                                        */
/*    crnt_stroke     -(UINT *)                                                     */
/*    cdp_array       -(CDP **)                                                     */
/*    time_array      -(DOUBLE **)                                                  */
/*    npoints         -(UINT *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <so_zvalue.c>                                                      */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT soz_get_time (NLIST_HEADER nlist, CDP cdp, UINT *crnt_stroke,
                             CDP **cdp_array, DOUBLE **time_array, UINT *npoints,
                             DOUBLE *time)
#else
static INT soz_get_time (nlist, cdp, crnt_stroke, cdp_array, time_array, npoints,
                             time)
NLIST_HEADER nlist;
CDP cdp;
UINT *crnt_stroke;
CDP **cdp_array;
DOUBLE **time_array;
UINT *npoints;
DOUBLE *time;
#endif
    {
    INT status = FAIL;
    UINT nstrokes;
    CDP stroke_min_cdp;
    CDP stroke_max_cdp;
    INT i, j;
    DOUBLE diff;
    UINT d_list[2];
    VOIDPTR v_list[2];
    INT units[2];
    size_t d_size[2];
    INT d_type[2];
    DOUBLE *local_time;
    CDP *local_cdp;
    
    /********************************************************************************/
    
    d_list[0] = 1;
    units[0] = 0;
    d_list[1] = 2;
    units[1] = 0;
    d_size[0] = sizeof(INT);
    d_type[0] = INT_TYPE;
    d_size[1] = sizeof(DOUBLE);
    d_type[1] = FLOAT_TYPE;
    
    /* * GET NUMBER OF STROKES TO SEARCH *                                          */
    /* -----------------------------------                                          */
    
    nl_inq_nlist_int (nlist, NL_NLIST_NSTROKES, (INT *)&nstrokes);
    
    /* * LOOP THRU STROKES TO FIND CDP *                                            */
    /* ---------------------------------                                            */
    
    for (i = 1; i <= nstrokes; i++)
        {
        nl_set_current_point (nlist, 1, i);
        
        /* CHECK AGAINST MINIMUM                                                    */
        /* NOTE: BEING LESS THAN THE STROKE MINIMUM IMPLIES                         */
        /* THE CDP IS EITHER BEFORE THE FIRST STROKE OR IS                          */
        /* BETWEEN STROKES. IN EITHER CASE IT HAS NO TIME VALUE                     */
        
        v_list[0] = (VOIDPTR)(&stroke_min_cdp);
        nl_inq_stroke_info (nlist, i, NL_STROKE_MIN, 1, d_list, v_list);
        if (cdp < stroke_min_cdp)
            {
            return status;
            }
        /* CHECK AGAINST MAXIMUM                                                    */
        /* NOTE: IF CDP IS GREATER THAN MAXIMUM, IMPLIES IT MUST                    */
        /* BE IN A LATER STROKE, IF IN THE NLIST AT ALL                             */
        
        v_list[0] = (VOIDPTR)(&stroke_max_cdp);
        nl_inq_stroke_info (nlist, i, NL_STROKE_MAX, 1, d_list, v_list);
        if (cdp <= stroke_max_cdp)
            {
            break;
            }
        }
    /* * CHECK IF WITHIN A STROKE *                                                 */
    /* ----------------------------                                                 */
    
    if (i > nstrokes)
        {
        return status;
        }
    /* * CDP IS IN THIS STROKE, SEE IF ALREADY HAVE STROKE POINTS *                 */
    /* ------------------------------------------------------------                 */
    
    if (i != *crnt_stroke)
        {
        if ((*cdp_array) != (CDP *)0)
            {
            tc_free ((VOIDPTR)*cdp_array);
            (*cdp_array) = (CDP *)0;
            }
        if ((*time_array) != (DOUBLE *)0)
            {
            tc_free ((VOIDPTR)*time_array);
            (*time_array) = (DOUBLE *)0;
            }
        /* GET STROKE POINTS                                                        */
        *crnt_stroke = i;
        nl_inq_stroke_int (nlist, (*crnt_stroke), NL_STROKE_NPOINTS, (INT *)npoints);
        
        (*cdp_array) = (CDP *) tc_zalloc ((*npoints) * sizeof(CDP));
        (*time_array) = (DOUBLE *) tc_zalloc ((*npoints) * sizeof(DOUBLE));
        
        v_list[0] = (VOIDPTR)(*cdp_array);
        v_list[1] = (VOIDPTR)(*time_array);
        nl_get_points_convert (nlist, *npoints, 2, d_list, v_list, units, d_size,
                               d_type, npoints);
        }
    /* * FIND POINT *                                                               */
    /* --------------                                                               */
    
    local_time = (*time_array);
    local_cdp = (*cdp_array);
    for (i = 0; i < *npoints; i++)
        {
        if (cdp EQUALS local_cdp[i])
            {
            status = SUCCESS;
            *time = local_time[i];
            break;
            }
        if (cdp < local_cdp[i])
            {
            status = SUCCESS;
            j = i - 1;
            diff = (DOUBLE)(cdp - local_cdp[j]) / (local_cdp[i] - local_cdp[j]);
            *time = diff * (local_time[i] - local_time[j]) + local_time[j];
            break;
            }
        }
    return status;
    }
/* END:                                                                             */
