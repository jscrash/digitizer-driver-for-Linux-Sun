/* DEC/CMS REPLACEMENT HISTORY, Element SL_CALL_METH.C */
/* *3    23-AUG-1990 15:30:35 JULIAN "(SPR 0) fix compiling error" */
/* *2    30-JUL-1990 17:23:24 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:13:43 JULIAN "function to call sl method" */
/* DEC/CMS REPLACEMENT HISTORY, Element SL_CALL_METH.C */
#ifndef ESI_SL_METH_X
#include "esi_sl_meth.x"
#endif
#ifndef ESI_SL_ERR_H
#include "esi_sl_err.h"
#endif
#ifndef ESI_SL_GB_H
#include "esi_sl_gb.h"
#endif
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

publicdef INT sl_call_method(data_type,operator,param)
SL_DATA_TYPE data_type;
CHAR *operator;
VOIDPTR *param;
{
    SL_METHOD_HEADER *sMethod;
    SL_OR_LOAD_STRUCT *sl_ols;
    SL_FEED_NL_STRUCT *sl_fns;
    NLIST_HEADER *nlist;
    SL_GB_RRMAP_STRUCT *sl_grs;
		
    for(sMethod = sl_method_defaults;
	sMethod->data_type IS_NOT_EQUAL_TO NULL_DATA_TYPE; sMethod++)
	if (sMethod->data_type EQUALS data_type &&
	    ARE_SAME(operator,sMethod->function_name))
	    {
	    if (ARE_SAME(operator,"OR_LOAD_LISTS"))
		{
		sl_ols = (SL_OR_LOAD_STRUCT *)param;
		return (sMethod->function)(sl_ols->unique_name,sl_ols->nlist);
		}
	    else if(ARE_SAME(operator,"SL_FEED_NLIST"))
		{
		sl_fns = (SL_FEED_NL_STRUCT *)param;
		return(sMethod->function)(sl_fns);
		}
	    else if(ARE_SAME(operator,"SL_INIT_SELECT_LIST") OR
		    ARE_SAME(operator,"GB_POLYGON"))
		{
		nlist = (NLIST_HEADER *)param;
		return(sMethod->function)(nlist);		
		}
	    else if(ARE_SAME(operator,"GB_CREATE_ON_MAP"))
		{
		sl_grs = (SL_GB_RRMAP_STRUCT *)param;
		return(sMethod->function)
		      (sl_grs->map,&sl_grs->nlist,sl_grs->flag);
		}
	    return SUCCESS; 
	    }
    return SL_INVALID_DATA_TYPE;    
}    
    
