/* DEC/CMS REPLACEMENT HISTORY, Element HO_DUMP.C*/
/* *2    17-AUG-1990 21:51:01 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:57:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_DUMP.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifdef primos

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif
#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ho_dump(CHAR *reportfile);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    reportfile      -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_dump (CHAR *reportfile) 
#else
publicdef INT ho_dump (reportfile)
CHAR *reportfile;
#endif
    {
    INT status;
    CHAR filename[32];
    CHAR username[33];
    CHAR filenameg[128];
    CHAR text[1024];
    
    /************************************************************************/
    
    status = SUCCESS;
    
    /*  inquire the user name, and force tha
    its length ito be shorter than 10. */
    
    ho_user_name (username);
    username[10] = '\0';
    
    sprintf (filename, "%s_%s.DUMP", username, ho_date_tag (0));
    
    ho_add_path ("ESI$ERRORS", filename, STMAX, filenameg);

#ifdef primos
    ho_syscom ("COMO -END");
    sprintf (text, "COMO %s", filenameg);
    ho_syscom (text);
    
    ho_syscom ("TYPE");
    ho_syscom ("TYPE ******************** DATE");
    ho_syscom ("TYPE");
    ho_syscom ("DATE");
    ho_syscom ("TYPE");
    ho_syscom ("TYPE ******************** DMSTK");
    ho_syscom ("TYPE");
    ho_syscom ("DMSTK");
    ho_syscom ("TYPE");
    ho_syscom ("TYPE ******************** LIST_EPF -NW -DETAIL");
    ho_syscom ("LIST_EPF -NW -DETAIL");
    ho_syscom ("TYPE ******************** LIST_SEARCH_RULES -NO_WAIT");
    ho_syscom ("LIST_SEARCH_RULES -NO_WAIT");
    ho_syscom ("TYPE");
    ho_syscom ("TYPE ******************** LIST_SEGMENT -NAME -NO_WAIT");
    ho_syscom ("TYPE");
    ho_syscom ("LIST_SEGMENT -NAME -NO_WAIT");
    ho_syscom ("TYPE ******************** STAT US");
    ho_syscom ("STAT US");
    
    ho_syscom ("COMO -END");
    sprintf (text, "SPOOL %s", filenameg);
    ho_syscom (text);
#endif

#ifdef vms
    /*      Probably want to spawn a job which uses the DCL command ANALYZE/SYSTEM
        to analyze this task. */

#endif
    
    /*   returns fileanme. Assume the length
     of returning REPORTFILE is longer th
     32.                */
    if (reportfile IS_NOT_EQUAL_TO NULL)
        strcpy (reportfile, filenameg);
    
    return status;
    }
/* END:     */
