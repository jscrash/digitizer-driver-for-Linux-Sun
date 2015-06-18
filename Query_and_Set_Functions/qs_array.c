/* DEC/CMS REPLACEMENT HISTORY, Element QS_ARRAY.C*/
/* *2    17-AUG-1990 22:12:25 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:22:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element QS_ARRAY.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* Set integer into an array */

#include "esi_c_lib.h" 
#include "esi_qs_err.h"
#include "esi_qs.h"
#include "esi_tc.h"

/* array of number of items allocated for each element of the qs_arrays */
static INT numi_allocated[MAXIINDEX + 1] = 
    {
    0
    };



/* array of pointers to allocated memory  */
static INT *qs_int_arrays[MAXIINDEX + 1] = 
    {
    0
    };



/* array of number of items allocated for each element of the qs_arrays */
static INT numf_allocated[MAXFINDEX + 1] = 
    {
    0
    };



/* array of pointers to allocated memory  */
static FLOAT *qs_float_arrays[MAXFINDEX + 1] = 
    {
    0
    };



/* array of number of items allocated for each element of the qs_arrays */
static INT numd_allocated[MAXDINDEX + 1] = 
    {
    0
    };



/* array of pointers to allocated memory  */
static DOUBLE *qs_double_arrays[MAXDINDEX + 1] = 
    {
    0
    };



/* array of number of items allocated for each element of the qs_arrays */
static INT nump_allocated[MAXPINDEX + 1] = 
    {
    0
    };



/* array of pointers to allocated memory  */
static BYTE *qs_point_arrays[MAXPINDEX + 1][MAXPINDEX + 1] = 
    {
    0
    };



/* array of number of items allocated for each element of the qs_arrays */
static INT numc_allocated[MAXCINDEX + 1] = 
    {
    0
    };



/* array of table of characters  */
static CHAR **qs_char_arrays[MAXCINDEX + 1] = 
    {
    0
    };



/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT qs_set_array_i( INT array_id, INT index, INT value );
    
    publicdef INT qs_inq_array_max(INT array_type,INT array_id, INT *max_num );
    
    publicdef VOID qs_term();
    
    publicdef INT qs_inq_array_c(INT array_id, INT index, CHAR value[], INT *slen );
    
    publicdef INT qs_set_array_c(INT array_id, INT index, CHAR *value, INT slen );
    
    publicdef INT qs_inq_array_p(INT array_id, INT index, BYTE **value );
    
    publicdef INT qs_set_array_p(INT array_id, INT index, BYTE *value );
    
    publicdef INT qs_inq_array_d(INT array_id, INT index, DOUBLE *value );
    
    publicdef INT qs_set_array_d(INT array_id, INT index, DOUBLE value );
    
    publicdef INT qs_inq_array_f(INT array_id, INT index, FLOAT *value );
    
    publicdef INT qs_set_array_f(INT array_id, INT index, DOUBLE value );
    
    publicdef INT qs_inq_array_i(INT array_id, INT index, INT *value );

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This routine allocates memory and sets an integer value into the 
    memory. It allows arrays of integers to be shared between 
    applications.

Prototype:
    publicdef INT qs_set_array_i( INT array_id, INT index, INT value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Integer index into the array.
    value           -(INT) Integer to load into array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_NO_MEMORY
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_set_array_i (INT array_id, INT index, INT value)
#else
publicdef INT qs_set_array_i (array_id, index, value)
INT array_id;
INT index;
INT value;
#endif
    {
    unsigned num_bytes;             /* number of cells of memory to allocate */
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXIINDEX))
        {
        return QS_BAD_QS_ID;
        }
    if (index < 0)
        {
        return QS_OUTSIDE_RANGE;
        }
    num_bytes = (index + 1) * sizeof(INT);
    
    /* If no space has been allocated before then allocate enough for this
        set call.  */
    
    if (numi_allocated[array_id] EQUALS 0)
        {
        qs_int_arrays[array_id] = (INT *)tc_zalloc (num_bytes);
        }
    /* If not enough space has been allocated before, then reallocate */
    
    else if (numi_allocated[array_id] < index)
        {
        qs_int_arrays[array_id] = (INT *)tc_realloc (qs_int_arrays[array_id],
                                                     num_bytes);
        }
    /* Test if enough memory could be allocated */
    
    if (qs_int_arrays[array_id] == (INT *)NULL)
        {
        return QS_NO_MEMORY;
        }
    if (numi_allocated[array_id] < index)
        {
        numi_allocated[array_id] = index;
        }
    *(qs_int_arrays[array_id] + index) = value;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine reads an integer value from dynamically allocated 
    memory. It allows arrays of integers to be shared between 
    applications.

Prototype:
    publicdef INT qs_inq_array_i(INT array_id, INT index, INT *value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Integer index into the array.
    value           -(INT *) Integer to load from array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_ERR_NEVER_SET
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_inq_array_i (INT array_id, INT index, INT *value)
#else
publicdef INT qs_inq_array_i (array_id, index, value)
INT array_id;
INT index;
INT *value;
#endif
    {
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXIINDEX))
        {
        return QS_BAD_QS_ID;
        }
    if ((index > numi_allocated[array_id]) OR (index < 0))
        {
        return QS_OUTSIDE_RANGE;
        }
    if ((qs_int_arrays[array_id] + index) EQUALS (INT *)NULL)
        {
        return QS_ERR_NEVER_SET;
        }
    *value = *(qs_int_arrays[array_id] + index);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine allocates memory and sets a float value into the 
    memory. It allows arrays of floats to be shared between 
    applications.

Prototype:
    publicdef INT qs_set_array_f(INT array_id, INT index, FLOAT value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Index into the array.
    value           -(DOUBLE) Float to load into array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_NO_MEMORY
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_set_array_f (INT array_id, INT index, DOUBLE value)
#else
publicdef INT qs_set_array_f (array_id, index, value)
INT array_id;
INT index;
DOUBLE value;
#endif
    {
    unsigned num_bytes;             /* number of cells of memory to allocate */
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXFINDEX))
        {
        return QS_BAD_QS_ID;
        }
    if (index < 0)
        {
        return QS_OUTSIDE_RANGE;
        }
    num_bytes = (index + 1) * sizeof(FLOAT);
    
    /* If no space has been allocated before then allocate enough for this
        set call.  */
    
    if (numf_allocated[array_id] EQUALS 0)
        {
        qs_float_arrays[array_id] = (FLOAT *)tc_zalloc (num_bytes);
        }
    /* If not enough space has been allocated before, then reallocate */
    
    else if (numf_allocated[array_id] < index)
        {
        qs_float_arrays[array_id] = (FLOAT *)tc_realloc (qs_float_arrays[array_id],
                                                         num_bytes);
        }
    /* Test if enough memory could be allocated */
    
    if (qs_float_arrays[array_id] == (FLOAT *)NULL)
        {
        return QS_NO_MEMORY;
        }
    if (numf_allocated[array_id] < index)
        {
        numf_allocated[array_id] = index;
        }
    *(qs_float_arrays[array_id] + index) = value;
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine reads a float value from dynamically allocated 
    memory. It allows arrays of floats to be shared between 
    applications.

Prototype:
    publicdef INT qs_inq_array_f(INT array_id, INT index, FLOAT *value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Integer index into the array.
    value           -(FLOAT *) Float to load from array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_ERR_NEVER_SET
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_inq_array_f (INT array_id, INT index, FLOAT *value)
#else
publicdef INT qs_inq_array_f (array_id, index, value)
INT array_id;
INT index;
FLOAT *value;
#endif
    {
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXFINDEX))
        {
        return QS_BAD_QS_ID;
        }
    if ((index > numf_allocated[array_id]) OR (index < 0))
        {
        return QS_OUTSIDE_RANGE;
        }
    if ((qs_float_arrays[array_id] + index) EQUALS (FLOAT *)NULL)
        {
        return QS_ERR_NEVER_SET;
        }
    *value = *(qs_float_arrays[array_id] + index);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine allocates memory and sets a double value into the 
    memory. It allows arrays of doubles to be shared between 
    applications.

Prototype:
    publicdef INT qs_set_array_d(INT array_id, INT index, DOUBLE value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Index into the array.
    value           -(DOUBLE) Double to load into array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_NO_MEMORY
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_set_array_d (INT array_id, INT index, DOUBLE value)
#else
publicdef INT qs_set_array_d (array_id, index, value)
INT array_id;
INT index;
DOUBLE value;
#endif
    {
    unsigned num_bytes;             /* number of cells of memory to allocate */
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXDINDEX))
        {
        return QS_BAD_QS_ID;
        }
    if (index < 0)
        {
        return QS_OUTSIDE_RANGE;
        }
    num_bytes = (index + 1) * sizeof(DOUBLE);
    
    /* If no space has been allocated before then allocate enough for this
        set call.  */
    
    if (numd_allocated[array_id] EQUALS 0)
        {
        qs_double_arrays[array_id] = (DOUBLE *)tc_zalloc (num_bytes);
        }
    /* If not enough space has been allocated before, then reallocate */
    
    else if (numd_allocated[array_id] < index)
        {
        qs_double_arrays[array_id] = (DOUBLE *)tc_realloc (qs_double_arrays[array_id],
                                                           num_bytes);
        }
    /* Test if enough memory could be allocated */
    
    if (qs_double_arrays[array_id] EQUALS (DOUBLE *)NULL)
        {
        return QS_NO_MEMORY;
        }
    if (numd_allocated[array_id] < index)
        {
        numd_allocated[array_id] = index;
        }
    *(qs_double_arrays[array_id] + index) = value;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine reads a double value from dynamically allocated 
    memory. It allows arrays of doubles to be shared between 
    applications.

Prototype:
    publicdef INT qs_inq_array_d(INT array_id, INT index, DOUBLE *value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Integer index into the array.
    value           -(DOUBLE *) Double to load from array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_ERR_NEVER_SET
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_inq_array_d (INT array_id, INT index, DOUBLE *value)
#else
publicdef INT qs_inq_array_d (array_id, index, value)
INT array_id;
INT index;
DOUBLE *value;
#endif
    {
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXDINDEX))
        {
        return QS_BAD_QS_ID;
        }
    if ((index > numd_allocated[array_id]) OR (index < 0))
        {
        return QS_OUTSIDE_RANGE;
        }
    if ((qs_double_arrays[array_id] + index) EQUALS (DOUBLE *)NULL)
        {
        return QS_ERR_NEVER_SET;
        }
    *value = *(qs_double_arrays[array_id] + index);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine allocates memory and sets a double value into the 
    memory. It allows arrays of doubles to be shared between 
    applications.

Prototype:
    publicdef INT qs_set_array_p(INT array_id, INT index, BYTE *value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Index into the array.
    value           -(BYTE *) Pointer to load into array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_NO_MEMORY
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_set_array_p (INT array_id, INT index, BYTE *value)
#else
publicdef INT qs_set_array_p (array_id, index, value)
INT array_id;
INT index;
BYTE *value;
#endif
    {
    
    /* Test if the QS ID is valid  */
    if ((array_id < 0) || (array_id > MAXPINDEX))
        return(QS_BAD_QS_ID);
    
    /* Test if the index is positive */
    if ((index < 0) || (index > MAXPINDEX))
        return(QS_OUTSIDE_RANGE);
    
    /* if no memory has been allocated, then allocate 
         Caution: triple indirection seems to behave differently on different
         C compilers. So let's avoid it if possible.
    
        if (qs_point_arrays[ array_id ] == (BYTE *)NULL )
        {
            *qs_point_arrays[ array_id ] = 
                (BYTE *)tc_zalloc( MAXPINDEX * sizeof( BYTE *) );
            
            if (*qs_point_arrays[ array_id ] == (BYTE *)NULL)
                return( QS_NO_MEMORY );
        }
    
    */
    
    /* To reach this point, enough memory must have already been allocated 
          by the application */
    
    if (nump_allocated[array_id] < index)
        nump_allocated[array_id] = index;
    
    qs_point_arrays[array_id][index] = value;
    return(SUCCESS);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine reads a double value from dynamically allocated 
    memory. It allows arrays of doubles to be shared between 
    applications.

Prototype:
    publicdef INT qs_inq_array_p(INT array_id, INT index, BYTE **value );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Integer index into the array.
    value           -(BYTE **) Pointer to load from array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_inq_array_p (INT array_id, INT index, BYTE **value)
#else
publicdef INT qs_inq_array_p (array_id, index, value)
INT array_id;
INT index;
BYTE **value;
#endif
    {
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) || (array_id > MAXPINDEX))
        {
        return(QS_BAD_QS_ID);
        }
    if ((index > MAXPINDEX) || (index < 0))
        {
        return(QS_OUTSIDE_RANGE);
        }
    if (qs_point_arrays[array_id][index] == (BYTE *)NULL)
        return(QS_ERR_NEVER_SET);
    
    *value = qs_point_arrays[array_id][index];
    
    return(SUCCESS);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine allocates memory and sets a character string into the 
    memory. It allows arrays of strings to be shared between 
    applications.

Prototype:
    publicdef INT qs_set_array_c(INT array_id, INT index, CHAR *value, INT slen );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Index into the array.
    value           -(CHAR *) String to load into array.
    slen            -(INT) Optional length of string.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_NO_MEMORY
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_set_array_c (INT array_id, INT index, CHAR *value, INT slen)
#else
publicdef INT qs_set_array_c (array_id, index, value, slen)
INT array_id;
INT index;
CHAR *value;
INT slen;
#endif
    {
    unsigned num_bytes;             /* number of cells of memory to allocate */
    INT length;                     /* used for the length of the tcp */
    
    /* Test if the QS ID is valid  */
    
    if ((array_id < 0) OR (array_id > MAXCINDEX))
        {
        return QS_BAD_QS_ID;
        }
    /* Test if the index is positive or larger than MAXCINDEX*/
    
    if ((index < 0) OR (index > MAXCINDEX))
        {
        return QS_OUTSIDE_RANGE;
        }
    /* If no space has been allocated for the array of table of character
        pointers then allocate */
    
    if (qs_char_arrays[array_id] EQUALS (CHAR **)NULL)
        {
        qs_char_arrays[array_id] = (CHAR **)tc_zalloc ((MAXCINDEX + 1) * sizeof(CHAR *)
                                                       );
        if (qs_char_arrays[array_id] EQUALS (CHAR **)NULL)
            {
            return QS_NO_MEMORY;
            }
        }
    /* If a length is given, then only that many bytes will be stored, 
        otherwise the entire string will be stored. If no length is given,
        then the string is assumed to be null terminated. */
    
    num_bytes = (slen > 0) ? slen : strlen (value);
    
    /* If no space has been allocated before then allocate enough for this
        set call.  */
    
    if (qs_char_arrays[array_id][index] EQUALS (CHAR *)NULL)
        {
        qs_char_arrays[array_id][index] = (CHAR *)tc_zalloc ((num_bytes + 1) *
                                                             sizeof(CHAR));
        }
    /* If not enough space has been allocated before, then reallocate */
    
    else if (num_bytes > strlen (qs_char_arrays[array_id][index]))
        {
        qs_char_arrays[array_id][index] = (CHAR *)
                                          tc_realloc (qs_char_arrays[array_id][index],
                                                      (num_bytes + 1) * sizeof(CHAR));
        }
    /* Test if enough memory could be allocated. If not then error return */
    
    if (qs_char_arrays[array_id][index] == (CHAR *)NULL)
        {
        return(QS_NO_MEMORY);
        }
    /* To reach this point, enough memory must have already been allocated */
    
    /* If a string length was specified, only store that many characters */
    
    if (slen > 0)
        {
        strncpy (qs_char_arrays[array_id][index], value, slen);
        (qs_char_arrays[array_id][index])[slen] = '\0';
        }
    else
        {
        strcpy (qs_char_arrays[array_id][index], value);
        }
    /* Increase the maximum length of array since the set was successful */
    
    if (numc_allocated[array_id] < index)
        {
        numc_allocated[array_id] = index;
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine reads a double value from dynamically allocated 
    memory. It allows arrays of doubles to be shared between 
    applications.

Prototype:
    publicdef INT qs_inq_array_c(INT array_id, INT index, CHAR value[], INT *slen );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_id        -(INT) Unique identifier in esi_qs.h.
    index           -(INT) Integer index into the array.
    value           -(CHAR []) String to load from array.
    slen            -(INT *) String length. May be zero.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_TOO_LONG_STRING
    QS_ERR_NEVER_SET
    QS_OUTSIDE_RANGE
    QS_BAD_QS_ID
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_inq_array_c (INT array_id, INT index, CHAR value[], INT *slen)
#else
publicdef INT qs_inq_array_c (array_id, index, value, slen)
INT array_id;
INT index;
CHAR value[];
INT *slen;
#endif
    {
    
    INT old_slen, len_to_use, temp_slen;
    
    /* Test if the QS ID is valid  */
    if ((array_id < 0) || (array_id > MAXCINDEX))
        return(QS_BAD_QS_ID);
    
    /* Test if outside allowable range */
    if ((index > MAXCINDEX) || (index < 0))
        return(QS_OUTSIDE_RANGE);
    
    /* Test if the array element was ever set */
    if (qs_char_arrays[array_id] == (CHAR **)NULL)
        return(QS_ERR_NEVER_SET);
    
    if (qs_char_arrays[array_id][index] == (CHAR *)NULL)
        return(QS_ERR_NEVER_SET);
    
    /* Test if the length was a valid pointer */
    if (slen EQUALS (INT *)NULL)
        temp_slen = 0;
    else
        temp_slen = *slen;
    
    /* If a string length was specified, check how many characters are in
        the stored string and return error if not long enough. */
    
    old_slen = strlen (qs_char_arrays[array_id][index]);
    
    if (temp_slen > 0)
        {
        
        /* check if old length is longer than allowable length given  */
        
        if (old_slen >= temp_slen)
            len_to_use = temp_slen;
        else
            len_to_use = old_slen;
        
        strncpy (value, qs_char_arrays[array_id][index], len_to_use);
        value[len_to_use] = '\0';
        
        if (!(slen EQUALS (INT *)NULL))
            *slen = old_slen;
        
        /* If stored string is longer than the program requests,
             return an error */
        if (old_slen > temp_slen)
            return(QS_TOO_LONG_STRING);
        else
            return(SUCCESS);
        
        }
    else                            /* No length specified */
        {
        strcpy (value, qs_char_arrays[array_id][index]);
        if (!(slen EQUALS (INT *)NULL))
            *slen = old_slen;
        return(SUCCESS);
        
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef VOID qs_term();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID qs_term ()
#else
publicdef VOID qs_term ()
#endif
    {
    INT i, j;
    
    /* free allocated memory for integer arrays */
    
    for (i = 0; i < MAXIINDEX; i++)
        {
        if (numi_allocated[i] > 0)
            {
            tc_free (qs_int_arrays[i]);
            }
        }
    for (i = 0; i < MAXFINDEX; i++)
        {
        if (numf_allocated[i] > 0)
            {
            tc_free (qs_float_arrays[i]);
            }
        }
    for (i = 0; i < MAXDINDEX; i++)
        {
        if (numd_allocated[i] > 0)
            {
            tc_free (qs_double_arrays[i]);
            }
        }
    for (i = 0; i < MAXCINDEX; i++)
        {
        if (qs_char_arrays[i] != (CHAR **)NULL)
            {
            for (j = 0; j < MAXCINDEX; j++)
                {
                if (qs_char_arrays[i][j] != (CHAR *)NULL)
                    {
                    tc_free (qs_char_arrays[i][j]);
                    }
                }
            tc_free (qs_char_arrays[i]);
            }
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine allows others to inquire the maximum array length set for
    that particular array index. NOTE: Since there could be holes in the
    array (unset elements) there still must be a check for QS_ERR_NEVER_SET 
    error status. 

Prototype:
    publicdef INT qs_inq_array_max(INT array_type,INT array_id, INT *max_num );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    array_type      -(INT) #defined in esi_qs.h (QS_INT_TYPE, QS_FLOAT_TYPE...).
    array_id        -(INT) #defined in esi_qs.h. Ex: QS_CUR_WELL.
    max_num         -(INT *) Returned value for length of array.

Return Value/Status:
    SUCCESS - Successful completion.
    QS_ERR_BAD_DATA_TYPE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT qs_inq_array_max (INT array_type, INT array_id, INT *max_num)
#else
publicdef INT qs_inq_array_max (array_type, array_id, max_num)
INT array_type;
INT array_id;
INT *max_num;
#endif
    {
    switch (array_type)
        {
    case QS_INT_TYPE:
        *max_num = numi_allocated[array_id];
        break;
    case QS_FLOAT_TYPE:
        *max_num = numf_allocated[array_id];
        break;
    case QS_DOUBLE_TYPE:
        *max_num = numd_allocated[array_id];
        break;
    case QS_POINTER_TYPE:
        *max_num = nump_allocated[array_id];
        break;
    case QS_CHAR_TYPE:
        *max_num = numc_allocated[array_id];
        break;
        
    default:
        return(QS_ERR_BAD_DATA_TYPE);
        
        }
    return(SUCCESS);
    
    }
/* END:     */
