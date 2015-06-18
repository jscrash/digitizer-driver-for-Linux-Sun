/* DEC/CMS REPLACEMENT HISTORY, Element SE_DEL.C*/
/* *4    17-AUG-1990 22:16:17 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-SEP-1989 16:20:02 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    25-JUL-1989 20:01:46 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:24:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SE_DEL.C*/
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

#include "esi_se_int.h"

/* Function Description -----------------------------------------------------
Description:
    Used to delete  seismic surface data from SEIS_SURFACE. 

Prototype:
    publicdef INT se_del (CHAR *surf_code ,SEIS_SURFACE_DATA_CLASS surf_type ,SEIS_SURFACE_DATA_TYPE data_type,
        FULL_LINE_NAME *line, CHAR *source , SEIS_ATTRIBUTE_DATA_TYPE attribute , 
        SEIS_SURFACE_NLIST_TYPE format,INT version ) ;

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) surf_code       -(CHAR *) Name of the surface to be stored.     
    (I) surf_type       -(SEIS_SURFACE_DATA_CLASS) Type of the surface to be stored.    
    (I) data_type       -(SEIS_SURFACE_DATA_TYPE) Type of the data  stored for the surface. 
    (I) line            -(FULL_LINE_NAME) Line and survey name structure.
    (I) source          -(CHAR *) Source to which pertains the data. 
    (I) attribute       -(SEIS_ATTRIBUTE_DATA_TYPE) Gives the attribute of the seismic 
                            traces where data was picked.
    (I) format          -(SEIS_SURFACE_NLIST_TYPE) Format in which the data is stored 
                            in the nlist.
    (I) version         -(INT) Version number for the pick.

Return Value/Status:
    SUCCESS - Successful completion.
    Status returned by the function  nlist error code if pb in write nlist or
    oracle error code if pb in oracle.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_del (CHAR *surf_code, SEIS_SURFACE_DATA_CLASS surf_type,
                      SEIS_SURFACE_DATA_TYPE data_type, FULL_LINE_NAME *line,
                      CHAR *source, SEIS_ATTRIBUTE_DATA_TYPE attribute,
                      SEIS_SURFACE_NLIST_TYPE format, INT version)
#else
publicdef INT se_del (surf_code, surf_type, data_type, line, source, attribute, format,
                      version)
CHAR *surf_code;
SEIS_SURFACE_DATA_CLASS surf_type;
SEIS_SURFACE_DATA_TYPE data_type;
FULL_LINE_NAME *line;
CHAR *source;
SEIS_ATTRIBUTE_DATA_TYPE attribute;
SEIS_SURFACE_NLIST_TYPE format;
INT version;
#endif
    {
    
    return sez_del (surf_code, surf_type, data_type, line, source, attribute, format,
                    version);
    
    }
/* END:     */
