/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_TOBJ.C*/
/* *4    27-JUN-1990 17:28:00 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:17:38 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"*/
/* *2     5-FEB-1990 23:34:17 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_TOBJ.C*/
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

/* ************************************************************************

   NAME:    WI_DEFINE_TEXT_OBJ

   DESCRIPTION:

    Define resident text object on the mac...

    Like dialogs and menus, this mechanism for text objects allows the
     host to define a text string as a resident ESITerm 'object'; this function
    will return an text 'id' which can be used in other functions such as
    'wi_assign_text_object' (to a specific dialog for use in a selector,
    edittext or stattext field) and 'wi_release_text_object' (recover the
    memory used for its definition, make id available again).

    INPUT:  'strings' ...... addr of Table of Character-string Pointers
                'numstrings' ... #of entries in 'strings'

    OUTPUT: 'textid' ....... where to return the text object 'id'

************************************************************************ */

#include "esi_wi_int.x"        /* external definitions required here */
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#define MC_DEFINE_TEXT 78     /* ESITerm opcode for this command  */

/* ********************************************************************** */
publicdef INT wi_define_text_object(strings, numstrings, textid)
/* ********************************************************************** */

CHAR **strings;
INT  *textid, numstrings;

    {
    PROGNAME("wi_define_text_object");
#define IDENT progname

    static CHAR nullstr[2] = "";    /* can't init an auto */

#ifdef USE_X
    E_CHECK_INIT;

    wiz_jp_name(progname);

    if (numstrings > 0)
      {
        wiv.text_id_count++;                    /* update text object id */
        *textid = wiv.text_id_count;          
        ts_tcp_append(&wiv.text_list,ts_pack_strings(numstrings, strings));
      }

#else
    CHAR *packed;

    E_CHECK_INIT;

    wiz_jp_name(progname);

    if (numstrings > 0)
        {
        packed = (CHAR *) ts_pack_strings(numstrings, strings);
        SEND_PROMPT(packed);
        tc_free(packed);
        }
    else
        {
        packed = nullstr;
        SEND_PROMPT(packed);
        }
    
    SEND_1_VALUE( MC_DEFINE_TEXT );
    
    wiv.playback_eatrcv = 1;    /* eat the returned ID from the MAC */
    wiz_set_echo(FALSE);
    wiz_gnum(textid);
    wiz_set_echo(TRUE);
    wiv.playback_eatrcv = 0;
    wiz_acknowledge();
#endif
    
    RETURN(SUCCESS); 
    }

