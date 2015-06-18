/* DEC/CMS REPLACEMENT HISTORY, Element SL_ASSIGN_DTYPE.C */
/* *2    30-JUL-1990 17:23:18 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:13:16 JULIAN "assing datatype" */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_ASSIGN_DTYPE.C */
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
#ifndef ESI_SL_METH_X
#include "esi_sl_meth.x"
#endif


publicdef INT sl_assign_data_type(table_name,data_type)
CHAR *table_name;
SL_DATA_TYPE *data_type;
{
    CHAR temp_string[8];
    SL_DTYPE_HEADER *sDtype;

    for(sDtype= sl_table_headings;
        sDtype->data_type IS_NOT_EQUAL_TO NULL_DATA_TYPE; sDtype++)
        if (!strncmp (table_name,sDtype->name,4))
            {
	    *data_type = sDtype->data_type;
            return SUCCESS;
            }

    return SL_INVALID_DATA_TYPE;
}

