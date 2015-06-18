/* DEC/CMS REPLACEMENT HISTORY, Element MP_DSP_TTL_BK.C*/
/* *6    17-AUG-1990 22:05:02 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    15-FEB-1990 11:12:44 GORDON "(SPR 0) Change bogus gstxci call to gsfaci"*/
/* *4    18-DEC-1989 12:33:18 CONROY "(SPR 5079) Set colors correctly"*/
/* *3    18-JUL-1989 10:56:38 GORDON "(SPR 100) Merge in UNIX changes"*/
/* *2    26-JUN-1989 10:55:10 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:11:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MP_DSP_TTL_BK.C*/
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

#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_wm.h"
#include "esi_qs.h"
#include "esi_tc.h"
#include "esi_lu.h"
#include "esi_gks.h"
#include "esi_wm_cls.h"

/* Function Description -----------------------------------------------------
Description:
    Function to display the supplied title block on the
    graphics screen.

Prototype:
    publicdef INT mp_display_title_block();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_display_title_block ()
#else
publicdef INT mp_display_title_block ()
#endif
    {
    PROGNAME ("MP_DISPLAY_TITLE_BLOCK");
    INT i;
    INT color;
    CHAR seg_name[10];
    MP_TTLBLK_VARS *ttlblk;
    FLOAT dsize, dummy;
    BOOL batch;
    static FLOAT size[5] = 
        {
        0.1,
        0.15,
        0.2,
        0.25,
        0.3
        };
    static FLOAT x_center[25] = 
        {
        2.0625,
        2.0625,
        2.0625,
        6.625,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        4.125,
        1.25,
        4.125,
        4.125,
        7.0
        };
    static FLOAT y_center[25] = 
        {
        10.68,
        10.28,
        9.88,
        10.48,
        9.28,
        8.48,
        7.68,
        6.88,
        6.18,
        5.58,
        5.08,
        4.68,
        4.28,
        3.88,
        3.48,
        3.08,
        2.68,
        2.28,
        1.88,
        1.48,
        1.08,
        0.48,
        0.68,
        0.28,
        0.48
        };
    static FLOAT x_rect[25][4] = 
        {
            {
            0.02,
            0.02,
            4.105,
            4.105
            },
            {
            0.02,
            0.02,
            4.105,
            4.105
            },
            {
            0.02,
            0.02,
            4.105,
            4.105
            },
            {
            5.02,
            5.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            8.23,
            8.23
            },
            {
            0.02,
            0.02,
            2.48,
            2.48
            },
            {
            2.52,
            2.52,
            5.73,
            5.73
            },
            {
            2.52,
            2.52,
            5.73,
            5.73
            },
            {
            5.77,
            5.77,
            8.23,
            8.23
            }
        };
    
    static FLOAT y_rect[25][4] = 
        {
            {
            10.52,
            10.82,
            10.82,
            10.52
            },
            {
            10.12,
            10.42,
            10.42,
            10.12
            },
            {
            9.72,
            10.02,
            10.02,
            9.72
            },
            {
            10.12,
            10.82,
            10.82,
            10.12
            },
            {
            8.92,
            9.62,
            9.62,
            8.92
            },
            {
            8.12,
            8.82,
            8.82,
            8.12
            },
            {
            7.32,
            8.02,
            8.02,
            7.32
            },
            {
            6.52,
            7.22,
            7.22,
            6.52
            },
            {
            5.92,
            6.42,
            6.42,
            5.92
            },
            {
            5.32,
            5.82,
            5.82,
            5.32
            },
            {
            4.92,
            5.22,
            5.22,
            4.92
            },
            {
            4.52,
            4.82,
            4.82,
            4.52
            },
            {
            4.12,
            4.42,
            4.42,
            4.12
            },
            {
            3.72,
            4.02,
            4.02,
            3.72
            },
            {
            3.32,
            3.62,
            3.62,
            3.32
            },
            {
            2.92,
            3.22,
            3.22,
            2.92
            },
            {
            2.52,
            2.82,
            2.82,
            2.52
            },
            {
            2.12,
            2.42,
            2.42,
            2.12
            },
            {
            1.72,
            2.02,
            2.02,
            1.72
            },
            {
            1.32,
            1.62,
            1.62,
            1.32
            },
            {
            0.92,
            1.22,
            1.22,
            0.92
            },
            {
            0.12,
            0.82,
            0.82,
            0.12
            },
            {
            0.52,
            0.82,
            0.82,
            0.52
            },
            {
            0.12,
            0.42,
            0.42,
            0.12
            },
            {
            0.12,
            0.82,
            0.82,
            0.12
            }
        };
    
    /****************************************************************************/
    
    am_open_workspace ("MP_TTLBLK_VARS", AM_APPLICATION, (VOIDPTR *) & ttlblk);
    
    /* * BRING THE WINDOW TO THE FRONT * */
    /* --------------------------------- */
    
    qs_inq_i (QS_IF_BATCH, &batch);
    if (NOT batch)
        {
        wm_inq_front_window (&i);
        if (i != ttlblk->window_id)
            {
            wmwfront (ttlblk->window_id);
            i = MAIN_WKID;
            wm_grsgwk (i);
            }
        }
    /* * OPEN THE WINDOW * */
    /* ------------------- */
    
    wmwopen (ttlblk->window_id);
    wmtselect (ttlblk->trans_id);
    
    /* * SET GKS ATTRIBUTES * */
    /* ---------------------- */
    
    wm_gstxp (GRIGHT);
    
    dsize = 0.0;
    dummy = 1.0;
    wm_gschup (dsize, dummy);
    wm_gschxp (dummy);
    wm_gschsp (dsize);
    
    wm_gstxfp (SIMPLEX, GSTRKP);
    
    wm_gstxal (GACENT, GAHALF);
    
    lu_text_to_index ("COLOR", &color, "NORMAL");
    wm_gstxci (color);
    
    dsize = size[ttlblk->size[0] - 1];
    wm_gschh (dsize);
    
    wm_gsfais (SOLID);
    
    lu_text_to_index ("COLOR", &color, "BACKGROUND");
    wm_gsfaci (color);
    
    /* UPDATE FIELD(S) */
    
    for (i = 0; i < 25; i++)
        {
        if (ttlblk->mod_field[i])
            {
            if (ttlblk->seg_id[i] != 0)
                {
                wmsdelete (ttlblk->seg_id[i]);
                ttlblk->seg_id[i] = 0;
                wm_gfa (4, x_rect[i], y_rect[i]);
                }
            if (strlen (ttlblk->text[i]) != 0)
                {
                wmsallocate (&(ttlblk->seg_id[i]));
                sprintf (seg_name, "FIELD #%d", i + 1);
                wmoset (ttlblk->seg_id[i], WM_CLASS_SEGMENT, "", seg_name);
                
                if (dsize != size[ttlblk->size[i] - 1])
                    {
                    dsize = size[ttlblk->size[i] - 1];
                    wm_gschh (dsize);
                    }
                wm_gtx (x_center[i], y_center[i], ttlblk->text[i]);
                wmsclose (ttlblk->seg_id[i]);
                }
            }
        }
    tc_zeroes (ttlblk->mod_field, 25 * sizeof(BOOL));
    wmwclose (ttlblk->window_id);
    
    return SUCCESS;
    }
/* END:     */
