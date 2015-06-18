/* DEC/CMS REPLACEMENT HISTORY, Element HO_RENAME.C*/
/* *3     9-NOV-1990 14:12:15 GILLESPIE "(SPR 1) Add #pragmas to quite compiles on VMS"*/
/* *2    17-AUG-1990 21:53:15 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:59:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_RENAME.C*/
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

#include "esi_gl_defs.h"
#include "esi_ho_files.h"

#ifdef vms

#include "esi_ho_err.h"
#include "esi_ho_desc.h"

#endif

#ifdef primos

#include "keys.ins.cc"

#endif

/* Function Description -----------------------------------------------------
Description:
    Given a file specification, return a list of all files      
    which statisfy the request.                               

Prototype:
    publicdef INT ho_rename (FILENAME old_file_spec, FILENAME new_file_spec);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) old_file_spec   -(FILENAME) Filename containing file's current name.     
    (I) new_file_spec   -(FILENAME) Filename containing file's new name.         

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_rename (FILENAME old_file_spec, FILENAME new_file_spec) 
#else
publicdef INT ho_rename (old_file_spec, new_file_spec)
FILENAME old_file_spec;
FILENAME new_file_spec;
#endif
    {                               /* start of HO_RENAME */

#if UNIX
    INT status;
    
    return status = rename (old_file_spec, new_file_spec);

#else       /* not UNIX */

#ifdef vms
    INT status;
    FILENAME old_file;
    FILENAME new_file;
#pragma nostandard
    $DESCRIPTOR (old_file_desc, old_file);
    $DESCRIPTOR (new_file_desc, new_file);
#pragma standard
    
    strcpy (old_file, old_file_spec);
    strcpy (new_file, new_file_spec);
    old_file_desc.dsc$w_length = strlen (old_file_spec);
    new_file_desc.dsc$w_length = strlen (new_file_spec);
    
    status = VAX_ROUTINE (lib$rename_file (&old_file_desc, &new_file_desc, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0));
    if (status & 1)
        return SUCCESS;
    else
        return status;

#endif      /* VMS */

#ifdef primos
    SHORT stat;
    fortran cnam$$(), at$(), at$hom ();
    
    struct 
        {
        SHORT len;
        CHAR chrs[128];
        } newpath;
    
    SHORT status, filelen, action, code;
    FILENAME xold_file, xnew_file;
    BOOL stayhere;
    CHAR *rindex (), *op, *np;
    
    stayhere = TRUE;
    status = 0;
    
    strcpy (xold_file, old_file_spec);
    strcpy (xnew_file, new_file_spec);
    
    if (op = rindex (xold_file, '>'))
        {
        stayhere = FALSE;
        *op++ = '\0';
        strcpy (newpath.chrs, xold_file);
        newpath.len = strlen (newpath.chrs);
        np = rindex (xnew_file, '>');
        *np++ = '\0';
        action = k$setc;
        at$(action, newpath.len, code); /* attach to directory */
        if (code)
            {
            status = code;
            at$hom (code);
            return(INT)status;
            }
        }
    else
        {
        op = xold_file;
        np = xnew_file;
        }
    cnam$$((char [])op, (SHORT)strlen (op), (char [])np, (SHORT)strlen (np), stat);
    
    if (!stayhere)
        at$hom (status);
    
    return(INT)stat;

#endif      /* PRIMOS */
#endif      /* UNIX */
    }                               /* end of HO_RENAME */
/* END:     */
