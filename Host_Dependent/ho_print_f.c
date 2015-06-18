/* DEC/CMS REPLACEMENT HISTORY, Element HO_PRINT_F.C */
/* *2    17-AUG-1990 21:52:56 VINCE "(SPR 5644) Code Cleanup" */
/* *1    30-JUN-1989 09:20:02 GILLESPIE "Print a File" */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_PRINT_F.C */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_PRINT_F.C*/
/* *5    17-DEC-1988 17:11:20 JHUANG "(SPR 4981) Unisource"*/
/* *4    31-MAR-1988 09:18:46 QA "(SPR 4315) gen 1, backout prime port checkin of gen 2."*/
/* *3    21-MAR-1988 14:20:32 CSE "(SPR 4315) Make include of errd.ins.cc host dependent"*/
/* *2    18-MAR-1988 13:05:05 CSE "(SPR 4315) V5 port to Prime"*/
/* *1     7-JUL-1987 10:55:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_PRINT_F.C*/
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
#include "esi_glue.h"

/* File Description ---------------------------------------------------------
Overview:
    Print module.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ho_print_f(CHAR *file,BOOL fortran_carriage);
    
    publicdef INT hog_print_f();
Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Printing routine (includes glue routine below).
    
Prototype:
    publicdef INT ho_print_f(CHAR *file,BOOL fortran_carriage);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) file                -(CHAR *) File name of file to print.
    (I) fortran_carriage    -(BOOL) Whether or not to use fortran carriage
                                control, attribute of file on VAX,
                                attribute of print (spool) command on Prime.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_print_f (CHAR *file, BOOL fortran_carriage) 
#else
publicdef INT ho_print_f (file, fortran_carriage)
CHAR *file;
BOOL fortran_carriage;
#endif
    {
    CHAR command[256];

#ifdef vms
    strcpy (command, "PRINT ");
#endif

#ifdef primos
    strcpy (command, "SPOOL ");
#endif
    strcat (command, file);

#ifdef primos
    if (fortran_carriage)
        {
        strcat (command, " -F");
        }
#endif
    return ho_syscom (command);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Glue routine.

Prototype:
    See below.
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    See below.
    
Return Value/Status:
    See below.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#ifdef vms
publicdef INT hog_print_f (file, fortran_carriage)
DSC *file;
INT *fortran_carriage;
    {
    CHAR string[256];
    DSCTOCS (file, string);
    return ho_print_f (string, (BOOL)(*fortran_carriage));
    }


#endif

#ifdef primos
publicdef INT hog_print_f (file, fortran_carriage, z1, z2)
CHAR *file;
INT fortran_carriage;               /* type 'BOOL' changed to 'INT' */
INT z1, z2;
    {
    extern INT ftocstr (), ctofstr ();
    extern INT ho_print_f ();
    INT ret;
    CHAR string1[256];
    
    ftocstr ((CHAR *) & file, string1, z1);
    ret = ho_print_f (string1, fortran_carriage);
    ctofstr ((CHAR *) & file, string1, z1);
    return(ret);
    }


#endif
/* END:     */
