/* DEC/CMS REPLACEMENT HISTORY, Element FM_UPD_EXP.C */
/* *2     6-JUN-1991 14:52:08 MING "(SPR 0) new checkin" */
/* *1     6-JUN-1991 14:51:28 MING "update expiration date" */
/* DEC/CMS REPLACEMENT HISTORY, Element FM_UPD_EXP.C */
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

#include "esi_fm_int.h"
#include "esi_fm_err.h"
#include "esi_oracle.h"

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Update the expiration date of the fm file.

Prototype:

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    fi_pathname     -(FM_PATH)
    fi_filename     -(FM_LONG)
    file_type       -(FM_TYPE)

Return Value/Status:
    FILE ID
    0

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef fm_update_expiration_date (FM_PATH fi_pathname, FM_LONG fi_filename, FM_TYPE file_type, CHAR *expired_date)
#else
publicdef fm_update_expiration_date (fi_pathname, fi_filename, file_type, expired_date)
FM_PATH fi_pathname;
FM_LONG fi_filename;
FM_TYPE file_type;
CHAR *expired_date;
#endif
    {
    
    INT status;
    FM_STRUCT *fm_hdr;
    CHAR key[FM_KEY_SIZE];
    ANALYST_NAME username;
    ANALYST_NAME creator;
    PROJECT_NAME project;
    FM_FILE filename;
    FILENAME os_filename;
    HASH_TABLE_P fm_hash_table = fm_sys_index ();
    
    /*************************************************************************/
    
    /* * CHECK THAT HAVE SOMETHING TO QUERY THE FM SYSTEM * */
    /* ---------------------------------------------------- */
    
     if (IS_EMPTY_STRING (fi_filename) OR IS_EMPTY_STRING (fi_pathname))
        {
        return(FM_NULL_PTR);
        }
    /* * CHECK HASH TABLE FOR RECORD * */
    /* ------------------------------- */
    
    fm_parse_long_name (fi_filename, creator, filename);
    tc_zeroes (key, FM_KEY_SIZE);
    sprintf (key, "%s%s%s%s", fi_pathname, filename, file_type, creator);
    if ((status = ht_find (fm_hash_table, key, &fm_hdr)) EQUALS SUCCESS)
        {
        strcpy (fm_hdr->expiration_date,expired_date);
        return SUCCESS;
        }
    else
        {
	return FAIL;
	}
    }
/* END:     */
