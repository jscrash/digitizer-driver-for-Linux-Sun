/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_CULTURE_WRITE.C */
/* *2     1-MAY-1991 21:53:16 KEE "(SPR -1) Landmark Import" */
/* *1     1-MAY-1991 21:17:35 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_CULTURE_WRITE.C */
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

   NAME: LD_LM2_IMP_CULTURE_WRITE.C

   AUTHOR: Rod Hanks
   DATE:   June 18th, 1990
   DESCRIPTION: Write out a completed nlist of culture.

   ************************************************************************ */

#ifndef ESI_ORACLE_H
#include "esi_oracle.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_NL_ERR_H
#include "esi_nl_err.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef LD_LM2_IMP_WORK_H
#include "ld_lm2_imp_work.h"
#endif

#if USE_PROTOTYPES
publicdef INT ld_lm2_imp_culture_write (BOOL *oldLine, BOOL *oldObject, 
                          NLIST_HEADER *nlist, CHAR *full_filename,
			  CHAR *nlist_id, CHAR *project, CHAR *source, 
                          CHAR *objectName, INT *currentObject,
			  DOUBLE *lower_left_x, DOUBLE *lower_left_y, 
                          DOUBLE *upper_right_x, DOUBLE *upper_right_y)
#else
publicdef INT ld_lm2_imp_culture_write (oldLine, oldObject, nlist, full_filename,
			  nlist_id, project, source, objectName, currentObject,
			  lower_left_x, lower_left_y, upper_right_x,
			  upper_right_y)
BOOL		*oldLine;
BOOL		*oldObject;
NLIST_HEADER	*nlist;
CHAR		*full_filename;
CHAR		*nlist_id;
CHAR		*project;
CHAR		*source;
CHAR		*objectName;
INT		*currentObject;
DOUBLE		*lower_left_x;
DOUBLE		*lower_left_y;
DOUBLE		*upper_right_x;
DOUBLE		*upper_right_y;
#endif
    {
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    INT 	   status;
    UINT	   colList[3];
    VOIDPTR	   valueList[3];
    FLOAT	   Fnew_lower_left_x;
    FLOAT	   Fnew_lower_left_y;
    FLOAT	   Fnew_upper_right_x;
    FLOAT	   Fnew_upper_right_y;
    DOUBLE	   new_lower_left_x;
    DOUBLE	   new_lower_left_y;
    DOUBLE	   new_upper_right_x;
    DOUBLE	   new_upper_right_y;
/*
				General variable declarations.
*/
/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    if (*oldLine)
	{
	colList[0] = 1;
	colList[1] = 2;
	valueList[0] = (VOIDPTR) &Fnew_lower_left_x;
	valueList[1] = (VOIDPTR) &Fnew_lower_left_y;
	status = nl_inq_nlist_info(*nlist, NL_NLIST_MIN,
						2, colList, valueList);
	if (status != SUCCESS)
	    {
#if 0
           /* if 0 out because the oldline data might not in the nlist if 
	      the line is outside the range of map corner, therefore, 
	      the sttatus != SUCCESS should be ok */
	    printf("Error getting min values \n%s\n",
				mg_message(status));
#endif
	    return FAIL;
	    }
	valueList[0] = (VOIDPTR) &Fnew_upper_right_x;
	valueList[1] = (VOIDPTR) &Fnew_upper_right_y;
	status = nl_inq_nlist_info(*nlist, NL_NLIST_MAX,
						2, colList, valueList);
	if (status != SUCCESS)
	    {
#if 0
           /* if 0 out because the oldline data might not in the nlist if 
	      the line is outside the range of map corner, therefore, 
	      the sttatus != SUCCESS should be ok */
	    printf("Error getting max values \n%s\n",
				mg_message(status));
#endif
	    return FAIL;
	    }
/*
					Retrieve lower left, upper right points.
*/
	new_lower_left_x = (DOUBLE) Fnew_lower_left_x;
	new_lower_left_y = (DOUBLE) Fnew_lower_left_y;
	new_upper_right_x = (DOUBLE) Fnew_upper_right_x;
	new_upper_right_y = (DOUBLE) Fnew_upper_right_y;
	status = nl_write_nlist(*nlist, full_filename, nlist_id);
	if (status != SUCCESS)
	    {
	    printf("Unable to dispose nlist, %s\n",
					mg_message(status));
	    }
	if (*nlist != (NLIST_HEADER) 0)
	    {
	    nl_free_nlist(*nlist);
	    *nlist = (NLIST_HEADER) 0;
	    }
/*
					Above, write out the nlist.
*/
	ld_lm2_imp_culture_header (*oldObject, project, source, 
				  objectName, currentObject, 
				  nlist_id, "LINE",
				  new_lower_left_x, new_lower_left_y,
				  new_upper_right_x, new_upper_right_y,
				  lower_left_x, lower_left_y,
				  upper_right_x, upper_right_y);
	*oldObject = FALSE;
	*oldLine = FALSE;
	}
    }



