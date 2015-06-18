/* DEC/CMS REPLACEMENT HISTORY, Element GR_CON_DMS2DEC.C */
/* *5    17-DEC-1990 17:28:11 JESSIE "(SPR 0) fix parameter type error for UNIX" */
/* *4     5-DEC-1990 10:21:32 JESSIE "(SPR 6233) accept xtic/ytic with DEGREES" */
/* *3    13-NOV-1990 11:04:55 JESSIE "(SPR 0) verify DMS input format" */
/* *2    13-NOV-1990 11:02:31 JESSIE "(SPR 0) verify DMS input format" */
/* *1    13-NOV-1990 11:01:51 JESSIE "check DMS input format" */
/* DEC/CMS REPLACEMENT HISTORY, Element GR_CON_DMS2DEC.C */

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
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



/* Function Description -----------------------------------------------------
Description:
    Conver degrees, minutes and seconds to decimal degrees.

Prototype:
    publicdef INT con_dms2dec (arg1,arg2);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg1	- string for conversion
    (O) arg2    - decimal degrees
	 
	EX:							
         Input :   +158 30 29.8						
		   +158 30' 29.8"					
		   +158d 30m 29.8s (+158D 30M 29.8S)        			
		   Format:						
		          SIGN DEG <sep> MIN <sep> SEC			
		          <sep> : "one" space				
		   Range:             					
		   (-180 0 0.0  -- 0 0 0 -- +180 0 0.0)			
									
         Output:   +158.5082778						

Return Value/Status:
    SUCCESS		- Successfull completion
    TU_INVALID_VALUE	- Invalid value input
    TU_INVALID_STRING	- Invalid string combination
-----------------------------------------------------------------------------*/


#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TU_ERR_H
#include "esi_tu_err.h"
#endif

#define DEGREE 0
#define MINUTE 1
#define SECOND 2

publicdef INT ck_num(CHAR *buf);
publicdef INT power(INT base, INT n);
publicdef INT reverse(CHAR *s);

#if USE_PROTOTYPES
publicdef INT con_dms2dec(CHAR *buffer, DOUBLE *accum)
#else

publicdef INT con_dms2dec(buffer,accum)
CHAR   *buffer;
DOUBLE *accum;
#endif
{
INT    field = SECOND;
INT    sign = 1;
INT    j,c,len;
INT    i = 0; 
DOUBLE value = 0.0;
CHAR   temp[20];
CHAR   buffer1[20],uom_string[20];
CHAR   *next_string;
INT    status;

    if (buffer EQUALS NULL) return TU_INVALID_VALUE;

    next_string = ts_next_word (buffer,buffer1);;
    if (next_string == (CHAR *)NULL)
	{
 	if ((status = ck_num(buffer1)) == SUCCESS)
	    {
	    return FAIL;
	    }
	else
	    goto start;
	}
    else
	{
	next_string = ts_next_word (next_string,uom_string);
	if ((status = strcmp (uom_string,"")) == SUCCESS)
	    {
/*	    *accum = (DOUBLE)atof(buffer1);*/
	    return FAIL;
	    }
        else if ((status = strcmp(uom_string,"DEGREES")) == SUCCESS || 
	         (status = strcmp(uom_string,"DEG"))== SUCCESS ||
		 (status = strcmp(uom_string,"degrees")) == SUCCESS || 
		 (status = strcmp(uom_string,"deg"))== SUCCESS)
	    {
	    *accum = (DOUBLE)atof(buffer1);
	    return SUCCESS;
	    }
	else
	    {
start:
    *accum = 0.0;
    len = strlen(buffer);
    for (c = len - 1; c >= 0 ; c--)
    {
	switch(buffer[c])
	{
	    case '+' :
	    case '-' :	    
	    case ' ' :
		    if (buffer[c+1] != ' ')
		    {
			if (buffer[c] == '+') sign = 1;
			if (buffer[c] == '-') sign = -1;
			temp[i] = '\0';
			reverse(temp);
			value = atof(temp);
		    
			if (field == SECOND)
			{
			    if (value >= 60) return TU_INVALID_VALUE;
			}
			if (field == MINUTE)
			{
			    if (value >= 60) return TU_INVALID_VALUE;
			}
			if (field == DEGREE)
			{
			    if (value > 180) return TU_INVALID_VALUE;
			}
			*accum += value * (1.0 / power(60,field));
		    
			field --;
			i = 0;
			temp[0] = '\0';
		    }
		    break;

	    case '0' : case '1' : case '2' : case '3' : case '4' :
	    case '5' : case '6' : case '7' : case '8' : case '9' :
		    temp[i] = buffer[c];
		    i++;
		    break;	
			    
            case '.' :
                    temp[i] = '\0';
		    reverse(temp);
		    value = atof(temp);
		    if (value > 9.0) value = 9.0;
		    if (value < 1.0) value = 0.0;
		    *accum += value * ( 1.0/(power(60,field) * power(10,1)));
		    i = 0;
		    temp[0] = '\0';
		    break;

	    case 'd':
	    case 'D':
		    field = 0;
		    break;

	    case 'm':
	    case 'M':
	    case '\'':
		    field = 1;
		    break;

	    case 's':
	    case 'S':
	    case '"':
		    field = 2;
		    break;
	    default :
		    return TU_INVALID_STRING;
		    break;	    
	}
    }
    if (buffer[0] != '+' && buffer[0] != '-')
    {
	temp[i] = '\0';
	reverse(temp);
	value = atof(temp);
	
	if (field == SECOND)
	{
		if (value >= 60) return TU_INVALID_VALUE;
	}
	if (field == MINUTE)
	{
		if (value >= 60) return TU_INVALID_VALUE;
	}
	if (field == DEGREE)
	{
		if (value > 180) return TU_INVALID_VALUE;
	}
	*accum += value * (1.0 / power(60,field));
		    
    }
    if (*accum > 180.0) return TU_INVALID_VALUE;
    *accum *= sign;
    return SUCCESS; 
	}
    }
}


#if USE_PROTOTYPES
publicdef INT ck_num(CHAR *buf)
#else
publicdef INT ck_num(buf)
CHAR *buf;
#endif
{
    int len, c;

    len = strlen(buf);
    for (c = len - 1; c >= 0 ; c--)
    {
	switch(buf[c])
	{
	    case '0' : case '1' : case '2' : case '3' : case '4' :
	    case '5' : case '6' : case '7' : case '8' : case '9' :
	    case '.' :
		    break;

	    default:
		    return FAIL;
		    break;
	}
     }
     return SUCCESS;	    
}

#if USE_PROTOTYPES
publicdef INT power(INT base, INT n)
#else
publicdef INT power(base, n)
INT base;
INT n;
#endif
{
    int i,p;

    p = 1;
    for (i = 1; i<=n; ++i)
	p = p * base;
    return p;
}

#if USE_PROTOTYPES
publicdef INT reverse(CHAR *s)
#else
publicdef INT reverse(s)
CHAR *s;
#endif
{
    int i,j;
    char c;

    for ( i= 0, j= strlen(s) - 1; i<j; i++, j--)
    {
	c = s[i];
        s[i] = s[j];
	s[j] = c;
    }
    return SUCCESS;
}
