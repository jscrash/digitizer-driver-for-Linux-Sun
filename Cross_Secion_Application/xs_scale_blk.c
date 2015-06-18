/* DEC/CMS REPLACEMENT HISTORY, Element XS_SCALE_BLK.C*/
/* *9    27-MAR-1991 13:30:54 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *8    26-OCT-1990 15:35:40 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *7    17-AUG-1990 22:43:59 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    20-APR-1990 18:31:21 JULIAN "(SPR 0) fix text leaking"*/
/* *5    20-SEP-1989 07:55:48 GORDON "Undo last checkin."*/
/* *4    19-SEP-1989 13:40:38 GILLESPIE "(SPR 100) GULF MODS"*/
/* *3    18-SEP-1989 12:45:37 JULIAN "Gulf mods under SPR 100"*/
/* *2    14-SEP-1989 16:24:42 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:45:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_SCALE_BLK.C*/
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
#include "esi_xs_gr.h"

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_TG_H
#include "esi_tg.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

typedef struct 
    {
    DOUBLE xmin, ymin, xmax;
    CHAR text[3][20];
    } SCALE_TEXT_STRUCT;

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Draw trace scale blocks.                                                      */
/*                                                                                  */
/*    Draw the trace scale blocks for each track in the                             */
/*    given well.  The size of the text as well as the                              */
/*    size of the individual trace scale annotation regions                         */
/*    is standardized for all the wells in advance.  The                            */
/*    information is passed in via a special drawing information                    */
/*    structure.                                                                    */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT lg_gr_plot_draw_scale(XS_STRUCT   *pxs,LAYOUT_STRUCT  *pls,XS_DIS *pd); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*    pls             -(LAYOUT_STRUCT *)                                            */
/*    pd              -(XS_DIS *)                                                   */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT lg_gr_plot_draw_scale (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, XS_DIS *pd)
#else
publicdef INT lg_gr_plot_draw_scale (pxs, pls, pd)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
XS_DIS *pd;
#endif
    {
    INT t, tt, st, ntracks, ntraces, tnum;
    
    DOUBLE a, b, c, d, ac, cd;
    DOUBLE char_height, minchh = 1000.0;
    DOUBLE y, x, xoff, loff, toff, base, ymax;
    LP_TRACE_STRUCT *ptrace;
    TRACK_STRUCT *pt;
    CHAR text[255];
    INDEX font, prec;
    COLOR_INDEX normal;
    CHAR units[32];
    
    LISTHEAD scale_text;
    SCALE_TEXT_STRUCT one_struct;
    
    GKS_REAL ext_x[5], ext_y[5];    /* the text extents array returned by GKS       */
    GKS_REAL cat_x, cat_y;          /* the text concatonation coords from GKS       */
    INT wkid;                       /* workstation id always is MAIN_WKID           */
    
    INT status;
    
    /************************* Start Code **************************                */
    
    wmtselect (pxs->drawing.tnr_id);    /* re-select Xform ....just-in-case....     */
    /* draw frame around scale block                                                */
    xs_gr_rect (pls->scale_block.xorigin, pls->scale_block.yorigin,
                (pls->scale_block.xorigin + pls->scale_block.width),
                (pls->scale_block.yorigin + pls->scale_block.height),
                pls->scale_block.line.color, pls->scale_block.line.width);
    
    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    
    wm_gschxp (1.0);                /* Char expansion factor. 1.0 = None            */

#if 0
    wm_gschh (pd->cs);              /* Character size                               */
#endif
    wm_gschsp (0.05);               /* Space between chars.                         */
    lu_text_to_index ("COLOR", &normal, "NORMAL");
    wm_gstxci (normal);             /* Color of text                                */
    
    /* toff = ((pd->region_ht - pd->cs) / 2.0)+ (pd->cs / 2.0);                     */
    /* loff = toff - (pd->cs / 2.0);                                                */
    
    toff = pd->region_ht * 0.40;    /* Trace name Y offset from region origin       */
    loff = pd->region_ht * 0.25;    /* Offset for trace sample line                 */
    xoff = loff * 0.15;             /* Trace name X offset form region origin       */
    
    ntracks = llcount (&pls->tracks);   /* Count # of tracks in linked list         */
    ntraces = llcount (&pls->traces);   /* Count # of traces.....                   */
    
    if (ntracks <= 0)
        return FAIL;
    
    /* char size depends on the space in scale block and the length of all text,    */
    /*while calculating char size we store text and its position so we don't have to*/
    /*retrieve them again                                                           */
    
    llinit (&scale_text, 0, sizeof(SCALE_TEXT_STRUCT), 0);
    
    for (t = 0; t < ntracks; t++)
        {
        st = (t) ? llnext (&pls->tracks, (BYTE *)&pt) : llfirst (&pls->tracks,
                                                                 (BYTE *)&pt);
        
        a = pt->xorigin;            /* left most x of track                         */
        b = a + pt->width;          /* right most x of track                        */
        c = a + xoff;
        d = b - xoff;
#if 0
        rlwid = pd->cs * 6.0;
        c = a + rlwid;              /* start of tname zone                          */
        d = b - rlwid;              /* end of tname zone                            */
        ac = c - a;                 /* width of lr string zone                      */
        cd = d - c;                 /* width of tname string zone                   */
#endif
        base = pls->scale_block.yorigin;
        for (tnum = tt = 0; tt < ntraces; tt++)
            {
            st = (tt) ? llnext (&pls->traces,
                                (VOIDPTR)&ptrace) : llfirst (&pls->traces,
                                                             (VOIDPTR)&ptrace);
            
            if (ptrace->track_num != pt->number)
                continue;
            
            if (tnum > 0)
                {
                base = (pls->scale_block.yorigin + ((DOUBLE)tnum * pd->region_ht));
                }
            y = base + toff;
            ymax = pls->scale_block.yorigin + pls->scale_block.height - (toff * 0.5);
            
            tnum++;
            
            /* draw the trace name itself and log units                             */
            
            strcpy (units, ptrace->trace_struct.trace_uom);
            
            if (ARE_SAME (ptrace->trace_units, "T"))
                {
                if (ARE_SAME (units, "MM"))
                    strcpy (units, "IN");
                else if (ARE_SAME (units, "K/M3"))
                    strcpy (units, "G/C3");
                else if (ARE_SAME (units, "US/M"))
                    strcpy (units, "US/F");
                }
            else if (ARE_SAME (ptrace->trace_units, "M"))
                {
                if (ARE_SAME (units, "IN"))
                    strcpy (units, "MM");
                else if (ARE_SAME (units, "G/C3"))
                    strcpy (units, "K/M3");
                else if (ARE_SAME (units, "US/F"))
                    strcpy (units, "US/M");
                }
            /* calculate char_height for each text and keep the smallest            */
            sprintf (text, "%-6.2f %s (%s) %6.2f", ptrace->left,
                     ptrace->trace_struct.trace_name, units, ptrace->right);
            status = tg_fit_string (text, c, y, d, ymax, pd->cs, &char_height);
            if (status != SUCCESS)
                goto error_status;
            if (minchh > char_height)
                minchh = char_height;
            
            /* store text and its position to be drawn later                        */
            one_struct.xmin = c;
            one_struct.ymin = y;
            one_struct.xmax = d;
            sprintf (one_struct.text[0], "%-6.2f", ptrace->left);
	    if (IS_EMPTY_STRING(units) == FALSE)
	      {
		sprintf (one_struct.text[1], "%s (%s)", ptrace->trace_struct.trace_name,
			 units);
	      }
	    else /* get rid of () if trace units is empty (ie. no info) */
	      {
		sprintf (one_struct.text[1], "%s", ptrace->trace_struct.trace_name);
	      }
            sprintf (one_struct.text[2], "%6.2f", ptrace->right);
            llappnd (&scale_text, (BYTE *)&one_struct);
            
            wm_gsln (ptrace->line.style);   /* Set GKS line type (solid,dotted,etc) */
            wm_gsplci (ptrace->line.color); /* "   "   line color                   */
            wm_gslwsc (ptrace->line.width); /* "   "   line thickness (x1, x2, xn ) */
            
            y = base + loff;
            xs_gr_drawline (a, y, b, y);    /* Draw Trace sample line               */
            }
        }
    /* draw scale block text                                                        */
    wm_gschh (minchh);              /* Character size                               */
    
    wkid = MAIN_WKID;               /* Always seems to work! MAIN_WKID is from ESI_GKS.H    */
    
    tnum = llcount (&scale_text);
    for (t = 0; t < tnum; t++)
        {
        st = (t) ? llnext (&scale_text,
                           (BYTE *)&one_struct) : llfirst (&scale_text,
                                                           (BYTE *)&one_struct);
        
        wm_gtx (one_struct.xmin, one_struct.ymin, one_struct.text[0]);
        
        wm_gqtxx (wkid, one_struct.xmin, one_struct.ymin, one_struct.text[1], &status,
                  &cat_x, &cat_y, ext_x, ext_y);
        if (status != SUCCESS)
            goto error_status;
        cd = (one_struct.xmax - one_struct.xmin);
        ac = (ext_x[1] - ext_x[0]);
        x = one_struct.xmin + ((cd - ac) / 2.0);
        wm_gtx (x, one_struct.ymin, one_struct.text[1]);
        
        wm_gqtxx (wkid, one_struct.xmin, one_struct.ymin, one_struct.text[2], &status,
                  &cat_x, &cat_y, ext_x, ext_y);
        if (status != SUCCESS)
            goto error_status;
        ac = (ext_x[1] - ext_x[0]);
        x = one_struct.xmax - ac;
        wm_gtx (x, one_struct.ymin, one_struct.text[2]);
        }
    llfree (&scale_text);
    return SUCCESS;
    
error_status:
    
    llfree (&scale_text);
    return status;
    }
/* END:                                                                             */
