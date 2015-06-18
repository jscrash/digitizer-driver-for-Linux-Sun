/* DEC/CMS REPLACEMENT HISTORY, Element TS_STR_ALPH.C */
/*  3     2-MAR-1992 15:20:29 JILL "(SPR 0) added proto" */
/* *2    18-MAR-1991 18:10:15 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 18:00:58 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_STR_ALPH.C */
/************************************************************************


                  Copyright Gulf Canada Resources, 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
WITHOUT PRIOR WRITTEN CONSENT OF

			Gulf Canada Resources, Inc.
			401 9th Avenue SW
			Calgary, Alberta
                        Canada T2P 2H7
			(403) 233-4000

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)
************************************************************************/
/* list ts_str_alpha
 *
 *     **Routine to find alph and numeric fields within a string ***
 *
 *     Calling sequence:
 *
 *     ts_str_alpha(string,field_ptrs,field_lengths,field_types,nfields)
 *
 *     Arguments:
 *
 *     Name           Input/     Type          Description
 *                    Output
 *
 *     string           I        *CHAR          Input character string.
 *
 *     field_ptrs       O       **CHAR          Field pointers
 *
 *     field_lengths    O        *INT           Field lengths 
 *
 *     field_types      O        *INT           Field types: -1=alphabetic
 *                                                            1=numeric
 *
 *     nfields          O        *INT           Number of fields found.
 *
 *     Comments:
 *
 *     This function identifies alpha and numeric substrings within
 *     the input string and returns start position and length which can
 *     be passed directly to c functions.  For example;
 *
 *     strncpy(substring,field_ptrs[j],field_lengths[j]);
 *     substring[field_lengths[j]]='\0';
 *
 *     would give you a null terminated substring of the jth field.
 *
 * nolist ts_str_alpha */

#include "esi_c_lib.h"
#include "esi_ctype.h"
void /*FUNCTION*/ ts_str_alpha(string, field_ptrs, field_lengths, field_types, nfields)
CHAR *string;
CHAR **field_ptrs;
INT *field_lengths;
INT *field_types;
INT *nfields;
{
INT i, this_field, last_field, start_posn, start_next,end_posn, nchar;
#define NCHRTMPS 2
#define ALPHA -1
#define BLANK 0
#define NUMERIC 1

	*nfields = 0;
	nchar = ts_str_trim( string );
	if( nchar == 0 ) return;
	last_field = BLANK;
	start_posn = 0;
	for( i = 0 ; i <= nchar; i++ )
        {
           /*
            *      Check to see if the current character is alphabetic
            *      or numeric.  Treat periods as decimal places if the
            *      current field type is numeric
            */
           if( i < nchar )
           {
	       this_field = ALPHA;
	       if( last_field == NUMERIC  && string[i] == '.' )
		  this_field = NUMERIC;
               if( isdigit(string[i]) )
                  this_field = NUMERIC;
               if( string[i] == ' ')
                  this_field = BLANK;
           }
           else
           {
               this_field = BLANK;
           }
		if( this_field != last_field )
                /*
                 *     The field type has changed 
		 */
                 {
                    if( last_field == BLANK)
                    {
                       (*nfields)++;
                       start_posn = i;
                    }
                    else
                    /*
                     *    Finish off the last field - treat leading dashes as 
                     *    minus signs if the next field is numeric
                     */
                    {
                       end_posn = i - 1;
                       start_next = i;
                       if( this_field == NUMERIC && string[end_posn] == '-')
                       {
                          end_posn -= 1;
                          start_next -= 1;
                       }
                       /*
                        *   Store pointers for the last field
                        */
                       field_ptrs[*nfields-1] = string + start_posn;
                       field_lengths[*nfields-1] = end_posn - start_posn +1;
                       field_types[*nfields-1] = last_field;
                       if( this_field != BLANK)
                       {
                          (*nfields)++;
                          start_posn = start_next;
                       }
                       else
                       /*
                        * Retain embedded blanks in an alphabetic field.
                        * These can be parsed out using ts_str_word if desired.
                        */
                          if( last_field == ALPHA ) this_field = ALPHA;
                    }
                    last_field = this_field;
                 }
        }
} /* end of function */
