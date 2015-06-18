/* DEC/CMS REPLACEMENT HISTORY, Element SE_LNAME_STC.C*/
/* *3    17-AUG-1990 22:17:21 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:03:13 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:24:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SE_LNAME_STC.C*/
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
#include "esi_se_int.h"

/* Function Description -----------------------------------------------------
Description:
    Transformation between long name and structure name.
    This routine creates the long_line_name from the structure. 

    In the long line name the short line name and survey name are separated
    by the separator LINE_NAME_SEPARATOR if the survey is NULL the 
    long line name is just equal to the line name.

Prototype:
    publicdef INT  se_long_name_from_struct (FULL_LINE_NAME *line_name , SEIS_LINE long_line_name  ) ;

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line-name       -(FULL_LINE_NAME *) The structure composed of the line name and 
                        the survey name.
    long_line_name  -(SEIS_LINE) The character string - concatenation of the line name
                        and the survey name. 

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_long_name_from_struct (FULL_LINE_NAME *line_name,
                                        SEIS_LINE long_line_name)
#else
publicdef INT se_long_name_from_struct (line_name, long_line_name)
FULL_LINE_NAME *line_name;
SEIS_LINE long_line_name;
#endif
    {
    if (!strcmp (line_name->survey, ""))
        {
        strcpy (long_line_name, line_name->line_name);
        }
    else
        {
        /* copy line name and pad first twenty characters with blanks */
        
        sprintf (long_line_name, "%s%c%s", line_name->line_name, LINE_NAME_SEPARATOR,
                 line_name->survey);
        }
    return SUCCESS;
    }
/* END:     */
