/* DEC/CMS REPLACEMENT HISTORY, Element AM_ALLOCATE.C*/
/* *2    14-AUG-1990 11:39:35 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 11:59:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_ALLOCATE.C*/
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

#include "esi_am_int.h"
#include "esi_tc.h"
#include "esi_er.h"
#include "esi_mg.h"
#include "esi_am_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_ALLOCATE allocates an area of memory for the caller and returns
    a pointer to the allocated memory.  AM_ALLOCATE replaces the C malloc() 
    function for purposes of Finder application memory management.

Prototype:
    publicdef VOIDPTR am_allocate(INT type, size_t length);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) type        -(INT) type of application that needs memory.
    (I) length      -(size_t) size of memory needed.

Return Value/Status:
    SUCCESS - Successfull completion of memory allocation;
    AM_ERR_APPLICATION_QUIT - If am_quit has been executed for application;
    AM_ERR_ILLEGAL_TYPE - If application type is incorrect;
    AM_ERR_NO_MEMORY - If there is no memory to allocate;
    The function returns a NUL pointer if an error occured, else it returns
    a pointer to the beginning of the new free memory.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    Calling AM_ALLOCATE without ever having initialized the application
    manager is allowed, but no memory management functions can be performed
    by the appliction manager on the caller's behalf. Therefore, malloc
    is called and it is left at that.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOIDPTR am_allocate (INT type, size_t length) 
#else
publicdef VOIDPTR am_allocate (type, length)
INT type;                           /* type: APPLICATION or GLOBAL */
size_t length;                      /* size, in bytes */
#endif
    {
    VOIDPTR pointer;                /* pointer to memory */
    DATA_LIST data_entry;           /* data node storage */
    INT status;                     /* Internal completion status */
    INT insert_status;
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ************************************************************************ */
    
    status = SUCCESS;
    
    /*   If AM not initialized, then everything
         is system. */
    
    if (!application_static->initialized)
        {
        type = AM_SYSTEM;
        }
    else
        {
        switch (type)
            {
        case AM_APPLICATION:
            if (application_static->application_id == -1)
                {
                status = AM_ERR_APPLICATION_QUIT;
                }
            else
                {
                data_entry.application_id = application_static->application_id;
                }
            break;
        case AM_SYSTEM:
            break;
        case AM_GLOBAL:
            data_entry.application_id = 0;
            break;
        default:
            status = AM_ERR_ILLEGAL_TYPE;
            break;
            }
        }
    /*   If everything is OK ... */
    
    if (status == SUCCESS)
        {
        
        /*   Allocate memory and return pointer. */
        
        pointer = tc_zalloc (length);
        
        /*   If trouble allocating memory,
             indicate grief. */
        
        if (pointer == (VOIDPTR)NULL)
            {
            status = AM_ERR_NO_MEMORY;
            }
        /*   Otherwise, save data information. */
        
        else
            {
            /*   Insert node into data_list hash table
                 node if not AM_SYSTEM type. */
            
            if (type != AM_SYSTEM)
                {
                /*   Set the type. */
                
                data_entry.type = type;
                
                insert_status = ht_insert (application_static->data_list,
                                           (VOIDPTR) & pointer,
                                           (VOIDPTR) & data_entry);
                if (insert_status != SUCCESS)
                    {
                    CHAR am_msg[200];
                    sprintf (am_msg, " AM_ALLOCATE: ht_insert returned bad status: %d",
                             insert_status);
                    er_log (am_msg);
                    }
                }
            }
        }
    /*   If anything went wrong, indicate
         problem by returning NULL pointer,
         and issue trace message. */
    
    if (status)
        {
        pointer = (VOIDPTR)NULL;
        }
    return pointer;
    }
/* END:     */
