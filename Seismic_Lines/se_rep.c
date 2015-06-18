/* DEC/CMS REPLACEMENT HISTORY, Element SE_REP.C*/
/* *6    17-AUG-1990 22:17:49 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    21-MAY-1990 17:37:06 CHARLIE "(SPR 1) Change calling sequence to set_put"*/
/* *4     2-NOV-1989 09:53:28 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *3    14-SEP-1989 16:20:27 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    25-JUL-1989 20:03:43 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:25:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SE_REP.C*/
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

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#include "esi_se_err.h"

/* Function Description -----------------------------------------------------
Description:
    Used to store  seismic surface data in SEIS_SURFACE. 
    If there is already the same data it is replaced.

Prototype:
    publicdef  INT se_rep (NLIST_HEADER surface_nlist , CHAR *surf_code , SEIS_SURFACE_DATA_CLASS surf_type , 
        SEIS_SURFACE_DATA_TYPE data_type,FULL_LINE_NAME *line, CHAR *source , 
        SEIS_ATTRIBUTE_DATA_TYPE attribute , SEIS_SURFACE_NLIST_TYPE format,INT version);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    surface_nlist   -(NLIST_HEADER) Pointer to nlist containing the seismic data surface
                        to be stored.    
                        (THE FORMAT OF THE NLIST IS NOT MODIFIED BY THIS FUNCTION)
    surf_code       -(CHAR *) Name of the surface to be stored. 
    surf_type       -(SEIS_SURFACE_DATA_CLASS) Type of the surface to be stored. 
    data_type       -(SEIS_SURFACE_DATA_TYPE) Type of the data  stored for the surface.     
    line            -(FULL_LINE_NAME *) Line name and survey structure pointer.
    source          -(CHAR *) Source to which pertains the data. 
    attribute       -(SEIS_ATTRIBUTE_DATA_TYPE) Gives the attribute of the seismic traces 
                        where data was picked.
    format          -(SEIS_SURFACE_NLIST_TYPE) Format in which the data is stored in 
                        the nlist.
    version         -(INT) Version number for the pick.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_rep (NLIST_HEADER surface_nlist, CHAR *surf_code,
                      SEIS_SURFACE_DATA_CLASS surf_type,
                      SEIS_SURFACE_DATA_TYPE data_type, FULL_LINE_NAME *line,
                      CHAR *source, SEIS_ATTRIBUTE_DATA_TYPE attribute,
                      SEIS_SURFACE_NLIST_TYPE format, INT version)
#else
publicdef INT se_rep (surface_nlist, surf_code, surf_type, data_type, line, source,
                      attribute, format, version)
NLIST_HEADER surface_nlist;
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
    
    INT istatus;
    
    /*   try to load the data   */
    
    istatus = se_put (surface_nlist, surf_code, surf_type, data_type, line, source,
                      attribute, format, version);
    
    /*   if status returned is SE_DUPLI  
         we delete the row and then add the row again */
    
    if (istatus == SE_DUPLI)
        {
        
        istatus = se_del (surf_code, surf_type, data_type, line, source, attribute,
                          format, version);
        
        if (istatus EQUALS SUCCESS)
            {
            
            /*   try to load the data   */
            version++;
            
            istatus = se_put (surface_nlist, surf_code, surf_type, data_type, line,
                              source, attribute, format, version);
            }
        }
    return istatus;
    
    }
/* END:     */
