/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CHK_XST.C*/
/* *3    25-OCT-1990 12:17:55 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:51:37 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:55:02 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CHK_XST.C*/
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

#ifndef ESI_HT_H

#include "esi_ht.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To check the existance of nlist, if the specified nlist does not exist.
    The function will return NL_NLIST_NOT_FOUND, otherwise return the header
    offset of the nlist.

Prototype:
    publicdef INT nlz_chk_nlist_exist (FILENAME filename, NLIST_ID name, FILE_UNIT *pfunit,
        BOOL directory, NL_SHARE_MODE share_mode);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    name            -(NLIST_ID)
    pfunit          -(FILE_UNIT *)
    directory       -(BOOL) If TRUE, returns the offset of directory, otherwise, 
                        returns offset of data.    
    share_mode      -(NL_SHARE_MODE)

Return Value/Status:
    SUCCESS - Successful completion.
    The function will return NL_NLIST_NOT_FOUND, otherwise return the header
    offset of the nlist.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_chk_nlist_exist (FILENAME filename, NLIST_ID name, FILE_UNIT *pfunit,
                                   BOOL directory, NL_SHARE_MODE share_mode) 
#else
publicdef INT nlz_chk_nlist_exist (filename, name, pfunit, directory, share_mode)
FILENAME filename;
NLIST_ID name;
FILE_UNIT *pfunit;
BOOL directory;
NL_SHARE_MODE share_mode;
#endif
    {
    INT status;
    HASH_TABLE_P curTree;
    BOOL found;
    NL_INDEX_DATA data;
    NLIST_ID key;
    
    /* checking the existance of index of requested
       filename.  */
    if ((status = nlz_chk_exist_index (filename, &curTree, share_mode, pfunit)) < 0)
        {
        return status;
        }
    /* if yes, search the name of id in the index.*/
    
    tc_zeroes ((VOIDPTR)key, sizeof key);
    strcpy ((char *)key, (char *)name);
    found = (BOOL)ht_find (curTree, (VOIDPTR)key, (VOIDPTR) & data);
    if ((found EQUALS SUCCESS) AND NOT data.delete)
        {
        if (directory)
            return (INT)data.directory_offset;
        else
            return (INT)data.offset;
        }
    else
        {
        return NL_NLIST_NOT_FOUND;
        }
    }
/* END:     */
