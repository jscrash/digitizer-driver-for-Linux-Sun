/* DEC/CMS REPLACEMENT HISTORY, Element SEF_LINE_STOR.C*/
/* *5    17-AUG-1990 22:14:55 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    31-OCT-1989 10:51:47 JULIAN "(SPR 5039) change sl files to se files"*/
/* *3    30-OCT-1989 14:36:29 JULIAN "(SPR 5039) Seismic Select List Mods"*/
/* *2    25-JUL-1989 19:39:55 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:23:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SEF_LINE_STOR.C*/
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
#include "esi_se.h"

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#define  MAX_CARAC    31

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sef_line_stor(CHAR *curr_seismic_list, INT  *nb_lines, CHAR ***line_buf, 
        CHAR **charbuf) ;

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    curr_seismic_list   -(CHAR *)
    nb_lines            -(INT *) There is a total of nb_lines in the buffer.
    line_buf            -(CHAR ***) Line_buf is the buffer containing the line_names 
                            for which we want the intersections, and
                            each line_name is on nbw_li words.
    charbuf             -(CHAR **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sef_line_stor (CHAR *curr_seismic_list, INT *nb_lines, CHAR ***line_buf,
                             CHAR **charbuf)
#else
publicdef INT sef_line_stor (curr_seismic_list, nb_lines, line_buf, charbuf)
CHAR *curr_seismic_list;
INT *nb_lines;
CHAR ***line_buf;
CHAR **charbuf;
#endif
    {
    INT status, i;
    CHAR *curr_point;
    NLIST_HEADER seismic_nlist;
    SL_DATA_TYPE data_type = SEISMIC_DATA_TYPE;
    UINT ndim = 3;
    VOIDPTR value_list[3];
    UINT dim_list[3];
    UINT nb_points;
    CHAR long_name[100];
    CHAR linename[100], survey[100];
    INT lineid, integer1, integer2;
    UINT nb;
    
    dim_list[0] = 1, dim_list[1] = 2, dim_list[2] = 3;
    
    *nb_lines = 0;
    
    /*  read the select list as it is stored on disk 
        linename and survey name are separated
     */
    
    status = sl_read_list (data_type, curr_seismic_list, &seismic_nlist);
    
    if (status)
        {
        return status;
        }
    /* go through the select list  to concatenate the line name and 
       survey name and build the full_line_name which is replacing the 
       line name in dimension 1  */
    
    /* inquire nb of points in the nlist */
    
    status = nl_inq_nlist_int (seismic_nlist, NL_NLIST_NPOINTS, (INT *) & nb_points);
    
    if (status)
        return status;
    
    /* allocate memory for the array of pointers  */
    
    *line_buf = (CHAR **)tc_alloc (sizeof(CHAR *) * nb_points);
    
    /* allocate and array of characters to store all the line names
       each line has a maximum of MAX_CARAC 
       the array of character pointers will point to each line name
       in this array 
    */
    
    *charbuf = (CHAR *)tc_alloc (nb_points * MAX_CARAC);
    
    /* loop on the number of points */
    
    value_list[0] = (VOIDPTR) & lineid;
    value_list[1] = (VOIDPTR) & integer1;
    value_list[2] = (VOIDPTR) & integer2;
    
    /*  update number of lines   */
    
    *nb_lines = nb_points;
    
    for (i = 0, curr_point = *charbuf; i < nb_points; curr_point += MAX_CARAC, i++)
        {
        status = nl_set_current_point (seismic_nlist, i + 1, 1);
        
        /* get current point   */
        
        status = nl_get_points (seismic_nlist, 1, ndim, dim_list, value_list, &nb);
        
        /*  create the full_line_name and put it in place of the
            old line_name */
        
        se_get_line (lineid, linename);
        se_get_survey (lineid, survey);
        status = se_cre_long_line_name (linename, survey, long_name);
        
        /* allocate memory for the line_name   */
        
        (*line_buf)[i] = curr_point;
        
        /* copy long line name in place of line name  */
        
        strcpy ((char *)(*line_buf)[i], (char *)long_name);
        
        }
    /* free the nlist  */
    
    status = nl_free_nlist (seismic_nlist);
    return SUCCESS;
    }
/* END:     */
