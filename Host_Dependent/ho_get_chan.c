/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_CHAN.C*/
/* *3    17-AUG-1990 21:51:44 VINCE "(SPR 5644) Code Cleanup"*/
/* *2     2-NOV-1989 15:24:12 JULIAN "(SPR 30) move hoz_st_chan, make hov static"*/
/* *1    19-JUN-1989 12:58:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_CHAN.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_HO_INT_H

#include "esi_ho_int.h"

#endif

#ifndef ESI_HO_ERR_H

#include "esi_ho_err.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifdef vms

#ifndef $DESCRIPTOR

#include "descrip.h"

#endif
#endif
static struct HOV hov = 
    {
    TRUE, 0
    };

/* Function Description -----------------------------------------------------
Description:
    hoz_set_channel_type(channel, type, efn)

    int channel......a vms channel

    int type:
        -1      available
        0       asynchronous line
        1       dma channel GDA0
        2       dma channel GDB0
    int efn.........local event flag for dma comms.

    Specifying a type of -1 frees the entry for the specified channel.

    hov.device_status[i][0] = channel number, or -1 if available
    hov.device_status[i][1] = type
    hov.device_status[i][2] = event flag number

Prototype:
    publicdef INT hoz_set_channel_type(INT channel,INT type, INT efn);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    See above.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    HO_INVALID_CHAN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoz_set_channel_type (INT channel, INT type, INT efn) 
#else
publicdef INT hoz_set_channel_type (channel, type, efn)
INT channel;
INT type;
INT efn;
#endif
    {
    INT status;
    INT channelx;                   /* index for requested channel */
    INT availx;                     /* index for available channel */
    INT i;
    
    /************************************************************************/
    
    /*  Assume success. */
    
    status = SUCCESS;
    
    /*  If first time, initialize sturct and free all slots. */
    
    if (hov.first_time)
        {
        tc_zeroes (&hov, sizeof hov);
        for (i = 0; i < HO_MAX_OPEN_DEVICES; i++)
            hov.device_status[i][0] = -1;
        hov.first_time = FALSE;
        }
    /*  Indicate channel entry not found yet. */
    
    channelx = -1;
    
    /*  Indicate available slot not found yet. */
    
    availx = -1;
    
    /*  Find indicated channel entry in table,
        and find empty slot in case this is a
        new entry. */
    
    for (i = 0; i < HO_MAX_OPEN_DEVICES; i++)
        {
        if ((hov.device_status[i][0] == -1) && (availx == -1))
            availx = i;
        if (hov.device_status[i][0] == channel)
            {
            channelx = i;
            break;
            }
        }
    /*  If call is to make the channel
        available ... */
    
    if (type == -1)
        
        /*  If channel entry was not found in table,
            then indicate bad channel specified. */
        
        if (channelx == -1)
            status = HO_INVALID_CHAN;
        
        /*  If entry was found in table, mark it
            as available. */
        
        else
            hov.device_status[i][0] = -1;
    
    /*  If call was to insert/modify a channel
        entry ... */
    
    else
        {
        
        /*  If channel was not found in table,
            then add new entry using the available
            index, if any are available. */
        
        if (channelx == -1)
            if (availx != -1)
                {
                channelx = availx;
                hov.device_status[channelx][0] = channel;
                }
            else
                status = HO_TOO_MANY_DEVS;
        
        /*  If entry now exists in table, update
            with specified data. */
        
        if (channelx != -1)
            {
            hov.device_status[channelx][1] = type;
            hov.device_status[channelx][2] = efn;
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    hoz_get_channel_type(channel, &type, &efn)

    int channel......a vms channel

    returns int type:
        -1      available
        0       asynchronous line
        1       dma channel

    returns int efn: (local event flag)
        -1      none has been set
        0       available for all processes
        <31 and < 64    valid efn for process

    this routine gets  an  element of
    the internal structure "esi_ho_int.h"
    and returns the channel type to the caller.

Prototype:
    publicdef INT ho_get_channel_type(INT channel,INT *type, INT *efn);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    See above.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.
    HO_INVALID_CHAN
    Function also returns the channel type to caller.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_get_channel_type (INT channel, INT *type, INT *efn) 
#else
publicdef INT ho_get_channel_type (channel, type, efn)
INT channel;
INT *type;
INT *efn;
#endif
    {
    INT status = SUCCESS;
    INT j;
    BOOL found;
    
    /************************************************************************/
    
    found = FALSE;
    for (j = 0; j < HO_MAX_OPEN_DEVICES; j++)
        if (hov.device_status[j][0] == channel)
            {
            found = TRUE;
            break;
            }
    if (found)
        {
        *type = hov.device_status[j][1];
        *efn = hov.device_status[j][2];
        }
    else
        status = HO_INVALID_CHAN;
    
    return status;
    }
/* END:     */
