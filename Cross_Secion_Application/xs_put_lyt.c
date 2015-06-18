/* DEC/CMS REPLACEMENT HISTORY, Element XS_PUT_LYT.C*/
/* *4    17-AUG-1990 22:42:56 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    18-SEP-1989 12:45:17 JULIAN "Gulf mods under SPR 100"*/
/* *2     3-AUG-1989 18:18:05 VINCE "(SPR -1) made binary layout processing conditional on NOT SUN4"*/
/* *1    19-JUN-1989 13:45:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_PUT_LYT.C*/
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

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#include "esi_time.h"

/* File Description ---------------------------------------------------------
Overview:
    A module of routines of Cross Section. This module includes
    all routines which perform function on database. 

    This module will access one table in data base, <AREA>.XS_VERB_KEYWORDS.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xs_put_layout(XS_STRUCT *pxs, CHAR layoutName[]);
    
    publicdef INT xsz_dump_to_uniq_file(XS_STRUCT *pxs, CHAR layoutName[], CHAR analyst[]);
    
    publicdef INT xsz_dump_to_uniq_file(XS_STRUCT *pxs, CHAR layoutName[], CHAR analyst[]);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Stores a layout to the database.
        
Prototype:
    publicdef INT xs_put_layout(XS_STRUCT *pxs, CHAR layoutName[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) A pointer to requested cross section structure.
    (I) layoutName  -(CHAR *) The specified layout name.

Return Value/Status:
    SUCCESS - Successful completion.
    SQL ERROR CODES

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_put_layout (XS_STRUCT *pxs, CHAR layoutName[])
#else
publicdef INT xs_put_layout (pxs, layoutName)
CHAR layoutName[];
XS_STRUCT *pxs;
#endif
    {
    INT status = SUCCESS;           /*   result status.                          */
    FILENAME filename;
    
    /* ************************************************************************* */
    
    /* error checkin.                    */
    /* checking for NULL POINTER.        */
    
    if (pxs EQUALS (XS_STRUCT *)NULL)
        return XS_INVALID_POINTER;
    
    /* checking for no layout name.      */
    
    if (strlen (layoutName) EQUALS 0)
        return XS_NO_LAYOUT_NAME;
    
    /* checking for the existance of layout*/
    
    /*  if(  xsz_chk_valid_layout( layoutName) )
        return XS_LAYOUT_DOES_EXIST;            !!! already tested !!!   */
    
    /* put the CROSS SECTION.            */
    
    status = xsz_add_cross_section (pxs, layoutName);
    if (status)
        return status;
    
    /* put the LOGPLOT TRACE.            */
    status = xsz_add_trace (pxs, layoutName);
    if (status)
        return status;
    
    /* put the LOGPLOT TRACK.            */
    status = xsz_add_track (pxs, layoutName);
    if (status)
        return status;
    
    /* put the LOGPLOT INTERVAL.         */
    status = xsz_add_interval (pxs, layoutName);
    if (status)
        return status;
    
    /* put the LOGPLOT HEADER.           */
    status = xsz_add_header (pxs, layoutName);
    if (status)
        return status;
    /* put the LOGPLOT SCALE and        */
    /* LOGPLOT SCALE BLOCK.             */
    
    status = xsz_add_scales (pxs, layoutName);
    if (status)
        return status;
    
    /* put the LOGPLOT TOP.            */
    status = xsz_add_top (pxs, layoutName);
    if (status)
        return status;

#ifndef sun
    xsz_dump_to_uniq_file (pxs, layoutName, "");
#endif
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT xsz_dump_to_uniq_file(XS_STRUCT *pxs, CHAR layoutName[], CHAR analyst[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    layoutName      -(CHAR [])
    pxs             -(XS_STRUCT *)
    analyst         -(CHAR [])

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_dump_to_uniq_file (XS_STRUCT *pxs, CHAR layoutName[], CHAR analyst[])
#else
publicdef INT xsz_dump_to_uniq_file (pxs, layoutName, analyst)
CHAR layoutName[];
XS_STRUCT *pxs;
CHAR analyst[];
#endif
    {
    INT status = SUCCESS;
    time_t ab_time;
    FILENAME filename, text;
    
    time (&ab_time);
    sprintf (text, "BIN_LYT_%d.DAT", ab_time);
    status = ho_add_path ("ESI$CROSS_SECT_LAYOUTS", text, sizeof(FILENAME), filename);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    
    /* get the unique filename.         */
    status = xsz_dump_bin_lyt (pxs, filename);
    if (status < 0)
        return status;
    
    if (analyst EQUALS (CHAR *)NULL OR strlen (analyst) EQUALS 0)
        {
        status = xsz_add_record (layoutName, "BINARY LAYOUT FILENAME", filename);
        }
    else
        {
        status = xsz_add_bin_record (layoutName, "BINARY LAYOUT FILENAME", filename,
                                     analyst);
        }
    return status;
    }
/* END:     */
