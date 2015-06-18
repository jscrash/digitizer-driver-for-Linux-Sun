/* DEC/CMS REPLACEMENT HISTORY, Element PF_TOOLS.C*/
/* *4     9-NOV-1990 10:33:57 MING "(SPR -1) replace ho_find_files and ho_find_files2 to ho_find_pathnames and ho_find_filenames"*/
/* *3    17-AUG-1990 22:11:38 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    28-JUL-1989 12:21:18 GILLESPIE "(SPR 2) Correct call to strchr (int rather than string as 2nd arg"*/
/* *1    19-JUN-1989 13:22:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element PF_TOOLS.C*/
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

#include "esi_ho.h"
#include "esi_pf.h"
#include "esi_ts.h"
#include "esi_am.h"
#include "esi_tc.h"

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT pf_get_files(PF_STRUCT *pfp,CHAR *fspec, INT neutral_only);
    
    publicdef INT pf_get_plotter(PDS *pinfo);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Give the user a selection of plotters to choose from, return data
    about users pick.

Prototype:
    publicdef INT pf_get_plotter(PDS *pinfo);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pinfo           -(PDS *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT pf_get_plotter (PDS *pinfo)
#else
publicdef INT pf_get_plotter (pinfo)
PDS *pinfo;
#endif
    {
    CHAR selplotter[81];
    CHAR **tcp;
    INT status = SUCCESS;
    
    /* 12/12/88 gs - use tcp for 2nd arg to ho_translate_symbol */
    
    if (ho_translate_symbol ("ESI$SELECTED_PLOTTER", &tcp) != SUCCESS)
        {
        status = pf_plotter_cdlg (pinfo);
        }
    else
        {
        strcpy (selplotter, tcp[0]);
        status = pf_inq_plotter (selplotter, pinfo);
        ts_tcp_free (tcp);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT pf_get_files(PF_STRUCT *pfp,CHAR *fspec, INT neutral_only);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pfp             -(PF_STRUCT *)
    fspec           -(CHAR *)
    neural_only     -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT pf_get_files (PF_STRUCT *pfp, CHAR *fspec, INT neutral_only)
#else
publicdef INT pf_get_files (pfp, fspec, neutral_only)
PF_STRUCT *pfp;
CHAR *fspec;
INT neutral_only;
#endif
    {
    PROGNAME ("pf_get_files");
    INT status;
    FILENAME dir_spec, fname_spec, ftype_spec, full_spec, file_spec;
    CHAR *ptr;
    CHAR *pspec;

#if 0
    /* Isolate Dir spec if any */
    if ((ptr = strchr (fspec, ']')))
        {
        strncpy (dir_spec, fspec, ((++ptr) - fspec));
        strcpy (fspec, ptr);
        if ((ptr = strchr (fspec, '.')))
            {
            strncpy (fname_spec, fspec, ((++ptr) - fspec));
            strcpy (ftype_spec, ptr);
            }
        else
            {
            strcpy (ftype_spec, ".NPF");
            }
        }
    else if ((ptr = strchr (fspec, '.')))
        {
        ptr++;
        strncpy (fname_spec, fspec, (ptr - fspec));
        strcpy (ftype_spec, ptr);
        }
    else
        {
        strcpy (ftype_spec, ".NPF");
        }
    if (strlen (dir_spec) < 2)
        strcpy (dir_spec, getenv ("PATH"));
    
    if (strlen (fname_spec) == 1AND fname_spec[0] == '*')
        fname_spec[0] = NUL;
#endif      
    strcpy (fname_spec, fspec);
    /* for now all i care about is the fname w/out ext. or dir. */
    if (neutral_only == TRUE)
        {
        sprintf (full_spec, "%s*.NPF", fname_spec);
        }
    else
        {
        sprintf (full_spec, "*%s*.*PF", fname_spec);
        strcpy (full_spec, getenv ("HOME"));
        ptr = (CHAR *)strchr (full_spec, ']');
        if (ptr)
            sprintf (ptr, "...]*%s*.NPF", fname_spec);
        else
            sprintf (full_spec, "[000000...]*%s*.NPF", fname_spec);
        }
    status = ho_find_filenames (full_spec, &pfp->neut_files.filelist);
    pfp->neut_files.num_files = ts_tcp_len (pfp->neut_files.filelist);
    ts_tcp_sort (pfp->neut_files.filelist);
    strcpy (pfp->neut_files.key, full_spec);
    
    return SUCCESS;
    }
/* END:     */
