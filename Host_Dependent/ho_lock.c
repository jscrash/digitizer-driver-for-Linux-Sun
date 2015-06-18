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
#include "esi_ho_files.h"

#if UNIX

#include "esi_c_lib.h"
#include <fcntl.h>

#define MAX_OPENTRY 5 /* maximum number try to open a file when it is locked */

#endif

/* Function Description -----------------------------------------------------
Description:
    Host dependent routine to call ANSI C-library fcntl().

Prototype:
    publicdef INT ho_lock (FILE *file_ptr, INT onoroff);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) file_ptr        -(FILE *) File descriptor
    (I) onoroff         -(INT) on for lock and off for unlock 

Return Value/Status:
    Function returns locking status.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_lock (FILE *file_ptr, INT onoroff)
#else
publicdef INT ho_lock (file_ptr, onoroff)
FILE *file_ptr;
INT onoroff;
#endif
    {
    INT status = SUCCESS;

#if UNIX
    struct flock lock_param;
    INT i;

    if (file_ptr == (FILE *)NULL)
      return(-1);

    lock_param.l_type = onoroff ? F_WRLCK : F_UNLCK;
    lock_param.l_whence = 0;
    lock_param.l_start = 0;
    lock_param.l_len = 0;
    status = fcntl (fileno (file_ptr), F_SETLK, &lock_param);

    if (onoroff && status == -1) /* if the file is locked, try few times to 
				     get the lock */
      {
	for (i = 0; status != 0, i < MAX_OPENTRY; i++)
	  {
	    sleep(1);
	    status = fcntl (fileno (file_ptr), F_SETLK, &lock_param);
	  }
      }
#endif
    return status;
    }                               /* end of HO_LOCK */
/* END:     */
