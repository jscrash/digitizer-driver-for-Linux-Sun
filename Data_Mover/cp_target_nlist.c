/* DEC/CMS REPLACEMENT HISTORY, Element CP_TARGET_NLIST.C */
/* *6    11-JUN-1991 16:17:59 PURNA "(SPR 0) add functionality" */
/* *5     6-MAY-1991 16:55:26 PURNA "(SPR 0) init change" */
/* *4     3-JAN-1991 07:48:21 VINCE "(SPR 1) Use esi_c_lib include instead of esi_stdlib to compile on UNIX" */
/* *3    23-AUG-1990 15:34:07 PURNA "(SPR 0) fix compiling error" */
/* *2    14-JUN-1990 11:33:44 PURNA "(SPR 0) new element" */
/* *1    14-JUN-1990 11:32:35 PURNA "child process for cp_project" */
/* DEC/CMS REPLACEMENT HISTORY, Element CP_TARGET_NLIST.C */

#include "esi_gl_defs.h"
#include "esi_c_lib.h"

#include "esi_fi.h"
#include "esi_qs.h"
#include "esi_ho.h"
#include "esi_ts.h"
#include "esi_cp.h"
#include "esi_ct.h"

typedef struct
{
  INT table_flag;
  INT_FUNCTION_PTR function;
} CP_EXTENDS_METHOD_HDR;

#define DEFAULT_RECSIZE 256

privatedef INT curline;
privatedef FILENAME nlist_file;
privatedef NLIST_ID nlist_id;
privatedef FILENAME full_filename;

int main(argc, argv)
INT argc;
CHAR **argv;
{
    CHAR *source_project;
    CHAR *target_project;
    FILENAME list_filename;

    PATHNAME logical_name;
    UINT col_list[2]; 
    INT table_flag;
    CHAR key_column[32]; 

    NLIST_HEADER nlist,nlist_out;
    BOOL is_batch;
    INT status,i;

    FILE *list_file;
    CHAR *getstat, *ptr;
    CHAR inbuf[DEFAULT_RECSIZE+1];
    CHAR temp_string[8];
    INT recsize;

    FLOAT ll_x,ll_y,ur_x,ur_y;
    VOIDPTR value_list[2];
    UINT ncols;
    PROJECTION_ID source_projection,target_projection;
    PROJECTION_STRUCTURE *source_data,*target_data;
    BOOL do_trans = FALSE;

			     /*	determine if this is a batch run	    */

    if (ho_user_mode() == HO_BATCH) is_batch = TRUE;
    else is_batch = FALSE;
    qs_set_i(QS_IF_BATCH, is_batch);

    if (or_login(argc, argv) IS_NOT_EQUAL_TO SUCCESS)
        {
        printf("Unable to log onto Oracle - Aborting");
        exit (EXIT_FAILURE);
        }

    source_project=ts_sto_upper(argv[2]);
    target_project=ts_sto_upper(argv[3]);
    strcpy(list_filename,argv[4]);

    printf("Copy nlist on target project %s using input file %s",
	   target_project,
	   list_filename);

			     /*	configure target project		    */

    qs_set_c(QS_PROJECT_NAME,target_project,strlen(target_project));
    status=finder_config_project(TRUE);
    if(status != SUCCESS)
	{
	printf(" failed on finder_config_project '%s', error status %d\n",
	target_project,status);
	or_logout(FALSE);
	exit(EXIT_FAILURE);
	}

    status = cp_get_project_projection(source_project,source_projection)
	     OR
             cp_get_project_projection(target_project,target_projection);
    if(status != SUCCESS) 
      {
	printf(" failed to retrieve project projection\n");
	or_logout(FALSE);
	exit(EXIT_FAILURE);
      }
        
    if(ARE_DIFFERENT(source_projection,target_projection))
      {
	ctz_get_projection(source_project,source_projection,&source_data);
	ctz_get_projection(target_project,target_projection,&target_data);
	do_trans = TRUE;
      }

    list_file = (FILE *)ho_open(list_filename,"r",NULL);
    if (list_file EQUALS (FILE *)0)
      {
	printf(" failed to open list file %s\n",list_filename);
	or_logout(FALSE);
	exit(EXIT_FAILURE);
      }

    printf("\n");

    recsize = DEFAULT_RECSIZE;
    while(getstat = (char *)fgets((char *)inbuf,recsize,list_file))
      {
	inbuf[recsize]='\0';

	    /* line no */
	ptr=inbuf;
	temp_string[0]='\0';
	strncpy(temp_string,ptr,6);
	temp_string[6]='\0';
	curline=atoi(temp_string);
	
	    /* full filename from source project */
	ptr=inbuf+6;
	full_filename[0]='\0';
	strncpy(full_filename,ptr,80);
	full_filename[80]='\0';
	ts_snowhite(full_filename);

	    /* logical name */
	ptr=inbuf+86;
	logical_name[0]='\0';
	strncpy(logical_name,ptr,30);
	logical_name[30]='\0';
	ts_snowhite(logical_name);

	    /* nlist file */
	ptr=inbuf+116;
	nlist_file[0]='\0';
	strncpy(nlist_file,ptr,30);
	nlist_file[30]='\0';
	ts_snowhite(nlist_file);

	    /* nlist id */
	ptr=inbuf+146;
	nlist_id[0]='\0';
	strncpy(nlist_id,ptr,30);
	nlist_id[30]='\0';
	ts_snowhite(nlist_id);

	    /* x coord dim list in nlist */
	ptr=inbuf+176;
	temp_string[0]='\0';
	strncpy(temp_string,ptr,4);
	temp_string[4]='\0';
	col_list[0] = (UINT)atoi(temp_string);
	
	    /* y coord dim list in nlist */
	ptr=inbuf+180;
	temp_string[0]='\0';
	strncpy(temp_string,ptr,4);
	temp_string[4]='\0';
	col_list[1] = (UINT)atoi(temp_string);

	    /* table flag */
	ptr=inbuf+184;
	temp_string[0]='\0';
	strncpy(temp_string,ptr,4);
	temp_string[4]='\0';
	table_flag = (UINT)atoi(temp_string);

	if(table_flag EQUALS CP_GRAPHIC_OBJECT_EXTENDS OR
	   table_flag EQUALS CP_LYNX_OBJECTS_EXTENDS) 
	    {
	    ptr=inbuf+188;
	    key_column[0]='\0';
	    strncpy(key_column,ptr,30);
	    key_column[30]='\0';
	    ts_snowhite(key_column);
	    status = cp_update_nlist_pathname(key_column,table_flag,nlist_file);
	    }
	
	inbuf[0]='\0';
			     /*	read nlist from source project		    */
	status=nl_read_nlist(&nlist,full_filename,nlist_id);
	if(status != SUCCESS)
	  {
	    printf("On line %d ",curline);
	    printf("failed on reading nlist '%s' in filename '%s', \
error status %d\n",nlist_id,full_filename,status);	
	    continue;
	  }


			     /*	transform nlist points into the default	    */
			     /*	project projection on target project	    */

	if(do_trans AND col_list[0] != 0 AND col_list[1] != 0)
	  {
	    status = ct_transform_nlist(nlist,source_data,&nlist_out,
					target_data,col_list,0.0);		
	    if(status == SUCCESS)
	      {
		nl_free_nlist(nlist);
		nlist=nlist_out;
	      }
	  }		     

	status = cpz_call_extends_method(table_flag,nlist,col_list);
	if(status != SUCCESS) continue;

			     /*	write nlist into target project directory   */
	status = ho_add_path(logical_name,nlist_file,MAXPATHLENGTH,
			     full_filename);
	if(status != SUCCESS)
	  {
	    printf("On line %d ",curline);
	    printf("failed on add pathname with logical name '%s' \
and filename '%s', error status %d\n",logical_name,nlist_file,status);	
	    continue;
	  }
    
	status=nl_write_nlist(nlist,full_filename,nlist_id);
	if(status != SUCCESS)
	  {
	    printf("On line %d ",curline);
	    printf("failed on writing nlist '%s' in filename '%s', \
error status %d\n",nlist_id,full_filename,status);
	    continue;
	  }
	printf("NLIST (File %s Id %s) is copied \n",full_filename,nlist_id);
      }
    
        /* close and delete input filename */
    fclose(list_file);
    ho_delete_file(list_filename,&status);

    printf("\n\nNormal successful completion\n");
    or_logout(TRUE);
    exit(EXIT_SUCCESS);
}



privatedef INT cpz_call_extends_method(table_flag,nlist,col_list)
INT table_flag;
NLIST_HEADER nlist;
UINT *col_list;
{
    FLOAT ll_x,ll_y,ur_x,ur_y;
    VOIDPTR value_list[2];
    UINT ncols=2;
    INT status;

    if(table_flag EQUALS NO_EXTENDS) return SUCCESS;

    value_list[0] = (VOIDPTR)&ll_x;
    value_list[1] = (VOIDPTR)&ll_y;
    status = nl_inq_nlist_info(nlist,NL_NLIST_MIN,ncols,
			   col_list,value_list);
    if(status != SUCCESS)
      {
	printf("On line %d ",curline);
	printf("failed on retrieving lower left extends for \
seismic line, error status %d\n",status);	
	return status;
      }
    value_list[0] = (VOIDPTR)&ur_x;
    value_list[1] = (VOIDPTR)&ur_y;
    status = nl_inq_nlist_info(nlist,NL_NLIST_MAX,ncols,
			       col_list,value_list);
    if(status != SUCCESS)
      {
	printf("On line %d ",curline);
	printf("failed on retrieving upper right extends for \
nlist %s, error status %d\n",nlist_file,status);	
	return status;
      }
    switch(table_flag)
      {
      case CP_SEIS_XY_EXTENDS :
	status = cp_upd_seis_extends(nlist_file,nlist_id,
				 (DOUBLE)ll_x,(DOUBLE)ll_y,
				 (DOUBLE)ur_x,(DOUBLE)ur_y);
	break;
      case CP_GRAPHIC_OBJECT_EXTENDS :
      case CP_FAULT_TRACE_EXTENDS :
	status = cp_upd_graphic_object(table_flag,nlist_file,nlist_id,
				 (DOUBLE)ll_x,(DOUBLE)ll_y,
				 (DOUBLE)ur_x,(DOUBLE)ur_y);
	break;
      case CP_LYNX_ARCS_EXTENDS :
	status = cp_upd_lynx_arc(nlist_file,nlist_id,
				 (DOUBLE)ll_x,(DOUBLE)ll_y,
				 (DOUBLE)ur_x,(DOUBLE)ur_y);
	break;
      case CP_LYNX_OBJECTS_EXTENDS :
	status = cp_upd_lynx_cult(nlist_file,nlist_id,
				 (DOUBLE)ll_x,(DOUBLE)ll_y,
				 (DOUBLE)ur_x,(DOUBLE)ur_y);
	break;
      }
    if(status != SUCCESS)
	return status;

    return SUCCESS;

}      
