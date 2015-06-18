/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_TBL_NAM.C */
/* *6    17-AUG-1990 22:15:32 VINCE "(SPR 5644) Code Cleanup" */
/* *5    17-JUL-1990 19:29:10 VINCE "(SPR 5615) Changed the max index test to use a known last index" */
/* *4     9-AUG-1989 11:34:23 JULIAN "(SPR -1) fixing minor casting problems" */
/* *3     9-AUG-1989 10:23:24 JULIAN "(SPR -1) fixing minor casting problem" */
/* *2    25-JUL-1989 20:01:24 GILLESPIE "(SPR 50) Seismic system revamp" */
/* *1     5-JUL-1989 08:01:26 GILLESPIE "Return name of appropriate seismic table" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEZ_TBL_NAM.C */
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

/* The following lookup allows the programmer to change which table
    is used to refer to the seismic data in the seismic interpretation system.
    To add a new table, first edit the include file ESI_SE.H and add a new
    symbol to the SEIS_TABLE_NAME typedef; 2) add a similarly named symbol
    to the TABLE_NAME array here.  Be sure that the enum value is placed
    IN THE SAME RELATIVE POSITION AS DEFINED IN THE TABLE_NAME ARRAY */

static CHAR *seismic_table_names[] = 
    {
    "SEIS_SURFACE",
    "SEIS_LINE_HDR",
    "SEIS_SURVEY_HDR",
    "FULL_TIE"
    };



/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sez_table_name(SEIS_TABLE_NAME index, TABLE_NAME name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(SEIS_TABLE_NAME)
    name            -(TABLE_NAME)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sez_table_name (SEIS_TABLE_NAME index, TABLE_NAME name)
#else
publicdef INT sez_table_name (index, name)
SEIS_TABLE_NAME index;
TABLE_NAME name;
#endif
    {
    if (((INT)index >= 0) AND ((INT)index < (INT)MAX_SEIS_TABLE_INDEX))
        {
        strcpy ((char *)name, seismic_table_names[(INT)index]);
        return SUCCESS;
        }
    else
        {
        name[0] = 0;
        return FAIL;
        }
    }
/* END:     */
