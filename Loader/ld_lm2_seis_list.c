/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_SEIS_LIST.C */
/* *2     1-MAY-1991 22:14:52 KEE "(SPR -1) Landmark Import & Export" */
/* *1     1-MAY-1991 21:25:59 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_SEIS_LIST.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* ************************************************************************

   NAME: LD_LM2_SEIS_LIST.C

   AUTHOR: Rod Hanks
   DATE:   May 4th, 1990
   DESCRIPTION: Given the select phrase and the select list, get an nlist
		of line ids which should be output.

   ************************************************************************ */


#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_SL_H
#include "esi_sl.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#if USE_PROTOTYPES
publicdef VOID ld_lm2_seis_list(CHAR *selectPhrase, CHAR *selectList, 
		   DOUBLE lower_left_x, DOUBLE lower_left_y, 
		   DOUBLE upper_right_x, DOUBLE upper_right_y,
		   INT *lineCount, NLIST_HEADER *lineList)
#else
publicdef VOID ld_lm2_seis_list(selectPhrase, selectList, 
				  lower_left_x, lower_left_y,
				  upper_right_x, upper_right_y,
				  lineCount, lineList)
CHAR        *selectPhrase;
CHAR        *selectList;
DOUBLE       lower_left_x;
DOUBLE       lower_left_y;
DOUBLE       upper_right_x;
DOUBLE       upper_right_y;
INT         *lineCount;
NLIST_HEADER *lineList;
#endif
{
    INT status;
    INT len = 0;
    DOUBLE xmin,xmax,ymin,ymax;
    NLIST_HEADER slist = (NLIST_HEADER) 0;
    CHAR *select_phrases;
    CHAR text[128];
    BOOL free_phrase = FALSE;

/**************************************************************************/


    slist  = (NLIST_HEADER) 0;

    if (selectList != (CHAR *) 0)
        {
        if (IS_STRING(selectList))
	    {
	    sl_read_list(SEISMIC_DATA_TYPE, selectList, &slist);
	    }
	}

/* * INTERSECT WITH MAP WINDOW AND SELECT PHRASE * */
/* ----------------------------------------------- */

    xmin = lower_left_x;
    ymin = lower_left_y;
    ct_project_xy(xmin,ymin,&xmin,&ymin);

    xmax = upper_right_x;
    ymax = upper_right_y;
    ct_project_xy(xmax,ymax,&xmax,&ymax);

    /* get new select list */
    sprintf(text, "UPPER_RIGHT_X >= %.2f AND UPPER_RIGHT_Y >= %.2f \
AND LOWER_LEFT_X <= %.2f AND LOWER_LEFT_Y <= %.2f", xmin, ymin, xmax, ymax);

    if (IS_STRING(selectPhrase))
        {
        len = strlen(selectPhrase);
        select_phrases = (CHAR *)TALLOC(len + strlen(text) + 10);
        sprintf(select_phrases,"%s AND (%s)",selectPhrase,text);
        free_phrase = TRUE;
        }
    else
        {
        select_phrases = text;
        }

    status = sl_select(slist,"SEIS_LINE_HDR",select_phrases,
				lineList,lineCount);
    if (free_phrase)
        {
        tc_free(select_phrases);    
        }
    nl_free_nlist(slist);
    return;
}

