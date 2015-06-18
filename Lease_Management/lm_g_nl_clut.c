/* DEC/CMS REPLACEMENT HISTORY, Element LM_G_NL_CLUT.C */
/* *3    11-SEP-1991 13:40:57 JULIAN "(SPR 0) add comments" */
/* *2    18-MAR-1991 18:08:56 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:54:46 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element LM_G_NL_CLUT.C */
#include "esi_lm.h"

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

/*****************************************************************************

    LMZ_GET_NLIST

    get nlist from file according to data type

*****************************************************************************/
publicdef INT lm_get_nlist_clut(nlist,type,nlist_id)
NLIST_HEADER *nlist;
INT type;
CHAR *nlist_id;
{
    INT status;
    FILENAME filename;

    switch(type)
        {
        case LM_DATE    :
            status=ho_add_path("ESI$LUT","date_lease.",MAXPATHLENGTH,filename);
            break;
        case LM_TEXT    :
            status=ho_add_path("ESI$LUT","text_lease.",MAXPATHLENGTH,filename);
            break;
        case LM_NUMERIC :
            status=ho_add_path("ESI$LUT","numeric_lease.",MAXPATHLENGTH,
		    filename);
            break;
        }

    if(status != SUCCESS) return status;

    status=nl_read_nlist(nlist,filename,nlist_id);
    if(status != SUCCESS)
        return status;

    return SUCCESS;
}

