/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_CULTURE.C */
/* *3     1-MAY-1991 22:08:09 KEE "(SPR -1) Landmark Import" */
/* *2     1-MAY-1991 22:01:33 KEE "(SPR -1) Landmark Import" */
/* *1     1-MAY-1991 21:12:59 KEE "Landmark Import & Export, Zycor Export and Mimic Export" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LM2_IMP_CULTURE.C */
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

   NAME: LD_LM2_IMP_CULTURE.c

   AUTHOR: Rod Hanks
   DATE:   June 18th, 1990
   DESCRIPTION: Load cultural lines from a file output by Landmark.

   ************************************************************************ */

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_NL_ERR_H
#include "esi_nl_err.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_HT_H
#include "esi_ht.h"
#endif
#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif
#ifndef ESI_CTYPE_H
#include "esi_ctype.h"
#endif
#ifndef LD_LM2_IMP_WORK_H
#include "ld_lm2_imp_work.h"
#endif

#define MAXCHAR	1024
#define polygon 0
#define line    1

#if USE_PROTOTYPES
publicdef INT ld_lm2_imp_culture (FILENAME fileName, PROJECT_NAME project,
			BOOL convertProj, 
			PROJECTION_STRUCTURE *defaultProj, PROJECTION_STRUCTURE *inputProj,
			CHAR *source, INT overwrite, DOUBLE proj_ll_x,
			DOUBLE proj_ll_y, DOUBLE proj_ur_x, DOUBLE proj_ur_y)
#else
publicdef INT ld_lm2_imp_culture (fileName, project,
			convertProj, defaultProj, inputProj,
			source, overwrite, proj_ll_x,
			proj_ll_y, proj_ur_x, proj_ur_y)
FILENAME	fileName;
PROJECT_NAME	project;
BOOL	              convertProj;
PROJECTION_STRUCTURE *defaultProj;
PROJECTION_STRUCTURE *inputProj;
CHAR		     *source;
INT		      overwrite;
DOUBLE		      proj_ll_x;
DOUBLE		      proj_ll_y;
DOUBLE		      proj_ur_x;
DOUBLE		      proj_ur_y;
#endif
    {
/****************************************************************************

	V a r i a b l e   D e c l a r a t i o n s .

******************************************************************************/

    INT 	   status;
    FILE	   *spfile;
    FILENAME	   full_filename;
    NLIST_HEADER   nlist = (NLIST_HEADER) 0;
    NLIST_ID	   nlist_id;
    CHAR	  *inputX;
    CHAR	  *inputY;
    CHAR	  *inputName;
    CHAR	  *pos;
    CHAR	  *pos2;
    INT		   tableNum;
    DOUBLE	   thisX;
    DOUBLE	   thisY;
    FLOAT          FX, FY;
    UINT	   colList[2];
    VOIDPTR	   valueList[2];
    INT		   loop;
    BOOL	   newObject;
    BOOL	   newLine;
    BOOL	   oldObject;
    BOOL	   oldLine;
    BOOL	   dataHit;
    BOOL	   clippedFlag;
    CHAR	   fileLine[MAXCHAR];
    CHAR	   objectName[51];
    CHAR	   lineColor[21];
    CHAR	   fillColor[21];
    CHAR	   polygonFlag[4];
    INT		   npts;
    HASH_TABLE_P   hash_table;
    INT		   objectType;
    INT		   currentObject;
    DOUBLE	   ll_x = 99999;
    DOUBLE	   ll_y = 99999;
    DOUBLE	   ur_x = -99999;
    DOUBLE	   ur_y = -99999;
    DOUBLE	   height;
    DOUBLE	   width;
    DOUBLE	   angle;
    DOUBLE	   slant;
    CHAR	  *sheight;
    CHAR	  *swidth;
    CHAR	  *sangle;
    CHAR	  *sslant;
    CHAR          *inputText;
    CHAR	  *justification;
    CHAR	  *font;
    CHAR	  *color;
    BOOL	   tryAgain;
/*
				General variable declarations.
*/
/****************************************************************************

	I n i t i a l i z a t i o n .

******************************************************************************/

    status = ht_init_hash_table(200, 10, HT_CHAR_KEY, 80, 0, 
				   HT_SYSTEM_TABLE, &hash_table);
/*
				Allocate a hash table to keep track of 
				objects which we have input during this
				run.
*/
    spfile = ho_open(fileName, "r", NULL);
    if (spfile == (FILE *)NULL)
	{
	printf("Unable to open culture file named %s\n", fileName);
	}
    else
	{
	printf("Loading cultural data from file named %s\n", fileName);
	strcpy(objectName, "BLANK");
	strcpy(lineColor, "BLACK");
	strcpy(polygonFlag, "NO");
	strcpy(fillColor, "");
	newObject = TRUE;
	newLine = TRUE;
	oldObject = FALSE;
	oldLine = FALSE;
	dataHit = FALSE;
	clippedFlag = FALSE;
	objectType = line;
	pos = fgets(fileLine, MAXCHAR, spfile);
	while (pos != (CHAR*) 0)
	    {
	    sheight = swidth = sangle = sslant = inputX = inputY = (CHAR *)0;
	    height = width = angle = slant = thisX = thisY = 0.0;
	    pos2 = strchr(fileLine, '\n');
	    if (pos2 != (CHAR*) 0)
		{
		*pos2 = 0;
		}
	    while (*pos != 0 && *pos != ',' && *pos != ' ' && *pos != 34)
		{
		pos++;
		}
	    if (*pos != 0)
		{
		*pos++ = 0;
		}
	    if (strcmp(fileLine, "METAFILE") == 0)
		{
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
		strcpy(objectName, inputName);
		newObject = TRUE;
		newLine = TRUE;
		}
	    else if (strcmp(fileLine, "IMAGE") == 0)
		{
		if (dataHit)
		    {
		    ld_lm2_imp_culture_write(&oldLine, &oldObject, 
					 &nlist, full_filename, nlist_id,
					 project, source,
					 objectName, &currentObject,
					 &ll_x, &ll_y, &ur_x, &ur_y);
		    }
/*
					Write out the one we have been working
					on, if any.
*/
		inputName = pos;
		while (*pos != 0 && *pos != ',' && *pos != ' ')
		    {
		    pos++;
		    }
	        if (*pos == ',' || *pos == ' ')
		    {
		    *pos++ = 0;
		    }
		strcpy(objectName, inputName);
		ts_sto_upper(objectName);
		newObject = TRUE;
		newLine = TRUE;
		}
	    else if (strcmp(fileLine, "POLYGON") == 0)
		{
		if (objectType != polygon)
		    {
		    newObject = TRUE;
		    }
		objectType = polygon;
		newLine = TRUE;
		strcpy(polygonFlag, "YES");
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
		if (strcmp(lineColor, inputName) != 0
		&&  strlen(inputName) > 0)
		    {
		    newObject = TRUE;
		    }
		strcpy(lineColor, inputName);
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
		if (strcmp(fillColor, inputName) != 0
		&&  strlen(inputName) > 0)
		    {
		    newObject = TRUE;
		    }
		strcpy(fillColor, inputName);
		}
	    else if (strcmp(fileLine, "LINE") == 0)
		{
		if (objectType != line)
		    {
		    newObject = TRUE;
		    }
		objectType = line;
		newLine = TRUE;
		strcpy(polygonFlag, "NO");
		strcpy(fillColor, "");
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
/*
					Style.
*/
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
/*
					dist.
*/
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
/*
					dash.
*/
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
/*
					iwidth.
*/
		inputName = pos;
		while (*pos != 0 && *pos != ',')
		    {
		    pos++;
		    }
	        if (*pos == ',')
		    {
		    *pos++ = 0;
		    }
		if (strcmp(lineColor, inputName) != 0
		&&  strlen(inputName) > 0)
		    {
		    newObject = TRUE;
		    }
		strcpy(lineColor, inputName);
		}
/*
					Color.
*/
	    else if (strcmp(fileLine, "TEXT") == 0)
		{
		tryAgain = TRUE;
		while (tryAgain)
		    {
	            pos2 = strchr(pos, '\0');
		    do
		        {
		        pos2--;
		        } while (*pos2 == ' ');
		    if (*pos2 == ',')
		        {
			pos2++;
	                fgets(pos2, MAXCHAR, spfile);
	                pos2 = strchr(pos, '\n');
	                if (pos2 != (CHAR*) 0)
		            {
		            *pos2 = 0;
		            }
		        }
		    else
		        {
		        tryAgain = FALSE;
		        }
		    }
/*
					Concatenate the lines together if
					it spreads over more than one.  We
					look for lines ending in ,.
*/
		inputText = pos;
		while (*inputText == ' '
		       || *inputText == ','
		       || *inputText == 39)
		    {
		    inputText++;
		    }
		pos = inputText + 1;
		while (*pos != 39)
		    {
		    pos++;
		    }
		*pos = 0;

		/* scan blanks and comma for the new code below */
		pos++;
		if (*pos == ' ') /* skip blanks */
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; /* if not comma, back up one character
				  for inputY = pos + 1 */
		      }
		  }

		/* new scan inputX code which takes care of 
		   no data for inputX 
		   ie. ,, in the input data file */
		inputX = pos + 1; 
		while (*inputX == ' ') /* skip blanks */
		    {
		    inputX++;
		    }
		pos = inputX;
		while (*pos != ' ' && *pos != ',') /* get value */
		    {
		    pos++;
		    }
		if (*pos == ' ') /* skip blanks */
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; /* if not comma, back up one character
				  for inputY = pos + 1 */
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		inputY = pos + 1;
		while (*inputY == ' ')
		    {
		    inputY++;
		    }
		pos = inputY;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		sheight = pos + 1;
		while (*sheight == ' ')
		    {
		    sheight++;
		    }
		pos = sheight;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		swidth = pos + 1;
		while (*swidth == ' ')
		    {
		    swidth++;
		    }
		pos = swidth;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		sangle = pos + 1;
		while (*sangle == ' ')
		    {
		    sangle++;
		    }
		pos = sangle;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		justification = pos + 1;
		while (*justification == ' ')
		    {
		    justification++;
		    }
		pos = justification;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		sslant = pos + 1;
		while (*sslant == ' ')
		    {
		    sslant++;
		    }
		pos = sslant;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		font = pos + 1;
		while (*font == ' ')
		    {
		    font++;
		    }
		pos = font;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

		color = pos + 1;
		while (*color == ' ')
		    {
		    color++;
		    }
		pos = color;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		if (*pos == ' ') 
		  {
		    *pos = 0;
		    pos++;
		    while (*pos == ' ')
		      {
			pos++;
		      }
		    if (*pos != ',')
		      {
			pos--; 
		      }
		  }
		else if (*pos == ',') 
		  {
		    *pos = 0;
		  }

#ifdef ORIGINAL
 /* following is the original code */
		inputX = pos + 1;
		while (*inputX == ' ' || *inputX == ',')
		    {
		    inputX++;
		    }
		pos = inputX + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0; 

		inputY = pos + 1;
		while (*inputY == ' ' || *inputY == ',')
		    {
		    inputY++;
		    }
		pos = inputY + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		sheight = pos + 1;
		while (*sheight == ' ' || *sheight == ',')
		    {
		    sheight++;
		    }
		pos = sheight + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		swidth = pos + 1;
		while (*swidth == ' ' || *swidth == ',')
		    {
		    swidth++;
		    }
		pos = swidth + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		sangle = pos + 1;
		while (*sangle == ' ' || *sangle == ',')
		    {
		    sangle++;
		    }
		pos = sangle + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		justification = pos + 1;
		while (*justification == ' ' || *justification == ',')
		    {
		    justification++;
		    }
		pos = justification + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		sslant = pos + 1;
		while (*sslant == ' ' || *sslant == ',')
		    {
		    sslant++;
		    }
		pos = sslant + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		font = pos + 1;
		while (*font == ' ' || *font == ',')
		    {
		    font++;
		    }
		pos = font + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;

		color = pos + 1;
		while (*color == ' ' || *color == ',')
		    {
		    color++;
		    }
		pos = color + 1;
		while (*pos != ' ' && *pos != ',')
		    {
		    pos++;
		    }
		*pos = 0;
#endif
		if (IS_EMPTY_STRING(sheight) == FALSE)
		  {
		    sscanf(sheight, "%lf", &height);
		  }
		if (IS_EMPTY_STRING(swidth) == FALSE)
		  {
		    sscanf(swidth, "%lf", &width);
		  }
		if (IS_EMPTY_STRING(sangle) == FALSE)
		  {
		    sscanf(sangle, "%lf", &angle);
		  }
		if (IS_EMPTY_STRING(sslant) == FALSE)
		  {
		    sscanf(sslant, "%lf", &slant);
		  }
		if (IS_EMPTY_STRING(inputX) == FALSE)
		  {
		    sscanf(inputX, "%lf", &thisX);
		  }
		if (IS_EMPTY_STRING(inputY) == FALSE)
		  {
		    sscanf(inputY, "%lf", &thisY);
		  }
		if (convertProj)
		    {
		    ct_transform_point(thisX, thisY, inputProj,
					  &thisX, &thisY, defaultProj);
		    }
		if (thisX >= proj_ll_x && thisX <= proj_ur_x
		&&  thisY >= proj_ll_y && thisY <= proj_ur_y)
		    {
		    ld_lm2_imp_culture_text (&oldObject, inputText,
			   objectName, thisX, thisY, height, width,
			   angle, justification, slant, font, color,
			   project, source, overwrite, hash_table, 
			   &currentObject, &ll_x, &ll_y, &ur_x, &ur_y);
		    }
		else
		    {
		    if (clippedFlag == FALSE)
			{
			printf("Clipped...");
			clippedFlag = TRUE;
			}
		    }
		}
	    else if (strcmp(fileLine, "XY") == 0
		 ||  strcmp(fileLine, "VECTOR") == 0)
		{
		dataHit = TRUE;
		if (strcmp(fileLine, "VECTOR") == 0)
		    {
		    newLine = TRUE;
		    strcpy(polygonFlag, "NO");
		    strcpy(fillColor, "");
		    npts = 2;
		    }
		else
		    {
		    npts = 1;
		    }
		if (newObject)
		    {
		    ld_lm2_imp_culture_write(&oldLine, &oldObject, 
					     &nlist, full_filename, nlist_id,
					     project, source,
					     objectName, &currentObject,
					     &ll_x, &ll_y, &ur_x, &ur_y);
/*
					Write out the one we have been working
					on, if any.
*/
		    ld_lm2_imp_culture_newobj(&newObject, &newLine, 
					      &oldObject, &oldLine,
					      &nlist, objectName, 
					      full_filename, nlist_id,
					      lineColor, polygonFlag,
					      fillColor, project, 
					      source, overwrite, hash_table,
					      &currentObject);
/*
					Setup to begin a new nlist for this
					image.
*/
		    }
		if (newLine == TRUE && oldLine == TRUE)
		    {
		    status = nl_start_table(nlist, (UINT *)&tableNum);
		    if (status != SUCCESS)
			{
			printf("Unable to start table in nlist, %s\n", 
					mg_message(status));
			return FAIL;
			}
		    status = nl_set_current_row(nlist, 0, tableNum);
		    if (status != SUCCESS)
		        {
			printf("Unable to set row in nlist, %s\n", 
					mg_message(status));
			return FAIL;
		        }
		    newLine = FALSE;
		    }
/*
					Lift the pen to begin a new stroke.
*/
		if (oldLine)
		    {
		    for (loop = 1; loop <= npts; loop++)
			{
		        inputX = pos;
		        while (*pos != 0 && *pos != ',')
		            {
		            pos++;
		            }
	                if (*pos == ',')
		            {
		            *pos++ = 0;
		            }
		        inputY = pos;
		        while (*pos != 0 && *pos != ',')
		            {
		            pos++;
		            }
	                if (*pos == ',')
		            {
		            *pos++ = 0;
		            }
	                sscanf(inputX, "%lf", &thisX);
	                sscanf(inputY, "%lf", &thisY);
		        if (convertProj)
		            {
		            ct_transform_point(thisX, thisY, inputProj,
					  &thisX, &thisY, defaultProj);
		            }
			if (thisX >= proj_ll_x && thisX <= proj_ur_x
			&&  thisY >= proj_ll_y && thisY <= proj_ur_y)
			    {
			    FX = (FLOAT) thisX;
			    FY = (FLOAT) thisY;
	                    colList[0] = 1;
	                    colList[1] = 2;
	                    valueList[0] = (VOIDPTR)&FX;
	                    valueList[1] = (VOIDPTR)&FY;
	                    status = nl_add_rows(nlist, 1, 2, colList, valueList);
		            if (status != SUCCESS)
		                {
		                printf("Unable to add to nlist, %s\n", 
			    		mg_message(status));
		                return FAIL;
		                }
			    }
/*
				Put this row into the nlist.
*/
			else
			    {
			    if (clippedFlag == FALSE)
				{
				printf("Clipped...");
				clippedFlag = TRUE;
				}
			    }
			}
		    }
		}
	    pos = fgets(fileLine, MAXCHAR, spfile);
	    }
	    ld_lm2_imp_culture_write(&oldLine, &oldObject, 
				     &nlist, full_filename, nlist_id,
				     project, source,
				     objectName, &currentObject,
				     &ll_x, &ll_y, &ur_x, &ur_y);
        fclose(spfile);
	}
    ht_free_table(&hash_table);
    }

