/* DEC/CMS REPLACEMENT HISTORY, Element LD_LG_BATCH.C */
/* *6    10-DEC-1990 12:53:55 VINCE "(SPR 6089) Removed extra call to init_parser before the filename is known" */
/* *5    23-AUG-1990 17:48:38 PURNA "(SPR 0) compiling error fix" */
/* *4     9-AUG-1990 22:02:11 GILLESPIE "(SPR 2001) Change OS dependency switch" */
/* *3    12-JUL-1990 17:23:51 JULIAN "(SPR -1) Make program a single module" */
/* *2    18-APR-1990 00:02:33 JULIAN "(SPR -1) Re-write this loader to support multiple input files + much more" */
/* *1    25-MAR-1990 09:19:41 JULIAN "standalone ascii log loader." */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LG_BATCH.C */

/* INCLUDE FILES */

#include "esi_c_lib.h"

#ifndef ESI_CTYPE_H
#include "esi_ctype.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_TIME_H
#include "esi_time.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef  ESI_LD_ASDEF_H
#include "esi_ld_asdef.h"
#endif

#ifndef ESI_LG_LDEFS_H
#include "esi_lg_ldefs.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_TU_ERR_H
#include "esi_tu_err.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_ORACLE_H
#include "esi_oracle.h"
#endif

#ifndef ESI_LD_ERR_H
#include "esi_ld_err.h"
#endif

#ifndef ESI_LD_MSG_H
#include "esi_ld_msg.h"
#endif

#ifndef ESI_WE_VFDEF_H
#include "esi_we_vfdef.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#include "ld_lg_ascii.rh"
#include "we_vfy_wl.rh"

#if USE_PROTOTYPES

privatedef INT ldlgb_print_msg (INT id,INT item, CHAR *string);
privatedef TCP ldlgb_ascii_tcp_traces (CHAR buffer[]);
privatedef INT ld_lg_ascii_begin_well (LD_LG_ASCII_STRUCT *pstruct);
privatedef INT ld_lg_ascii_load_requested (LD_LG_ASCII_STRUCT *pstruct);
privatedef INT ld_lg_ascii_verify_well_switch (VERIFY_WELL_STRUCT *pvws);
privatedef INT ld_lg_ascii_begin_data (LD_LG_ASCII_STRUCT *pstruct);

privatedef INT ldlgb_add_to_nlist (LD_LG_ASCII_STRUCT *ptrBIG, FLOAT *record);
privatedef INT ldlgb_determine_buffer_type (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_fill_buffer (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_get_parameter_string (LD_LG_ASCII_STRUCT *ptrBIG,
					   CHAR *keyword, CHAR *string);

privatedef INT ldlgb_get_next_token (LD_LG_ASCII_STRUCT *ptrBIG, CHAR *token);
privatedef INT ldlgb_strip_all_blanks (CHAR *string);
privatedef INT ldlgb_string_to_parms (CHAR *string, CHAR ***out_parameter);
privatedef INT ldlgb_get_data_records (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_save_data (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_mk_lt_struct (LD_LG_ASCII_STRUCT *ptrBIG,
				   LOG_TRACE_STRUCT *log_trace,
				   NLIST_HEADER nlist, CHAR *trace_name);

privatedef INT ldlgb_mk_trace_name (LD_LG_ASCII_STRUCT *ptrBIG, INT column,
				    CHAR *trace_name);

privatedef INT ldlgb_convert_format_parameter (CHAR *parameter,
					       FS *format_structure);

privatedef INT ldlgb_format_param_to_struct (CHAR **str_parms, FS ***exp_fs);
privatedef INT ldlgb_expand_format_structure (FS **from, FS ***ptr_to);
privatedef INT ldlgb_isfloat (CHAR *x);

privatedef INT ldlgb_get_uom_conv (LD_LG_ASCII_STRUCT *ptrBIG, DOUBLE *conv,
				   LOG_UOM depth_uom);

privatedef INT ldlgb_load_data_record (LD_LG_ASCII_STRUCT *ptrBIG,
				       INT *number_errors);

privatedef INT ldlgb_init_load_data (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_cleanup_log_data (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_create_a_stroke (LD_LG_ASCII_STRUCT *ptrBIG);
privatedef INT ldlgb_compute_fill_number (LD_LG_ASCII_STRUCT *ptrBIG);

privatedef CHAR *ldlgb_timestamp ();
privatedef void usage ();

#else

privatedef CHAR *ldlgb_timestamp ();
privatedef INT ldlgb_print_msg ();
privatedef CHAR **ldlgb_ascii_tcp_traces ();
privatedef INT ld_lg_ascii_begin_well ();
privatedef INT ld_lg_ascii_load_requested ();
privatedef INT ld_lg_ascii_verify_well_switch ();
privatedef INT ld_lg_ascii_begin_data ();
privatedef INT ldlgb_add_to_nlist ();
privatedef INT ldlgb_determine_buffer_type ();
privatedef INT ldlgb_fill_buffer ();
privatedef INT ldlgb_get_parameter_string ();
privatedef INT ldlgb_get_next_token ();
privatedef INT ldlgb_strip_all_blanks ();
privatedef INT ldlgb_string_to_parms ();
privatedef INT ldlgb_get_data_records ();
privatedef INT ldlgb_save_data ();
privatedef INT ldlgb_mk_lt_struct ();
privatedef INT ldlgb_mk_trace_name ();
privatedef INT ldlgb_convert_format_parameter ();
privatedef INT ldlgb_format_param_to_struct ();
privatedef INT ldlgb_expand_format_structure ();
privatedef INT ldlgb_isfloat ();
privatedef INT ldlgb_get_uom_conv ();
privatedef INT ldlgb_load_data_record ();
privatedef INT ldlgb_init_load_data ();
privatedef INT ldlgb_cleanup_log_data ();
privatedef INT ldlgb_create_a_stroke ();
privatedef INT ldlgb_compute_fill_number  ();
privatedef void usage ();

#endif

#ifdef primos
#define CTIME_TIME_OFFSET 10
#else
#define CTIME_TIME_OFFSET 11
#endif

privatedef FILENAME LD_LG_ASCII_CDLGS_FILE = "LD_LG_ASCII";
privatedef FILENAME last_file_used         = "";

#define WE_VERIFY_WELL_CDLG_FILE "we_vfy_wl"

#define XALLOCATE(dtype)        ((dtype *) tc_zalloc( sizeof (dtype)))
#define XSTRING_ALLOCATE(stype) ((CHAR *) tc_zalloc( strlen(stype) + 1))
#define xtc_zalloc(x)           tc_zalloc( (x))
#define xfree(x)                tc_free(x)

#define YSTRING_ALLOCATE(stype) ((CHAR *) tc_zalloc(strlen(stype) + 1))
#define ytc_zalloc(x)           tc_zalloc((x))
#define yfree(x)                tc_free(x)


#ifdef ZERO
#undef ZERO
#endif
#define ZERO                       '0'

/* GLOBAL DEFINES */

#define PARM_SIZE                  50

#ifdef NULL_STRING
#undef NULL_STRING
#endif
#define NULL_STRING                ""

#ifdef NULL_CHAR
#undef NULL_CHAR
#endif
#define NULL_CHAR                  '\0'

#ifdef LEFT_PAREN
#undef LEFT_PAREN
#endif
#define LEFT_PAREN                 '('

#ifdef RIGHT_PAREN
#undef RIGHT_PAREN
#endif
#define RIGHT_PAREN                ')'

#ifdef SPACE
#undef SPACE
#endif
#define SPACE                      ' '

#ifdef COMMA
#undef COMMA
#endif
#define COMMA                      ','

#define NOT_DONE                   7301
#define DONE                       7302
#define FACTOR_STATE               7341
#define FWIDTH_STATE               7342
#define IWIDTH_STATE               7343
#define DIVISOR_STATE              7344
#define SKIP_STATE                 7345
#define UWI_HEADER_RECORD          7361
#define FORMAT_HEADER_RECORD       7362
#define CURVES_HEADER_RECORD       7363
#define TRACES_HEADER_RECORD       7364
#define NULL_HEADER_RECORD         7365
#define SAMPLE_HEADER_RECORD       7366
#define DELIMITER_HEADER_RECORD    7367
#define DATA_RECORD                7368
#define END_OF_FILE_RECORD         7369
#define UOM_HEADER_RECORD          7370
#define SOURCE_HEADER_RECORD       7371
#define END_OF_FILE_TOKEN          7381
#define COMMA_TOKEN                7382
#define RIGHT_PAREN_TOKEN          7383
#define LEFT_PAREN_TOKEN           7384
#define STRING_TOKEN               7385


/* LOCAL FUNCTIONS */
#ifdef MIN_ARGS
#undef MIN_ARGS
#endif

#define MIN_ARGS	3		   /* pgm  ora_login datafile */
#define LDSTRMAX	64
privatedef INT ldlgb_global_ifbatch = FALSE;

privatedef CHAR  msgbuf  [1024];
privatedef UINT Dim_List[2] = {1,2};
privatedef INT theargc = 0;
privatedef CHAR **theargv;

/***************************************************************************/
privatedef void usage(pgmname)
     CHAR *pgmname;
{
  FILENAME pgm;
  
  ldlgb_print_msg(0,1,"\r  (v1.2)  Copyright 1989,90    Finder Graphics Inc.\n");
  ldlgb_print_msg(0,1,"\rUsage:  LD_LG_BATCH  acct/pass  -proj project  [filename]");
  ldlgb_print_msg(0,1,"");
  ldlgb_print_msg(0,1,"\rOptions & Datafile header overrides:\n");
  ldlgb_print_msg(0,1,"\r       -DELimit  <data Delimiter>   Def: <---->");
  ldlgb_print_msg(0,1,"\r       -Depth    <depth trace>      Def: DEPTH");
  ldlgb_print_msg(0,1,"\r       -Filelist <filename>   ");
  ldlgb_print_msg(0,1,"\r       -Null     <null value>       Def: -99999.0");
  ldlgb_print_msg(0,1,"\r       -SAmple   <sample_inc>       Def: {from hdr}");
  ldlgb_print_msg(0,1,"\r       -SOurce   <data source>      Def: ASCII");
  ldlgb_print_msg(0,1,"\r       -TRaces   <trace[,...]>      Def: {from hdr}");
  ldlgb_print_msg(0,1,"\r       -Log				  ");
  ldlgb_print_msg(0,1,"\r       -Uom      <uom spec>         Def: Project Def.");
  ldlgb_print_msg(0,1,"\r       -Help                  ");
  ldlgb_print_msg(0,1,"");
  ldlgb_print_msg(0,1,"\r\tSpecify either a single data file name, or the");
  ldlgb_print_msg(0,1,"\r\tname of a file containing a list of names. (-File arg.)");
  exit(1);
}

main (argc,argv)
     INT argc;
     CHAR *argv[];
{
  FILE *flist_fp = (FILE *)0;
  FILE *locfp    = (FILE *)0;
  
  INT i;
  INT last_str;
  INT status  = SUCCESS;
  INT count   = 0;
  INT ifbatch = TRUE;
  
  CHAR  text[1024];
  CHAR  uom_buf[64];
  CHAR *delimit     = NULL;
  CHAR *depth_trace = NULL;
  CHAR *filelist    = NULL;
  CHAR *source      = NULL;
  CHAR *project     = NULL;
  CHAR *str_index   = NULL;
  CHAR *uom         = NULL;
  
  DOUBLE nullvalue = 0.0;
  DOUBLE sample    = 0.0;
  
  FILENAME		fname;
  UOM_STRUCTURE		uom_data;
  PROJECT_DEFAULT	*proj;
  LD_LG_ASCII_STRUCT	*pstruct;
  VERIFY_WELL_STRUCT	*pvws;
  WELL_STRUCT		*pwell;
  WELL_HDR_1		*pwell1;
  
  /* ------------------------------------------------------------ */
  theargc = argc;
  theargv = argv;
  
  if (argc <= MIN_ARGS) 
    usage(argv[0]);
  
  if (strchr(argv[1],'/') == NULL)
    {
      ldlgb_print_msg(0,FAIL,"Oracle account/password must be first argument.");
      usage(argv[0]);
    }
  
  sprintf(text,"\t*** Ascii Log Loader starting at %s ***",ldlgb_timestamp());
  ldlgb_print_msg(0,1,text);
  
  ldlgb_global_ifbatch = (ho_user_mode() == HO_BATCH) ? TRUE : FALSE;
  qs_set_i(QS_IF_BATCH,ldlgb_global_ifbatch);
  if (or_login(argc,argv) != SUCCESS)
    {
      sprintf(text,"Problem logging into account %s.\t  status = %d",
	      argv[0],argv[1],status);
      ldlgb_print_msg(0,0,text);
      usage(argv[0]);
    }
  
  status =  am_define_workspace( ASCII_WORKSPACE,AM_APPLICATION,
				sizeof (LD_LG_ASCII_STRUCT),(VOIDPTR *) &pstruct);
  
  pstruct->format       = (CHAR *) tc_zalloc(sizeof(CHAR) * LDSTRMAX);
  pstruct->delimit      = (CHAR *) tc_zalloc(sizeof(CHAR) * LDSTRMAX);
  pstruct->depth_trace  = (CHAR *) tc_zalloc(sizeof(CHAR) * LDSTRMAX);
  pstruct->trace_names  = (CHAR **)0;
  
  for (i = 2; i < argc; i++) 
    {
      /* ----------------------------------------------------------- */
      /* don't upper case the argument until after it is checked for */
      /* filenameness, this will let it be UNIX friendly.            */
      /* ----------------------------------------------------------- */
      
      if (strncmp(argv[i],"-",1) != 0)
	{
	  strcpy(fname,argv[i]);
	  continue;
	}
      
      ts_sto_upper(argv[i]);
      
      if (strncmp(argv[i], "-H", 2) == 0)  
	{
	  usage(argv[0]);
	}
      
      if (strncmp(argv[i], "-L", 2) == 0)  
	{
	  qs_set_i(QS_IF_BATCH,TRUE);
	  ldlgb_global_ifbatch = TRUE;		  
	}
      
      if (strncmp(argv[i], "-SO", 3) == 0)
	{
	  if (++i >= argc) usage(argv[0]);
	  ts_sto_upper(argv[i]);
	  source = argv[i];
	  continue;
	}
      
      if (strncmp(argv[i], "-U", 2) == 0)
	{
	  if (++i >= argc) usage(argv[0]);
	  ts_sto_upper(argv[i]);
	  uom = argv[i];
	  continue;
	}
      
      if (strncmp(argv[i], "-SA", 3) == 0) 
	{
	  if (++i >= argc) usage(argv[0]);
	  sample = atof(argv[i]);
	  continue;
	}
      
      if (strncmp(argv[i], "-DE", 3) == 0) 
	{
	  if (++i >= argc) usage(argv[0]);
	  ts_sto_upper(argv[i]);
	  delimit = argv[i];
	  continue;
	}
      
      if (strncmp(argv[i], "-TR", 3) == 0)  
	{
	  if (++i >= argc) usage(argv[0]);
	  pstruct->trace_names = ldlgb_ascii_tcp_traces( argv[i] );
	  
	  /* Identify which field is depth.       */
	  continue;
	}
      
      if (strncmp(argv[i], "-F", 2) == 0)  
	{
	  if (++i >= argc) usage(argv[0]);
	  
	  filelist = argv[i];
	  if (*filelist == '-' OR (strlen(filelist) == 0) )
	    {
	      ldlgb_print_msg(0,FAIL," -FILE arg needs more...");
	      usage(argv[0]);
	    }
	  
	  flist_fp = ho_open(filelist,"r",NULL);
	  if (flist_fp == (FILE *)0)
	    {
	      sprintf(text,"Can't open file '%s'.\n\tAborting...",
		      filelist);
	      ldlgb_print_msg(0,FAIL,text);
	      goto exit_vec;
	    }
	  else
	    {
	      sprintf(text,"Reading file names from list: %s",filelist);
	      ldlgb_print_msg(0,0,text);
	    }
	  continue;
	}
      
      if (strncmp(argv[i], "-P", 2) == 0) 
	{
	  if (++i >= argc) 
	    {
	      ldlgb_print_msg(0,FAIL,"Missing project argument.");
	      usage(argv[0]);
	    }
	  
	  ts_sto_upper(argv[i]);
	  project = argv[i];
	  continue;
	}
      
      if (strncmp(argv[i], "-DE", 3) == 0) 
	{
	  if (++i >= argc) usage(argv[0]);
	  ts_sto_upper(argv[i]);
	  depth_trace = argv[i];
	  continue;
	}
      
      if (strncmp(argv[i], "-N", 2) == 0) 
	{
	  if (++i >= argc) usage(argv[0]);
	  nullvalue = atof(argv[i]);
	  continue;
	}
    }
  
  /* Set up some default parameters. */
  pstruct->err_limit    = 10;
  pstruct->depth_column = 0;
  pstruct->sample_incr  = 0.5;
  pstruct->null_value   = LOGNULL;
  pstruct->uwi[0]       = '\0';
  
  strcpy( pstruct->format, "8F10.4");
  if (depth_trace != NULL )
    strcpy( pstruct->depth_trace, depth_trace);
  else
    strcpy( pstruct->depth_trace, "DEPTH" );
  
  if (delimit == NULL)
    strcpy( pstruct->delimit, "<--->");
  else
    strcpy(pstruct->delimit,delimit);
  
  if (source == NULL ) 
    strcpy( pstruct->source, "ASCII" );
  else
    strcpy(pstruct->source,ts_sto_upper(source));
  
  pstruct->units_button = LD_LG_ASCII_1OF2_FEET;
  if (strchr(pstruct->source,':') == NULL AND 
      strchr(pstruct->source,';') == NULL AND
      strchr(pstruct->source,'.') == NULL AND
      strchr(pstruct->source,',') == NULL)
    {}
  else                      
    {                                           /* Disallow punctuation.  */
      ts_convert_char(pstruct->source, ':', ' ');        /* ':'--->' '     */
      ts_convert_char(pstruct->source, '.', ' ');        /* '.'--->' '     */
      ts_convert_char(pstruct->source, ';', ' ');        /* ';'--->' '     */
      ts_convert_char(pstruct->source, ',', ' ');        /* ','--->' '     */
    }
  
  if (project == NULL)
    {
      ldlgb_print_msg(0,FAIL,"A project must be specified.    (-proj projname)");
      usage();
    }
  
  qs_set_c(QS_PROJECT_NAME,project,0);
  
  if ((status =  am_initialize()) != SUCCESS)
    {
      ldlgb_print_msg(0,FAIL,"Problem initializing application manager:");
      ldlgb_print_msg(0,0,mg_message(status));
    }

  if ((status = finder_config_project(FALSE)) != SUCCESS)
    {
      ldlgb_print_msg(0,FAIL,"Problem configuring project:");
      ldlgb_print_msg(0,0,mg_message(status));
    }
  
  status = am_open_workspace("PROJECT_DEFAULTS",AM_GLOBAL,(VOIDPTR *)&proj);
  if (status != SUCCESS)
    {
      ldlgb_print_msg(0,FAIL,"Problem opening project defaults workspace:");
      ldlgb_print_msg(0,0,mg_message(status));
    }
  
  if (uom == NULL)
    {
      strcpy(pstruct->depth_uom,proj->unit.vertical);
      sprintf(text,"Depth UOM: %s\t(Project Default)",pstruct->depth_uom);
      ldlgb_print_msg(0,1,text);
    }
  else
    strcpy(pstruct->depth_uom,uom);
  
  if (tu_verify_uom(pstruct->depth_uom,&uom_data) == TU_INVALID_UOM)
    {
      sprintf(text,"Invalid Depth UOM. [%s]",pstruct->depth_uom);
      ldlgb_print_msg(0,FAIL,text);
      ldlgb_print_msg(0,1,mg_message(LD_LG_ASCII_BAD_DEFAULT_UNITS));
      ldlgb_print_msg(0,0,"Aborting...");
      goto exit_vec;
    }
  
  
  sprintf(text,"\tProject is %s",project);
  ldlgb_print_msg(0,1,text);
  
  /* Initialize the application structure */
  /* These values should come from the DB */
  
  status = am_define_workspace( VERIFY_WORKSPACE,AM_APPLICATION,
			       sizeof (VERIFY_WELL_STRUCT),(VOIDPTR *) &pvws);
  
  pstruct->pf = (FILE *)0;
  locfp = (FILE *)0;
  
  while(ldlgb_next_filename(flist_fp,fname) == SUCCESS)
    {
      sprintf(text,"\nProcessing %s...",fname);
      ldlgb_print_msg(0,0,text);
      if (pstruct->pf != (FILE *)0 AND (pstruct->pf == locfp))
	{
	  if (pstruct->ptrLOCAL)
	    {
	      tc_free(pstruct->ptrLOCAL);
	      fclose(pstruct->pf);
	    }
	}
      
      locfp =  ho_open(fname,"r",NULL);
      pstruct->pf = locfp;
      
      if (pstruct->pf == NULL_FILE_PTR )
	{
	  sprintf(text,"Can't find %s\tSkipping %s...",
		  mg_message( LD_LG_ASCII_FILE_NOT_FOUND ),fname);
	  ldlgb_print_msg(0,FAIL,text);
	  continue;
	}
      
      ld_lg_ascii_init_parser ( pstruct);
      
      status =  ld_lg_ascii_begin_well(pstruct);
      
      /* Verify that number of fields    */
      /* found in the format stmt match  */
      /* the number of trace names.      */
      
      status = ld_lg_ascii_verify_fmt(pstruct);
      switch (status)
	{
	case SUCCESS:
	  /* Format OK. Verify well exists.  */
	  pvws = (VERIFY_WELL_STRUCT *)tc_zalloc(sizeof (VERIFY_WELL_STRUCT) +4);
	  strcpy(pvws->uwi,pstruct->uwi);
	  pvws->show_well = FALSE;
	  if(we_get_well_header(pvws->uwi,WELL_HDR_1_DETAIL,&pwell) == SUCCESS)
	    {
	      pvws->well_exists = TRUE;
	      pwell1 = (WELL_HDR_1 *)pwell->detail.ptr;
	      
	      ldlgb_print_msg(0,1,"Dumping Header information:");
	      
	      sprintf(text,"UWI: %s   Operator: %s",
		      pwell->uwi,pwell1->operator);
	      ldlgb_print_msg(0,1,text);
	      
	      sprintf(text,"Name: %s",pwell1->name);
	      ldlgb_print_msg(0,1,text);
	      
	      sprintf( text, "Elevation: %8.2f", pwell1->elevation.value );
	      ldlgb_print_msg(0,1,text);
	      
	      sprintf( text, "Elev. Ref: %s ",pwell1->elevation.ref);
	      ldlgb_print_msg(0,1, text);
	      
	      status = ld_lg_ascii_begin_data( pstruct );
	      switch (status)
		{
		case DATA_BLOCK_NOT_FOUND:
		  ldlgb_print_msg(0,FAIL,mg_message(LD_LG_ASCII_SEQUENCE_ERROR));
		  status = ld_lg_ascii_begin_well(pstruct);   /* Try This well */
		  break;
		  
		case END_OF_FILE_BLOCK_FOUND:
		  ldlgb_print_msg(0,FAIL,mg_message(LD_LG_ASCII_UNEXPECTED_EOF));
		  continue;
		  
		case DATA_BLOCK_FOUND:
		  sprintf(text,"Loading %s...into %s.",
			  fname,pstruct->uwi);
		  ldlgb_print_msg(0,0,text);
		  status = ld_lg_ascii_load_data( pstruct );    /* LOAD! */
		  
		  switch (status) {
		  case LD_LG_ASCII_ERROR_LIMIT:
		    /* Error limit caused abort. Alert the operator.        */
		    sprintf(text, "%s --- %s", pstruct->uwi,
			    mg_message(LD_LG_ASCII_ERROR_LIMIT));
		    ldlgb_print_msg(0,FAIL,text);
		    break;
		    
		  case SUCCESS:            /* Successfully loaded!       */
		    sprintf(text, "%s --- %s", pstruct->uwi,
			    mg_message(LD_LG_ASCII_DATA_LOADED));
		    ldlgb_print_msg(0,0,text);
		    break;
		  }
		  break;
		}
	      tc_free(pvws);
	    }
	  else
	    {
	      sprintf(text,"UWI %s not in database.  Skipping this file...",
		      pstruct->uwi);
	      ldlgb_print_msg(0,FAIL,text);
	      pvws->well_exists = FALSE;    /* Don't verify */
	    }
	  break;
	  
	case FORMAT_BAD:
	  /* Format was not acceptable. */
	  sprintf(text,"%s",mg_message(LD_LG_ASCII_FMT_BAD));
	  ldlgb_print_msg(0,FAIL,text);
	  
	  sprintf(text,"Bad FORMAT field. (%s)   Skipping this file...",
		  pstruct->format);
	  ldlgb_print_msg(0,FAIL,text);
	  break;
	  
	case NOT_ENOUGH_TRACE_NAMES:
	  /* Format was not acceptable. */
	  /* Put up CDLG & try again.   */
	  ldlgb_print_msg(0,FAIL,mg_message(LD_LG_ASCII_FMT_UNMATCHED));
	  break;
	}
    }
  
 exit_vec:    
  if (flist_fp != (FILE *)0)    fclose(flist_fp);
  if (pstruct->pf != (FILE *)0) fclose( pstruct->pf );
  exit(1);
}


/***************************************************************************/
privatedef INT ld_lg_ascii_init_parser(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  ptrBIG->ptrLOCAL = XALLOCATE (PLS);
  
  ptrBIG->ptrLOCAL->format_structures  = (FS **)NULL;
  ptrBIG->ptrLOCAL->number_all_formats = 0;
  ptrBIG->ptrLOCAL->number_ns_formats  = 0;
  ptrBIG->ptrLOCAL->data_records       = (DS **)NULL;
  ptrBIG->ptrLOCAL->eof_flag           = FALSE;
  
  /* fill buffer */
  strcpy(ptrBIG->ptrLOCAL->buffer, NULL_STRING);
  ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
  ldlgb_fill_buffer(ptrBIG);
}

/***************************************************************************/
privatedef INT ld_lg_ascii_get_block_type(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT status;
  INT buffer_type;
  
  buffer_type = ldlgb_determine_buffer_type(ptrBIG);
  
  switch (buffer_type)
    {
    case UWI_HEADER_RECORD:
    case FORMAT_HEADER_RECORD:
    case CURVES_HEADER_RECORD:
    case TRACES_HEADER_RECORD:
    case NULL_HEADER_RECORD:
    case SAMPLE_HEADER_RECORD:
    case SOURCE_HEADER_RECORD:
    case UOM_HEADER_RECORD:
      status = HEADER_BLOCK_FOUND;
      break;
      
    case DELIMITER_HEADER_RECORD:
      status = DELIMITER_BLOCK_FOUND;
      break;
      
    case DATA_RECORD:
      status = DATA_BLOCK_FOUND;
      break;
      
    case END_OF_FILE_RECORD:
      status = END_OF_FILE_BLOCK_FOUND;
      break;
    }
  
  return( status );
}

/***************************************************************************/
privatedef INT ld_lg_ascii_skip_block(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT blktype;
  INT pblktype;
  
  pblktype = blktype = ld_lg_ascii_get_block_type(ptrBIG);
  
  while (blktype EQUALS pblktype)
    {
      strcpy(ptrBIG->ptrLOCAL->buffer, NULL_STRING);
      ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
      ldlgb_fill_buffer(ptrBIG);
      
      blktype = ld_lg_ascii_get_block_type(ptrBIG);
    }
}

/***************************************************************************/
privatedef INT ld_lg_ascii_load_data(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT  status;
  INT  blktype;
  INT  num_errors, number_errors, record_number = 0;
  CHAR text[512];
  
  status = ldlgb_init_load_data(ptrBIG);
  if (status != SUCCESS)
    return status;
  
  blktype = ld_lg_ascii_get_block_type(ptrBIG);
  
  if (blktype EQUALS DELIMITER_BLOCK_FOUND)
    ld_lg_ascii_skip_block(ptrBIG);
  
  blktype = ld_lg_ascii_get_block_type(ptrBIG);
  
  if (blktype != DATA_BLOCK_FOUND)
    status = LD_LG_ASCII_DATA_BLK_NOT_FOUND;
  
  /* reset parameters */
  num_errors = 0;
  record_number = 0;
  while (blktype EQUALS DATA_BLOCK_FOUND)
    {
      record_number++;
      if ((record_number%100) EQUALS 0)
	{
	  sprintf(text, mg_message(LD_LG_ASCII_READING), record_number);
	  ldlgb_print_msg(0,1,text); 
	}
      
      if (num_errors <= ptrBIG->err_limit)
	{
	  status = ldlgb_get_data_records(ptrBIG);
	  if (status != SUCCESS)
	    {
	      /* an error occured while reading, so increment num_errors
		 and go back and read another record */
	      sprintf(msgbuf,mg_message(LD_LG_ASCII_ERR_IN_REC),
		      record_number, ptrBIG->ptrLOCAL->buffer);
	      ldlgb_print_msg(ptrBIG->message_id, LD_LG_ASCII_ERROR_PROMPT, msgbuf);
	      num_errors++;
	    }
	  else
	    {
	      status = ldlgb_load_data_record(ptrBIG, &number_errors);
	      if (status != SUCCESS)
		break;
	      else
		num_errors += number_errors;
	    }
	}
      else
	status = LD_LG_ASCII_ERROR_LIMIT;
      
      *(ptrBIG->ptrLOCAL->buffer) = NULL_CHAR;
      ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
      ldlgb_fill_buffer(ptrBIG);
      
      blktype = ld_lg_ascii_get_block_type(ptrBIG);
    }
  
  if (status EQUALS SUCCESS)
    status = ldlgb_save_data(ptrBIG);
  
  if (status != SUCCESS)
    {
      ldlgb_cleanup_log_data(ptrBIG);
      return ( status );
    }
  else
    {
      status = ldlgb_cleanup_log_data(ptrBIG);
      return (status);
    }
}

/***************************************************************************/
privatedef INT ld_lg_ascii_read_headers(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT status = SUCCESS;
  INT blktype;
  INT buffer_type;
  INT slen;
  CHAR string[BUF_SIZE+1];
  
  blktype = ld_lg_ascii_get_block_type(ptrBIG);
  
  if (blktype EQUALS DELIMITER_BLOCK_FOUND)
    ld_lg_ascii_skip_block(ptrBIG);
  
  blktype = ld_lg_ascii_get_block_type(ptrBIG);
  
  if (blktype != HEADER_BLOCK_FOUND)
    status = LD_LG_ASCII_HDR_BLK_NOT_FOUND;
  
  while (blktype EQUALS HEADER_BLOCK_FOUND)
    {
      buffer_type = ldlgb_determine_buffer_type(ptrBIG);
      
      switch(buffer_type)
	{
	case UWI_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "UWI", string) != SUCCESS)
	    status = LD_LG_ASCII_BAD_HEADER;
	  slen = MIN(strlen(string), sizeof(UWI)-1);
	  strncpy(ptrBIG->uwi, string, slen);
	  ptrBIG->uwi[slen] = NULL_CHAR;
	  break;
	  
	case FORMAT_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "FORMAT", string) != SUCCESS)
	    status = HEADER_BAD;
	  ptrBIG->format = XSTRING_ALLOCATE(string);
	  strcpy(ptrBIG->format, string);
	  break;
	  
	case CURVES_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "CURVES", string) != SUCCESS)
	    status = HEADER_BAD;
	  if (ldlgb_string_to_parms(string, &ptrBIG->trace_names) != SUCCESS)
	    status = HEADER_BAD;
	  break;
	  
	case TRACES_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "TRACES", string) != SUCCESS)
	    status = HEADER_BAD;
	  if (ldlgb_string_to_parms(string, &ptrBIG->trace_names) != SUCCESS)
	    status = HEADER_BAD;
	  break;
	  
	case NULL_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "NULL", string) != SUCCESS)
	    status = HEADER_BAD;
	  if (ldlgb_strip_all_blanks(string) EQUALS SUCCESS AND 
	      ldlgb_isfloat(string) > 0)
	    ptrBIG->null_value =  atof(string);
	  else
	    {
	      ptrBIG->null_value = LOGNULL;
	      status  = HEADER_BAD;
	    }
	  break;
	  
	case SAMPLE_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "SAMPLE", string) != SUCCESS)
	    status  = HEADER_BAD;
	  if (ldlgb_strip_all_blanks(string) EQUALS SUCCESS AND
	      ldlgb_isfloat(string) > 0)
	    ptrBIG->sample_incr =  atof(string);
	  else
	    {
	      ptrBIG->sample_incr = .05;
	      status  = LD_LG_ASCII_BAD_HEADER;
	    }
	  
	  break;
	  
	case SOURCE_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "SOURCE", string) != SUCCESS)
	    status  = HEADER_BAD;
	  if (ldlgb_strip_all_blanks(string) EQUALS SUCCESS)
	    strcpy(ptrBIG->source,string);
	  else
	    {
	      strcpy(ptrBIG->source,"ASCII");
	      status  = LD_LG_ASCII_BAD_HEADER;
	    }
	  break;
	  
	case UOM_HEADER_RECORD:
	  if (ldlgb_get_parameter_string(ptrBIG, "UOM", string) != SUCCESS)
	    status  = HEADER_BAD;
	  if (ldlgb_strip_all_blanks(string) EQUALS SUCCESS)
	    strcpy(ptrBIG->depth_uom,string);
	  else
	    {
	      strcpy(ptrBIG->depth_uom,"METERS");
	      status  = LD_LG_ASCII_BAD_HEADER;
	    }
	  break;
	  
	}
      
      strcpy(ptrBIG->ptrLOCAL->buffer, NULL_STRING);
      ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
      ldlgb_fill_buffer(ptrBIG);
      
      blktype = ld_lg_ascii_get_block_type(ptrBIG);
    }
  
  return (status);
}

/***************************************************************************/
privatedef INT ld_lg_ascii_verify_fmt(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT i;
  INT status = SUCCESS;
  INT ncurves = 0;
  INT nformats = 0;
  CHAR **f_parms = (CHAR **) NULL;
  
  if (ldlgb_string_to_parms(ptrBIG->format, &f_parms) != SUCCESS)
    status = FORMAT_BAD;
  else if (ldlgb_format_param_to_struct(f_parms, &ptrBIG->ptrLOCAL->format_structures) != SUCCESS)
    status = FORMAT_BAD;
  else
    {
      /* count number of trace names */
      for (i = 0; ((ptrBIG->trace_names != (CHAR **) NULL) AND
		   (ptrBIG->trace_names[i] != (CHAR *) NULL)); i++)
	{
	  ncurves++;
	}
      
      nformats = 0;
      
      /* count number of non-skip formats */
      for (i = 0; ((ptrBIG->ptrLOCAL->format_structures != (FS **) NULL) AND
		   (ptrBIG->ptrLOCAL->format_structures[i] != (FS *) NULL));
	   i++)
	{
	  if ((ptrBIG->ptrLOCAL->format_structures[i]->type != 'X') AND
	      (ptrBIG->ptrLOCAL->format_structures[i]->type != 'x'))
	    {
	      nformats++;
	    }
	}
      
      ptrBIG->ptrLOCAL->number_ns_formats = nformats;
      
      nformats= 0;
      /* count number of skiped and non-skipped formats */
      for (i = 0; ((ptrBIG->ptrLOCAL->format_structures != (FS **) NULL) AND
		   (ptrBIG->ptrLOCAL->format_structures[i] != (FS *) NULL));
	   i++)
	{
	  nformats++;
	}
      
      ptrBIG->ptrLOCAL->number_all_formats = nformats;
      
      if (ptrBIG->ptrLOCAL->number_ns_formats EQUALS ncurves)
	status = SUCCESS;
      else
	status = NOT_ENOUGH_TRACE_NAMES;
    }
  
  return( status );
}

/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_BEGIN_WELL                                 */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        To commence processing of a well, with or without      */
/*                 header records.                                        */
/*                                                                        */
/* Parameters:     pstruct - Application data structure.                  */
/*                                                                        */
/* Status Code:    SUCCESS                                                */
/*                                                                        */
/* ********************************************************************** */
privatedef INT ld_lg_ascii_begin_well( pstruct )
     
     LD_LG_ASCII_STRUCT *pstruct;
     
{
  INT status;
  CHAR text[512];
  
  /* Determine the block type.                 */
  
  status = ld_lg_ascii_get_block_type( pstruct );
  
  /* Skip any delimiter blocks.                */
  
  while (status == DELIMITER_BLOCK_FOUND)
    {
      status = ld_lg_ascii_skip_block( pstruct );
      status = ld_lg_ascii_get_block_type( pstruct );
    }
  
  /* Since we are looking for a header block,  */
  /* data blocks should have uwi blanked and   */
  /* the verify header invoked.  EOF starts    */
  /* the server to get a file name.            */
  
  switch (status)
    {
    case HEADER_BLOCK_FOUND:
      status = ld_lg_ascii_read_headers( pstruct );
      if (status != SUCCESS)
	{
	  ldlgb_print_msg(0,FAIL,mg_message(LD_LG_ASCII_BAD_HDR));
	}
      else
	{
	  sprintf(text,"UWI: %s\tSource: %s",pstruct->uwi,
		  pstruct->source);
	  ldlgb_print_msg(0,1,text);
	  
	  sprintf(text,"Format: %s\tDelimiter: %s",pstruct->format,pstruct->delimit);
	  ldlgb_print_msg(0,1,text);
	  
	  sprintf(text,"Depth UOM: %s\tMax Errors: %d",pstruct->depth_uom,pstruct->err_limit);
	  ldlgb_print_msg(0,1,text);
	  
	  sprintf(text,"Null Value: %f",pstruct->null_value);
	  ldlgb_print_msg(0,1,text);
	}
      break;
      
    case DATA_BLOCK_FOUND:
      strcpy(pstruct->uwi, "");
      break;
      
    case END_OF_FILE_BLOCK_FOUND:
      
      fclose( pstruct->pf );
      break;
    }
  return status;
}

/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_BEGIN_DATA                                 */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        This function positions the file to the beginning of   */
/*                 the next block of data records.                        */
/*                                                                        */
/* Parameters:     pstruct - pointer to the application data structure.   */
/*                                                                        */
/* Status Codes:   DATA_BLOCK_FOUND                                       */
/*                 DATA_BLOCK_NOT_FOUND                                   */
/*                 END_OF_FILE_BLOCK_FOUND                                */
/*                                                                        */
/* ********************************************************************** */
privatedef INT ld_lg_ascii_begin_data(pstruct)
     
     LD_LG_ASCII_STRUCT *pstruct;       /* Pointer to appl. data structure */
     
{
  INT status;
  INT n_hdr;
  
  n_hdr = 0;
  
  status = ld_lg_ascii_get_block_type(pstruct);
  
  while (status == HEADER_BLOCK_FOUND OR status == DELIMITER_BLOCK_FOUND)
    {
      if (status == HEADER_BLOCK_FOUND)
	{
	  ++n_hdr;
	  if (n_hdr > 1) return DATA_BLOCK_NOT_FOUND;
	}
      
      status = ld_lg_ascii_skip_block( pstruct );
      status = ld_lg_ascii_get_block_type( pstruct );
    }
  
  return status;
}

/* ************************************************************* */
#define LDMAXFNAME 256
privatedef INT ldlgb_next_filename(fp,outptr)
     FILE *fp;
     CHAR *outptr;
{
  CHAR	   buf[LDMAXFNAME*2];
  INT	   status,
  done;
  static FILENAME lastfile;
  INT len;
  
  status = FAIL;		/* assume the worst */
  if (fp == (FILE *)0)
    {
      if ((len=strlen(outptr)) > 0 AND (strncmp(lastfile,outptr,len-1)))
	{
	  ts_snowhite(outptr);
	  status = (hoexists(outptr) == TRUE) ?  SUCCESS : FAIL;
	}
    }
  else
    {
      status = SUCCESS;
      done = FALSE;
      while (!done)
	{
	  if(fgets(buf,LDMAXFNAME,fp) != NULL)
	    {
	      ts_snowhite(buf);
	      if (hoexists(buf) == TRUE)
		{
		  strcpy(outptr,buf);
		  done = TRUE;
		}
	      else
		{
		  *outptr = '\0';
		  done = FALSE;
		}
	    }
	  else
	    {
	      status = FAIL;
	      done = TRUE;
	    }
	}
    }
  
  if (status == SUCCESS)
    strcpy(lastfile,outptr);
  return status;
}	

/* ********************************************************************** */

privatedef CHAR *ldlgb_timestamp()
{
  static struct loc_time {
    time_t	bintime;
    CHAR	timestr[64];
    CHAR	*timeptr;
  } ltime;
  
  time(&ltime.bintime);
  ltime.timeptr = (char *)ctime(&ltime.bintime);
  strncpy(ltime.timestr,ltime.timeptr+CTIME_TIME_OFFSET,9);
  ltime.timestr[8] = '\0';
  
  return ltime.timestr;
}

/* **************************************************************** */


privatedef CHAR **ldlgb_ascii_tcp_traces( buffer )
     
     CHAR buffer[];
{
  INT len;
  INT n_str;
  INT i;
  CHAR **tcp;
  CHAR *pbuffer;
  
  ts_noblanks( buffer );             /* Remove all blanks.              */
  ts_sto_upper( buffer );            /* Convert to all UPPER CASE.      */
  len = strlen( buffer );            /* Determine the length of buffer. */
  ts_str_convert( buffer, len, ',', '\0');    /* Convert ',' to NULL.   */
  pbuffer = buffer;
  for (n_str=1, i=0; i<len; i++, pbuffer++)   /* Count substrings.     */
    if (*pbuffer == NUL) n_str++;
  tcp = ts_make_tcp( buffer, n_str );          /* Build TCP!            */
  
  return tcp;
}


/***************************************************************************/
privatedef INT ldlgb_compute_fill_number(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  DOUBLE sample_incr, value_wanted, current, difference, tolerance;
  INT depth, num_errors = 0, done = FAIL;
  INT retval;
  
  if (ptrBIG->ptrLOCAL->tot_count EQUALS 0)
    return 0;
  
  depth = ptrBIG->depth_column;
  sample_incr = ptrBIG->sample_incr;
  
  if (sample_incr <= 0.0)
    sample_incr = 0.5;
  
  tolerance = sample_incr * 0.025;
  
  current = ptrBIG->ptrLOCAL->curr_record[depth];
  value_wanted = ptrBIG->ptrLOCAL->prev_record[depth] + sample_incr;
  
  num_errors = 0;
  while (num_errors <= ptrBIG->err_limit AND done EQUALS FAIL)
    {
      difference = current - value_wanted;
      
      if ( fabs(difference) < tolerance)
	{
	  /* don't need any bogus records */
	  retval = 0;
	  done = SUCCESS;
	}
      else if (difference < 0)
	{
	  /* this record is decreasing, not increasing */
	  retval = -1;
	  done = SUCCESS;
	}
      else if (difference > 0)
	{
	  /* a bogus record is needed */
	  num_errors++;
	  value_wanted += sample_incr;
	}
    }
  
  return retval;
}

/***************************************************************************/
privatedef INT ldlgb_create_a_stroke(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT i, status;
  UINT NDim = 1, DList[1];
  UINT stroke_num;
  VOIDPTR VList[1];
  
  DList[0] = 1;
  for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
    {
      /* skip n-list if it is the depth column */
      if (i IS_NOT_EQUAL_TO ptrBIG->depth_column)
	{
	  status = nl_start_stroke(ptrBIG->ptrLOCAL->nlist[i], &stroke_num);
	  if (status IS_NOT_EQUAL_TO SUCCESS)
	    return status;
	  
	  if (stroke_num EQUALS 1)
	    {
	      /* using current record, set the stroke information */
	      VList[0] = (VOIDPTR) &ptrBIG->ptrLOCAL->curr_record[ptrBIG->depth_column];
	      status = nl_set_stroke_info(ptrBIG->ptrLOCAL->nlist[i],
					  stroke_num, NL_STROKE_START_VALUE,
					  NDim, DList, VList);
	      if (status IS_NOT_EQUAL_TO SUCCESS)
		return status;
	      /* Now set the input sample increment  */
	      VList[0] = (VOIDPTR) &ptrBIG->sample_incr;
	      status = nl_set_stroke_info(ptrBIG->ptrLOCAL->nlist[i],
					  stroke_num, NL_STROKE_INCREMENT,
					  NDim, DList, VList);
	      if (status IS_NOT_EQUAL_TO SUCCESS)
		return status;
	    }
	  else
	    {
	      /*using previous record, set the stroke information */
	      VList[0] = (VOIDPTR) &ptrBIG->ptrLOCAL->prev_record[ptrBIG->depth_column];
	      status = nl_set_stroke_info(ptrBIG->ptrLOCAL->nlist[i],
					  stroke_num, NL_STROKE_START_VALUE,
					  NDim, DList, VList);
	      if (status IS_NOT_EQUAL_TO SUCCESS)
		return status;
	      /* Now set the input sample increment  */
	      VList[0] = (VOIDPTR) &ptrBIG->sample_incr;
	      status = nl_set_stroke_info(ptrBIG->ptrLOCAL->nlist[i],
					  stroke_num, NL_STROKE_INCREMENT,
					  NDim, DList, VList);
	      if (status IS_NOT_EQUAL_TO SUCCESS)
		return status;
	    }
	}
    }
  
  if (stroke_num > 1)
    {
      /* add previous record (last stroke entry) to first element of nlist,
	 increment total count first so we dont recursively create a stroke */
      
      ptrBIG->ptrLOCAL->tot_count++;
      
      status = ldlgb_add_to_nlist(ptrBIG, ptrBIG->ptrLOCAL->prev_record);
      if (status IS_NOT_EQUAL_TO SUCCESS)
	return status;
    }
  
  return( SUCCESS );
}

/***************************************************************************/
privatedef INT ldlgb_add_to_nlist(ptrBIG, record)
     LD_LG_ASCII_STRUCT *ptrBIG;
     FLOAT *record;
{
  INT i, status;
  UINT Ndim = 2;
  VOIDPTR Value_List[2];
  
  if ((ptrBIG->ptrLOCAL->tot_count % 1000) EQUALS 0)
    {
      status = ldlgb_create_a_stroke(ptrBIG);
      if (status IS_NOT_EQUAL_TO SUCCESS)
	return status;
    }
  
  for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
    {
      /* add data to n-lists, except the depth one */
      if (i IS_NOT_EQUAL_TO ptrBIG->depth_column)
	{
	  Value_List[0] = (VOIDPTR) &record[ptrBIG->depth_column];
	  Value_List[1] = (VOIDPTR) &record[i];
	  
	  status = nl_add_points(ptrBIG->ptrLOCAL->nlist[i],
				 1, Ndim, Dim_List, Value_List);
	  if (status IS_NOT_EQUAL_TO SUCCESS)
	    return status;
	}
    }
  
  /* copy the newly added record to previous record */
  for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
    ptrBIG->ptrLOCAL->prev_record[i] = record[i];
  
  return( SUCCESS );
}

/***************************************************************************/
privatedef INT ldlgb_cleanup_log_data(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  int i, status = SUCCESS;
  
  yfree( ptrBIG->ptrLOCAL->curr_record );
  yfree( ptrBIG->ptrLOCAL->bogus_record );
  yfree( ptrBIG->ptrLOCAL->prev_record );
  
  for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
    status = nl_free_nlist( ptrBIG->ptrLOCAL->nlist[i] );
  
  return (status EQUALS SUCCESS) ? SUCCESS : status;
}


/***************************************************************************/
privatedef INT ldlgb_init_load_data(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT i, status, nformats;
  UINT Ndim = 2;
  INT Units[2];
  INT DType[2];
  size_t DSize[2];
  INT VInc[2];
  UINT Blocksize = 1001;
  privatedef CHAR *Dim_Names[] = {"DEPTH","VALUE"};
  
  /* for setting null value info */
  UINT NullDim = 1;
  UINT NDList[1];
  FLOAT fxnull;
  VOIDPTR Null_List[1];
  
  NDList[0] = 2;
  Null_List[0] = (VOIDPTR)&fxnull;
  DType[0] = DType[1] = FLOAT_TYPE;
  DSize[0] = DSize[1] = sizeof (FLOAT);
  VInc[0] = INCREMENT_LIST, VInc[1] = VALUE_LIST;
  
  nformats = ptrBIG->ptrLOCAL->number_ns_formats;
  fxnull = ptrBIG->null_value;
  
  ptrBIG->ptrLOCAL->tot_count = 0;
  
  if ((ptrBIG->ptrLOCAL->curr_record =
       (FLOAT *) ytc_zalloc(nformats * sizeof(FLOAT))) EQUALS (FLOAT *)0)
    return FAIL;
  
  if ((ptrBIG->ptrLOCAL->bogus_record =
       (FLOAT *) ytc_zalloc(nformats * sizeof(FLOAT))) EQUALS (FLOAT *)0)
    return FAIL;
  
  if ((ptrBIG->ptrLOCAL->prev_record =
       (FLOAT *) ytc_zalloc(nformats * sizeof(FLOAT))) EQUALS (FLOAT *)0)
    return FAIL;
  
  if ((ptrBIG->ptrLOCAL->nlist = (NLIST_HEADER *)
       ytc_zalloc(nformats *sizeof(NLIST_HEADER))) EQUALS (NLIST_HEADER *)0)
    return FAIL;
  
  /* initialize bogus_record */
  for (i = 0; i < nformats; i++)
    {
      ptrBIG->ptrLOCAL->bogus_record[i] = ptrBIG->null_value;
      
      /* initialize nlists */
      
      if ((status = nl_init_nlist(&ptrBIG->ptrLOCAL->nlist[i], Ndim, Units,
				  DSize, DType, VInc, Blocksize)) IS_NOT_EQUAL_TO SUCCESS)
	return status;
      
      if (status IS_NOT_EQUAL_TO SUCCESS)
	return status;
      
      status = nl_set_nlist_info(ptrBIG->ptrLOCAL->nlist[i],
				 NL_NLIST_NULL_VALUE, NullDim, NDList, Null_List);
      if (status IS_NOT_EQUAL_TO SUCCESS)
	return status;
      status = nl_set_nlist_info(ptrBIG->ptrLOCAL->nlist[i],
				 NL_NLIST_DIM_NAME, Ndim, Dim_List, (VOIDPTR *)Dim_Names);
      if (status IS_NOT_EQUAL_TO SUCCESS)
	return status;
    }
  
  return( SUCCESS );
}

/***************************************************************************/
privatedef INT ldlgb_fill_buffer(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  BOOL get_another_line;
  INT  line_len;
  CHAR line[BUF_SIZE+1];
  
  get_another_line = TRUE;
  while (ptrBIG->ptrLOCAL->eof_flag IS_NOT_EQUAL_TO TRUE AND get_another_line EQUALS TRUE)
    {
      if (fgets(line, BUF_SIZE, ptrBIG->pf) EQUALS NULL)
	{
	  ptrBIG->ptrLOCAL->eof_flag = TRUE;
	  get_another_line = FALSE;
	}
      else
	{
	  line_len = strlen(line);
	  line[line_len-1] = NULL_CHAR;
	}
      
      if (strlen(line) EQUALS strspn(line, " "))
	get_another_line = TRUE;
      
      else
	{
	  get_another_line = FALSE;
	  
	  strcpy(ptrBIG->ptrLOCAL->buffer, ptrBIG->ptrLOCAL->buffer_ptr);
	  strcat(ptrBIG->ptrLOCAL->buffer, line);
	  ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
	}
    }
}

/***************************************************************************/
privatedef INT ldlgb_determine_buffer_type(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT status;
  CHAR *string;
  
  string = ptrBIG->ptrLOCAL->buffer_ptr;
  string += strspn(string, " ");
  
  if (ptrBIG->ptrLOCAL->eof_flag EQUALS TRUE)
    status = END_OF_FILE_RECORD;
  
  else if (strncmp(string, "UWI", 3) EQUALS 0)
    status = UWI_HEADER_RECORD;
  
  else if (strncmp(string, "FORMAT", 6) EQUALS 0)
    status = FORMAT_HEADER_RECORD;
  
  else if (strncmp(string, "CURVES", 6) EQUALS 0)
    status = CURVES_HEADER_RECORD;
  
  else if (strncmp(string, "TRACES", 6) EQUALS 0)
    status = TRACES_HEADER_RECORD;
  
  else if (strncmp(string, "NULL", 4) EQUALS 0)
    status = NULL_HEADER_RECORD;
  
  else if (strncmp(string, "SAMPLE", 6) EQUALS 0)
    status = SAMPLE_HEADER_RECORD;
  
  else if (strncmp(string, "SOURCE", 6) EQUALS 0)
    status = SOURCE_HEADER_RECORD;
  
  else if (strncmp(string, "UOM", 3) EQUALS 0)
    status = UOM_HEADER_RECORD;
  
  else if (strncmp(string, ptrBIG->delimit,strlen(ptrBIG->delimit)) EQUALS 0)
    status = DELIMITER_HEADER_RECORD;
  
  else
    status = DATA_RECORD;
  
  return( status );
}

/***************************************************************************/
privatedef INT ldlgb_get_parameter_string(ptrBIG, keyword, string)
     LD_LG_ASCII_STRUCT *ptrBIG;
     CHAR *keyword;
     CHAR *string;
{
  INT state = 1;
  INT status = NOT_DONE;
  INT type;
  CHAR token[BUF_SIZE+1];
  
  string[0] = NULL_CHAR;
  
  /* find keyword, then move buffer_ptr */
  ptrBIG->ptrLOCAL->buffer_ptr += strspn(ptrBIG->ptrLOCAL->buffer_ptr, " ");
  if (strncmp(ptrBIG->ptrLOCAL->buffer_ptr, keyword,strlen(keyword))
      IS_NOT_EQUAL_TO 0)
    status = LD_LG_ASCII_NO_HEADER;
  else
    ptrBIG->ptrLOCAL->buffer_ptr += strlen(keyword);
  
  while (status EQUALS NOT_DONE)
    {
      type = ldlgb_get_next_token(ptrBIG, token);
      
      switch (state)
	{
	case 1:
	  if (type IS_NOT_EQUAL_TO LEFT_PAREN_TOKEN)
	    status = LD_LG_ASCII_NO_LEFT_PAREN;
	  else
	    state = 2;
	  break;
	  
	case 2:
	  if (type IS_NOT_EQUAL_TO STRING_TOKEN)
	    status = LD_LG_ASCII_NO_PARAMETER;
	  else
	    {
	      strcpy(string, token);
	      state = 3;
	    }
	  break;
	  
	case 3:
	  if (type EQUALS RIGHT_PAREN_TOKEN)
	    status = DONE;
	  else
	    status = LD_LG_ASCII_NO_RIGHT_PAREN;
	  break;
	}
    }
  
  if (status EQUALS DONE)
    {
      if (strlen(ptrBIG->ptrLOCAL->buffer_ptr) EQUALS
	  strspn(ptrBIG->ptrLOCAL->buffer_ptr, " "))
	{
	  status = SUCCESS;
	}
      else
	{
	  status = LD_LG_ASCII_GARBAGE_AT_EOL;
	}
    }
  
  return(status);
}

/***************************************************************************/
privatedef INT ldlgb_get_next_token(ptrBIG, token)
     LD_LG_ASCII_STRUCT *ptrBIG;
     CHAR *token;
{
  INT status = NOT_DONE;
  INT return_status;
  INT delimiter_position;
  INT buffer_length;
  
  while (status EQUALS NOT_DONE)
    {
      /* strip leading blanks */
      ptrBIG->ptrLOCAL->buffer_ptr += strspn(ptrBIG->ptrLOCAL->buffer_ptr, " ");
      buffer_length = strlen(ptrBIG->ptrLOCAL->buffer_ptr);
      
      if (buffer_length EQUALS 0)
	{
	  if (ptrBIG->ptrLOCAL->eof_flag EQUALS TRUE)
	    status = DONE;
	  else
	    ldlgb_fill_buffer(ptrBIG);
	}
      else
	{
	  delimiter_position = strcspn(ptrBIG->ptrLOCAL->buffer_ptr, "()");
	  
	  if ((buffer_length < BUF_SIZE) AND
	      (buffer_length EQUALS delimiter_position))
	    {
	      if (ptrBIG->ptrLOCAL->eof_flag EQUALS TRUE)
		status = DONE;
	      else
		ldlgb_fill_buffer(ptrBIG);
	    }
	  else
	    status = DONE;
	}
    }
  
  delimiter_position = strcspn(ptrBIG->ptrLOCAL->buffer_ptr, "()");
  
  if (buffer_length EQUALS 0)
    return_status = END_OF_FILE_TOKEN;
  
  else if (delimiter_position EQUALS 0)
    {
      switch (*ptrBIG->ptrLOCAL->buffer_ptr)
	{
	case LEFT_PAREN:
	  return_status = LEFT_PAREN_TOKEN;
	  break;
	  
	case RIGHT_PAREN:
	  return_status = RIGHT_PAREN_TOKEN;
	  break;
	}
      
      token[0] = NULL_CHAR;
      ptrBIG->ptrLOCAL->buffer_ptr++;
    }
  else
    {
      delimiter_position = MIN(BUF_SIZE, delimiter_position);
      strncpy(token, ptrBIG->ptrLOCAL->buffer_ptr, delimiter_position);
      token[delimiter_position] = NULL_CHAR;
      ptrBIG->ptrLOCAL->buffer_ptr += delimiter_position;
      ldlgb_strip_all_blanks(token);
      return_status = STRING_TOKEN;
    }
  
  return( return_status );
}

/***************************************************************************/
privatedef INT ldlgb_strip_all_blanks(string)
     CHAR *string;
{
  CHAR *from = string;
  CHAR *to = string;
  
  while (*from IS_NOT_EQUAL_TO NULL_CHAR)
    {
      if ((*from IS_NOT_EQUAL_TO SPACE) AND (*from IS_NOT_EQUAL_TO '\n') AND (*from IS_NOT_EQUAL_TO '\t'))
	*to++ = *from;
      
      from++;
    }
  
  *to = NULL_CHAR;
  
  return(SUCCESS);
}

/***************************************************************************/
privatedef INT ldlgb_convert_format_parameter(parameter, format_structure)
     CHAR *parameter;
     FS *format_structure;
{
  INT  factor = 0;
  CHAR type = SPACE;
  INT  width = 0;
  INT  divisor = 0;
  INT  state = FACTOR_STATE;
  INT  status = NOT_DONE;
  
  /* if first character is a letter, then factor is assummed to be 1 */
  if ((*parameter IS_NOT_EQUAL_TO NULL_CHAR) AND
      ((*parameter EQUALS 'F') OR (*parameter EQUALS 'f') OR
       (*parameter EQUALS 'I') OR (*parameter EQUALS 'i') OR
       (*parameter EQUALS 'E') OR (*parameter EQUALS 'e') OR
       (*parameter EQUALS 'X') OR (*parameter EQUALS 'x')))
    {
      factor = 1;
    }
  
  while ((status EQUALS NOT_DONE) AND (*parameter IS_NOT_EQUAL_TO NULL_CHAR))
    {
      if (state EQUALS SKIP_STATE)
	status = LD_LG_ASCII_BAD_FORMAT_CHAR;
      
      else if (isdigit(*parameter) IS_NOT_EQUAL_TO 0)
	{
	  switch (state)
	    {
	    case FACTOR_STATE:
	      factor = factor*10 + (*parameter - ZERO);
	      break;
	      
	    case IWIDTH_STATE:
	    case FWIDTH_STATE:
	      width = width*10 + (*parameter - ZERO);
	      break;
	      
	    case DIVISOR_STATE:
	      divisor = divisor*10 + (*parameter - ZERO);
	      break;
	      
	    default:
	      status = LD_LG_ASCII_BAD_FORMAT_CHAR;
	      break;
	    }
	}
      else if ((*parameter EQUALS 'F') OR (*parameter EQUALS 'f') OR
	       (*parameter EQUALS 'I') OR (*parameter EQUALS 'i') OR
	       (*parameter EQUALS 'E') OR (*parameter EQUALS 'e') OR
	       (*parameter EQUALS 'X') OR (*parameter EQUALS 'x'))
	{
	  if (state EQUALS FACTOR_STATE)
	    {
	      switch (type = *parameter)
		{
		case 'F':
		case 'f':
		case 'E':
		case 'e':
		  state = FWIDTH_STATE;
                  break;
		case 'I':
		case 'i':
		  state = IWIDTH_STATE;
                  break;
		case 'X':
		case 'x':
		  state = SKIP_STATE;
                  break;
		}
	    }
	  else
	    status = LD_LG_ASCII_BAD_FORMAT_CHAR;
	}
      else if (*parameter EQUALS '.')
	{
	  if (state EQUALS FWIDTH_STATE)
	    state = DIVISOR_STATE;
	  else
	    status = LD_LG_ASCII_BAD_FORMAT_CHAR;
	}
      else
	status = LD_LG_ASCII_BAD_FORMAT_CHAR;
      
      parameter++;
    }
  
  if (status IS_NOT_EQUAL_TO LD_LG_ASCII_BAD_FORMAT_CHAR)
    {
      if (factor EQUALS 0)
	status = LD_LG_ASCII_BAD_FORMAT_CHAR;
      
      else if ((type EQUALS 'X') OR (type EQUALS 'x'))
	{
	  format_structure->factor = 1;
	  format_structure->type = type;
	  format_structure->width = factor;
	  format_structure->divisor = 0;
	  
	  status = SUCCESS;
	}
      else if (width EQUALS 0)
	status = LD_LG_ASCII_BAD_FORMAT_CHAR;
      
      else
	{
	  format_structure->factor = factor;
	  format_structure->type = type;
	  format_structure->width = width;
	  format_structure->divisor = divisor;
	  
	  status = SUCCESS;
	}
    }
  
  return( status );
}

/***************************************************************************/
privatedef INT ldlgb_string_to_parms(string, out_parameter)
     CHAR *string;
     CHAR ***out_parameter;
{
  INT i;
  INT  memsize = 0;
  CHAR parameter[PARM_SIZE+1];
  INT  delimiter_position;
  INT status = NOT_DONE;
  CHAR **temp_arr;
  
  if (*out_parameter IS_NOT_EQUAL_TO (CHAR **) NULL)
    {
      ts_tcp_free( *out_parameter );
      *out_parameter = (CHAR **) NULL ;
    }
  
  if (IS_EMPTY_STRING(string ))
    {
      status = LD_LG_ASCII_NO_PARAMETER;
    }
  
  for (i = 0; status EQUALS NOT_DONE; i++)
    {
      if (i%100 EQUALS 0)
	{
	  memsize += 100;
	  temp_arr = (CHAR **) ytc_zalloc((memsize + 1) * sizeof(CHAR *));
#if 0
	  if (*out_parameter IS_NOT_EQUAL_TO (CHAR **) NULL)
	    {
	      for (j = 0; (*out_parameter)[j] IS_NOT_EQUAL_TO (CHAR *) NULL; j++)
		temp_arr[j] = (*out_parameter)[j];
	      
	      yfree(*out_parameter);
	    }
#endif
	  *out_parameter = temp_arr;
	}
      
      delimiter_position = strcspn(string, ",");
      
      if (delimiter_position EQUALS 0)
	status = LD_LG_ASCII_NO_PARAMETER;
      else
	{
	  delimiter_position = MIN(PARM_SIZE, delimiter_position);
	  strncpy(parameter, string, delimiter_position);
	  parameter[delimiter_position] = NULL_CHAR;
	  
	  (*out_parameter)[i] = YSTRING_ALLOCATE(parameter);
	  strcpy((*out_parameter)[i], parameter);
	  
	  string += delimiter_position;
	  
	  if (IS_EMPTY_STRING(string))
	    status = SUCCESS;
	  
	  else if ((strlen(string) > 1) AND (*string EQUALS COMMA))
	    string++;
	  
	  else if ((strlen(string) EQUALS 1) AND (*string EQUALS COMMA))
	    status = LD_LG_ASCII_NO_PARAMETER;
	}
    }
  
  return( status );
}

/***************************************************************************/
privatedef INT ldlgb_expand_format_structure(from, ptr_to)
     FS **from;
     FS ***ptr_to;
{
  INT nforms = 0;
  INT i;
  INT j;
  INT k;
  FS **to = *ptr_to;
  
#if 0
  if ( to IS_NOT_EQUAL_TO (FS **) NULL)
    {
      for (i = 0; to[i] IS_NOT_EQUAL_TO (FS *) NULL; i++)
	xfree(to[i]);
      
      xfree(to);
    }
#endif
  
  if (from IS_NOT_EQUAL_TO (FS **) NULL)
    {
      for (i = 0; from[i] IS_NOT_EQUAL_TO (FS *) NULL; i++)
	nforms += from[i]->factor;
    }
  
  *ptr_to = (FS **) xtc_zalloc((nforms+1)*sizeof(FS *));
  to = *ptr_to;
  
  for (i = 0, k = 0; from[i] IS_NOT_EQUAL_TO (FS *) NULL; i++)
    {
      for (j = 0; j < from[i]->factor; j++)
	{
	  to[k] = XALLOCATE( FS ) ;
	  
	  to[k]->factor = 1;
	  to[k]->type = from[i]->type;
	  to[k]->width = from[i]->width;
	  to[k]->divisor = from[i]->divisor;
	  k++;
	}
    }
}

/***************************************************************************/
privatedef INT ldlgb_format_param_to_struct(str_parms, exp_fs)
     CHAR **str_parms;
     FS ***exp_fs;
{
  FS **com_fs    = (FS **) NULL;
  INT i;
  INT nparms = 0;
  INT status = SUCCESS;
  
  /* allocate format structures */
  
  if (str_parms IS_NOT_EQUAL_TO (CHAR **) NULL)
    {
      for (i = 0; str_parms[i] IS_NOT_EQUAL_TO (CHAR *) NULL; i++)
	nparms++;
    }
  
  com_fs = (FS **) xtc_zalloc((nparms+1)*sizeof(FS *));
  
  for (i = 0; ((i < nparms) AND (status EQUALS SUCCESS)); i++)
    {
      com_fs[i] = XALLOCATE( FS ) ;
      status = ldlgb_convert_format_parameter(str_parms[i], com_fs[i]);
    }
  
  if ((com_fs[0] IS_NOT_EQUAL_TO (FS *) NULL) AND (status EQUALS SUCCESS))
    ldlgb_expand_format_structure(com_fs, exp_fs);
  
#if 0
  if (com_fs IS_NOT_EQUAL_TO (FS **) NULL)
    {
      for (i = 0; com_fs[i] IS_NOT_EQUAL_TO (FS *) NULL; i++)
	xfree(com_fs[i]);
      
      xfree(com_fs);
    }
#endif
  
  return (status);
}

/***************************************************************************/
privatedef INT ldlgb_get_data_records(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT  i, j, k, xsl, sl, status = SUCCESS, a_period;
  CHAR string[BUF_SIZE+1], *pp;
  
  j = 0;
  for (i = 0; i < ptrBIG->ptrLOCAL->number_all_formats; i++)
    {
      /* string copy to string[] and check if there is a period */
      /* set sl to MIN(width, strlen(buffer_ptr)) */
      for (xsl = 0, sl = 0, pp = ptrBIG->ptrLOCAL->buffer_ptr, a_period = FALSE;
	   sl < ptrBIG->ptrLOCAL->format_structures[i]->width AND
	   *pp IS_NOT_EQUAL_TO NULL_CHAR;
	   sl++, pp++)
	{
	  if (*pp IS_NOT_EQUAL_TO SPACE)
	    {
	      string[xsl] = *pp;
	      xsl++;
	      if (*pp EQUALS '.')
		a_period = TRUE;
	    }
	}
      string[xsl] = NULL_CHAR;
      
      switch (ptrBIG->ptrLOCAL->format_structures[i]->type)
	{
	case 'X':
	  break;
	  
	case 'F':
	case 'f':
	case 'E':
	case 'e':
	case 'I':
	case 'i':
	  if (sl EQUALS ptrBIG->ptrLOCAL->format_structures[i]->width)
	    {
	      if (ldlgb_isfloat(string) > 0)
		{
		  ptrBIG->ptrLOCAL->curr_record[j] = atof(string);
		  
		  if (a_period EQUALS FALSE)
		    for (k = 0; k < ptrBIG->ptrLOCAL->format_structures[i]->divisor; k++)
		      ptrBIG->ptrLOCAL->curr_record[j] /= 10.0;
		}
	      else
		{
		  status = LD_LG_ASCII_BAD_DATA_RECORD;
		  ptrBIG->ptrLOCAL->curr_record[j] = ptrBIG->null_value;
		}
	    }
	  else
	    {
	      status = LD_LG_ASCII_BAD_DATA_RECORD;
	      ptrBIG->ptrLOCAL->curr_record[j] = ptrBIG->null_value;
	    }
	  j++;
	  break;
	}
      
      ptrBIG->ptrLOCAL->buffer_ptr += MIN(sl, ptrBIG->ptrLOCAL->format_structures[i]->width);
    }
  
  return(status);
}


/***************************************************************************/
privatedef INT ldlgb_save_data(ptrBIG)
     LD_LG_ASCII_STRUCT *ptrBIG;
{
  INT nformats = 0;
  INT column;
  INT depth = ptrBIG->depth_column;
  LOG_TRACE_NAME trace_name;
  INT status = SUCCESS;
  LOG_TRACE_STRUCT log_trace;
  CHAR text[512];
  
  
  if ( ptrBIG->ptrLOCAL->tot_count <= 0 )
    return FAIL;
  
  nformats =  ptrBIG->ptrLOCAL->number_ns_formats;
  
  for (column = 0; column < nformats; column++)
    {
      if (column IS_NOT_EQUAL_TO depth)	/* skip this processing if this is the depth column */
	{
	  status = ldlgb_mk_trace_name(ptrBIG, column, trace_name);
	  if (status IS_NOT_EQUAL_TO SUCCESS)
	    return(status);
	  sprintf(text,"Processing trace %s....",trace_name);
	  ldlgb_print_msg(0,0,text);
	  
	  status = ldlgb_mk_lt_struct(ptrBIG, &log_trace,
				      ptrBIG->ptrLOCAL->nlist[column], trace_name);
	  if (status IS_NOT_EQUAL_TO SUCCESS)
	    return(status);
	  
	  sprintf(text,"%s\t(%s)", mg_message(LD_LG_ASCII_WRITING),
		  trace_name);
	  ldlgb_print_msg(0,0,text);
	  
	  /* trim trailing spaces from the TRACE_UOM field, so that */
	  /* it won't be too long for LG_LTDM_ADD_TRACE             */
	  ts_trim (log_trace.trace_uom);
	  
	  status = lg_ltdm_add_trace(&log_trace);
	  if (status IS_NOT_EQUAL_TO SUCCESS)
	    {
	      sprintf(text,"%s\t(%s)", mg_message(LD_LG_ASCII_NOT_WRITTEN),
		      trace_name);
	      ldlgb_print_msg(0,FAIL,text);
	      return(status);
	    }
	  sprintf(text,"%s\t(%s)",mg_message(LD_LG_ASCII_WRITTEN),
		  trace_name);
	  ldlgb_print_msg(0,0,text);
	}
    }
  return (status);
}

/***************************************************************************/
privatedef INT ldlgb_mk_lt_struct(ptrBIG, log_trace, nlist, trace_name)
     LD_LG_ASCII_STRUCT *ptrBIG;
     LOG_TRACE_STRUCT *log_trace;
     NLIST_HEADER nlist;
     CHAR *trace_name;
{
  INT status = SUCCESS;
  UINT stroke_num = 0;
  UINT NDim;
  UINT DList[2];
  INT xtype, ytype;
  VOIDPTR Type[2];
  FLOAT fxmin, fymin, fxmax, fymax;
  INT   ixmin, iymin, ixmax, iymax;
  VOIDPTR MinVal[2];
  VOIDPTR MaxVal[2];
  DOUBLE conv;
  FLOAT fconv;
  VOIDPTR VList[1];
  LOG_UOM depth_units;
  
  tc_zeroes((VOIDPTR)log_trace, sizeof (LOG_TRACE_STRUCT));
  
  status = ldlgb_get_uom_conv( ptrBIG, &conv, depth_units );
  
  fconv = conv;
  stroke_num = 0;
  NDim = 1;
  DList[0] = 1, DList[1] = 2;
  VList[0] = (VOIDPTR)&fconv;
  status = nl_convert_stroke_info(nlist, stroke_num, NL_STROKE_START_VALUE,
				  NDim, DList, VList);
  status = nl_convert_stroke_info(nlist, stroke_num, NL_STROKE_INCREMENT,
				  NDim, DList, VList);
  
  fxmin = fymin = fxmax = fymax = -4321.0;
  ixmin = iymin = ixmax = iymax = -4321;
  
  NDim = 2;
  
  Type[0] = (VOIDPTR)&xtype; Type[1] = (VOIDPTR)&ytype;
  status = nl_inq_nlist_info(nlist, NL_NLIST_DATA_TYPES, NDim, DList, Type);
  if (status IS_NOT_EQUAL_TO SUCCESS)
    return(status);
  
  if (xtype EQUALS INT_TYPE)
    {
      MinVal[0] = (VOIDPTR) &ixmin;
      MaxVal[0] = (VOIDPTR) &ixmax;
    }
  else
    {
      MinVal[0] = (VOIDPTR) &fxmin;
      MaxVal[0] = (VOIDPTR) &fxmax;
    }
  
  if (ytype EQUALS INT_TYPE)
    {
      MinVal[1] = (VOIDPTR) &iymin;
      MaxVal[1] = (VOIDPTR) &iymax;
    }
  else
    {
      MinVal[1] = (VOIDPTR) &fymin;
      MaxVal[1] = (VOIDPTR) &fymax;
    }
  
  status = nl_inq_nlist_info(nlist, NL_NLIST_MIN, NDim, DList, MinVal);
  if (status IS_NOT_EQUAL_TO SUCCESS)
    return(status);
  
  status = nl_inq_nlist_info(nlist, NL_NLIST_MAX, NDim, DList, MaxVal);
  if (status IS_NOT_EQUAL_TO SUCCESS)
    return(status);
  
  if (xtype EQUALS INT_TYPE)
    {
      log_trace->top  = (FLOAT) ixmin;
      log_trace->base = (FLOAT) ixmax;
    }
  else
    {
      log_trace->top  = fxmin;
      log_trace->base = fxmax;
    }
  
  if (ytype EQUALS INT_TYPE)
    {
      log_trace->min  = (FLOAT) iymin;
      log_trace->max =  (FLOAT) iymax;
    }
  else
    {
      log_trace->min  = fymin;
      log_trace->max =  fymax;
    }
  
  hoblockmove(ptrBIG->uwi, log_trace->uwi, sizeof(UWI));
  hoblockmove(trace_name, log_trace->trace_name, sizeof(LOG_TRACE_NAME));
  log_trace->dinc = ptrBIG->sample_incr * conv;
  hoblockmove(depth_units, log_trace->depth_uom, sizeof(LOG_UOM));
  ts_fill(log_trace->trace_uom, ' ', sizeof(LOG_UOM) - 1);
  log_trace->ptrace = nlist;
  
  return status;
}


/*********************************************************************/
/*                                                                   */
/* ROUTINE:  ldlgb_get_uom_conv                                      */
/*                                                                   */
/* PURPOSE:  to provide the conversion factor and units of measure   */
/*           for the depth domain of an output trace.                */
/*                                                                   */
/* INPUTS:   ptrBIG   pointer to the loader structure.               */
/*                                                                   */
/* OUTPUTS:  conv     conversion factor...input uom to stored uom.   */
/*           depth_units   Character string for db table.            */
/*                                                                   */
/* STATUS:   SUCCESS                                                 */
/*                                                                   */
/* METHOD:   1) Open PROJECT_DEFAULTS workspace & get default UOM    */
/*           2) Try to convert input uom to project uom.             */
/*           3) If INVALID or INCOMPATIBLE UOM, we know it is input  */
/*              uom since project uom was validated as loader started*/
/*              Therefore, conv = 1.0 and depth_units = input uom    */
/*           4) Else, conv has been set and depth_units = project.   */
/*                                                                   */
/*********************************************************************/
privatedef INT ldlgb_get_uom_conv(ptrBIG, conv, depth_uom)
     LD_LG_ASCII_STRUCT *ptrBIG;
     DOUBLE  *conv;
     LOG_UOM depth_uom;
{
  PROJECT_DEFAULT *proj;
  DOUBLE	    uconv;
  UOM_NAME        uom_project;
  UOM_NAME        uom_input;
  INT             status;
  
  status = am_open_workspace("PROJECT_DEFAULTS",AM_GLOBAL,(VOIDPTR *)&proj);
  
  strcpy( uom_input,   ptrBIG->depth_uom );
  strcpy( uom_project, proj->unit.vertical );
  
  status = tu_unit_conversion( 1.0, uom_input, &uconv, uom_project);
  
  if (status EQUALS SUCCESS)
    {
      *conv = (DOUBLE) uconv; /* cast to DOUBLE for ldlgb_get_uom_conv's caller */
      strcpy( (char *)depth_uom, (char *)uom_project );
    }
  else
    {
      *conv = 1.0;
      strcpy( (char *)depth_uom, (char *)uom_input );
    }
  
  return SUCCESS;
}

/***************************************************************************/
privatedef INT ldlgb_mk_trace_name(ptrBIG, column, trace_name)
     LD_LG_ASCII_STRUCT *ptrBIG;
     INT column;
     CHAR *trace_name;
{
  CHAR **traces = ptrBIG->trace_names;
  CHAR *uwi = ptrBIG->uwi;
  CHAR *source = ptrBIG->source;
  INT status = SUCCESS;
  
  /* create trace name */
  if ((uwi EQUALS (CHAR *) NULL) OR
      (traces EQUALS (CHAR **) NULL) OR
      (traces[column] EQUALS (CHAR *) NULL))
    {
      return FAIL;
    }

  sprintf(trace_name, "%s:%s", source,traces[column]);
  
  return status;
}

/***************************************************************************/
privatedef INT ldlgb_isfloat(x)
     CHAR *x;
{
  INT position = 1;
  INT xlen;
  BOOL exponent = FALSE;
  BOOL plus_minus = FALSE;
  BOOL decimal = FALSE;
  BOOL type = 0; /* 1 = digit, 2 = 'E|e', 3 = '.', 4 = '+|-' */
  CHAR *ps = x;
  
  /* strip all blanks from x */
  /*
    while (*x IS_NOT_EQUAL_TO NULL_CHAR)
    {
    if (*x IS_NOT_EQUAL_TO SPACE)
    *to++ = *x;
    
    x++;
    }
    *to = NULL_CHAR;
    */
  
  if (*ps >= '0' AND *ps <= '9')
    type = 1;
  else if (*ps EQUALS '.')
    type = 3;
  else if (*ps EQUALS '+' OR  *ps EQUALS '-')
    {
      type = 4;
      plus_minus = TRUE;
    }
  else
    return -1;
  
  for (ps++, position++; *ps; ps++, position++)
    {
      if (*ps >= '0' AND *ps <= '9')
	type = 1;
      else if (*ps EQUALS '.')
	{
	  if (decimal OR exponent)
	    return -position;
	  else
	    {
	      decimal = TRUE;
	      type = 3;
	    }
	}
      else if (*ps EQUALS 'E' OR *ps EQUALS 'e')
	{
	  if (exponent)
	    return -position;
	  if (*(ps+1) EQUALS '+' OR *(ps+1) EQUALS '-')
	    {
	      type = 4;
	      ps++;
	      position++;
	    }
	  else
	    type = 2;
	  
	  exponent = TRUE;
	}
      else
	return -position;
    }
  
  if (type EQUALS 3)
    {
      xlen = strlen(x);
      if (plus_minus EQUALS TRUE AND xlen > 2)
	;
      else if (plus_minus EQUALS FALSE AND xlen > 1)
	;
      else
	return -position;
    }
  else if (type IS_NOT_EQUAL_TO 1)
    return -position;
  
  return(position-1);
}

/***************************************************************************/
privatedef INT ldlgb_load_data_record(ptrBIG, number_errors)
     LD_LG_ASCII_STRUCT *ptrBIG;
     INT *number_errors;
{
  INT i, num_bogus, err_limit, depth, status;
  
  err_limit = ptrBIG->err_limit;
  depth  = ptrBIG->depth_column;
  
  /* calculate the needed number of bogus records to add */
  num_bogus = ldlgb_compute_fill_number(ptrBIG);
  
  if (num_bogus < 0)
    {
      /* depth read was not increasing */
      /* increment error counter and go and read another record */
      *number_errors = 1;
    }
  else if (num_bogus EQUALS 0)
    {
      *number_errors = 0;
      
      /* no need to add bogus, just add current record */
      status = ldlgb_add_to_nlist(ptrBIG, ptrBIG->ptrLOCAL->curr_record);
      if (status EQUALS SUCCESS)
	ptrBIG->ptrLOCAL->tot_count++;
    }
  else if (num_bogus > 0)
    {
      /* increment error counter */
      *number_errors = num_bogus;
      
      if (*number_errors > err_limit)
	status = LD_LG_ASCII_ERROR_LIMIT;
      
      /* first add all needed bogus records */
      for (i = 1; (status EQUALS SUCCESS) AND (i <= num_bogus); i++)
	{
	  /*increment bogus record depth */
	  ptrBIG->ptrLOCAL->bogus_record[depth] =
	    ptrBIG->ptrLOCAL->prev_record[depth] + ptrBIG->sample_incr;
	  
	  status = ldlgb_add_to_nlist(ptrBIG, ptrBIG->ptrLOCAL->bogus_record);
	  if (status EQUALS SUCCESS)
	    ptrBIG->ptrLOCAL->tot_count++;
	}
      
      /* then add current record */
      status = ldlgb_add_to_nlist(ptrBIG, ptrBIG->ptrLOCAL->curr_record);
      if (status EQUALS SUCCESS)
	ptrBIG->ptrLOCAL->tot_count++;
    }
  
  return( status );
}


/***************************************************************************/
privatedef INT ldlgb_print_msg(id,item,string)
     INT id,item;
     CHAR *string;
{
  FILE *fd;
  INT status = SUCCESS;
  static BOOL firsttime = TRUE;
  
  static struct MsgFileStruct {
    FILENAME	fname;
    INT ifbatch;
  } ldmsg;
  
  if (firsttime == TRUE)
    {
      firsttime = FALSE;
      qs_inq_i(QS_IF_BATCH, &ldmsg.ifbatch);
      
      /* setup logfile name */
      sprintf(ldmsg.fname,"sys$login:ldlog_%s.log",ho_date_tag(0)+6);
    }    
#if 0     
  if ((fd = ho_open (ldmsg.fname,"a+",NULL)) != (FILE *)0)
    {
      fprintf(fd,"\t\t******* Ascii Log Loader ********");
      fprintf(fd,"Started: %s\n",ldlgb_timestamp());
      fprintf(fd,"By %s.\tId: %d\n",ho_user_name(0),ho_user_number());
      if (fclose(fd))
	{
	  printf("%s\tNote: Can't close %s\n",theargv[0],ldmsg.fname); 
	}
      status = SUCCESS;
    }
  else
    {
      printf("%s\tError! Can't open Err/Msg file %s, Attention is required\n",
	     theargv[0],ldmsg.fname);
      printf("Aborting at %s.....\n",ldlgb_timestamp());
      status = FAIL;
    }
#endif
  
  if ( status == SUCCESS) 
    {	       
      switch (item)
	{
	case 0:
	case LD_LG_ASCII_RECORD_PROMPT:
	  printf("%s  %s\n",ldlgb_timestamp(),string);
	  
	  if (ldmsg.ifbatch AND ((fd = fopen (ldmsg.fname,"a+")) != (FILE *)0))
	    {
	      fprintf(fd,"%s  %s\n",ldlgb_timestamp(),string);
	      fclose(fd);
	    }
	  break;
	  
	case FAIL:	
	case LD_LG_ASCII_ERROR_PROMPT:
	  printf("Error: %s\n",string);
	  
	  if (ldmsg.ifbatch AND ((fd = fopen (ldmsg.fname,"a+")) != (FILE *)0))
	    {
	      fprintf(fd,"Error: %s\n",string);
	      fclose(fd);
	    }
	  break;
	  
	case 1:
	default:
	  printf("          %s\n",string);
	  
	  if (ldmsg.ifbatch AND ((fd = fopen (ldmsg.fname,"a+")) != (FILE *)0))
	    {
	      fprintf(fd,"          %s\n",string);
	      fclose(fd);
	    }
	  break;
	}
    }	
  return status;
}
