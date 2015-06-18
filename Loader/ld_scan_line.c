/* DEC/CMS REPLACEMENT HISTORY, Element LD_SCAN_LINE.C */
/* *3    10-SEP-1991 08:20:24 KEE "(SPR -1) Add one more element in the scan_line structure" */
/* *2     1-MAY-1991 22:12:24 KEE "(SPR -1) Loader Scanner to scan line by line of data file for Landmark Import" */
/* *1     1-MAY-1991 21:29:53 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_SCAN_LINE.C */
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

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_LD_H
#include "esi_ld.h"
#endif

#ifndef ESI_CTYPE_H
#include "esi_ctype.h"
#endif

#if USE_PROTOTYPES
publicdef INT ld_scan_line(FILE *spfile, LD_SCANNER_STRUCT *ld_scan)
#else
publicdef INT ld_scan_line(spfile, ld_scan)
FILE	   *spfile;
LD_SCANNER_STRUCT *ld_scan;
#endif
{

  CHAR   buffer[LD_STMAX];
  CHAR   tempstr[LD_STMAX];
  CHAR   str[LD_STMAX];
  CHAR   *pos, *temp_pos;
  INT    i, j;
  BOOL   found;

  if (ld_scan->out != (LD_PARM_STRUCT *)0)
    {  /* free out info struct if necessary */
      tc_free(ld_scan->out);
      ld_scan->out = (LD_PARM_STRUCT *)0;
    }

  ld_scan->out_nitems = 0; /* reset number of out items */

  pos = fgets(buffer, LD_STMAX, spfile); /* get line */

  if (pos == (CHAR *)0) /* check end of file */
    {
      ld_scan->eof = TRUE;
      return(SUCCESS);
    }
  else
    {
      strcpy(ld_scan->line, buffer);
    }

  if (ld_scan->in.fix_format == TRUE) 
    { /* check fix format */
      for (i=0; i<ld_scan->in.num_pair; i++)
	{ /* check whether start/end pairs are invalid */
	  if (ld_scan->in.end[i] < ld_scan->in.start[i])
	    {
	      printf("Invalid pair - ld_scan_line fail\n");
	      return(FAIL);
	    }
	}

      ld_scan->out = (LD_PARM_STRUCT *) 
	tc_zalloc (sizeof(LD_PARM_STRUCT) * ld_scan->in.num_pair);
      /* allocate out items struct */

      for (i=0; i<ld_scan->in.num_pair; i++)
	{
	  if (strlen(buffer) >= ld_scan->in.end[i]) 
	    { /* check end of line before scan word */

	      /* get word */
	      temp_pos = pos + ld_scan->in.start[i] - 1;
	      strncpy(tempstr, temp_pos,
		      (ld_scan->in.end[i]-ld_scan->in.start[i]+1));
	      tempstr[ld_scan->in.end[i]-ld_scan->in.start[i]+1] = 0;
	      strcpy(str, tempstr);
	      strcpy(tempstr , ts_snowhite(tempstr)); 
	      
	      if (IS_EMPTY_STRING(tempstr) == FALSE AND
		  ld_scan->in.string_only == FALSE AND
		  ld_isdigitstr(tempstr) == TRUE)
		{ /* is digit string ? */
		  ld_scan->out[ld_scan->out_nitems].parmtype = LD_INT;
		  sscanf(tempstr, "%d", &ld_scan->out[ld_scan->out_nitems].parmval.ival);
		}
	      else if (IS_EMPTY_STRING(tempstr) == FALSE AND
		       ld_scan->in.string_only == FALSE AND
		       ld_isfloatstr(tempstr) == TRUE)
		{ /* is float string ? */
		  ld_scan->out[ld_scan->out_nitems].parmtype = LD_DOUBLE;
		  /* calculate number of digit */
		  for (j=strlen(tempstr) - 1; 
		       tempstr[j] != '.';
		       j--, ld_scan->out[ld_scan->out_nitems].num_digit++);
		  sscanf(tempstr, "%lf", &ld_scan->out[ld_scan->out_nitems].parmval.dbval);
		  ld_scan->out[ld_scan->out_nitems].parmval.dbval /= 1.0;
		}
	      else 
		{ /* string */
		  ld_scan->out[ld_scan->out_nitems].parmtype = LD_STRING;
		  if (ld_scan->out[ld_scan->out_nitems].parmval.cval != (CHAR *) 0)
		    { /* free string if necessary */
		      tc_free(ld_scan->out[ld_scan->out_nitems].parmval.cval);
		    }
		  if (ld_scan->in.snowhite == TRUE)
		    {
		      ld_scan->out[ld_scan->out_nitems].parmval.cval = (CHAR *)
			tc_zalloc(sizeof(CHAR) * (strlen(tempstr) + 1));
		      strcpy(ld_scan->out[ld_scan->out_nitems].parmval.cval,tempstr);
		    }
		  else
		    {
		      ld_scan->out[ld_scan->out_nitems].parmval.cval = (CHAR *)
			tc_zalloc(sizeof(CHAR) * (strlen(str) + 1));
		      strcpy(ld_scan->out[ld_scan->out_nitems].parmval.cval,str);
		    }
		}
	      ld_scan->out_nitems++;
	    }
	}
    }
  else /* free format */
    { 
      /* allocate memory for out items */
      ld_scan->out = (LD_PARM_STRUCT *) 
	tc_zalloc (sizeof(LD_PARM_STRUCT) * 10);
      
      FOREVER /* get each word of the line */
	{
	  if (*pos == 0 OR *pos == '\n' OR *pos == '\r')
	    break; /* line break */
	  
	  FOREVER /* skip seperators */
	    {
	      for (found = FALSE, i = strlen(ld_scan->in.seperator)-1;
		   found == FALSE AND i >= 0; i--)
		{
		  if (*pos == ld_scan->in.seperator[i])
		    {
		      pos++;
		      found = TRUE;
		    }
		}
	      if (found == FALSE OR *pos == 0 OR *pos == '\n' OR *pos == '\r') 
		break;
	    } /* FOREVER skip seperators */
	  
	  if (*pos == 0 OR *pos == '\n' OR *pos == '\r')
	    break; /* line break */
	  
	  j = 0;
	  FOREVER /* get word */
	    {
	      /* skip the indicated skipping characters */
	      for (found = FALSE, i = strlen(ld_scan->in.skip_char)-1;
		   found == FALSE AND i >= 0; i--)
		{
		  if (*pos == ld_scan->in.skip_char[i])
		    {
		      pos++;
		      found = TRUE;
		    }
		}

	      if (found == FALSE) 
		{ /* Get a character that we don't wanna skip */
		  if (*pos == 0 OR *pos == '\n' OR *pos == '\r')
		    { /* line break */
		      tempstr[j] = 0;
		      break;
		    }
		  else
		    {
		      for (found = FALSE, i = strlen(ld_scan->in.seperator)-1;
			   found == FALSE AND i >= 0; i--)
			{ /* check if seperators */
			  if (*pos == ld_scan->in.seperator[i])
			    {
			      pos++;
			      found = TRUE;
			    }
			}
		      if (found == FALSE) /* the character we want */
			{
			  tempstr[j++] = *pos;
			  pos++;
			}
		      else /* it is a seperator - end of word */
			{ 
			  tempstr[j] = 0;
			  break;
			}
		    }
		}
	    }

	  if (IS_EMPTY_STRING(tempstr) == FALSE)
	    {
	      if (ld_scan->in.string_only == FALSE AND ld_isdigitstr(tempstr) == TRUE)
		{ /* is digit string ? */
		  ld_scan->out[ld_scan->out_nitems].parmtype = LD_INT;
		  sscanf(tempstr, "%d", &ld_scan->out[ld_scan->out_nitems].parmval.ival);
		}
	      else if (ld_scan->in.string_only == FALSE AND ld_isfloatstr(tempstr) == TRUE)
		{ /* is float string ? */
		  ld_scan->out[ld_scan->out_nitems].parmtype = LD_DOUBLE;
		  /* calculate number of digit */
		  for (j=strlen(tempstr) - 1; 
		       tempstr[j] != '.';
		       j--, ld_scan->out[ld_scan->out_nitems].num_digit++);
		  sscanf(tempstr, "%lf", &ld_scan->out[ld_scan->out_nitems].parmval.dbval);
		  ld_scan->out[ld_scan->out_nitems].parmval.dbval /= 1.0;
		}
	      else 
		{ /* string */
		  ld_scan->out[ld_scan->out_nitems].parmtype = LD_STRING;
		  if (ld_scan->out[ld_scan->out_nitems].parmval.cval != (CHAR *) 0)
		    { /* free string if necessary */
		      tc_free(ld_scan->out[ld_scan->out_nitems].parmval.cval);
		    }
		  ld_scan->out[ld_scan->out_nitems].parmval.cval = STRING_ALLOCATE(tempstr);
		  strcpy(ld_scan->out[ld_scan->out_nitems].parmval.cval,tempstr);
		}
	      ld_scan->out_nitems++;
	      if (ld_scan->out_nitems % 10 == 0)/* if memory allocation needed */
		{
		  ld_scan->out = (LD_PARM_STRUCT *) 
		    tc_realloc (ld_scan->out, sizeof(LD_PARM_STRUCT) * 
				(ld_scan->out_nitems + 10));
		} 
	    } /* EMPTY STRING */
	} /* get each word of the line */
    } /* free format */
  return(SUCCESS);
}


#if USE_PROTOTYPES
publicdef INT ld_isdigitstr (CHAR *str)
#else
publicdef INT ld_isdigitstr (str)
CHAR *str;
#endif
{
  INT i, j;
    
  for (i = 0, j = strlen (str); i < j; i++)
    {
      if (isdigit (str[i]) == FALSE)
	{
	  if (i != 0 || (i == 0 && str[i] != '-' && str[i] != '+'))
	    return(FALSE);
	}
    }
  return(TRUE);
}

#if USE_PROTOTYPES
publicdef INT ld_isfloatstr (CHAR *str)
#else
publicdef INT ld_isfloatstr (str)
CHAR *str;
#endif
{
  INT status, i, j, period_count = 0;
  
  for (i = 0, j = strlen (str); i < j; i++)
    {
      if (isdigit (str[i]) == FALSE)
	{
	  if (str[i] == '.')
	    {
	      period_count++;
	      if (period_count > 1)
		return(FALSE);
	    }
	  else
	    {
	      if (i != 0 || (i == 0 && str[i] != '-' && str[i] != '+'))
		return(FALSE);
	    }
	}
    }
  return(TRUE);
}


  
