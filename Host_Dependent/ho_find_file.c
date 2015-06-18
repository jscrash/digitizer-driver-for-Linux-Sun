/* DEC/CMS REPLACEMENT HISTORY, Element HO_FIND_FILE.C */
/* *3     2-AUG-1991 17:16:24 JESSIE "(SPR 8724) Add ts_tcp_sort function to sort tcp list case#3416" */
/* *2     9-NOV-1990 10:33:23 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames" */
/* *1     9-NOV-1990 10:00:46 MING "REPLACEMENT OF HO_FIND_FINES.C" */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_FIND_FILE.C */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_FIND_FILES.C*/
/* *8    30-OCT-1990 14:16:07 GILLESPIE "(SPR 37) Add nostandard pragmas for vms"*/
/* *7    17-AUG-1990 21:51:24 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    23-JUL-1990 11:46:43 VINCE "(SPR 1) final fixes for recursive logical names on Unix"*/
/* *5    28-JUN-1990 10:21:13 MING "(SPR -1) fix bug on SUN (add check if ho_find_files2 return (CHAR **) NULL ptr"*/
/* *4    19-JUN-1990 16:19:20 MING "(SPR 0) return pathname for sure"*/
/* *3    26-FEB-1990 18:58:39 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2    18-JUL-1989 15:31:40 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:58:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_FIND_FILES.C*/
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
#include "esi_ho_files.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_ho_err.h"

#ifdef vms

#include "descrip.h"
#include "rmsdef.h"

#endif

#ifdef primos
#include "keys.ins.cc"
#endif

#if UNIX

#include <sys/types.h>
#include <sys/dir.h>
#include "esi_stat.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Given a file specification, return a list of all files      
    which statify the request.  Note that the tcp space        
    which is allocated in this routine is not freed by it       
    so that is the responsibility of the caller.               
    The path name is included in each file in returned tcp.     

Prototype:
    publicdef INT ho_find_filenames (FILENAME file_spec, CHAR ***tcp);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) file_spec   -(FILENAME) The file specification.             
    (M) tcp         -(CHAR ***) A pointer points the TCP table.       

Return Value/Status:
    SUCCESS.                                                    
    OS-DEPENDENT CODES.                                      

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_find_filenames (FILENAME file_spec, TCP *tcp) 
#else
publicdef INT ho_find_filenames (file_spec, tcp)
FILENAME file_spec;
TCP *tcp;
#endif
    {
    INT status;
    TCP table = (TCP)0;

#ifdef vms
    INT flags = 2;                  /* allows wild card.            */
    INT stv;
    INT context = 0;
    size_t len;
    FILENAME result_file;
    FILENAME default_file;
    FILENAME related_file;
    CHAR *p;
    CHAR *t;
    
#pragma nostandard
    $DESCRIPTOR (source_file_desc, file_spec);
    $DESCRIPTOR (result_file_desc, result_file);
    $DESCRIPTOR (default_file_desc, default_file);
    $DESCRIPTOR (related_file_desc, related_file);
#pragma standard
    
    /* find the number of satisfied file*/
    
    strcpy ((char *)default_file, "*.*;0");
    strcpy ((char *)related_file, "");
    source_file_desc.dsc$w_length = strlen ((char *)file_spec);
    result_file_desc.dsc$w_length = sizeof result_file - 1;
    default_file_desc.dsc$w_length = strlen (default_file);
    related_file_desc.dsc$w_length = strlen (related_file);
    
    while ((status = lib$find_file (&source_file_desc, &result_file_desc, &context,
                                    &default_file_desc, &related_file_desc, &stv,
                                    &flags)) EQUALS RMS$_NORMAL)
        {
        result_file[result_file_desc.dsc$w_length] = 0;
        len = strlen ((char *)ts_snowhite (result_file));
        p = (CHAR *)tc_alloc (len + 1);
        strcpy ((char *)p, (char *)result_file);
    	t = strrchr (p,']') + 1;
        ts_tcp_append (&table, t);
        }
    lib$find_file_end (&context);
    
    if (status EQUALS RMS$_NMF OR status & 1)   /* NMF = No More Files */
        {
        *tcp = table;
        return SUCCESS;
        }
    else
        {
        *tcp = (TCP)0;
        return HO_ERR_VMS;
        }
#endif

#ifdef primos

#define SAMFILE 0
#define DAMFILE 1
#define UFD     4
    struct charvar
        {
        SHORT len;
        CHAR chrs[128];
        };
    struct shortcharvar
        {
        SHORT len;
        CHAR chrs[32];
        };
    
    struct 
        {                           /* directory entry returned by DIR$LS */
        USHORT ecw;
        struct shortcharvar filename;
        USHORT protection;
        USHORT fileinfo;
        INT dtm;                    /* date and time modified (fsdate) */
        USHORT non_dflt_acl;        /* bit(1) aligned */
        INT spare1;
        INT spare2;
        INT spare3;
        } dentry, *dp;
    
    struct ltcp
        {
        struct ltcp *next;
        CHAR *name;
        }  *tp, *root_tcp;
    
    struct charvar wildcard, *wildp;
    
    fortran at$(), dir$ls (), srch$$();
    extern CHAR *strpbrk ();
    SHORT savedir, curdir;
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
    BOOL stayhere = TRUE;
    INT filetype;
    INT lzero = 0;
    INT filecount = 0;
    CHAR *savtp;
    CHAR *p;
    CHAR xfile_spec[128];           /* for local copy */
    CHAR curfile[32];
    CHAR origin_dir[128], new_dir[128], temp_dir[128];
    
    strcpy (xfile_spec, file_spec);
    wildp = &wildcard;
    dp = &dentry;
    *tcp = (TCP)NULL;           /* set TCP to empty   */
    
    ho_get_current_path (origin_dir);
    ts_sto_upper (xfile_spec);      /* must be upper case */
    
    /* get last token */
    
    for (p = xfile_spec + strlen (xfile_spec); p > xfile_spec; --p)
        {
        if (*p == '>')
            {
            ++p;
            break;
            }
        }
    /* change directories if necessary */
    
    if (p != xfile_spec)
        {
        *(p - 1) = '\0';
        if (strcmp (xfile_spec, "*"))
            {
            stayhere = FALSE;
            strcpy (new_dir, xfile_spec);
            if (status = ho_change_directory (new_dir, NULL))
                return FAIL;
            }
        }
    /* does the last token contain a wildcard character? */
    
    if (strpbrk (p, "@+^") == NULL)
        {
        
        /* is last token a file in current directory? */
        
        filetype = ftype (p);
        if (filetype == DAMFILE || filetype == SAMFILE)
            {
            
            /* if so, return it */
            
            *tcp = table = (TCP)tc_zalloc (sizeof(CHAR *) * 2);
            if (stayhere)
                {
                table[0] = tc_zalloc (strlen (p) + strlen (origin_dir) + 2);
                sprintf (table[0], "%s>%s", origin_dir, p);
                }
            else
                {
                table[0] = tc_zalloc (strlen (p) + strlen (new_dir) + 2);
                sprintf (table[0], "%s>%s", new_dir, p);
                }
            table[1] = NULL;
            if (!stayhere)
                return(ho_change_directory (origin_dir, NULL));
            return SUCCESS;
            }
        if (filetype != UFD)        /* if not SAM or DAM file, should be UFD */
            return FAIL;
        
        /* if not, go to the directory & process @@ */
        
        strcpy (temp_dir, new_dir);
        sprintf (new_dir, "%s>%s", temp_dir, p);
        status = ho_change_directory (new_dir, NULL);
        stayhere = FALSE;
        
        strcpy (wildp->chrs, "@@");
        wildp->len = 2;
        }
    else
        {
        strcpy (wildp->chrs, p);
        wildp->len = strlen (wildp->chrs);
        }
    /* process wildcard */
    
    action = k$read + k$getu;       /* get new directory unit */
    object = k$curr;
    srch$$(action, object, szero, funit, type, code);
    if (code || type != 4)
        return(FAIL);
    code = 0;
    action = k$read + k$getu;
    
    init = -1;
    desired_types = -1;
    max_entries = wcount = 1;
    entry_size = sizeof(dentry) / 2;
    
    dir$ls (funit, type, init, desired_types, wildp, wcount, dp, max_entries,
            entry_size, rtncnt, typcnt, lzero, lzero, code);
    if (code != SUCCESS || rtncnt == 0)
        {
        if (stayhere == FALSE)
            status = ho_change_directory (origin_dir, NULL);
        action = k$clos;
        srch$$(action, object, szero, funit, type, code);
        return FAIL;
        }
    init = 0;
    root_tcp = tp = 0;
    
    for (filecount = 0; code == 0 && rtncnt; )
        {
        if ((dentry.fileinfo & 0xff) <= 1)  /* a SAM or DAM file */
            {
            filecount++;
            /* keep tcp in linked list */
            if (!root_tcp)
                {
                root_tcp = (struct ltcp *)tc_zalloc (sizeof(struct ltcp));
                tp = root_tcp;
                }
            else
                {
                tp->next = (struct ltcp *)tc_zalloc (sizeof(struct ltcp));
                tp = tp->next;
                }
            tp->next = 0;
            strncpy (curfile, dentry.filename.chrs, dentry.filename.len);
            curfile[dentry.filename.len] = '\0';
            if (stayhere == FALSE)
                {
                tp->name = tc_zalloc (strlen (curfile) + strlen (new_dir) + 2);
                sprintf (tp->name, "%s>%s", new_dir, curfile);
                }
            else
                {
                tp->name = tc_zalloc (strlen (curfile) + 1);
                strcpy (tp->name, curfile);
                }
            }
        dir$ls (funit, type, init, desired_types, wildp, wcount, dp, max_entries,
                entry_size, rtncnt, typcnt, lzero, lzero, code);
        }
    /* create a straight tcp from the linked tcp */
    *tcp = table = (TCP)tc_zalloc (sizeof(CHAR *) * (filecount + 1));
    tp = root_tcp;
    while (tp)
        {
        *table++ = tp->name;
        savtp = (CHAR *)tp;
        tp = tp->next;
        tc_free ((VOIDPTR)savtp);
        }
    if (*tcp)
        ts_tcp_sort (*tcp);         /* alpha sort tcp */
    action = k$clos;
    srch$$(action, object, szero, funit, type, code);
    
    /* go home, if needed */
    if (!stayhere)
        {
        status = ho_change_directory (origin_dir, NULL);
        if (status)
            return status;
        }
    if (code)
        return(code);
    return(SUCCESS);

#endif

#if UNIX
    DIR *dirp;
    struct direct *dp;
    UINT count = 0;
    struct stat statbuf;
    FILENAME parent_dir, comp_string, temp;
    CHAR *cp;
    CHAR *getenv ();
    
    strcpy (parent_dir, file_spec); /* make a local copy            */
    
    if (parent_dir[0] == '~')
        {                           /* translate the C-Shell home-dir character */
        strcpy (temp, parent_dir + 1);
        *parent_dir = NUL;
        cp = getenv ("HOME");
        if (cp != NULL)
            strcpy (parent_dir, cp);
        strcat (parent_dir, temp);
        }
    if ((cp = strrchr (parent_dir, '/')) == NULL)   /* no pathname; find local files*/
        {
        strcpy (comp_string, parent_dir);
        strcpy (parent_dir, ".");
        }
    else
        {
        *cp++ = '\0';               /* terminate the parent directory path  */
        strcpy (comp_string, cp);   /* move the wildcarded filename     */
        }
    if ((dirp = opendir (parent_dir)) == (DIR *)NULL)
        return FAIL;
    
    while ((dp = readdir (dirp)) != (struct direct *)NULL)
        {
        if (*dp->d_name != '.' && ts_like (dp->d_name, comp_string, '*'))
            {
            ts_tcp_append (&table, dp->d_name);
            count++;
            }
        }
    closedir (dirp);
    
    if (count > 0)
        *tcp = table;
    else
        *tcp = (TCP)0;
    
    ts_tcp_sort(*tcp);
    return SUCCESS;

#endif /* UNIX */
    }
/* END:     */
