/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_RS.C */
/* *2     2-JAN-1991 14:31:15 KEE "(SPR -1) Add sun version" */
/* *1    13-NOV-1990 11:27:08 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_RS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_RS.C */
/* *3     9-NOV-1990 10:44:59 MING "change ho_find_files2 to ho_fin_filenames" */
/* *2     3-JUL-1990 16:42:45 KEE "(SPR -1) New test resource " */
/* *1     6-MAR-1990 08:29:53 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element TM_TEST_RS.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*************************************************************************

NAME:	TM_TEST_RESOURCE

DESCRIPTION:

    TM_TEST_RESOURCE is a Finder V5 application which allows the user to 
    display menus and dialog boxes.

    The user choose a resource file and the resoure item from the selectors.

    TM_TEST_RESOURCE then displays the entity.

    Pushing a button on a dialog box, or selecting a command from a menu,
    will cause the corresponding resource to disappear.

    Of course, quitting the TM_TEST_RESOURCE application will cause all of 
    the tested resources to disappear.

*************************************************************************/

#include "esi_c_lib.h"
#include "esi_ts.h"
#include "esi_tc.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_ho.h"
#include "esi_ho_files.h"
#include "esi_am_err.h"
#include "esi_tm_msg.h"
#include "esi_mg.h"

#include "tm_test_resource.rh"

#define MAX_RESOURCE_ITEMS 256

typedef struct{
	TCP  tcp_rs_files;   /* resource file name tcp */
	INT  cdlgid;           /* application dialog id */
	INT  resource_id_list[MAX_RESOURCE_ITEMS];  /* resource id array */
	INT  Dialog_Menu_type[MAX_RESOURCE_ITEMS];  /* resource dialog/menu type */
	} TM_TEST_RESOURCE_WS;

privatedef INT tm_load_allrs_files();
privatedef INT tm_load_rs_items();
privatedef INT tm_test_rs_server();
privatedef INT tm_test_dialog_server();
privatedef INT tm_test_menu_server();


/* ********************************************************************** */
publicdef INT tm_test_rs(pointer)
BYTE *pointer;
/* ********************************************************************** */
{
#define IDENT progname

  PROGNAME("tm_test_rs_main");

  INT status;
  TM_TEST_RESOURCE_WS *rs_ws;

  status = am_declare("Test Resource");
  status = am_open_workspace("TM_TEST_RESOURCE_WS",AM_APPLICATION,
			     (VOIDPTR *)&rs_ws);
  if (status == AM_ERR_NOT_DEFINED)
    {
      status = am_define_workspace("TM_TEST_RESOURCE_WS",AM_APPLICATION,
				   sizeof(TM_TEST_RESOURCE_WS),(VOIDPTR *)&rs_ws);
    }
  /* Define application control dialog */
  status = am_define_resource(AM_DIALOG,"TM_TEST_RESOURCE",TM_TEST_RESOURCE,
			      tm_test_rs_server,&rs_ws->cdlgid, rs_ws);
  status = wi_set_selection_limit(rs_ws->cdlgid, TR_LISTBOX1, 1);
  /* set single selection */
  status = wi_activate(rs_ws->cdlgid,TR_LISTBOX1,ON);
  /* activate event callback for resource file listbox */
  status = wi_set_selection_limit(rs_ws->cdlgid, TR_LISTBOX2, 1);
  status = load_allrs_files(rs_ws);
  /* load all resource files from current directory and esi$src directory */
  status = load_rs_items(rs_ws);
  /* load all items in resource file */
  status = wi_open_dialog(rs_ws->cdlgid);
  return(status);
}


/*************************************************************************

ROUTINE:    LOAD_ALLRS_FILES

DESCRIPTION:

    LOAD_ALLRS_FILES first load all the resource files in current directory
    and then load the ones in esi$src directory without duplicate.

*************************************************************************/

/************************************************************************/
privatedef INT load_allrs_files(rs_ws)
/************************************************************************/
TM_TEST_RESOURCE_WS *rs_ws;
{
  INT tcp_len1, tcp_len2;
  INT i, j, file_exist, status;
  CHAR *tempstr;
  TCP temp_tcp_files = (TCP) 0;
  FILENAME full_filename;
  
  /* get all resource files from current directory */
  status = ho_find_filenames("*.rs",&rs_ws->tcp_rs_files);
  tcp_len1 = ts_tcp_len(rs_ws->tcp_rs_files);

  /* get all resource files from esi$src directory */
#ifdef vms
  status = ho_find_filenames("esi$src:*.rs",&temp_tcp_files);
#endif
#if UNIX
  tempstr = getenv("FS");
  sprintf(full_filename,"%s/*.rs",tempstr);
  status = ho_find_filenames(full_filename,&temp_tcp_files);
#endif
  tcp_len2 = ts_tcp_len(temp_tcp_files);
  
  for (i=0; i<tcp_len1; i++)
    { /* extract the filename without extension */
      tempstr = strchr(rs_ws->tcp_rs_files[i],'.');
      *tempstr = 0;
    }
  
  for (i=0; i<tcp_len2; i++)
    {
      tempstr = strchr(temp_tcp_files[i],'.');
      *tempstr = 0;
      file_exist = FALSE;
      for (j=0; j<tcp_len1; j++)
	{
	  if (strcmp(rs_ws->tcp_rs_files[j],temp_tcp_files[i]) == 0)
	    {
	      file_exist = TRUE;
	      break;
	    }
	}
      if (file_exist == FALSE)
	{
	  ts_tcp_append(&rs_ws->tcp_rs_files,temp_tcp_files[i]);
	}
    }

  tempstr = ts_pack_strings(ts_tcp_len(rs_ws->tcp_rs_files),rs_ws->tcp_rs_files);
  wi_set_dialog_text(rs_ws->cdlgid,TR_LISTBOX1,tempstr);  
  /* put all file names in the resource file selector */
  wi_set_default_sel(rs_ws->cdlgid,TR_LISTBOX1,1);
  wi_set_selection_limit(rs_ws->cdlgid,TR_LISTBOX1,1);
  /* set default selection to be the first one */
  ts_tcp_free(temp_tcp_files);
  tc_free(tempstr);
  
  return(status);
}


/*************************************************************************

ROUTINE:    LOAD_RS_ITEMS

DESCRIPTION:

    LOAD_RS_ITEMS load all the resource items in the resource file.

*************************************************************************/

/************************************************************************/
privatedef INT load_rs_items(rs_ws)
TM_TEST_RESOURCE_WS *rs_ws;
{
  INT c, i = 0, file_index, dummy, status;
  INT Dialog_Or_Menu;
  BOOL loaded = FALSE;
  CHAR *tempstr;
  TCP  tcp_item = (TCP) 0;
  CHAR line[ST_MAC_MAX];
  FILENAME filename;
  FILE *fd;
  
  wi_query_selections(rs_ws->cdlgid, TR_LISTBOX1, &dummy, &file_index); 
  /* query resource file selector selection */
  sprintf(filename,"%s.rs",rs_ws->tcp_rs_files[file_index-1]); 

  if ((fd = ho_open(filename,"r",NULL)) == (FILE *) 0)
    {                /* search and open resource file in current directory
                        first, if not exist then open esi$src one */
#ifdef vms
      sprintf(filename,"esi$src:%s.rs",rs_ws->tcp_rs_files[file_index-1]);
#endif
#if UNIX
      tempstr = getenv("FS");
      sprintf(filename,"%s/%s.rs",tempstr,rs_ws->tcp_rs_files[file_index-1]);
#endif
      fd = ho_open(filename,"r",NULL);
    }

  if (fd == (FILE *) 0)
  {
    am_quit();
    printf("No rs files being found\n");
  }
  else 
  { 
  while (TRUE)
    {
      if ((c = getc(fd)) == EOF)
	{
	  break;
	}
      else 
	{
	  ungetc(c,fd);
	  fscanf(fd,"%[^\n]",line); /* get next line */
	  fscanf(fd,"%[\n]",filename);   /* get <cr> */
	  if (line[0] == '*' && line[1] == 'U')
	    {
	      if (loaded == TRUE)
		{
		  break;
		}
	      else
		{
		  loaded = TRUE;
		}
	    }
	  else if ((line[0] == '*' && line[1] == 'T') || (line[0] == ','))
	    {
	      if (line[0] == '*')
		{
		  Dialog_Or_Menu = (line[3] == 'M') ?
		    AM_MENU : AM_DIALOG;    /* check dialog or menu type */
		  fscanf(fd,"%[^\n]",line); /* get resource id line */
		  fscanf(fd,"%[\n]",filename);  
		}
	      tempstr = line + 1;
	      rs_ws->resource_id_list[i] = atoi(tempstr);
	      rs_ws->Dialog_Menu_type[i] = Dialog_Or_Menu;
	      fscanf(fd,"%[^\n]",line); /* get symbol name line */
	      fscanf(fd,"%[\n]",filename); 
	      tempstr = line + 3;
	      ts_tcp_append(&tcp_item,tempstr);
	      i++;
	    }
	}
    }

  if (i > 0)
    {
      tempstr = ts_pack_strings(i,tcp_item);
      wi_set_dialog_text(rs_ws->cdlgid,TR_LISTBOX2,tempstr);  
	  /* put all resource items name */
      wi_set_default_sel(rs_ws->cdlgid,TR_LISTBOX2,1);
      wi_set_selection_limit(rs_ws->cdlgid,TR_LISTBOX2,1);
      ts_tcp_free(tcp_item);
      tc_free(tempstr);
    }
  return(SUCCESS);
  } 
}
  

/*************************************************************************

ROUTINE:    TM_TEST_RS_SERVER

DESCRIPTION:

    TM_TEST_RS_SERVER is an application event server for the TM_TEST_RS
    application's control dialog.

    The function of the control dialog is to allow the user to choose
    resource files and id's for dialogs and menus which the user wishes
    to preview.

*************************************************************************/

/************************************************************************/
privatedef INT tm_test_rs_server(cdlgid, itemid, rs_ws)
INT cdlgid;
INT itemid;
TM_TEST_RESOURCE_WS *rs_ws;
{
  INT i, file_index, item_index, dummy, test_cdlgid, status = SUCCESS;
  INT (*test_server)();
  CHAR string[ST_COMMAND_MAX];
  
  switch (itemid)
    {
      case TR_LISTBOX1:   /* resource file being selected, load resource
			     items of that file */
        load_rs_items(rs_ws);
	break;
      case TR_DISPLAY_BTN:
	wi_query_selections(cdlgid, TR_LISTBOX1, &dummy, &file_index);
	wi_query_selections(cdlgid, TR_LISTBOX2, &dummy, &item_index);
	sprintf(string,"Resource File : %s\nResource Id : %3d",
		rs_ws->tcp_rs_files[file_index-1],
		rs_ws->resource_id_list[item_index-1]);
	/* get the resource file name, item id and print it on the dialog */
	wi_set_dialog_text(cdlgid, TR_STEXT, string);
	test_server = (rs_ws->Dialog_Menu_type[item_index-1] == AM_MENU) ?
	  tm_test_menu_server : tm_test_dialog_server;
	/* set up appropricate server */
	status = am_define_resource(rs_ws->Dialog_Menu_type[item_index-1], 
			   rs_ws->tcp_rs_files[file_index-1], 
			   rs_ws->resource_id_list[item_index-1], 
			   test_server, &test_cdlgid, NULL);
	/* define application control dialog/menu and present it */
	(rs_ws->Dialog_Menu_type[item_index-1] == AM_MENU) ?
	  wi_enable_menu(test_cdlgid,ON) : wi_open_dialog(test_cdlgid);
	break;
      case TR_QUIT_BTN:
	ts_tcp_free(rs_ws->tcp_rs_files);
	am_quit();
	break;
      }
  return (status);
}	


/*************************************************************************

ROUTINE:    TM_TEST_RS_DIALOG_SERVER

DESCRIPTION:

    TM_TEST_RS_DIALOG_SERVER is an event server which is called whenever
    an event is generated on a dialog which the user is previewing.

    Generating such an event causes the dialog to be removed.

*************************************************************************/

/************************************************************************/
privatedef INT tm_test_dialog_server(cdlgid, itemid, pointer)
/************************************************************************/
INT cdlgid;
INT itemid;
BYTE *pointer;
{
  CHAR string[ST_COMMAND_MAX];
  INT status;

/************************************************************************/

				/*  Form and display message to describe
				    event. */
  sprintf(string,mg(TM_TEST_RS_DIALOG_EVENT),itemid,cdlgid);
  am_message(AM_STATUS,string);
				/*  Remove the dialog. */
  am_release_resource(cdlgid);
  return(SUCCESS);
}


/*************************************************************************

ROUTINE:    TM_TEST_RS_MENU_SERVER

DESCRIPTION:

    TM_TEST_RS_MENU_SERVER is an event server which is called whenever
    an event is generated on a menu which the user is previewing.

    Generating such an event causes the menu to be removed.

*************************************************************************/

/************************************************************************/
privatedef INT tm_test_menu_server(cdlgid, itemid, pointer)
/************************************************************************/
INT cdlgid;
INT itemid;
BYTE *pointer;
{
  CHAR string[ST_COMMAND_MAX];
  INT  status;

/************************************************************************/

				/*  Form and display message to describe
				    event. */
  sprintf(string,mg(TM_TEST_RS_MENU_EVENT),itemid,cdlgid);
  am_message(AM_STATUS,string);
				/*  Remove the menu. */
  status = am_release_resource(cdlgid);
  return(status);
}
