/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_DISP_TOPS.C*/
/*  20    6-MAR-1992 16:37:33 JESSIE "(SPR 6180) add printf command for testing in TVD display"*/
/*  19    3-MAR-1992 11:16:38 JILL "(SPR 0) set line.width=3 (was 1 in xs_zz_draw_line)"*/
/*  18   27-FEB-1992 14:40:50 JESSIE "(SPR 6180) SP=6180"*/
/*  17   27-FEB-1992 09:23:14 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *16    3-MAY-1991 16:31:42 KEE "(SPR -1) Bug fix when porting MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *15   27-MAR-1991 13:28:31 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *14   12-OCT-1990 23:11:01 GILLESPIE "(SPR 100) Merge Ernie deltas"*/
/*  12B1 12-OCT-1990 18:04:46 GILLESPIE "Merge Ernie Deltas"*/
/* *13   17-AUG-1990 22:34:14 VINCE "(SPR 5644) Code Cleanup"*/
/* *12    4-JUL-1990 21:10:36 SCD "(SPR 1) Rewrite top name posting"*/
/* *11   23-APR-1990 12:28:07 JULIAN "(SPR 0) fix text aspect ratio"*/
/* *10   20-APR-1990 18:30:41 JULIAN "(SPR 0) fix text leaking"*/
/* *9    18-APR-1990 10:06:17 JULIAN "(SPR 1) fix depth tracks bad aspect ratio"*/
/* *8    14-APR-1990 16:27:12 SCD "(SPR 1) Eliminate redundant screen refreshes."*/
/* *7    17-JAN-1990 15:41:30 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *6     2-NOV-1989 10:53:13 GILLESPIE "(SPR 1) Fix data types to quiet compiler"*/
/* *5    19-SEP-1989 13:40:12 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    15-SEP-1989 08:53:56 GORDON "(SPR 100) ddd"*/
/* *3    14-SEP-1989 16:23:21 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    20-JUL-1989 12:58:48 GORDON "(SPR 100) Sun port mods"*/
/* *1    19-JUN-1989 13:42:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_DISP_TOPS.C*/
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

#include "esi_c_lib.h"
#include "esi_xs_gr.h"

#ifndef ESI_TG_H
#include "esi_tg.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#define DEBUG  FALSE
#define TOP_NAME_LENGTH 10
#define SEG_RETURN(stat)    wmsclose(ptinfo->seg_id);   \
                            wmsdelete(ptinfo->seg_id);  \
                            return (stat)
        
#define MAX_DEPTH  500000.00   /* impossibly big */
/* --------------------------------------- */
/* The static variable char_height is      */
/* computed when the current cross section */
/* is drawn.  It may be later used by such */
/* functions as tops digitizing via the    */
/* public function xs_gr_draw_top          */
/* --------------------------------------- */
static DOUBLE char_height = 0;
static UINT ndim = 2, blocksize = 100;
static UINT dim_list[2] = 
    {
    1,
    2
    };



static INT units[2] = 
    {
    0,
    0
    };



static size_t data_sizes[2] = 
    {
    sizeof(FLOAT), sizeof(FLOAT)
    };



static INT data_types[2] = 
    {
    FLOAT_TYPE,
    FLOAT_TYPE
    };



static INT value_inc[2] = 
    {
    VALUE_LIST,
    VALUE_LIST
    };



static VOIDPTR Var_List[2];
static CHAR *c_name[3] = 
    {
    "X",
    "Y",
    NULL
    };



#if USE_PROTOTYPES
static INT xs_gr_nlist_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo,
                                     NLIST_HEADER nlist, INT use_list);
static INT xs_zz_draw_line (DOUBLE x1, DOUBLE x2, DOUBLE y1, DOUBLE y2, COLOR_INDEX color);

#else       /* no prototypes */
static INT xs_gr_nlist_draw_top ();
static INT xs_zz_draw_line ();

#endif      /* of #if USE_PROTOTYPES */

/* File Description ---------------------------------------------------------
Overview:
    Graphics cross section routines.
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT xsz_display_tops (XS_STRUCT *pxs, DOUBLE min_char_height);

    publicdef INT xs_gr_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo);

Private_Functions:
    static INT xs_gr_nlist_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo,
        NLIST_HEADER nlist, INT use_list);

    static INT xs_zz_draw_line (DOUBLE x1, DOUBLE x2, DOUBLE y1, DOUBLE y2,
        COLOR_INDEX color);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Cross Section Graphics Entry routine; Draw tops on a layout

    Traverse the linked list of tops and call xs_gr_nlist_draw_top
    with each one that is flagged as visible.  For each top drawn, 
    add a stroke to the LAYERS nlist containing the points for
    that set of tops.

Prototype:
    publicdef INT xsz_display_tops (XS_STRUCT *pxs, DOUBLE min_char_height);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) Pointer to the Cross Section object.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_display_tops (XS_STRUCT *pxs, DOUBLE min_char_height)
#else
publicdef INT xsz_display_tops (pxs, min_char_height)
XS_STRUCT *pxs;
DOUBLE min_char_height;
#endif
    {
    INT t, l,                       /*  loop control                 */
        ntps,                       /*  top count                    */
        nlts,                       /*  layout (i.e., # wells) count */
        dummy, stat;                /*  status                       */
    INT ll_stat;
    INT font, prec;
    
    LAYOUT_STRUCT *pls;             /*  ptr to a layout struct                  */
    TOP_INFO *ptinfo;               /*  contains btree of tops keyed by UWI     */
    UINT stroke;                    /*  nlist definitions                       */
    INT batch = FALSE;
    
    DOUBLE scr_xmin, scr_xmax,      /*  Range of the drawing avail for tops */
        scr_ymin, scr_ymax,         /*  Range of the drawing height         */
        log_xmin = HUGE_VAL,        /*  Range of the drawing taken by wells */
    log_xmax = -HUGE_VAL, text_max_width,   /*  No top string can be wider          */
    chh;                            /*  size of GKS text for tops.          */
    
    /*  check entry conditions              */
    if (pxs == ((XS_STRUCT *)0))
        {
        return FAIL;                /*  our object has been stomped on!         */
        }
    wmtselect (pxs->drawing.tnr_id);
    /*  how many tops to deal with?         */
    ntps = llcount (&pxs->drawing.top_table);
    
    /*  linked list o.k. ?              */
    if (ntps <= 0)
        {
        return FAIL;
        }
    /*  determine if we are running in batch mode   */
    qs_inq_i (QS_IF_BATCH, &batch);
    
    if (!batch)
        {
        /* -------------------------------------------- */
        /*  we are going to create or re-create the     */
        /*  nlist of tops for the modelling package.    */
        /*  Check the nlist, and if neccessary delete   */
        /*  and free the old one                        */
        /* -------------------------------------------- */
        stat = nl_inq_nlist_int (pxs->layers, NL_NLIST_NDIM, &dummy);
        if (stat == SUCCESS)
            {
            /*  there is a valid nlist here, delete it.     */
            nl_free_nlist (pxs->layers);
            }
        /*  create our new nlist                */
        stat = nl_init_nlist (&pxs->layers, ndim, units, data_sizes, data_types,
                              value_inc, blocksize);
        stat = nl_set_nlist_info (pxs->layers, NL_NLIST_COLUMN_NAME, ndim, dim_list,
                                  (VOIDPTR)c_name);
        if (stat != SUCCESS)
            {
            return stat;
            }
        }
    /* ---------------------------------------- */
    /*  Set the character size to use for tops: */
    /*  We compute the largest char size which  */
    /*  will fit within the smaller of left and */
    /*  right margins for any of the tops       */
    /* ---------------------------------------- */
    /*  First, set the boundaries of the screen  */
    /*  as is relevent to tops                   */
    scr_xmin = pxs->drawing.xorigin;    /*  left origin                     */
    scr_xmax = pxs->drawing.sc_bar_xorigin; /*  max x before sc bar territory   */
    scr_ymin = pxs->drawing.yorigin;
    scr_ymax = scr_ymin + pxs->drawing.height;
    
    /*  Preliminary text size is   */
    /*  1.0% of the screens height */
    chh = ABS (scr_ymax - scr_ymin) * 0.01;
    
    /* ------------------------------------------ */
    /*  Next, examine the wells via their layout  */
    /*  structs to find the left and right limits */
    /*  of the tracks.                            */
    /* ------------------------------------------ */
    
    nlts = llcount (&pxs->drawing.log_plots);
    if (nlts <= 0)
        {
        return FAIL;
        }
    for (l = 0; l < nlts; l++)
        {
        ll_stat = (l == 0) ? llfirst (&pxs->drawing.log_plots,
                                      (BYTE *) & pls) :
                  llnext (&pxs->drawing.log_plots, (BYTE *) & pls);
        
        if (ll_stat != SUCCESS)
            {
            return FAIL;
            }
        log_xmin = MIN (log_xmin, pls->xorigin);
        log_xmax = MAX (log_xmax, pls->xorigin + pls->width);
        }
    text_max_width = MIN (ABS (log_xmin - scr_xmin), ABS (scr_xmax - log_xmax));
    /* Reduce allowed width to allow a 1/2 nominal */
    /* char width margin on each side of the text  */
    text_max_width -= chh;
    
    /* ------------------------------------- */
    /*  Now compute the smallest char height */
    /*  which will satisfy all of the tops   */
    /* ------------------------------------- */
#if 0
    char_height = HUGE_VAL;
    for (t = 0; t < ntps; t++)
        {
        stat = (t == 0) ? llfirst (&pxs->drawing.top_table,
                                   (BYTE *) & ptinfo) :
               llnext (&pxs->drawing.top_table, (BYTE *) & ptinfo);
        
        tg_fit_string (ptinfo->top, 0.0, scr_ymin, text_max_width, 0.0, chh, &chh);
        char_height = MIN (char_height, chh);
        }
#else
    char_height = min_char_height;
#endif

    /* ------------------------------------- */
    /* Set the GKS text attributes that will */
    /* be constant for every top label       */
    /* ------------------------------------- */
    wm_gschh (char_height);
    wm_gschxp (1.0);
    
    /*  select the SIMPLEX font for annotating this */
    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    
    /* ------------------------------------ */
    /*  Finally, we can to draw the         */
    /*  tops correlation lines, and label   */
    /*  with the formation names            */
    /* ------------------------------------ */
    for (t = 0; t < ntps; t++)
        {
        stat = (t == 0) ? llfirst (&pxs->drawing.top_table,
                                   (BYTE *) & ptinfo) :
               llnext (&pxs->drawing.top_table, (BYTE *) & ptinfo);
        
        /*  is this list any good?              */
        if (stat < 0 || (stat == FAIL))
            {
            return FAIL;            /*  they just don't make 'em like they used to! */
            }
        if (!batch)
            {
            /*  each set of tops-connections is in a        */
            /*  seperate stroke                 */
            stat = nl_start_stroke (pxs->layers, &stroke);
            }
        if (ptinfo->visible == TRUE)
            {
            /*  if visible, draw it             */
            stat = (batch) ? xs_gr_nlist_draw_top (pxs, ptinfo, pxs->layers,
                                                   FALSE) :
                   xs_gr_nlist_draw_top (pxs, ptinfo, pxs->layers, TRUE);
            }
        else
            {
            /*  if its not visible, delete the segment      */
            if (ptinfo->seg_id > 0)
                {
                stat = wmsdelete (ptinfo->seg_id);
                ptinfo->seg_id = 0;
                }
            }
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Cross Section Graphics Entry routine; Draw valid tops associated with ptinfo.

    This routine traverses the btree of tops on a well by well
    basis displaying all tops that have a valid measured depth.
    An invalid depth is marked by a number > (HUGE / 2.0).

    For each top pos. drawn, the coords of that point are added 
    to the supplied nlist.  This nlist is used later by modelling.

Prototype:
    static INT xs_gr_nlist_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo, NLIST_HEADER nlist,
        use_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Main Cross Section object.
    ptinfo          -(TOP_INFO *) Pointer to a top info struct.
    nlist           -(NLIST_HEADER)
    use_list        -(INT) Set to TRUE if nlist is to be filled.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <xsz_disp_tops.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_gr_nlist_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo,
                                     NLIST_HEADER nlist, INT use_list)
#else
static INT xs_gr_nlist_draw_top (pxs, ptinfo, nlist, use_list)
XS_STRUCT *pxs;
TOP_INFO *ptinfo;
NLIST_HEADER nlist;
INT use_list;
#endif
    {
    
    DOUBLE old_xmax = 0.0;
    GKS_REAL this_xmin = 0.0,       /* min & max coords of current layout      */
    this_xmax = 0.0;
    
    DOUBLE scr_xmin,                /*  WCS X coords of edges of usable screen area */
        scr_xmax, scr_ymin,         /*  WCS Y of top & bottom of window         */
        scr_ymax;                   /*  size of GKS text.               */
    
    INT nlts,                       /*  # of layouts to proccess            */
        l,                          /*  loop counter for layouts            */
        drawn,                      /*  TRUE after at least one top is drawn        */
        ll_stat,                    /*  status for linked list operations       */
        bt_stat,                    /*  status for btree operations         */
        bad_interval;               /*  depth interval validity. (for each well)    */
    
    INT left_align,                 /*  text alignment codes */
        right_align, vertical_align;
    
    TOP_STRUCT top;                 /*  top data structure              */
    
    LAYOUT_STRUCT *pls;             /*  ptr to a layout struct              */
    
    INT gen_int;                    /*  general purpose integer             */
    DOUBLE gen_float;               /*  general purpose float               */
    
    DOUBLE text_x,                  /*  text X,Y coord vars.                */
        text_y, text_xmax,          /*  maximum extent of text string on screen     */
        y,                          /*  general purpose Y calculating var.      */
        old_y;                      /*  last value of y before current          */
    INT last_well_valid;
    
    TRACK_STRUCT *pt;               /*  ptr to a track struct               */
    
    CHAR top_name[32];              /*  buffer for top name             */
    
    
    /************************* start of code *************************/
    /* ---------------------------------------- */
    /* Save the text alignment codes, so we can */
    /* use as needed to align each top string   */
    /* ---------------------------------------- */
    lu_text_to_index ("TEXT_HORIZ_ALIGN", &left_align, "LEFT");
    lu_text_to_index ("TEXT_HORIZ_ALIGN", &right_align, "RIGHT");
    lu_text_to_index ("TEXT_VERT_ALIGN", &vertical_align, "HALF");
    
    /*  set GKS attributes for drawing  starting    */
    /*  with the line type              */
    if (ptinfo->line.style <= 0 || (ptinfo->line.style > 4))
        {
        gen_int = 1;                /*  defualt is solid line               */
        wm_gsln (gen_int);
        }
    else
        {
        wm_gsln (ptinfo->line.style);
        }
    /*  set the line thickness              */
    if (ptinfo->line.width <= 0.0 || (ptinfo->line.width > 4.0))
        {
        gen_float = 1.0;            /*  def.                        */
        wm_gslwsc (gen_float);
        }
    else
        {
        wm_gslwsc (ptinfo->line.width);
        }
    /*  if there is a top color, use it. if not     */
    /*  make it NORMAL                  */
    if (ptinfo->line.color >= 0)
        {
        wm_gstxci (ptinfo->line.color);
        }
    else
        {
        lu_text_to_index ("COLOR", (INDEX *)&gen_int, "NORMAL");
        wm_gstxci (gen_int);
        }
    /*  set the boundaries of the screen as is */
    /*  relevent to tops                       */
    scr_xmin = pxs->drawing.xorigin;
    scr_xmax = pxs->drawing.sc_bar_xorigin;
    scr_ymin = pxs->drawing.yorigin;
    scr_ymax = scr_ymin + pxs->drawing.height;
    
    /*  delete the old segment for this top if any      */
    if (ptinfo->seg_id != 0)
        {
        wmsdelete (ptinfo->seg_id);
        }
    /*  and create a new segment                */
    wmsallocate (&ptinfo->seg_id);
    
    /*  have'nt started yet...              */
    drawn = FALSE;
    
    nlts = llcount (&pxs->drawing.log_plots);
    if (nlts <= 0)
        {
        SEG_RETURN (FAIL);
        }
    /* ------------------------------------------------------ */
    /* Outer loop:  Scan through wells for first one to have  */
    /* a valid top.  This may or may not be the leftmost well.*/
    /* ------------------------------------------------------ */
    for (l = 0; l < nlts; l++)
        {
        ll_stat = (l == 0) ? llfirst (&pxs->drawing.log_plots,
                                      (BYTE *) & pls) :
                  llnext (&pxs->drawing.log_plots, (BYTE *) & pls);
        
        if (ll_stat < 0)
            {
            SEG_RETURN (FAIL);      /*  linked list error - ABORT       */
            }
        old_xmax = this_xmax;       /*  to remember where to connect lines from */
        this_xmin = pls->xorigin;   /*  current xmin, xmax of well layout   */
        this_xmax = pls->xorigin + pls->width;
        
        /*  get an arbitrary track & use its           */
        /*  xmin,xmax,ymin,ymax etc. for depth to Y    */
        /*  calculations                               */
        ll_stat = llfirst (&pls->tracks, (BYTE *) & pt);
        
        /*  Retrieve a top for this well        */
        bt_stat = btfind (&ptinfo->uwis, (BYTE *)pls->pwell->uwi, (BYTE *) & top);
        
        /*  now try to eliminate this well, and */
        /*  skip on to the next  btree          */
        if (bt_stat != TRUE || (top.Depth.md > MAX_DEPTH) ||/* no top for this well */
            (ll_stat < 0) ||        /* bad track linked list */
            (pls->interval.track_base == pls->interval.track_top))
            {
            continue;               /*  move on to next well                */
            }
        /*  if we get here then we've got the first     */
        /*  good well in case there are no more, make a */
        /*  note of our position                */
        
        if (l == 0)
            /* --------------------------- */
            /*  This is the left most well */
            /* --------------------------- */
            {
            /*  translate the depth into a Y coord */
            y = (pt->yorigin + pt->length) - ((top.Depth.md -
                                               pls->interval.track_top) *
                                              (pt->length / (pls->interval.track_base -
                                                             pls->interval.
                                                             track_top)));
            
            /*  did Y come out reasonable?          */
            if (y < scr_ymin || (y > scr_ymax))
                {
                continue;           /*  something went wrong, skip this well    */
                }
            }
        else                        /* ----------------------------------------- */
                                    /* The first top is not on the leftmost well */
                                    /* ----------------------------------------- */
            {
            
            if (pls->interval.track_base == pls->interval.track_top)
                {
                bad_interval = TRUE;
                }
            else
                {
                bad_interval = FALSE;
                
                /*  transform the depth into a Y coord      */
                y = (pt->yorigin + pt->length) - ((top.Depth.md -
                                                   pls->interval.track_top) *
                                                  (pt->length /
                                                   (pls->interval.track_base -
                                                    pls->interval.track_top)));
                }
            /*  did Y come out reasonable?          */
            if (y < scr_ymin || (y > scr_ymax) || bad_interval == TRUE)
                {
                continue;           /*  something went wrong, skip this well    */
                }
            }
        /* ---------------------- */
        /* Draw top right aligned */
        /* ---------------------- */
        wm_gstxal (right_align, vertical_align);
        wm_gtx (this_xmin - char_height / 2, y, ptinfo->top);
        
        /* ----------------- */
        /*  Draw the top bar */
        /* ----------------- */
        xs_zz_draw_line (this_xmin, this_xmax, y, y, ptinfo->line.color);
        
        /*  add this point to nlist */
        if (use_list == TRUE)
            {
            Var_List[0] = (VOIDPTR) & this_xmin;
            Var_List[1] = (VOIDPTR) & y;
            nl_add_points (nlist, 1, ndim, dim_list, Var_List);
            }
        drawn = TRUE;
        break;
        }
    /* -------------------------------------------- */
    /*  at this point we have scanned to the first  */
    /*  valid well, drawn the top name and a bar    */
    /*  across the well.  Now we move along the     */
    /*  rest of the wells.                          */
    /* -------------------------------------------- */
    
    last_well_valid = TRUE;         /*  implicit if we made it here.            */
    
    for (l++; l < nlts; l++)
        {
        ll_stat = llnext (&pxs->drawing.log_plots, (VOIDPTR) & pls);
        
        if (ll_stat < 0)
            {
            SEG_RETURN (FAIL);      /*  linked list error - ABORT       */
            }
        /*  store the Y pos on the previous */
        /*  well for connecting lines       */
        old_y = y;
        
        old_xmax = this_xmax;       /* remember where to connect lines from */
        this_xmin = pls->xorigin;   /*  current xmin, xmax          */
        this_xmax = pls->xorigin + pls->width;
        
        /*  get an arbitrary track & use its        */
        /*  xmin,xmax,ymin,ymax etc. for depth to Y */
        /*  calculations                            */
        ll_stat = llfirst (&pls->tracks, (VOIDPTR) & pt);
        
        /*  try retrieving a top for this well      */
        bt_stat = btfind (&ptinfo->uwis, (VOIDPTR)pls->pwell->uwi, (VOIDPTR) & top);
        
        if (pls->interval.track_base == pls->interval.track_top)
            {
            bad_interval = TRUE;
            }
        else
            {
            bad_interval = FALSE;
            
            /*  transform the depth into a Y coord      */
            y = (pt->yorigin + pt->length) - ((top.Depth.md -
                                               pls->interval.track_top) *
                                              (pt->length / (pls->interval.track_base -
                                                             pls->interval.
                                                             track_top)));
            }
        /* ------------------------------------ */
        /*  now try to eliminate this well, and */
        /*  skip on to the next well            */
        /* ------------------------------------ */
        if (bt_stat != TRUE ||      /*  bad btree             */
            (top.Depth.md > MAX_DEPTH) ||   /*  no top for this well  */
            (ll_stat < 0) ||        /*  bad track linked list */
            (y < scr_ymin) || (y > scr_ymax) || (bad_interval == TRUE))
            {
            
            /* ----------------------------------- */
            /* Since this well will not have a top */
            /* displayed, we need to label the top */
            /* at the right of the previous well   */
            /* using left aligned text             */
            /* ----------------------------------- */
            if (last_well_valid == TRUE)
                {
                wm_gstxal (left_align, vertical_align);
                wm_gtx (old_xmax + char_height / 2.0, old_y, ptinfo->top);
                }
            last_well_valid = FALSE;
            continue;               /* move on to next well */
            }
        /*  arrive here with a good well top.  Check to see */
        /*  if we skipped the last one or more wells        */
        if (last_well_valid == FALSE)
            {
            /* ------------------------------------ */
            /* if we did skip well(s), then we need */
            /* to label the top to the left of this */
            /* well with right aligned text         */
            /* ------------------------------------ */
            
            wm_gstxal (right_align, vertical_align);
            wm_gtx (this_xmin - char_height / 2.0, y, ptinfo->top);
            
            /*  draw the bar across this well */
            xs_zz_draw_line (this_xmin, this_xmax, y, y, ptinfo->line.color);
            
            /*  add this point to nlist */
            if (use_list == TRUE)
                {
                Var_List[0] = (VOIDPTR) & this_xmin;
                Var_List[1] = (VOIDPTR) & y;
                nl_add_points (nlist, 1, ndim, dim_list, Var_List);
                }
            }
        else
            {
            /*  last well was good, connect with it */
            xs_zz_draw_line (old_xmax, this_xmin, old_y, y, ptinfo->line.color);
            
            /*  add this point to nlist */
            if (use_list == TRUE)
                {
                Var_List[0] = (VOIDPTR) & this_xmin;
                Var_List[1] = (VOIDPTR) & y;
                nl_add_points (nlist, 1, ndim, dim_list, Var_List);
                }
            xs_zz_draw_line (this_xmin, this_xmax, y, y, ptinfo->line.color);
            }
        last_well_valid = TRUE;
        }
    /* ------------------------------------------------------------ */
    /* at this point we have finished all the wells. All that is    */
    /* left to do is draw the top name on the right most valid well */
    /* ------------------------------------------------------------ */
    /*  if no drawing has occurred yet, cancel the  */
    /*  segment and abort               */
    if (drawn == FALSE)
        {
        /*  nothing was ever drawn so close */
        /*  and delete  this segment        */
        wmsclose (ptinfo->seg_id);
        wmsdelete (ptinfo->seg_id);
        return SUCCESS;
        }
    if (last_well_valid == TRUE)
        {
        
        /* --------------------- */
        /* Draw top left aligned */
        /* --------------------- */
        wm_gstxal (left_align, vertical_align);
        wm_gtx (this_xmax + char_height / 2.0, y, ptinfo->top);
        }
    /*  close and name the segment          */
    wmsclose (ptinfo->seg_id);
#if 0
    wmsvis (ptinfo->seg_id, GVISI);
#endif
    sprintf (top_name, "Top: %s", ptinfo->top);
    wmoset (ptinfo->seg_id, WM_CLASS_SEGMENT, "", top_name);
    
    return SUCCESS;                 /*  SUCCESS whether any drawing or not      */
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This is a simple here-to-there line draw routine 
    used exclusively by xs_gr_draw_top.

Prototype:
    static INT xs_zz_draw_line (DOUBLE x1, DOUBLE x2, DOUBLE y1, DOUBLE y2, 
        COLOR_INDEX color);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x1              -(DOUBLE)
    x2              -(DOUBLE)
    y1              -(DOUBLE)
    y2              -(DOUBLE)
    color           -(COLOR_INDEX)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <xsz_disp_tops.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT xs_zz_draw_line (DOUBLE x1, DOUBLE x2, DOUBLE y1, DOUBLE y2, COLOR_INDEX color)
#else
static INT xs_zz_draw_line (x1, x2, y1, y2, color)
DOUBLE x1, x2, y1, y2;
COLOR_INDEX color;
#endif
    {
    LINE_ATTRIBUTE line;
    INT lnpts;
    
    FLOAT xarray[10], yarray[10];
    
    line.color = color;
    line.style = (INDEX)1;
    line.width = 1.0;
    
    wm_gsln (line.style);
    wm_gsplci (line.color);
    wm_gslwsc (line.width);
    
    xarray[0] = x1;
    xarray[1] = x2;
    yarray[0] = y1;
    yarray[1] = y2;
    
    lnpts = 2;
    wm_gpl (lnpts, xarray, yarray);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This is a shell around XS_GR_NLIST_DRAW_TOP that allows
    programs to draw tops without caring about the nlist .

Prototype:
    publicdef INT xs_gr_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *)
    ptinfo          -(TOP_INFO *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_gr_draw_top (XS_STRUCT *pxs, TOP_INFO *ptinfo)
#else
publicdef INT xs_gr_draw_top (pxs, ptinfo)
XS_STRUCT *pxs;
TOP_INFO *ptinfo;
#endif
    {
    NLIST_HEADER dummy_nlist;
    
    /*  make a call to the normal tops drawing      */
    /*  routine with the 'use nlist' flag set to    */
    /*  FALSE                                       */
    
    xs_gr_nlist_draw_top (pxs, ptinfo, dummy_nlist, FALSE);
    
    return SUCCESS;
    }
/* END:     */
