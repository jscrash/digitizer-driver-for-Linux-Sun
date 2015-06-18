/* DEC/CMS REPLACEMENT HISTORY, Element AM_FREE.C*/
/* *3    14-AUG-1990 23:56:05 GILLESPIE "(SPR 5644) Fix last checkin"*/
/* *2    14-AUG-1990 11:39:57 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 11:59:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_FREE.C*/
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
#include "esi_am_err.h"
#include "esi_ht_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_FREE replaces the C free() function to free memory which has been
    previously allocated using the AM_ALLOCATE directive.  AM_FREE can be called, 
    just like AM_ALLOCATE, even if the application manager is not initialized. 
    In that case, it is just like calling the C free() function.

Prototype:
    publicdef INT am_free(VOIDPTR pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) pointer     -(VOIDPTR) Pointer to released memory. 

Return Value/Status:
    SUCCESS - Successfull release of memory;
    AM_ERR_APPLICATION_QUIT - Am_quit has been executed for the application;
    AM_ERR_BAD_DATA_POINTER - Attempt was made to free memory not previously 
        allocated.
        
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_free (VOIDPTR pointer) 
#else
publicdef INT am_free (pointer)
VOIDPTR pointer;
#endif
    {
    PROGNAME ("AM_FREE");
    INT status;                     /* service completion status */
    CHAR am_msg[200];               /* err log message     */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already
         quit, only if AM initialized. */
    
    if (application_static->initialized AND application_static->application_id == -1)
        {
        status = AM_ERR_APPLICATION_QUIT;
        }
    /*   If everything was OK ... */
    
    else
        {
        
        /*   If user specified a specific buffer
             to free ... */
        
        if (pointer != (VOIDPTR)NULL)
            {
            
            /*   Delete the entry from the DATA LIST
                 if it is in fact there. If delete 
                 returns key_not_found, dont worry -
                 the pointer may have referenced sys
                 memory, in which case it would have
                 never been put into the DATA LIST
                                 NOTE: only do this if am has been
                     initialized                 */
            
            if (application_static->initialized)
                {
                
                status = ht_delete (application_static->data_list,
                                    (VOIDPTR) & pointer);
                if (status != SUCCESS AND status IS_NOT_EQUAL_TO HT_KEY_NOT_FOUND)
                    {
                    sprintf (am_msg, "%s:%s %d", progname,
                             "ht_delete returned bad status:", status);
                    er_log (am_msg);
                    }
                /*   Free the memory. */
                
                status = tc_free (pointer);
                
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    status = AM_ERR_BAD_DATA_POINTER;
                }
            /* END IF (INITIALIZED) */
            }
        /* END IF POINTER != NULL */
        }
    /* END IF AM not initialized */
    return status;
    }
/* END:     */
