/* DEC/CMS REPLACEMENT HISTORY, Element WL_PAR_LAB.C*/
/* *3    24-SEP-1990 17:32:49 GILLESPIE "Beta Deltas"*/
/* *2    17-JUL-1990 12:54:28 KEE "(SPR -1) bug fixing : sqlstmt should using tc_alloc because it's being tc_free at*/
/*wl_post_wells"*/
/* *1    19-JUN-1989 13:40:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_PAR_LAB.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/





/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: WL_PARSE_LABEL.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: Function to parse the label string, analyze it and construct
		the correct sql statement to pass to oracle.

   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_WL_H
#include "esi_wl.h"
#endif

#ifndef ESI_WL_ERR_H
#include "esi_wl_err.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

/* * FUNCTION TITLE * */
/* ------------------ */

INT wl_parse_label(label,sqlstmt)

/* * ARGUEMENT LIST * */
/* ------------------ */

CHAR *label;				/* LABEL STRING TO PARSE */
CHAR **sqlstmt;				/* SQL STATEMENT TO RETURN */

/* * VARIABLE LIST * */
/* ----------------- */

{

PROGNAME("WL_PARSE_LABEL");
INT status;				/* RETURN STATUS */
CHAR *word;				/* WORD FROM LABEL STRING */
INT lab_len;				/* LENGTH OF LABEL STRING */
CHAR *dummy;				/* DUMMY CHAR POINTER */
CHAR project[31];			/* PROJECT NAME */
CHAR *select_phrase;			/* SELECT PHRASE OF LABEL STRING */
INT sel_len;				/* LENGTH OF SELECT PHRASE */
CHAR special_key[3][13];		/* SPECIAL KEYS FOR ISOPACH AND DEPTH */
BOOL thickness = FALSE;			/* THICKNESS SQL FLAG */
BOOL depth = FALSE;			/* SUBSEA DEPTH SQL FLAG */
INT num_key;				/* NUMBER OF SPECIAL KEYS TO LOOK FOR */
INT num_found;				/* NUMBER OF SPECIAL KEYS FOUND */

/* ********************************************************************** */

status = SUCCESS;

dummy = label;
ts_sto_upper(dummy);
lab_len = strlen(dummy);

if (lab_len EQUALS 0)
	{
	status = WL_NO_SELECTION_CRITERIA;
	goto finished;
	}
/* (CP) we just copy for now. The stack option is done elsewhere */

if (strncmp(dummy, "@STACK;", 7) == 0)
	{
	status = WL_DO_TCP;
	*sqlstmt = tc_alloc(strlen(dummy)+1);
	strcpy(*sqlstmt,dummy);
	goto finished;
	}

qs_inq_c(QS_PROJECT_NAME,project,(INT *) 0);
word = (CHAR *)am_allocate(AM_APPLICATION,lab_len+1);


/* * GET FIRST WORD IN LABEL STRING * */
/* ---------------------------------- */
dummy = ts_next_word(dummy,word);


/* * FULL SELECT STATEMENT * */
/* ------------------------- */

if (strcmp(word,"SELECT") EQUALS 0) 
    {
					/* GET FIELD AND ANALYZE */
    if (dummy[strlen(dummy)-1] == ';')
      dummy[strlen(dummy)-1] = ' ';
    dummy = ts_next_word(dummy,word);


/* * POSSIBLE ISOPACH SQL OR DEPTH SQL * */
/* ------------------------------------- */

    if (strcmp(word,"THICKNESS") EQUALS 0)
	{
	thickness = TRUE;
	}

    else if (strcmp(word,"SUBSEA_DEPTH") EQUALS 0)
	{
	depth = TRUE;
	}

    if (thickness OR depth)
	{
					/* CHECK FOR FROM */

	dummy = ts_next_word(dummy,word);
	if (strcmp(word,"FROM") != 0)
	    {
	    status = WL_BAD_SQL_STATEMENT;
	    goto finished;
	    }

					/* CHECK FOR WELL_TOPS */

	dummy = ts_next_word(dummy,word);
	if (strcmp(word,"WELL_TOPS") != 0)
	    {
	    if ( (word=strrchr(word,'.')) EQUALS NULL OR
					    strcmp(word,".WELL_TOPS") != 0)
		{
		status = WL_BAD_SQL_STATEMENT;
		goto finished;
		}
	    }
			
					/* CHECK FOR WHERE */

	dummy = ts_next_word(dummy,word);
	if (strcmp(word,"WHERE") != 0)
	    {
	    status = WL_BAD_SQL_STATEMENT;
	    goto finished;
	    }

					/* ANALYZE WHERE CLAUSE */

	if (thickness)
	    {
	    strcpy(special_key[0],"UPPER");
	    strcpy(special_key[1],"LOWER");
	    strcpy(special_key[2],"SOURCE");
	    num_key = 3;
	    }
	else
	    {
	    strcpy(special_key[0],"FORM_CODE");
	    strcpy(special_key[1],"SOURCE");
	    num_key = 2;
	    }

	status = wl_parse_where(dummy,special_key,num_key,&num_found);
	if (status < 0)
	    {
	    goto finished;
	    }

					/* ALLOCATE AND CREATE SQL STATEMENT */

	*sqlstmt = (CHAR *)tc_zalloc(sizeof(CHAR) * 300);
	if (thickness)
	    {
	    sprintf(*sqlstmt,
		"SELECT (Y.MD-X.MD) FROM %s.WELL_TOPS X, %s.WELL_TOPS Y \
		WHERE X.UWI= :v AND X.UWI= Y.UWI AND X.SOURCE=Y.SOURCE \
		AND X.FORM_CODE = %s AND Y.FORM_CODE = %s",
				project,project,special_key[0],special_key[1]);
	    }
	else
	    {
	    sprintf(*sqlstmt,
		"SELECT (ELEVATION-MD) 	FROM %s.WELL_TOPS X, %s.WELL_HDR Y \
		WHERE X.UWI= :v AND X.UWI= Y.UWI AND FORM_CODE = %s",
				project,project,special_key[0]);
	    }

	if (num_found EQUALS num_key)
	    {
	    select_phrase = (CHAR *)am_allocate(AM_APPLICATION,30);
	    sprintf(select_phrase," AND X.SOURCE = %s",special_key[num_key-1]);
	    strcat(*sqlstmt,select_phrase);
	    am_free(select_phrase);
	    }
	}



/* * GENERAL SQL * */
/* --------------- */

    else
	{
					/* FIND "FROM" */

	FOREVER
	    {
	    dummy = ts_next_word(dummy,word);
	    if (strcmp(word,"FROM") EQUALS 0)
		{
		break;
		}
	    if (NOT dummy)
		{
		status = WL_BAD_SQL_STATEMENT;
		goto finished;
		}
	    }

					/* GET SELECT PHRASE */

	sel_len = dummy - label;
	select_phrase = (CHAR *)am_allocate(AM_APPLICATION,sel_len+1);
	strncpy(select_phrase,label,sel_len);
	select_phrase[sel_len] = '\0';

					/* CHECK FOR ONLY ONE RETRIEVE FIELD */

	if (strrchr(select_phrase,',') != NULL)
	    {
	    status = WL_BAD_SQL_STATEMENT;
	    goto finished;
	    }


					/* MAKE SQL STATEMENT */

	*sqlstmt = (CHAR *) tc_zalloc(sizeof(CHAR) * (lab_len+50));
	ts_next_word(dummy,word);
	if (strchr(word,'.') EQUALS NULL)
	    {
	    sprintf(*sqlstmt,"%s %s.%s",select_phrase,project,dummy);
	    }
	else
	    {
	    strcpy(*sqlstmt,label);
	    }

	dummy = ts_next_word(dummy,word);
	dummy = ts_next_word(dummy,word);
	if (dummy EQUALS NULL)
	    {
	    strcat(*sqlstmt," WHERE UWI = :v");
	    }
	else if (strcmp(word,"WHERE") EQUALS 0)
	    {
	    strcat(*sqlstmt," AND UWI = :v");
	    }
	else
	    {
	    status = WL_BAD_SQL_STATEMENT;
	    goto finished;
	    }

	am_free(select_phrase);
	}

    }



/* * POSSIBLE SIMPLE SQL * */
/* ----------------------- */

else
    {
					/* CHECK FOR ONLY ONE WORD */
    if (ts_next_word(dummy,word) != NULL)
	{
	status = WL_IMPROPER_LABEL;
	goto finished;
	}

					/* ALLOCATE AND MAKE SQL STATEMENT */

    *sqlstmt = (CHAR *) tc_zalloc(sizeof(CHAR) * (68+lab_len));
    sprintf(*sqlstmt,"SELECT %s FROM %s.WELL_HDR WHERE UWI = :v",
						label,project);
    }


finished:

return status;
}
