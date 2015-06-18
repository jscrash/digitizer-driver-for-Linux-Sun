/* DEC/CMS REPLACEMENT HISTORY, Element VI_ARGS.C */
/* *2    12-JUN-1991 20:18:53 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:02:40 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_ARGS.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1991
                   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                        Finder Graphics Systems, Inc.
                        201 Tamal Vista Blvd
                        Corte Madera, CA  USA 94925
                        (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/
/* Function Description -------------------------------------------------------
Description:
    Find if specifed command line argument exists.  If it exists,
    return count of parameters following it not beginning with '-'.

Prototype:
    publicdef INT vi_args (INT argc, CHAR *argv[], CHAR *pat, INT *num)
Parameters:
    (I) argc        -(INT)     Number of command line arguments.
    (I) argv        -(CHAR *)  Array of pointers to command line arguments.
    (I) pat         -(CHAR *)  Argument to be found.
    (O) num         -(INT *)   Number of strings directly following 'pat' 
                               not beginning with '-'.

Return Value/Status:
    If 'pat' not found, '-1' returned.
    If 'pat' found, location index in 'argv' returned.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#include "esi_vi.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#if USE_PROTOTYPES
publicdef INT vi_args (INT argc, CHAR *argv[], CHAR *pat, INT *num)
#else
publicdef INT vi_args (argc, argv, pat, num)
INT   argc;     /* Input - number of command line arguments    */
CHAR *argv[];   /* Input - array of pointers to cmnd line args */
CHAR *pat;      /* Input - arg type to look for                */
INT  *num;      /*Output - no. strings directly following      */
                /*         'pat' not beginning with '-'        */
                /* If 'pat' not found: '-1' returned           */
                /* If 'pat' found: loc in argv returned        */
#endif
    {
    INT status;
    INT i, j;
    
    status = -1;                    /* Init to 'not found' */
    *num = 0;                       /* Init to none following */
    
    for (i = 1; i < argc; i++)
        {
        if (strcmp (argv[i], pat) == 0)
            {
            status = i;
            for (j = i + 1; j < argc; j++)
                {
                if (*argv[j] != '-')
                    *num += 1;
                else
                    return status;
                }
            }
        }

    return status;
    }
