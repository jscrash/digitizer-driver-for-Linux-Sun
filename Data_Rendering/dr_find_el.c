/* DEC/CMS REPLACEMENT HISTORY, Element DR_FIND_EL.C*/
/* *2    18-JUL-1990 07:57:39 GILLESPIE "(SPR 1) Change calling arg type"*/
/* *1    19-JUN-1989 12:46:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_FIND_EL.C*/
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

NAME:           dr_find_element

DESCRIPTION:    Search the specified object list for a given line/text...
                element id -- set current line/text... to
                point to the found element, and current
                segment to the specified segment.

    Input:      dr_data     - Pointer to DR standard data structure
                item_id     - INT element id of line, text, ...
                seg_id      - INT window manager segment id.
                type_code   - INT Which object type (DR_LINE_OBJECT,
                              DR_TEXT_OBJECT, ...)
    Output:     dr_data     - updated as follows:
                                current_line/text       <== ptr to element
                                current_line/text_seg   <== seg_id

    Status:     SUCCESS     - if current line/text updated.
                FAIL        - if no matching line/text found.
                
******************************************************************************/

#include    "esi_dr.h"

#if USE_PROTOTYPES
publicdef INT dr_find_element(DR_DATA *dr_data, INT item_id, INT seg_id,
                              DR_OBJECT_TYPE_T type_code)
#else
publicdef INT dr_find_element(dr_data, item_id, seg_id, type_code)
DR_DATA *dr_data;
INT item_id, seg_id;
DR_OBJECT_TYPE_T type_code;
#endif
    {
    INT status = 0;
    OBJECT_STRUCTURE *this_object;
    
    switch (type_code)
        {
    case DR_LINE_OBJECT:
        this_object = dr_data->line_ptr;
        
        while (this_object != (OBJECT_STRUCTURE *)NULL)
            {
            
            if (this_object->object_ptr.current_line->line_id == item_id)
                {
                dr_data->current_line = this_object->object_ptr.current_line;
                dr_data->current_line_seg = seg_id;
                break;
                }
            this_object = this_object->next_object;
            }
        /* 1-sep-1988 LUCIA changed = for == in the next statement                  */
        if (this_object == (OBJECT_STRUCTURE *)NULL)
            {
            status = FAIL;
            }
        break;
        
    case DR_TEXT_OBJECT:
        this_object = dr_data->text_ptr;
        
        while (this_object != (OBJECT_STRUCTURE *)NULL)
            {
            
            if (this_object->object_ptr.current_text->text_id == item_id)
                {
                dr_data->current_text = this_object->object_ptr.current_text;
                dr_data->current_text_seg = seg_id;
                break;
                }
            this_object = this_object->next_object;
            }
        /* 1-sep-1988 LUCIA changed = for == in the next statement                  */
        if (this_object == (OBJECT_STRUCTURE *)NULL)
            {
            status = FAIL;
            }
        break;
        
    default:
        status = FAIL;
        break;
        }
    return status;
    }
