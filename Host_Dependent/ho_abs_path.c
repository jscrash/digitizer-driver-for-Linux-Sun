/* DEC/CMS REPLACEMENT HISTORY, Element HO_ABS_PATH.C*/
/* *7    14-AUG-1991 16:08:06 JTM "(SPR 0) Added init value to temp path as submitted by Unocal."*/
/* *6    28-SEP-1990 12:09:40 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  3B1  24-SEP-1990 16:25:09 GILLESPIE "Beta Deltas"*/
/* *5    27-AUG-1990 15:12:21 VINCE "(SPR 1) added call to ho_translate_path to translate any internal logical names in the path."*/
/* *4    17-AUG-1990 21:50:17 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     5-SEP-1989 15:45:01 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    11-AUG-1989 10:26:06 JULIAN "(SPR -1) redeclaration of ho_absolute_path"*/
/* *1    19-JUN-1989 12:56:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ABS_PATH.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ABS_PATH.C                               */
/* *5    27-AUG-1990 15:12:21 VINCE "(SPR 1) added call to ho_translate_path to translate any internal logical names in the path."  */
/* *4    17-AUG-1990 21:50:17 VINCE "(SPR 5644) Code Cleanup"                       */
/* *3     5-SEP-1989 15:45:01 GORDON "(SPR 200) Gulf changes (August trip)"         */
/* *2    11-AUG-1989 10:26:06 JULIAN "(SPR -1) redeclaration of ho_absolute_path"   */
/* *1    19-JUN-1989 12:56:25 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_ABS_PATH.C                               */
/************************************************************************************/
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

#include "esi_c_lib.h"

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Convert a pathname containing Finder logical names into an absolute           */
/*    pathname for the current environment.                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT ho_absolute_path(CHAR *path);                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (M) path   - (CHAR *)  Path name to be translated.                            */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT ho_absolute_path (CHAR *path)
#else
publicdef INT ho_absolute_path (path)
CHAR *path;
#endif
    {

#ifdef vms
    CHAR *pbracket;
    
    /* look for concealed-type directories (in VMS)                                 */
    /* these are in the form dev:[dir.]                                             */
    /* turn it into dev:[dir]                                                       */
    
    pbracket = strchr (path, ']');
    if ((pbracket != NULL) && (*(pbracket - 1) == '.'))
        {
        strcpy (pbracket - 1, "]");
        }
#else
    FILENAME temp;

    (void) strcpy( temp, "" );    
    ho_translate_path (path, temp);
    ho_translate_filename (temp, path);
#endif
    return SUCCESS;
    }
/* END:                                                                             */
