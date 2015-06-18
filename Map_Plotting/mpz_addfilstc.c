/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_ADDFILSTC.C*/
/*  *3    30-OCT-1990 11:53:11 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/*  *2    17-AUG-1990 22:02:39 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:09:48 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_ADDFILSTC.C*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_ADDFILSTC.C                            */
/*  *2    17-AUG-1990 22:02:39 VINCE "(SPR 5644) Code Cleanup"                      */
/*  *1    19-JUN-1989 13:09:48 SYSTEM ""                                            */
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_ADDFILSTC.C                            */
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
#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_ho.h"
#include "esi_fm.h"
#include "esi_fm_err.h"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine to add the supplied name to the                                       */
/*    supplied map file structure.                                                  */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mpz_add_to_file_struct(FM_FILE log_name,FM_FILE name,FM_TYPE type,  */
/*        MP_FILES *file_struct);                                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    log_name        -(FM_FILE)                                                    */
/*    name            -(FM_FILE)                                                    */
/*    type            -(FM_TYPE)                                                    */
/*    file_struct     -(MP_FILES *)                                                 */
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
publicdef INT mpz_add_to_file_struct (FM_FILE log_name, FM_FILE name, FM_TYPE type,
                                      MP_FILES *file_struct)
#else
publicdef INT mpz_add_to_file_struct (log_name, name, type, file_struct)
FM_FILE log_name;
FM_FILE name;
FM_TYPE type;
MP_FILES *file_struct;
#endif
    {
    
    INT i;
    
    /* **********************************************************************       */
    /* * CHECK FOR EXISTENCE *                                                      */
    /* -----------------------                                                      */
    
    if (fm_check_access (log_name, name, type, "X") EQUALS FM_NO_SUCH_FILE)
        {
        
        /* * CHECK IF NAME IS ALREADY IN THE FILE STRUCT *                          */
        /* -----------------------------------------------                          */
        
        for (i = 0; i < file_struct->n_names; i++)
            {
            if (strcmp (name, file_struct->name[i]) EQUALS 0)
                {
                (file_struct->n_references[i])++;
                break;
                }
            }
        /* * ADD TO FILE STRUCTURE *                                                */
        /* -------------------------                                                */
        
        if (i >= file_struct->n_names)
            {
            /* ADD TO DEFINED TCP                                                   */
            
            ts_tcp_append (&(file_struct->name), name);
            file_struct->n_names++;
            
            /* ADD TO DEFINING OVERLAY INFO ARRAY                                   */
            /* RE-ALLOCATE                                                          */
            if (file_struct->n_names > file_struct->n_define)
                {
                file_struct->n_define += 20;
                i = file_struct->n_define * sizeof(INT);
                
                if (file_struct->n_define EQUALS 20)
                    {
                    file_struct->n_references = (INT *) tc_alloc (i);
                    }
                else
                    {
                    file_struct->n_references =
                        (INT *) tc_realloc (file_struct->n_references, i);
                    }
                }
            file_struct->n_references[file_struct->n_names - 1] = 1;
            }
        }
    strcpy (file_struct->last_used, name);
    
    return SUCCESS;
    }
/* END:                                                                             */
