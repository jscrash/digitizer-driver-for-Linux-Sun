/* DEC/CMS REPLACEMENT HISTORY, Element HO_DELETE_DIR.C*/
/*  7     9-MAR-1992 09:34:51 LOEBL "(SPR 6056) Correctly name VMS directory before deleting."*/
/* *6     9-NOV-1990 10:33:11 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames"*/
/* *5    30-OCT-1990 14:15:52 GILLESPIE "(SPR 37) Add nostandard pragmas for vms"*/
/* *4    17-AUG-1990 21:50:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    26-FEB-1990 18:58:23 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2    27-OCT-1989 11:42:16 GILLESPIE "(SPR 1) Fix ref to descrip.h (put in double quotes)"*/
/* *1    19-JUN-1989 12:57:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_DELETE_DIR.C*/
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

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifdef primos
#include "keys.ins.cc"
#endif

#ifdef vms
#include "descrip.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Deletes an indicated directory.  If there are sub-directories
    or files, all of them will be deleted too.                      

Prototype:
    publicdef INT ho_delete_directory(PATHNAME directory_spec);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) directory_spec  -(PATHNAME ) The indicated directory.   

Return Value/Status:
    SUCESS.                                                     
    VMS CODE_VALUES.                                            

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_delete_directory (PATHNAME directory_spec) 
#else
publicdef INT ho_delete_directory (directory_spec)
PATHNAME directory_spec;
#endif
    {
#ifdef vms
    INT status;
    INT mode;
    INT i = 0;
    USHORT current_dir_len = 0;
    PATHNAME current_dir;
    FILENAME deleted_file;
    PATHNAME deleted_dir;
    PATHNAME buffer_dir;
    CHAR *pbracket, *pdot;
    CHAR **son_dirs;
    CHAR **files_in_dir;
#pragma nostandard
    $DESCRIPTOR (deleted_dir_desc, deleted_dir);
    $DESCRIPTOR (directory_spec_desc, directory_spec);
    $DESCRIPTOR (current_dir_desc, current_dir);
    $DESCRIPTOR (deleted_file_desc, deleted_file);
#pragma standard
    
    strcpy ((char *)buffer_dir, (char *)directory_spec);
    
    /* First, set the default directory*/
    /* to the specified directory.     */
    /* construct valid directory.      */
    
    pbracket = strchr ((char *)buffer_dir, ']');
    if (pbracket IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        pdot = strchr ((char *)pbracket, '.');
        if (pdot IS_NOT_EQUAL_TO (CHAR *)NULL)
            {
            *pdot = 0;
            *pbracket = '.';
            sprintf ((char *)deleted_dir, "%s]", (char *)buffer_dir);
            }
        else
            {
            if (strlen ((char *)buffer_dir) EQUALS (pbracket - buffer_dir + 1))
                strcpy ((char *)deleted_dir, (char *)buffer_dir);
            else
                {
                *pbracket = '.';
                sprintf ((char *)deleted_dir, "%s]", (char *)buffer_dir);
                }
            }
        current_dir_desc.dsc$w_length = sizeof current_dir;
        deleted_dir_desc.dsc$w_length = strlen ((char *)deleted_dir);
        status = VAX_ROUTINE (sys$setddir (&deleted_dir_desc, &current_dir_len,
                                           &current_dir_desc));
        }
    else
        {
        pdot = strchr ((char *)buffer_dir, '.');
        if (pdot IS_NOT_EQUAL_TO (CHAR *)NULL)
            {
            *pdot = 0;
            }
        sprintf ((char *)deleted_dir, "[.%s]", (char *)buffer_dir);
        
        }
    sprintf ((char *)buffer_dir, "%s*.DIR", (char *)deleted_dir);
    status = ho_find_pathnames (buffer_dir, &son_dirs);
    if (status)
        return status;
    
    /* go through all son-directories */
    /* to delete its son_directories. */
    while (son_dirs[i] IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        status = ho_delete_directory (son_dirs[i++]);
        }
    ts_tcp_free (son_dirs);
    
    /* set the deleted mode.           */
    
    mode = 0001 | 0002 | 0004 | 0010 | 0020 | 0040 | 0100 | 0200 | 0400;
    
    /*  delete files in the specified  */
    /*  directory.                     */
    
    sprintf ((char *)buffer_dir, "%s*.*", (char *)deleted_dir);
    status = ho_find_pathnames (buffer_dir, &files_in_dir);
    if (status)
        return status;
    
    i = 0;
    while (files_in_dir[i] IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        status = ho_change_protection (files_in_dir[i], mode);
        strcpy ((char *)deleted_file, (char *)files_in_dir[i]);
        deleted_file_desc.dsc$w_length = strlen (files_in_dir[i++]);
        status = VAX_ROUTINE (lib$delete_file (&deleted_file_desc, 0, 0, 0, 0, 0, 0, 0,
                                               0));
        }
    ts_tcp_free (files_in_dir);
    
    /* reset the default directory to  */
    /* the previous current directory. */
    
    deleted_dir_desc.dsc$w_length = sizeof deleted_dir;
    current_dir_desc.dsc$w_length = current_dir_len;
    current_dir[current_dir_len] = 0;
    status = VAX_ROUTINE (sys$setddir (&current_dir_desc, &current_dir_len,
                                       &deleted_dir_desc));
    
    /*  delete the specified directory */
    
    status = ho_change_protection (directory_spec, mode);
    directory_spec_desc.dsc$w_length = strlen (directory_spec);
    status = VAX_ROUTINE (lib$delete_file (&directory_spec_desc, 0, 0, 0, 0, 0, 0, 0,
                                           0));
    
    if (status & 1)
        return SUCCESS;
    else
        return status;
#endif

#ifdef primos
    struct shortcharvar
        {
        SHORT len;
        CHAR chrs[32];
        };
    
    struct charvar
        {
        SHORT len;
        CHAR chrs[256];
        };
    
    struct 
        {                           /* directory entry returned by DIR$LS */
        USHORT ecw;
        struct shortcharvar filename;
        USHORT protection;
        USHORT fileinfo;
        INT dtm;                    /* date and time modified (fsdate) */
        USHORT non_dflt_acl;        /* bit(1) aligned */
        USHORT spare;
        } dentry, *dp;
    
    fortran at$(), dir$ls (), srch$$(), gpath$();
    SHORT pathlen, tlen;
    CHAR origin[256];               /* original directory */
    CHAR tpath[128];
    SHORT funit, type, wcount;
    SHORT action, filnam;
    USHORT init;
    USHORT desired_types;
    SHORT max_entries;
    SHORT entry_size;
    SHORT rtncnt;
    SHORT typcnt[4];
    SHORT code;                     /* returned status code from DIR$LS */
    SHORT szero = 0;
    SHORT object;
    BOOL stayhere = FALSE;
    INT lzero = 0;
    INT status;
    CHAR sub_dir[128], doomed_file[128];
    struct charvar dirpath, originpath, wildcard, *wildp;
    
    wildp = &wildcard;
    wildp->len = 2;
    strcpy (wildp->chrs, "@@");
    dp = &dentry;
    
    action = k$cura;
    tlen = sizeof(origin);
    gpath$(action, szero, (CHAR[]) ORigin, tlen, pathlen, code);
    origin[pathlen] = '\0';
    strcpy (originpath.chrs, origin);
    originpath.len = strlen (originpath.chrs);
    
    ts_sto_upper (directory_spec);  /* must be upper case */
    
    /* if dir spec is only "current directory" don't do anything.  */
    if (strcmp (directory_spec, "*") == 0)
        return SUCCESS;
    
    /* if it's a relative pathname expand it */
    if ((directory_spec[0] == '*') && (directory_spec[1] == '>'))
        sprintf (dirpath.chrs, "%s>%s", origin, &directory_spec[2]);
    else
        strcpy (dirpath.chrs, directory_spec);
    dirpath.len = strlen (dirpath.chrs);
    
    action = k$setc;
    at$(action, dirpath.len, code); /* attach to directory */
    
    if (code)
        {
        action = k$setc;
        at$(action, originpath.len, code);  /* return to origin */
        return(FAIL);
        }
    tlen = sizeof(tpath);
    action = k$cura;
    gpath$(action, szero, (CHAR[])tpath, tlen, pathlen, code);
    tpath[pathlen] = '\0';
    
    action = k$read + k$getu;       /* get new directory unit */
    object = k$curr;
    srch$$(action, object, szero, funit, type, code);
    if (code || type != 4)
        return(FAIL);
    
    code = 0;
    type = 4;
    init = -1;                      /* turn all the bits on */
    desired_types = -1;
    max_entries = wcount = 1;
    entry_size = 24;
    
    dir$ls (funit, type, init, desired_types, wildp, wcount, dp, max_entries,
            entry_size, rtncnt, typcnt, lzero, lzero, code);
    
    init = 0;
    
    while (!code && rtncnt)
        {
        if ((dentry.fileinfo & 0xff) == 4)
            {                       /* a directory */
            dentry.filename.chrs[dentry.filename.len] = '\0';
            sprintf (sub_dir, "%s>%s", tpath, dentry.filename.chrs);
            ho_delete_directory (sub_dir);
            }
        else
            {
            dentry.filename.chrs[dentry.filename.len] = '\0';
            strcpy (doomed_file, dentry.filename.chrs);
            ho_delete_file (doomed_file, &status);
            if (status)
                {
                action = k$clos;
                srch$$(action, object, szero, funit, type, code);
                return(status);
                }
            }
        dir$ls (funit, type, init, desired_types, wildp, wcount, dp, max_entries,
                entry_size, rtncnt, typcnt, lzero, lzero, code);
        }
    action = k$clos;
    srch$$(action, object, szero, funit, type, code);
    if (code)
        return(code);
    code = 0;
    action = k$setc;
    at$(action, originpath.len, code);  /* return to origin */
    if (code)
        return(code);
    ho_delete_file (directory_spec, &status);   /* delete the directory */
    if (status)
        return(status);
    
    return(SUCCESS);

#endif

#if UNIX

#include "esi_stat.h"
#include <dirent.h>
    INT status;
    CHAR *tcp;
    CHAR curentpath[256];
    DIR *dirptr = (DIR *)0;
    struct dirent *entptr;
    struct stat statbuf;
    
    dirptr = opendir (directory_spec);
    if (dirptr == (DIR *)0)
        return FAIL;
    
    for (entptr = readdir (dirptr); entptr != (struct dirent *)0;
                                                                           entptr =
                                                                           readdir (
                                                                                dirptr))
                                                                           
        {
        sprintf (curentpath, "%s/%s", directory_spec, entptr->d_name);
        status = stat (curentpath, &statbuf);
        if (status != 0)
            break;
        if (statbuf.st_mode & S_IFDIR)  /* it's a directory */
            {
            if (strcmp (entptr->d_name, ".") && strcmp (entptr->d_name, ".."))
                {
                status = ho_delete_directory (curentpath);
                if (status != SUCCESS)
                    break;
                }
            }
        else                        /* assume it's a file */
            {
            ho_delete_file (curentpath, &status);
            if (status != SUCCESS)
                break;
            }
        }
    closedir (dirptr);
    status = rmdir (directory_spec);
    return status;
    

#endif    
    }
/* END:     */
