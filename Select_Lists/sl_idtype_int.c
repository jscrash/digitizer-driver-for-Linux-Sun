/* DEC/CMS REPLACEMENT HISTORY, Element SL_IDTYPE_INT.C */
/* *2    30-JUL-1990 17:24:05 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:16:08 JULIAN "integer datatype translation" */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_IDTYPE_INT.C */

#include "esi_gl_defs.h"

#ifndef ESI_SL_METH_X
#include "esi_sl_meth.x"
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

publicdef INT sl_inq_dtype_int(table_translation,data_type,domain)
CHAR *table_translation;
SL_DATA_TYPE data_type;
INT *domain;
{

    SL_DOMAIN_HEADER *sDtype,*pickTable;

    if(ARE_SAME(table_translation,"COLUMN_DOMAIN"))
        pickTable = sl_column_domain;
    else if (ARE_SAME(table_translation,"QS_CUR_COUNT"))
        pickTable = sl_qs_cur_count;
    else if (ARE_SAME(table_translation,"QS_CUR_LIST"))
        pickTable = sl_qs_cur_list;
    else if (ARE_SAME(table_translation,"QS_CUR"))
        pickTable = sl_qs_cur;
    else if (ARE_SAME(table_translation,"QS_LIST_EXISTS"))
        pickTable = sl_qs_list_exists;
    else if (ARE_SAME(table_translation,"GB_NO_ON_MAP"))
        pickTable = sl_gb_no_on_map;

    for(sDtype= pickTable;
        sDtype->data_type IS_NOT_EQUAL_TO NULL_DATA_TYPE; sDtype++)
        if (sDtype->data_type EQUALS data_type)
            {
	    *domain = sDtype->domain;
            return SUCCESS;
            }

    return SL_INVALID_DATA_TYPE;
}

