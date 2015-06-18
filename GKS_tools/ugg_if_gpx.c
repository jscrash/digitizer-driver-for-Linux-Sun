/* DEC/CMS REPLACEMENT HISTORY, Element UGG_IF_GPX.C */
/* *3    26-FEB-1990 19:01:16 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)" */
/* *2    11-AUG-1989 08:38:05 GORDON "(SPR 0) New glue file for ug_if_gpx" */
/* *1    11-AUG-1989 08:36:07 GORDON "New glue file for ug_if_gks" */
/* DEC/CMS REPLACEMENT HISTORY, Element UGG_IF_GPX.C */
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
#include "esi_gl_defs.h"

#if  UNIX
INT ugg_if_gpx_()
{
return ug_if_gpx();
}
#else
INT ugg_if_gpx()
{
return ug_if_gpx();
}
#endif 
