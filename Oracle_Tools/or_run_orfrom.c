/* DEC/CMS REPLACEMENT HISTORY, Element OR_RUN_ORFROM.C*/
/*  8    19-NOV-1991 15:48:39 LOEBL "(SPR 0) Convert to sqlforms30"*/
/* *7    27-NOV-1990 14:32:01 JESSIE "(SPR 6087) add -c option on finder command line"*/
/* *6    30-AUG-1990 12:45:36 GILLESPIE "(SPR 1) Clean up include file references"*/
/* *5    25-JUL-1990 19:57:54 VINCE "(SPR 5644) Header Standardization"*/
/* *4     1-NOV-1989 14:50:34 GILLESPIE "(SPR 30) Add esi_ho_err.h"*/
/* *3     6-SEP-1989 09:07:45 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    20-JUL-1989 08:25:44 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 13:21:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OR_RUN_ORFROM.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_HO_ERR_H
#include "esi_ho_err.h"
#endif

#ifndef ESI_OR_ERR_H
#include "esi_or_err.h"
#endif

#ifndef ESI_OR_MSG_H
#include "esi_or_msg.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT or_run_oracle_form(FILENAME xformname);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xformname       -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    OR_INVALID_FORM_NAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT or_run_oracle_form (FILENAME xformname)
#else
publicdef INT or_run_oracle_form (xformname)
FILENAME xformname;
#endif
    {
    INT opt = 0;
    INT status;
    INT formlen;
    INT dblen;
    INT front_dialog;
    ORACLE_NAME dbname;
    CONFIGURATION_WORKSPACE *c;
    CHAR buffer[255];
    CHAR crtname[31];
    INT crtlen; 
    CHAR text[30];
    FILENAME formname;
    FILENAME filespec;
    
    filespec[0] = NUL;
    dblen = sprintf (dbname, "");
    
    if (IS_EMPTY_STRING (xformname))
        {
        status = OR_INVALID_FORM_NAME;
        }
    else
        {
        /*
         * Due to a bug in ORACLE v5.1.17 and SQLFORMS 2.0.18, it does not honor
         * logical name lists.  
         * At sites, the directory search list for forms is ESI$FORMS, 
         * which is normally defines as:
         *  ESI$PROJECT:[CUSTOMIZED.FORMS]
         *  ESI$PROJECT_FORMS
         *  ESI$SYSTEM_FORMS
         */
        strcpy (formname, xformname);
        ts_sto_lower (formname);
        
	/* --------------------- */
	/*    set crt value      */
	/* --------------------- */

	if ((status = am_open_workspace("CONFIGURATION", AM_GLOBAL, 
			(VOIDPTR *)&c)) != SUCCESS)
		{
		sprintf(text,"CANNOT OPEN CRT!");
		am_message(AM_STATUS,text);
		er_log(text);
		}
	if (c->crt_name EQUALS NULL)
		{
		strcpy(crtname,"VT100");
		}
	else
		{
		strcpy(crtname,c->crt_name);
		}
        crtlen = strlen (crtname);
	
        status = ho_add_path ("ESI$FORMS", formname, sizeof(FILENAME), filespec);
        if (status EQUALS HO_NO_SUCH_PATH)
            {
            sprintf (buffer, mg_message (OR_BAD_LOGICAL), "ESI$FORMS");
            am_message (AM_ALERT, buffer);
            status = SUCCESS;
            return status;
            }
        /* add the standard Oracle file extension for  */
        /* forms                       */
        strcat (filespec, ".frm");
        
        /* get a fully qualified file name     */
        status = ho_translate_filename (filespec, filespec);
        
        if (status == SUCCESS)
            {
            if (hoexists (filespec) == TRUE)
                status = SUCCESS;
            else
                status = FAIL;
            }
        if (status EQUALS SUCCESS)
            {
            wi_query_front_dialog (&front_dialog);
            formlen = strlen (filespec);
            status = iapexe (opt, dbname, dblen, crtname, crtlen, filespec, formlen);
            wi_open_dialog (front_dialog);
            status = SUCCESS;
            }
        else
            {
            status = FAIL;
            }
        }
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        sprintf (buffer, mg_message (OR_CANNOT_ACCESS_FORM), filespec);
        am_message (AM_STATUS, buffer);
        }
    return status;
    }
/* END:     */
