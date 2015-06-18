/* DEC/CMS REPLACEMENT HISTORY, Element LG_LTDM_PARS.C*/
/* *1    19-JUN-1989 13:05:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LG_LTDM_PARS.C*/

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

/* LG_LTDM_PARSER.CC,  JAMES>LTDM>SOURCE,  JAMES CHANG.,  11/13/86
   A parser of Log Trace Data Manager.  To get standard name form.
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : LG_LTDM_PARSER - A parser of Log Trace Data Manager.  To get standard name form. */

/* START-HISTORY :
   11/13/86 JAMES    The first release
   10/17/86 JAMES    The first release.
   10/16/86 JAMES CHANG. Template constructed
   END-HISTORY */

/* START-DESCRIPTION :
    description to be inserted
   END-DESCRIPTION */

/* START-DESIGN :
   LG_LTDM_PARSER:
   BEGIN
    design to be inserted here
   END
   END-DESIGN */

/* START-CODE : */
/* ***************************************************************************

    MODULE:   LG_LTDM_PARSER

    DESCRIPTION
         A sub-module of Log Trace Data Manager.  The function of this module is
         performing parsing a standard composite trace name, or legally part of
         standard trace name.  The standard trace name form is as follows:

         SOURCE:TRACE.SERVICE;VERSION

         Where -
              SOURCE = Dara Source(LIS, BIT, ASCII, FINDERLOG, etc).
              TRACE  = Trace mnemonic or name(DT, RHOB, SW,PHIE, etc).
              SERVICE= Too mnemonic or computation algorithm(BHC, FDC, JOE, etc.)
              VERSION= Integer version number.  Used by loaders to store run
                       number.  Can be used by operators to explicitly create
                       version numbers.

    ASUMMPTION:
         The name should not cantain BLANK.

    *********************************************************************** */
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif
#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif
#ifndef ESI_LG_ERR_H
#include "esi_lg_err.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

/*  **************************************************************************

    ROUTINE:  LG_LTDM_PARSER

    DESCRIPTION
         A parser fo Log Trace Data Manager.  Parsing a standard trace name, and
         returns back the SOURCE, TRACE, SERVER, VERSION.

    ************************************************************************* */
INT lg_ltdm_parser(  logTrace    )
    /*************************************************************************/

LOG_TRACE_STRUCT *logTrace;       /*%P a pointer of input LOG_TRACE.         */
{
    INT i, preBlank;              /*   indexes.                              */
    INT status = SUCCESS;         /*   the result status.                    */
    INT defaultSource = FALSE;    /*   flag of using default SOURCE value.   */
    INT noTrace = FALSE;          /*   flag of existance of TRACE.           */
    INT noService = FALSE;        /*   flag of existance of SERVICE.         */
    INT noVersion = FALSE;        /*   flag of existance of VERSION.         */
    CHAR buffer[128];             /*   a buffer for parsing name.            */
    CHAR *exist;
/* ***************************************************************************/

                                  /*   error checking.                       */

    if(  logTrace EQUALS (LOG_TRACE_STRUCT *)NULL  )
         return LG_INVALID_POINTER;

    if( logTrace -> trace_name EQUALS NULL )
         return LG_NO_TRACE_NAME;

                                 /*    check the existance of filelds, e.g.  */
                                 /*    SOURCE, TRACE, SERVICE, VERSION.      */

    hoblockmove(logTrace -> trace_name, buffer, sizeof buffer );

				 /* zeros out the fields.                    */
    logTrace -> version = 0;
    strcpy(logTrace -> source, "");
    strcpy(logTrace -> trace, "");
    strcpy(logTrace -> service, "");

                                 /*    If there is one or more blank in the  */
                                 /*    trace_name, returns error.            */


    exist = strchr( logTrace -> trace_name, ' ' );
    if(   exist IS_NOT_EQUAL_TO NULL )
            return LG_INVALID_BLANK;

    exist = strchr( logTrace -> trace_name, ':' );
    defaultSource = (   exist EQUALS NULL ) ? TRUE : FALSE;

    exist = strchr( logTrace -> trace_name, '.' );
    noService = (  exist EQUALS NULL    )? TRUE : FALSE;

    exist = strchr( logTrace -> trace_name, ';' );
    noVersion = (  exist EQUALS NULL    )? TRUE : FALSE;

                                  /*   copy the name to a buffer, then conver*/
                                  /*   all sperated mark to be BLANK.        */

    for( i = 0, preBlank = -1;
         i < strlen( logTrace -> trace_name);
         i++)
    {
         if(  buffer[i] EQUALS ':' OR
              buffer[i] EQUALS '.' OR
              buffer[i] EQUALS ';'  )
         {
              buffer[i] = ' ';
              if(  i EQUALS preBlank + 1)
                   return LG_INVALID_FORMAT;
              preBlank = i;
         }
    }
                                  /*   cleans all fields to NULL before function*/

    tc_zeroes(logTrace -> source,    sizeof  logTrace -> source    );
    tc_zeroes(logTrace -> trace,     sizeof  logTrace -> trace     );
    tc_zeroes(logTrace -> service,   sizeof  logTrace -> service   );
    
                                  /*   analyze the existance of fields and tears*/
                                  /*   down the TRACE composite name.          */

    if(  defaultSource  )
    {

                                  /*   if there is no SERVICE and no VERSION */
         if(  noService AND noVersion  )
         {
              sscanf(buffer, "%s", logTrace -> trace );
         }

                                  /*   if there is VERSION only.             */
         else
         if(  noService  AND ! noVersion     )
         {
              sscanf(buffer, "%s %d", logTrace -> trace,
                        &logTrace -> version);
         }

                                  /*   if there is SERVICE only.             */
         else
         if( ! noService AND noVersion      )
         {
              sscanf(buffer, "%s %s", logTrace -> trace,
                        logTrace -> service);
         }

                                  /*   there are VERSION and SERVICE both.   */

         else
              sscanf(buffer, "%s %s %d", logTrace -> trace,
                        logTrace -> service, &logTrace -> version);
    }
    else
                                  /*   the source name is not default source. */
    {
                                  /*   if there is no SERVICE and no VERSION */
         if(  noService AND noVersion  )
         {
              sscanf(buffer, "%s %s", logTrace -> source,
                     logTrace -> trace );
         }

                                  /*   if there is VERSION only.             */
         else
         if(  noService  AND ! noVersion     )
         {
              sscanf(buffer, "%s %s %d", logTrace -> source,
                     logTrace -> trace, &logTrace -> version);
         }

                                  /*   if there is SERVICE only.             */
         else
         if( ! noService AND noVersion      )
         {
              sscanf(buffer, "%s %s %s", logTrace -> source,
                     logTrace -> trace, logTrace -> service);
         }

                                  /*   there are VERSION and SERVICE both.   */

         else
              sscanf(buffer, "%s %s %s %d", logTrace -> source,
                     logTrace -> trace, logTrace -> service, &logTrace -> version);
    }

                                  /*   if the TRCAE field is NULL return error.*/

    if( strlen(logTrace -> trace ) EQUALS 0 )
         return LG_NO_TRACE_FIELD;

    return SUCCESS;
}

/* END-CODE */
