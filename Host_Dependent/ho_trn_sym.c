/* DEC/CMS REPLACEMENT HISTORY, Element HO_TRN_SYM.C*/
/* *9    20-DEC-1990 16:51:12 MING "(SPR 6424) return HO_ERR_NO_SYMBOL instead of FAIL"*/
/* *8    24-SEP-1990 16:42:26 GILLESPIE "Merge: Beta Deltas"*/
/*  5B1  24-SEP-1990 16:40:27 GILLESPIE "Beta Deltas"*/
/* *7    27-AUG-1990 15:48:17 VINCE "(SPR 1) changed hoz_translate_path to a public def ho_translate_path to be called by*/
/*ho_absolute_path"*/
/* *6    23-AUG-1990 16:13:13 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *5    23-JUL-1990 11:47:14 VINCE "(SPR 1) final fixes for recursive logical names on Unix"*/
/* *4    18-JUN-1990 11:20:21 VINCE "(SPR 1) add John G's recursive translation"*/
/* *3    26-FEB-1990 18:59:19 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2     1-NOV-1989 10:12:45 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *1    19-JUN-1989 12:59:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_TRN_SYM.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_TRN_SYM.C*/
/* *5    23-JUL-1990 11:47:14 VINCE "(SPR 1) final fixes for recursive logical names on Unix"*/
/* *4    18-JUN-1990 11:20:21 VINCE "(SPR 1) add John G's recursive translation"*/
/* *3    26-FEB-1990 18:59:19 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2     1-NOV-1989 10:12:45 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *1    19-JUN-1989 12:59:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_TRN_SYM.C*/

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

/*************************************************************************

NAME:   HO_TRANSLATE_SYMBOL

DESCRIPTION:

    To retrieve the value of a given symbol from local/global
    Global Variable File.

    Input  Parameters:
	 SYMBOL = (CST) the given symbol.

    Output Parameters:
	 VALUE = (CST) the returned value of given symbol.
	 HO_TRANSLATE_SYMBOL = (INT) error status.
			     =  0 -- ok;
			     = -1 -- symbol does not exist.

*************************************************************************/

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_HO_ERR_H
#include "esi_ho_err.h"
#endif

#ifndef ESI_TC_ERR_H
#include "esi_tc_err.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifdef vms
#pragma nostandard
#include descrip
#include ssdef
#include lnmdef
#include psldef
#pragma standard

typedef struct item_struct
{
    USHORT buflen;
    USHORT code;
    BYTE *bufadr;
    USHORT *retlenadr;
}ITEM_STRUCT;

static INT hoz_translate_symbol ();
static INT hoz_find_symbol (CHAR *symbol,TCP *ptcp);
/************************************************************************/
publicdef INT ho_translate_symbol(symbol, ptcp)
/************************************************************************/

CHAR *symbol;
CHAR ***ptcp;
{
    INT attribute = 0, status = SUCCESS;
    INT total_symbols, symbol_indexes[4], i, table_index, tcp_index;
    USHORT length;
    ITEM_STRUCT *item_list, *item_list2;
    INT *returnedLen;
    CHAR value[256], **tcp;
    static CHAR *default_table[] = {"LNM$JOB","LNM$PROCESS",
					"LNM$GROUP","LNM$SYSTEM"};  
#pragma nostandard
    $DESCRIPTOR(logical_table_desc, default_table[0]);
    $DESCRIPTOR(logical_name_desc, symbol);
#pragma standard

    if (symbol EQUALS NULL OR strlen(symbol) EQUALS 0)
    {
	return HO_NULL_LOG_NAME;
    }

    logical_name_desc.dsc$w_length = strlen(symbol);


    item_list = (ITEM_STRUCT *)tc_alloc( sizeof(ITEM_STRUCT ) * 3);
    item_list[0].buflen = sizeof(INT );
    item_list[0].retlenadr = 0;
    item_list[0].code = LNM$_MAX_INDEX;

    item_list[1].buflen = item_list[2].buflen = 0;
    item_list[1].bufadr = item_list[2].bufadr = 0;
    item_list[1].retlenadr = item_list[2].retlenadr = 0;
    item_list[1].code = item_list[2].code = 0;

    for( i = 0, status = SUCCESS, total_symbols = 0; i < 4; i++)
    {
	logical_table_desc.dsc$w_length = strlen( default_table[i] );
	logical_table_desc.dsc$a_pointer = (CHAR *)default_table[i];
        item_list[0].bufadr = (BYTE *)&symbol_indexes[i];
	status = SYS$TRNLNM( 0, &logical_table_desc, 
					&logical_name_desc, 0,
					item_list);
	if( symbol_indexes[i] EQUALS -1 OR ( status EQUALS SS$_NOLOGNAM) )
	{
	    symbol_indexes[i] = -1;
	    continue;
	}
	else
	if (! (status & 1))
	    status = HO_ERR_VMS;
	else
	    status = SUCCESS;

	if( status != SUCCESS )
	{
	    tc_free(item_list);
	    return status;
	}
	total_symbols += (symbol_indexes[i] + 1);
    }
    if( total_symbols EQUALS 0)
    {
	tc_free(item_list);
	return HO_ERR_NO_SYMBOL;
    }
    *ptcp = tcp = (CHAR **)tc_zalloc(sizeof (CHAR *) * (total_symbols + 1));
    if(tcp == (CHAR **)0) 
	return TC_ERR_NO_MEMORY;

    item_list[0].buflen = 4;
    item_list[0].code = LNM$_INDEX;
    item_list[1].buflen = 256;
    item_list[1].bufadr = (BYTE *)value;
    item_list[1].retlenadr = (USHORT *)&length;
    item_list[1].code = LNM$_STRING;

    for(table_index = 0, tcp_index = 0; table_index < 4; table_index++)
    {
	logical_table_desc.dsc$a_pointer = (CHAR *)default_table[table_index];
	logical_table_desc.dsc$w_length = strlen( default_table[table_index]);
	for( i = 0; 
	     i <= symbol_indexes[table_index] AND status != HO_ERR_VMS;
	     i++)
	{
	    item_list[0].bufadr = (BYTE *)&i;
	    status = SYS$TRNLNM( 0, &logical_table_desc, 
					&logical_name_desc, 0,
					item_list);
	
	    status = SUCCESS;
	    tcp[tcp_index]=(CHAR *)tc_zalloc(sizeof(CHAR )*(length + 1));
	    if(tcp[tcp_index] == (CHAR *)0) 
		return TC_ERR_NO_MEMORY;
	    hoblockmove(value, tcp[tcp_index++], length);
	}
    }
    tc_free(item_list);
    return status;
}

#else

static INT hoz_translate_symbol ();
static INT hoz_find_symbol (CHAR *symbol,TCP *ptcp);
#include "esi_lognode.h"
extern struct lognode *lnmprocess;

/************************************************************************/
publicdef INT ho_translate_symbol(symbol, ptcp)
/************************************************************************/
CHAR *symbol;	/* Symbol to be translated			*/
TCP  *ptcp;	/* pointer to the callers TCP for output	*/
{
    INT      ii, status, n_names;
    TCP      temp_tcp;
    FILENAME temp_path, abs_path;

    *ptcp    = (TCP)NULL;	/* initialize the return pointer	*/
    temp_tcp = (TCP)NULL;	/* initialize the local TCP		*/
    
    /* start recursive symbol translation */
    status = hoz_translate_symbol (symbol, &temp_tcp);

    if (status == SUCCESS) {
	n_names = ts_tcp_len (temp_tcp);
	for (ii = 0; ii < n_names; ii++) {
	    /*----------------------------------*/
	    /* translate internal logical names */
	    /*----------------------------------*/
	    temp_path[0] = NUL;
	    ho_translate_path (temp_tcp[ii], temp_path);

	    /*--------------------------------------*/
	    /* translate any OS symbols in the path */
	    /*--------------------------------------*/
	    abs_path[0] = NUL;
	    ho_translate_filename (temp_path, abs_path);

	    ts_tcp_append (ptcp, abs_path);
	}
    }

    return status;
}

static INT hoz_translate_symbol (symbol, ptcp)
CHAR *symbol;	/* symbol to be looked up		*/
TCP  *ptcp;	/* pointer to callers TCP for output	*/
{
    INT ii, status;
    TCP temp;
    FILENAME full_path, absolute_path;
    static INT recursion_level = 0;

    recursion_level++;
    if (recursion_level > 20) {
	ts_tcp_append (ptcp, symbol);
	return FAIL;
    }

    temp = (TCP)NULL;
    status = hoz_find_symbol (symbol, &temp);
    if (status == SUCCESS) {		/* a translation was found	*/
	/* recursivly translate each symbol in the current list.	*/
	for (ii=0; temp[ii]; ii++) {
	    hoz_translate_symbol (temp[ii], ptcp);
	}
    }
    else {
    	ts_tcp_append (ptcp, symbol);
    }

    recursion_level--;
    return status;
}

publicdef INT ho_translate_path (path, outpath)
CHAR *path;
CHAR *outpath;
{
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Input path is in form "head <separator> tail"
    where separator is the left-most separator.
    Returns a pointer to the end of outpath after
    any copying of translations necessary
    
    The results of the translation will appear in outpath
    It is the programmer's responsibility to assure that
    outpath points to a buffer large enough to hold the
    translated tokens

    Uses:
        This function is used to expand logical names of the
        form a/B/c, where any of the tokens a, B or C may itself
        be a logical name.  For instance, if the name B translates
        to e/g, then the result of calling ho_translate_path would be
        a/e/g/c/.

    Author:
        J Gillespie 23 Mar 1990

    Limitations:
        Max. # tokens:          32
        Input buffer length:    255
	Max. recursion levels:	20

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    INT  status;
    TCP  tcp;
    TCP  ntcp;
    TCP  translation;
    CHAR *token[32];
    CHAR *ptoken;
    CHAR buffer[256];
    CHAR sep[2];
    BOOL found;
    INT len;
    static INT recursion_level = 0;

    recursion_level++;
    if (recursion_level > 20) {
	strcpy (outpath, path);
	return FAIL;
    }

    strcpy(buffer, path);

    /* if this path starts with 'root' put that in the output */
    if (buffer[0] == '/') strcat(outpath, "/");		/* UNIX	    */
    if (buffer[0] == '<') strcat(outpath, "<");		/* Primos   */

    sep[0] = ENTRYSEPERATOR;
    sep[1] = NUL;

    /* break the input string into tokens */
    for (ptoken = buffer, tcp = token, *tcp = buffer; *tcp; ptoken = NULL)
	{
	if (ptoken == NULL) tcp++;
    	*tcp = strtok(ptoken, sep);
	}

    /* Translate each token */
    for (ntcp = token; *ntcp; ntcp++) {
	status = hoz_find_symbol (*ntcp, &translation);
	if (status == SUCCESS) ho_translate_path (translation[0], outpath);
	else {
	    strcat(outpath,*ntcp);
	    strcat(outpath,sep);
	}
    }

    recursion_level--;
    if ((recursion_level == 0) && outpath[0] != NUL) {
	outpath[strlen(outpath)-1] = NUL;	/* lose the last seperator */
    }

    return SUCCESS;
}

static INT hoz_find_symbol (symbol, ptcp)
CHAR *symbol;
TCP  *ptcp;
{
    struct lognode *tlp;

    *ptcp = (TCP)NULL;
    for (tlp = lnmprocess; tlp; tlp=tlp->next) {
	if (strcmp(symbol, tlp->log_name) == 0) {
	    ts_tcp_copy (ptcp, tlp->tcp);
	    return SUCCESS;
	}
    }

    /*	symbol not found; put the symbol itself in the return TCP and 
	indicate not_found in the status code.				*/
    ts_tcp_append (ptcp, symbol);
    return HO_ERR_NO_SYMBOL;
}
#endif
