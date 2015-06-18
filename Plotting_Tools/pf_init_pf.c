/* DEC/CMS REPLACEMENT HISTORY, Element PF_INIT_PF.C */
/* *2     6-JUN-1991 17:21:21 MING "(SPR 0) new checkin" */
/* *1     6-JUN-1991 17:20:28 MING "init plot file" */
/* DEC/CMS REPLACEMENT HISTORY, Element PF_INIT_PF.C */
#include "esi_pf.h"

#if USE_PROTOTYPES
publicdef INT pf_init_plotfile (PF_INFO *pf_info)
#else
publicdef INT pf_init_plotfile (pf_info)
PF_INFO *pf_info;
#endif
    {
    INT status; 
    CHAR type[20];

    strcpy (type,"PLOTFILE");

    status = fm_init_file ("ESI$PLOTS",pf_info->filename,type,"w",
               pf_info->save,pf_info->remark,NULL_FUNCTION_PTR,pf_info->pathname,pf_info->app);

    return status;
    }
