/* DEC/CMS REPLACEMENT HISTORY, Element GF_DSCTOCS.C*/
/* *5    26-AUG-1990 14:29:56 GILLESPIE "(SPR 1) Fix conditional compiles; Ad esi_glue for prototypes"*/
/* *4    23-AUG-1990 16:12:26 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *3    14-AUG-1990 13:28:03 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-FEB-1990 18:57:58 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:52:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GF_DSCTOCS.C*/
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

#include "esi_glue.h"

#if USE_DESCRIPTORS

#include "esi_c_lib.h"
#include "esi_ctype.h"

#define DSC     struct dsc$descriptor

/* Function Description -----------------------------------------------------
Description:
    Convert a VAX string descriptor to a C string for glueing purposes.

Prototype:
    publicdef VOID gf_desc_to_str(DSC *desc,CHAR *cs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    desc            -(DSC *) Pointer to a string descriptor as described in descrip.h.
    cs              -(CHAR *) C string.

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID gf_desc_to_str (DSC *desc, CHAR *cs)
#else
publicdef VOID gf_desc_to_str (desc, cs)
DSC *desc;
CHAR *cs;
#endif
    {
    CHAR *pend;
    CHAR zero = 
        {
        '\0'
        };
    unsigned short srclen, deslen;
    
    srclen = desc->dsc$w_length;
    deslen = srclen + 1;
    lib$movc5 (&srclen, desc->dsc$a_pointer, &zero, &deslen, cs);

    for (pend = cs + strlen ((char *)cs) - 1;
         pend >= cs AND (NOT isgraph (*pend)); pend--)
        {
        *pend = '\0';
        }
    
    return;
    }
/* END:     */

#endif
