/* DEC/CMS REPLACEMENT HISTORY, Element HO_CREATE_DIR.C*/
/* *6    30-SEP-1990 10:27:16 GILLESPIE "(SPR 1) Suppress portability warnings for vms"*/
/* *5    17-AUG-1990 21:50:35 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    26-FEB-1990 18:58:09 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *3     5-SEP-1989 15:48:02 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    18-JUL-1989 15:30:19 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:57:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_CREATE_DIR.C*/
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

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_HO_ERR_H
#include "esi_ho_err.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifdef vms
#include "descrip.h"
#endif

#ifdef primos
#include "keys.ins.cc"
#endif

/* Function Description -----------------------------------------------------
Description:
    Creates a indicated directory.                              

Prototype:
    publicdef INT ho_create_directory(PATHNAME directory_spec);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) directory_spec  -(PATHNAME) The indicated directory.    

Return Value/Status:
    SUCESS.                                                     
    VMS CODE_VALUES.                                           

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_create_directory (PATHNAME directory_spec) 
#else
publicdef INT ho_create_directory (directory_spec)
PATHNAME directory_spec;
#endif
    {

#ifdef vms
    INT status;
#pragma nostandard
    $DESCRIPTOR (directory_spec_desc, directory_spec);
#pragma standard
    
    directory_spec_desc.dsc$w_length = strlen ((char *)directory_spec);
    status = VAX_ROUTINE (lib$create_dir (&directory_spec_desc, 0, 0, 0, 0));
    
    if (status & 1)
        return SUCCESS;
    else
        return HO_ERR_VMS;
#endif

#ifdef primos
    struct charvar
        {
        SHORT len;
        CHAR str[128];
        };
    
    struct 
        {
        SHORT version;
        SHORT dir_type;
        INT max_quota;
        struct charvar access_cat;
        } attribute;
    
    SHORT *attptr;
    SHORT code;
    CHAR parent[200];
    struct charvar dirname;
    static INT hoz_get_parent ();
    
    fortran dir$cr ();
    
    /*  do nothing if directory_spec is "current_directory" */
    code = 0;
    if ((strlen (directory_spec) > 0) && (strcmp (directory_spec, "*") != SUCCESS))
        {
        /* get the parent dir spec,               */
        /* if the parent doesn't exist create it. */
        if (hoz_get_parent (parent, directory_spec) && (!fexists (parent)))
            ho_create_directory (parent);
        
        attribute.version = 1;
        attribute.dir_type = k$same;
        attribute.max_quota = 0;
        attribute.access_cat.len = 0;
        attribute.access_cat.str[0] = '\0';
        attptr = (SHORT *) & attribute;
        dirname.len = strlen (directory_spec);
        strncpy (dirname.str, directory_spec, 128);
        dirname.str[127] = 0;
        
        dir$cr (dirname, attptr, code);
        }
    if (code == 0)
        return(SUCCESS);
    else
        return HO_BAD_DIRECTORY;
#endif

#if UNIX
    INT status;
    PATHNAME parent;
    CHAR *cp;
    
    status = SUCCESS;
    
    /* do nothing if directory_spec is "current_directory"  */
    if ((strlen (directory_spec) == 0) || (strcmp (directory_spec, ".") == SUCCESS))
        return SUCCESS;
    
    /* check that parent directory exists; if not create it. */
    strcpy (parent, directory_spec);    /* move full spec to parent spec   */
    cp = strrchr (parent, '/');     /* look backward for a '/'       */
    if (cp)
        {
        *cp = NUL;                  /* if found truncate at that point */
        if (!hoexists (parent))
            status = ho_create_directory (parent);
        }
    /* if the directory exists don't bother to create it.  */
    if ((status == SUCCESS) && (!hoexists (directory_spec)))
        {
        status = mkdir (directory_spec, 0777);
        }
    if (status != SUCCESS)
        return HO_BAD_DIRECTORY;
    return SUCCESS;
#endif
    }


#ifdef primos
static INT hoz_get_parent (parent, full)
char *parent, *full;
    {
    char *cp, *rindex ();
    int valid_parent;
    
    strcpy (parent, full);          /* move full spec to parent spec. */
    
    cp = rindex (parent, '>');      /* truncate the parent spec. at the last > */
    if (cp)
        *cp = '\0';
    
    valid_parent = TRUE;            /* a valid parent will still have at least  */
    cp = rindex (parent, '>');      /* one more > in it.                        */
    if (cp == 0)
        valid_parent = FALSE;
    
    return(valid_parent);
    }


#endif
/* END:     */
