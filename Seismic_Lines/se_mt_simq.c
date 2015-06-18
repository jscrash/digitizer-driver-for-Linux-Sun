/* DEC/CMS REPLACEMENT HISTORY, Element SE_MT_SIMQ.C */
/* *2    12-JUN-1991 19:36:16 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 19:35:25 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_MT_SIMQ.C */
/*-------------------------------------------------------------------------
 
  +---------------------------------------------------------------------+
  |    Copyright Donald A. Vossler, 1991                                |
  |    3018 Apple Valley Lane                                           |
  |    Missouri City, Texas 77459                                       |
  |    (713) 437-9144                                                   |
  |                                                                     |
  |    Unpublished -- All rights reserved                               |
  |                                                                     |
  |    This software is the proprietary property of Donald A. Vossler.  |
  |                                                                     |
  |    General permission to use and modify this software without fee   |
  |    is granted provided that this notice remains a permanent part of |
  |    the code.  Any modifications to this code, including deletions   |
  |    or extensions, are to be considered bound by this copyright      |
  |    notice.                                                          |
  |    No representations are made about the suitability of use of this |
  |    software for any purpose.  It is provided 'as is' without        |
  |    express or implied warranty.                                     |
  |                                                                     |
  |    (The above notice does not constitute an admission of publication|
  |    or unrestricted dissemination of the work.)                      |
  +---------------------------------------------------------------------+

-------------------------------------------------------------------------*/
/* Function Description -------------------------------------------------------
Description:
    Obtain solution of a set of simultaneous linear equations,
    AX = B.

Prototype:
    publicdef INT se_mt_simq(DOUBLE *a, DOUBLE *b, INT n)

Parameters:
    (M) a           -(DOUBLE) Matrix of coefficients stored columnwise.
                              These are destroyed in the computation.  
                              The size of matrix 'a' is 'n' by 'n'.
    (M) b           -(DOUBLE) Vector of original constants (length 'n').
                              These are replaced by final solution values.
    (I) n           -(INT)    Number of equations and variables.  'n' must
                              be >= 1.

Return Value/Status:
    0 = Normal solution.
    1 = Set of equations is singular.

Scope:
    PUBLIC
 
Limitations/Assumptions:
    Matrix 'a' must be general.  If the matrix is singular, solution
    values are meaningless. 
    The forward solution to obtain variable 'n' is done in 'n' stages.
    The back solution for the other variables is calculated by successive
    substitutions.  Final solution values are developed in vector 'b',
    with variable 1 in b[0], varible 2 in b[1],..., varible 'n' in 
    b[n-1].  If no pivot can be found exceeding a tolerance of 0.0, the
    matrix is considered singular. This tolerance can be modified by
    replacing the first statement.

-----------------------------------------------------------------------------*/

#include "esi_se.h"

#if USE_PROTOTYPES
publicdef INT se_mt_simq (DOUBLE *a, DOUBLE *b, INT n)
#else
publicdef INT se_mt_simq (a, b, n)
DOUBLE a[], b[];
INT n;
#endif
    {
    DOUBLE tol, biga, sav;
    INT jj, j, i, ij, it, i1, i2, imax, iqs, ixj, ix, jx, jy, ixjx, jjx, ny, ia, ib,
        ic, k;
    INT ks;
    /*
     
             FORWARD SOLUTION
     
    */
    tol = 0.0;
    ks = 0;
    jj = -n;
    for (j = 1; j <= n; j++)
        {
        jy = j + 1;
        jj = jj + n + 1;
        biga = 0;
        it = jj - j;
        for (i = j; i <= n; i++)
            {
            /*
             
                     SEARCH FOR MAXIMUM COEFFICIENT IN COLUMN
             
            */
            ij = it + i - 1;
            if (ABS (biga) < ABS(a[ij]))
                {
                biga = a[ij];
                imax = i;
                }
            }
        /*
         
                 TEST FOR PIVOT LESS THAN TOLERANCE (SINGULAR MATRIX)
         
        */
        if (ABS (biga) <= tol)
            {
            ks = 1;
            return ks;
            }
        /*
         
                 INTERCHANGE ROWS IF NECESSARY
         
        */
        i1 = j + n * (j - 2) - 1;
        it = imax - j;
        for (k = j; k <= n; k++)
            {
            i1 = i1 + n;
            i2 = i1 + it;
            sav = a[i1];
            a[i1] = a[i2];
            a[i2] = sav;
            /*
             
                     DIVIDE EQUATION BY LEADING COEFFICIENT
             
            */
            a[i1] = a[i1] / biga;
            }
        sav = b[imax - 1];
        b[imax - 1] = b[j - 1];
        b[j - 1] = sav / biga;
        /*
         
                 ELIMINATE NEXT VARIABLE
         
        */
        if (j == n)
            goto L_70;
        iqs = n * (j - 1);
        for (ix = jy; ix <= n; ix++)
            {
            ixj = iqs + ix;
            it = j - ix;
            for (jx = jy; jx <= n; jx++)
                {
                ixjx = n * (jx - 1) + ix;
                jjx = ixjx + it;
                a[ixjx - 1] -= a[ixj - 1] * a[jjx - 1];
                }
            b[ix - 1] -= b[j - 1] * a[ixj - 1];
            }
        }
    /*
     
             BACK SOLUTION
     
    */
L_70:
    ny = n - 1;
    it = n * n;
    for (j = 1; j <= ny; j++)
        {
        ia = it - j - 1;
        ib = n - j - 1;
        ic = n - 1;
        for (k = 1; k <= j; k++)
            {
            b[ib] -= a[ia] * b[ic];
            ia = ia - n;
            ic = ic - 1;
            }
        }
    return ks;
    }
