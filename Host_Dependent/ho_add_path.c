/* DEC/CMS REPLACEMENT HISTORY, Element HO_ADD_PATH.C*/
/* *7    25-OCT-1991 14:40:21 LOEBL "(SPR 4669) Ignore path if filename contains a logical or diskname"*/
/* *6    23-AUG-1990 16:13:01 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *5    23-JUL-1990 11:46:25 VINCE "(SPR 1) final fixes for recursive logical names on Unix"*/
/* *4     1-NOV-1989 10:12:22 GILLESPIE "(SPR 30) Adjust for include file changes; add pragmas to HO routines"*/
/* *3    27-OCT-1989 11:35:49 GILLESPIE "(SPR 1) Fix args to match prototype"*/
/* *2     5-SEP-1989 15:46:56 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *1    19-JUN-1989 12:56:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ADD_PATH.C*/
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

NAME:   HO_ADD_PATH

DESCRIPTION:

    HO_ADD_PATH accepts a two components of a file specification, and
    constructs a resulting file specification which can then be used to
    access operating system files.

    The first component of the file specification is a logical which
    may or may not be defined (defined as with the VMS/DCL DEFINE or
    ASSIGN commands).

    The second component is a file specification, containing a filename
    and a file type, but excluding all other components of a valid
    operating system file specification. Specifically, this file
    specification should not contain any node, device, or directory
    components.

    HO_ADD_PATH attempts to translate the logical which may or may
    not define a directory. If the logical can be translated, then
    HO_ADD_PATH uses it in the construction of the final full
    file specification. If the logical cannot be translated, then
    HO_ADD_PATH ignores it (considers it to be null) and reports this
    fact to the caller by returning the status HO_NO_SUCH_PATH.

    If the directory logical can be translated, it is prepended to
    the second argument/component, the two components being seperated
    by a colon ":" (for VMS only). The full file specification is
    then copied into the users "full_filename" buffer, provided that
    the "maxchars" argument indicates enough room.

    HO_ADD_PATH should be used whenever the location of a file can
    be, but need not be, determined by the definition of a logical.
    Such files are located, by default (logical undefined), in the
    user's (and thus the process's) current default directory. Should
    the specified logical name be defined, then it is incorporated
    into the constructed full file specification.

CALLING SEQUENCE:

    ho_add_path(logical, filename, maxchars, full_filename)

    logical = (CST) logical name of directory (in)
    filename   = (CST) name of file (in)
    maxchars = (INT) size of buffer for full_filename (in)
    full_filename  = (CST) constructed filename (out)

    Maxchars length must include room for the trailing zero byte.

EXAMPLE 1:

    For example, consider a logical ABC$STUFF, and a file named
    xyzfile.abc. Suppose that the logical ABC$STUFF is not defined.
    Consider the following call to HO_ADD_PATH.

	INT status;
	char s[255];
	status = ho_add_path("ABC$STUFF","xyzfile.abc",sizeof s,s);

    Because the logical was not defined, the resulting string s would
    contain "xyzfile.abc", and the status returned would be
    HO_NO_SUCH_PATH. File operations would therefore be done in the
    current default directory.

EXAMPLE 2:

    For example, consider a logical ABC$STUFF, and a file named
    xyzfile.abc. Suppose that the logical ABC$STUFF had previously
    been defined via the DCL command:

	$ DEFINE ABC$STUFF DUA0:[BILL.STUFF]

    Consider the following call to HO_ADD_PATH.

	INT status;
	char s[255];
	status = ho_add_path("ABC$STUFF","xyzfile.abc",sizeof s,s);

    In this case, the resulting string s would contain the file
    specification "ABC$STUFF:xyzfile.abc". Notice that HO_ADD_PATH does
    not bother to actually translate the logical into the resultant
    string, since the operating system will do this when a reference
    to the file is made.

*************************************************************************/


#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_HO_ERR_H
#include "esi_ho_err.h"
#endif


/************************************************************************/
publicdef INT ho_add_path(logical, filename, maxchars, full_filename)
/************************************************************************/

CHAR *logical;                  /* path name */
CHAR *filename;                 /* file name */
size_t maxchars;                /* size of full_filename */
CHAR *full_filename;            /* constructed file plus path */
{
#ifndef vms
    INT status;
    PATHNAME path_buf;			/* temporary pathname storage */
    CHAR **tcp;				/* table of pointers to paths */
    CHAR **pp;

    status = HO_NO_SUCH_PATH;
    if (ho_translate_symbol(logical,&tcp) == HO_ERR_NO_SYMBOL) 
	{
	strcpy(path_buf,filename);

	/* expand any symbolic name in the filename */
	ho_translate_filename (path_buf, full_filename);

	return status;
        }

    status = SUCCESS;
    for (pp=tcp; *pp; ++pp) 
	{
	sprintf( path_buf, "%s%c%s", *pp, ENTRYSEPERATOR, filename );
	if (hoexists( path_buf ) == TRUE) 
	    break;
        }

    if (!*pp)         /* we didn't find one so use the first one in list */
	sprintf( path_buf, "%s%c%s", *tcp, ENTRYSEPERATOR, filename );

    if (maxchars <= strlen(path_buf)) {
	path_buf[maxchars-1] = NUL;	/* truncate to the callers length */
	status = HO_STRING_TOO_LONG;	/* and let him know		  */
    }

    /* expand any symbolic name in the filename */
    ho_translate_filename (path_buf, full_filename);

    return status;

#else

/******  VMS SECTION ********/

    CHAR constructed_filename[255];     /* working file plus path */
    CHAR **logical_translated = (CHAR**)0;       /* junk */
    INT status;                         /* completion status */

/************************************************************************/

    /* look for a : in filename and dont process if it is there */

    if ( ts_search ( filename, ":", TRUE ) != NULL )
        {
        /* Since the filename contains a ":", then the filename 
           probably contains a logical and the input
           assumptions have been violated. So return the filename unchanged */

        if (strlen(filename) > maxchars-1)
           {
           return HO_STRING_TOO_LONG;
           }
        else
           {
           strcpy(full_filename, filename );
           return SUCCESS;
           }
        }
        
				/*  See if logical is defined. */

    status = ho_translate_symbol(logical,&logical_translated);
    ts_tcp_free(logical_translated );
				/*  If not, indicate logical not defined. */

    if (status == HO_ERR_NO_SYMBOL)
	status = HO_NO_SUCH_PATH;

				/*  If nothing strange happened while
				    translating the logical ... */

    if ((status == SUCCESS) || (status == HO_NO_SUCH_PATH)) 
        {
      				/*  Initialize constructed file name. */

	constructed_filename[0] = '\0';

				/*  If logical was defined, insert it
				    into the constructed file name. */

	if (status == SUCCESS) 
            {
	    strcpy(constructed_filename,logical);

				/*  Append a colon if the logical doesn't
				    already have one. */

	    if (logical[strlen(logical)] != ':')
		strcat(constructed_filename,":");
	    }

				/*  Append file name to final version. */

	strcat(constructed_filename,filename);
	}

				/*  If constructed file name is too long
				    for users buffer, indicate grief. */

    if (strlen(constructed_filename) > maxchars-1)
	status = HO_STRING_TOO_LONG;

				/*  If nothing went wrong, then copy
				    constructed file name to user's
				    buffer. */

    if ((status == SUCCESS) || (status == HO_NO_SUCH_PATH))
	strcpy(full_filename,constructed_filename);

    return status;

#endif
}
