/* DEC/CMS REPLACEMENT HISTORY, Element LM_INIT_NL.C */
/* *3    11-SEP-1991 13:41:02 JULIAN "(SPR 0) add comments" */
/* *2    18-MAR-1991 18:09:02 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:55:21 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element LM_INIT_NL.C */

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
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


#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_LM_H
#include "esi_lm.h"
#endif

static UINT  ndim		= 3;
static INT  units[3]	= { 0, 0, 0 };
static INT  value_inc[3]	= { VALUE_LIST, VALUE_LIST, VALUE_LIST };

#if USE_PROTOTYPES
static INT lmz_init_date_nlist(NLIST_HEADER *nlist);
static INT lmz_init_text_nlist(NLIST_HEADER *nlist);
static INT lmz_init_numeric_nlist(NLIST_HEADER *nlist);
#else
static INT lmz_init_date_nlist();
static INT lmz_init_text_nlist();
static INT lmz_init_numeric_nlist();
#endif

publicdef INT lm_init_nlist(nlist,type)
NLIST_HEADER *nlist;
INT type;
{

    return lmz_init_text_nlist(nlist);
#if 0
    switch(type)
	{
	case LM_DATE	:   
	    return lmz_init_date_nlist(nlist);	
	case LM_TEXT	:   
	    return lmz_init_text_nlist(nlist);
	case LM_NUMERIC : 
	    return lmz_init_numeric_nlist(nlist);
	}
#endif
}

static INT lmz_init_date_nlist(nlist)
NLIST_HEADER *nlist;
{
    static INT  dim_type[3]	    = { CHAR_TYPE, INT_TYPE, INT_TYPE };
    static size_t  dim_size[3]  = { sizeof(CHAR)*30, sizeof(INT), 
					sizeof(INT) };

    return nl_init_nlist (nlist, ndim, units, dim_size, dim_type,
                            value_inc, 500);
}

static INT lmz_init_text_nlist(nlist)
NLIST_HEADER *nlist;
{
    static INT  dim_type[3]	    = { CHAR_TYPE, CHAR_TYPE, CHAR_TYPE };
    static size_t  dim_size[3]  = { sizeof(CHAR)*30, sizeof(CHAR)*30, 
					sizeof(CHAR)*30 };

    return nl_init_nlist (nlist, ndim, units, dim_size, dim_type,
                            value_inc, 500);

}

static INT lmz_init_numeric_nlist(nlist)
NLIST_HEADER *nlist;
{
    INT status;
 
    static INT  dim_type[3]	    = { CHAR_TYPE, FLOAT_TYPE, FLOAT_TYPE };
    static size_t  dim_size[3]  = { sizeof(CHAR)*30, sizeof(DOUBLE), 
					sizeof(DOUBLE) };

    return nl_init_nlist (nlist, ndim, units, dim_size, dim_type,
                            value_inc, 500);

}
