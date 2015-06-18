/* DEC/CMS REPLACEMENT HISTORY, Element GF_CSTODSC.C*/
/* *5    26-AUG-1990 14:29:52 GILLESPIE "(SPR 1) Fix conditional compiles; Ad esi_glue for prototypes"*/
/* *4    23-AUG-1990 16:12:14 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *3    14-AUG-1990 13:27:58 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-FEB-1990 18:57:54 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:52:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GF_CSTODSC.C*/
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
#include "esi_c_lib.h"

#if USE_DESCRIPTORS

#define DSC     struct dsc$descriptor

/* Function Description -----------------------------------------------------
Description:
    Convert a C string to a VAX string descriptor for glueing purposes.

Prototype:
    publicdef VOID gf_str_to_desc(CHAR *cs,DSC  *desc);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    cs              -(CHAR *) C string.
    desc            -(DSC *) Pointer to a string descriptor as described in descrip.h.
    
Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID gf_str_to_desc (CHAR *cs, DSC *desc)
#else
publicdef VOID gf_str_to_desc (cs, desc)
CHAR *cs;
DSC *desc;
#endif
    {
    CHAR blank = 
        {
        ' '
        };
    unsigned short srclen, deslen;
    
    deslen = desc->dsc$w_length;
    srclen = strlen (cs);
    lib$movc5 (&srclen, cs, &blank, &deslen, desc->dsc$a_pointer);
    return;
    }
/* END:     */

#endif
