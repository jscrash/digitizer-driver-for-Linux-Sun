/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_CULTURE_NEWOBJ.C */
/* *2     1-MAY-1991 22:01:40 KEE "(SPR -1) Landmark Import" */
/* *1     1-MAY-1991 22:00:50 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_CULTURE_NEWOBJ.C */
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

   NAME: LD_LM2_IMP_CULTURE_NEWOBJ.C

   AUTHOR: Rod Hanks
   DATE:   June 19th, 1990
   DESCRIPTION: Get ready to begin a new cultural object import.

   ************************************************************************ */

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_NL_ERR_H
#include "esi_nl_err.h"
#endif
#ifndef LD_LM2_IMP_WORK_H
#include "ld_lm2_imp_work.h"
#endif

#if USE_PROTOTYPES
publicdef INT ld_lm2_imp_culture_newobj (BOOL *newObject, BOOL *newLine, 
			  BOOL *oldObject, BOOL *oldLine,
			  NLIST_HEADER *nlist, CHAR *objectName, 
			  CHAR *full_filename, CHAR *nlist_id, 
			  CHAR *lineColor, CHAR *polygonFlag, CHAR *fillColor,
			  CHAR *project, CHAR *source, INT overwrite, 
			  HASH_TABLE_P hash_table, INT *currentObject)
#else
publicdef INT ld_lm2_imp_culture_newobj (newObject, newLine, oldObject, oldLine,
			   nlist, objectName, full_filename,
			   nlist_id, lineColor, polygonFlag, fillColor,
			   project, source, overwrite, hash_table, 
			   currentObject, lower_left_x, lower_left_y,
			   upper_right_x, upper_right_y)
BOOL		*newObject;
BOOL		*newLine;
BOOL		*oldObject;
BOOL		*oldLine;
NLIST_HEADER	*nlist;
CHAR		*objectName;
CHAR		*full_filename;
CHAR		*nlist_id;
CHAR		*lineColor;
CHAR		*polygonFlag;
CHAR		*fillColor;
CHAR		*project;
CHAR		*source;
INT		 overwrite;
INT		*currentObject;
HASH_TABLE_P	 hash_table;
#endif
    {
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    INT 	   status;
    UINT	   ncols;
    INT		   units[2];
    INT		   data_types[2];
    size_t	   data_sizes[2];
    INT		   value_inc[2];
    UINT	   blocksize;
    INT		   result;
/*
				General variable declarations.
*/
/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    ho_add_path("ESI$CULTURE", objectName, sizeof(FILENAME),
					full_filename);
    result = ld_lm2_imp_culture_check (objectName, project, source, overwrite, 
			   hash_table, currentObject);

    switch (result)
	{
	case 1:
	    ld_lm2_imp_culture_newline(lineColor, fillColor,
				   project, nlist_id, polygonFlag,
				   full_filename);
/*
				Create the new entry in the lines table.
*/
	    *oldObject = TRUE;
	    *oldLine = TRUE;
	    *newLine = TRUE;
	    break;
/*
				Set flags to show we need to write
				this nlist out at the end and we
				need to put a row into the 
				GRAPHIC_OBJECTS table for it.
*/
	case 2:
	    ld_lm2_imp_culture_newline(lineColor, fillColor,
				   project, nlist_id, polygonFlag,
				   full_filename);
/*
				Create the new entry in the lines table.
*/
	    *oldObject = FALSE;
	    *oldLine = TRUE;
	    *newLine = TRUE;
	    printf("Adding to object already begun.\n");
	    break;
/*
				    We have previously entered information for
				    this object.  Therefore, we are just going
				    to be adding a new line to this object.
*/
	case 3:
    	    printf("overwriting culture named %s\n", objectName);
	    ld_lm2_imp_culture_delete (objectName, *currentObject, project);
/*
					Delete the existing object.
*/
	    ld_lm2_imp_culture_newline(lineColor, fillColor,
				   project, nlist_id, polygonFlag,
				   full_filename);
/*
				Create the new entry in the lines table.
*/
	    *oldLine = TRUE;
	    *oldObject = TRUE;
	    *newLine = TRUE;
	    break;
/*
					Set a flag to show that we will need
					to write out the nlist when we are
					done.  But we won't need to make 
					entries into the GRAPHIC_OBJECTS tables.
*/
	default:
	    printf("Cultural data already exist from this source, \
not overwritten\n");
	    *oldObject = FALSE;
	    *oldLine = FALSE;
	    *newLine = FALSE;
/*
					Set flags to skip any further processing
					on this object.
*/
	}
/*
					Above, decide if we have visited
					this object before and what we should
					do about it.
*/
    if (*oldLine == TRUE)
	{
	ncols = 2;
	units[0] = units[1] = 0;
	data_types[0] = data_types[1] = FLOAT_TYPE;
	data_sizes[0] = data_sizes[1] = sizeof(FLOAT);
	value_inc[0] = value_inc[1] = VALUE_LIST;
	blocksize = 25;
	status = nl_init_nlist(nlist, ncols, units, data_sizes,
					   data_types, value_inc, blocksize);
	if (status != SUCCESS)
	    {
	    printf("Unable to initialize nlist, %s\n", 
					mg_message(status));
	    return FAIL;
	    }
	}
    *newObject = FALSE;
    }
