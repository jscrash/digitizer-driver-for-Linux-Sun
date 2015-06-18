/* DEC/CMS REPLACEMENT HISTORY, Element SL_GDTYPE_F_STR.C */
/* *2     3-AUG-1990 18:31:33 JULIAN "(SPR 0) new element" */
/* *1     3-AUG-1990 18:29:23 JULIAN "get dtype given a string" */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_GDTYPE_F_STR.C */
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
#ifndef ESI_SL_METH_X
#include "esi_sl_meth.x"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif


publicdef INT sl_get_dtype_from_string(table_translation,string_name,data_type)
CHAR *table_translation;
CHAR *string_name;
SL_DATA_TYPE *data_type;
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
        if (ARE_SAME(string_name,sDtype->name))
            {
	    *data_type = sDtype->data_type;
            return SUCCESS;
            }

    return SL_INVALID_DATA_TYPE;
}

