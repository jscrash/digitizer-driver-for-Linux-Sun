/* DEC/CMS REPLACEMENT HISTORY, Element VI_GET_VELFN.C */
/* *2    12-JUN-1991 20:19:12 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:05:19 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_GET_VELFN.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1991
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_fm.h"
#include "esi_vi.h"
#include "esi_c_lib.h"

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

/* Function Description ------------------------------------------------------- 
Description:                                                                  
    Reads BHP type velocity functions from ASCII file - line,SP numeric       
                                                                              
Prototype:                                                                    
    publicdef INT vi_get_velfn(CHAR *filename, INT *iflag, INT *line_id,      
                               INT *shot_point, INT *ntv, INT *time, 
                               INT *vel);
                               
Parameters                                                                    
    (I) filename    -(CHAR) The name of the ASCII data file.                  
    (M) iflag       -(INT)  Set to '0' on first call.  Value updated          
                            internally.  Only reset to zero for new file.
    (O) line_id     -(INT)  Line number for current velocity function.        
    (O) shot_point  -(INT)  Shotpoint number for velocity function.           
    (O) ntv         -(INT)  Length of returned time,velocity arrays           
    (O) time        -(INT)  Array of times.                                   
    (O) vel         -(INT)  Array of velocities.                              
                                                                              
Return Value/Status:                                                          
    0 = normal return                                                         
   -1 = could not open file                                                   
   -2 = unexpected card image type                                            
   -3 = not same number of times & vel's on card image                        
    1 = EOF                                                                   
                                                                              
Scope:                                                                        
    PUBLIC                                                                    
                                                                              
Limitations/Assumptions:                                                      
    Line and shot point must be numeric                                       

Revisions:
    04 Jun 1991: DAV - Increased bufr from 81 to 161.
-----------------------------------------------------------------------------*/
#if USE_PROTOTYPES
publicdef INT vi_get_velfn (CHAR *filename, INT *iflag, INT *line_id,
                            INT *shot_point, INT *ntv, INT *time, INT *vel)
#else
publicdef INT vi_get_velfn (filename, iflag, line_id, shot_point, ntv, 
                            time, vel)
CHAR *filename;
INT  *iflag;
INT  *line_id;
INT  *shot_point;
INT  *ntv;
INT  *time;
INT  *vel;
#endif
{
    static FILE *fhan;
    INT status = SUCCESS;
    INT knt;
    static INT line;
    static INT ready;
    static INT buf_max = 161;
    static CHAR bufr[161];
    CHAR tmp[80];
    CHAR *next;
    
    /* If flag cleared, then open file and reset */
    /* ----------------------------------------- */
    if (*iflag EQUALS 0)
        {
        fhan = fopen (filename, "r");
        if (fhan EQUALS (FILE *)0)
            {
            status = -1;
            return status;
            }
        ready = 0;
        }

    /* Get next line from file */
    /* ----------------------- */
    if (ready EQUALS 0)
        {
        if ((fgets (bufr, buf_max, fhan)) EQUALS (CHAR *)0)
            {
            status = 1;
            return status;
            }
        *iflag += 1;
        }

    /* Clear ready flag */
    /* ---------------- */
    ready = 0;
    
    /* Input line number or next SP */
    /* ---------------------------- */
    if (*bufr EQUALS'L')
        {
        *line_id = atoi (&bufr[7]);
        line = *line_id;
        }
    else if (*bufr EQUALS'*')
        {
        *line_id = line;
        *shot_point = atoi (&bufr[7]);
        goto vt;
        }
    else
        {
        status = -2;
        return status;
        }

    /* Get next line from file */
    /* ----------------------- */
    if ((fgets (bufr, buf_max, fhan)) EQUALS (CHAR *)0)
        {
        status = 1;
        return status;
        }
    *iflag += 1;
    
    /* Input shot point number */
    /* ----------------------- */
    if (*bufr EQUALS'*')
        {
        *shot_point = atoi (&bufr[7]);
        }
    else
        {
        status = -2;
        return status;
        }

    /* Get next line from file */
    /* ----------------------- */
vt:
    *ntv = 0;
tvloop:
    if ((fgets (bufr, buf_max, fhan)) EQUALS (CHAR *)0)
        {
        if (*ntv EQUALS 0)
            status = 1;
        return status;
        }
    *iflag += 1;
    
    /* If not t-v record, set ready flag and return */
    /* -------------------------------------------- */
    if (*bufr != 'V')
        {
        ready = 1;
        return status;
        }

    /* Store t-v data */
    /* -------------- */
    knt = 0;
    next = &bufr[6];
    while ((next = ts_next_word (next, tmp)) != (CHAR *)0)
        {
        knt += 1;
        if (knt& 1)
            {
            *time++ = atoi (tmp);
            }
        else
            {
            *vel++ = atoi (tmp);
            }
        }

    knt += 1;
    if (knt & 1)
        {
        *time++ = atoi (tmp);
        }
    else
        {
        *vel++ = atoi (tmp);
        }
    if (knt & 1)
        {
        status = -3;
        return status;
        }

    *ntv += knt / 2;
    if (!(knt % 6))
        goto tvloop;
    
    return status;
}   /* end of vi_get_velfn */
