/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_JOURNAL.C*/
/* *6    14-AUG-1991 12:02:02 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *5     6-MAR-1990 17:25:41 JULIAN "(SPR 6010) Added ULTRIX mods."*/
/* *4     2-MAR-1990 11:43:21 JULIAN "(SPR 6012) fix ifdef USE_X == 0"*/
/* *3     2-MAR-1990 11:14:25 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *2     5-FEB-1990 23:28:35 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:35:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_JOURNAL.C*/
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

/****************************************************************

WIZ_JOURNAL.C

   This file defines the routines used internally by the
journal/playback programs.

*****************************************************************/

#include "esi_c_lib.h"
#include "esi_wi_int.x"     /* external definitions required here */
#include "esi_wi_err.h"
#include "esi_wi_msg.h"
#include "esi_am_defs.h"
#include "esi_mg.h"
#include "esi_ho_files.h"
#include "esi_ts.h"

#define JP_BREAK 1500

#if USE_PROTOTYPES
static INT wiz_jp_write_newline();
static INT wiz_jp_write_line(CHAR *string);
static INT wiz_jp_playback_error(CHAR *is, CHAR *shouldbe);
#else
static INT wiz_jp_write_newline();
static INT wiz_jp_write_line();
static INT wiz_jp_playback_error();
#endif

/*****************************************************************
******************************************************************

   This set of words are used to setup or terminate the
journal/playback sessions.

******************************************************************
*****************************************************************/


/*****************************************************************/

publicdef INT wiz_jp_initialize()
/*****************************************************************

This routine is used to open the file in read or write mode, based
on jpmode == JOURNAL or PLAYBACK.  Additional initializations
should be added here.

*****************************************************************/
{
#ifndef USE_X
     /* try to open the file in the correct mode ... */
     INT status;

     if (wiv.jpmode == WI_JOURNAL) {
	wiv.jfile = ho_open( wiv.jfilename, "w", NULL );
	status = ho_lock(wiv.jfile, ON);
	if (status != SUCCESS)
	  wiv.jfile = NULL_FILE_PTR;
     }
     else {
	if (wiv.jpmode == WI_PLAYBACK) {
	   wiv.jfile = ho_open(wiv.jfilename, "r", NULL );
	}
	else {
	   wiv.jfile = NULL_FILE_PTR;
	}
     }
     if (wiv.jfile == NULL_FILE_PTR) {
	wiv.jpmode =  WI_CANT_OPEN_JP_FILE;
	return(WI_CANT_OPEN_JP_FILE);
     }

     /* empty the jstring buffer... */
     wiv.jpstring[0] = 0;
     wiv.jparsed=0;
     wiv.playback_nosend = 0;   /* permit output if in PLAYBACK */
     wiv.playback_eatrcv = 0;   /* if PLAYBACK, do not wait to eat mac stuff */
#endif
     return(SUCCESS);
}


/*****************************************************************/
publicdef INT wiz_jp_terminate()
/*****************************************************************

This closes the journaling file.  Add any other cleanup here.

*****************************************************************/
{
#ifndef USE_X
     INT status;

     CHECK_JPMODE;

     status = ho_lock(wiv.jfile, OFF);
     fclose(wiv.jfile);
     wiv.jpmode = 0;
#endif
     return(SUCCESS);
}


/*****************************************************************
******************************************************************

This set of words are used to write to the journal file...
they all check to see that 'jpmode' is set to JOURNAL.

******************************************************************
*****************************************************************/

/*****************************************************************/
static INT wiz_jp_write_newline()
/*****************************************************************

This just sends a '\n' to the file, but more functionality
may be added here later.

*****************************************************************/
{
#ifndef USE_X
     CHECK_JMODE;

     putc('\n', wiv.jfile);
#endif
     return(SUCCESS);
}


/*****************************************************************/
static INT wiz_jp_write_line(string)
/*****************************************************************

This routine sends a string to the file.  If the last char is
not '\n' it adds it.

*****************************************************************/
CHAR *string;
{
#ifndef USE_X
     CHECK_JMODE;

     fputs(string, wiv.jfile);

     if  (string[(strlen(string)-1)] != '\n')  {
	 wiz_jp_write_newline();
     }

#endif
     return(SUCCESS);
}

/*****************************************************************/
publicdef INT wiz_jp_write_string(string)
/*****************************************************************

This routine sends a string to the file.

*****************************************************************/
CHAR *string;
{
#ifndef USE_X
     CHECK_JMODE;

     fputs(string, wiv.jfile);
#endif
     return(SUCCESS);
}

/*****************************************************************/
publicdef INT wiz_jp_write_long(thelong)
/*****************************************************************

This routine converts a long int to a string, then sends
it to the journal file.

*****************************************************************/
long thelong;
{
#ifndef USE_X
     CHAR tempbuff[64];

     CHECK_JMODE;

     sprintf(tempbuff, "%ld \n", thelong);
     wiz_jp_write_line(tempbuff);
#endif
     return(SUCCESS);
}

/*****************************************************************/
publicdef INT wiz_jp_write_float(thefloat)
/*****************************************************************

This routine converts a floating pt. number to a string, then sends
it to the journal file.

*****************************************************************/
DOUBLE thefloat;
{
#ifndef USE_X
     CHAR tempbuff[64];

     CHECK_JMODE;

     sprintf(tempbuff, "%f \n", thefloat);
     wiz_jp_write_line(tempbuff);
#endif
     return(SUCCESS);
}

/****************************************************************/
publicdef INT wiz_jp_add_text( string )
/*****************************************************************

This routine adds the text on one line with TEXT: preceeding it.

*****************************************************************/
CHAR *string;
{
#ifndef USE_X
     CHAR text[200];

     CHECK_JMODE;

     sprintf(text, "TEXT: %s", string);
     wiz_jp_write_line( text );
#endif
     return(SUCCESS);
}

/*****************************************************************
******************************************************************

This set of words are used to read from the journal file...
they all check to see that 'jpmode' is set to PLAYBACK.

******************************************************************
*****************************************************************/


/*****************************************************************/
publicdef INT wiz_jp_read_line()
/*****************************************************************

This routine gets the next line in the opened journal file,
placing the string at the 'jpstring' buffer.  It also clears
the counter for parsing so that we look at the start.

If the received string is "BREAK:" (case-insensitive), it turns off
journal mode.

If the received string is "WAIT:", it does until a 'cr' is received.

*****************************************************************/
{
#ifndef USE_X
     INT   i, another;
     CHAR  text[100];

     CHECK_PMODE;

     do {

	/* initially, set to NOT loop... */
	another = 0;

	/* read a line from the file... */
	if ( fgets(wiv.jpstring, ST_MAC_MAX, wiv.jfile) == NULL ) {
	   return(WI_PLAY_EOF);
	}
	wiv.jparsed++;

	/* convert to upper case in temp buffer... */
	strcpy( text, wiv.jpstring );
	ts_sto_upper( text );

	/* isolate the first word; find the 1st non-character ... */
	/* NOTE: 'trap' words MUST begin in first column */
	for (i=0; text[i] > ' '; i++);  text[i] = NUL;

	if (strcmp( text, "TEXT:") == 0)
	   another = TRUE;
	else  {
	   if (strcmp( text, "WAIT:") == 0) {

	      /* notify that we're waiting... */
	      sprintf(text, mg(WI_JP_WAITING), wiv.jparsed, wiv.jfilename);
	      am_message(AM_STATUS, text);

	      /* turn off the PLAYBACK mode... */
	      wiv.jpmode=0;

	      /* get a line of anything... */
	      wi_beep(1);
	      wi_enable_keyboard(1);
	      /* set mac play mode to accept CR from any dialog once */
	      SEND_2_VALUES(2, MC_SHUTUP);
	      hogets( text, 99 );

	      /* re-enable PLAYBACK mode... */
	      wi_enable_keyboard(0);
	      SEND_2_VALUES(1, MC_SHUTUP);
	      wiv.jpmode = WI_PLAYBACK;

	      another = TRUE;
	   }
	}
     } while (another);

     if (strcmp( text, "BREAK:") == 0)  {
	wiz_jp_terminate();
	wiv.jpmode=0;
	SEND_2_VALUES(0, MC_SHUTUP);
	sprintf(text, mg(WI_PLAYBACK_CANCELED), wiv.jparsed, wiv.jfilename);
	am_message(AM_STATUS, text);
	return(JP_BREAK);
     }
#endif
     return(SUCCESS);
}


/*****************************************************************/
publicdef INT wiz_jp_read_long(thelong)
/*****************************************************************

This routine interprets the next string in the file as a
longint.

*****************************************************************/
long *thelong;
{
#ifndef USE_X
     CHECK_PMODE;

     wiz_jp_read_line();
     sscanf(wiv.jpstring, "%ld", thelong);
#endif
     return(SUCCESS);
}

/*****************************************************************/
publicdef INT wiz_jp_read_float(thefloat)
/*****************************************************************

This routine interprets the next string in the file as a
floating pt. number.

*****************************************************************/
FLOAT *thefloat;
{
#ifndef USE_X
     CHECK_PMODE;

     wiz_jp_read_line();
     sscanf(wiv.jpstring, "%f", thefloat);
#endif
     return(SUCCESS);
}


/*****************************************************************
******************************************************************

This set is used for general j/p processing in the wi_routines.

******************************************************************
*****************************************************************/

/*****************************************************************/
static INT wiz_jp_playback_error(is, shouldbe)
/*****************************************************************

Called on a sequencing error during playback.

*****************************************************************/
CHAR *is, *shouldbe;
{
#ifndef USE_X
     CHAR text[100];

     wiv.jpmode=0;

     sprintf(text, mg(WI_PLAYBACK_BAD_SEQUENCE), wiv.jparsed, wiv.jfilename);
     am_message(AM_STATUS, text);

     sprintf(text, mg(WI_PLAYBACK_BAD_SEQUENCE1), is, shouldbe);
     am_message(AM_STATUS, text);
#endif
     exit(EXIT_FAILURE);
}

/*****************************************************************/
publicdef INT wiz_jp_name(name)
/*****************************************************************

wiz_jp_name() is executed by most wi routines at their beginning...

In journal mode, it simply sends its name to the journal file and
it adds a '\n', terminating the line.

In playback mode it verifies it should be the next function executing.

If neither journaling or playing, nothing happens.

*****************************************************************/
CHAR *name;
{
#ifndef USE_X

     INT i, len;

     if (wiv.jpmode == WI_JOURNAL) {          /* are we journaling? */
	wiz_jp_write_string( name );    /* yes, write my name */
	wiz_jp_write_newline();
     }

     else {

	if (wiv.jpmode == WI_PLAYBACK) {       /* no, are we playing back? */
	   if (wiz_jp_read_line() != JP_BREAK) { /* yes, get the next line */
#if 0
  /* this starts at the end of the string and strips any trailing cr or blank */
	      for (i=(strlen(wiv.jpstring)-1);
		   (wiv.jpstring[i] == '\n' || wiv.jpstring[i] == ' ') &&
								       (i > 0);
		    i--)  {
		   wiv.jpstring[i] = NUL;
	       }
#endif

  /* this starts at the BEGINNING and places a null after the 1st word. */

	       if ((len=strlen(wiv.jpstring)) > 0)  {

		  /* find the 1st non-blank character */
		  for (i=0;
		       (wiv.jpstring[i] == ' ') && (i < len);
		       i++);

		  /* find the 1st non-character ... */
		  for (; (wiv.jpstring[i] > ' ') && (i < len); i++);

		  wiv.jpstring[i] = NUL;
	       }
	       if (strcmp(name, wiv.jpstring) != 0)  {   /* is it wrong? */
		  wiz_jp_playback_error(name, wiv.jpstring);
	       }
	   }
	}
     }
#endif
     return(SUCCESS);
}
                                                                                                                      
