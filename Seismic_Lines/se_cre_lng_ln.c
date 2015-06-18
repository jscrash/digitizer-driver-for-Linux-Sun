/* DEC/CMS REPLACEMENT HISTORY, Element SE_CRE_LNG_LN.C */
/* *2    17-AUG-1990 22:16:13 VINCE "(SPR 5644) Code Cleanup" */
/* *1     5-MAR-1990 16:59:24 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_CRE_LNG_LN.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

#if USE_PROTOTYPES
static INT sez_cre_struct_line_name (FULL_LINE_NAME *seis_line,
                                         SEIS_SURV survey_name,
                                         SEIS_LINE_NAME short_name);
#else
static INT sez_cre_struct_line_name ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT se_cre_long_line_name (SEIS_LINE_NAME short_name, SEIS_SURV survey_name, 
        SEIS_LINE long_line_name);

Private_Functions:
    static INT sez_cre_struct_line_name (FULL_LINE_NAME *seis_line,
                                        SEIS_SURV survey_name,
                                        SEIS_LINE_NAME short_name);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This routine creates the long_line_name.

Prototype:
    publicdef INT se_cre_long_line_name (SEIS_LINE_NAME short_name, SEIS_SURV survey_name, 
        SEIS_LINE long_line_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    short_name      -(SEIS_LINE_NAME)
    survey_name     -(SEIS_SURV) Survey_name is the survey name short_name
                        is the line name inside the survey.
    long_line_name  -(SEIS_LINE) Long_line_name is the character string -
                        concatenation of the line name and the
                        survey name.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_cre_long_line_name (SEIS_LINE_NAME short_name, SEIS_SURV survey_name,
                                     SEIS_LINE long_line_name)
#else
publicdef INT se_cre_long_line_name (short_name, survey_name, long_line_name)
SEIS_LINE_NAME short_name;
SEIS_SURV survey_name;
SEIS_LINE long_line_name;
#endif
    {
    FULL_LINE_NAME seis_line;
    
    sez_cre_struct_line_name (&seis_line, survey_name, short_name);
    se_long_name_from_struct (&seis_line, long_line_name);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine creates the structure line_name.

Prototype:
    static INT sez_cre_struct_line_name (FULL_LINE_NAME *seis_line, SEIS_SURV survey_name, 
        SEIS_LINE_NAME short_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *) Seis_line is the structure composed of the
                        line name and the survey name.
    survey_name     -(SEIS_SURV) Survey_name is the survey name.
    short_name      -(SEIS_LINE_NAME) Short_name is the line name inside the
                        survey.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <se_cre_lng_ln.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sez_cre_struct_line_name (FULL_LINE_NAME *seis_line,
                                         SEIS_SURV survey_name,
                                         SEIS_LINE_NAME short_name)
#else
static INT sez_cre_struct_line_name (seis_line, survey_name, short_name)
FULL_LINE_NAME *seis_line;
SEIS_SURV survey_name;
SEIS_LINE_NAME short_name;
#endif
    {
    strcpy (seis_line->survey, survey_name);
    strcpy (seis_line->line_name, short_name);
    return SUCCESS;
    }
/* END:     */
