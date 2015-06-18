/* DEC/CMS REPLACEMENT HISTORY, Element TS_STR_WRD.C */
/*  3     2-MAR-1992 15:23:19 JILL "(SPR 0) added proto" */
/* *2    18-MAR-1991 18:10:48 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 18:02:55 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_STR_WRD.C */
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
/*list ts_str_word
 *
 *    ts_str_word - Routine to separate words in strings
 *
 *    Calling sequence:
 *
 *    ts_str_word(image,word_ptrs,word_lengths,nwords)
 *
 *    Arguments:
 *
 *    Name         Input/  Type             Description
 *                 Output
 *    =====        ======  =============    ==========================
 *
 *    image          I     *CHAR            Input character string.
 *
 *    word_ptrs      O     **CHAR           Pointer array indicating the
 *                                          positions for each word 
 *                                          identified within the string.
 *
 *    word_lengths   O     *int[]           Lengths of words within string.
 *  
 *    nword          O     *INT             The number of words found.
 *
 *    Comments:
 *
 *    This subroutine examines an input character string image
 *    and locates words separated by blanks.  A pointer and length
 *    for each word are passed back to the calling program.
 *
 *nolist ts_str_word
 */
#include "esi_c_lib.h"
#include "esi_ctype.h"
INT ts_str_word(image,word_ptrs,word_lengths,nwords)
   CHAR *image;
   CHAR **word_ptrs;
   INT *word_lengths;
   INT *nwords;
{
   INT last_char,n,n1,n2,spaces,quote,status;
   INT nullvalue = -1;
   last_char=ts_str_trim(image);
   *nwords=0;
   n=0;
   n1 = nullvalue;
   n2 = nullvalue;
   spaces=1;
   quote=0;
   while(n++<=last_char)
   {
      if(isspace(image[n-1]))
      {
         if(spaces == 1)continue;
         if(spaces == 0 && quote == 1)continue;
      }
      else
      {
         if(image[n-1] == '"')
         {
            if(quote == 1)
            {
               quote = 0;
               spaces = 0;
            }
            else
            {
               quote = 1;
               if(spaces == 0)
               {
                  word_ptrs[*nwords-1]=image+n1;
                  word_lengths[*nwords-1]=n-1-n1;
                  n1 = nullvalue;
               }
               spaces = nullvalue;
               continue;
            }
         }
         else
            if(spaces == 0)continue;
      }
/*
 *  A field has just started or ended
 */
      if(spaces == 0)
      {
         spaces = 1;
         word_ptrs[*nwords-1]=image+n1;
         word_lengths[*nwords-1]=n-1-n1;
         n1 = nullvalue;
      }
      else
      {
         spaces = 0;
         n1 = n-1;
         (*nwords)++;
      }
   }
/*
 *  End the last field on the image
 */
   if(n1 != nullvalue)
   {
      word_ptrs[*nwords-1]=image+n1;
      word_lengths[*nwords-1]=n-1-n1;
   }
}
