/* DEC/CMS REPLACEMENT HISTORY, Element TS_TOKEN.C */
/* *2    18-MAR-1991 18:10:56 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 18:03:36 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_TOKEN.C */
#include "esi_gl_defs.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to return TCP of string which are separated by the delimeter

Prototype:
    publicdef TCP ts_token(CHAR *string,CHAR *delimeter)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I)	string	    -(CHAR *) input string where the tcp will be created from
    (I)	delimeter   -(CHAR *) delimeter that will separate each of the string in
			tcp

Return Value/Status:
    constructed tcp

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
publicdef TCP ts_token(CHAR *string,CHAR *delimeter)
#else
publicdef TCP ts_token(string,delimeter)
CHAR *string;
CHAR *delimeter;
#endif
{
    CHAR *token;
    CHAR *temp_string;
    TCP ptcp;

    temp_string=(CHAR *)tc_alloc((sizeof(CHAR)*strlen(string))+1);
    strcpy(temp_string,string);
    ptcp=(TCP)0;
    token = strtok(temp_string,delimeter);
    
    FOREVER
	{
	if(!token) break;
	ts_tcp_append(&ptcp,token);
	token = strtok(NULL,delimeter);
	}

    tc_free(temp_string);	
    return ptcp;
}

