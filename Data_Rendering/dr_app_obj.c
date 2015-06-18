/* DEC/CMS REPLACEMENT HISTORY, Element DR_APP_OBJ.C*/
/* *4    18-JUL-1990 07:30:14 GILLESPIE "(SPR 1) Change calling arg types"*/
/* *3     5-FEB-1990 11:43:52 ADEY "(SPR 1) fix d_list[1] to be d_list[i]"*/
/* *2    17-JAN-1990 15:30:21 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *1    19-JUN-1989 12:46:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_APP_OBJ.C*/

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

NAME:           dr_append_object

DESCRIPTION:    1)  Append a new object (line, text, ...) to the
                    corresponding list of objects. 
                     
                2)  Fill the object with the specified type_data block,
                    (e.g., default line or line loaded from database).
                    
                3)  Init the nlist for lines (If the nlist does not
                    yet exist).
                
                4)  Set the current object of type: type_code to point to
                    the new (line, text,  ...).
                    NOTE: dr_data->current_line/text points to the
                    "atomic" structure LINE/TEXT,
                    not to the object structure in the list of objects.

    Input:      dr_data     - Pointer to DR standard data structure
                type_data   - Pointer to the data block for this line/text/...
                type_code   - Which object type (DR_LINE_OBJECT,
                              DR_TEXT_OBJECT, ...)
                new_flag    - TRUE if the object does not yet exist in
                              the database.

    Output:     dr_data     - updated with the new object properly placed
                              into the object chain.

    Status:     SUCCESS when    object appended,
                FAIL    when    type_code is not recognized,
                                or allocation fails.
                
******************************************************************************/

#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_ho.h"

#if USE_PROTOTYPES
publicdef INT dr_append_object (DR_DATA *dr_data, VOIDPTR type_data,
	DR_OBJECT_TYPE_T type_code, INT new_flag)
#else
publicdef INT dr_append_object (dr_data, type_data, type_code, new_flag)
DR_DATA *dr_data;
VOIDPTR type_data;
DR_OBJECT_TYPE_T type_code;
INT new_flag;
#endif
    {
    INT status = 0;
    INT i;
    OBJECT_STRUCTURE *this_object;
    size_t datasize[2];
    INT datatype[2];
    INT valueinc[2];
    INT units[2];
    UINT ndim = 2;
    static CHAR *c_name[3] = 
        {
        "X", "Y", NULL
        };
    UINT d_list[2];


    /*  Set this_object pointer to head of proper type chain */
    switch (type_code)
        {
    case DR_LINE_OBJECT:
        this_object = dr_data->line_ptr;
        break;
    case DR_TEXT_OBJECT:
        this_object = dr_data->text_ptr;
        break;
    default:
        status = FAIL;
        return status;
        }

    /*  If no objects of this type on chain, create one */

    if (this_object == (OBJECT_STRUCTURE *)NULL)
        {
        this_object = (OBJECT_STRUCTURE *)am_allocate (AM_APPLICATION,
                                                sizeof(OBJECT_STRUCTURE));
        if (this_object == (OBJECT_STRUCTURE *)NULL)
            {
            status = FAIL;
            return status;
            }

        /*      Set the pointer to the NEW head of the proper type_code chain */

            
        switch (type_code)
            {
        case DR_LINE_OBJECT:
            dr_data->line_ptr = this_object;
            break;
        case DR_TEXT_OBJECT:
            dr_data->text_ptr = this_object;
            break;
            }
        }

    else
        {
        /*      Find end of object chain & attach a new object
                (head of object chain is guaranteed to be defined here) */

        while (this_object->next_object != (OBJECT_STRUCTURE *)NULL)
            {
            this_object = this_object->next_object;
            }

        this_object->next_object =
            (OBJECT_STRUCTURE *)am_allocate (AM_APPLICATION,
                                                  sizeof(OBJECT_STRUCTURE));
        this_object = this_object->next_object;
        if (this_object == (OBJECT_STRUCTURE *)NULL)
            {
            status = FAIL;
            return status;
            }
        }

    /*  This_object now points to the freshly allocated block, hook in the
        new space for the line,text,... and set the "current" pointer */

    status = SUCCESS;
    switch (type_code)
        {
    case DR_LINE_OBJECT:
        this_object->new_object = new_flag;
        this_object->object_type = DR_LINE_OBJECT;
        this_object->object_ptr.current_line =
            (LINE *)am_allocate (AM_APPLICATION, sizeof(LINE));
        if (this_object->object_ptr.current_line == (LINE *)NULL)
            {
            status = FAIL;
            break;
            }
        hoblockmove (type_data, this_object->object_ptr.current_line,
                     sizeof(LINE));
        /*  Init the nlist if this is a new line element */
        if (this_object->object_ptr.current_line->line_nlist ==
            ((NLIST_HEADER)0))
            {
            for (i = 0; i < ndim; i++)
                {
                units[i] = 0;
                datasize[i] = sizeof(FLOAT);
                datatype[i] = FLOAT_TYPE;
                valueinc[i] = VALUE_LIST;
                d_list[i] = i + 1;
                }

            nl_init_nlist (&this_object->object_ptr.current_line->line_nlist,
                           ndim, units, datasize, datatype, valueinc, 0);
            status = nl_set_nlist_info (this_object->object_ptr.current_line->line_nlist,
                            NL_NLIST_COLUMN_NAME, ndim, d_list,
                            (VOIDPTR)c_name);
            if (status != SUCCESS)
                {
                return status;
                }
            }
        dr_data->current_line = this_object->object_ptr.current_line;
        break;

    case DR_TEXT_OBJECT:
        this_object->new_object = new_flag;
        this_object->object_type = DR_TEXT_OBJECT;
        this_object->object_ptr.current_text =
            (TEXT *)am_allocate (AM_APPLICATION,sizeof(TEXT));
        if (this_object->object_ptr.current_text == (TEXT *)NULL)
            {
            status = FAIL;
            break;
            }
        hoblockmove (type_data, this_object->object_ptr.current_text,
                     sizeof(TEXT));
        dr_data->current_text = this_object->object_ptr.current_text;
        break;
        }

    return status;
    }
