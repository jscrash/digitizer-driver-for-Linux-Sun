/* DEC/CMS REPLACEMENT HISTORY, Element SE_STC_LNAME.C*/
/* *4    23-AUG-1990 17:19:54 JESSIE "(SPR 1) fix include files"*/
/* *3    17-AUG-1990 22:18:22 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:03:51 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:25:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SE_STC_LNAME.C*/
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
    This routine creates the structure line_name from the long_line_name. 

Prototype:
    publicdef INT  se_struct_from_long_name(FULL_LINE_NAME *long_line_name , SEIS_LINE line_name ) ;

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line_name       -(FULL_LINE_NAME *) Line_name  is the structure composed of the 
                        line name and the survey name.
    long_line_name  -(SEIS_LINE) Long_line_name is the character string - concatenation 
                        of the line name and the survey name. 

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_struct_from_long_name (SEIS_LINE long_line_name,
                                        FULL_LINE_NAME *line_name)
#else
publicdef INT se_struct_from_long_name (long_line_name, line_name)
FULL_LINE_NAME *line_name;
SEIS_LINE long_line_name;
#endif
    {
    CHAR *pcar;
    INT i, sep_pos;
    INT separ = LINE_NAME_SEPARATOR;
    SEIS_LINE temp;
    
    /* the short line name and survey name are separated
       by the separator separ 
       if the separator doesn't exists this means that the 
       survey is null 
    */
    
    /* look for the separator  */
    
    sep_pos = -1;
    
    for (i = strlen (long_line_name) - 1; i > 0; i--)
        {
        if (long_line_name[i] EQUALS separ)
            {
            sep_pos = i;
            break;
            }
        }
    /* sep_pos = 0 is an error because the line name must be
       not null 
    */
    
    if (sep_pos EQUALS 0)
        {
        return FAIL;
        }
    if (sep_pos < 0)
        {
        /* there is no separator so there is no survey 
           we just copy the line name **/
        
        strcpy (line_name->line_name, long_line_name);
        strcpy (line_name->survey, "");
        }
    else
        {
        /*    copy first the survey name   */
        
        pcar = long_line_name + sep_pos + 1;
        
        strcpy (line_name->survey, pcar);
        
        /* copy the line name  */
        
        pcar = line_name->line_name;
        
        for (i = 0; i < sep_pos; i++, pcar++)
            {
            *pcar = long_line_name[i];
            };
        
        *pcar = 0;
        
        }
    return SUCCESS;
    }
/* END:     */
