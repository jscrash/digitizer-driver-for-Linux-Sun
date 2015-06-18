/* DEC/CMS REPLACEMENT HISTORY, Element LD_LG_ASCII.C*/
/* *9    12-AUG-1991 14:33:31 JTM "(SPR 0) Modernized, ANSIfied (somewhat), and fixed a bug or two."*/
/* *8    29-JUL-1991 15:15:46 JTM "(SPR 0) Unocal changes. we_verify_well_server called directly."*/
/* *7    31-JUL-1990 17:32:28 GILLESPIE "(SPR 1) Modify include file structure"*/
/* *6    12-FEB-1990 08:06:31 WALTERS "(SPR 0) Change we_ routines to static"*/
/* *5     5-SEP-1989 15:59:06 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *4    11-AUG-1989 10:48:48 JULIAN "(SPR -1) redeclaration of ld_lg_ascii_load_requested "*/
/* *3     9-AUG-1989 11:09:14 JULIAN "(SPR -1) remove redefine LOGNULL"*/
/* *2    26-JUN-1989 10:46:42 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:02:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LG_ASCII.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


/* LD_LG_ASCII.CC, DOUG>LOADERS>ASCII>SOURCE, CANFIELD & PUMROY, 11/17/86
   Loads ESI defined ASCII log data files.
   Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA. */

/* TITLE : LD_LG_ASCII - Loads ESI defined ASCII log data files. */

/* START-HISTORY :
   11/17/86 CHARLIE  cleaned up comments
   11/15/86 DOUG     Initial checkin.
   11/15/86 D.J. CANFIELD & C.E. Template constructed
   END-HISTORY */

/* START-CODE : */
#ifndef ESI_C_LIB_H
# include "esi_c_lib.h"
#endif
#ifndef ESI_AM_H
# include "esi_am.h"
#endif
#ifndef ESI_HO_FILES_H
# include "esi_ho_files.h"
#endif
#ifndef ESI_MG_H
# include "esi_mg.h"
#endif
#ifndef ESI_WI_H
# include "esi_wi.h"
#endif
#ifndef ESI_TS_H
# include "esi_ts.h"
#endif
#ifndef ESI_TU_H
# include "esi_tu.h"
#endif
#ifndef ESI_WE_VFDEF_H
# include "esi_we_vfdef.h"
#endif
#ifndef ESI_LD_ASDEF_H
# include "esi_ld_asdef.h"
#endif
#ifndef ESI_GL_PROJECT_H
# include "esi_gl_project.h"
#endif
#ifndef LD_LG_ASCII_RH
# include "ld_lg_ascii.rh"
#endif
#ifndef WE_VFY_WL_RH
# include "we_vfy_wl.rh"
#endif
#ifndef ESI_STDIO_H
# include "esi_stdio.h"
#endif
#ifndef ESI_TU_ERR_H
# include "esi_tu_err.h"
#endif
#ifndef ESI_LD_ERR_H
# include "esi_ld_err.h"
#endif
#ifndef ESI_LD_MSG_H
# include "esi_ld_msg.h"
#endif
#ifndef ESI_STRING_H
# include "esi_string.h"
#endif

#if USE_PROTOTYPES

static INT ld_lg_ascii_file_cdlg_init(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_file_cdlg_server(INT id, INT item, LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_file_cdlg_operate(INT id, LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_hdr_cdlg_init(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_hdr_cdlg1_server(INT id, INT item, LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_hdr_cdlg2_server(INT id, INT item, LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_hdr_cdlg_operate(LD_LG_ASCII_STRUCT *pstruct);
static TCP ld_lg_ascii_tcp_traces(CHAR buffer[]);
static INT ld_lg_ascii_hdr_cdlg_term(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_message_init(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_message_term(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_begin_well(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_load_requested(LD_LG_ASCII_STRUCT *pstruct);
static INT ld_lg_ascii_verify_well_switch(VERIFY_WELL_STRUCT *pvws);
static INT ld_lg_ascii_begin_data(LD_LG_ASCII_STRUCT *pstruct);
static INT we_verify_well_term(INT id);
static INT we_verify_wells_cdlg_server(INT id,INT item,
		    VERIFY_WELL_STRUCT *pverify_well_struct);
static INT we_verify_well_cdlg_init(VERIFY_WELL_STRUCT *pverify_well_struct);
static INT we_verify_well_server(VERIFY_WELL_STRUCT *pverify_well_struct);

#else

static INT ld_lg_ascii_file_cdlg_init();
static INT ld_lg_ascii_file_cdlg_server();
static INT ld_lg_ascii_file_cdlg_operate();
static INT ld_lg_ascii_hdr_cdlg_init();
static INT ld_lg_ascii_hdr_cdlg1_server();
static INT ld_lg_ascii_hdr_cdlg2_server();
static INT ld_lg_ascii_hdr_cdlg_operate();
static TCP ld_lg_ascii_tcp_traces();
static INT ld_lg_ascii_hdr_cdlg_term();
static INT ld_lg_ascii_message_init();
static INT ld_lg_ascii_message_term();
static INT ld_lg_ascii_begin_well();
static INT ld_lg_ascii_load_requested();
static INT ld_lg_ascii_verify_well_switch();
static INT ld_lg_ascii_begin_data();
static INT we_verify_well_term();
static INT we_verify_wells_cdlg_server();
static INT we_verify_well_cdlg_init();
publicdef INT we_verify_well_server();

#endif

static FILENAME LD_LG_ASCII_CDLGS_FILE = "LD_LG_ASCII";
static FILENAME last_file_used = "";

#define WE_VERIFY_WELL_CDLG_FILE "we_vfy_wl"

#define MAX_DEPTH_TRACE (80)

/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_MAIN                                       */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        This is the entry point for the ASCII log data file    */
/*                 loader.  It defines the workspaces and starts the      */
/*                 first server.                                          */
/*                                                                        */
/* Parameters:     junk - an unused pointer.                              */
/*                                                                        */
/* Status Codes:   The application manager status codes are returned.     */
/*                                                                        */
/* ********************************************************************** */

publicdef INT ld_lg_ascii(junk)

BYTE *junk;

    {
    INT status;
    LD_LG_ASCII_STRUCT *pstruct;
    VERIFY_WELL_STRUCT *pvws;
    PROJECT_DEFAULT    *proj;
    CHAR                unit_of_measure[31];
    UOM_STRUCTURE       uom_data;

    status = am_declare("ASCII");

                                  /* Open the PROJECT DEFAULTS workspace  */
                                  /* and insure that the project has been */
                                  /* initialized with a valid vertical    */
                                  /* unit of measure.                     */

    status = am_open_workspace("PROJECT_DEFAULTS",AM_GLOBAL,(VOIDPTR *)&proj);
    (void) strcpy(unit_of_measure,proj->unit.vertical);
    status = tu_verify_uom(unit_of_measure,&uom_data);

    if (status == TU_INVALID_UOM)  /* Alert and cause an abort!            */
       {
        status = am_message(AM_ALERT,mg_message(LD_LG_ASCII_BAD_DEFAULT_UNITS));
        status = FAIL;
       }
    else                           /* Allocate the application structure.  */
       {
        status = am_define_workspace( ASCII_WORKSPACE,
                                      AM_APPLICATION,
                                      sizeof (LD_LG_ASCII_STRUCT),
                                      (VOIDPTR *) &pstruct);

                                  /* Initialize the application structure */
                                  /* These values should come from the DB */

        pstruct->uwi[0]       = '\0';
        pstruct->format       = (CHAR *) am_allocate(AM_APPLICATION, (size_t)8);
                                (void) strcpy( pstruct->format, "8F10.4");
        pstruct->trace_names  = (TCP) 0;
	pstruct->depth_trace  = (CHAR *) am_allocate(AM_APPLICATION, (size_t)MAX_DEPTH_TRACE);
        pstruct->delimit      = (CHAR *) am_allocate(AM_APPLICATION, (size_t)8);
                                (void) strcpy( pstruct->delimit, "<--->");
        pstruct->err_limit    = 10;
        pstruct->depth_column = 0;
        pstruct->sample_incr  = 0.5;
        pstruct->null_value   = LOGNULL;
	(void) strcpy( pstruct->source, "ASCII" );
        pstruct->units_button = LD_LG_ASCII_1OF2_FEET;


        if (status == SUCCESS)
             status = am_define_workspace( VERIFY_WORKSPACE,
                                           AM_APPLICATION,
                                           sizeof (VERIFY_WELL_STRUCT),
                                           (VOIDPTR *) &pvws);

        if (status == SUCCESS)
             status = ld_lg_ascii_file_cdlg_init(pstruct);
       }

    if (status < SUCCESS )          /* Unsuccessful startup...ABORT!   */
       am_quit();

    return SUCCESS;

    }

/*

*/
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


static INT ld_lg_ascii_begin_well( pstruct )

    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT status;

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

    switch (status) {

         case HEADER_BLOCK_FOUND:

              status = ld_lg_ascii_read_headers( pstruct );
              if (status IS_NOT_EQUAL_TO SUCCESS)
                   status = am_message(AM_ALERT,mg_message(LD_LG_ASCII_BAD_HDR));
              status = ld_lg_ascii_hdr_cdlg_init( pstruct );
              break;

         case DATA_BLOCK_FOUND:

              (void) strcpy(pstruct->uwi, "");
              status = ld_lg_ascii_hdr_cdlg_init( pstruct );
              break;

         case END_OF_FILE_BLOCK_FOUND:

              (void) fclose( pstruct->pf );
              status = ld_lg_ascii_file_cdlg_init( pstruct );
              break;
         }
    return SUCCESS;
    }
/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_LOAD_REQUESTED                             */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        This routine is called when the server verifying that  */
/*                 file header records are correct terminates.  It        */
/*                 examines the status code and branches accordingly.     */
/*                                                                        */
/* Parameters:     pstruct - pointer to the application data structure.   */
/*                                                                        */
/* Status Codes:   SUCCESS                                                */
/*                                                                        */
/* ********************************************************************** */

static INT ld_lg_ascii_load_requested( pstruct )

    LD_LG_ASCII_STRUCT *pstruct;

    {
    VERIFY_WELL_STRUCT *pvws;
    INT status;

                                       /* All is well.  Load the data.    */

                                       /* But first, verify that number   */
                                       /* of fields in the format match   */
                                       /* the number of trace names.      */

    status = ld_lg_ascii_verify_fmt( pstruct );

    switch (status) {
         case SUCCESS:
                                       /* Format is ok so verify that the */
                                       /* correct well exists in the db.  */

              status = am_open_workspace( VERIFY_WORKSPACE,
                                          AM_APPLICATION,
                                          (VOIDPTR *)&pvws);
              if (status == SUCCESS)
                   {
                   (void) strcpy(pvws->uwi,pstruct->uwi);
                   pvws->show_well = TRUE;
                   pvws->well_exists = FALSE;
 /*                  status = am_start_server(we_verify_well_server,
                                      ld_lg_ascii_verify_well_switch,pvws);
 */
                     status = we_verify_well_server(pvws);
                   }
              break;

         case FORMAT_BAD:
                                            /* Format was not acceptable. */
                                            /* Put up CDLG & try again.   */

             status = am_message(AM_ALERT,
                                 mg_message(LD_LG_ASCII_FMT_BAD));
             status = ld_lg_ascii_hdr_cdlg_init( pstruct );
             break;

         case NOT_ENOUGH_TRACE_NAMES:
                                            /* Format was not acceptable. */
                                            /* Put up CDLG & try again.   */

             status = am_message(AM_ALERT,mg_message(LD_LG_ASCII_FMT_UNMATCHED));
             status = ld_lg_ascii_hdr_cdlg_init( pstruct );
             break;
         }
    return SUCCESS;
    }
/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_VERIFY_WELL_SWITCH                         */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        This routine is called when the server verifying that  */
/*                 the correct well exists in the database quits.  It     */
/*                 examines the status code and performs the appropriate  */
/*                 processing...                                          */
/*                   1) LOAD the data and check for success.              */
/*                   2) MODIFY the header data (UWI?!).                   */
/*                                                                        */
/* Parameters:     pvws - (input) pointer to the verify_well_structure    */
/*                                                                        */
/* Status Codes:   SUCCESS                                                */
/*                                                                        */
/* ********************************************************************** */

static INT ld_lg_ascii_verify_well_switch( pvws )

VERIFY_WELL_STRUCT *pvws;

    {
    INT status;
    LD_LG_ASCII_STRUCT *pstruct;
    CHAR message[132];

    status = am_open_workspace( ASCII_WORKSPACE,
                                AM_APPLICATION,
                                (VOIDPTR *)&pstruct );

    if (pvws->well_exists == TRUE )
         {
                                  /* This is the correct well.  Put up    */
                                  /* the message dialog box and load the  */
                                  /* data.                                */
         status = ld_lg_ascii_begin_data( pstruct );

         switch (status){

              case DATA_BLOCK_NOT_FOUND:
                   am_message(AM_ALERT,mg_message(LD_LG_ASCII_SEQUENCE_ERROR));
                   status = ld_lg_ascii_begin_well(pstruct);   /* Try This well */
                   break;

              case END_OF_FILE_BLOCK_FOUND:
                   am_message(AM_ALERT,mg_message(LD_LG_ASCII_UNEXPECTED_EOF));
                   (void) fclose( pstruct->pf );
                   status = ld_lg_ascii_file_cdlg_init( pstruct );
                   break;

              case DATA_BLOCK_FOUND:
                   status = ld_lg_ascii_message_init( pstruct );
                   status = ld_lg_ascii_load_data( pstruct );    /* LOAD! */
                   switch (status) {
                        case LD_LG_ASCII_ERROR_LIMIT:
                                            /* Error limit caused abort.  */
                                            /* Alert the operator.        */
                             (void) sprintf(message, "%s --- %s", pstruct->uwi,
                                      mg_message(LD_LG_ASCII_ERROR_LIMIT));
                             status = am_message(AM_ALERT,message);
                        break;

                        case SUCCESS:            /* Successfully loaded!       */
                             (void) sprintf(message, "%s --- %s", pstruct->uwi,
                                      mg_message(LD_LG_ASCII_DATA_LOADED));
                             status = am_message(AM_STATUS,message);
                        break;
			   default:
                             status = am_message(AM_ALERT,"Some errors encountered");
                             (void) sprintf(message, "%s --- %s", pstruct->uwi,
                                      mg_message(LD_LG_ASCII_DATA_LOADED));
                             status = am_message(AM_STATUS,message);
                        break;
                   }
                   status = ld_lg_ascii_message_term( pstruct ); /* Clear Msg  */
                   status = ld_lg_ascii_begin_well( pstruct );   /* Next Well  */
                   break;
              }
          }
    else
         {
                                  /* Either this is the wrong well, or    */
                                  /* it does not exist in the database.   */

         status = am_message(AM_ALERT,mg_message(LD_LG_ASCII_CHANGE_UWI));

         status = ld_lg_ascii_hdr_cdlg_init( pstruct );
         }

    return SUCCESS;
    }
/*

*/
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

static INT ld_lg_ascii_begin_data(pstruct)

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

/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_FILE_CDLG_INIT                             */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        To display the dialog box prompting for the file name  */
/*                 where ASCII log data are stored.                       */
/*                                                                        */
/* Parameters:     pstruct - pointer to application data structure.  It   */
/*                           is passed in to be given to the define_rsrc  */
/*                           routine.  When events are generated by the   */
/*                           dialog box, it is passed to the cdlg server. */
/*                                                                        */
/* Status Codes:   SUCCESS if the dialog box was displayed on the MAC     */
/*                 FAIL if the dialog box was not displayed on the MAC    */
/*                                                                        */
/* ********************************************************************** */

static INT ld_lg_ascii_file_cdlg_init( pstruct )

    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT status;
    INT id;
 
    status = am_define_resource( AM_DIALOG,
                                 LD_LG_ASCII_CDLGS_FILE,
                                 LD_LG_ASCII_CDLG_1,
                                 ld_lg_ascii_file_cdlg_server,
                                 &id,
                                 (VOIDPTR) pstruct );

    status = wi_set_dialog_text(id, LD_LG_ASCII_F_SOURCE, pstruct->source);
    status = wi_set_dialog_text(id, LD_LG_ASCII_FILENAME_FIELD, last_file_used);
    status = wi_open_dialog(id);
    status = wi_activate(id,LD_LG_ASCII_FILENAME_FIELD,1);

    return status;
    }
/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_FILE_CDLG_SERVER                           */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        To service events from the dialog box prompting for    */
/*                 the file name where ASCII log data are stored.         */
/*                                                                        */
/* Parameters:     id - (input) dialog box id.                            */
/*                 item - (input) event id.                               */
/*                 pstruct - (output) receives status code and is passed  */
/*                           to the operate routine.                      */
/*                                                                        */
/* Status Codes:   (pstruct->status) QUIT_REQUESTED if item = abort.      */
/*                                   undefined if item = proceed.         */
/*                                                                        */
/* ********************************************************************** */


static INT ld_lg_ascii_file_cdlg_server( id, item, pstruct )

    INT id;
    INT item;
    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT status;

    switch (item) {

         case LD_LG_ASCII_CDLG_1_PROCEED:
              status = ld_lg_ascii_file_cdlg_operate(id,pstruct);
              if (status == SUCCESS)
                   {
                   status = am_release_resource( id );
                   status = ld_lg_ascii_init_parser(pstruct);
                   status = ld_lg_ascii_begin_well(pstruct);
                   }
              else;
              break;

         case LD_LG_ASCII_CDLG_1_ABORT:
              pstruct->status = QUIT_REQUESTED;
              status = am_release_resource( id );
              status = am_quit();
              break;

         default:
              break;
         }
 
    return SUCCESS;
    }
/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_FILE_CDLG_OPERATE                          */
/*                                                                        */
/* Author:         Douglas J. Canfield                                    */
/*                                                                        */
/* Purpose:        To read the file name from the custom dialog box field */
/*                 and open the file for input.                           */
/*                                                                        */
/* Parameters:     id (input) resource id of the custom dialog box.       */
/*                 pstruct (output) application data structure.  It is    */
/*                      used here to receive the file pointer and the     */
/*                      server status code (pstruct->status) for          */
/*                      subsequent routines.                              */
/*                                                                        */
/* Status Codes:   (pstruct->status)  SUCCESS                             */
/*                                    ASCII_FILE_NOT_FOUND                */
/*                                                                        */
/* ********************************************************************** */

static INT ld_lg_ascii_file_cdlg_operate( id, pstruct )

    INT id;                       /* id is the cdlg id number.            */
    LD_LG_ASCII_STRUCT *pstruct;  /* pstruct points to the ap. data struct*/

    {
    INT status;
    INT unused;
    CHAR text[132];
    CHAR *ptext;

    wiqccdi( id, LD_LG_ASCII_F_SOURCE, &unused, text ); /* Read SOURCE    */
    ts_sto_upper( text );
    if (strchr(text,':') == NULL AND 
        strchr(text,';') == NULL AND
        strchr(text,'.') == NULL AND
        strchr(text,',') == NULL)
     {}
     else                      
       {                                           /* Disallow punctuation.  */
       am_message(AM_ALERT,mg_message(LD_LG_ASCII_NO_PUNCT));
       ts_convert_char(text, ':', ' ');        /* ':'--->' '     */
       ts_convert_char(text, '.', ' ');        /* '.'--->' '     */
       ts_convert_char(text, ';', ' ');        /* ';'--->' '     */
       ts_convert_char(text, ',', ' ');        /* ','--->' '     */
       status = wi_set_dialog_text(id, LD_LG_ASCII_F_SOURCE, text);    /* Change CDLG    */
       status = wi_activate(id,LD_LG_ASCII_F_SOURCE,1);
       return FAIL;
       }              

    ptext = ts_snowhite(text);
    if ( IS_EMPTY_STRING(ptext) )     /* Disallow NULL source. */
      {
      am_message(AM_ALERT,mg_message(LD_LG_ASCII_NULL_SOURCE));
      status = wi_set_dialog_text(id, LD_LG_ASCII_F_SOURCE, "ASCII");
      status = wi_activate(id,LD_LG_ASCII_F_SOURCE,1);
      return FAIL;
      }

    ts_convert_char(text, ' ', '_');    /* ' '--->'_'         */
    (void) strcpy(pstruct->source, ptext);                 /* Accept the source. */

                                  /* Read the filename from the CDLG.     */
    wiqccdi( id, LD_LG_ASCII_FILENAME_FIELD, &unused, last_file_used);

                                  /*  Convert any \n to a blank.          */
    /*
    ts_convert_char(last_file_used, '\n', ' ');
    */
    ptext = ts_snowhite( last_file_used );

                                   /*  Attempt to open the file.           */
    pstruct->pf = ho_open( ptext, "r", NULL );
    if ( pstruct->pf == NULL_FILE_PTR )
         {
         am_message( AM_ALERT, mg_message( LD_LG_ASCII_FILE_NOT_FOUND ));
         status = wi_set_dialog_text(id,LD_LG_ASCII_FILENAME_FIELD, ptext);
         status = wi_activate(id,LD_LG_ASCII_FILENAME_FIELD,1);
         status = ASCII_FILE_NOT_FOUND;
         }
    else
         status = SUCCESS;

    return status;
    }
/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_CDLG_INIT                                     */
/*                                                                           */
/* Author:         Douglas J. Canfield and Charles E. Pumroy                 */
/*                                                                           */
/* Purpose:        Put up dialog box allowing user to override any params    */
/*                 found in the file, or to enter the loading parameters.    */
/*                                                                           */
/* Parameters:     pstruct - the application data structure. It is used      */
/*                 here to display the data found during the load header     */
/*                 process.                                                  */
/*                                                                           */
/* Status Codes:   unused on return                                                          */
/*                                                                           */
/* ************************************************************************* */



static INT ld_lg_ascii_hdr_cdlg_init( pstruct )

    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT status;
    INT id;
    CHAR text[128];
    CHAR buffer[4096];
    CHAR *bp;
    INT nstrings;

    bp = buffer;

                                /*define the dialog for the second screen -
                                      this one has the buttons on it */

    status = am_define_resource( AM_DIALOG,
                                 LD_LG_ASCII_CDLGS_FILE,
                                 LD_LG_ASCII_HEADER_2OF2,
                                 ld_lg_ascii_hdr_cdlg2_server,
                                 &pstruct->hdr2_id,
                                 (VOIDPTR) pstruct);

    id = pstruct->hdr2_id;

    if ( IS_STRING(pstruct->format) )
         wi_set_dialog_text(id, LD_LG_ASCII_2OF2_FORMAT, pstruct->format);

                             /* If the trace name table is not empty,     */
                             /* then make one string of all entries.      */

    if (pstruct->trace_names IS_NOT_EQUAL_TO (TCP)0)
    {
         nstrings = ts_tcp_len( pstruct->trace_names);
         bp = ts_pack_strings( nstrings, pstruct->trace_names);
         ts_convert_char(bp, '\n', ',');
     /*    len = strlen( bp ) - 1;             Clip the trailing comma.   */
     /*    bp[len] = '\0';                                                */
         wi_set_dialog_text(id, LD_LG_ASCII_2OF2_TRACE_NAMES, bp);

       if ( IS_EMPTY_STRING(pstruct->depth_trace) )
       {
          (void) strncpy( pstruct->depth_trace, pstruct->trace_names[0], MAX_DEPTH_TRACE); 
       }

    }
    wi_set_dialog_text( id, LD_LG_ASCII_2OF2_DEPTH_NAME, pstruct->depth_trace);

    status = wi_open_dialog(id);          /* open the dialog */

              /*define the front screen - the one with the load parameters */

    status = am_define_resource( AM_DIALOG,
                                 LD_LG_ASCII_CDLGS_FILE,
                                 LD_LG_ASCII_HEADER_1OF2,
                                 ld_lg_ascii_hdr_cdlg1_server,
                                 &pstruct->hdr1_id,
                                 (VOIDPTR) pstruct);

    id = pstruct->hdr1_id;

                                  /* Use previously used depth units.     */
    wiscdv(id, pstruct->units_button, 1);

    wi_set_dialog_text(id, LD_LG_ASCII_1OF2_UWI, pstruct->uwi);
    (void) sprintf(text,"%10.3f",pstruct->null_value);
    wi_set_dialog_text(id, LD_LG_ASCII_1OF2_NULL, text);
    (void) sprintf(text,"%d",pstruct->err_limit);
    wi_set_dialog_text(id, LD_LG_ASCII_1OF2_MAX_ERR, text);
    (void) sprintf(text,"%9.4f",pstruct->sample_incr);
    wi_set_dialog_text(id, LD_LG_ASCII_1OF2_SAMPLE_INCR, text);
/*                                                    DELETED FROM RESOURCE
    wi_set_dialog_text(id, LD_LG_ASCII_1OF2_DELIMITER, pstruct->delimit);       */  

    status = wi_open_dialog(id);          /* open the dialog */
    wi_activate(id, LD_LG_ASCII_1OF2_UWI, 1);

    return status;
    }

/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_HDR_CDLG1_SERVER                              */
/*                                                                           */
/* Author:         Douglas J. Canfield and Charles E. Pumroy                 */
/*                                                                           */
/* Purpose:        Trap events on front screen and install units of measure  */
/*                 into the application data structure.                      */
/*                                                                           */
/* Parameters:     id - unused                                               */
/*                 item - used as case switch for log units of measure       */
/*                 pstruct - the application data structure                  */
/*                                                                           */
/* Status Codes:   SUCCESS                                                   */
/*                                                                           */
/* ************************************************************************* */



static INT ld_lg_ascii_hdr_cdlg1_server( id, item, pstruct )

    INT id;
    INT item;
    LD_LG_ASCII_STRUCT *pstruct;

    {

    pstruct->units_button = item;

    return SUCCESS;
    }
/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_HDR_CDLG2_SERVER                              */
/*                                                                           */
/* Author:         Douglas J. Canfield and Charles E. Pumroy                 */
/*                                                                           */
/* Purpose:        accept events from the second screen (load or abort)      */
/*                                                                           */
/* Parameters:     id - unused                                               */
/*                 item - case switch for the load or abort buttons          */
/*                 pstruct - the application data structure                  */
/*                                                                           */
/* Status Codes:   SUCCESS                                                   */
/*                                                                           */
/* ************************************************************************* */



static INT ld_lg_ascii_hdr_cdlg2_server( id, item, pstruct )

    INT id;
    INT item;
    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT status;

    switch (item) {

         case LD_LG_ASCII_2OF2_LOAD:
              status = ld_lg_ascii_hdr_cdlg_operate( pstruct );
              if (status == SUCCESS)
                   {
                   status = ld_lg_ascii_hdr_cdlg_term( pstruct );
                   status = ld_lg_ascii_load_requested( pstruct );
                   }
              break;

         case LD_LG_ASCII_2OF2_ABORT:
              status = ld_lg_ascii_hdr_cdlg_term( pstruct );
              (void) fclose( pstruct->pf );
              status = ld_lg_ascii_file_cdlg_init( pstruct );
              break;

         default:
              break;

         }

    return SUCCESS;
    }

/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_HDR_CDLG_OPERATE                              */
/*                                                                           */
/* Author:         Douglas J. Canfield and Charles E. Pumroy                 */
/*                                                                           */
/* Purpose:        To accept user parameters from the dialog with error      */
/*                 checking                                                  */
/*                                                                           */
/* Parameters:     pstruct - the application data structure                  */
/*                                                                           */
/* Status Codes:   SUCCESS                                                   */
/*                 FAIL                                                      */
/*                                                                           */
/* ************************************************************************* */



static INT ld_lg_ascii_hdr_cdlg_operate( pstruct )

    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT id1, id2;                 /* Dialog ID's.                         */
    INT unused;
    INT status;
    INT i;                        /* Index used in for loop.              */
    INT last_str;                 /* Index of last string in TCP.         */
    CHAR text[128];
    CHAR *ptext;
    CHAR buffer[4096];

    id1 = pstruct->hdr1_id;
    id2 = pstruct->hdr2_id;
    status = SUCCESS;

                                  /* Store UWI in structure */
    wiqccdi(id1, LD_LG_ASCII_1OF2_UWI, &unused, pstruct->uwi);

                                  /* Store null value as float */
    wiqccdi(id1, LD_LG_ASCII_1OF2_NULL, &unused, text);
    ptext = ts_snowhite(text);
    if (ts_isfloat(ptext) > 0)
        (void) sscanf(ptext, "%f", &pstruct->null_value);
    else {
          am_message( AM_ALERT, mg_message(LD_LG_ASCII_INVALID_NULL));
          status = FAIL;
         }

                                  /* Store error cutoff as integer */
   wiqccdi(id1, LD_LG_ASCII_1OF2_MAX_ERR ,&unused, text);
   ptext = ts_snowhite(text);
   if (ts_isfloat(ptext) > 0)
         (void) sscanf(ptext, "%d", &pstruct->err_limit);
   else {
         am_message(AM_ALERT, mg_message(LD_LG_ASCII_INVALID_MAX_ERROR));
         status = FAIL;
        }

                               /*  Fill in the units field from button value */
    switch (pstruct->units_button) {

         case LD_LG_ASCII_1OF2_FEET:
              (void) strcpy(pstruct->depth_uom, "FEET");
              break;

         case LD_LG_ASCII_1OF2_METERS:
              (void) strcpy(pstruct->depth_uom, "METERS");
              break;

         case LD_LG_ASCII_1OF2_INCHES:
              (void) strcpy(pstruct->depth_uom, "INCHES");
              break;

         case LD_LG_ASCII_1OF2_CENTIMETERS:
              (void) strcpy(pstruct->depth_uom, "CENTIMETERS");
              break;

         case LD_LG_ASCII_1OF2_SECONDS:
              (void) strcpy(pstruct->depth_uom, "SECONDS");
              break;

         case LD_LG_ASCII_1OF2_MILLISECONDS:
              (void) strcpy(pstruct->depth_uom, "MILLISECONDS");
              break;

         case LD_LG_ASCII_1OF2_OTHER:
              (void) strcpy(pstruct->depth_uom, "");
              break;

         default:
              (void) strcpy(pstruct->depth_uom, "");
              break;

         }

                                  /* Store the sample increment as float */
   wiqccdi(id1, LD_LG_ASCII_1OF2_SAMPLE_INCR, &unused, text);
   ptext = ts_snowhite(text);
   if (ts_isfloat(ptext) > 0)
         (void) sscanf(ptext, "%f", &pstruct->sample_incr);
   else {
         am_message(AM_ALERT, mg_message(LD_LG_ASCII_SAMPLE_INCR));
         status = FAIL;
        }

                          /* Store the delimiter text --- DELETED FROM RESOURCE
   wiqccdi(id1,LD_LG_ASCII_1OF2_DELIMITER, &unused, text);
   if (ts_isfloat(text) IS_NOT_EQUAL_TO 0)
        {
         pstruct->delimit = (CHAR *) am_allocate(AM_APPLICATION, strlen(text)+1);
         (void) strcpy(pstruct->delimit, text);
        }
*/
                                  /* Store the format to read the data with */
   wiqccdi(id2, LD_LG_ASCII_2OF2_FORMAT, &unused, text);
   ptext = ts_snowhite( text );
   if ( IS_EMPTY_STRING( ptext ) )
         {
         pstruct->format = (CHAR *) am_allocate(AM_APPLICATION, (size_t)2 );
         pstruct->format[0] = '\0';
         am_message(AM_ALERT,mg_message(LD_LG_ASCII_NO_FORMAT));
         status = FAIL;
         }
   else  {
         pstruct->format = (CHAR *) am_allocate(AM_APPLICATION, 
				                strlen(ptext)+1);
         (void) strcpy(pstruct->format,ptext);
         }

                                  /* Store the depth name identifying the
                                     trace name to be used as depth */
   wiqccdi(id2, LD_LG_ASCII_2OF2_DEPTH_NAME, &unused, text);
   ts_noblanks(text);
   if ( IS_STRING(text) )
         {
         ts_sto_upper(text);
         (void) strncpy( pstruct->depth_trace, text, MAX_DEPTH_TRACE );
         }

                                  /* Store the users trace names entered */
    wiqccdi(id2, LD_LG_ASCII_2OF2_TRACE_NAMES,&unused, buffer);
    if (ts_isfloat(buffer) EQUALS 0)
         {
         am_message(AM_ALERT,mg_message(LD_LG_ASCII_NO_TRACE_NAMES));
         status = FAIL;
         }
    else {                        /* Free and rebuild trace_name TCP.     */
         ts_tcp_free(pstruct->trace_names);
         pstruct->trace_names = ld_lg_ascii_tcp_traces( buffer );
                                  /* Identify which field is depth.       */
         last_str = ts_tcp_len( pstruct->trace_names ) - 1;
                                  /* identify for parser which item is the
                                     depth trace */
         pstruct->depth_column = -1;
         for (i=0; i<= last_str; i++)
              if ( strcmp(pstruct->depth_trace, pstruct->trace_names[i])
                  == SUCCESS )
                   {
                   pstruct->depth_column = i;
                   break;
                   }
                                  /* if we didnt get a match, then try FAIL */
         if (pstruct->depth_column < 0)
              {
              am_message(AM_ALERT,mg_message(
                                  LD_LG_ASCII_INVALID_DEPTH_TRACE));
              status = FAIL;
              }
         }
    return status;
    }

/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_TCP_TRACES                                    */
/*                                                                           */
/* Author:         Douglas J. Canfield and Charles E. Pumroy                 */
/*                                                                           */
/* Purpose:        Rebuild the table of character pointers after the trace   */
/*                 names are retrieved from the dialog menu                  */
/*                                                                           */
/* Parameters:     buffer - the long string of data which must be put back   */
/*                 into a table of character pointers in pstruct, the        */
/*                 application data structure.
                                                                             */
/* Status Codes:                                                             */
/*                                                                           */
/* ************************************************************************* */


static TCP ld_lg_ascii_tcp_traces( buffer )

    CHAR buffer[];
    {
    INT len;
    INT n_str;
    INT i;
    TCP tcp;
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
/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_HDR_CDLG_TERM                                 */
/*                                                                           */
/* Author:         Douglas J. Canfield and Charles E. Pumroy                 */
/*                                                                           */
/* Purpose:        Release the resources for the verify parameters dialogs   */
/*                                                                           */
/* Parameters:     pstruct - the application data structure                  */
/*                                                                           */
/* Status Codes:   status - the result of the resource release               */
/*                                                                           */
/* ************************************************************************* */



static INT ld_lg_ascii_hdr_cdlg_term( pstruct)

    LD_LG_ASCII_STRUCT *pstruct;

    {
    INT status;

    status = am_release_resource( pstruct->hdr1_id);
    status = am_release_resource( pstruct->hdr2_id);

    return status;
    }

/*

*/
/* ********************************************************************** */
/*                                                                        */
/* Routine:        LD_LG_ASCII_MESSAGE_INIT                               */
/*                                                                        */
/* Author:         Doug Canfield                                          */
/*                                                                        */
/* Purpose:        To display a dialog box informing the operator that    */
/*                 the log data are being loaded.                         */
/*                                                                        */
/* Parameters:     id - (output) the id of the opened resource.           */
/*                                                                        */
/* Status Codes:   SUCCESS                                                */
/*                                                                        */
/* ********************************************************************** */

static INT ld_lg_ascii_message_init( pstruct )

LD_LG_ASCII_STRUCT *pstruct;

    {
    INT id;

    am_define_resource( AM_DIALOG,
                        LD_LG_ASCII_CDLGS_FILE,
                        LD_LG_ASCII_MESSAGE_DIALOG,
                        NULL_FUNCTION_PTR,
                        &id,
                        (VOIDPTR) NULL);

    wi_open_dialog(id);
    pstruct->message_id = id;

    return SUCCESS;
    }
/*

*/
/* ************************************************************************* */
/*                                                                           */
/* Routine:        LD_LG_ASCII_MESSAGE_TERM                                  */
/*                                                                           */
/* Author:         Douglas J. Canfield                                       */
/*                                                                           */
/* Purpose:        Close the message dialog and return SUCCESS               */
/*                                                                           */
/* Parameters:     id - dialog to be closed                                  */
/*                                                                           */
/* Status Codes:   SUCCESS                                                   */
/*                                                                           */
/* ************************************************************************* */


static INT ld_lg_ascii_message_term( pstruct )

LD_LG_ASCII_STRUCT *pstruct;

    {
    am_release_resource(pstruct->message_id);
    return SUCCESS;
    }

/* END-CODE */

/* ********************************************************************** */
/*                                                                        */
/*  ROUTINE NAME:  WE_VERIFY_WELL_SERVER                                  */
/*                                                                        */
/*  AUTHOR:        Doug Canfield                                          */
/*                                                                        */
/*  PURPOSE:       To verify that a UWI exists in the WELLS db table.     */
/*                                                                        */
/*  CALLI[NG:       status = am_start_server( we_verify_well_server,       */
/*                                           NEXT_ROUTINE,                */
/*                                           pverify_well_structure);     */
/*                                                                        */
/*                 NEXT_ROUTINE is the module to call upon termination    */
/*                 of the we_verify_well.                                 */
/*                                                                        */
/*                 Verify_well_structure has three data items...          */
/*                                                                        */
/*                   uwi:         The uwi of the well to be tested.       */
/*                   show_well:   A flag set to TRUE if a dialog is to    */
/*                               be presented to the operator requesting */
/*                                acceptance of the well.                 */
/*                   well_exists: Set to TRUE if the correct well exists. */
/*                                                                        */
/* ********************************************************************** */


/* ********************************************************************** */
/*                                                                        */
/* Start of code...Entry point!                                           */
/*                                                                        */
/* ********************************************************************** */

static INT we_verify_well_server(pverify_well_struct)

    VERIFY_WELL_STRUCT *pverify_well_struct;

    {
    INT status;
    WELL_STRUCT *pwell;

    status = we_get_well_header(pverify_well_struct->uwi, WELL_HDR_1_DETAIL,
				&pwell);

                                       /*  If the well does not exist, set*/
                                       /*  well_exists to FALSE and return*/

    if (status IS_NOT_EQUAL_TO SUCCESS)
         {
         pverify_well_struct->well_exists = FALSE;
    /*     status = am_quit(); */
	  status = ld_lg_ascii_verify_well_switch( pverify_well_struct );
         }
                                       /*  If the well exists, check to   */
                                       /*  see of the program wants the   */
                                       /*  operator to verify that it is  */
                                       /*  indeed the correct well.       */
    else
    { 
        if ( pverify_well_struct->show_well IS_NOT_EQUAL_TO TRUE)
        {
              pverify_well_struct->well_exists = TRUE;    /* Don't verify */
              status = am_quit();
         }
         else                          /*  Verify!     */
	 {
            status = we_verify_well_cdlg_init(pverify_well_struct); 
         }
    }

    return SUCCESS;
    }

/* ********************************************************************** */
/*                                                                        */
/*  WE_VERIFY_WELL_CDLG_INIT                                              */
/*  Put up the custom dialog box to display infomration from the well     */
/*  retrieved by we_get_well_header.                                      */
/*                                                                        */
/* ********************************************************************** */

static INT we_verify_well_cdlg_init(pverify_well_struct)

    VERIFY_WELL_STRUCT *pverify_well_struct;

    {
    INT status;
    INT id;
    CHAR text[16];
    WELL_STRUCT *pwell;
    WELL_HDR_1 *pwell1;

    status = we_get_well_header(pverify_well_struct->uwi, WELL_HDR_1_DETAIL,
				&pwell);

    pwell1 = (WELL_HDR_1 *)pwell -> detail.ptr;
    am_define_resource( AM_DIALOG,
                        WE_VERIFY_WELL_CDLG_FILE,
                        WE_VERIFY_WELL_SELECTION,
                        we_verify_wells_cdlg_server,
                        &id,
                        (VOIDPTR) pverify_well_struct);

                                            /* Load the text fields.      */

    status = wiscdt(id, WE_VERIFY_UWI_FIELD,  pwell->uwi);
    status = wiscdt(id, WE_VERIFY_OPER_FIELD, pwell1->operator);
    status = wiscdt(id, WE_VERIFY_NAME_FIELD, pwell1->name);
    status = wiscdt(id, WE_VERIFY_NUMB_FIELD, pwell1->number);
    (void) sprintf( text, "%8.2f", pwell1->elevation.value );
    (void) sprintf( text, "%s  %s", text,  pwell1->elevation.ref);
    status = wiscdt(id, WE_VERIFY_ELEV_FIELD, text);

                                            /* Make the CDLG visible.     */

    status = wi_open_dialog(id);

    return status;
    }

/* ********************************************************************** */
/*                                                                        */
/*  WE_VERIFY_WELLS_CDLG_SERVER                                           */
/*                                                                        */
/*  Get events from the custom dialog box and invoke the                  */
/*  appropriate functions.                                                */
/*                                                                        */
/* ********************************************************************** */

static INT we_verify_wells_cdlg_server(id,item,pverify_well_struct)

    INT  id;
    INT  item;
    VERIFY_WELL_STRUCT *pverify_well_struct;

    {
    INT status;
    LD_LG_ASCII_STRUCT *pstruct;

    status = SUCCESS;

    status = am_open_workspace( ASCII_WORKSPACE,
                                AM_APPLICATION,
                                (VOIDPTR *)&pstruct );


    switch (item) {                         /* Dispatch event to process. */
         case WE_VERIFY_ACCEPT:
              pverify_well_struct->well_exists = TRUE;
              status = we_verify_well_term(id);
              status = ld_lg_ascii_verify_well_switch(pverify_well_struct);
              break;
         case WE_VERIFY_REJECT:
              pverify_well_struct->well_exists = FALSE;
              status = we_verify_well_term(id);
	      status = ld_lg_ascii_hdr_cdlg_init( pstruct ); 
              break;
         default:
              break;
         }
    return status;
    }

/* ********************************************************************** */
/*                                                                        */
/*  WE_VERIFY_WELL_TERM                                                   */
/*                                                                        */
/*  Close the custom dialog box and quit the application server.          */
/*                                                                        */
/* ********************************************************************** */

static INT we_verify_well_term(id)

    INT id;
    {
    INT status;

    status = am_release_resource(id);
/*    status = am_quit(); */
    return  status;
    }
