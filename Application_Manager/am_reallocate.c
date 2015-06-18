/* DEC/CMS REPLACEMENT HISTORY, Element AM_REALLOCATE.C*/
/* *3    14-AUG-1990 11:40:38 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-OCT-1989 11:41:49 GILLESPIE "(SPR 1) Fix calling args to match  prototype"*/
/* *1    19-JUN-1989 12:00:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_REALLOCATE.C*/
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
#include "esi_am_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_REALLOCATE replaces the C malloc function for all Finder
    applications.

    You need not specify any "type" of memory buffer (as specified to
    am_allocate) because the type of memory re-allocated is the same
    as the type which was previously allocated.

Prototype:
    publicdef VOIDPTR am_reallocate(VOIDPTR pointer, size_t size);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pointer     -(VOIDPTR) pointer to existing memory buffer.
    (I) size        -(size_t) new size of buffer.
    
Return Value/Status:
    NULL - Requested memory could not be reallocated.  Indicates either
        the requested memory was unavailable or that the buffer was 
        never allocated memory initially.
        
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOIDPTR am_reallocate (VOIDPTR pointer, size_t size) 
#else
publicdef VOIDPTR am_reallocate (pointer, size)
VOIDPTR pointer;
size_t size;
#endif
    {
    INT status, delete_status;      /* service completion status */
    DATA_LIST data;                 /* data node storage */
    VOIDPTR new_pointer;
    CHAR text[80];                  /* messages */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    /*   Assume success. */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already
         quit, only if AM initialized. */
    
    if (application_static->initialized)
        if (application_static->application_id == -1)
            {
            status = AM_ERR_APPLICATION_QUIT;
            }
    /*   If everything was OK ... */
    
    if (status == SUCCESS)
        {
        
        /*   Reallocate memory. */
        
        new_pointer = tc_realloc (pointer, size);
        
        /*   If the indicated storage was
             allocated successfully ... */
        
        if ((new_pointer) && (new_pointer != pointer))
            {
            /*   Try deleting the original pointer
                 from the data list. If success, then the
                 original pointer was in the list, so
                 insert the new pointer. Otherwise, skip
                 the insert.                */
            delete_status = ht_delete (application_static->data_list,
                                       (VOIDPTR) & pointer);
            if (delete_status == SUCCESS)
                {
                status = ht_insert (application_static->data_list,
                                    (VOIDPTR) & new_pointer, (VOIDPTR) & data);
                if (status != SUCCESS)
                    {
                    sprintf (text, "AM_REALLOCATE:%s:%d",
                             "ht_insert returned bad status", status);
                    er_log (text);
                    }
                }
            }
        }
    /*   If any trouble, zero new pointer to
                 indicate grief to caller. */
    
    if (status != SUCCESS)
        {
        new_pointer = (VOIDPTR)NULL;
        }
    return new_pointer;
    }
/* END:     */
