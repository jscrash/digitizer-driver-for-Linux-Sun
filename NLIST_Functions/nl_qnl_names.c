/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_NAMES.C */
/* *4    25-OCT-1990 12:25:22 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *3    25-JUL-1990 15:03:39 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:49:12 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:58 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QNL_NAMES.C */
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

#include "esi_nl_int.h"
#include "esi_nl_io.h"
#include "esi_ho_files.h"

/* Function Description -----------------------------------------------------
Description:
    To inquire the existing nlists name. 

Prototype:
    publicdef INT nl_inq_nlist_names (FILENAME in_filename, UINT *totalNlists, 
        CHAR ***nlistTable);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)
    nlist_Table     -(CHAR ***)
    totalNlists     -(UINT *)

Return Value/Status:
    NL_FILE_NOT_FOUND
    Function returns status from nlz_build_nlist_table.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_nlist_names (FILENAME in_filename, UINT *totalNlists,
                                  TCP *nlistTable)
#else
publicdef INT nl_inq_nlist_names (in_filename, totalNlists, nlistTable)
FILENAME in_filename;
TCP *nlistTable;
UINT *totalNlists;
#endif
    {
    INT status;
    FILENAME filename;
    FILE_UNIT funit;
    
    /* translate the file name first to
        get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return NL_FILE_NOT_FOUND;
        }
    /*
       * build nlist name table.
    */
    return nlz_build_nlist_table (filename, totalNlists, nlistTable, SHARE, &funit);
    
    }
/* END:     */
