/* DEC/CMS REPLACEMENT HISTORY, Element SO_MK_CDP_LST.C */
/* *3    26-JUL-1990 17:21:37 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting" */
/* *2    23-JUL-1990 11:20:03 GILLESPIE "(SPR 1) Fixes to allow negative CDP/SP ratio" */
/* *1     5-MAR-1990 17:02:28 VINCE "(spr 6015) shorten name for UNIX compatiblilty." */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_MK_CDP_LST.C */

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/***********************************************************************/
/*                                                                     */
/* NAME:     SO_MAKE_CDP_LIST.C                                        */
/* AUTHOR:   STEVE SPENCE                                              */
/* FUNCTION: FUNCTION TO MANAGE SHOTPOINT LABELING AND Z VALUES        */
/*                                                                     */
/***********************************************************************/

#include "esi_so.h"
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#if USE_PROTOTYPES
publicdef INT so_make_cdp_list(CDP first_cdp, CDP last_cdp, SP first_sp,
                        INT cdps_per_shot, INT interval, INT offset,
                        CDP **list, UINT *npts)
#else
publicdef INT so_make_cdp_list(first_cdp,last_cdp,first_sp,cdps_per_shot,
                        interval,offset,list,npts)
CDP first_cdp;
CDP last_cdp;
SP first_sp;
INT cdps_per_shot;
INT interval;
INT offset;
CDP **list;
UINT *npts;
#endif
    {
    INT ishot;
    INT test;
    CDP test_cdp;
    register CDP *temp;
    register UINT i = 0;
    
    ishot = first_sp;
    test = offset - (ishot % interval);
    
    test_cdp = first_cdp + (cdps_per_shot * test);
    if (test < 0)
        {
        test_cdp += cdps_per_shot * interval;
        }
    
    interval *= ABS(cdps_per_shot);
    test = (last_cdp - first_cdp) / interval + 3; 
    *list = temp = (CDP *) tc_alloc(test*sizeof(CDP));
    
    if (first_cdp != test_cdp)
        {
        *temp++ = first_cdp;
        i++;
        }
    
    while (test_cdp < last_cdp)
        {
        *temp++ = test_cdp;
        i++;
        test_cdp += interval;
        }
    
    *temp = last_cdp;
        
    *npts = ++i;
    return SUCCESS;
    }
