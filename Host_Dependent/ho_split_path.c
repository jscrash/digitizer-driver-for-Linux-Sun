/* DEC/CMS REPLACEMENT HISTORY, Element HO_SPLIT_PATH.C*/
/* *3    17-AUG-1990 21:53:33 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    12-APR-1990 17:09:48 GILLESPIE "(SPR 1) For Unix systems, remove trailing separator from path name"*/
/* *1    19-JUN-1989 12:59:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_SPLIT_PATH.C*/
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

#include "esi_c_lib.h"
#include "esi_ho_files.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to extract the path and/or filename parts from a
    pathname.  If either the pathpart or filepart arguments are NULL,
    that one is not returned.

Prototype:
    publicdef INT ho_split_pathname(PATHNAME pathname, PATHNAME pathpart, FILENAME filepart);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pathname    -(PATHNAME) The pathname to parse.
    (O) pathpart    -(PATHNAME) The path part of the pathname.
    (O) filepart    -(FILENAME) The file part of the pathname.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    Unix systems: the trailing / is NOT included in the path name, e.g.
    A/B/C has pathpart A/B and and filepart C.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_split_pathname (PATHNAME pathname, PATHNAME pathpart,
                                 FILENAME filepart) 
#else
publicdef INT ho_split_pathname (pathname, pathpart, filepart)
PATHNAME pathname;
PATHNAME pathpart;
FILENAME filepart;
#endif
    {
    
    CHAR *p;
    INT len = 0;
    
    if (pathname == NULL)
        return FAIL;

#ifdef vms
    p = strrchr (pathname, ']');
    if (p == NULL)
        p = strrchr (pathname, ':');    /* Check if path is a logical */
    
    if (p != NULL)
        len = p - pathname;
    
    if (pathpart != NULL)           /* Return path portion */
        {
        if (len > 0)
            {
            if (*p == ']')          /* The ']' gets returned as part of path */
                ++len;
            strncpy (pathpart, pathname, len);
            pathpart[len] = '\0';
            }
        else
            *pathpart = '\0';       /* There was no path */
        }
    if (filepart != NULL)           /* Return filename part */
        {
        if (p != NULL)
            strcpy (filepart, p + 1);
        else
            strcpy (filepart, pathname);
        }
#endif

#ifdef primos
    p = strrchr (pathname, '>');
    
    if (p != NULL)
        len = (p - pathname);
    
    if (pathpart != NULL)
        {
        if (len > 0)
            {
            strncpy (pathpart, pathname, len);
            pathpart[len] = '\0';
            }
        else
            *pathpart = '\0';
        }
    if (filepart != NULL)
        {
        if (p != NULL)
            strcpy (filepart, p + 1);
        else
            strcpy (filepart, pathname);
        }
    return SUCCESS;

#endif

#if UNIX
    p = strrchr (pathname, '/');
    
    if (p != NULL)
        len = (p - pathname);
    
    if (pathpart != NULL)
        {
        if (len > 0)
            {
            strncpy (pathpart, pathname, len);
            pathpart[len] = '\0';
            }
        else
            *pathpart = '\0';
        }
    if (filepart != NULL)
        {
        if (p != NULL)
            strcpy (filepart, p + 1);
        else
            strcpy (filepart, pathname);
        }
    return SUCCESS;

#endif
    }
/* END:     */
