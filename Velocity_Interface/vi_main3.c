/* DEC/CMS REPLACEMENT HISTORY, Element VI_MAIN3.C */
/* *2    14-JUN-1991 17:58:26 DOUG "(SPR 0) Initial checkin in correct library." */
/* *1    14-JUN-1991 17:54:43 DOUG "Smooth velocity grids." */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_MAIN3.C */
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
    Back interpolate from velocity grid to lines (horizons).
    VERSION 2.0

Prototype:
    vi_main3 -oid aaa -proj bbb -gin ccc -gout ddd -hor eee -smooth fff

Parameters:
    -oid aaa                    Oracle ID/Password, e.g. EUREKA/EUREKA
    -proj bbb                   Project name
    -gin ccc                    Prefix used for input grids
    -gout ddd                   Prefix to be used for output grids
    -hor  eee                   Seismic horizon names
    -smooth fff                 Type of smoothing requested (optional),
                                legal values are 
                                -smooth MEDIAN
                                -smooth WT 1
                                -smooth WT 2
 
Limitations/Assumptions:
 
-----------------------------------------------------------------------------*/

#include "esi_c_lib.h"            /* for strings */
#ifndef ESI_FM_H
#include "esi_fm.h"
#endif

#ifndef ESI_VI_H
#include "esi_vi.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"            /* for project projection */
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

publicdef INT main(argc, argv)
INT argc;
CHAR *argv[];
{
    PROJECT_NAME project;
    INT status;
    INT i,j,k;
    INT num;
    BOOL go;
    CHAR vel[4];
    CHAR *horz;
    INT  numhorz;
    CHAR *c;
    INT  smu_type = 0;
    INT  smu_wt;    
    CHAR gin[13];
    CHAR gout[13];

    /* process command line arguments */
    /* ------------------------------ */
    go = TRUE;

    /* Get ORACLE id/password */
    /* ---------------------- */
    i = vi_args(argc,argv,"-oid",&num);
    if(i < 0)
        {
        fprintf(stderr,"No Oracle ID.\n");
        go = FALSE;
        }
    else
        {
        if ((status = or_login(argc, argv)) IS_NOT_EQUAL_TO SUCCESS)
            {
            fprintf(stderr,
                    "Unable to connect to Oracle using %s: Terminating.\n",
                    argv[i+1]);
            exit(EXIT_FAILURE);
            }
        }

    /* Get project name */
    /* ---------------- */
    i = vi_args(argc,argv,"-proj",&num);
    if(i < 0)
        {
        fprintf(stderr,"No project.\n");
        go = FALSE;
        }
    else
        {
        qs_set_c(QS_PROJECT_NAME, ts_sto_upper(argv[i+1]), 0);
        strcpy(project, argv[i+1]);
        if ((status = finder_config_project(FALSE)) IS_NOT_EQUAL_TO SUCCESS)
            {
            fprintf(stderr,"Unable to connect to project %s: Terminating.\n",
                    argv[i+1]);
            or_logout(FALSE);
            goto NO_PROCESS;
            }
        }

    /* Get input grid prefix */
    /* --------------------- */
    i = vi_args (argc, argv, "-gin", &num);
    if (i < 0 || num EQUALS 0)
        {
        fprintf (stderr, "No output grid prefix.\n");
        go = FALSE;
        }
    else
        {
        strcpy (gin, ts_sto_upper (argv[i + 1]));
        strcat (gin, "_");
        }

    /* Get output grid prefix */
    /* ---------------------- */
    i = vi_args (argc, argv, "-gout", &num);
    if (i < 0 || num EQUALS 0)
        {
        fprintf (stderr, "No output grid prefix.\n");
        go = FALSE;
        }
    else
        {
        strcpy (gout, ts_sto_upper (argv[i + 1]));
        strcat (gout, "_");
        }

    /* Get list of horizon names */
    /* ------------------------- */
    i = vi_args(argc,argv,"-hor",&numhorz);
    if(i > 0 AND numhorz > 0 AND go == TRUE)
        {
        horz = (CHAR *)tc_alloc(30*numhorz);
        for(j = 0; j < numhorz; j++)
            strcpy(&horz[30*j],argv[i+1+j]);
        }

    /* Get grid smoothing info */
    /* ----------------------- */
    i = vi_args(argc,argv,"-smooth",&num);
    if(i > 0)
        {
        ts_sto_upper(argv[i+1]);
        if (strcmp (argv[i + 1], "MEDIAN")  != 0
            AND strcmp (argv[i + 1], "FFT") != 0
            AND strcmp (argv[i + 1], "1")  != 0
            AND strcmp (argv[i + 1], "2")  != 0)
            {
            go = FALSE;
            fprintf(stderr,
            "-smooth can only have 'MEDIAN', '1' or '2' as argument\n");
            }   
        else
            {
            if(strcmp(argv[i+1],"MEDIAN") EQUALS 0)
                smu_type = 1;
            else if(strcmp(argv[i+1],"FFT") EQUALS 0)
                smu_type = 2;
            else if (strcmp (argv[i+1], "1") EQUALS 0)
                {
                smu_type = 3;
                smu_wt = 1;
                }
            else if (strcmp (argv[i+1], "2") EQUALS 0)
                {
                smu_type = 3;
                smu_wt = 2;
                }
            }
        }
    

    /* If main parameters were present, then can proceed. */
    /* -------------------------------------------------- */
    if(go EQUALS FALSE)
    {
    or_logout(TRUE);
    goto NO_PROCESS;
    }
   
    /* we're ready to process */
    /* ---------------------- */
    status = viz_main3(gin, gout,numhorz,horz,smu_type,smu_wt);

    or_logout(TRUE);

    exit(EXIT_SUCCESS);

NO_PROCESS: 
    fprintf(stderr,
       "Usage: vi_main3 -oid <oracle_login>\n");
    fprintf(stderr,
       "                -proj <project>\n");
    fprintf(stderr,
       "                -gin <input name prefix>\n");
    fprintf(stderr,
       "                -gout <output name prefix>\n");
    fprintf(stderr,
       "                -hor <Horizon list>\n");
    fprintf(stderr,
       "                -smooth <Smoothing type>\n");
    exit(EXIT_FAILURE);

}

/* ========================================================================*/

/* Function Description -------------------------------------------------------
Description:
    Function to convert velocity functions to required velocity type.

Prototype:
    privatedef INT viz_main3(CHAR *gin, CHAR *gout, INT numhorz,
                            CHAR *horz, INT smu_type, INT smu_wt)
    
Parameters:
    (I) gin         -(CHAR *)   Input grid name prefix.
    (I) gout        -(CHAR *)   Output grid name prefix.
    (I) numhorz     -(INT)      Number of horizon names input.
    (I) horz        -(CHAR *)   Horizon name array.  Each horizon name
                                starts 30 characters from the start of
                                the previous horizon name.  
    (I) smu_type    -(INT)      Smoothing type code
                                = 0, No smooth of output grid
                                = 1, Bi-directional application of
                                     median of 3 filtering
                                = 2, FFT filtering
                                = 3, Weighted average smoothing    
    (I) smu_wt      -(INT)      Smoothing code associated with smu_type.

Return Value/Status:
    SUCCESS.
    FAIL.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#ifdef PROTOTYPES
privatedef INT viz_main3(CHAR *gin, CHAR *gout, INT numhorz,
                            CHAR *horz, INT smu_type, INT smu_wt)
#else
privatedef INT viz_main3(gin, gout, numhorz, horz, smu_type, smu_wt)
CHAR *gin;
CHAR *gout;
INT   numhorz;
CHAR *horz;
INT   smu_type;
INT   smu_wt;
#endif
{
    
    INT status;         /* status flag for function calls          */
    INT stat_save;      /* error status save                       */
    INT stat_flag = 0;  /* error occurrance flag                   */
    INT i;              /* loop index                              */
    INT j;              /* loop index                              */
    INT k;              /* loop index                              */
    INT fmsiz;          /* size of each element in name array      */
    FM_FILE nameg;      /* horizon grid file name                  */
    FM_FILE namegout;   /* horizon grid file name                  */
    FLOAT     *grida;   /* grid pointer for average vel grid       */
    INT       grows;    /* number rows in grids                    */
    INT       gcols;    /* number columns in grids                 */
    DOUBLE    gint;     /* computed grid interval                  */
    DOUBLE    znull;    /* null value for grid                     */
    DOUBLE    min_x;    /* minimum x-value for grid                */
    DOUBLE    max_x;    /* maximum x-value for grid                */
    DOUBLE    min_y;    /* minimum y-value for grid                */
    DOUBLE    max_y;    /* maximum y-value for grid                */
    DOUBLE    min_z;    /* minimum z-value for grid                */
    DOUBLE    max_z;    /* maximum z-value for grid                */
    UINT      Col_List[2]; /* Column list for nlist access         */
    VOIDPTR   Var_List[2]; /* Address list for nlist access        */
    INT       Units[2]; /* UOM codes                               */
    size_t    Data_Sizes[2];/* Data width in bytes                 */
    INT       Data_Types[2];/* Data type codes                     */
    UINT      nallocated;  /* no. input values allocated           */
    UINT      ncols;    /* no. columns in Col_List                 */
    UINT      nrows;    /* no. rows actually returned              */
    INT       jj;       /* working index                           */
    CHAR      pid[60];  /* Projection system                       */
    CHAR      coord_sys[60]; /* Grid coordinate system, eg, "XY"   */
    CHAR      flt_nam[60];   /* fault name                         */
    CHAR      top_surf[60];  /* top surface name                   */
    CHAR      base_surf[60]; /* base surface name                  */

    /* Loop over horizons */
    /* ------------------ */
    for(i = 0; i < numhorz; i++)
        {

        /* Build grid file names */
        /* --------------------- */
        strcpy(nameg, gin);
        strcat(nameg, &horz[30 * i]);
        strcpy(namegout, gout);
        strcat(namegout, &horz[30 * i]);

        /* Get grid from DB */
        /* ---------------- */
        status = nl_read_grid(nameg, &znull, &min_x, &max_x,
                              &min_y, &max_y, &min_z, &max_z,
                              &grows, &gcols, &grida, pid,
                              coord_sys, flt_nam, top_surf,
                              base_surf);
        if(status < SUCCESS)
            {
            fprintf(stderr,"Cound not read grid '%s'.\n",nameg);
            continue;
            }

        /* Grid interval */
        /* ------------- */
        gint = (max_y - min_y)/(DOUBLE)(grows - 1);

        /* Now is the time to do smoothing */
        /* ------------------------------- */
        if(smu_type EQUALS 1)
            {
            status = vi_median_2d(grows, gcols, grida);
            if(status < SUCCESS)
                {
                fprintf(stderr,"Could not get workspace for filtering\n");
                fprintf(stderr,"Grid is not filtered\n");
                }
            }
        else if(smu_type EQUALS 2)
            {
            fprintf(stderr,"FFT option not implemented!\n");
            fprintf(stderr,"Grid is not filtered\n");
            }
        else if(smu_type EQUALS 3)
            {
            status = vi_wt_avg(smu_wt, grows, gcols, grida);
            if(status < SUCCESS)
                {
                fprintf(stderr,"Could not get workspace for filtering\n");
                fprintf(stderr,"Grid is not filtered\n");
                }
            }

        /* Store grid in nlist -- put in DB, then done */
        /* ------------------------------------------- */
        status = nl_write_grid(namegout, znull, min_x, max_x,
                               min_y, max_y, grows, gcols,
                               grida, pid, coord_sys, flt_nam,
                               "VI_MAIN", top_surf, base_surf);
        if(status < SUCCESS)
            {
            fprintf(stderr,"Problem from 'nl_write_grid':status = %d\n",
                    status);
            stat_flag = 1;
            stat_save = status;
            goto CLEANUP;
            }
        } /* end of horizon loop */

    /* All ok - clear status flag */
    /* -------------------------- */
    stat_flag = 0;

    /* Deallocate work space */
    /* --------------------- */
CLEANUP:
    status = tc_free(grida);

    if(stat_flag)
        {
        status = stat_save;
        }
    else
        {
        status = SUCCESS;
        }
    return status;

nospace:
    fprintf(stderr,"Could not allocate space for one or more variables.\n");
    stat_save = -1;
    stat_flag = 1;
    goto CLEANUP;
}
