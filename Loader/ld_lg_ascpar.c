/* DEC/CMS REPLACEMENT HISTORY, Element LD_LG_ASCPAR.C*/
/* *11   12-AUG-1991 14:36:39 JTM "(SPR 0) Modernized and ANSIfied (somewhat)."*/
/* *10   29-JUL-1991 15:18:19 JTM "(SPR 0) Unocal changes. tmp_status and error messages added."*/
/* *9    27-MAR-1991 13:34:39 KEE "(SPR -1) change INCREMENT_LIST, to INCREMENT_LIST;"*/
/* *8    23-AUG-1990 17:56:36 JESSIE "(SPR 1) fix include files"*/
/* *7    14-AUG-1990 13:31:51 VINCE "(SPR 5644) Header Standardization"*/
/* *6     7-MAR-1990 17:23:23 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *5    17-JAN-1990 15:32:23 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *4     5-SEP-1989 16:01:57 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *3    11-AUG-1989 10:54:05 JULIAN "(SPR -1) 'i' maybe used before set"*/
/* *2    26-JUN-1989 10:47:03 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:03:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_LG_ASCPAR.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_c_lib.h"
#include "esi_ctype.h"

#ifndef  ESI_LD_ASDEF_H

#include "esi_ld_asdef.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_STRING_H
#include "esi_string.h"
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

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#ifndef ESI_LD_ERR_H

#include "esi_ld_err.h"

#endif

#ifndef ESI_LD_MSG_H

#include "esi_ld_msg.h"

#endif

#include "ld_lg_ascii.rh"

#define XALLOCATE(dtype) ((dtype *) am_allocate(AM_APPLICATION,\
(size_t)sizeof (dtype)))
        
#define XSTRING_ALLOCATE(stype) ((CHAR *) am_allocate(AM_APPLICATION, strlen(stype) + 1))

#define xtc_zalloc(x)           am_allocate(AM_APPLICATION, (size_t)(x))
#define xfree(x)                am_free(x)
#define YSTRING_ALLOCATE(stype) ((CHAR *) tc_zalloc(strlen(stype) + 1))
#define ytc_zalloc(x)           tc_zalloc((size_t)(x))
#define yfree(x)                tc_free(x)

/* GLOBAL DEFINES */

#define PARM_SIZE                  50
#define NULL_STRING                ""
#define NULL_CHAR                  '\0'
#define LEFT_PAREN                 '('
#define RIGHT_PAREN                ')'
#define SPACE                      ' '
#define COMMA                      ','
#define ZERO                       '0'
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
#define END_OF_FILE_TOKEN          7381
#define COMMA_TOKEN                7382
#define RIGHT_PAREN_TOKEN          7383
#define LEFT_PAREN_TOKEN           7384
#define STRING_TOKEN               7385

#if USE_PROTOTYPES
static INT add_to_nlist (LD_LG_ASCII_STRUCT *ptrBIG, FLOAT *record);
static INT determine_buffer_type (LD_LG_ASCII_STRUCT *ptrBIG);
static INT fill_buffer (LD_LG_ASCII_STRUCT *ptrBIG);
static INT get_parameter_string (LD_LG_ASCII_STRUCT *ptrBIG, CHAR *keyword,
                                     CHAR *string);
static INT get_next_token (LD_LG_ASCII_STRUCT *ptrBIG, CHAR *token);
static INT strip_all_blanks (CHAR *string);
static INT string_to_parms (CHAR *string, TCP *out_parameter);
static INT get_data_records (LD_LG_ASCII_STRUCT *ptrBIG);
static INT save_data (LD_LG_ASCII_STRUCT *ptrBIG);
static INT mk_lt_struct (LD_LG_ASCII_STRUCT *ptrBIG, LOG_TRACE_STRUCT *log_trace,
                             NLIST_HEADER nlist, CHAR *trace_name);
static INT mk_trace_name (LD_LG_ASCII_STRUCT *ptrBIG, INT column,
                              CHAR *trace_name);
static INT convert_format_parameter (CHAR *parameter, FS *format_structure);
static INT format_parameters_to_structures (TCP str_parms, FS ***exp_fs);
static INT expand_format_structure (FS **from, FS ***ptr_to);
static INT isfloat (CHAR *x);
static INT get_uom_conv (LD_LG_ASCII_STRUCT *ptrBIG, DOUBLE *conv,
                             LOG_UOM depth_uom);
static INT load_data_record (LD_LG_ASCII_STRUCT *ptrBIG, INT *number_errors);
static INT init_load_data (LD_LG_ASCII_STRUCT *ptrBIG);
static INT cleanup_log_data (LD_LG_ASCII_STRUCT *ptrBIG);
static INT create_a_stroke (LD_LG_ASCII_STRUCT *ptrBIG);
static INT compute_fill_number (LD_LG_ASCII_STRUCT *ptrBIG);

#else
static INT add_to_nlist ();
static INT determine_buffer_type ();
static INT fill_buffer ();
static INT get_parameter_string ();
static INT get_next_token ();
static INT strip_all_blanks ();
static INT string_to_parms ();
static INT get_data_records ();
static INT save_data ();
static INT mk_lt_struct ();
static INT mk_trace_name ();
static INT convert_format_parameter ();
static INT format_parameters_to_structures ();
static INT expand_format_structure ();
static INT isfloat ();
static INT get_uom_conv ();
static INT load_data_record ();
static INT init_load_data ();
static INT cleanup_log_data ();
static INT create_a_stroke ();
static INT compute_fill_number ();

#endif
static CHAR msgbuf[1024];
static UINT Dim_List[2] = 
    {
    1,
    2
    };



/* File Description ---------------------------------------------------------
Overview:
    Parses and converts ESI ASCII log data files.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ld_lg_ascii_init_parser(LD_LG_ASCII_STRUCT *ptrBIG);
    
    publicdef INT ld_lg_ascii_get_block_type(LD_LG_ASCII_STRUCT *ptrBIG);
    
    publicdef INT ld_lg_ascii_skip_block(LD_LG_ASCII_STRUCT *ptrBIG);

    publicdef INT ld_lg_ascii_load_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
    publicdef INT ld_lg_ascii_read_headers(LD_LG_ASCII_STRUCT *ptrBIG);
    
    publicdef INT ld_lg_ascii_verify_fmt(LD_LG_ASCII_STRUCT *ptrBIG);
Private_Functions:
    static INT   add_to_nlist(LD_LG_ASCII_STRUCT *ptrBIG, FLOAT *record);
    
    static INT   determine_buffer_type(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   fill_buffer(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   get_parameter_string(LD_LG_ASCII_STRUCT *ptrBIG, CHAR *keyword,
        CHAR *string);
    
    static INT   get_next_token(LD_LG_ASCII_STRUCT *ptrBIG, CHAR *token);
    
    static INT   strip_all_blanks(CHAR *string);
    
    static INT   string_to_parms(CHAR *string, TCP *out_parameter);
    
    static INT   get_data_records(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   save_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   mk_lt_struct(LD_LG_ASCII_STRUCT *ptrBIG, LOG_TRACE_STRUCT *log_trace,
        NLIST_HEADER nlist, CHAR *trace_name);
    
    static INT   mk_trace_name(LD_LG_ASCII_STRUCT *ptrBIG, INT column, CHAR *trace_name);
    
    static INT   convert_format_parameter(CHAR *parameter, FS *format_structure);
    
    static INT   format_parameters_to_structures(TCP str_parms, FS ***exp_fs);
    
    static INT   expand_format_structure(FS **from, FS ***ptr_to);
    
    static INT   isfloat(CHAR *x);
    
    static INT   get_uom_conv(LD_LG_ASCII_STRUCT *ptrBIG, DOUBLE *conv,
        LOG_UOM depth_uom);
    
    static INT   load_data_record(LD_LG_ASCII_STRUCT *ptrBIG,
        INT *number_errors);
    
    static INT   init_load_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   cleanup_log_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   create_a_stroke(LD_LG_ASCII_STRUCT *ptrBIG);
    
    static INT   compute_fill_number(LD_LG_ASCII_STRUCT *ptrBIG);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ld_lg_ascii_init_parser(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) prtBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_lg_ascii_init_parser (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
publicdef INT ld_lg_ascii_init_parser (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    ptrBIG->ptrLOCAL = XALLOCATE (PLS);
    
    ptrBIG->ptrLOCAL->format_structures = (FS **)NULL;
    ptrBIG->ptrLOCAL->number_all_formats = 0;
    ptrBIG->ptrLOCAL->number_ns_formats = 0;
    ptrBIG->ptrLOCAL->data_records = (DS **)NULL;
    
    ptrBIG->ptrLOCAL->eof_flag = FALSE;
    
    /* fill buffer */
    (void) strcpy (ptrBIG->ptrLOCAL->buffer, NULL_STRING);
    ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
    fill_buffer (ptrBIG);

    return( SUCCESS );

    }

/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ld_lg_ascii_get_block_type(LD_LG_ASCII_STRUCT *ptrBIG);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    HEADER_BLOCK_FOUND - If header record is found.
    DELIMITER_BLOCK_FOUND - If delimiter record is found.
    DATA_BLOCK_FOUND - If data block record is found.
    END_OF_FILE_BLOCK_FOUND - If end of file record is found.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_lg_ascii_get_block_type (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
publicdef INT ld_lg_ascii_get_block_type (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT status;
    INT buffer_type;
    
    buffer_type = determine_buffer_type (ptrBIG);
    
    switch (buffer_type)
        {
    case UWI_HEADER_RECORD:
    case FORMAT_HEADER_RECORD:
    case CURVES_HEADER_RECORD:
    case TRACES_HEADER_RECORD:
    case NULL_HEADER_RECORD:
    case SAMPLE_HEADER_RECORD:
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
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ld_lg_ascii_skip_block(LD_LG_ASCII_STRUCT *ptrBIG);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_lg_ascii_skip_block (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
publicdef INT ld_lg_ascii_skip_block (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT blktype;
    INT pblktype;
    
    pblktype = blktype = ld_lg_ascii_get_block_type (ptrBIG);
    
    while (blktype EQUALS pblktype)
        {
        (void) strcpy (ptrBIG->ptrLOCAL->buffer, NULL_STRING);
        ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
        fill_buffer (ptrBIG);
        
        blktype = ld_lg_ascii_get_block_type (ptrBIG);
        }

     return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ld_lg_ascii_load_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    LD_LG_ASCII_DATA_BLK_NOT_FOUND
    LD_LG_ASCII_ERROR_LIMIT
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_lg_ascii_load_data (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
publicdef INT ld_lg_ascii_load_data (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT status;
    INT blktype;
    INT num_errors, number_errors, record_number = 0;
    
    status = init_load_data (ptrBIG);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    
    blktype = ld_lg_ascii_get_block_type (ptrBIG);
    
    if (blktype EQUALS DELIMITER_BLOCK_FOUND)
        (void) ld_lg_ascii_skip_block (ptrBIG);
    
    blktype = ld_lg_ascii_get_block_type (ptrBIG);
    
    if (blktype IS_NOT_EQUAL_TO DATA_BLOCK_FOUND)
        status = LD_LG_ASCII_DATA_BLK_NOT_FOUND;
    
    /* reset parameters */
    num_errors = 0;
    record_number = 0;
    while (blktype EQUALS DATA_BLOCK_FOUND)
        {
        record_number++;
        if ((record_number % 100) EQUALS 0)
            {
            (void) sprintf (msgbuf, mg_message (LD_LG_ASCII_READING), record_number);
            wiscdt (ptrBIG->message_id, LD_LG_ASCII_RECORD_PROMPT, msgbuf);
            }
        if (num_errors <= ptrBIG->err_limit)
            {
            status = get_data_records (ptrBIG);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                /* an error occured while reading, so increment num_errors
                and go back and read another record */
                (void) sprintf (msgbuf, mg_message (LD_LG_ASCII_ERR_IN_REC), record_number,
                         ptrBIG->ptrLOCAL->buffer);
                wiscdt (ptrBIG->message_id, LD_LG_ASCII_ERROR_PROMPT, msgbuf);
                am_message (AM_STATUS, msgbuf);
                num_errors++;
                }
            else
                {
                status = load_data_record (ptrBIG, &number_errors);
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    break;
                else
                    num_errors += number_errors;
                }
            }
        else
            status = LD_LG_ASCII_ERROR_LIMIT;
        
        *(ptrBIG->ptrLOCAL->buffer) = NULL_CHAR;
        ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
        fill_buffer (ptrBIG);
        
        blktype = ld_lg_ascii_get_block_type (ptrBIG);
        }
    if (status EQUALS SUCCESS)
        status = save_data (ptrBIG);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        (void) cleanup_log_data (ptrBIG);
        return(status);
        }
    else
        {
        status = cleanup_log_data (ptrBIG);
        return(status);
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ld_lg_ascii_read_headers(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ptrBIG          -(LD_LG_ASCII_STRUCT *)
    
Return Value/Status:
    SUCCESS - Successful completion.
    LD_LG_ASCII_HDR_BLK_NOT_FOUND
    HEADER_BAD
    LD_LG_ASCII_BAD_HEADER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_lg_ascii_read_headers (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
publicdef INT ld_lg_ascii_read_headers (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT status = SUCCESS;
    INT blktype;
    INT buffer_type;
    INT slen;
    CHAR string[BUF_SIZE + 1];
    INT tmp;
    
    blktype = ld_lg_ascii_get_block_type (ptrBIG);
    
    if (blktype EQUALS DELIMITER_BLOCK_FOUND)
        (void) ld_lg_ascii_skip_block (ptrBIG);
    
    blktype = ld_lg_ascii_get_block_type (ptrBIG);
    
    if (blktype IS_NOT_EQUAL_TO HEADER_BLOCK_FOUND)
        status = LD_LG_ASCII_HDR_BLK_NOT_FOUND;
    
    while (blktype EQUALS HEADER_BLOCK_FOUND)
        {
        buffer_type = determine_buffer_type (ptrBIG);
        
        switch (buffer_type)
            {
        case UWI_HEADER_RECORD:
            if( get_parameter_string (ptrBIG, "UWI", string) 
		IS_NOT_EQUAL_TO SUCCESS )
            {
                status = LD_LG_ASCII_BAD_HEADER;
            }

            tmp = (INT) strlen( string );
            slen = MIN( tmp , sizeof(UWI) - 1 );
            (void) strncpy (ptrBIG->uwi, string, slen);
            ptrBIG->uwi[slen] = NULL_CHAR;
            break;
            
        case FORMAT_HEADER_RECORD:
            if (get_parameter_string (ptrBIG, "FORMAT", string) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            ptrBIG->format = XSTRING_ALLOCATE (string);
            (void) strcpy (ptrBIG->format, string);
            break;
            
        case CURVES_HEADER_RECORD:
            if (get_parameter_string (ptrBIG, "CURVES", string) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            if (string_to_parms (string, &ptrBIG->trace_names) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            break;
            
        case TRACES_HEADER_RECORD:
            if (get_parameter_string (ptrBIG, "TRACES", string) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            if (string_to_parms (string, &ptrBIG->trace_names) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            break;
            
        case NULL_HEADER_RECORD:
            if (get_parameter_string (ptrBIG, "NULL", string) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            if (strip_all_blanks (string) EQUALS SUCCESS AND isfloat (string) > 0)
                ptrBIG->null_value = atof (string);
            else
                {
                ptrBIG->null_value = LOGNULL;
                status = HEADER_BAD;
                }
            break;
            
        case SAMPLE_HEADER_RECORD:
            if (get_parameter_string (ptrBIG, "SAMPLE", string) IS_NOT_EQUAL_TO SUCCESS)
                status = HEADER_BAD;
            if (strip_all_blanks (string) EQUALS SUCCESS AND isfloat (string) > 0)
                ptrBIG->sample_incr = atof (string);
            else
                {
                ptrBIG->sample_incr = 0.05;
                status = LD_LG_ASCII_BAD_HEADER;
                }
            break;
            }
        (void) strcpy (ptrBIG->ptrLOCAL->buffer, NULL_STRING);
        ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
        fill_buffer (ptrBIG);
        
        blktype = ld_lg_ascii_get_block_type (ptrBIG);
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ld_lg_ascii_verify_fmt(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    NOT_ENOUGH_TRACE_NAMES
    FORMAT_BAD
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ld_lg_ascii_verify_fmt (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
publicdef INT ld_lg_ascii_verify_fmt (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT i;
    INT status = SUCCESS;
    INT ncurves = 0;
    INT nformats = 0;
    TCP f_parms = (TCP)NULL;
    
    if (string_to_parms (ptrBIG->format, &f_parms) IS_NOT_EQUAL_TO SUCCESS)
        status = FORMAT_BAD;
    
    else if (format_parameters_to_structures (f_parms,
                                              &ptrBIG->ptrLOCAL->
                                              format_structures) IS_NOT_EQUAL_TO 
             SUCCESS)
        status = FORMAT_BAD;
    
    else
        {
        /* count number of trace names */
        for (i = 0;
         ((ptrBIG->trace_names IS_NOT_EQUAL_TO (TCP)
           NULL) AND (ptrBIG->trace_names[i] IS_NOT_EQUAL_TO (CHAR *)NULL)); i++)
            ncurves++;
        
        nformats = 0;
        /* count number of non-skip formats */
        for (i = 0;
             ((ptrBIG->ptrLOCAL->format_structures IS_NOT_EQUAL_TO (FS **)
               NULL) AND (ptrBIG->ptrLOCAL->format_structures[i] IS_NOT_EQUAL_TO (FS *)
                         NULL)); i++)
            {
            if ((ptrBIG->ptrLOCAL->format_structures[i]->
                 type IS_NOT_EQUAL_TO 'X') AND (ptrBIG->ptrLOCAL->format_structures[i]->
                                              type IS_NOT_EQUAL_TO 'x'))
                nformats++;
            }
        ptrBIG->ptrLOCAL->number_ns_formats = nformats;
        
        nformats = 0;
        /* count number of skiped and non-skipped formats */
        for (i = 0;
             ((ptrBIG->ptrLOCAL->format_structures IS_NOT_EQUAL_TO (FS **)
               NULL) AND (ptrBIG->ptrLOCAL->format_structures[i] IS_NOT_EQUAL_TO (FS *)
                         NULL)); i++)
            {
            nformats++;
            }
        ptrBIG->ptrLOCAL->number_all_formats = nformats;
        
        if (ptrBIG->ptrLOCAL->number_ns_formats EQUALS ncurves)
            {
            status = SUCCESS;
            }
        else
            status = NOT_ENOUGH_TRACE_NAMES;
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT fill_buffer(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT fill_buffer (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT fill_buffer (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    BOOL get_another_line;
    INT line_len;
    CHAR line[BUF_SIZE + 1];
    
    get_another_line = TRUE;
    while (ptrBIG->ptrLOCAL->
           eof_flag IS_NOT_EQUAL_TO TRUE AND get_another_line EQUALS TRUE)
        {
        if (fgets (line, BUF_SIZE, ptrBIG->pf) EQUALS (CHAR *)NULL)
            {
            ptrBIG->ptrLOCAL->eof_flag = TRUE;
            get_another_line = FALSE;
            }
        else
            {
            line_len = strlen (line);
            line[line_len - 1] = NULL_CHAR;
            }
        if (strlen (line) EQUALS strspn (line, " "))
            get_another_line = TRUE;
        
        else
            {
            get_another_line = FALSE;
            
            (void) strcpy (ptrBIG->ptrLOCAL->buffer, ptrBIG->ptrLOCAL->buffer_ptr);
            (void) strcat (ptrBIG->ptrLOCAL->buffer, line);
            ptrBIG->ptrLOCAL->buffer_ptr = ptrBIG->ptrLOCAL->buffer;
            }
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT determine_buffer_type(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)
    
Return Value/Status:
    DATA_RECORD
    DELIMITER_HEADER_RECORD
    SAMPLE_HEADER_RECORD
    NULL_HEADER_RECORD
    TRACES_HEADER_RECORD
    CURVES_HEADER_RECORD
    FORMAT_HEADER_RECORD
    UWI_HEADER_RECORD
    END_OF_FILE_RECORD
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT determine_buffer_type (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT determine_buffer_type (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT status;
    CHAR *string;
    
    string = ptrBIG->ptrLOCAL->buffer_ptr;
    string += strspn (string, " ");
    
    if (ptrBIG->ptrLOCAL->eof_flag EQUALS TRUE)
        status = END_OF_FILE_RECORD;
    
    else if (strncmp (string, "UWI", 3) EQUALS 0)
        status = UWI_HEADER_RECORD;
    
    else if (strncmp (string, "FORMAT", 6) EQUALS 0)
        status = FORMAT_HEADER_RECORD;
    
    else if (strncmp (string, "CURVES", 6) EQUALS 0)
        status = CURVES_HEADER_RECORD;
    
    else if (strncmp (string, "TRACES", 6) EQUALS 0)
        status = TRACES_HEADER_RECORD;
    
    else if (strncmp (string, "NULL", 4) EQUALS 0)
        status = NULL_HEADER_RECORD;
    
    else if (strncmp (string, "SAMPLE", 6) EQUALS 0)
        status = SAMPLE_HEADER_RECORD;
    
    else if (strncmp (string, ptrBIG->delimit, strlen (ptrBIG->delimit)) EQUALS 0)
        status = DELIMITER_HEADER_RECORD;
    
    else
        status = DATA_RECORD;
    
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT get_parameter_string(LD_LG_ASCII_STRUCT *ptrBIG, CHAR *keyword, 
                                    CHAR *string);
                                    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)
    (I) keyword     -(CHAR *)
    (M) string      -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    LD_LG_ASCII_GARBAGE_AT_EOL
    LD_LG_ASCII_NO_RIGHT_PAREN
    LD_LG_ASCII_NO_LEFT_PAREN
    LD_LG_ASCII_NO_HEADER
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT get_parameter_string (LD_LG_ASCII_STRUCT *ptrBIG, CHAR *keyword,
                                     CHAR *string) 
#else
static INT get_parameter_string (ptrBIG, keyword, string)
LD_LG_ASCII_STRUCT *ptrBIG;
CHAR *keyword;
CHAR *string;
#endif
    {
    INT state = 1;
    INT status = NOT_DONE;
    INT type;
    CHAR token[BUF_SIZE + 1];
    
    string[0] = NULL_CHAR;
    
    /* find keyword, then move buffer_ptr */
    ptrBIG->ptrLOCAL->buffer_ptr += strspn (ptrBIG->ptrLOCAL->buffer_ptr, " ");
    if (strncmp (ptrBIG->ptrLOCAL->buffer_ptr, keyword,
                 strlen (keyword)) IS_NOT_EQUAL_TO 0)
        status = LD_LG_ASCII_NO_HEADER;
    else
        ptrBIG->ptrLOCAL->buffer_ptr += strlen (keyword);
    
    while (status EQUALS NOT_DONE)
        {
        type = get_next_token (ptrBIG, token);
        
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
                (void) strcpy (string, token);
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
        if (strlen (ptrBIG->ptrLOCAL->buffer_ptr) EQUALS strspn (ptrBIG->ptrLOCAL->
                                                                buffer_ptr, " "))
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
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT get_next_token(LD_LG_ASCII_STRUCT *ptrBIG, CHAR *token);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)
    (M) token       -(CHAR *)

Return Value/Status:
    STRING_TOKEN
    RIGHT_PAREN_TOKEN
    LEFT_PAREN_TOKEN
    END_OF_FILE_TOKEN
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT get_next_token (LD_LG_ASCII_STRUCT *ptrBIG, CHAR *token) 
#else
static INT get_next_token (ptrBIG, token)
LD_LG_ASCII_STRUCT *ptrBIG;
CHAR *token;
#endif
    {
    INT status = NOT_DONE;
    INT return_status;
    INT delimiter_position;
    INT buffer_length;
    
    while (status EQUALS NOT_DONE)
        {
        /* strip leading blanks */
        ptrBIG->ptrLOCAL->buffer_ptr += (INT) strspn (ptrBIG->ptrLOCAL->buffer_ptr, " ");
        buffer_length = strlen (ptrBIG->ptrLOCAL->buffer_ptr);
        
        if (buffer_length EQUALS 0)
            {
            if (ptrBIG->ptrLOCAL->eof_flag EQUALS TRUE)
                status = DONE;
            else
                fill_buffer (ptrBIG);
            }
        else
            {
            delimiter_position = strcspn (ptrBIG->ptrLOCAL->buffer_ptr, "()");
            
            if ((buffer_length <
                 BUF_SIZE) AND (buffer_length EQUALS delimiter_position))
                {
                if (ptrBIG->ptrLOCAL->eof_flag EQUALS TRUE)
                    status = DONE;
                else
                    fill_buffer (ptrBIG);
                }
            else
                status = DONE;
            }
        }
    delimiter_position = strcspn (ptrBIG->ptrLOCAL->buffer_ptr, "()");
    
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
        delimiter_position = MIN (BUF_SIZE, delimiter_position);
        (void) strncpy (token, ptrBIG->ptrLOCAL->buffer_ptr, 
			delimiter_position);
        token[delimiter_position] = NULL_CHAR;
        ptrBIG->ptrLOCAL->buffer_ptr += delimiter_position;
        (void) strip_all_blanks (token);
        return_status = STRING_TOKEN;
        }
    return(return_status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT strip_all_blanks(CHAR *string);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT strip_all_blanks (CHAR *string) 
#else
static INT strip_all_blanks (string)
CHAR *string;
#endif
    {
    CHAR *from = string;
    CHAR *to = string;
    
    while (*from IS_NOT_EQUAL_TO NULL_CHAR)
        {
        if ((*from IS_NOT_EQUAL_TO SPACE) AND (
                                  *from IS_NOT_EQUAL_TO '\n') AND (*from
                                                                 IS_NOT_EQUAL_TO '\t'))
            *to++ = *from;
        
        from++;
        }
    *to = NULL_CHAR;
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT convert_format_parameter(CHAR *parameter, FS *format_structure);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) parameter       -(CHAR *)
    (O) format_structure    -(FS *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT convert_format_parameter (CHAR *parameter, FS *format_structure) 
#else
static INT convert_format_parameter (parameter, format_structure)
CHAR *parameter;
FS *format_structure;
#endif
    {
    INT factor = 0;
    CHAR type = SPACE;
    INT width = 0;
    INT divisor = 0;
    INT state = FACTOR_STATE;
    INT status = NOT_DONE;
    
    /* if first character is a letter, then factor is assummed to be 1 */
    if ((*parameter IS_NOT_EQUAL_TO NULL_CHAR) AND ((
                                            *parameter EQUALS 'F') OR (*parameter
                                                                     EQUALS 'f') OR (
                                                                            *parameter 
                                                                                EQUALS 'I'
                                                                                   ) OR (
                                                                                        *
                                                                                        parameter
                                                                                         EQUALS
                                                                                        'i'
                                                                                        )
                                                   OR (
                                                   *parameter 
                                                       EQUALS 'E') OR (*parameter
                                                                     EQUALS 'e') OR (
                                                                            *parameter 
                                                                                EQUALS 'X'
                                                                                   ) OR (
                                                                                        *
                                                                                        parameter
                                                                                         EQUALS
                                                                                        'x'
                                                                                        )
                                                   ))
        {
        factor = 1;
        }
    while ((status EQUALS NOT_DONE) AND (*parameter IS_NOT_EQUAL_TO NULL_CHAR))
        {
        if (state EQUALS SKIP_STATE)
            status = LD_LG_ASCII_BAD_FORMAT_CHAR;
        
        else if (isdigit (*parameter) IS_NOT_EQUAL_TO 0)
            {
            switch (state)
                {
            case FACTOR_STATE:
                factor = factor * 10 + (*parameter - ZERO);
                break;
                
            case IWIDTH_STATE:
            case FWIDTH_STATE:
                width = width * 10 + (*parameter - ZERO);
                break;
                
            case DIVISOR_STATE:
                divisor = divisor * 10 + (*parameter - ZERO);
                break;
                
            default:
                status = LD_LG_ASCII_BAD_FORMAT_CHAR;
                break;
                }
            }
        else if ((*parameter EQUALS 'F') OR (*parameter EQUALS 'f') OR (
                                                                  *parameter 
                                                                    EQUALS 'I') OR (
                                                                            *parameter 
                                                                                EQUALS 'i'
                                                                                  ) OR (
                                                                                     *parameter
                                                                                        EQUALS
                                                                                       'E'
                                                                                       )
                 OR (*parameter EQUALS 'e') OR (
                                            *parameter EQUALS 'X') OR (*parameter
                                                                     EQUALS 'x'))
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
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT string_to_parms(CHAR *string, TCP *out_parameter);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string          -(CHAR *)
    (M) out_parameter   -(TCP *)

Return Value/Status:
    SUCCESS - Successful completion.
    LD_LG_ASCII_NO_PARAMETER
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT string_to_parms (CHAR *string, TCP *out_parameter) 
#else
static INT string_to_parms (string, out_parameter)
CHAR *string;
TCP *out_parameter;
#endif
    {
    INT i;
    INT memsize = 0;
    CHAR parameter[PARM_SIZE + 1];
    INT delimiter_position;
    INT status = NOT_DONE;
    TCP temp_arr;
    
    if (*out_parameter IS_NOT_EQUAL_TO (TCP)NULL)
        {
        ts_tcp_free (*out_parameter);
        *out_parameter = (TCP)NULL;
        }
    if (IS_EMPTY_STRING (string))
        {
        status = LD_LG_ASCII_NO_PARAMETER;
        }
    for (i = 0; status EQUALS NOT_DONE; i++)
        {
        if (i % 100 EQUALS 0)
            {
            memsize += 100;
            temp_arr = (TCP) ytc_zalloc ((memsize + 1) * sizeof(CHAR *));
#if 0
            if (*out_parameter IS_NOT_EQUAL_TO (TCP)NULL)
                {
                for (j = 0; (*out_parameter)[j] IS_NOT_EQUAL_TO (CHAR *)NULL; j++)
                    temp_arr[j] = (*out_parameter)[j];
                
                yfree (*out_parameter);
                }
#endif
            *out_parameter = temp_arr;
            }
        delimiter_position = (INT) strcspn (string, ",");
        
        if (delimiter_position EQUALS 0)
            status = LD_LG_ASCII_NO_PARAMETER;
        else
            {
            delimiter_position = MIN (PARM_SIZE, delimiter_position);
            (void) strncpy (parameter, string, delimiter_position);
            parameter[delimiter_position] = NULL_CHAR;
            
            (*out_parameter)[i] = YSTRING_ALLOCATE (parameter);
            (void) strcpy ((*out_parameter)[i], parameter);
            
            string += delimiter_position;
            
            if (IS_EMPTY_STRING (string))
                status = SUCCESS;
            
            else if ((strlen (string) > 1) AND (*string EQUALS COMMA))
                string++;
            
            else if ((strlen (string) EQUALS 1) AND (*string EQUALS COMMA))
                status = LD_LG_ASCII_NO_PARAMETER;
            }
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT expand_format_structure(FS **from, FS ***ptr_to);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) from        -(FS **)
    (O) to          -(FS ***)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <ld_lg_acpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT expand_format_structure (FS **from, FS ***ptr_to) 
#else
static INT expand_format_structure (from, ptr_to)
FS **from;
FS ***ptr_to;
#endif
    {
    INT nforms = 0;
    INT i;
    INT j;
    INT k;
    FS **to = *ptr_to;

#if 0
    if (to IS_NOT_EQUAL_TO (FS **)NULL)
        {
        for (i = 0; to[i] IS_NOT_EQUAL_TO (FS *)NULL; i++)
            xfree (to[i]);
        
        xfree (to);
        }
#endif
    if (from IS_NOT_EQUAL_TO (FS **)NULL)
        {
        for (i = 0; from[i] IS_NOT_EQUAL_TO (FS *)NULL; i++)
            nforms += from[i]->factor;
        }
    *ptr_to = (FS **)xtc_zalloc ((nforms + 1) * sizeof(FS *));
    to = *ptr_to;
    
    for (i = 0, k = 0; from[i] IS_NOT_EQUAL_TO (FS *)NULL; i++)
        {
        for (j = 0; j < from[i]->factor; j++)
            {
            to[k] = XALLOCATE (FS);
            
            to[k]->factor = 1;
            to[k]->type = from[i]->type;
            to[k]->width = from[i]->width;
            to[k]->divisor = from[i]->divisor;
            k++;
            }
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT format_parameters_to_structures(TCP str_parms, FS ***exp_fs);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    str_parms       -(TCP)
    exp_fs          -(FS ***)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT format_parameters_to_structures (TCP str_parms, FS ***exp_fs) 
#else
static INT format_parameters_to_structures (str_parms, exp_fs)
TCP str_parms;
FS ***exp_fs;
#endif
    {
    FS **com_fs = (FS **)NULL;
    INT i;
    INT nparms = 0;
    INT status = SUCCESS;
    
    /* allocate format structures */
    
    if (str_parms IS_NOT_EQUAL_TO (TCP)NULL)
        {
        for (i = 0; str_parms[i] IS_NOT_EQUAL_TO (CHAR *)NULL; i++)
            nparms++;
        }
    com_fs = (FS **)xtc_zalloc ((nparms + 1) * sizeof(FS *));
    
    for (i = 0; ((i < nparms) AND (status EQUALS SUCCESS)); i++)
        {
        com_fs[i] = XALLOCATE (FS);
        status = convert_format_parameter (str_parms[i], com_fs[i]);
        }
    if ((com_fs[0] IS_NOT_EQUAL_TO (FS *)NULL) AND (status EQUALS SUCCESS))
        expand_format_structure (com_fs, exp_fs);

#if 0
    if (com_fs IS_NOT_EQUAL_TO (FS **)NULL)
        {
        for (i = 0; com_fs[i] IS_NOT_EQUAL_TO (FS *)NULL; i++)
            xfree (com_fs[i]);
        
        xfree (com_fs);
        }
#endif
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT get_data_records(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    LD_LG_ASCII_BAD_DATA_RECORD
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT get_data_records (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT get_data_records (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT i, j, k, xsl, sl, status = SUCCESS, a_period;
    CHAR string[BUF_SIZE + 1], *pp;
    
    j = 0;
    for (i = 0; i < ptrBIG->ptrLOCAL->number_all_formats; i++)
        {
        /* string copy to string[] and check if there is a period */
        /* set sl to MIN(width, strlen(buffer_ptr)) */
        for (xsl = 0, sl = 0, pp = ptrBIG->ptrLOCAL->buffer_ptr, a_period = FALSE;
             sl <
             ptrBIG->ptrLOCAL->format_structures[i]->width AND *
             pp IS_NOT_EQUAL_TO NULL_CHAR; sl++, pp++)
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
                if (isfloat (string) > 0)
                    {
                    ptrBIG->ptrLOCAL->curr_record[j] = atof (string);
                    
                    if (a_period EQUALS FALSE)
                        for (k = 0;
                             k < ptrBIG->ptrLOCAL->format_structures[i]->divisor; k++)
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
        ptrBIG->ptrLOCAL->buffer_ptr += MIN (sl,
                                             ptrBIG->ptrLOCAL->format_structures[i]->
                                             width);
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT save_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)]
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT save_data (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT save_data (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT nformats = 0;
    INT column;
    INT depth = ptrBIG->depth_column;
    LOG_TRACE_NAME trace_name;
    INT status = SUCCESS;
    INT tmp_status = SUCCESS;
    LOG_TRACE_STRUCT log_trace;
    
    if (ptrBIG->ptrLOCAL->tot_count <= 0)
        return FAIL;
    
    nformats = ptrBIG->ptrLOCAL->number_ns_formats;
    
    for (column = 0; column < nformats; column++)
        {
        if (column IS_NOT_EQUAL_TO depth)   /* skip this processing if this is the depth column */
            {
            status = mk_trace_name (ptrBIG, column, trace_name);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                return(status);
            
            am_message (AM_STATUS, trace_name);
            wiscdt (ptrBIG->message_id, LD_LG_ASCII_RECORD_PROMPT, trace_name);
            wiscdt (ptrBIG->message_id, LD_LG_ASCII_ERROR_PROMPT, "");
            
            status = mk_lt_struct (ptrBIG, &log_trace, ptrBIG->ptrLOCAL->nlist[column],
                                   trace_name);
            if (status IS_NOT_EQUAL_TO SUCCESS)
	        {
                am_message(AM_STATUS,"!!! trace not added to database.");
                (void) sprintf(msgbuf,mg_message(status));
                am_message(AM_STATUS,msgbuf);
                tmp_status = status;
	        }
            else
        /*        return(status);  */
	    {
            (void) sprintf (msgbuf, mg_message (LD_LG_ASCII_WRITING));
            wiscdt (ptrBIG->message_id, LD_LG_ASCII_ERROR_PROMPT, msgbuf);
            am_message (AM_STATUS, msgbuf);
            
            /* trim trailing spaces from the TRACE_UOM field, so that */
            /* it won't be too long for LG_LTDM_ADD_TRACE             */
            ts_trim (log_trace.trace_uom);
            
            status = lg_ltdm_add_trace (&log_trace);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                am_message (AM_ALERT, mg_message (LD_LG_ASCII_NOT_WRITTEN));
                am_message (AM_STATUS, mg_message (status));
       /*         return(status);  */
                tmp_status = status;
                }
            (void) sprintf (msgbuf, mg_message (LD_LG_ASCII_WRITTEN));
            wiscdt (ptrBIG->message_id, LD_LG_ASCII_ERROR_PROMPT, msgbuf);
            am_message (AM_STATUS, msgbuf);
            }         
            }
        }
    return(tmp_status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT mk_lt_struct(LD_LG_ASCII_STRUCT *ptrBIG, LOG_TRACE_STRUCT *log_trace,
                            NLIST_HEADER nlist, CHAR *trace_name);
                            
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)
    (M) log_trace   -(LOG_TRACE_STRUCT *)
    (I) nlist       -(NLIST_HEADER)
    (I) trace_name  -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mk_lt_struct (LD_LG_ASCII_STRUCT *ptrBIG, LOG_TRACE_STRUCT *log_trace,
                             NLIST_HEADER nlist, CHAR *trace_name) 
#else
static INT mk_lt_struct (ptrBIG, log_trace, nlist, trace_name)
LD_LG_ASCII_STRUCT *ptrBIG;
LOG_TRACE_STRUCT *log_trace;
NLIST_HEADER nlist;
CHAR *trace_name;
#endif
    {
    INT status = SUCCESS;
    UINT stroke_num = 0;
    UINT NDim;
    UINT DList[2];
    INT xtype, ytype;
    VOIDPTR Type[2];
    FLOAT fxmin, fymin, fxmax, fymax;
    INT ixmin, iymin, ixmax, iymax;
    VOIDPTR MinVal[2];
    VOIDPTR MaxVal[2];
    DOUBLE conv;
    FLOAT fconv;
    VOIDPTR VList[1];
    LOG_UOM depth_units;
    
    tc_zeroes ((VOIDPTR)log_trace, sizeof(LOG_TRACE_STRUCT));
    
    status = get_uom_conv (ptrBIG, &conv, depth_units);
    
    fconv = conv;
    stroke_num = 0;
    NDim = 1;
    DList[0] = 1, DList[1] = 2;
    VList[0] = (VOIDPTR) & fconv;
    status = nl_convert_stroke_info (nlist, stroke_num, NL_STROKE_START_VALUE, NDim,
                                     DList, VList);
    status = nl_convert_stroke_info (nlist, stroke_num, NL_STROKE_INCREMENT, NDim,
                                     DList, VList);
    
    fxmin = fymin = fxmax = fymax = -4321.0;
    ixmin = iymin = ixmax = iymax = -4321;
    
    NDim = 2;
    
    Type[0] = (VOIDPTR) & xtype;
    Type[1] = (VOIDPTR) & ytype;
    status = nl_inq_nlist_info (nlist, NL_NLIST_DATA_TYPES, NDim, DList, Type);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return(status);
    
    if (xtype EQUALS INT_TYPE)
        {
        MinVal[0] = (VOIDPTR) & ixmin;
        MaxVal[0] = (VOIDPTR) & ixmax;
        }
    else
        {
        MinVal[0] = (VOIDPTR) & fxmin;
        MaxVal[0] = (VOIDPTR) & fxmax;
        }
    if (ytype EQUALS INT_TYPE)
        {
        MinVal[1] = (VOIDPTR) & iymin;
        MaxVal[1] = (VOIDPTR) & iymax;
        }
    else
        {
        MinVal[1] = (VOIDPTR) & fymin;
        MaxVal[1] = (VOIDPTR) & fymax;
        }
    status = nl_inq_nlist_info (nlist, NL_NLIST_MIN, NDim, DList, MinVal);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return(status);
    
    status = nl_inq_nlist_info (nlist, NL_NLIST_MAX, NDim, DList, MaxVal);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return(status);
    
    if (xtype EQUALS INT_TYPE)
        {
        log_trace->top = (FLOAT)ixmin;
        log_trace->base = (FLOAT)ixmax;
        }
    else
        {
        log_trace->top = fxmin;
        log_trace->base = fxmax;
        }
    if (ytype EQUALS INT_TYPE)
        {
        log_trace->min = (FLOAT)iymin;
        log_trace->max = (FLOAT)iymax;
        }
    else
        {
        log_trace->min = fymin;
        log_trace->max = fymax;
        }
    hoblockmove (ptrBIG->uwi, log_trace->uwi, sizeof(UWI));
    hoblockmove (trace_name, log_trace->trace_name, sizeof(LOG_TRACE_NAME));
    log_trace->dinc = ptrBIG->sample_incr * conv;
    hoblockmove (depth_units, log_trace->depth_uom, sizeof(LOG_UOM));
    ts_fill (log_trace->trace_uom, ' ', sizeof(LOG_UOM) - 1);
    log_trace->ptrace = nlist;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine to provides the conversion factor and units of measure 
    for the depth domain of an output trace.                

    METHOD:   1) Open PROJECT_DEFAULTS workspace & get default UOM    
              2) Try to convert input uom to project uom.             
              3) If INVALID or INCOMPATIBLE UOM, we know it is input  
                    uom since project uom was validated as loader started
                    Therefore, conv = 1.0 and depth_units = input uom    
              4) Else, conv has been set and depth_units = project.   

Prototype:
    static INT get_uom_conv(LD_LG_ASCII_STRUCT *ptrBIG, DOUBLE  *conv, 
                            LOG_UOM depth_uom);
                            
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *) Pointer to the loader structure.               
    (O) conv        -(DOUBLE *) Conversion factor...input uom to stored uom.   
    (O) depth_units -(LOG_UOM) Character string for db table.            

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT get_uom_conv (LD_LG_ASCII_STRUCT *ptrBIG, DOUBLE *conv,
                             LOG_UOM depth_uom) 
#else
static INT get_uom_conv (ptrBIG, conv, depth_uom)
LD_LG_ASCII_STRUCT *ptrBIG;
DOUBLE *conv;
LOG_UOM depth_uom;
#endif
    {
    PROJECT_DEFAULT *proj;
    DOUBLE uconv;
    UOM_NAME uom_project;
    UOM_NAME uom_input;
    INT status;
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *) & proj);
    
    (void) strcpy (uom_input, ptrBIG->depth_uom);
    (void) strcpy (uom_project, proj->unit.vertical);
    
    status = tu_unit_conversion (1.0, uom_input, &uconv, uom_project);
    
    if (status EQUALS SUCCESS)
        {
        *conv = (DOUBLE)uconv;      /* cast to DOUBLE for GET_UOM_CONV's caller */
        (void) strcpy ((char *)depth_uom, (char *)uom_project);
        }
    else
        {
        *conv = 1.0;
        (void) strcpy ((char *)depth_uom, (char *)uom_input);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT mk_trace_name(LD_LG_ASCII_STRUCT *ptrBIG, INT column, 
                            CHAR *trace_name);
                            
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)
    (I) column      -(INT)
    (I) trace_name  -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT mk_trace_name (LD_LG_ASCII_STRUCT *ptrBIG, INT column,
                              CHAR *trace_name) 
#else
static INT mk_trace_name (ptrBIG, column, trace_name)
LD_LG_ASCII_STRUCT *ptrBIG;
INT column;
CHAR *trace_name;
#endif
    {
    TCP traces = ptrBIG->trace_names;
    CHAR *uwi = ptrBIG->uwi;
    CHAR *source = ptrBIG->source;
    INT status = SUCCESS;
    
    /* create trace name */
    if ((uwi EQUALS (CHAR *)NULL) OR (traces EQUALS (TCP)
                                     NULL) OR (traces[column] EQUALS (CHAR *)NULL))
        {
        return FAIL;
        }
    (void) sprintf (trace_name, "%s:%s", source, traces[column]);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT isfloat(CHAR *x);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) x           -(CHAR *)

Return Value/Status:
    See below.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT isfloat (CHAR *x) 
#else
static INT isfloat (x)
CHAR *x;
#endif
    {
    INT position = 1;
    INT xlen;
    BOOL exponent = FALSE;
    BOOL plus_minus = FALSE;
    BOOL decimal = FALSE;
    BOOL type = 0;                  /* 1 = digit, 2 = 'E|e', 3 = '.', 4 = '+|-' */
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
    
    if (*ps >= '0' AND * ps <= '9')
        type = 1;
    else if (*ps EQUALS '.')
        type = 3;
    else if (*ps EQUALS '+' OR * ps EQUALS '-')
        {
        type = 4;
        plus_minus = TRUE;
        }
    else
        return -1;
    
    for (ps++, position++; *ps; ps++, position++)
        {
        if (*ps >= '0' AND * ps <= '9')
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
        else if (*ps EQUALS 'E' OR * ps EQUALS 'e')
            {
            if (exponent)
                return -position;
            if (*(ps + 1) EQUALS '+' OR * (ps + 1) EQUALS '-')
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
        xlen = (INT) strlen (x);
        if (plus_minus EQUALS TRUE AND xlen > 2)
            ;
        else if (plus_minus EQUALS FALSE AND xlen > 1)
            ;
        else
            return -position;
        }
    else if (type IS_NOT_EQUAL_TO 1)
        return -position;
    
    return(position - 1);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT load_data_record(LD_LG_ASCII_STRUCT *ptrBIG, INT *number_errors);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG          -(LD_LG_ASCII_STRUCT *)
    (O) number_errors   -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT load_data_record (LD_LG_ASCII_STRUCT *ptrBIG, INT *number_errors) 
#else
static INT load_data_record (ptrBIG, number_errors)
LD_LG_ASCII_STRUCT *ptrBIG;
INT *number_errors;
#endif
    {
    INT i, num_bogus, err_limit, depth, status;
    
    err_limit = ptrBIG->err_limit;
    depth = ptrBIG->depth_column;
    
    /* calculate the needed number of bogus records to add */
    num_bogus = compute_fill_number (ptrBIG);
    
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
        status = add_to_nlist (ptrBIG, ptrBIG->ptrLOCAL->curr_record);
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
                                                    ptrBIG->ptrLOCAL->
                                                    prev_record[depth] +
                                                    ptrBIG->sample_incr;
            
            status = add_to_nlist (ptrBIG, ptrBIG->ptrLOCAL->bogus_record);
            if (status EQUALS SUCCESS)
                ptrBIG->ptrLOCAL->tot_count++;
            }
        /* then add current record */
        status = add_to_nlist (ptrBIG, ptrBIG->ptrLOCAL->curr_record);
        if (status EQUALS SUCCESS)
            ptrBIG->ptrLOCAL->tot_count++;
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT init_load_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT init_load_data (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT init_load_data (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    INT i, status, nformats;
    UINT Ndim = 2;
    INT Units[2];
    INT DType[2];
    size_t DSize[2];
    INT VInc[2];
    UINT Blocksize = 1001;
    static CHAR *Dim_Names[] = 
        {
        "DEPTH",
        "VALUE"
        };
    
    /* for setting null value info */
    UINT NullDim = 1;
    UINT NDList[1];
    FLOAT fxnull;
    VOIDPTR Null_List[1];
    
    NDList[0] = 2;
    Null_List[0] = (VOIDPTR) & fxnull;
    DType[0] = DType[1] = FLOAT_TYPE;
    DSize[0] = DSize[1] = sizeof(FLOAT);
    VInc[0] = INCREMENT_LIST, VInc[1] = VALUE_LIST;
    
    nformats = ptrBIG->ptrLOCAL->number_ns_formats;
    fxnull = ptrBIG->null_value;
    
    ptrBIG->ptrLOCAL->tot_count = 0;
    
    if ((ptrBIG->ptrLOCAL->curr_record = (FLOAT *)ytc_zalloc (nformats *
                                                              sizeof(
                                                                 FLOAT))) EQUALS (
                                                                               FLOAT *)
                                         0)
        return FAIL;
    
    if ((ptrBIG->ptrLOCAL->bogus_record = (FLOAT *)ytc_zalloc (nformats *
                                                               sizeof(
                                                                 FLOAT))) EQUALS (
                                                                               FLOAT *)
                                          0)
        return FAIL;
    
    if ((ptrBIG->ptrLOCAL->prev_record = (FLOAT *)ytc_zalloc (nformats *
                                                              sizeof(
                                                                 FLOAT))) EQUALS (
                                                                               FLOAT *)
                                         0)
        return FAIL;
    
    if ((ptrBIG->ptrLOCAL->nlist = (NLIST_HEADER *)ytc_zalloc (nformats *
                                                               sizeof(
                                                                NLIST_HEADER))) EQUALS (
                                                                          NLIST_HEADER 
                                                                                     *)0
                                   )
        return FAIL;
    
    /* initialize bogus_record */
    for (i = 0; i < nformats; i++)
        {
        ptrBIG->ptrLOCAL->bogus_record[i] = ptrBIG->null_value;
        
        /* initialize nlists */
        
        if ((status = nl_init_nlist (&ptrBIG->ptrLOCAL->nlist[i], Ndim, Units, DSize,
                                     DType, VInc, Blocksize)) IS_NOT_EQUAL_TO SUCCESS)
            return status;
        
        status = nl_set_nlist_int (ptrBIG->ptrLOCAL->nlist[i], NL_NLIST_DEFERRED_STATS,
                                   TRUE);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        
        status = nl_set_nlist_info (ptrBIG->ptrLOCAL->nlist[i], NL_NLIST_NULL_VALUE,
                                    NullDim, NDList, Null_List);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        status = nl_set_nlist_info (ptrBIG->ptrLOCAL->nlist[i], NL_NLIST_DIM_NAME,
                                    Ndim, Dim_List, (VOIDPTR *)Dim_Names);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT cleanup_log_data(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT cleanup_log_data (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT cleanup_log_data (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
    {
    int i, status = SUCCESS;
    
    yfree (ptrBIG->ptrLOCAL->curr_record);
    yfree (ptrBIG->ptrLOCAL->bogus_record);
    yfree (ptrBIG->ptrLOCAL->prev_record);
    
    for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
        status = nl_free_nlist (ptrBIG->ptrLOCAL->nlist[i]);
    
    return(status EQUALS SUCCESS) ? SUCCESS : status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT add_to_nlist(LD_LG_ASCII_STRUCT *ptrBIG, FLOAT *record);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) ptrBIG      -(LD_LG_ASCII_STRUCT *)
    (I) record      -(FLOAT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpac.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT add_to_nlist (LD_LG_ASCII_STRUCT *ptrBIG, FLOAT *record) 
#else
static INT add_to_nlist (ptrBIG, record)
LD_LG_ASCII_STRUCT *ptrBIG;
FLOAT *record;
#endif
    {
    INT i, status;
    UINT Ndim = 2;
    VOIDPTR Value_List[2];
    
    if ((ptrBIG->ptrLOCAL->tot_count % 1000) EQUALS 0)
        {
        status = create_a_stroke (ptrBIG);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        }
    for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
        {
        /* add data to n-lists, except the depth one */
        if (i IS_NOT_EQUAL_TO ptrBIG->depth_column)
            {
            Value_List[0] = (VOIDPTR) & record[ptrBIG->depth_column];
            Value_List[1] = (VOIDPTR) & record[i];
            
            status = nl_add_points (ptrBIG->ptrLOCAL->nlist[i], 1, Ndim, Dim_List,
                                    Value_List);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                return status;
            }
        }
    /* copy the newly added record to previous record */
    for (i = 0; i < ptrBIG->ptrLOCAL->number_ns_formats; i++)
        ptrBIG->ptrLOCAL->prev_record[i] = record[i];
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT create_a_stroke(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT create_a_stroke (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT create_a_stroke (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
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
            status = nl_start_stroke (ptrBIG->ptrLOCAL->nlist[i], &stroke_num);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                return status;
            
            if (stroke_num EQUALS 1)
                {
                /* using current record, set the stroke information */
                VList[0] = (VOIDPTR) &
                           ptrBIG->ptrLOCAL->curr_record[ptrBIG->depth_column];
                status = nl_set_stroke_info (ptrBIG->ptrLOCAL->nlist[i], stroke_num,
                                             NL_STROKE_START_VALUE, NDim, DList,
                                             VList);
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    return status;
                /* Now set the input sample increment  */
                VList[0] = (VOIDPTR) & ptrBIG->sample_incr;
                status = nl_set_stroke_info (ptrBIG->ptrLOCAL->nlist[i], stroke_num,
                                             NL_STROKE_INCREMENT, NDim, DList, VList);
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    return status;
                }
            else
                {
                /*using previous record, set the stroke information */
                VList[0] = (VOIDPTR) &
                           ptrBIG->ptrLOCAL->prev_record[ptrBIG->depth_column];
                status = nl_set_stroke_info (ptrBIG->ptrLOCAL->nlist[i], stroke_num,
                                             NL_STROKE_START_VALUE, NDim, DList,
                                             VList);
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    return status;
                /* Now set the input sample increment  */
                VList[0] = (VOIDPTR) & ptrBIG->sample_incr;
                status = nl_set_stroke_info (ptrBIG->ptrLOCAL->nlist[i], stroke_num,
                                             NL_STROKE_INCREMENT, NDim, DList, VList);
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
        
        status = add_to_nlist (ptrBIG, ptrBIG->ptrLOCAL->prev_record);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT compute_fill_number(LD_LG_ASCII_STRUCT *ptrBIG);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) ptrBIG      -(LD_LG_ASCII_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_lg_ascpar.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT compute_fill_number (LD_LG_ASCII_STRUCT *ptrBIG) 
#else
static INT compute_fill_number (ptrBIG)
LD_LG_ASCII_STRUCT *ptrBIG;
#endif
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
        
        if (fabs (difference) < tolerance)
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
/* END:     */
