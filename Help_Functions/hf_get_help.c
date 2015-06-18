/* DEC/CMS REPLACEMENT HISTORY, Element HF_GET_HELP.C*/
/* *7    23-AUG-1990 16:12:32 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *6    23-AUG-1990 15:30:24 JULIAN "(SPR 0) fix compiling error"*/
/* *5    22-AUG-1990 16:43:34 KEE "(SPR -1) fixing compile error"*/
/* *4    17-AUG-1990 21:48:38 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     1-NOV-1989 10:12:18 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *2    26-JUN-1989 10:45:44 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:54:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HF_GET_HELP.C*/
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

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_GLUE_H

#include "esi_glue.h"

#endif

#ifndef ESI_HF_INT_X

#include "esi_hf_int.x"

#endif

#ifndef ESI_HO_ERR_H

#include "esi_ho_err.h"

#endif

#ifdef vms

#include "hlpdef.h"
#include "ssdef.h"

#endif

/* File Description ---------------------------------------------------------
Overview:
    VMS specifc routines.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT hf_get_help (CHAR key_string[]);

Private_Functions:
    static INT hf_fill_help_buf (DSC *msg_string);

-----------------------------------------------------------------------------*/


#ifdef vms

/* Function Description -----------------------------------------------------
Description:
    Called by LIB$OUTPUT_HELP to fill the help message into          
    the displaying buffer.                                           

Prototype:
    static INT hf_fill_help_buf (DSC *msg_string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) msg_str     -(DSC *) A pointer to message descriptor.            

Return Value/Status:
    SS$_NORMAL                                                       

Scope:
    PRIVATE to <hf_get_help.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT hf_fill_help_buf (DSC *msg_string)
#else
static INT hf_fill_help_buf (msg_string)
DSC *msg_string;
#endif
    {
    CHAR *buffer_ptr;
    
    if ((hf_struct->total_used_chars + msg_string->dsc$w_length + 1) >=
        hf_struct->total_allocated_chars)
        {
        hf_struct->message_ptr = (CHAR *)am_reallocate ((BYTE *)hf_struct->message_ptr,
                                                        hf_struct->
                                                        total_allocated_chars +
                                                        MAX_NUM_CHARS * 23);
        }
    buffer_ptr = hf_struct->message_ptr + hf_struct->total_used_chars;
    
    if (msg_string->dsc$w_length > 0)
        {
        hoblockmove ((BYTE *)msg_string->dsc$a_pointer, (BYTE *)buffer_ptr,
                     msg_string->dsc$w_length);
        hf_struct->total_used_chars += msg_string->dsc$w_length;
        }
    hf_struct->message_ptr[hf_struct->total_used_chars] = '\n';
    hf_struct->total_used_chars += 1;
    hf_struct->message_ptr[hf_struct->total_used_chars] = '\0';
    hf_struct->total_lines += 1;
    return SS$_NORMAL;
    }
/* END:     */

#else 

/* if HOST is anything but VMS routine is a stub    */

#ifdef DSC

#undef DSC

#endif

#define DSC CHAR    /* do this so function prototypes won't break   */
static INT hf_fill_help_buf (msg_string)
DSC *msg_string;
    {
    return SUCCESS;
    }


#endif

#ifdef vms

/* Function Description -----------------------------------------------------
Description:
    To get the help message for a specific key and to                
    display the message througth a specific routine.                     

Prototype:
    publicdef INT hf_get_help (CHAR key_string[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) key_string  -(CHAR *) A requested key string.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    HO_ERR_VMS

Scope :
    PUBLIC

Limitations/Assumptions:
    This routine will onlywork under VMS.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hf_get_help (CHAR key_string[])
#else
publicdef INT hf_get_help (key_string)
CHAR key_string[];
#endif
    {
    INT status;
    INT output_width = 65;
    INT flags;
    CHAR *string, *next_string;
    BOOL found = FALSE;
    static CHAR lib_name[] = "ESI$HELP:FINDER_HELP";
    static $DESCRIPTOR (lib_name_desc, lib_name);
#pragma nostandard
    $DESCRIPTOR (key_string_desc, key_string);
#pragma standard
    
    status = am_open_workspace ("HELP_WORKSPACE", AM_GLOBAL, (VOIDPTR *) & hf_struct);
    if (status)
        return status;
    
    lib_name_desc.dsc$w_length = strlen (lib_name);
    key_string_desc.dsc$w_length = strlen (key_string);
    flags = HLP$M_PROCESS + HLP$M_GROUP + HLP$M_SYSTEM + HLP$M_LIBLIST + HLP$M_HELP;
    status = lbr$output_help (hf_fill_help_buf, &output_width, &key_string_desc,
                              &lib_name_desc, &flags, 0);
    
    /* because the routine will not return the     */
    /* error status when the index key is not      */
    /* found, tempereryly, we only can search for  */
    /* the string 'Sorry'              */
    next_string = hf_struct->message_ptr;
    do
        {
        string = next_string;
        next_string = strchr (string, 'S');
        if (next_string != (CHAR *)0)
            {
            found = !(strncmp (next_string, "Sorry", 5));
            next_string++;
            }
        } while (!found AND next_string != (CHAR *)0);
    
    if (status & 1AND!found)
        return SUCCESS;
    else
        return HO_ERR_VMS;
    }
/* END:     */

#else

/*      If HOST is anything but VMS, stub out  */
/*      routine                    */

publicdef INT hf_get_help (key_string)
CHAR key_string[];
    {
    return SUCCESS;
    }


#endif
