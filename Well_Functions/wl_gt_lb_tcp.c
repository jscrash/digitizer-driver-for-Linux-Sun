/* DEC/CMS REPLACEMENT HISTORY, Element WL_GT_LB_TCP.C */
/* *1    28-SEP-1990 11:06:26 GILLESPIE "Beta Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element WL_GT_LB_TCP.C */
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

/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: WL_GET_LABEL_TCP.C

   AUTHOR: Charlie Pumroy 

   DESCRIPTION: 
			mode 
			     = 0 => execute fetch, leave cursor opened,
			     = 1 => execute fetch, close cursor,
			     = 2 => close cursor, no fetch.

   ************************************************************************ */

/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to get the label information.

Prototype:
    publicdef INT wl_get_label_tcp(CHAR *uwi, TCP sql_tcp, INT mode,
        TCP *label_tcp);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    uwi             -(CHAR *) well identifier
    sql_tcp         -(TCP) sql statement
    mode            -(INT) processing mode
    label_tcp       -(TCP *) text string to return

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_get_label_tcp(CHAR *uwi, TCP sql_tcp, INT mode, TCP *label_tcp)
#else
publicdef INT wl_get_label_tcp(uwi,sql_tcp,mode,label_tcp)
CHAR	*uwi;					/* UWI */
TCP 	sql_tcp;				/* SQL STATEMENT */
INT	mode;					/* PROCESSING MODE */
TCP	*label_tcp;				/* TEXT STRING TO RETURN */
#endif
{
  INT status;
  CHAR label[STMAX];
  INT count,i;

  status = SUCCESS;
  *label_tcp = (CHAR **) 0;
  count = ts_tcp_len(sql_tcp);

  for (i=0; i<count; i++)
    {
      status = wl_get_label(uwi, sql_tcp[i], mode, label);
      if (status == SUCCESS)
	{
	  ts_tcp_append(label_tcp, label);
	}
      else
	{
	  /* placeholder in line (no/bad data) */	  
	  ts_tcp_append(label_tcp, "--");		
	  status = SUCCESS;
	}
    }
  
finished:
  
  return status;
}
