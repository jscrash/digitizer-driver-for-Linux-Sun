/* DEC/CMS REPLACEMENT HISTORY, Element LD_SHOW_ERROR.C */
/* *2     1-MAY-1991 22:13:36 KEE "(SPR -1) Landmark Import & Export, Zycor Export and Mimic Export" */
/* *1     1-MAY-1991 21:30:31 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_SHOW_ERROR.C */
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

/*
	show_error.c

	Rod Hanks	May 9th, 1990

	This simple module wakes me up when Oracle errors occur.
*/
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#if USE_PROTOTYPES
publicdef VOID ld_show_error(INT errorCode, CHAR *inWhat)
#else
publicdef VOID ld_show_error(errorCode, inWhat)
INT	errorCode;
CHAR	*inWhat;
#endif
{
	printf("Oracle error %d while %s\n%s\n", errorCode, inWhat,
				mg_message(errorCode));
        return;
}




