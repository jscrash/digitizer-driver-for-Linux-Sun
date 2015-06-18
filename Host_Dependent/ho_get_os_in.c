/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_OS_IN.C*/
/* *5     4-JAN-1991 10:43:32 GILLESPIE "(SPR 1) More general routine for establishing original environment"*/
/* *4    23-AUG-1990 16:13:05 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *3    17-AUG-1990 21:52:14 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    24-SEP-1989 23:43:56 GILLESPIE "(SPR 103) Fix up call a bit to match usage,prototype"*/
/* *1    19-JUN-1989 12:58:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_OS_IN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_OS_IN.C                              */
/* *2    24-SEP-1989 23:43:56 GILLESPIE "(SPR 103) Fix up call a bit to match usage,prototype"  */
/* *1    19-JUN-1989 12:58:54 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_OS_IN.C                              */

/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Exploration Systems, Inc.                                               */
/*          579 Market Street                                                       */
/*          San Francisco, CA  USA 94105                                            */
/*          (415) 974-1600                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* HO_GET_OS_INFO.C                                                                 */
/* HISTORY:                                                                         */
/*   05/05/89 Gordon  Created                                                       */
/*                                                                                  */

#include "esi_ho.h"

/****************************************************************************       */
/**                                                                                 */
/**  FUNCTION: ho_get_os_info                                                       */
/**                                                                                 */
/**  DESCRIPTION: Fill an array of 8 chars with specific os info. This array        */
/**      currently corresponds to the array "os" within NLIST_CONTROL_RECORD.       */
/**                                                                                 */
/****************************************************************************       */

#include "esi_ho.h"

#if USE_PROTOTYPES
publicdef INT ho_get_os_info (CHAR os_info[8])
#else
publicdef INT ho_get_os_info (os_info)
CHAR os_info[8];
#endif
    {
	union {
		short short_int;
		unsigned char bytes[2];
		} test;
    
    /* This routine is HOST SPECIFIC.  It is intended to allow each machine to      */
    /*write an n-List into a native format, and to stamp information into the file  */
    /*that will allow network access, even to the extent that a file written on     */
    /*one machine can be cracked on another.                                        */
    /*The entries in this structure will enable a translator routine to decide      */
    /*what transformations will be necessary to read the file.  Note that ALL entries   */
    /*in a file should follow these conventions.  To this end, it may be desirable  */
    /*to have files not written by the local OS to be read-only files...            */
    /*                                                                              */
    /*    os_info[0] - CHAR_TYPE  (0 = ASCII; 1 = EBCDIC; 2 = ASCII w/high bit on)  */
    /*    os_info[1] - INT_TYPE   (0 = big-endian; 1 = little-endian)               */
    /*    os_info[2] - FLOAT_TYPE (Floating point format: 0 = VAX; 1 = IEEE)        */
    /*    os_info[3] - PTR_SIZE;  (size of a pointer, in bytes)                     */
    /*    os_info[4] - CHAR_SIZE  (1 = byte-oriented; 2 = word oriented)            */
    /*    os_info[5] - INT_SIZE (size of INT)                                       */
    /*    os_info[6] - DOUBLE_SIZE (size of DOUBLE)                                 */
    /*    os_info[7] - UNUSED                                                       */
    /*                                                                              */

    os_info[0] = 'A' == 61;         /* test for ASCII                               */
	test.short_int = 1;
	os_info[1] = test.bytes[0] > test.bytes[1];
    os_info[3] = sizeof(char *);    /* pointer_size                                 */
    os_info[4] = sizeof(char);      /* char_size                                    */
    os_info[5] = sizeof(INT);       /* INT size                                     */
    os_info[6] = sizeof(DOUBLE);    /* DOUBLE size                                  */
    os_info[7] = 0;                 /* unused                                       */
#ifdef vms
    os_info[2] = 0;                 /* float_type                                   */
#endif

#ifdef primos
    os_info[0] = 2;                 /* char_type                                    */
    os_info[2] = 0;                 /* float_type                                   */

#endif

#ifdef mips
    os_info[2] = 1;                 /* float_type                                   */
#endif

#ifdef sun
    os_info[2] = 1;                 /* float_type                                   */
#endif
    return SUCCESS;
    }
