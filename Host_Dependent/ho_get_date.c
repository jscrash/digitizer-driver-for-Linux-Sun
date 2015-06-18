/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_DATE.C*/
/* *4    17-AUG-1990 21:51:53 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    12-FEB-1990 12:38:39 MING "(SPR 0) change date format to YYMMDD_HHMISS\0"*/
/* *2    18-JUL-1989 15:32:50 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:58:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_DATE.C*/
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
#include "esi_ho.h"

#ifdef vms

#include "fab.h"
#include "nam.h"
#include "xab.h"
#include "libdef.h"
#include "descrip.h"

#endif

#if UNIX

#include "esi_stat.h"
#include "esi_time.h"

#endif

#define DATE_SIZE 20

/* Function Description -----------------------------------------------------
Description:
    HO_GET_DATE subroutine is invoked by FM_INIT_FILE subroutine and
    is a host-dependent function.  It retrieves the file size, creation
    date and last modified date from the operating system.

Prototype:
    publicdef INT ho_get_date (FILENAME in_str, UINT *file_size, DATE_TIME create_date,
                                DATE_TIME mod_date);
                                
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) in_str      -(FILENAME) Filename to check.
    (O) file_size   -(UINT *) File size.
    (O) create_date -(DATE_TIME) Date of file creation.
    (O) mod_date    -(DATE_TIME) Date of last modification.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_get_date (FILENAME in_str, UINT *file_size, DATE_TIME create_date,
                           DATE_TIME mod_date) 
#else
publicdef INT ho_get_date (in_str, file_size, create_date, mod_date)
FILENAME in_str;
UINT *file_size;
DATE_TIME create_date;
DATE_TIME mod_date;
#endif
    {
    INT cond1, cond2;
    INT status;
    SHORT time_len;
    CHAR c_date[80];
    CHAR m_date[80];

#ifdef vms
    struct FAB fab;
    struct NAM nam;
    struct XABDAT xabdat;
    struct dsc$descriptor desc;
    
    fab = cc$rms_fab;
    nam = cc$rms_nam;
    xabdat = cc$rms_xabdat;
    
    fab.fab$l_fna = in_str;
    fab.fab$b_fns = strlen (in_str);
    fab.fab$l_nam = &nam;
    fab.fab$l_xab = (char *) & xabdat;
    nam.nam$b_nop = NAM$M_NOCONCEAL;
    
    status = sys$open (&fab);
    if (status & 1)
        {
        status = sys$display (&fab);
        if (status & 1)
            {
            *file_size = fab.fab$l_alq;
            desc.dsc$w_length = 80;
            desc.dsc$b_class = DSC$K_CLASS_S;
            desc.dsc$b_dtype = DSC$K_DTYPE_T;
            desc.dsc$a_pointer = c_date;
            cond1 = sys$asctim (&time_len, &desc, &xabdat.xab$q_cdt, 0);
            if (cond1 & 1)
                {
                hoz_cv_date (create_date, c_date);
                }
            desc.dsc$w_length = 80;
            desc.dsc$b_class = DSC$K_CLASS_S;
            desc.dsc$b_dtype = DSC$K_DTYPE_T;
            desc.dsc$a_pointer = m_date;
            cond2 = sys$asctim (&time_len, &desc, &xabdat.xab$q_rdt, 0);
            if (cond2 & 1)
                {
                hoz_cv_date (mod_date, m_date);
                }
            status = SUCCESS;
            }
        /* end of sys$display call */
        
        sys$close (&fab);
        }
    /* end of sys$open call */
    return status;
#endif

#ifdef primos
    INT c_unit, temp_size;
    INT fsize (), bio$primosfileunit ();
    SHORT p_unit, info_wsize, dow, code;
    CHAR cdate[50], parent[129], *cp;
    fortran ENT$RD (), CV$FDV ();
    
    struct 
        {                           /* file info structure filled in by ENT$RD */
        struct 
            {                       /* Entry Control Word                 */
            BYTE type;              /* type of entry (2 = normal, 3 = ACAT)  */
            BYTE length;            /* number of valid words in info struct  */
            } ecw;
        CHAR filename[32];          /* name of directory entry            */
        USHORT pw_protection;       /* password protection bits           */
        USHORT fileinfo;            /* OS control bits                    */
        INT dtm;                    /* Date/Time modified (in FS format)  */
        USHORT non_dflt_acl;        /* zero if protected by default acl   */
        SHORT logical_type;         /* 0 = normal, 1 = RBF file           */
        SHORT trunc;                /* non-zero if truncated by FIX_DISK  */
        INT dtb;                    /* Date/Time backed up (in FS format) */
        INT dtc;                    /* Date/Time created (in FS format)   */
        INT dta;                    /* Date/Time accessed (in FS format)  */
        } info;
    
    struct 
        {
        SHORT len;
        CHAR arr[33];
        } entryname;
    
    struct 
        {
        SHORT len;
        CHAR arr[28];
        } vdate;
    
    info_wsize = sizeof(info) / 2;
    tc_zeroes (&info, sizeof(info));
    
    *file_size = 0;
    c_date[0] = NUL;
    m_date[0] = NUL;
    status = FAIL;
    
    temp_size = fsize (in_str);
    if (temp_size != -1)
        {
        status = SUCCESS;
        
        /* send the file size to the caller */
        *file_size = temp_size;
        
        /* get the date/time created and modified  */
        strcpy (parent, in_str);    /* parse out the directory name */
        cp = strrchr (parent, '>'); /* look for last seperator  */
        if (cp == NULL)
            {                       /* no seperators; its local */
            strcpy (entryname.arr, parent);
            entryname.len = strlen (entryname.arr);
            strcpy (parent, "_current-ufd_");
            }
        else
            {
            *cp++ = NUL;            /* terminate the directory and move to the entryname */
            strcpy (entryname.arr, cp);
            entryname.len = strlen (entryname.arr);
            }
        /* open the parent directory */
        c_unit = open (parent, 0);
        if (c_unit != -1)
            {
            /* retrieve the primos file unit number */
            p_unit = bio$primosfileunit (c_unit);
            
            ENT$RD (p_unit, entryname, &info, info_wsize, code);
            close (c_unit);
            
            if (code == 0)
                {
                /* convert the date created */
                CV$FDV (info.dtc, dow, vdate);
                if (dow != -1)
                    {
                    vdate.arr[2] = '-';
                    vdate.arr[6] = '-';
                    vdate.arr[9] = NUL;
                    strcpy (c_date, vdate.arr);
                    hoz_cv_date (create_date, c_date);
                    }
                /* convert the date modified */
                CV$FDV (info.dtm, dow, vdate);
                if (dow != -1)
                    {
                    vdate.arr[2] = '-';
                    vdate.arr[6] = '-';
                    vdate.arr[9] = NUL;
                    strcpy (m_date, vdate.arr);
                    hoz_cv_date (mod_date, c_date);
                    }
                }
            }
        }
    return status;
#endif

#if UNIX
    struct stat st;
    CHAR *p;
    
    *c_date = *m_date = '\0';       /* assume failure */
    *file_size = 0;
    
    status = stat (in_str, &st);
    if (status != 0)
        return FAIL;
    
    *file_size = (UINT)st.st_size;
    
    p = ctime (&st.st_ctime);       /* get creation time */
    sprintf (c_date, "%2.2s-%3.3s-%4.4s %8.8s", p + 8, p + 4, p + 20, p + 11);
    
    p = ctime (&st.st_mtime);       /* get modified time */
    sprintf (m_date, "%2.2s-%3.3s-%4.4s %8.8s", p + 8, p + 4, p + 20, p + 11);
    
    hoz_cv_date (create_date, c_date);
    hoz_cv_date (mod_date, m_date);
    
    return SUCCESS;

#endif
    }
/* END:     */
