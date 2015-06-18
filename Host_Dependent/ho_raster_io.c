/* DEC/CMS REPLACEMENT HISTORY, Element HO_RASTER_IO.C*/
/* *2    17-AUG-1990 21:53:04 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:59:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_RASTER_IO.C*/
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

#ifndef UNIX        /* Not used by unix os */

#include "esi_c_lib.h"

#ifdef vms

#include "esi_ho_int.h"
#include "esi_ho_err.h"

#ifndef $DESCRIPTOR

#include "descrip.h"

#endif

#include "iodef.h"
#include "ssdef.h"
#include "ttdef.h"
#include "tt2def.h"
#include "dvidef.h"
extern int LIB$GETDVI ();

/* File Description ---------------------------------------------------------
Overview:
    Raster IO routines.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT hoclosio (INT program_channel);
    
    publicdef INT hobsend (INT program_channel, INT length, BYTE *buffer);
    
    publicdef INT hobrecv (INT *program_channel, INT *max_bytes, BYTE *buffer, 
                    INT *timeout, INT *actual_number_received);

    publicdef INT hoopenio (INT *program_channel_ptr, BYTE *device_name);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    The local efn is used to synchronize i/o to this process
    for dma. Although serial could also use an efn, we default to
    the "0" efn so that  other processes can use the line (seismic
    bypass ? ). Because dma calls depend on a foreign device driver
    the efn should be used for synch and i/o since vms cannot synchronize
    with an "unknown" device. This efn will be stored in the global
    channel structure (HOV) only for dma; serial efn's are = -1

    clear the efn since it may be already set to an unknown state;
    qio will automatically clear the efn so that this need be done once.

    set the efn. Note that the efn in the range of 32-63 is local
    to this ("HOOPEN") process not parent or subprocesses! We must assume
    that only the HO routines will do subsequent i/o calls for safety; if you
    do the i/o from other processes, this efn may not be valid for your
    calls. If so, get your local efn and synch with it.

    We are assuming that an ef must be set for dma in rugged, "bullet-proof"
    applications. It is possible to proceed without the ef if the cycle
    rates on the Rt controller are tuned close to the VMS bus rates but
    unsynchronized comms. can be dangerous to vms (primary disk paging)

Prototype:
    publicdef INT hoopenio (INT *program_channel_ptr, BYTE *device_name);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    program_channel_ptr -(INT *)    
    device_name         -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    HO_NO_SUCH_DEVICE
    HO_DEVICE_IN_USE
    HO_DEVICE_OFFLINE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoopenio (INT *program_channel_ptr, BYTE *device_name) 
#else
publicdef INT hoopenio (program_channel_ptr, device_name)
INT *program_channel_ptr;
BYTE *device_name;
#endif
    {
    short schan;
    INT status;
    INT comm_type;
    INT efn;
    INT channel;
    static struct dsc$descriptor d = 
        {
        0, DSC$K_DTYPE_T, DSC$K_CLASS_S, 0
        };
    
    /************************************************************************/
    
    status = SS$_NORMAL;
    channel = -1;
    
    d.dsc$w_length = strlen ((char *)device_name);
    d.dsc$a_pointer = (char *)device_name;
    
    /*  dma channels on the vax are
        "gda0", "gdb0", etc. */
#if 0
    MODIFIED6 - 3 -
    88RICJ.
        oneread and onewrit (
                         dma read and write)routines determineif communication type is 
        DMA or serial by the comm_type flag.If comm_type =
                                                           0the communication is 
                                                           serial.If comm_type =
                                                           1then communication is DMA.
                                                           oneread and onewritedo
        not recognize comm_type > 1.if (((device_name[0] == 'G') ||
                                                           (device_name[0] == 'g')) &&
                                                           ((device_name[1] == 'D') ||
                                                            (device_name[1] == 'd')))
            {
            if ((device_name[2] == 'A') || (device_name[2] == 'a'))
                comm_type = 1;
            else
                comm_type = 2;
            }
#endif
    if (((device_name[0] == 'G') || (device_name[0] == 'g')) ||
        ((device_name[0] == 'X') || (device_name[0] == 'x')))
        comm_type = 1;              /*  DMA device  */
    else
        comm_type = 0;              /*  Serial device. */
    
    if (comm_type > 0)
        {
        
        /*  Perform special DMA init.
            you must allocate the device to
            local process; excluding other accesss */
        
        status = VAX_ROUTINE (SYS$ALLOC (&d, 0, 0, 0));
        switch (status)
            {
        case SS$_DEVALRALLOC:
            status = SS$_NORMAL;
            break;
            }
        }
    if (status == SS$_NORMAL)
        
        /* use vms assign for both serial and dma */
        
        status = VAX_ROUTINE (SYS$ASSIGN (&d, &schan, 0, 0));
    
    if (comm_type > 0)
        {
        if (status == SS$_NORMAL)
            
            /*   get a local event flag if DMA  */
            
            status = VAX_ROUTINE (LIB$GET_EF (&efn));
        if (status == SS$_NORMAL)
            status = VAX_ROUTINE (SYS$CLREF (efn));
        if (status & 1)
            status = VAX_ROUTINE (SYS$SETEF (efn));
        }
    if (status & 1)
        status = SS$_NORMAL;
    
    switch (status)
        {
    case SS$_NORMAL:
        channel = schan;
        status = SUCCESS;
        break;
    case SS$_DEVALLOC:
        status = HO_DEVICE_IN_USE;
        break;
    case SS$_DEVMOUNT:
        status = HO_DEVICE_IN_USE;
        break;
    case SS$_DEVOFFLINE:
        status = HO_DEVICE_OFFLINE;
        break;
    case SS$_NODEVAVL:
        status = HO_DEVICE_IN_USE;
        break;
    case SS$_NOSUCHDEV:
        status = HO_NO_SUCH_DEVICE;
        break;
    default:
        status = HO_ERR_VMS;
        break;
        }
    if (status == SUCCESS)
        {
        *program_channel_ptr = schan;
        status = hoz_set_channel_type (channel, comm_type, efn);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function will return only when max_bytes characters has been
    received. If timeout is specified, then a timeout will be generated
    and an incomplete read operation may occur.  Note that this function
    ignores CR as a terminator.

Prototype:
    publicdef INT hobrecv (INT *program_channel, INT *max_bytes, BYTE *buffer, 
                    INT *timeout, INT *actual_number_received);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) program_channel         -(INT *) OS channel number.
    (I) max_bytes               -(INT *) Buffer size.
    (I) buffer                  -(BYTE *) Address of buffer.
    (O) actual_number_received  -(INT *) Number of bytes received.
    (O) timeout                 -(INT *) Number of seconds before timeout signalled.

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    HO_TIMEOUT
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hobrecv (INT *program_channel, INT *max_bytes, BYTE *buffer, INT *timeout,
                       INT *actual_number_received) 
#else
publicdef INT hobrecv (program_channel, max_bytes, buffer, timeout,
                       actual_number_received)
INT *program_channel;
INT *max_bytes;
BYTE *buffer;
INT *actual_number_received;
INT *timeout;
#endif
    {
    unsigned short iosb[4];
    static unsigned short terminator = 
        {
        0
        };
    INT func = IO$_TTYREADALL | IO$_READVBLK | IO$M_NOECHO;
    INT status;
    
    /************************************************************************/
    
    if (timeout IS_NOT_EQUAL_TO (INT *)0 AND * timeout IS_NOT_EQUAL_TO 0)
        {
        func |= IO$M_TIMED;
        }
    status = VAX_ROUTINE (SYS$QIOW (0, *program_channel, func, iosb, 0, 0, buffer,
                                    *max_bytes, *timeout, 0, terminator, 0));
    
    if (status EQUALS SS$_NORMAL)
        {
        *actual_number_received = iosb[1];
        if (iosb[0] EQUALS SS$_NORMAL)
            {
            status = SUCCESS;
            }
        else if (iosb[0] EQUALS SS$_TIMEOUT)
            {
            status = HO_TIMEOUT;
            }
        else
            {
            VAX_ROUTINE (iosb[0]);  /* log this error */
            status = HO_ERR_VMS;
            }
        }
    else
        {
        status = HO_ERR_VMS;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT hobsend (INT program_channel, INT length, BYTE *buffer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    program_channel -(INT)
    length          -(INT)
    buffer          -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hobsend (INT program_channel, INT length, BYTE *buffer) 
#else
publicdef INT hobsend (program_channel, length, buffer)
INT program_channel;
INT length;
BYTE *buffer;
#endif
    {
    short iosb[4];
    INT func = IO$_WRITEVBLK | IO$M_NOFORMAT;
    INT status;
    
    /************************************************************************/
    
    status = VAX_ROUTINE (SYS$QIOW (0, program_channel, func, iosb, 0, 0, buffer,
                                    length, 0, 0, 0, 0));
    
    if ((status EQUALS SS$_NORMAL) AND (iosb[0] EQUALS SS$_NORMAL))
        {
        status = SUCCESS;
        }
    else
        {
        VAX_ROUTINE (iosb[0]);      /* log this error */
        status = HO_ERR_VMS;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT hoclosio (INT program_channel);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    program_channel -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    HO_ERR_VMS
    HO_DEVICE_NOT_OPEN
    HO_INVALID_CHAN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hoclosio (INT program_channel) 
#else
publicdef INT hoclosio (program_channel)
INT program_channel;
#endif
    {
    INT status;
    INT type;
    INT efn;
    INT channel;
    SHORT iosb[4];
    INT devno;
    char dvi_name[64];
    int func = DVI$_DEVNAM;
    unsigned short dvi_channel;
    unsigned short dvi_len;
    struct dsc$descriptor_s dvi_name_dsc;
    INT console_id = 0;
    INT errfil;
    INT ibuf = 0;
    
    /************************************************************************/
    
    /*  Get device name for specified channel. */
    
    status = ho_get_channel_type (program_channel, &type, &efn);
    
    if (status == SUCCESS)
        {
        if (type == 1)
            strcpy (dvi_name, "GDA0");
        if (type == 2)
            strcpy (dvi_name, "GDB0");
        
        dvi_channel = program_channel;
        dvi_name_dsc.dsc$a_pointer = dvi_name;
        dvi_name_dsc.dsc$w_length = sizeof dvi_name;
        dvi_name_dsc.dsc$b_class = DSC$K_CLASS_S;
        dvi_name_dsc.dsc$b_dtype = DSC$K_DTYPE_T;
        
        status = LIB$GETDVI (&func, &dvi_channel, 0, 0, &dvi_name_dsc, &dvi_len);
        
        switch (status)
            {
        case SS$_NORMAL:
            status = SUCCESS;
            dvi_name_dsc.dsc$w_length = dvi_len;
            break;
            
        default:
            status = HO_ERR_VMS;
            break;
            }
        }
    if (status == SUCCESS)
        {
        status = VAX_ROUTINE (SYS$DASSGN (program_channel));
        
        /* dma */
        /* release channel-type and efn for DMA */
        
        if (status == SS$_NORMAL)
            {
            channel = program_channel;
            if (type > 0)
                {
                if ((efn >= 32) && (efn <= 63))
                    {
#if 0
                    status = VAX_ROUTINE (SYS$SYNCH (efn, iosb));
#endif
                    status = VAX_ROUTINE (LIB$FREE_EF (&efn));
                    status = VAX_ROUTINE (SYS$DALLOC (&dvi_name_dsc, 0));
                    }
                }
            }
        switch (status)
            {
        case SS$_NORMAL:
            status = SUCCESS;
            break;
        case SS$_IVCHAN:
            status = HO_INVALID_CHAN;
            break;
        case SS$_NOPRIV:
            status = HO_DEVICE_NOT_OPEN;
            break;
        default:
            status = HO_ERR_VMS;
            break;
            }
        }
    if (status == SUCCESS)
        status = hoz_set_channel_type (channel, -1, -1);
    
    return status;
    }


#endif
/* END:     */


#endif      /* end of ifndef UNIX */
