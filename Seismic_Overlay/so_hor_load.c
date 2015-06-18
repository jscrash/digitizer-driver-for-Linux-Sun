/* DEC/CMS REPLACEMENT HISTORY, Element SO_HOR_LOAD.C*/
/* *11   12-OCT-1990 17:56:00 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/*  9B1  12-OCT-1990 17:55:18 GILLESPIE "Merge Ernie Deltas"*/
/* *10   31-AUG-1990 10:23:26 JULIAN "(SPR 0) add return version number if version=0"*/
/* *9    26-JUL-1990 17:20:57 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *8    26-JUL-1990 11:21:40 VINCE "(SPR 5644) Fixed compile errors introduced by the Header change."*/
/* *7    25-JUL-1990 20:04:22 VINCE "(SPR 5644) Header Standardization"*/
/* *6     1-JUL-1990 12:33:57 GILLESPIE "(SPR 1) Changes to allow ANY surface type to be retrieved; walk tree of types"*/
/* *5    29-JUN-1990 19:04:12 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *4    23-FEB-1990 13:18:32 GILLESPIE "(SPR 1) Change horizon_nlist to nlist in seis_surface structure"*/
/* *3    14-SEP-1989 16:20:46 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    25-JUL-1989 20:06:25 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_HOR_LOAD.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_HOR_LOAD.C*/
/* *9    26-JUL-1990 17:20:57 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *8    26-JUL-1990 11:21:40 VINCE "(SPR 5644) Fixed compile errors introduced by the Header change."*/
/* *7    25-JUL-1990 20:04:22 VINCE "(SPR 5644) Header Standardization"*/
/* *6     1-JUL-1990 12:33:57 GILLESPIE "(SPR 1) Changes to allow ANY surface type to be retrieved; walk tree of types"*/
/* *5    29-JUN-1990 19:04:12 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *4    23-FEB-1990 13:18:32 GILLESPIE "(SPR 1) Change horizon_nlist to nlist in seis_surface structure"*/
/* *3    14-SEP-1989 16:20:46 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    25-JUL-1989 20:06:25 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_HOR_LOAD.C*/
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
#include "esi_so.h"

#ifndef ESI_SE_H

#include "esi_se.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#if USE_PROTOTYPES
static INT sez_fetch (FULL_LINE_NAME *pName, SEIS_SURFACE_HDR *pSurface,
                          SEIS_SURFACE_DATA_TYPE *tried_types);
#else
static INT sez_fetch ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    Function to load horizon nlists for posting.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT so_hor_load(SO_STRUCT *pso);

Private_Functions:
    static INT sez_fetch(FULL_LINE_NAME *pName, SEIS_SURFACE_HDR *pSurface,
        SEIS_SURFACE_DATA_TYPE *tried_types);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT so_hor_load(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso   - (SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_hor_load (SO_STRUCT *pso)
#else
publicdef INT so_hor_load (pso)
SO_STRUCT *pso;
#endif
    {
    INT status = SUCCESS;
    SEIS_SURFACE_HDR *pSurface = &pso->seismic_surface;
    SEIS_SURFACE_DATA_TYPE tried_types[10];
    
    /* read horizon ... seismic_surface type */
    
    tc_zeroes ((VOIDPTR)tried_types, sizeof tried_types);
    
    if (IS_STRING (pSurface->name))
        {
        pSurface = &pso->seismic_surface;
        pSurface->data_type = pso->seismic_generic.surface_data_type;
        strcpy ((char *)pSurface->source, (char *)pso->seismic_generic.source);
        status = sez_fetch (&pso->current_line, pSurface, tried_types);
        
        if (status EQUALS SUCCESS)
            {
            /* save in b tree if mistie map                                             */
            
            if (pso->ov_type == SO_SEISMIC_MISTIE AND status == SUCCESS)
                {
                so_tie_tree (pso);
                }
            }
        return status;
        }
    /* read horizon ... seimic interval type                                        */
    
    if (IS_STRING (pso->seismic_interval.top_horizon.name))
        {
        pSurface = &pso->seismic_interval.top_horizon;
        pSurface->data_type = pso->seismic_generic.surface_data_type;
        strcpy ((char *)pSurface->source, (char *)pso->seismic_generic.source);
        status = sez_fetch (&pso->current_line, pSurface, tried_types);
        }
    else
        {
        return FAIL;
        }
    if (IS_STRING (pso->seismic_interval.base_horizon.name))
        {
        tc_zeroes ((VOIDPTR)tried_types, sizeof tried_types);
        pSurface = &pso->seismic_interval.base_horizon;
        /* Note that we start with the same data type found for the top surface */
        pSurface->data_type = pso->seismic_interval.top_horizon.data_type;
        strcpy ((char *)pSurface->source, (char *)pso->seismic_generic.source);
        status = sez_fetch (&pso->current_line, pSurface, tried_types);
        }
    else
        {
        return FAIL;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    The following function handles the case where several data classes are
    similar, and we don't care (for now) whether time is generic, snapped or
    unsnapped or whatever...
    
Prototype:
    static INT sez_fetch(FULL_LINE_NAME *pName, SEIS_SURFACE_HDR *pSurface,
        SEIS_SURFACE_DATA_TYPE *tried_types);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pName           -(FULL_LINE_NAME *)
    pSurface        -(SEIS_SURFACE_HDR *)
    tried_types     -(SEIS_SURFACE_DATA_TYPE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <so_hor_load.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sez_fetch (FULL_LINE_NAME *pName, SEIS_SURFACE_HDR *pSurface,
                          SEIS_SURFACE_DATA_TYPE *tried_types)
#else
static INT sez_fetch (pName, pSurface, tried_types)
FULL_LINE_NAME *pName;
SEIS_SURFACE_HDR *pSurface;
SEIS_SURFACE_DATA_TYPE *tried_types;
#endif
    {
    INT status;
    FLOAT upper_time, lower_time;
    CDP_FLOAT first_cdp, last_cdp;
    INT i,temp_version;
    SEIS_SURFACE_DATA_TYPE next_type;
    
    temp_version = pSurface->version;
    status = se_get (&pSurface->nlist, (CHAR *)pSurface->name, SE_HORIZ, pSurface->data_type,
                     pName, (CHAR *)pSurface->source, SE_AT_NM, SE_FINDER, 
		     &temp_version,
                     &upper_time, &lower_time, &first_cdp, &last_cdp);
    
    if (status EQUALS SUCCESS)
        {
        pSurface->first_cdp = (CDP)first_cdp;
        pSurface->last_cdp = (CDP)last_cdp;
        pSurface->upper_time = upper_time;
        pSurface->lower_time = lower_time;
        return status;
        }
    /* We get here because the requested horizon was not found.  Try another
    data type, based on the previous type.  If we hit the end of the appropriate
    type list without success, then we can't be faulted for not trying ... */
    
    switch (pSurface->data_type)
        {
    case SE_TIME:
        next_type = SE_TIME_REFINED;
        break;
    case SE_TIME_REFINED:
        next_type = SE_UNSNAPPED_TIME;
        break;
    case SE_UNSNAPPED_TIME:
        next_type = SE_UNKNOWN;
        break;
    case SE_UNKNOWN:
        next_type = SE_TIME;
        break;
    default:
        return status;
        }
    /* The following implements a ring buffer.  We start somewhere in the ring
    and visit each node (defined by the above case statement).  If we ever get to the
    point where we try to access a type that has already been searched for, then
    all of the types for this group have been visited (unsuccessfully) */
    
    for (i = 0; i < 10; i++)
        {
        if ((INT)tried_types[i] EQUALS (INT)0)
            {
            tried_types[i] = pSurface->data_type;
            break;
            }
        }
    for (i = 0; i < 10; i++)
        {
        if ((INT)tried_types[i] EQUALS (INT)0)
            {
            /* Scan the list of attempts.  If this one isn't found, then try another fetch */
            pSurface->data_type = next_type;
            return sez_fetch (pName, pSurface, tried_types);
            }
        if (tried_types[i] EQUALS next_type)
            {
            /* We've tried all of the data types in this set.  Let the calling
            function have the bad news... */
            return status;
            }
        }
    }
/* END:     */
