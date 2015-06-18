/* DEC/CMS REPLACEMENT HISTORY, Element VI_GET_CDP_VALUE.C */
/* *2    12-JUN-1991 20:19:06 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:04:40 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_GET_CDP_VALUE.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1991
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* Function Description -------------------------------------------------------
Description:
    Function to return value at specified CDP.

Prototype:
    publicdef INT vi_get_cdp_value(NLIST_HEADER tlist, DOUBLE tst_cdp,
                                  DOUBLE *value)
    
Parameters:
    (I) tlist       -(NLIST_HEADER) Nlist containing horizon data.
    (I) tst_cdp     -(DOUBLE)       CDP at which data value desired.
    (O) value       -(DOUBLE)       Returned data value.

Return Value/Status:
    SUCCESS.
    FAIL - '-1' for 'CDP' outside nlist range, any other fail code
            indicates the problem area.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif 

#if USE_PROTOTYPES
publicdef INT vi_get_cdp_value(NLIST_HEADER tlist, DOUBLE tst_cdp,
                               DOUBLE *value)
#else
publicdef INT vi_get_cdp_value(tlist, tst_cdp, value)
NLIST_HEADER tlist;
DOUBLE tst_cdp;
DOUBLE *value;
#endif
{
    INT status;             /* status flag for function calls               */
    INT i;                  /* loop index                                   */
    INT j;                  /* loop index                                   */
    INT k;                  /* loop index                                   */
    INT m;                  /* loop index                                   */
    DOUBLE fcdp;            /* cdp value returned from surface nlist        */
    DOUBLE ftime;           /* time value returned from surface nlist       */
    DOUBLE cdp1;            /* 1st cdp for interpolation                    */
    DOUBLE cdp2;            /* 2nd cdp for interpolation                    */
    DOUBLE time1;           /* 1st data value for interpolation             */
    DOUBLE time2;           /* 2nd data value for interpolation             */
    DOUBLE fvel;            /* vel value returned from surface nlist        */
    DOUBLE avel;            /* interpolated avg-velocity                    */
    UINT Col_List[2];       /* Column list for nlist access                 */
    VOIDPTR Var_List[2];    /* Address list for nlist access                */
    INT Units[2];           /* UOM codes                                    */
    size_t Data_Sizes[2];   /* Data width in bytes                          */
    INT Data_Types[2];      /* Data type codes                              */
    UINT nallocated;        /* no. input values allocated                   */
    UINT ncols;             /* no. columns in Col_List                      */
    UINT table_kntt;        /* number tables in nlist                       */
    DOUBLE min_cdp;         /* min CDP for line                             */
    DOUBLE max_cdp;         /* max CDP for line                             */
    UINT row_knt;           /* number rows in nlist                         */
    INT ndx;                /* working index                                */
    INT nret;               /* number rows returned                         */
                            
    /* Get the number of tables in the n-list */
    /* -------------------------------------- */
    status = nl_inq_nlist_int(tlist, NL_NLIST_NTABLES, (INT *)&table_kntt);
        
    /* Setup nlist access parameters */
    /* ----------------------------- */
    ncols = 1;
    Col_List[0] = 1;
    Data_Sizes[0] = sizeof(DOUBLE);
    Data_Types[0] = FLOAT_TYPE;

    /* Get CDP range */
    /* ------------- */
    Var_List[0] = (VOIDPTR)&min_cdp;
    status = nl_inq_nlist_info_convert(tlist, NL_NLIST_MIN, ncols, Col_List,
                              Var_List, Units, Data_Sizes, Data_Types);
    Var_List[0] = (VOIDPTR)&max_cdp;
    status = nl_inq_nlist_info_convert(tlist, NL_NLIST_MAX, ncols, Col_List,
                              Var_List, Units, Data_Sizes, Data_Types);
    
    /* Check if in global range - fail if not */
    /* -------------------------------------- */
    if(tst_cdp < min_cdp || tst_cdp > max_cdp)
        {
        return FAIL;
        }
  
    /* Setup nlist access parameters */
    /* ----------------------------- */
    nallocated = 1;
    ncols = 2;
    Col_List[0] = 1;
    Col_List[1] = 2;
    Var_List[0] = (VOIDPTR)&fcdp;
    Var_List[1] = (VOIDPTR)&ftime;
    Units[0] = 0;
    Units[1] = 0;
    Data_Sizes[0] = sizeof(DOUBLE);
    Data_Sizes[1] = sizeof(DOUBLE);
    Data_Types[0] = FLOAT_TYPE;
    Data_Types[1] = FLOAT_TYPE;
    
    /* Loop over all tables */
    /* -------------------- */
    for(k = 0; k < table_kntt; k++)
        {                         
        /* Set the current table active */
        /* ---------------------------- */
        status = nl_set_current_row(tlist, 1, k+1);

        /* Get row count for this table */
        /* ---------------------------- */
        status = nl_inq_table_int (tlist, k+1, NL_TABLE_NROWS,
                                   (INT *) &row_knt);
        
        /* Get min,max CDP for table */
        /* ------------------------- */
        Var_List[0] = (VOIDPTR)&min_cdp;
        status = nl_inq_table_info_convert(tlist, k+1, NL_TABLE_MIN, ncols,
                        Col_List, Var_List, Units, Data_Sizes, Data_Types);
        Var_List[0] = (VOIDPTR)&max_cdp;
        status = nl_inq_table_info_convert(tlist, k+1, NL_TABLE_MAX, ncols,
                        Col_List, Var_List, Units, Data_Sizes, Data_Types);
        
        /* Check if in this table */
        /* ---------------------- */
        if(tst_cdp < min_cdp)
            {
            return FAIL;
            }
        if(tst_cdp > max_cdp)
            {
            continue;
            }
        
        /* Setup initial values of index and increment */
        /* ------------------------------------------- */
        ndx = row_knt/2;
        i = MAX(row_knt/4, 1);
        
        /* Loop thru data until increment down to '1' */
        /* ------------------------------------------ */
        Var_List[0] = (VOIDPTR)&fcdp;
        while(i > 1)
            {
            /* Set the desired point current */
            /* ----------------------------- */
            status = nl_set_current_row(tlist, ndx, k+1);
            
            /* Get the point */
            /* ------------- */    
            status = nl_get_rows_convert(tlist, nallocated,
                                         ncols, Col_List, Var_List,
                                         Units, Data_Sizes,
                                         Data_Types, (UINT *)&nret);
            
            /* Adjust the increment value */
            /* -------------------------- */
            if(fcdp < tst_cdp)
                {
                ndx += i;
                if(ndx >= row_knt)
                    {
                    ndx = row_knt - 1;
                    }
                }
            else if(fcdp > tst_cdp)
                {
                ndx -= i;
                if(ndx < 0)
                    {
                    ndx = 0;
                    }
                }
            else
                {
                *value = ftime;
                return SUCCESS;
                }
                
            i += 1;
            i /= 2;
            }
        
        /* Bracket desired value - then interpolate */
        /* ---------------------------------------- */
        if(fcdp < tst_cdp)
            {
            while(fcdp < tst_cdp)
                {
                cdp1 = fcdp;
                time1 = ftime;
            
                ndx += 1;
                
                /* Set the desired point current */
                /* ----------------------------- */
                status = nl_set_current_row(tlist, ndx, k+1);
            
                /* Get the point */
                /* ------------- */    
                status = nl_get_rows_convert(tlist, nallocated,
                                            ncols, Col_List, Var_List,
                                            Units, Data_Sizes,
                                            Data_Types, (UINT *)&nret);
                
                cdp2 = fcdp;
                time2 = ftime;
                }
            }
        else
            {
            while(fcdp > tst_cdp)
                {
                cdp2 = fcdp;
                time2 = ftime;
            
                ndx -= 1;
                
                /* Set the desired point current */
                /* ----------------------------- */
                status = nl_set_current_row(tlist, ndx, k+1);
            
                /* Get the point */
                /* ------------- */    
                status = nl_get_rows_convert(tlist, nallocated,
                                            ncols, Col_List, Var_List,
                                            Units, Data_Sizes,
                                            Data_Types, (UINT *)&nret);
                
                cdp1 = fcdp;
                time1 = ftime;
                }
            }
        /* Now interpolate to get final value */
        /* ---------------------------------- */
        *value = time1 + (tst_cdp - cdp1)/(cdp2 - cdp1)*(time2 - time1);
        return SUCCESS;
                
        } /* end of table loop */
        
    /* If reach here, then not found */
    /* ----------------------------- */
    return FAIL;
}
