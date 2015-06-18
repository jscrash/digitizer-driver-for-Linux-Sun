/* DEC/CMS REPLACEMENT HISTORY, Element RS_MANAGER.C*/
/* *5    17-AUG-1990 22:13:13 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     5-MAR-1990 16:19:09 KEE "(SPR 6016) Change .rb to be .rbx or .rbm for X and Mac Version"*/
/* *3    11-AUG-1989 13:35:12 JULIAN "(SPR -1) function rsmdclos, rsfetch, rsfree, rsgets has return(e), and return;"*/
/* *2    28-JUL-1989 12:40:44 GILLESPIE "(SPR 1) Fix a minor casting problem"*/
/* *1    19-JUN-1989 13:22:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element RS_MANAGER.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_BT_H

#include "esi_bt.h"

#endif

#ifndef ESI_RS_H

#include "esi_rs.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

/* -------------- Type define of B-tree Nodes -------------- */
/* ----- opened files list under each module ------ */
typedef struct f_block
    {
    FILE *fp_resource;
    FILENAME filename;
    TREEHEAD *map_ptr;
    struct f_block *ptr;
    } F_BLOCK;

/* ----- B-tree node in module B-Tree.-------- */
typedef struct md_block
    {
    INT delete;
    INT t_files;
    F_BLOCK *ptr;
    } MD_BLOCK;

/* ----- B-Tree node in file opened B-Tree.--- */
typedef struct open_block
    {
    INT delete;
    FILE *fp_resource;
    FILENAME sfilename;
    INT t_users;
    TREEHEAD *map_ptr;
    } OPEN_BLOCK;

/* ------ B-Tree node in data map B-Tree.----- */
typedef struct map_block
    {
    INT flag;
    INT offset;
    INT length;
    CHAR *str_ptr;
    INT loaded;
    } MAP_BLOCK;

/* ------ B-Tree nnode in data type B-Tree.--- */
typedef struct type_block
    {
    TREEHEAD *tree;
    } TYPE_BLOCK;

/*  RS.MANAGER MACROS  */
#define CHECK_CURMDID if (cur_mdid EQUALS 0) return RS_NO_MODULE_OPEN
#define CHECK_SYS_INIT if (sys_init EQUALS FALSE) rsinit()
#define FILEOPEN(name, fp) if((fp=ho_open(name,"r",NULL)) EQUALS NULL_FILE_PTR)\
    return RS_CANT_OPEN_FILE
        
#define FIND_DATA(a,b,c)  if (btfind(a,b,c) EQUALS TRUE)
#define DATA_NOT_FOUND else

/* RS.MANAGER External variable. */
static INT sys_init = FALSE;    /* flag for indicating system initial.  */
static INT mdid_stack[100];     /* I. D. stack.                          */
static INT st_ptr = -1;         /* stack pointer.                        */
static INT cur_mdid = 0;        /* current module I.D.                   */
static TREEHEAD module, fopened;

CHAR *rsgets ();
static INT rsclosef ();
static INT rscmapt ();
static INT rsfpath ();

/* File Description ---------------------------------------------------------
Overview:
    The resource manager keeps track of resources in resource files and
    provides routines that allow applications and other parts of the
    Toolbox to access them.  There is a system resource file, which con-
    tains standard resources shared by all applications.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT rsinit();
    
    publicdef INT rsterm();
    
    publicdef INT rsmdopen(INT mdid);
    
    publicdef INT rsmdclos(INT mdid);
    
    publicdef INT rsopen(FILENAME rsrcfile);
    
    publicdef INT rsclose(FILENAME rsrcfile);
    
    publicdef INT rspush(mdid);
    
    publicdef INT rspop(INT *pmdid);
    
    publicdef INT rsfetch(INT rsrcid, CHAR rsrctype[], CHAR **rsrcptr);
    
    publicdef INT rsfree(INT rsrcid, CHAR rsrctype[]);
    
    publicdef CHAR *rsgets(CHAR *ptr, CHAR s[]);

Private_Functions:
    static INT rsclosef(FILENAME filename, INT *mdid);
    
    static CHAR *rsldata(FILE *fp, MAP_BLOCK *data);
    
    static INT rscmapt(FILE *fp_map, TREEHEAD *new);
    
    static INT rsfpath(CHAR os_res_file[],  CHAR res_name[]);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Resource Manager initilization.     

Prototype:
    publicdef INT rsinit();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsinit ()
#else
publicdef INT rsinit ()
#endif
    {
    INT mdkey;                      /* the key for module B-tree.                       */
    FILENAME tfilename;             /* filename for temp. file copy.                    */
    F_BLOCK fblock;                 /* file status block in file list under module I.D. */
    MD_BLOCK mdblock;               /* module status block in modlue B-tree.            */
    OPEN_BLOCK openblock;           /* status block of file in opened file list.     */
    INT status;
    
    if (sys_init EQUALS FALSE)
        {
        
        sys_init = TRUE;
        
        /* Initilaize two globable B-Tree.                        */
        status = btinit (&module, 0, sizeof mdkey, sizeof(MD_BLOCK), BT_TREE_SYSTEM);
        status = btinit (&fopened, 0, sizeof(FILENAME), sizeof(OPEN_BLOCK),
                         BT_KEY_IS_C_STRING | BT_TREE_SYSTEM);
        }
    return status;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To terminate the Resource Manager System.             

Prototype:
    publicdef INT rsterm();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    RS_NOT_INITIALIZED
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsterm ()
#else
publicdef INT rsterm ()
#endif
    {
    if (sys_init EQUALS FALSE)
        return RS_NOT_INITIALIZED;
    else
        {
        btfree (&fopened);
        btfree (&module);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Open a module.                                     

Prototype:
    publicdef INT rsmdopen(INT mdid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mdid        -(INT) The module I.D.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_MODULE_OPEN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsmdopen (INT mdid)
#else
publicdef INT rsmdopen (mdid)
INT mdid;
#endif
    {
    MD_BLOCK *mdblock, emdblock;
    INT status;
    
    /* Create a node and add to Module B-Tree.                */
    CHECK_SYS_INIT;
    FIND_DATA (&module, &mdid, &emdblock)
        {
        if (emdblock.delete)
            {
            emdblock.delete = FALSE;
            emdblock.t_files = 0;
            emdblock.ptr = (F_BLOCK *)NULL;
            status = btrepcur (&module, &mdid, &emdblock);
            }
        else
            {
            return RS_MODULE_OPEN;
            }
        }
    DATA_NOT_FOUND
        {
        mdblock = ALLOCATE (MD_BLOCK);
        mdblock->delete = FALSE;
        mdblock->t_files = 0;
        mdblock->ptr = (F_BLOCK *)NULL;
        status = btadd (&module, &mdid, mdblock);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Close a module and close all files under this module automatically.  

Prototype:
    publicdef INT rsmdclos(INT mdid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mdid        -(INT) module I.D.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_MODULE_NOT_OPEN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsmdclos (INT mdid)
#else
publicdef INT rsmdclos (mdid)
INT mdid;
#endif
    {
    MD_BLOCK data;
    INT status;
    F_BLOCK *next_file;
    
    CHECK_SYS_INIT;
    
    /* Foundv the required module and close all opened files under this module.
       Then delete this node from Module B-Tree.              */
    FIND_DATA (&module, &mdid, &data)
        {
        next_file = data.ptr;
        while (next_file IS_NOT_EQUAL_TO (F_BLOCK *)NULL)
            {
            rsclosef (next_file->filename, &mdid);
            next_file = next_file->ptr;
            }
        data.delete = TRUE;
        status = btrepcur (&module, &mdid, &data);
        }
    /* If not found, show error message to programmer.        */
    DATA_NOT_FOUND
        {
        return RS_MODULE_NOT_OPEN;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Open a reource file under the current module.      

Prototype:
    publicdef INT rsopen(FILENAME rsrcfile);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) rsrcfile    -(FILENAME) Filename.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_CANT_OPEN_FILE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsopen (FILENAME rsrcfile)
#else
publicdef INT rsopen (rsrcfile)
FILENAME rsrcfile;
#endif
    {
    CHAR s[STMAX];                  /* temp. storage for string input.    */
    FILENAME os_filename;           /* source  binary filename.                   */
    FILENAME bt_filename;           /* temp. copy of resource binary  file.       */
    
    FILE *fp_map, *fp_resource;
    TREEHEAD *new;                  /* New TREEHEAD pointer.              */
    MD_BLOCK mdblock;               /* Temp. nodes in different B-Tree.   */
    F_BLOCK *nf_block;
    OPEN_BLOCK *data, openblock;
    INT status = 0;
    
    CHECK_SYS_INIT;
    CHECK_CURMDID;

#ifdef USE_X
    sprintf (os_filename, "%s.RBX", rsrcfile);
#else 
    sprintf (os_filename, "%s.RBM", rsrcfile);
#endif
    strcpy (bt_filename, os_filename);
    
    /* bt_filename is the upper-case filename without path or extension which will
        go into the btrees.  os_filename is the lower-case (for case-sensitive
        operating systems) filename which will have path & extension added & will
        eventually be used to open the resource binary file. */
    
    ts_sto_upper (bt_filename);
    ts_sto_lower (os_filename);
    
    status = rsfpath (os_filename, rsrcfile);
    
    /* error checking  */
    if (status != SUCCESS)
        return RS_CANT_OPEN_FILE;
    
    nf_block = ALLOCATE (F_BLOCK);
    
    /* If found the file has been opened, copy the information of file
        from OpenedFile B-Tree and increase the number of sharing users. */
    FIND_DATA (&fopened, bt_filename, &openblock)
        {
        if (openblock.delete)
            {
            FILEOPEN (os_filename, fp_resource);
            data = ALLOCATE (OPEN_BLOCK);
            new = ALLOCATE (TREEHEAD);
            rscmapt (fp_resource, new);
            data->fp_resource = fp_resource;
            strcpy (data->sfilename, bt_filename);
            data->t_users = 1;
            data->map_ptr = new;
            data->delete = FALSE;
            status = btrepcur (&fopened, bt_filename, data);
            nf_block->map_ptr = new;
            nf_block->fp_resource = fp_resource;
            
            }
        else
            {
            openblock.t_users += 1;
            nf_block->map_ptr = openblock.map_ptr;
            nf_block->fp_resource = openblock.fp_resource;
            status = btrepcur (&fopened, bt_filename, &openblock);
            }
        }
    /* If not found, open the file and create a node in OpenedFile B-Tree. */
    DATA_NOT_FOUND
        {
        FILEOPEN (os_filename, fp_resource);
        data = ALLOCATE (OPEN_BLOCK);
        new = ALLOCATE (TREEHEAD);
        rscmapt (fp_resource, new);
        data->fp_resource = fp_resource;
        strcpy (data->sfilename, bt_filename);
        data->t_users = 1;
        data->map_ptr = new;
        data->delete = FALSE;
        status = btadd (&fopened, bt_filename, data);
        nf_block->map_ptr = new;
        nf_block->fp_resource = fp_resource;
        }
    strcpy (nf_block->filename, bt_filename);
    nf_block->ptr = (F_BLOCK *)0;
    
    /* Add a node to the opened file list of current module.  */
    FIND_DATA (&module, &cur_mdid, &mdblock)
        {
        nf_block->ptr = mdblock.ptr;
        mdblock.ptr = nf_block;
        mdblock.t_files += 1;
        status = btrepcur (&module, &cur_mdid, &mdblock);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To close a opened file.                          

Prototype:
    publicdef INT rsclose(FILENAME rsrcfile);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) rsrcfile    -(FILENAME) filename.

Return Value/Status:
    Function returns status from rsclosef.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsclose (FILENAME rsrcfile)
#else
publicdef INT rsclose (rsrcfile)
FILENAME rsrcfile;
#endif
    {
    FILENAME filename;
    
    /* Copy the filename.                                    */
#ifdef USE_X
    sprintf (filename, "%s.RBX", rsrcfile);
#else 
    sprintf (filename, "%s.RBM", rsrcfile);
#endif
    /* Release this from cuurent module I.D.                 */
    ts_sto_upper (filename);
    return rsclosef (filename, &cur_mdid);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To release a opened file under specific module I.D.
    and decrease the sharing.                       

Prototype:
    static INT rsclosef(FILENAME filename, INT *mdid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    mdid            -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <rs_manager.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT rsclosef (FILENAME filename, INT *mdid)
#else
static INT rsclosef (filename, mdid)
FILENAME filename;
INT *mdid;
#endif
    {
    INT file_order = 1;             /* to indicate the order of file list.     */
    INT cmp;                        /* compare result.                         */
    OPEN_BLOCK data;
    MAP_BLOCK mapblock;             /* temp. MAP_BLOCK.                        */
    TYPE_BLOCK tyblock;             /* temp. TYPE_BLOCK.                       */
    INT iflast = FALSE;             /* flag for last node.                     */
    INT last_type = FALSE;          /* flag for finishing cleaned last node.   */
    INT last_map = FALSE;           /* flag for finishing cleaned last node.   */
    /*-----------------------------------------*/
    /* flag for finishing cleaned the specific */
    /* B-Tree.                                 */
    /*-----------------------------------------*/
    INT finish_maptree = FALSE;
    
    INT finish_tytree = FALSE;
    INT id, count;
    INT status = SUCCESS;
    CHAR type[5];                   /* temp. type i.d.                         */
    MD_BLOCK mdblock;
    F_BLOCK *nfblock, *lfblock;
    
    /* If found the file in OpenedFile B-tree, decrease the
       number of sharing users.                               */
    FIND_DATA (&fopened, filename, &data)
        {
        data.t_users -= 1;
        
        /* If the number of sharing users is zero, then delete the node
           from OpenedFile B-Tree, release the memory , close the file. */
        if (data.t_users EQUALS 0)
            {
            data.delete = TRUE;
            status = btrepcur (&fopened, filename, &data);
            fclose (data.fp_resource);
            status = btfirst (data.map_ptr, type, &tyblock);
            while (finish_tytree IS_NOT_EQUAL_TO TRUE)
                {
                status = btfirst (tyblock.tree, &id, &mapblock);
                while (finish_maptree IS_NOT_EQUAL_TO TRUE)
                    {
                    if (mapblock.loaded EQUALS TRUE)
                        {
                        mapblock.loaded = FALSE;
                        tc_free (mapblock.str_ptr);
                        status = btrepcur (tyblock.tree, &id, &mapblock);
                        }
                    if (last_map EQUALS TRUE)
                        {
                        
                        /* if finished all, free the data map B-Tree.            */
                        status = btfree (tyblock.tree);
                        finish_maptree = TRUE;
                        last_map = FALSE;
                        }
                    else
                        {
                        if ((count = btcount (tyblock.tree)) EQUALS 1)
                            last_map = TRUE;
                        else
                            {
                            iflast = btnext (tyblock.tree, &id, &mapblock);
                            if (iflast EQUALS TRUE)
                                last_map = TRUE;
                            }
                        }
                    }
                if (last_type EQUALS TRUE)
                    {
                    
                    /* If finished all, free the data type B-Tree.          */
                    btfree (data.map_ptr);
                    finish_tytree = TRUE;
                    }
                else
                    {
                    if ((count = btcount (data.map_ptr)) EQUALS 1)
                        {
                        btfree (data.map_ptr);
                        finish_tytree = TRUE;
                        }
                    else
                        {
                        iflast = btnext (data.map_ptr, type, &tyblock);
                        finish_maptree = FALSE;
                        if (iflast EQUALS TRUE)
                            last_type = TRUE;
                        }
                    }
                }
            }
        else                        /* If the number of sharing users is not equal to zero, update data. */
            status = btrepcur (&fopened, filename, &data);
        
        /* find the current module and delete the node from opened file list. */
        FIND_DATA (&module, mdid, &mdblock)
            {
            nfblock = mdblock.ptr;
            while (nfblock IS_NOT_EQUAL_TO (F_BLOCK *)NULL)
                {
                if ((cmp = strcmp (nfblock->filename, filename)) EQUALS 0)
                    {
                    if (file_order++EQUALS 1)
                        {
                        mdblock.ptr = nfblock->ptr;
                        status = btrepcur (&module, mdid, &mdblock);
                        tc_free (nfblock);
                        break;
                        }
                    else
                        {
                        lfblock->ptr = nfblock->ptr;
                        tc_free (nfblock);
                        break;
                        }
                    }
                else
                    {
                    lfblock = nfblock;
                    nfblock = nfblock->ptr;
                    }
                }
            }
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Push the input module I.D. into Stack and set it to be
    the current module I.D.                                

Prototype:
    publicdef INT rspush(mdid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mdid        -(INT) Module I.D.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rspush (mdid)
#else
publicdef INT rspush (mdid)
INT mdid;
#endif
    {
    st_ptr += 1;
    mdid_stack[st_ptr] = mdid;
    cur_mdid = mdid;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To pop an item from Stack, and set the current module I.D.
    to be the top item of Stack.                           

Prototype:
    publicdef INT rspop(INT *pmdid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mdid        -(INT) Module I.D.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_MODULE_STACK_EMPTY
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rspop (INT *pmdid)
#else
publicdef INT rspop (pmdid)
INT *pmdid;
#endif
    {
    CHECK_SYS_INIT;
    /* check the stack */
    if (st_ptr EQUALS - 1)
        {
        return RS_MODULE_STACK_EMPTY;
        }
    *pmdid = mdid_stack[st_ptr];
    st_ptr -= 1;
    if (st_ptr EQUALS - 1)
        cur_mdid = 0;
    else
        cur_mdid = mdid_stack[st_ptr];
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To load string data into memory and return the address.   

Prototype:
    static CHAR *rsldata(FILE *fp, MAP_BLOCK *data);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) fp          -(FILE *) Input file pointer.
    (I) data        -(MAP_BLOCK *) The information of the required string.

Return Value/Status:
    Function returns the value of oldptr.
    
Scope:
    PRIVATE to <rs_manager.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static CHAR *rsldata (FILE *fp, MAP_BLOCK *data)
#else
static CHAR *rsldata (fp, data)
FILE *fp;
MAP_BLOCK *data;
#endif
    {
    CHAR *ptr, *oldptr;
    INT count = 0;                  /* counter for string length    */
    INT length;
    
    CHECK_SYS_INIT;
    length = data->length;
    ptr = (CHAR *)tc_zalloc (length + 10);
    oldptr = ptr;
    fseek (fp, data->offset, 0);
    for (count = 0; count < length; count++)
        {
        *ptr++ = fgetc (fp);
        }
    *ptr = '\0';
    return(oldptr);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    According to I.D. and Type, to fetch a memory address,
    where the string is stored in.                  

Prototype:
    publicdef INT rsfetch(INT rsrcid, CHAR rsrctype[], CHAR **rsrcptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) rscid       -(INT) Resource string I.D.
    (I) rsrctype    -(CHAR []) 4 CHARs string for resource string type.
    (O) rsrcptr     -(CHAR **) Returned pointer, which the string start at.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_NO_SUCH_ID
    TRUE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsfetch (INT rsrcid, CHAR rsrctype[], CHAR **rsrcptr)
#else
publicdef INT rsfetch (rsrcid, rsrctype, rsrcptr)
INT rsrcid;
CHAR rsrctype[];
CHAR **rsrcptr;
#endif
    {
    MD_BLOCK mdblock;               /* temp. MD_BLOCK.       */
    F_BLOCK *fblock;                /* temp. F_BLOCK.        */
    OPEN_BLOCK openblock;           /* temp. OPEN_BLOCK.     */
    TYPE_BLOCK tyblock;             /* temp. TYPE_BLOCK.     */
    MAP_BLOCK mapblock;             /* temp. MAP_BLOCK.      */
    INT status;
    
    CHECK_SYS_INIT;
    ts_sto_upper (rsrctype);
    
    /* search in current module first.                       */
    FIND_DATA (&module, &cur_mdid, &mdblock)
        {
        fblock = mdblock.ptr;
        while (fblock IS_NOT_EQUAL_TO (F_BLOCK *)NULL)
            {
            FIND_DATA (fblock->map_ptr, rsrctype, &tyblock)
                {
                
                /* If found and been loaded already, return the pointer.    */
                /* If found, but not been loaded yet, load the string into
                   memory and return the pointer.                           */
                FIND_DATA (tyblock.tree, &rsrcid, &mapblock)
                    {
                    if (mapblock.loaded EQUALS TRUE)
                        {
                        *rsrcptr = mapblock.str_ptr;
                        }
                    else
                        {
                        *rsrcptr = rsldata (fblock->fp_resource, &mapblock);
                        mapblock.str_ptr = *rsrcptr;
                        mapblock.loaded = TRUE;
                        status = btrepcur (tyblock.tree, &rsrcid, &mapblock);
                        }
                    return TRUE;
                    }
                }
            fblock = fblock->ptr;
            }
        }
    DATA_NOT_FOUND
	{
	return RS_NO_SUCH_ID;
	}
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Free a resource from memory.                 
 
Prototype:
    publicdef INT rsfree(INT rsrcid, CHAR rsrctype[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) rsrcid      -(INT) Resource string I.D.
    (I) rsrctype    -(CHAR []) 4 CHARs string for resource string type.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_NO_SUCH_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT rsfree (INT rsrcid, CHAR rsrctype[])
#else
publicdef INT rsfree (rsrcid, rsrctype)
INT rsrcid;
CHAR rsrctype[];
#endif
    {
    MD_BLOCK mdblock;               /* temp. MD_BLOCK.       */
    F_BLOCK *fblock;                /* temp. F_BLOCK.        */
    TYPE_BLOCK tyblock;             /* temp. TYPE_BLOCK.     */
    MAP_BLOCK mapblock;             /* temp. MAP_BLOCK.      */
    INT status;
    INT count;                      /* count for free length */
    CHAR *ptr;                      /* pointer of being freed space.*/
    
    CHECK_SYS_INIT;
    CHECK_CURMDID;
    ts_sto_upper (rsrctype);
    /* search the specified string under current I.D. */
    FIND_DATA (&module, &cur_mdid, &mdblock)
        {
        fblock = mdblock.ptr;
        while (fblock IS_NOT_EQUAL_TO (F_BLOCK *)NULL)
            {
            FIND_DATA (fblock->map_ptr, rsrctype, &tyblock)
                {
                FIND_DATA (tyblock.tree, &rsrcid, &mapblock)
                    {
                    
                    /* If found, free the memory and update the status.        */
                    if (mapblock.loaded EQUALS TRUE)
                        {
                        mapblock.loaded = FALSE;
                        tc_free (mapblock.str_ptr);
                        mapblock.str_ptr = NULL;
                        status = btrepcur (tyblock.tree, &rsrcid, &mapblock);
                        status = btrepcur (fblock->map_ptr, rsrctype, &tyblock);
                        }
                    return SUCCESS;
                    }
                }
            fblock = fblock->ptr;
            }
        return RS_NO_SUCH_ID;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To get a line of resource data.                      
 
Prototype:
    publicdef CHAR *rsgets(CHAR *ptr, CHAR s[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptr         -(CHAR *) Points to the first char.
    (I) s           -(CHAR []) A string pointer for returning line.

Return Value/Status:
    Function returns the value of return_ptr, points to first char of next 
    new line.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *rsgets (CHAR *ptr, CHAR s[])
#else
publicdef CHAR *rsgets (ptr, s)
CHAR s[], *ptr;
#endif
    {
    INT i = 0;                      /* counter  for string.              */
    INT done = FALSE;               /* ready for return.                 */
    CHAR *return_ptr;               /* return the next char. pointer.    */
    
    CHECK_SYS_INIT;
    while (done IS_NOT_EQUAL_TO TRUE)
        {
        if ((ptr EQUALS (CHAR *)NULL) OR (*ptr EQUALS 0))
            {
            return_ptr = (CHAR *)NULL;
            return(return_ptr);
            }
        i = 0;
        while ((s[i++] = *ptr++) IS_NOT_EQUAL_TO '\n')
            ;
        s[i] = '\0';
        if (*ptr EQUALS 0)
            return_ptr = (CHAR *)NULL;
        else
            return_ptr = ptr;
        if (s[0] IS_NOT_EQUAL_TO ',')
            {
            if (s[0] IS_NOT_EQUAL_TO '*')
                return(return_ptr);
            else if (s[1] IS_NOT_EQUAL_TO 'T' AND s[
                                           1] IS_NOT_EQUAL_TO 'N' AND s[
                                                                  1] IS_NOT_EQUAL_TO 'S' AND
                      s[1] IS_NOT_EQUAL_TO 'I' AND s[
                                           1] IS_NOT_EQUAL_TO 'L' AND s[
                                                                  2] IS_NOT_EQUAL_TO ' ')
                return(return_ptr);
            }
        }
    return(return_ptr);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To create a B-tree structure data map for a resource file.  
 
Prototype:
    static INT rscmapt(FILE *fp_map, TREEHEAD *new);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) fp_map      -(FILE *) The file pointer of data map.
    (O) new         -(TREEHEAD *) Returned pointer of created B-Tree.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <rs_manager.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT rscmapt (FILE *fp_map, TREEHEAD *new)
#else
static INT rscmapt (fp_map, new)
FILE *fp_map;
TREEHEAD *new;
#endif
    {
    TREEHEAD *map_tree, *ty_tree;
    TYPE_BLOCK *tyblock;            /* node in the map_tree.             */
    MAP_BLOCK data;                 /* data block for map_tree, under type_block.  */
    CHAR c, s[STMAX];               /* temp. storage for input string.   */
    CHAR t[5];                      /* error type.                       */
    INT i, key, done = FALSE;       /* integer temp. storage.            */
    INT end_of_map = FALSE;         /* flag of end_of_map.               */
    INT skip_map = 0;               /* the skipped byte of map.          */
    UINT str_len, data_len;
    CHAR *eof_flag;                 /* EOF flag.                         */
    INT status;
    
    /* Initilize a new B-Tree and read data from data map file.  */
    ty_tree = ALLOCATE (TREEHEAD);
    status = btinit (ty_tree, 0, sizeof t, sizeof(TYPE_BLOCK),
                     BT_TREE_SYSTEM | BT_KEY_IS_C_STRING);
    
    /* to calculate the skipped length first.  */
    eof_flag = fgets (s, STMAX, fp_map);
    ho_query_offset_length (s, &str_len, &data_len);
    sscanf (s, "%d", &skip_map);
    skip_map += str_len;
    
    eof_flag = fgets (s, STMAX, fp_map);
    while (eof_flag IS_NOT_EQUAL_TO (CHAR *)NULL AND (!done))
        {
        
        /* analyze the data. */
        switch (s[0])
            {
        case 'T':
            sscanf (s, "%c %s", &c, t); /* get type */
            ts_sto_upper (t);
            end_of_map = FALSE;
            break;
            
        case 'F':
            sscanf (s, "%c %d", &c, &i);    /* get flag */
            data.flag = i;
            end_of_map = FALSE;
            break;
            
        case 'O':
            sscanf (s, "%c %d", &c, &i);    /* get offset */
            data.offset = i + skip_map;
            end_of_map = FALSE;
            break;
            
        case 'L':
            sscanf (s, "%c %d", &c, &i);    /* get length */
            data.length = i;
            end_of_map = FALSE;
            break;
            
        case ',':
            sscanf (s, "%c %d", &c, &key);
            end_of_map = FALSE;
            break;
            
        case '*':
            if (end_of_map)
                {
                done = TRUE;
                break;
                }
            FIND_DATA (ty_tree, t, tyblock)
                {
                status = btadd (tyblock->tree, &key, &data);
                status = btrepcur (ty_tree, t, tyblock);
                }
            else                    /* If data type is a new one, create a new B-Tree for it.  */
                {
                map_tree = ALLOCATE (TREEHEAD);
                status = btinit (map_tree, 0, sizeof key, sizeof data, BT_TREE_SYSTEM);
                data.loaded = FALSE;
                status = btadd (map_tree, &key, &data);
                tyblock = ALLOCATE (TYPE_BLOCK);
                tyblock->tree = map_tree;
                status = btadd (ty_tree, t, tyblock);
                }
            end_of_map = TRUE;
            break;
            }
        eof_flag = fgets (s, STMAX, fp_map);
        if (s[0] EQUALS '#')
            done = TRUE;
        }
    *new = *ty_tree;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To find out if this file is in the local directory or in FINDER system.      
 
Prototype:
    static INT rsfpath(CHAR os_res_file[],  CHAR res_name[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) res_name    -(CHAR []) The filename before added path 
                        (no extension, upper case).
    (M) os_res_file -(CHAR []) Resource filename.

Return Value/Status:
    SUCCESS - Successful completion.
    RS_CANT_OPEN_FILE
    
Scope:
    PRIVATE to <rs_manager.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT rsfpath (CHAR os_res_file[], CHAR res_name[])
#else
static INT rsfpath (os_res_file, res_name)
CHAR os_res_file[], res_name[];
#endif
    {
    FILENAME filename;              /* resource filename with path & extension.        */
    FILENAME loc_res_name;          /* local copy of resource name        */
    
    FILE *fp_source;
    
    if ((fp_source = ho_open (os_res_file, "r", NULL)) == NULL_FILE_PTR)
        {
        strcpy (loc_res_name, res_name);    /* for case-sensitive OS's */
        ts_sto_lower (loc_res_name);
        ho_add_path ("ESI$RESOURCES", loc_res_name, STMAX, filename);
#ifdef USE_X
        strcat (filename, ".rbx");
#else
        strcat (filename, ".rbm");
#endif
        if (!hoexists (filename))   /* make sure it's there */
            return RS_CANT_OPEN_FILE;
        
        strcpy (os_res_file, filename); /* send full path back to caller */
        return SUCCESS;
        }
    else
        {
        fclose (fp_source);
        return SUCCESS;
        }
    }
/* END:     */
