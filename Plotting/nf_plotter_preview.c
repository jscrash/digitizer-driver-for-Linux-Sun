/* DEC/CMS REPLACEMENT HISTORY, Element NF_PLOTTER_PREVIEW.C */
/* *1    13-NOV-1990 11:26:10 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element NF_PLOTTER_PREVIEW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NF_PLOTTER_PREVIEW.C */
/* *2     5-JUL-1990 16:07:37 PURNA "(SPR 0) new element" */
/* *1     5-JUL-1990 11:57:51 PURNA "plotter preview resource file" */
/* DEC/CMS REPLACEMENT HISTORY, Element NF_PLOTTER_PREVIEW.C */
#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_HO_H
#include "esi_ho.h"
#endif
#ifndef ESI_MATH_H
#include "esi_math.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_LU_H
#include "esi_lu.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

typedef struct
    {
    INT nf_trans;           /* Plotter Preview Information dialog */
	FILENAME input_file;
	BOOL color_display;
    INT msg_cdlg_id;          /* Message Dialog server id */
    } NF_STRUCT;

typedef struct 
    {
    INT version;
    DOUBLE x_size,y_size;
    }NF_NL_USER_STRUCT;

#define UP 3
#define DOWN 2

#define ON 0
#define OFF 1

#define NF_WORKSPACE "PlotterPreview_Workspace"
    
#define FILE_LEN 80

/* Resource files */
#ifndef NF_CDLGS_RH
#define NF_CDLGS_RSRC "NF_CDLGS"
#include "nf_cdlgs.rh"
#endif

/* LOCAL FUNCTIONS DEFINED HERE */

#if USE_PROTOTYPES
privatedef INT nfz_dialog_server(INT id,INT item,NF_STRUCT *pex);
privatedef INT nfz_init (NF_STRUCT *pex);
privatedef INT nfz_get_extends(CHAR *filename,DOUBLE *x_size,DOUBLE *y_size);
privatedef INT nfz_load_nlist(FILE *in_file,NLIST_HEADER *nlist,
	   INT *end_of_file,INT *color);
privatedef INT nfz_msg_server(INT id,INT item,NF_STRUCT *pex);
privatedef INT nfz_to_screen(CHAR *filename,BOOL color_display);
#else
privatedef INT nfz_dialog_server();
privatedef INT nfz_init ();
privatedef INT nfz_get_extends();
privatedef INT nfz_load_nlist();
privatedef INT nfz_msg_server();
privatedef INT nfz_to_screen();
#endif

/**************************************************************************/
/*                                                                        */
/*  NF_MAIN_SERVER							  */
/*  Main entry to the CopyProject System.                                 */
/*  Calls the system initialization routines & checks for success.        */
/*                                                                        */
/*  Returns:  SUCCESS                                                     */
/*                                                                        */
/**************************************************************************/
publicdef INT nf_main_server(ptr)
BYTE *ptr;
    {
    
    INT status, tag_status;
    NF_STRUCT *pex;

    tag_status = am_declare("CopyProject");

                                   /*  Try to open the global workspace.  */
                                   /*  If we are successful, continue.    */
                                   /*  If not, allocate & initialize it.  */
    status = am_open_workspace(NF_WORKSPACE,AM_GLOBAL,(VOIDPTR *)&pex);
    if (status != SUCCESS)
      {                            /*  and initialize it                  */
      am_define_workspace(NF_WORKSPACE,AM_GLOBAL,
  			  sizeof(NF_STRUCT),(VOIDPTR *)&pex);
      }
    status = nfz_init(pex);

    return SUCCESS;
   
    }

/**************************************************************************/
/*                                                                        */
/*  NFZ_INIT                                                               */
/*                                                                        */
/*  Open the menus and the message custom dialog.                         */
/*                                                                        */
/*  Returns:  SUCCESS, FAIL                                               */
/*                                                                        */
/**************************************************************************/
privatedef INT nfz_init(pex)
NF_STRUCT *pex;

    {
    INT   status=SUCCESS;
    INT	  nsources=0,ntargets=0;
    INT	  default_source_index=0,default_target_index=0;

                                   /* allocate dialog dialog        */
    status = am_define_resource(AM_DIALOG, NF_CDLGS_RSRC, NF_DIALOG,
	     nfz_dialog_server, &pex->nf_trans, pex);
	    
                                   /* open dialog dialog            */

    /*------------------------------------------------------------------*/
    /* Allocate memory for this invocation, which is freed on return    */
    /*------------------------------------------------------------------*/
    pex->color_display=FALSE;
    wiscdv(pex->nf_trans,NF_COLOR_DISPLAY,0);
    wi_set_dialog_text(pex->nf_trans,NF_DIALOG_ERR_MSG,
	"Enter NPF input filename");
    wi_open_dialog(pex->nf_trans);
    return status;

    }


/**************************************************************************/
/*                                                                        */
/*  NFZ_DIALOG_SERVER							  */
/*                                                                        */
/*  The dialog server - Different file and help options			  */
/*                                                                        */
/*  Returns:  SUCCESS                                                     */
/*                                                                        */
/**************************************************************************/
privatedef INT nfz_dialog_server(id, item, pex)
INT id;
INT item;
NF_STRUCT *pex;
    {
    INT	    unused;
    INT status;
    FILENAME full_filename;
    FILE *file_ptr;

    switch (item)
        {
        case NF_DIALOG_DONE   :
	    wi_query_dialog_item(id,NF_DIALOG_E_FILENAME, &unused, full_filename);
	    sscanf(full_filename,"%s",pex->input_file);
	    if(strlen(pex->input_file) >= FILE_LEN)
            {
                wi_set_dialog_text(id,NF_DIALOG_ERR_MSG,
                "Input File Name is too long");
                wi_set_dialog_text(id,NF_DIALOG_E_FILENAME," ");
		break;
            }
	    if(strlen(pex->input_file) <= 0)
            {
                wi_set_dialog_text(id,NF_DIALOG_ERR_MSG,
                "Please enter Input filename");
                break;
            }		    
	    if((file_ptr = ho_open(pex->input_file,"r",NULL)) EQUALS (FILE *)0)
	    {
		wi_set_dialog_text(id,NF_DIALOG_ERR_MSG,
		    "Can't find input file ");
		break;
	    }

	    strcpy(pex->input_file,full_filename);
	    status = am_define_resource(AM_DIALOG, NF_CDLGS_RSRC, NF_MSG,
                                         nfz_msg_server, &pex->msg_cdlg_id, pex);

	    wi_set_dialog_text(pex->msg_cdlg_id,NF_MSG_STAT,
		    "Start plotting preview");
	    wi_open_dialog(pex->msg_cdlg_id);
	    break;
                                   /* cLOSE THE DIALOG BEFORE QUIT*/
	case NF_COLOR_DISPLAY :
	    if (pex->color_display)
		{
		pex->color_display=FALSE;
		wiscdv(pex->nf_trans,NF_COLOR_DISPLAY,0);
		}
	    else
		{
		pex->color_display=TRUE;
		wiscdv(pex->nf_trans,NF_COLOR_DISPLAY,1);
		}
	    break;
        case NF_DIALOG_CANCEL :
	    am_quit();
            break;
        }

    return status;

    }

/**************************************************************************/
/*                                                                        */
/*  NFZ_MSG_SERVER	                                                  */
/*                                                                        */
/*  The msg server - Different file and help options			  */
/*                                                                        */
/*  Returns:  SUCCESS                                                     */
/*                                                                        */
/**************************************************************************/
privatedef INT nfz_msg_server(id, item, pex)
INT id;
INT item;
NF_STRUCT *pex;
{
    INT status;

    switch (item)
    {
	case NF_MSG_CANCEL :
	    wi_close_dialog(id);
            break;
	case NF_MSG_OK :
	    status = nfz_to_screen(pex->input_file,pex->color_display);
	    if(status != SUCCESS)
		wi_set_dialog_text(id,NF_MSG_STAT,"Exit failure - Try again");
	    wi_close_dialog(id);
	    break;
    }
}


/*  
    function	NFZ_TO_SCREEN
    input	filename -  neutral file in full pathname
    description plot neutral file to screen
*/

privatedef INT nfz_to_screen(filename,color_display)
CHAR *filename;
BOOL color_display;
{
    INT status;
    NLIST_HEADER nlist=(NLIST_HEADER )0;
    DOUBLE x_size,y_size,x_temp,y_temp;
    size_t n_bytes;
    INT dummy,window,segment;
    GKS_REAL ndcMinX,ndcMinY,ndcMaxX,ndcMaxY,aspectRatio;
    GKS_REAL ndcDX,ndcDY;
    INT prev_color,color,i,eof=FALSE,nrows;
    FILE *in_file=(FILE *)0;

			     /*	find map corners for display		    */
    status = nfz_get_extends(filename,&x_size,&y_size);
    if(status != SUCCESS) return status;
    
			     /*	open neutral file, ready to read from this  */
			     /*	file					    */
    in_file = ho_open(filename,"r",NULL);
    if (in_file EQUALS (FILE *)0)
	{
	am_message(AM_ALERT,"Error opening input file");
	status = FAIL;
	goto error;
	}

			     /*	skip comments and all unneeded information  */
    for(i=0;i<4;i++)
	{
	while ( (dummy = fgetc(in_file)) != '\n'  AND  dummy != EOF);
	if (dummy EQUALS EOF)
	    {
            am_message(AM_ALERT,"Input file contain unexpected end of file");
            goto error;
            }
        }	

			     /*	set aspect ratio of xsize & ysize, set
			     	display window size according to the aspect
			     	ratio					    */
    aspectRatio=y_size/x_size;

    wm_inq_wkwn(&ndcMinX,&ndcMaxX,&ndcMinY,&ndcMaxY);
    ndcDX = ndcMaxX - ndcMinX;
    ndcDY = ndcMaxY - ndcMinY;
    if (aspectRatio != (ndcDY/ndcDX))
	{
	if (aspectRatio > (ndcDY/ndcDX))
	    ndcMaxX = ndcMinX + (ndcDY / aspectRatio);
	else
	    ndcMaxY = ndcMinY + (ndcDX * aspectRatio);
	}

			     /*	set background color			    */
    lu_text_to_index("COLOR",&color,"BACKGROUND");

			     /*	create gks window			    */
    window=0;
    wmwallocate(&window,ndcMinX,ndcMaxX,ndcMinY,ndcMaxY,color);
    wmtallocate(&window,0.0,1.0,0.0,1.0,0.0,x_size,0.0,y_size);
    wmsallocate(&segment);

			     /*	set color to normal for monochrome display  */
    lu_text_to_index("COLOR",&color,"NORMAL");
    wm_gsplci(color);	

    while(!eof)
	{
			     /*	get nlist for every color changes	    */
	status = nfz_load_nlist(in_file,&nlist,&eof,&color);
	if(status != SUCCESS) return status;

			     /*	display nlist on screen			    */
	status = nl_inq_nlist_int (nlist, NL_NLIST_NROWS, &nrows);
	if(nrows > 0)
	    {
	    status=nl_draw_nlist(nlist,1,2);
	    if(status != SUCCESS)
		{
		am_message(AM_ALERT,"Error in drawing nlist");
		wmwclose(window);
		goto error;
		}
	    }
			     /*	set color for color display only	    */
	if(color_display)
	    wm_gsplci(color);	
			     /*	free nlist				    */
	nl_free_nlist(nlist);
	}
			     /*	close session				    */
    fclose(in_file);
    wmsclose(segment);
    wmwclose(window);

    return SUCCESS;
error:

    if(nlist != (NLIST_HEADER)0)
	nl_free_nlist(nlist);
    return status;
}

privatedef INT nfz_get_extends(filename,x_out,y_out)
CHAR *filename;
DOUBLE *x_out,*y_out;
{
    INT i,status;
    DOUBLE x,y;
    DOUBLE xmin,xmax,ymin,ymax;    
    FILE *in_file=(FILE *)0;
    INT first=TRUE;
    INT dummy,pen;

    in_file = ho_open(filename,"r",NULL);
    if (in_file EQUALS (FILE *)0)
	{
	am_message(AM_ALERT,"Error opening input file");
	return FAIL;
	}

    for(i=0;i<4;i++)
	{
	while ( (dummy = fgetc(in_file)) != '\n'  AND  dummy != EOF);
	if (dummy EQUALS EOF)
	    {
            am_message(AM_ALERT,"Input file contain unexpected end of file");
	    return FAIL;
            }
        }	
    while (fscanf(in_file,"%f %f %d\n",&x,&y,&pen) != EOF)
	{
	switch (pen)
	    {
	    case DOWN:
		if(first)
		    {
		    xmin=x; ymin=y;
		    xmax=x; ymax=y;
		    first=FALSE;
		    }
		 else
		    {
		    if(x > xmax) xmax = x;
		    if(x < xmin) xmin = x;		
		    if(y > ymax) ymax = y;
		    if(y < ymin) ymin = y;		
		    }
		break;
	    }
	}
    *x_out=xmax;
    *y_out=ymax;
    fclose(in_file);
    return SUCCESS;
}


privatedef INT nfz_load_nlist(in_file,nlist,end_of_file,color)
FILE *in_file;
NLIST_HEADER *nlist;
INT *end_of_file;
INT *color;
{
    INT status;
    DOUBLE x,y;
    INT pen;
    UINT table_number;
    INT i,dummy;

    privatedef UINT  ndim        = 2;
    privatedef INT  units[2]     = { 0,             0               };
    privatedef INT  dim_type[2] = { FLOAT_TYPE,    FLOAT_TYPE      };
    privatedef size_t  dim_size[2] = {sizeof(DOUBLE), sizeof(DOUBLE)  };
    privatedef INT  value_inc[2] = { VALUE_LIST,    VALUE_LIST      };
    privatedef UINT dim_list[2] = { 1,2 };
    privatedef VOIDPTR value_list[2];    

    status = nl_init_nlist (nlist, ndim, units, dim_size, dim_type, 
	value_inc, 500);
    value_list[0] = (VOIDPTR)&x;
    value_list[1] = (VOIDPTR)&y;

    while (fscanf(in_file,"%f %f %d\n",&x,&y,&pen) != EOF)
	{
	switch (pen)
	    {
	    case UP:
		status = nl_start_table(*nlist,&table_number);
		if(status != SUCCESS) goto error;
		break;
	    case DOWN:
		status = nl_add_points(*nlist,1,ndim,dim_list,value_list);
		if(status != SUCCESS) goto error;
		break;
	    default:
		if(pen >= 1000)
		    *color = pen - 1000;
		return SUCCESS;
	    }	
	}
	
	*end_of_file=TRUE;
	return SUCCESS;	
	
error:
    if(*nlist != (NLIST_HEADER)0)
	nl_free_nlist(*nlist);
    return status;
}
