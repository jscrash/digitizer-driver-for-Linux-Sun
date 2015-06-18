/* DEC/CMS REPLACEMENT HISTORY, Element HO_TRN_FILE.C*/
/* *7    23-AUG-1990 16:13:09 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *6    23-JUL-1990 11:47:02 VINCE "(SPR 1) final fixes for recursive logical names on Unix"*/
/* *5     2-JUL-1990 17:24:32 VINCE "(SPR 1) added call to ho_translate_symbol to expand internal logical names."*/
/* *4    26-FEB-1990 18:59:16 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *3    19-OCT-1989 14:42:40 GORDON "(SPR 999) clean up output on directories, smooth up concealed logicals"*/
/* *2     5-SEP-1989 15:52:29 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *1    19-JUN-1989 12:59:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_TRN_FILE.C*/

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

#include "esi_ho_files.h"
#include "esi_c_lib.h"

#ifdef vms
#include "fab.h"
#include "nam.h"
#endif

/* **********************************************************************

    ROTINUE: HO_TRANSLATE_FILENAME.

    DESCRIPTION:
	To translate a file descriptor into a full path file name.

    AUTHOR: James Chang.
    DATE : May 4, 1988

    Modified: J Gillespie
    Date: 2 Aug 88

	Added SEARCH to look in directories
	Note that if a wildcard spec is made (or a directory search list
	is specified, only the FIRST file is found.  You should use
	HO_FIND_FILES et al if you wish a list of files to be returned...

    ********************************************************************** */

publicdef INT ho_translate_filename(in_str, out_str )
FILENAME in_str;
FILENAME out_str;
{
    INT		status = SUCCESS;
    FILENAME	extended;
    FILENAME	resultant;
    TCP		translations;
    
#ifdef vms
    FILENAME tempname;
    struct FAB fab;
    struct NAM nam;
    CHAR *p;

    fab = cc$rms_fab;
    nam = cc$rms_nam;

			     /* set up the input parameters.                */
    fab.fab$l_fna = in_str;
    fab.fab$b_fns = strlen(in_str );
    fab.fab$w_ifi = 0;
    fab.fab$l_nam = &nam;

    nam.nam$l_esa = extended;
    nam.nam$b_ess = sizeof (FILENAME);
    nam.nam$b_nop = NAM$M_NOCONCEAL;

    status = sys$parse(&fab);
			     /* if parse succeeds                           */
    if (status & 1)
	{
			     /* look up file in directory                   */
	nam.nam$l_rsa = resultant;
	nam.nam$b_rss = sizeof (FILENAME);
	status = sys$search(&fab);
	}

    if (status & 1)
	{
			     /* search found a real file - copy full name   */
			     /* into output buffer                          */
	resultant[nam.nam$b_rsl ] = 0;
	strcpy(tempname, resultant);
	}
    else
			     /* parse failed, or search failed, so load     */
			     /* extended file name instead                  */
	{
	extended[nam.nam$b_esl] = 0;
	strcpy(tempname, extended);
        if (nam.nam$b_name == 0)          /* if it's a directory... */
	    {
	    if ((p=strstr(tempname,".;")) != NULL)
		*p = NUL;                /* smash the ".;" */  
	    }

	}


/* Now get rid of any extraneous "][" sequences.  This is necessary if it's a 
   directory name which a process will try to set default to.
   "diskname:[dir1.][dir2]" becomes "diskname:[dir1.dir2]"		    */

    if ((p=strstr(tempname,"][")) != NULL)
	{
	*p = NUL;
	strcpy(out_str,tempname);
	strcat(out_str,p+2);
	}
    else
	strcpy(out_str,tempname);
	
			     /* release the internal context space          */
    nam.nam$b_nop = NAM$M_SYNCHK;
    nam.nam$l_rlf = 0;
    fab.fab$b_dns = 0;
    sys$parse(&fab);

    return (status & 1 ) ? SUCCESS : FAIL;

#endif

#ifdef primos

    /* translate any internal symbols */
    translations = (TCP)NULL;
    status = ho_translate_symbol (in_str, &translations);
    if (status == SUCCESS) strcpy (extended, translations[0]);
    else strcpy (extended, in_str);
    
    strcpy(out_str, extended);

#endif

#if UNIX

    CHAR	*cp, *sp;
    FILENAME	symbol;

    /* translate any internal symbols */
    translations = (TCP)NULL;
    status = ho_translate_symbol (in_str, &translations);
    if (status == SUCCESS) strcpy (extended, translations[0]);
    else strcpy (extended, in_str);
    
    /* a pathname starting with a '$' or '~' gets special processing
       otherwise just copy it to the output.				*/

    switch (*extended) {
	case '$': strcpy (symbol, extended + 1);
		  cp = strchr(symbol, ENTRYSEPERATOR);
		  if (cp != NULL) *cp = NUL;		    /* terminate the symbol */
		  sp = (CHAR *)getenv(symbol);
		  if (sp == NULL) strcpy (out_str, extended); /* no symbol defined */
		  else {
		      strcpy (out_str, sp);
		      if (cp != NULL) {
		          *cp = ENTRYSEPERATOR;		    /* put it back */
		          strcat (out_str, cp);
		      }
		  }
		  break;
		 
	case '~': strcpy (out_str, getenv("HOME"));
		  strcat (out_str, extended + 1);  /* add rest of path */
		  break;
		 
	default : strcpy (out_str, extended);
		  break;
    }
    
#endif

    return SUCCESS;
}
