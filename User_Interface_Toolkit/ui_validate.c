/* DEC/CMS REPLACEMENT HISTORY, Element UI_VALIDATE.C*/
/* *1    19-JUN-1989 13:35:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element UI_VALIDATE.C*/

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


/* TITLE : UI_VALIDATION - validate that numbers are within specified range */

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_wi.h"

/* **************************************************************************** */
publicdef BOOL ui_validate_int(value,minimum,maximum,text,help)
INT value;
INT minimum;
INT maximum;
CHAR *text;
CHAR *help;
{
/* %s validate that an integer lies within range */
    BOOL status = SUCCESS;  /* assume success */
    CHAR error_text[128];

    if ((minimum > value) OR (value > maximum))
       {
       status = FAIL;
       sprintf(error_text,"The value %d is outside the permissible range of %d to %d",value,minimum,maximum);
       wialert(1,help,error_text,text);
       }

    return status;

}

publicdef BOOL ui_validate_float(value,minimum,maximum,text,help)
DOUBLE value;
DOUBLE minimum;
DOUBLE maximum;
CHAR *text;
CHAR *help;
{
/* %s validate that a floating point number lies within range */
    BOOL status = SUCCESS;  /* assume success */
    CHAR error_text[128];

    if ((minimum > value) OR (value > maximum))
       {
       status = FAIL;
       sprintf(error_text,"The value %6.2f is outside the permissible range of %6.2f to %6.2f",value,minimum, maximum);
       wialert(1,help,error_text,text);
       }

    return status;

}
/* END-CODE */
