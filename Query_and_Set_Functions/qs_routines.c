/* DEC/CMS REPLACEMENT HISTORY, Element QS_ROUTINES.C*/
/* *3    17-AUG-1990 22:13:07 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    11-AUG-1989 13:51:23 JULIAN "(SPR -1) static variable string1,string2 unused"*/
/* *1    19-JUN-1989 13:22:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element QS_ROUTINES.C*/
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

#include "esi_qs.h"
static INT ivalues[MAXIINDEX + 1] = 
    {
    0
    };

/* integer values */
static FLOAT fvalues[MAXFINDEX + 1] = 
    {
    0.0
    };

/* float values */
static CHAR cvalues[MAXCINDEX + 1][81] = 
    {
    0
    };

/* character strings */
static BYTE *pvalues[MAXPINDEX + 1] = 
    {
    0
    };

/* pointer values */

/* File Description ---------------------------------------------------------
Overview:
    Three routines are provided for setting state variables: QS_SET_C for
    character strings; QS_SET_F for floating point values and QS_SET_I for
    integer (i*4) values.  Three routines are also provided for querying these
    values: QS_INQ_C, QS_INQ_F and QS_INQ_I.

    Calling from INFO:
                     The Prime GCI is used to call these routines.  INFO
                     users do not have to worry about call by name/call by value
                     problems.
                     Example:
                        $INSERT FINDER>INS>ESI.QS
                        PROGRAM.NAME = "TEST.TIME"
                        * EXAMPLE OF SET ROUTINES
                        XMIN = 0.0
                        IDEV = 1
                        MAP.NAME = 'BOZOS PLAYGROUND'
                        LNAME = LEN(MAP.NAME)

                        CALL $SETF(QS.XMIN.I,XMIN)
                        CALL *REPORT.GCI.ERROR(PROGRAM.NAME,"SETF")
                        CALL $SETI(QS.DEV, IDEV)
                        CALL *REPORT.GCI.ERROR(PROGRAM.NAME,"SETI")
                        CALL *SETC(QS.MAP.NAME,MAP.NAME,LNAME)
                        CALL *REPORT.GCI.ERROR(PROGRAM.NAME,"SETC")
                        ...
                        * EXAMPLE OF INQUERY ROUTINES
                        CALL *INQF(QS.XMIN.I,X)
                        CALL *REPORT.GCI.ERROR(PROGRAM.NAME,"INQF")
                        CALL *INQC(QS.MAP.NAME,MNAME, LMNAME)
                        CALL *REPORT.GCI.ERROR(PROGRAM.NAME,"INQC")

    Calling from C:  The SET routines are passed by value, and the INQ routines
                     by address.  The index values should ALWAYS be referenced
                     symbolically.  Include the file esi.qs.h in your source
                     file to get the current ESI state variable equates.
                     Example:
                           #include "esi_qs.h"

                           CHAR text[81];
                           INT tlen, itest = 1;
                           FLOAT xmin = 1.0;

                           qs_set_c(QS_AREA_NAME,text,strlen(text));
                           qs_inq_c(QS_AREA_NAME,text,&tlen);
                           qs_set_f(QS_XMIN,xmin);
                           qs_inq_f(QS_XMIN,&xmin);
                           qs_set_i(QS_TEST,itest);
                           qs_inq_i(QS_TEST,&itest);

                       NOTE: you can use zero in the length parameter of
                             qs_inq_c and qs_set_c:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef BOOL qs_set_c(INT index,CHAR *string, INT slen);
    
    publicdef BOOL qs_set_f(INT index,DOUBLE value);
    
    publicdef BOOL qs_set_i(INT index,INT value);
    
    publicdef BOOL qs_set_p(INT index,BYTE *value);
    
    publicdef BOOL qs_inq_c(INT index,CHAR *string, INT *slen);
    
    publicdef BOOL qs_inq_f(INT index,FLOAT *value);
    
    publicdef BOOL qs_inq_i(INT index,INT *value);
    
    publicdef BOOL qs_inq_p(INT index,BYTE **value);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_set_c(INT index,CHAR *string, INT slen);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    string          -(CHAR *)
    slen            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_set_c (INT index, CHAR *string, INT slen)
#else
publicdef BOOL qs_set_c (index, string, slen)
INT index;
CHAR *string;
INT slen;
#endif
    {
    INT i = MIN (slen, 80);
    if (index > MAXCINDEX || index < 0)
        return FAIL;
    
    if (i)                          /* allow 0 length (implies trailing null byte */
        {
        strncpy (cvalues[index], string, i);
        cvalues[index][i] = NUL;    /* guarantee training null byte */
        }
    else
        {
        strncpy (cvalues[index], string, 80);
        cvalues[index][80] = NUL;   /* guarantee training null byte */
        }
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_set_f(INT index,DOUBLE value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    value           -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_set_f (INT index, DOUBLE value)
#else
publicdef BOOL qs_set_f (index, value)
INT index;
DOUBLE value;
#endif
    {
    if (index > MAXFINDEX || index < 0)
        return FAIL;
    fvalues[index] = value;
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_set_i(INT index,INT value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    value           -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_set_i (INT index, INT value)
#else
publicdef BOOL qs_set_i (index, value)
INT index;
INT value;
#endif
    {
    if (index > MAXIINDEX || index < 0)
        return FAIL;
    ivalues[index] = value;
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_set_p(INT index,BYTE *value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    value           -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_set_p (INT index, BYTE *value)
#else
publicdef BOOL qs_set_p (index, value)
INT index;
BYTE *value;
#endif
    {
    if (index > MAXPINDEX || index < 0)
        return FAIL;
    pvalues[index] = value;
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_inq_c(INT index,CHAR *string, INT *slen);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    string          -(CHAR *)
    slen            -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_inq_c (INT index, CHAR *string, INT *slen)
#else
publicdef BOOL qs_inq_c (index, string, slen)
INT index;
CHAR *string;
INT *slen;
#endif
    {
    INT len;
    
    if (index > MAXCINDEX || index < 0)
        return FAIL;
    strcpy (string, cvalues[index]);
    if (slen != (INT *)0)
        *slen = strlen (string);    /* if no pointer to length passed, don't use */
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_inq_f(INT index,FLOAT *value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    value           -(FLOAT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_inq_f (INT index, FLOAT *value)
#else
publicdef BOOL qs_inq_f (index, value)
INT index;
FLOAT *value;
#endif
    {
    if (index > MAXFINDEX || index < 0)
        return FAIL;
    *value = fvalues[index];
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_inq_i(INT index,INT *value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    value           -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_inq_i (INT index, INT *value)
#else
publicdef BOOL qs_inq_i (index, value)
INT index;
INT *value;
#endif
    {
    if (index > MAXIINDEX || index < 0)
        return FAIL;
    *value = ivalues[index];
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef BOOL qs_inq_p(INT index,BYTE **value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    index           -(INT)
    value           -(BYTE **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL qs_inq_p (INT index, BYTE **value)
#else
publicdef BOOL qs_inq_p (index, value)
INT index;
BYTE **value;
#endif
    {
    if (index > MAXPINDEX || index < 0)
        return FAIL;
    *value = pvalues[index];
    return SUCCESS;
    
    }
/* END:     */
