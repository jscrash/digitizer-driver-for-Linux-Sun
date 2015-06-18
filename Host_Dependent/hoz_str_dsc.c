/* DEC/CMS REPLACEMENT HISTORY, Element HOZ_STR_DSC.C*/
/* *6    28-SEP-1990 12:18:59 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  2B1  24-SEP-1990 16:24:06 GILLESPIE "Beta Deltas"*/
/* *5    27-AUG-1990 15:03:56 VINCE "(SPR 1) added if UNIX precompiler section to make a null function"*/
/* *4    20-AUG-1990 17:07:39 KEE "(SPR -1) fixing compile error at sun"*/
/* *3    17-AUG-1990 21:50:14 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:06 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:56:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HOZ_STR_DSC.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifdef vms
#include "descrip.h"
#endif

#ifdef mips
#include "GKS3Ddescrip.h"
#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine to create a VAX descriptor for a character string.                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef struct dsc$descriptor *hoz_str_descpt(CHAR *pchar);                 */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pchar           -(CHAR *)                                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    Function returns the address of the descriptor string.                        */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*    Warning - only 1 call to DESCRIPTOR can be active at once - no more!!!        */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */

#ifdef vms
#define RETURN_TYPE struct dsc$descriptor *
#endif
#ifdef mips
#define RETURN_TYPE struct dsc$descriptor *
#endif
#ifndef RETURN_TYPE
#define RETURN_TYPE BYTE *
#endif

#if USE_PROTOTYPES
publicdef RETURN_TYPE hoz_str_descpt (CHAR *pchar)
#else
publicdef  RETURN_TYPE hoz_str_descpt (pchar)
CHAR *pchar;
#endif
    {

#ifdef vms
    static  struct dsc$descriptor d = 
        {
        0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0
        };
    d.dsc$w_length = strlen (pchar);
    d.dsc$a_pointer = pchar;
    
    return &d;
#endif

#ifdef mips
    static  struct dsc$descriptor d;
    d.dsc$b_dtype = DSC$K_DTYPE_T;
    d.dsc$b_class = DSC$K_CLASS_S;
    d.dsc$w_length = strlen (pchar);
    d.dsc$a_pointer = pchar;
    
    return &d;
#endif

#if UNIX
    return(BYTE *)0;
#endif
    }
/* END:                                                                             */
