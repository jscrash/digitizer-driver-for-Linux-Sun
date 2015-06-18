/* DEC/CMS REPLACEMENT HISTORY, Element SO_CHECK_SHOT.C*/
/* *4    26-JUL-1990 17:18:26 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:02:27 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:04:35 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:25:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_CHECK_SHOT.C*/
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
#include "esi_so.h"

/* Function Description -----------------------------------------------------
Description:
    Function to see if check shot passes trigger value test.

Prototype:
    publicdef INT so_check_shot(SO_STRUCT *pso,DOUBLE shot,CHAR   *ovt,INT ovv,CHAR   *lt,
        INT lv,BOOL   *ok);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *)
    shot            -(DOUBLE) Shot point value.
    ovt             -(CHAR *) Overlay trigger.
    ovv             -(INT) Overlay trigger value.
    lt              -(CHAR *) Line trigger.
    lv              -(INT ) Line trigger value.
    ok              -(BOOL *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_check_shot (SO_STRUCT *pso, SP shot, CHAR *ovt, INT ovv, CHAR *lt,
                             INT lv, BOOL *ok)
#else
publicdef INT so_check_shot (pso, shot, ovt, ovv, lt, lv, ok)
SO_STRUCT *pso;
DOUBLE shot;
CHAR *ovt;
INT ovv;
CHAR *lt;
INT lv;
BOOL *ok;
#endif
    {
    INT status;
    DOUBLE frac;
    INT ishot;
    CHAR checker;
    INT checkv;
    CHAR ishot_string[20];
    CHAR check_string[20];
    
    status = SUCCESS;
    
    /* check to see if it is a whole number */
    
    ishot = shot;
    frac = shot - ishot;
    if (frac > 0.001)
        {
        *ok = FALSE;
        return status;
        }
    /* check to see if it passes test */
    
    if (*ovt != '\0')
        {
        checker = *ovt;
        checkv = ovv;
        }
    else
        {
        checker = *lt;
        checkv = lv;
        }
    switch (checker)
        {
        
    case 'N':
        if (checkv == 0)
            {
            *ok = FALSE;
            break;
            }
        if (pso->disp_opt.ntrigger_last == 0 OR pso->disp_opt.ntrigger_last == -999)
            {
            *ok = TRUE;
            }
        else if (pso->disp_opt.ntrigger_last % checkv == 0)
            {
            *ok = TRUE;
            }
        else
            {
            *ok = FALSE;
            }
        break;
        
    case 'M':
        if (pso->disp_opt.ntrigger_last == 0 OR pso->disp_opt.ntrigger_last == -999)
            {
            *ok = TRUE;
            }
        else if (ishot % checkv == 0)
            {
            *ok = TRUE;
            }
        else
            {
            *ok = FALSE;
            }
        break;
        
    case 'E':
        if (pso->disp_opt.ntrigger_last == 0 OR pso->disp_opt.ntrigger_last == -999)
            {
            *ok = TRUE;
            break;
            }
        sprintf ((char *)check_string, "%d", checkv);
        sprintf ((char *)ishot_string, "%d", ishot);
        if (ishot_string[strlen ((char *)ishot_string) - 1] == check_string[0])
            {
            *ok = TRUE;
            }
        else
            {
            *ok = FALSE;
            }
        break;
        
        }
    return status;
    }
/* END:     */
