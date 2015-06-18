/* DEC/CMS REPLACEMENT HISTORY, Element HO_Q_OFFSET.C*/
/* *2    17-AUG-1990 21:53:00 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:59:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_Q_OFFSET.C*/
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

#include "esi_ho.h"
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    This help routine is designed to count the offset of a CHAR string in the
    PRIME file and the data length should read back from PRIME file by FGETC
    routine.

Prototype:
    publicdef INT ho_query_offset_length (CHAR s[], INT *offset, INT *data_len);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) s           -(CHAR []) The CHAR string given.
    (O) offset      -(INT *) The offset value returned.
    (O) data_len    -(INT *) The read back charactor count( by FGETC() routine).

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_query_offset_length (CHAR s[], INT *offset, INT *data_len) 
#else
publicdef INT ho_query_offset_length (s, offset, data_len)
INT *offset;
INT *data_len;
CHAR s[];
#endif
    {

#if UNIX
    *offset = *data_len = strlen (s);   /* a NO-OP for UNIX */
#else       /* not UNIX */

#ifdef vms
    *offset = *data_len = strlen (s);   /* ditto fo VMS */
#endif

#ifdef primos
    INT i;
    INT previous_blank = FALSE;
    INT count_for_offset;
    
    /* start at begin until NUL or <CR>     */
    for (i = 0, count_for_offset = 0; s[i] != '\0' && s[i] != '\n'; i++)
        {
        /* if there are more than one blanks, ' ', 
           conpress the multiple blanks as
            PRIMOS does.   */
        
        if (s[i] == ' ' && previous_blank)
            {
            count_for_offset++;
            while (s[i] == ' ')
                {
                i++;
                if (s[i] == '\0')
                    break;
                else if (s[i] != ' ')
                    {
                    previous_blank = FALSE;
                    count_for_offset++;
                    }
                }
            }
        else                        /* the charactor is ' ', set the flag 
                                        and increses count.    */
            if (s[i] == ' ')
                {
                count_for_offset++;
                previous_blank = TRUE;
                }
            else                    /* the charactor is not ' ', 
                                       increases the count.     */
                {
                previous_blank = FALSE;
                count_for_offset += 1;
                }
        }
    /* end of for loop */
    /* increases the count for '\n'
               and take care the word boundary. */
    *offset = (count_for_offset % 2) ? count_for_offset + 1 : count_for_offset + 2;
    *data_len = (i == 0) ? 1 : i + 1;

#endif      /* PRIMOS */
#endif      /* UNIX */
    return SUCCESS;
    }
/* END:     */
