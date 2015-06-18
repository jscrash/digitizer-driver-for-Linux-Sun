/* DEC/CMS REPLACEMENT HISTORY, Element WL_PAR_LAB_TCP.C */
/* *2    29-NOV-1990 10:27:38 JULIAN "(SPR 6090) make source option work for @stack" */
/* *1    28-SEP-1990 11:07:01 GILLESPIE "Beta Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element WL_PAR_LAB_TCP.C */
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

#ifndef ESI_WL_ERR_H

#include "esi_wl_err.h"

#endif

#ifndef ESI_WL_MSG_H

#include "esi_wl_msg.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to parse label tcp

Prototype:
   publicdef INT wl_parse_label_tcp(CHAR *sent_sql, CHAR *source, TCP *sql_tcp)
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    sent_sql        -(CHAR *) sql statement
    source	    -(CHAR *) top source use when labeling
    sql_tcp	    -(TCP *) return sql phrases to process

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_parse_label_tcp(CHAR *sent_sql, CHAR *source, TCP *sql_tcp)
#else
publicdef INT wl_parse_label_tcp(sent_sql,source,sql_tcp)
CHAR	*sent_sql;		        /* SQL STATEMENT */
CHAR    *source;			/* TOP SOURCE USED WHEN LABELING */
TCP	*sql_tcp;			/* RETURNED SQL PHRASES TO PROCESS */
#endif
{
  CHAR *token;
  CHAR *pstring;
  CHAR sentence[STMAX];
  CHAR well_hdr[STMAX];
  CHAR well_tops[STMAX];
  CHAR *tmp_token;
  CHAR project[31];
  CHAR sql[512];
  CHAR sql_source[STMAX];
  INT status;
  BOOL subsea;
  INT nchars;
  CHAR label[STMAX];
  
  status = SUCCESS;
  *sql_tcp = (CHAR **) 0;
  qs_inq_c(QS_PROJECT_NAME,project,(INT *) 0);
  pstring = sent_sql;
  ts_sto_upper(pstring);
  subsea = TRUE; 		/* default */
  if (source == (CHAR) 0)
    {
      sql_source[0] = (CHAR) 0;
    }
  else
    {
      sprintf(sql_source, " AND SOURCE = '%s'",source);
    }
  
  /* First, Let's parse the string. It can belong to above, below, etc. We 
     don't care 

     (CP) In this case, sent_sql is just a copy of the wells->above, etc 

     (CP) We are assuming from the BNF (Backus Naur Form) that each key item
          (including the '@STACK' identifier and the end of the string) is 
	  separated by a semi-colon (';'). We break each piece as a token for 
	  further processing */

  /* while input string is not empty */
  while(*pstring)		
    {
      /* skip over blanks */
      while (*pstring == ' ')		
	pstring++;
      
      /* find the next ';', set token, point pstring after */
      tmp_token = strchr(pstring,';');	
      if (tmp_token != NULL)
	{
	  nchars = tmp_token - pstring + 1;
	  strncpy(sentence, pstring,nchars);
	  sentence[nchars] = (CHAR) 0;
	  pstring += nchars;
	  token = sentence;
	}
      else
	{
	  return WL_IMPROPER_LABEL;
	}
      
      /* Let's see if it's a stack command */
      if (strncmp(token, "@STACK;", 7) == 0)
	{
	  /* Well, thats nice. At least we are where we're supposed to be.
	     So, let's go on */
	  continue;
	}

      /* (CP) I really wanted to use the '=' as the next token, using 
	 everything to the left of the '=' as the table name. This evokes 
	 complete generality.
	 (Plot any column from any table). However, this does not solve the 
	 problem of subsea depths. Looking at the table name to be selected 
	 from is cheating and would cause this code to break if the table name 
	 changed. For example, you are looking for a specific formation name's 
	 measured depth rather than a column called 'form_code' ('form_code' 
	 can be any formation code).  I suggest that as users request more 
	 columns be selectable that we will just have to upgrade this routine 
	 to support more tables	(production values, IP values, tests, etc) */

      if (strncmp(token, "WELL_HDR", 8) == 0)
	{
	  /* We are going to process a list of (hopefully) valid column names 
	     in WELL_HDR table */
	  /* get rid of = */
	  tmp_token = strchr(token,'=');		
	  if (tmp_token != NULL)
	    {
	      nchars = tmp_token - token +1;
	      token = token + nchars;
	    }
	  else
	    {
	      return WL_IMPROPER_LABEL;
	    }
	  /* ignore any blank separators */
	  while (*token == ' ')		
	    token++;
	  
	  /* we now grab each header column name by looking for a comma (,) 
	     or the terminating semi-colon (;) for the WELL_HDR keyword */
	  while( (tmp_token = strchr(token,',')) != NULL)
	    {
	      nchars = tmp_token - token;
	      strncpy( well_hdr, token, nchars);
	      well_hdr[nchars] = (CHAR) 0;
	      token = token + nchars + 1;
	      /* while we're at it, skip next blanks */
	      while (*token == ' ')		
		token++;

	      /* build sql for call to wl_get_label then ts_tcp_append */
	      sprintf(sql,"SELECT %s FROM %s.WELL_HDR WHERE UWI = :v", 
		      well_hdr, project);
	      ts_tcp_append(sql_tcp, sql);
	    }
	  
	  /* we ran out of commas in this token so check it for semi-colon */
	  while( (tmp_token = strchr(token,';')) != NULL)
	    {
	      nchars = tmp_token - token;
	      strncpy(well_hdr, token, nchars);
	      well_hdr[nchars] = (CHAR) 0;
	      /* this may not be necessary */
	      token = token + nchars + 1;		
	      while (*token == ' ')
		token++;
	      
	      sprintf(sql, "SELECT %s FROM %s.WELL_HDR WHERE UWI = :v", 
		      well_hdr, project);
	      ts_tcp_append(sql_tcp, sql);
	    }
	  
	  if (*token != (CHAR) NULL)
	    {
	      /* we had stuff left over. thats a no no */
	      return WL_IMPROPER_LABEL;
	    }
	  
	  /* we successfully?? processed this token so we get another */
	  continue;		
	}
      
      if (strncmp(token, "WELL_TOPS", 9) == 0)
	{
	  /* We are going to process a list of (hopefully) valid column names 
	     in WELL_TOPS table */
	  /* get rid of = */
	  tmp_token = strchr(token,'=');
	  if (tmp_token != NULL)
	    {
	      nchars = tmp_token - token +1;
	      token += nchars;
	    }
	  else
	    {
	      return WL_IMPROPER_LABEL;
	    }
	  /* ignore any blank separators */
	  while (*token == ' ')
	    token++;
	  
	  /* we now grab each top name by looking for a comma (,) or the
	     terminating semi-colon (;) for the WELL_TOPS keyword */
	  while( (tmp_token = strchr(token,',')) != NULL)
	    {
	      nchars = tmp_token - token;
	      strncpy( well_tops, token, nchars);
	      well_tops[nchars] = (CHAR) 0;
	      token = token + nchars + 1;
	      /* while we're at it, skip next blanks */
	      while (*token == ' ')
		token++;

	      /* build sql for call to wl_get_label then ts_tcp_append */
	      if (subsea)
		{
		  sprintf(sql,"SELECT (ELEVATION-MD) FROM %s.WELL_TOPS X, %s.WELL_HDR Y \
		WHERE X.UWI = :v AND X.UWI = Y.UWI AND FORM_CODE = '%s'%s",
			  project, project, well_tops,sql_source);
		}
	      else
		{
		  sprintf(sql,"SELECT MD FROM %s.WELL_TOPS WHERE UWI = :v AND FORM_CODE = '%s'%s",
			  project,well_tops,sql_source);
		}
	      
	      ts_tcp_append(sql_tcp, sql);
	    }

	  /* we ran out of commas in this token so check it for semi-colon */
	  while( (tmp_token = strchr(token,';')) != NULL)
	    {
	      nchars = tmp_token - token;
	      strncpy(well_tops, token, nchars);
	      well_tops[nchars] = 0;
	      /* this may not be necessary */
	      token = token + nchars + 1;
	      while (*token == ' ')
		token++;
	      
	      if (subsea)
		{
		  sprintf(sql, "SELECT (ELEVATION-MD) FROM %s.WELL_TOPS X, %s.WELL_HDR Y \
		WHERE X.UWI = :v AND X.UWI = Y.UWI AND FORM_CODE = '%s'%s",
			  project, project, well_tops, sql_source);
		}
	      else
		{
		  sprintf(sql, "SELECT MD FROM %s.WELL_TOPS WHERE UWI = :v AND FORM_CODE = '%s'%s",
			  project, well_tops, sql_source);
		}
	      
	      ts_tcp_append(sql_tcp, sql);
	    }
	  
	  if (*token != (CHAR) NULL)
	    {
	      /* we had leftovers. Not correct     */
	      return WL_IMPROPER_LABEL;
	    }

	  /* we successfully?? processed this token so we get another */
	  continue;	
	}
      
      if (strncmp(token, "SUBSEA;", 7) == 0)
	{
	  subsea = TRUE;
	  continue;
	}

      if (strncmp(token, "MD;", 3) == 0)
	{
	  subsea = FALSE;
	  continue;
	}
      
    }	/* while (*pstring) */
  
finished:

  return status;
}
