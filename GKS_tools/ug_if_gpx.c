/* DEC/CMS REPLACEMENT HISTORY, Element UG_IF_GPX.C*/
/* *4    24-MAY-1990 18:54:33 MING "(SPR 0) ADD METAFILE AS GPX WORKSTATION"*/
/* *3    26-FEB-1990 19:01:19 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2    20-JUL-1989 11:02:15 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:35:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element UG_IF_GPX.C*/

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

#include "esi_ug.h"
#include "esi_qs.h"

/*******************************************************************

FILE: UG_IF_GPX.C

DESCRIPTION:
    This routine checks the QS state variable WORKSTATION_TYPE to
    determine if the current graphics host is a VAXSTATION.  The
    value returned is either TRUE or FALSE.
    
AUTHOR:
    Julian Carlisle	Feb 2, 1989
********************************************************************/
publicdef INT ug_if_gpx()
{
CHAR workstation_type[50];
INT len = 0;
static INT init = FALSE;
static INT is_gpx_type = FALSE;

if (init == FALSE)
    {
		    /* Inquire the workstation type.	*/
    qs_inq_c(QS_WORKSTATION_TYPE, workstation_type, &len);
    if( len != 0 && ((strncmp(workstation_type, "GPX", 3) == 0) OR
	(strncmp(workstation_type, "SUN", 3) == 0) OR
	(strncmp(workstation_type, "METAFILE", 8) == 0) OR
	(strncmp(workstation_type, "DECSTATION", 10) == 0))   ) 
	{	
	is_gpx_type = TRUE;
	}
	
    init = TRUE;
    }
return is_gpx_type;
}
