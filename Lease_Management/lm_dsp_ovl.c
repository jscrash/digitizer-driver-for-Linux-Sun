/* DEC/CMS REPLACEMENT HISTORY, Element LM_DSP_OVL.C */
/* *5    11-SEP-1991 13:40:49 JULIAN "(SPR 0) add comments" */
/* *4     9-AUG-1991 12:46:28 JULIAN "(SPR 3595) fixed memory allocation problem" */
/* *3     6-AUG-1991 16:26:05 JULIAN "(SPR 3595) removed parenthesis from sql stmt" */
/* *2    18-MAR-1991 18:08:41 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:53:40 JULIAN "0" */
/* DEC/CMS REPLACEMENT HISTORY, Element LM_DSP_OVL.C */
#include "esi_am.h"

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_LM_H
#include "esi_lm.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to display lease management overlay

Prototype:
    publicdef INT lm_display_overlay(LM_STRUCT *plm);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    plm		-(LM_STRUCT *) Lease overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT lm_display_overlay(LM_STRUCT *plm)
#else
publicdef INT lm_display_overlay(plm)
LM_STRUCT *plm;
#endif
    {
    INT status;
    INT iline;                /* number of lines to process */
    COORD xmin, xmax, ymin, ymax;
    NLIST_HEADER slist = (NLIST_HEADER)0;   /* temporary select list pointer */
    NLIST_HEADER process_list = (NLIST_HEADER)0;    /* final list to process */
    CHAR *select_phrases;
    CHAR text[256];
    BOOL free_phrase = FALSE;
    INT n_leases,len;
    PROJECTION_ID map_projection;
    PROJECTION_STRUCTURE *geo_data;
    PROJECTION_STRUCTURE *map_data;
    MAP_STRUCTURE       *pmap;    
    PROJECT_NAME project_name;

    /*--------------------------------------------------------------------*/
    /* nlist value list                                                   */
    /* The following values are set by the SELECT SLIST SEISMIC SUBSYSTEM */
    /* and must not be dicked with                                        */
    /*--------------------------------------------------------------------*/
    
    /**************************************************************************/
    
			     /*	get map structure for map corners	    */
			     /*	coordinates				    */
    status = mp_get_current_map(&pmap);
    if(status != SUCCESS)
	return status;

	/* get project name for oracle statement */
    qs_inq_c(QS_PROJECT_NAME,project_name,(INT *)0);

    status = SUCCESS;
    slist = (NLIST_HEADER)0;
    
    /* * IF SELECT LIST PRESENT, READ IT * */
    /* ----------------------------------- */
    
    if (plm->select_list != (CHAR *)0)
	{
	status = sl_read_list (LEASE_DATA_TYPE, plm->select_list,&slist);
	if(status != SUCCESS)
	    {
	    am_message(AM_ALERT,mg_message (status));
	    return status;
	    }
	} 

/* * INTERSECT WITH MAP WINDOW AND SELECT PHRASE * */ 
/* ----------------------------------------------- */
    xmin = pmap->lower_left_xy.x;
    ymin = pmap->lower_left_xy.y;
    xmax = pmap->upper_right_xy.x;
    ymax = pmap->upper_right_xy.y;
    qs_inq_c (QS_MAP_PROJECTION, map_projection, (INT *)0);
    if(ARE_DIFFERENT("GEODETIC",map_projection)) 
	{	
	ct_get_projection ("GEODETIC", &geo_data);
 	ct_get_projection (map_projection, &map_data);
        ct_transform_point (xmin, ymin, map_data, 
			    &xmin, &ymin, geo_data);
	ct_transform_point (xmax, ymax, map_data,
			    &xmax, &ymax, geo_data);
        }
    
    /* get new select list */
    sprintf (text, 
    "LEASE_ID IN \
(SELECT LEASE_ID FROM %s.LEASE_POLYGONS \
WHERE LATITUDE BETWEEN %.2f AND %.2f AND \
      LONGITUDE BETWEEN %.2f AND %.2f) ORDER BY LEASE_ID",
project_name,ymin,ymax,xmin,xmax); 
    

    if (IS_STRING (plm->select_by_phrase))
        {
			     /*	allocate new space for select by phrase.
			     	When ov_chk_where is called, it will throw
			     	away the old string and create a new one.
			     	Plm->select_by_phrase is just a pointer to
			     	one of the structure in map def in which we
			     	do not want to discard the contents of the
			     	string					    */

        len = strlen (plm->select_by_phrase);
        select_phrases = 
            (CHAR *)tc_alloc(sizeof(CHAR)*(len+strlen(text)+10));
        sprintf (select_phrases, "%s AND %s",
                 plm->select_by_phrase, text);
        free_phrase = TRUE;
        }
    else
        {
        select_phrases = text;
        }

    status = sl_select (slist, "LEASE", select_phrases, &process_list,
 	&n_leases); 

#if NEED_TO_FREE 
    if (free_phrase) 
	{
        tc_free (select_phrases);
        }
#endif
    
    /* * CHECK FOR GOOD SELECT * */
    /* ------------------------- */
    
    if (status != SUCCESS OR n_leases == 0)
	goto error;

    /* * COPY ANNOTATION PARAM INTO TCP, * */
    /* * SINCE IT'S A MULTIPLE PARAMETER * */ 
    /* ----------------------------------- */	

    if(plm->annotate != (CHAR *)0)
	{
	plm->annotate_tcp = (TCP)0;
	plm->annotate_tcp = ts_token(plm->annotate,";");
	}

    /* * DRAW POLYGONS OF LEASES AND THE ANNOTATION * */
    /* ---------------------------------------------- */

    status = lm_draw_polygons(process_list,plm);
    if(status != SUCCESS)
	goto error;
    
    return status;
error:

    if (slist != (NLIST_HEADER)0)
	{
        nl_free_nlist (slist);
        }

    if (process_list IS_NOT_EQUAL_TO (NLIST_HEADER)0)
	{
        nl_free_nlist (process_list);
        }

    return status;
    }
/* END:     */
