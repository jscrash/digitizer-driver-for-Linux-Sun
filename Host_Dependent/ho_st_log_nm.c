/* DEC/CMS REPLACEMENT HISTORY, Element HO_ST_LOG_NM.C*/
/* *5    17-AUG-1990 21:53:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    26-FEB-1990 18:59:06 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *3    18-DEC-1989 16:40:45 JULIAN "(SPR 5069) export facility mods"*/
/* *2     1-NOV-1989 10:12:36 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *1    19-JUN-1989 12:59:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ST_LOG_NM.C*/
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
#include "esi_ho_err.h"

#ifdef vms

#include "descrip.h"
#include "lnmdef.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Creates a logical name for the indicated name in the       
    indecated logical name table.  If the logical name table is    
    NULL, then the PROCESS logical name table will be used.         

Prototype:
    publicdef INT ho_set_logical_name(CHAR logical_table[], CHAR logical_name[], 
    CHAR value[], INT conceal, BOOL set_process);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) logical_table   -(CHAR []) The name of logical table.         
    (I) logical_name    -(CHAR []) The name of logical name. 
    (I) value           -(CHAR *) The equivalence name.     
    (I) conceal         -(INT) The option of conceal.
    (I) set_process     -(BOOL)
    
Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_set_logical_name (CHAR logical_table[], CHAR logical_name[],
                                   CHAR value[], INT conceal, BOOL set_process) 
#else
publicdef INT ho_set_logical_name (logical_table, logical_name, value, conceal,
                                   set_process)
CHAR logical_table[];
CHAR logical_name[];
CHAR value[];
INT conceal;
BOOL set_process;
#endif

#ifdef vms
    {
#pragma nostandard
    $DESCRIPTOR (logical_table_desc, logical_table);
    $DESCRIPTOR (logical_name_desc, logical_name);
    $DESCRIPTOR (value_desc, value);
#pragma standard
    INT status;
    INT attribute;
    static CHAR job_logical_table[10];
    
    if (set_process)
        strcpy (job_logical_table, "LNM$PROCESS");
    else
        strcpy (job_logical_table, "LNM$JOB");
    
    /* if the logical_table given NULL, */
    /* set to be default JOB logical    */
    if ((logical_table EQUALS (CHAR *)NULL) OR (strlen (logical_table) EQUALS 0))
        {
        logical_table_desc.dsc$w_length = strlen (job_logical_table);
        logical_table_desc.dsc$a_pointer = job_logical_table;
        }
    else
        {
        logical_table_desc.dsc$w_length = strlen (logical_table);
        }
    logical_name_desc.dsc$w_length = strlen (logical_name);
    value_desc.dsc$w_length = strlen (value);
    
    if (conceal)
        {
        attribute = LNM$M_CONCEALED;
        }
    else
        attribute = 0;
    
    status = VAX_ROUTINE (lib$set_logical (&logical_name_desc, &value_desc,
                                           &logical_table_desc, &attribute, 0));
    if (status & 1)
        return SUCCESS;
    else
        return HO_ERR_VMS;
    }

#else

    {
    CHAR *tcp[2];                   /* pointer to list of 1 name & NULL */
    
    tcp[0] = value;
    tcp[1] = NULL;
    
    return(ho_set_logical_list (logical_table, logical_name, tcp));
    
    }


#endif
/* END:     */
