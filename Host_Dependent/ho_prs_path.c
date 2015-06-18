/* DEC/CMS REPLACEMENT HISTORY, Element HO_PRS_PATH.C */
/* *2    21-FEB-1991 18:02:01 MING "(SPR -1) initial checkin" */
/* *1    21-FEB-1991 18:01:18 MING "parse OS path into OS dir and OS filename" */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_PRS_PATH.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
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



/* Function Description -----------------------------------------------------
Description: To parse input OS pathname into OS dir and OS filename

Prototype:
    publicdef INT ho_parse_pathname (pathname, os_dir, os_filename)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg	- description
	CHAR *pathname; 	(I)	 
	CHAR *os_dir;		(O)	
	CHAR *os_filename;	(O)

Return Value/Status:
    SUCCESS	- Successfull completion

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/


#include "esi_c_lib.h"

#if USE_PROTOTYPES
publicdef INT ho_parse_pathname (CHAR *pathname, CHAR *os_dir, CHAR *os_filename)
#else
publicdef INT ho_parse_pathname (pathname, os_dir, os_filename)
CHAR *pathname;
CHAR *os_dir;
CHAR *os_filename;
#endif
    {
    CHAR *pdot;

    strcpy (os_dir,pathname);
#if UNIX
    pdot = strrchr (os_dir,'/');
    if (pdot == (CHAR *) 0)
        {
	strcpy (os_dir,"");
	strcpy (os_filename,pathname);
	}
    else
	{
	pdot ++;
        strcpy (os_filename,pdot);
        *pdot = '\0';
        }
#endif
#if vms 
    pdot = strrchr (os_dir,']');
    if (pdot == (CHAR *) 0)
        {
	pdot = strrchr (os_dir,':');
	if (pdot == (CHAR *)0)
	    {
	    strcpy (os_dir,"");
	    strcpy (os_filename,pathname);
	    }
        else
	    {
	    pdot ++;
            strcpy (os_filename,pdot);
            *pdot = '\0';
	    }
	}
    else
	{
	pdot ++;
        strcpy (os_filename,pdot);
        *pdot = '\0';
	}
#endif
    return SUCCESS;
    }


