/* DEC/CMS REPLACEMENT HISTORY, Element HO_LGL_FNAME.C*/
/* *3    17-AUG-1990 21:52:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JUL-1989 15:33:59 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:59:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_LGL_FNAME.C*/
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
#include "esi_ho_err.h"

#ifdef vms

#include "esi_ctype.h"
#include "descrip.h"
#include "ssdef.h"
#include "fscndef.h"
static CHAR fileset[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijlkmnopqrstuvwxyz0123456789$_-";

/* File Description ---------------------------------------------------------
Overview:
    Function to determine if the supplied string is a legal
    filename.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ho_chk_fname(CHAR *str, INT len);

    publicdef INT ho_lgl_fname(FILENAME name);

    publicdef INT ho_lgl_fname(FILENAME name);
    
    publicdef INT ho_chk_fname(CHAR *name,INT length);
    
Private_Functions:
    static INT ho_chk_node(CHAR *name,INT length);
    
    static INT ho_chk_fvers(CHAR *name,INT length);
    
    static INT ho_chk_ftype(CHAR *name,INT length);
    
    static INT ho_chk_dvice(CHAR *name,INT length);
    
    static INT ho_chk_dir(CHAR *name,INT length);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Function to check acceptability of supplied name as a
    directory list.

Prototype:
    static INT ho_chk_dir(CHAR *name,INT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) name            -(CHAR *) File name.
    (I) length          -(INT) Length.

Return Value/Status:
    SUCCESS - Successful completion.
    HO_BAD_DIRECTORY
    
Scope:
    PRIVATE to <ho_lgl_fname>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ho_chk_dir (CHAR *name, INT length) 
#else
static INT ho_chk_dir (name, length)
CHAR *name;
INT length;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    INT i;
    CHAR *pname;
    CHAR *pdot;
    INT len;
    INT tot;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* SPEC MUST AT LEAST BE "[]" */
    if (length < 2)
        {
        status = HO_BAD_DIRECTORY;
        }
    /* CHECK THAT IS ENCLOSED IN BRACKETS [] or <> */
    
    else if ((name[0] != '[' AND name[0] != '<') OR (name[length - 1] !=
                                                   ']' AND name[length - 1] != '>'))
        {
        status = HO_BAD_DIRECTORY;
        }
    else if (length > 2)
        {
        /* each component of a directory is actually a file name.  Parse this string
        into the file name component and call ho_chk_fname... */
        /* allow at most 8 levels */
        
        name[length - 1] = 0;
        for (i = 0, pname = name + 1, tot = 1; tot < length;
                                                                           i++,
                                                                           pname +=
                                                                           len + 1,
                                                                           tot += len +
                                                                           1)
            {
            pdot = strchr (pname, '.'); /* find a dot */
            if (pdot IS_NOT_EQUAL_TO NULL)
                {
                *pdot = 0;          /* convert it to zero */
                }
            len = strlen (pname);
            
            if (i EQUALS 0 AND pname[0] EQUALS '-')
                /* OK to use several '-', in first position
                    as long as that's all that's there */
                {
                while (*pname++EQUALS '-')
                    {
                    }
                if (*pname IS_NOT_EQUAL_TO 0)
                    {
                    status = HO_BAD_DIRECTORY;
                    break;
                    }
                continue;
                }
            /* check this individual component - make sure
                it is a legal file name */
            status = ho_chk_fname (pname, len);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                break;
                }
            }
        }
    if (status IS_NOT_EQUAL_TO SUCCESS OR i > 8)
        {
        status = HO_BAD_DIRECTORY;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to check acceptability of supplied name as a
    device name.

Prototype:
    static INT ho_chk_dvice(CHAR *name,INT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) name        -(CHAR *)
    (I) length      -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_BAD_DEVICE
    
Scope:
    PRIVATE to <ho_lgl_fname>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ho_chk_dvice (CHAR *name, INT length) 
#else
static INT ho_chk_dvice (name, length)
CHAR *name;
INT length;
#endif

/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (length <= 1OR name[length - 1] != ':')
        {
        status = HO_BAD_DEVICE;
        }
    else
        {
        for (i = 0; i < length - 1; i++)
            {
            if (NOT isalnum (name[i]) AND name[i] != '$' AND name[i] != '_')
                {
                status = HO_BAD_DEVICE;
                break;
                }
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to check acceptability of supplied name as a
    filename field of a file specification.

Prototype:
    publicdef INT ho_chk_fname(CHAR *name,INT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)
    length          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_BAD_FILENAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_chk_fname (CHAR *name, INT length) 
#else
publicdef INT ho_chk_fname (name, length)
CHAR *name;
INT length;
#endif

/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* CHECK SIZE AND START OF FILENAME */
    
    if (length < 1OR length > 39OR  /* length check */
        /* illegal to start with -$_ */
            (name[0] EQUALS '-' OR name[0] EQUALS '$' OR name[0] EQUALS '_') OR/* must be alphanum or $-_ */
        NOT strspn (name, fileset))
        {
        status = HO_BAD_FILENAME;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to check acceptability of supplied name as a
    filetype field of a file specification.

Prototype:
    static INT ho_chk_ftype(CHAR *name,INT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)
    length          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_BAD_FILETYPE
    
Scope:
    PRIVATE to <ho_lgl_fname>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ho_chk_ftype (CHAR *name, INT length) 
#else
static INT ho_chk_ftype (name, length)
CHAR *name;
INT length;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* CHECK SIZE AND START OF FILENAME */
    /* names are of the form .zjdfgj, where the first character is ALWAYS a
        period (.) . The first character cannot be "$_-".  The type can be empty. */
    
    if (length < 1OR length > 39OR  /* length check */
        /* illegal to start with -$_ */
            length < 1AND ((name[1] EQUALS '-' OR name[1] EQUALS '$' OR name[1] EQUALS '_') OR
            /* must be alphanum or $-_ */
            NOT strspn (name, fileset)))
        {
        status = HO_BAD_FILETYPE;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to check acceptability of supplied name as a
    file version field of a file specification.

Prototype:
    static INT ho_chk_fvers(CHAR *name,INT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)
    length          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_BAD_FILE_VERSION
    
Scope:
    PRIVATE to <ho_lgl_fname>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ho_chk_fvers (CHAR *name, INT length) 
#else
static INT ho_chk_fvers (name, length)
CHAR *name;
INT length;
#endif

/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* CHECK SIZE AND START OF FILENAME */
    
    if (length < 1OR (name[0] != ';' AND name[0] != '.'))
        {
        status = HO_BAD_FILE_VERSION;
        }
    /* CHECK STRING */
    
    else
        {
        for (i = 1; i < length; i++)
            {
            if (NOT isdigit (name[i]) AND NOT (i EQUALS 1AND name[1] EQUALS '-'))   /* minus signs OK */
                {
                status = HO_BAD_FILE_VERSION;
                break;
                }
            }
        if (status EQUALS SUCCESS AND length > 1)
            {                       /* can't have just a minus sign */
            if (length EQUALS 2AND name[1] EQUALS '-')
                {
                status = HO_BAD_FILE_VERSION;
                }
            else
                {
                i = atol (name + 1);
                if (ABS (i) > 32767)    /* negative numbers are permitted */
                    {
                    status = HO_BAD_FILE_VERSION;
                    }
                }
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Function to check acceptability of supplied name as a
    node name.

Prototype:
    static INT ho_chk_node(CHAR *name,INT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(CHAR *)
    length          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_BAD_NODE
    
Scope:
    PRIVATE to <ho_lgl_fname>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ho_chk_node (CHAR *name, INT length) 
#else
static INT ho_chk_node (name, length)
CHAR *name;
INT length;
#endif

/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (length <= 2OR length > 8OR name[length - 2] != ':' OR name[length - 1] != ':')
        {
        status = HO_BAD_NODE;
        }
    else
        {
        i = 0;
        while (i < length - 2)
            {
            if (NOT isalnum (name[i]))
                {
                status = HO_BAD_NODE;
                break;
                }
            i++;
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ho_lgl_fname(FILENAME name);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) name        -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    HO_ILLEGAL_NAME
    HO_NO_NAME_SUPPLIED
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_lgl_fname (FILENAME name) 
#else
publicdef INT ho_lgl_fname (name)
FILENAME name;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;
    USHORT flag;
    CHAR *filespec;
    FILENAME temp;
    struct dsc$descriptor fil_desc;
    struct 
        {
        USHORT length;
        USHORT code;
        CHAR *address;
        } item_desc[9];
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * SET UP ITEM DESCRIPTOR ARRAY * */
    /* -------------------------------- */
    
    item_desc[0].code = FSCN$_FILESPEC;
    item_desc[1].code = FSCN$_NODE;
    item_desc[2].code = FSCN$_DEVICE;
    item_desc[3].code = FSCN$_ROOT;
    item_desc[4].code = FSCN$_DIRECTORY;
    item_desc[5].code = FSCN$_NAME;
    item_desc[6].code = FSCN$_TYPE;
    item_desc[7].code = FSCN$_VERSION;
    
    item_desc[8].code = 0;
    item_desc[8].length = 0;
    item_desc[8].address = (CHAR *)0;
    
    /* * SET UP DESCRIPTOR FOR FILE NAME * */
    /* ----------------------------------- */
    
    fil_desc.dsc$b_class = DSC$K_CLASS_S;
    fil_desc.dsc$b_dtype = DSC$K_DTYPE_T;
    fil_desc.dsc$a_pointer = name;
    fil_desc.dsc$w_length = strlen (name);
    
    /* * USE SYSTEM SERVICE TO PARSE FILENAME STRING * */
    /* ----------------------------------------------- */
    
    flag = 0;
    status = sys$filescan (&fil_desc, item_desc, &flag);
    
    /* * CHECK PARTS OF FILENAME RETURNED FROM SYSTEM SERVICE * */
    /* -------------------------------------------------------- */
    
    if (status EQUALS SS$_NORMAL)
        {
        
        /* FULL RETURNED != FILENAME SENT */
        
        if (item_desc[0].length != fil_desc.dsc$w_length)
            {
            status = HO_ILLEGAL_NAME;
            }
        else
            {
            status = SUCCESS;
            
            /* NULL STRING SUPPLIED */
            if (flag EQUALS 0)
                {
                status = HO_NO_NAME_SUPPLIED;
                }
            /* CHECK NODE */
            
            if (item_desc[1].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[1].address;
                strncpy (temp, filespec, item_desc[1].length);
                temp[item_desc[1].length] = '\0';
                status = ho_chk_node (temp, item_desc[1].length);
                }
            /* CHECK DEVICE */
            
            if (item_desc[2].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[2].address;
                strncpy (temp, filespec, item_desc[2].length);
                temp[item_desc[2].length] = '\0';
                status = ho_chk_dvice (temp, item_desc[2].length);
                }
            /* CHECK ROOT DIRECTORY */
            
            if (item_desc[3].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[3].address;
                strncpy (temp, filespec, item_desc[3].length);
                temp[item_desc[3].length] = '\0';
                status = ho_chk_dir (temp, item_desc[3].length);
                }
            /* CHECK DIRECTORY NAME */
            
            if (item_desc[4].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[4].address;
                strncpy (temp, filespec, item_desc[4].length);
                temp[item_desc[4].length] = '\0';
                status = ho_chk_dir (temp, item_desc[4].length);
                }
            /* CHECK FILENAME */
            
            if (item_desc[5].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[5].address;
                strncpy (temp, filespec, item_desc[5].length);
                temp[item_desc[5].length] = '\0';
                status = ho_chk_fname (temp, item_desc[5].length);
                }
            /* CHECK FILETYPE */
            
            if (item_desc[6].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[6].address;
                strncpy (temp, filespec, item_desc[6].length);
                temp[item_desc[6].length] = '\0';
                status = ho_chk_ftype (temp, item_desc[6].length);
                }
            /* CHECK FILE VERSION */
            
            if (item_desc[7].length > 0 AND status EQUALS SUCCESS)
                {
                filespec = item_desc[7].address;
                strncpy (temp, filespec, item_desc[7].length);
                temp[item_desc[7].length] = '\0';
                status = ho_chk_fvers (temp, item_desc[7].length);
                }
            }
        }
    else
        {
        status = HO_ERR_VMS;
        }
    return status;
    }


#endif
/* END:     */

/* THESE FUNCTION SHOULDN'T BE CALLED ANYMORE */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ho_lgl_fname(FILENAME name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    name            -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if UNIX
publicdef INT ho_lgl_fname (name)
FILENAME name;
    {
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT ho_chk_fname(CHAR *str, INT len);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    str             -(CHAR *
    len             -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
publicdef INT ho_chk_fname (str, len)
CHAR *str;
INT len;
    {
    return SUCCESS;
    }


#endif
/* END:     */
