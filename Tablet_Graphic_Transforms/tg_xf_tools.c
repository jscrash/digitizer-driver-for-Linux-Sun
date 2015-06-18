/* DEC/CMS REPLACEMENT HISTORY, Element TG_XF_TOOLS.C*/
/* *3    17-AUG-1990 22:27:49 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    13-NOV-1989 11:52:55 JULIAN "(SPR 1) Added support for CALCOMP tablets"*/
/* *1    19-JUN-1989 13:32:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TG_XF_TOOLS.C*/
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

#include "esi_gl_defs.h"
#include "esi_tg_mat.h"
#include "esi_math.h"

#ifdef REG_INT

#undef REG_INT

#endif

/* if this is VAX version then we can use register variables. */
/* but not sure if it makes sense for others like the Prime. */
#ifdef primos

#define REG_INT     INT

#else

#define REG_INT     register INT    

#endif

/* File Description ---------------------------------------------------------
Overview:
    The following are matrix manipulation subroutines written
    for support of the tablet registration & calculation routines.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tg_zeromat(MATRIX mat);
    
    publicdef INT tg_set_identity(MATRIX mat);
    
    publicdef INT tg_copy_mat(MATRIX mat1,MATRIX mat2);
    
    publicdef INT tg_transform_points(DOUBLE x[],DOUBLE y[],MATRIX mat,INT num);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Set mat to MATSIZE x MATSIZE matrix of zeroes.

Prototype:
    publicdef INT tg_zeromat(MATRIX mat);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mat             -(MATRIX)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_zeromat (MATRIX mat)
#else
publicdef INT tg_zeromat (mat)
MATRIX mat;
#endif
    {
    REG_INT i, j;
    
    for (i = 0; i < MATSIZE; i++)
        for (j = 0; j < MATSIZE; j++)
            mat[i][j] = 0.0;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Set mat to the identity matrix.

Prototype:
    publicdef INT tg_set_identity(MATRIX mat);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mat             -(MATRIX)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_set_identity (MATRIX mat)
#else
publicdef INT tg_set_identity (mat)
MATRIX mat;
#endif
    {
    REG_INT i;
    
    tg_zeromat (mat);
    for (i = 0; i < MATSIZE; i++)
        mat[i][i] = 1.0;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Assign matrix2 to matrix1.

Prototype:
    publicdef INT tg_copy_mat(MATRIX mat1,MATRIX mat2);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mat1            -(MATRIX)
    mat2            -(MATRIX)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_copy_mat (MATRIX mat1, MATRIX mat2)
#else
publicdef INT tg_copy_mat (mat1, mat2)
MATRIX mat1, mat2;
#endif
    {
    REG_INT i, j;
    
    for (i = 0; i < MATSIZE; i++)
        for (j = 0; j < MATSIZE; j++)
            mat1[i][j] = mat2[i][j];
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Multiply mat1 * mat2 and place result mat in mat2.

Prototype:
    publicdef INT tg_mat_mult(MATRIX    mat1,MATRIX mat2,MATRIX result);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mat1            -(MATRIX)
    mat2            -(MATRIX)
    result          -(MATRIX)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_mat_mult (MATRIX mat1, MATRIX mat2, MATRIX result)
#else
publicdef INT tg_mat_mult (mat1, mat2, result)
MATRIX mat1, mat2, result;
#endif
    {
    REG_INT i, j, k;
    
    for (i = 0; i < MATSIZE; i++)
        for (j = 0; j < MATSIZE; j++)
            for (k = 0; k < MATSIZE; k++)
                result[i][j] += mat1[i][k] * mat2[k][j];
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Multiply array of 2d points by the 3x3 mat.

Prototype:
    publicdef INT tg_transform_points(DOUBLE x[],DOUBLE y[],MATRIX mat,INT num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(DOUBLE [])
    y               -(DOUBLE [])
    mat             -(MATRIX)
    num             -(INT)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tg_transform_points (DOUBLE x[], DOUBLE y[], MATRIX mat, INT num)
#else
publicdef INT tg_transform_points (x, y, mat, num)
DOUBLE x[], y[];
MATRIX mat;
INT num;
#endif
    {
    INT status;
    DOUBLE x1, y1;
    REG_INT i;
    
    for (i = 0; i < num; i++)
        {
        x[i] = (mat[0][0] * x[i] + mat[1][0] * y[i] + mat[2][0]);
        y[i] = (mat[0][1] * x[i] + mat[1][1] * y[i] + mat[2][1]);
        }
    }
/* END:     */
