/* DEC/CMS REPLACEMENT HISTORY, Element WI_REL_TOBJ.C*/
/* *4    27-JUN-1990 17:31:17 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:24:31 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:43:16 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_REL_TOBJ.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* ************************************************************************

   NAME:    WI_RELEASE_TEXT_OBJECT

   DESCRIPTION:

        This function returns the memory comprising the definition and
    makes the text id number again available.

    INPUT:  'textid' ....... ID of the defined text object.

************************************************************************ */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#endif

#define MC_RELEASE_TEXT 79     /* ESITerm opcode for this command  */

/* ********************************************************************** */
publicdef INT wi_release_text_object(textid)
/* ********************************************************************** */

INT  textid;
{
    INT status = SUCCESS;

    PROGNAME("wi_assign_text_object");
#define IDENT progname

/* ********************************************************************** */

#ifdef USE_X

    E_CHECK_INIT;

    wiz_jp_name(progname);

    if (textid < 1 || wiv.text_id_count < textid)
      {
	E_WIZ_ERROR("Invalid text id",progname);
      }
    else
      {
	/* textid -1 : textid start from 1 and index of wiv.text_list start 
	   from 0 */
	if (IS_EMPTY_STRING(wiv.text_list[textid-1]) == FALSE)
	  {
	    wiv.text_list[textid-1][0] = 0;
	  }
	else
	  {
	    E_WIZ_ERROR("Text object has already been released",progname);
	  }
      }
	
	    
#else        /* ESITERM code */
    E_CHECK_INIT;

    wiz_jp_name(progname);

    SEND_2_VALUES( textid, MC_RELEASE_TEXT );
#endif

    RETURN(status);
}

