/* DEC/CMS REPLACEMENT HISTORY, Element SL_IDTYPE_STR.C */
/* *2    30-JUL-1990 17:24:10 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:16:35 JULIAN "string datatype translation" */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_IDTYPE_STR.C */
#include "esi_gl_defs.h"

#ifndef ESI_SL_METH_X
#include "esi_sl_meth.x"
#endif

#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif

#ifndef ESI_SL_TS_H
#include "esi_ts.h"
#endif

publicdef INT sl_inq_dtype_string(table_translation,data_type,data_type_name)
CHAR *table_translation;
SL_DATA_TYPE data_type;
CHAR *data_type_name;
{

    SL_DTYPE_HEADER *sDtype,*pickTable;

    if(ARE_SAME(table_translation,"DATA_TYPE_NAME"))
	pickTable = sl_data_type_names;
    else if (ARE_SAME(table_translation,"TABLE_HDR_NAME"))
	pickTable = sl_table_header_names;
    else if (ARE_SAME(table_translation,"TABLE_HEADING"))
	pickTable = sl_table_headings;

    for(sDtype= pickTable;
        sDtype->data_type IS_NOT_EQUAL_TO NULL_DATA_TYPE; sDtype++)
        if (sDtype->data_type EQUALS data_type)
	    {
	    strcpy(data_type_name, sDtype->name);
	    return SUCCESS;
	    }	
    return SL_INVALID_DATA_TYPE;
}

