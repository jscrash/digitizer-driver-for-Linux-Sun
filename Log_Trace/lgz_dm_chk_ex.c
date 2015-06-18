/* DEC/CMS REPLACEMENT HISTORY, Element LGZ_DM_CHK_EX.C*/
/* *2     6-SEP-1989 08:12:24 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *1    19-JUN-1989 13:04:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LGZ_DM_CHK_EX.C*/

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


/* ***************************************************************************

    Author:   James Chang
    ************************************************************************ */

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_LG_DEFS_H
#include "esi_lg_defs.h"
#endif

/*  **************************************************************************

 ROUTINE: LGZ_CHECK_EXIST

 DESCRIPTION
      A internal help routine og Log Trace Data Manager.  The function of this
      routine is to preform checking the existance of a specific character
      string.

************************************************************************* */

publicdef INT lgz_check_exist(string)
CHAR *string;
{
    return IS_STRING(ts_snowhite(string)) ? SUCCESS : FAIL;
}
