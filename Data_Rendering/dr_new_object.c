/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_OBJECT.C*/
/* *2    18-JUL-1990 08:54:58 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *1    19-JUN-1989 12:47:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_NEW_OBJECT.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/******************************************************************************

NAME:          dr_new_object

DESCRIPTION:   Get an object


     Input:    dr_data - The standard data structure

     Output:   dr_data - updated

     Status:   SUCCESS;

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_ov.h"
#include    "esi_wi.h"

#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif

#include "esi_dr_msg.h"

/************************************************************************************/
/*   Global Definitions                                                             */
/************************************************************************************/

#define null_string ""

/************************************************************************************/
/*   End of Global Definitions                                                      */
/************************************************************************************/

#if USE_PROTOTYPES
publicdef INT dr_new_object(DR_DATA *dr_data)
#else
publicdef INT dr_new_object(dr_data)
DR_DATA *dr_data;
#endif
    {
    INT status = 0;
    CHAR msg[80];
    BOOL ifyes;
    
    /*  Query whether to save or release the object                                 */
    
    if ((dr_data->object_changed) && (dr_data->current_object_key > 0))
        {
        sprintf(msg, mg_message(DR_SAVE_CHANGES), dr_data->current_object_key);
        wiyesno(msg, null_string, &ifyes, &status);
        if (ifyes)
            status = dr_save_object(dr_data);
        else
            status = dr_release_object(dr_data);
        }
    else if (dr_data->current_object_key > 0)
        status = dr_release_object(dr_data);
    
    if (status != SUCCESS)
        {
        return status;
        }
    /*  Reset the object                                                            */
    
    status = dr_reset_object(dr_data);
    if (status != SUCCESS)
        {
        return status;
        }
    /*  Get new id                                                                  */
    
    status = dr_new_project_id(&dr_data->current_object_key);
    if (status != SUCCESS)
        {
        return status;
        }
    /*  Load object                                                                 */
    
    status = dr_load_object(dr_data);
    if (status != SUCCESS)
        {
        return status;
        }
    /*  Create new WM group for this object                                         */
    
    ov_new_object(&dr_data->current_object, dr_data->current_object_type,
                  dr_data->current_object_class, dr_data->current_object_key);
    
    return SUCCESS;
    }
