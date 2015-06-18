/* DEC/CMS REPLACEMENT HISTORY, Element PROJMN_.C */
/* *2     1-MAY-1991 11:38:12 KEE "(SPR -1) Modified Gulf Canada DLS/NTS for vms and sun version" */
/* *1     1-MAY-1991 11:35:25 KEE "DLS/NTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element PROJMN_.C */
/************************************************************************


                  Copyright Gulf Canada Resources, 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
WITHOUT PRIOR WRITTEN CONSENT OF

			Gulf Canada Resources, Inc.
			401 9th Avenue SW
			Calgary, Alberta
                        Canada T2P 2H7
			(403) 233-4000

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)
************************************************************************/
#include "esi_gl_defs.h"
#include "esi_stdio.h"
#include "esi_math.h"
#include "esi_ct.h"
#include "esi_qs.h"
#include "esi_glue.h"

/* list PROJMN
 *
 *     *** Initialize projection constants ***
 *
 *     Calling Sequence:
 *
 *     projmn_(pjcode,direction,units,return_code,print_flag,len1,len2,len3)
 *
 *     Arguments:
 *
 *     Name       Input/     Type       Description
 *                output
 *
 *     pjcode       I       *char[]     FINDER projectionn code.
 *
 *     direction    I       *char[]     Direction (LLXY or XYLL).
 *
 *     units        I       *char[]     Survey units (FS or MS).
 *
 *     return_code  O       *int        Return code.
 *
 *     print_flag   I       *int        Print flag.
 *                                      (for upward compatibility only)
 *
 *     len1,...     I        int        Fortran implied length variables.
 *
 *     Comments:
 *
 *     This routine is Fortran glue to the FINDER projection routines.
 *     It sets up two global structures called geodectic_proj and
 *     user_proj
 *
 * nolist PROJMN
 */
PROJECTION_STRUCTURE *geo_proj_ptr,*user_proj_ptr;
char current_direction[6];
char current_units[6];

#ifdef vms
void /*FUNCTION*/ projmn(pjcode,direction,units,return_code,print_flag)
DSC *pjcode,*direction,*units;
int *return_code,*print_flag;
{
     char usr_proj_code[20];
     int one = 1,status,set_proc=0;

/*
 *   Open FINDER data base
 */
     status=am_initialize();
     status=finder_config_project(set_proc);

/*   Set BATCH flag to turn off prompts */

   qs_set_i(QS_IF_BATCH,TRUE);


/*
 *   Get geodetic and user projections
 */
     if(ct_get_projection("GEODETIC",&geo_proj_ptr) != SUCCESS)
     {
        printf("Unable to get GEODETIC projection parameters");
     }
     qs_inq_c(QS_PROJECT_PROJECTION,usr_proj_code,(INT *)0);

     if(ct_get_projection(usr_proj_code,&user_proj_ptr) != SUCCESS)
     {
        printf("Unable to get %s projection parameters",usr_proj_code);
     }

     DSCTOCS(direction, current_direction);
     DSCTOCS(units, current_units);

}
#endif

#if UNIX
void /*FUNCTION*/ projmn_(pjcode,direction,units,return_code,print_flag,len1,len2,len3)
char *pjcode,*direction,*units;
int *return_code,*print_flag;
int len1,len2,len3;
{
     char usr_proj_code[20];
     int one = 1,status,set_proc=0;

/*
 *   Open FINDER data base
 */
     status=am_initialize();
     status=finder_config_project(set_proc);

/*   Set BATCH flag to turn off prompts */

   qs_set_i(QS_IF_BATCH,TRUE);


/*
 *   Get geodetic and user projections
 */
     if(ct_get_projection("GEODETIC",&geo_proj_ptr) != SUCCESS)
     {
        printf("Unable to get GEODETIC projection parameters");
     }
     qs_inq_c(QS_PROJECT_PROJECTION,usr_proj_code,(INT *)0);

     if(ct_get_projection(usr_proj_code,&user_proj_ptr) != SUCCESS)
     {
        printf("Unable to get %s projection parameters",usr_proj_code);
     }

     ftocstr(direction, current_direction, len2);
     ftocstr(units, current_units, len3);
     current_direction[ts_str_trim(current_direction)]='\0';
     current_units[ts_str_trim(current_units)]='\0';

}
#endif

#ifdef primos
void /*FUNCTION*/ projmn(pjcode,direction,units,return_code,print_flag,len10,len1,len20,len2,len30,len3)
char *pjcode,*direction,*units;
int return_code,print_flag;
int len10, len20, len30;
int len1,len2,len3;
{
     extern INT am_initialize();
     extern INT finder_config_project();
     extern INT qs_set_i();
     extern INT ct_get_projection();

     char usr_proj_code[20];
     int one = 1,status,set_proc=0;

/*
 *   Open FINDER data base
 */
     status=am_initialize();
     status=finder_config_project(set_proc);

/*   Set BATCH flag to turn off prompts */

   qs_set_i(QS_IF_BATCH,TRUE);


/*
 *   Get geodetic and user projections
 */
     if(ct_get_projection("GEODETIC",&geo_proj_ptr) != SUCCESS)
     {
        printf("Unable to get GEODETIC projection parameters");
     }
     qs_inq_c(QS_PROJECT_PROJECTION,usr_proj_code,(INT *)0);

     if(ct_get_projection(usr_proj_code,&user_proj_ptr) != SUCCESS)
     {
        printf("Unable to get %s projection parameters",usr_proj_code);
     }

     ftocstr((CHAR *)&direction, current_direction, len2);
     ftocstr((CHAR *)&units, current_units, len3);
     current_direction[ts_str_trim(current_direction)]='\0';
     current_units[ts_str_trim(current_units)]='\0';
     ctofstr((CHAR *)&direction, current_direction, len2);
     ctofstr((CHAR *)&units, current_units, len3);

}
#endif


