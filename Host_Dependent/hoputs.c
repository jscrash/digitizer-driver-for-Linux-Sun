/* DEC/CMS REPLACEMENT HISTORY, Element HOPUTS.C*/
/* *4    17-AUG-1990 21:49:50 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-AUG-1990 13:30:16 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-SEP-1989 18:21:51 VINCE "(SPR -1) added io redirection"*/
/* *1    19-JUN-1989 12:56:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HOPUTS.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifdef vms

#include "esi_qs.h"
#include "jpidef.h"
#include "syidef.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Send a string to the standard output (terminal for interactive;
    log file for batch).

Prototype:
    publicdef INT hoputs (VOIDPTR lineout, UINT slen);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) lineout     -(CHAR *) Line buffer.
    (I) slen        -(UINT) # of CHARs in line buffer.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoputs (VOIDPTR lineout, UINT slen) 
#else
publicdef INT hoputs (lineout, slen)
VOIDPTR lineout;
UINT slen;
#endif
    {                               /* start of HOPUTS */
    INT status = SUCCESS;

#if UNIX
    /* use FPUTS to output the string UNCHANGED (EOF ret'd for failures) */
    if (slen > 0)
        {
        if (fputs ((char *)lineout, stdout) == EOF)
            {
            status = FAIL;
            }
        }
#else

#ifdef primos
    SHORT short_len = slen;
    fortran tnoua ();
    
    if (slen > 0)
        {
        tnoua ((char [])lineout, short_len);
        }
#endif

#ifdef vms
    static BOOL first = TRUE;
    static INT jtrmchn;
    static BOOL batch;
    static INT maxlen;
    
    INT len_left;
    BYTE *pb;
    CHAR mac_device[31];
    
    if (first)
        {
        INT maxbuf_code = SYI$_MAXBUF;
        CHAR mode[81];
        
        /*  if this is a batch job, use LIB$PUT_OUTPUT
            if this is interactive, use hobsend for
            ESITERM determine this by calling getjpi */
        
        qs_inq_c (QS_WORKSTATION_MAC_DEVICE, mac_device, (INT *)0);
        if (strlen (mac_device) == 0)
            strcpy (mac_device, "TT:");
        
        status = hoopenio (&jtrmchn, mac_device);
        
        hoz_getjpi (JPI$_MODE, mode, sizeof mode, (INT *)0);
        batch = strcmp (mode, "BATCH") EQUALS 0;
        
        /* inquire the sysgen parameter MAXBUF... */
        VAX_ROUTINE (LIB$GETSYI (&maxbuf_code, &maxlen, 0, 0, 0, 0));
        
        first = FALSE;
        }
    if (batch)
        {
        printf ("%s", lineout);     /* only way to put out w/out cr/lf */
        }
    else
        {                           /* send as many maxlen bytes as possible */
        for (len_left = slen, pb = (BYTE *)lineout; len_left > maxlen;
                                                                                   len_left
                                                                                    -= 
                                                                                   maxlen
                                                                                   , pb 
                                                                                   += 
                                                                                   maxlen
                                                                                   )
            {
            hobsend (jtrmchn, maxlen, pb);
            }
        if (len_left > 0)           /* flushes any leftover bytes */
            {
            hobsend (jtrmchn, len_left, pb);
            }
        }
#endif
#endif      /* UNIX */
    return status;
    }
/* END:     */
