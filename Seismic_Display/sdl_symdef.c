/* DEC/CMS REPLACEMENT HISTORY, Element SDL_SYMDEF.C*/
/* *2    17-AUG-1990 22:14:13 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:23:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SDL_SYMDEF.C*/
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

#include "esi_c_lib.h"
#include "esi_ho_files.h"
#include "esi_wi.h"
#include "esi_ps.h"
#include "esi_ci.h"
#include "esi_cs.h"
#include "esi_sdl.h"
#include "esi_sdl_mac.h"
#include "esi_math.h"
static INT sdl_Status = 0;
static ENTRYNAME sdl_Group = 
    {
    0
    };


static INT sdl_Symbol = 0;
static INT sdl_Type = 0;
static INT sdl_NumPrims = 0;
static INT sdl_NumPoints = 0;
static INT sdl_PrimNpts = 0;
static INT sdl_Types[CS_MAX_PRIMITIVES] = 
    {
    0
    };


static INT sdl_Npts[CS_MAX_PRIMITIVES] = 
    {
    0
    };


static DOUBLE sdl_Px[CS_MAX_POINTS] = 
    {
    0.0
    };


static DOUBLE sdl_Py[CS_MAX_POINTS] = 
    {
    0.0
    };



static VOID sdl_start_group ();
static VOID sdl_end_group ();
static VOID sdl_start_symbol ();
static VOID sdl_end_symbol ();
static VOID sdl_start_primitive ();
static VOID sdl_end_primitive ();
static VOID sdl_add_point ();
static VOID sdl_line ();
static VOID sdl_filled_circle ();
static VOID sdl_hollow_circle ();
static VOID sdl_filled_rect ();
static VOID sdl_hollow_rect ();
static VOID sdl_gather_points ();
static VOID sdl_polyline ();
static VOID sdl_filled_area ();
static VOID sdl_radial_segment ();

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sdl_init();
    
    publicdef INT sdl_term();
    
    publicdef INT sdl_load(ENTRYNAME group );
    
    publicdef INT sdl_InitPolymarkerGroup();

Private_Functions:
    static VOID sdl_start_group();

    static VOID sdl_end_group();

    static VOID sdl_start_symbol();

    static VOID sdl_end_symbol();
    
    static VOID sdl_start_primitive();
    
    static VOID sdl_end_primitive();
    
    static VOID sdl_add_point();

    static VOID sdl_line();

    static VOID sdl_filled_circle();

    static VOID sdl_hollow_circle();

    static VOID sdl_filled_rect();

    static VOID sdl_hollow_rect();

    static VOID sdl_gather_points();

    static VOID sdl_polyline();

    static VOID sdl_filled_area();

    static VOID sdl_radial_segment();

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sdl_init();

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
publicdef INT sdl_init ()
#else
publicdef INT sdl_init ()
#endif
    {
    PROGNAME ("sdl_init");
    
    INT status = SUCCESS;
    
    ci_add_command ("sdl_start_group", sdl_start_group);
    ci_add_command ("sdl_end_group", sdl_end_group);
    ci_add_command ("sdl_start_symbol", sdl_start_symbol);
    ci_add_command ("sdl_end_symbol", sdl_end_symbol);
    ci_add_command ("sdl_start_primitive", sdl_start_primitive);
    ci_add_command ("sdl_end_primitive", sdl_end_primitive);
    ci_add_command ("sdl_add_point", sdl_add_point);
    ci_add_command ("sdl_line", sdl_line);
    ci_add_command ("sdl_filled_circle", sdl_filled_circle);
    ci_add_command ("sdl_hollow_circle", sdl_hollow_circle);
    ci_add_command ("sdl_filled_rect", sdl_filled_rect);
    ci_add_command ("sdl_hollow_rect", sdl_hollow_rect);
    ci_add_command ("sdl_gather_points", sdl_gather_points);
    ci_add_command ("sdl_polyline", sdl_polyline);
    ci_add_command ("sdl_filled_area", sdl_filled_area);
    ci_add_command ("sdl_radial_segment", sdl_radial_segment);
    
    status = SUCCESS;
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sdl_term();

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
publicdef INT sdl_term ()
#else
publicdef INT sdl_term ()
#endif
    {
    PROGNAME ("sdl_term");
    
    INT status = SUCCESS;
    
    status = SUCCESS;
    
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sdl_load(ENTRYNAME group );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    group           -(ENTRYNAME)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sdl_load (ENTRYNAME group)
#else
publicdef INT sdl_load (group)
ENTRYNAME group;
#endif
    {
    PROGNAME ("sdl_load");
    
    INT status = SUCCESS;
    PATHNAME groupPath;             /* filename of symbol group */
    PATHNAME destpath;              /* destination string for ho_add_path */
    CHAR aMsgStr[STMAX];
    
    sprintf (groupPath, "%s.SDL", group);   /* construct file name */
    
    /* lower case the name for operating system that care about that. */
    status = ts_sto_lower (groupPath);
    
    if (hoexists (groupPath))
        {
        sprintf (aMsgStr, "%s - Using local SDL file: %s", progname, groupPath);
        wimsg (aMsgStr);
        }
    else
        {
        ho_add_path ("ESI$SYMBOLS", groupPath, MAXPATHLENGTH, destpath);
        }
    if (hoexists (destpath))
        {
        SDL_START_GROUP (group);
        ci_load (destpath);
        SDL_END_GROUP;
        status = sdl_Status;
        }
    else
        {
        status = FAIL;
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_start_group();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_start_group ()
#else
static VOID sdl_start_group ()
#endif
    {
    PROGNAME ("sdl_start_group");
    
    SDL_START_GROUP (ps_pointer_pop ());
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_end_group();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_end_group ()
#else
static VOID sdl_end_group ()
#endif
    {
    PROGNAME ("sdl_end_group");
    
    SDL_END_GROUP;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_start_symbol();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_start_symbol ()
#else
static VOID sdl_start_symbol ()
#endif
    {
    PROGNAME ("sdl_start_symbol");
    
    SDL_START_SYMBOL (ps_long_pop ());
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_end_symbol();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_end_symbol ()
#else
static VOID sdl_end_symbol ()
#endif
    {
    PROGNAME ("sdl_end_symbol");
    
    SDL_END_SYMBOL;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_start_primitive();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_start_primitive ()
#else
static VOID sdl_start_primitive ()
#endif
    {
    PROGNAME ("sdl_start_primitive");
    
    SDL_START_PRIMITIVE (ps_long_pop ());
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_end_primitive();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_end_primitive ()
#else
static VOID sdl_end_primitive ()
#endif
    {
    PROGNAME ("sdl_end_primitive");
    
    SDL_END_PRIMITIVE;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_add_point();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_add_point ()
#else
static VOID sdl_add_point ()
#endif
    {
    PROGNAME ("sdl_add_point");
    
    DOUBLE x, y;
    
    y = ps_double_pop ();
    x = ps_double_pop ();
    SDL_ADD_POINT (x, y);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_line();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_line ()
#else
static VOID sdl_line ()
#endif
    {
    PROGNAME ("sdl_line");
    
    DOUBLE x1, y1, x2, y2;
    
    y2 = ps_double_pop ();
    x2 = ps_double_pop ();
    y1 = ps_double_pop ();
    x1 = ps_double_pop ();
    SDL_LINE (x1, y1, x2, y2);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_filled_circle();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_filled_circle ()
#else
static VOID sdl_filled_circle ()
#endif
    {
    PROGNAME ("sdl_filled_circle");
    
    DOUBLE x, y, r;
    
    r = ps_double_pop ();
    y = ps_double_pop ();
    x = ps_double_pop ();
    SDL_FILLED_CIRCLE (x, y, r);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_hollow_circle();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_hollow_circle ()
#else
static VOID sdl_hollow_circle ()
#endif
    {
    PROGNAME ("sdl_hollow_circle");
    
    DOUBLE x, y, r;
    
    r = ps_double_pop ();
    y = ps_double_pop ();
    x = ps_double_pop ();
    SDL_HOLLOW_CIRCLE (x, y, r);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_filled_rect();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_filled_rect ()
#else
static VOID sdl_filled_rect ()
#endif
    {
    PROGNAME ("sdl_filled_rect");
    
    DOUBLE x1, y1, x2, y2;
    
    y2 = ps_double_pop ();
    x2 = ps_double_pop ();
    y1 = ps_double_pop ();
    x1 = ps_double_pop ();
    SDL_FILLED_RECT (x1, y1, x2, y2);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_hollow_rect();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_hollow_rect ()
#else
static VOID sdl_hollow_rect ()
#endif
    {
    PROGNAME ("sdl_hollow_rect");
    
    DOUBLE x1, y1, x2, y2;
    
    y2 = ps_double_pop ();
    x2 = ps_double_pop ();
    y1 = ps_double_pop ();
    x1 = ps_double_pop ();
    SDL_HOLLOW_RECT (x1, y1, x2, y2);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_gather_points();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_gather_points ()
#else
static VOID sdl_gather_points ()
#endif
    {
    PROGNAME ("sdl_gather_points");
    
    SDL_GATHER_POINTS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_polyline();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_polyline ()
#else
static VOID sdl_polyline ()
#endif
    {
    PROGNAME ("sdl_polyline");
    
    SDL_POLYLINE;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_filled_area();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_filled_area ()
#else
static VOID sdl_filled_area ()
#endif
    {
    PROGNAME ("sdl_filled_area");
    
    SDL_FILLED_AREA;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID sdl_radial_segment();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <sdl_symdef.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID sdl_radial_segment ()
#else
static VOID sdl_radial_segment ()
#endif
    {
    PROGNAME ("sdl_radial_segment");
    
    DOUBLE x, y, r1, r2, d;
    
    d = ps_double_pop ();
    r2 = ps_double_pop ();
    r1 = ps_double_pop ();
    y = ps_double_pop ();
    x = ps_double_pop ();
    SDL_RADIAL_SEGMENT (x, y, r1, r2, d);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sdl_InitPolymarkerGroup();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    Function returns the value of sdl_Status.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sdl_InitPolymarkerGroup ()
#else
publicdef INT sdl_InitPolymarkerGroup ()
#endif
    {
    INT status = SUCCESS;
    
    SDL_START_GROUP (CS_POLYMARKER_GROUP);
    
    /* Point */
    SDL_START_SYMBOL (CS_POLYMARKER_POINT);
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (0.0, 0.0);       /* center to center */
    SDL_ADD_POINT (0.0, 0.0);
    SDL_END_PRIMITIVE;
    SDL_END_SYMBOL;
    
    /* Plus */
    SDL_START_SYMBOL (CS_POLYMARKER_PLUS);
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (-1.0, 0.0);      /* left to right */
    SDL_ADD_POINT (1.0, 0.0);
    SDL_END_PRIMITIVE;
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (0.0, -1.0);      /* bottom to top */
    SDL_ADD_POINT (0.0, 1.0);
    SDL_END_PRIMITIVE;
    SDL_END_SYMBOL;
    
    /* Asterix */
    SDL_START_SYMBOL (CS_POLYMARKER_ASTERIX);
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (-1.0, 0.0);      /* left to right */
    SDL_ADD_POINT (1.0, 0.0);
    SDL_END_PRIMITIVE;
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (0.7071, -0.7071);    /* right bottom to left top */
    SDL_ADD_POINT (-0.7071, 0.7071);
    SDL_END_PRIMITIVE;
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (0.0, 1.0);       /* top to bottom */
    SDL_ADD_POINT (0.0, -1.0);
    SDL_END_PRIMITIVE;
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (-0.7071, -0.7071);   /* left bottom to right top */
    SDL_ADD_POINT (0.7071, 0.7071);
    SDL_END_PRIMITIVE;
    SDL_END_SYMBOL;
    
    /* Circle */
    SDL_START_SYMBOL (CS_POLYMARKER_CIRCLE);
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (-1.0000, 0.0000);
    SDL_ADD_POINT (-0.7071, 0.7071);
    SDL_ADD_POINT (0.0000, 1.0000);
    SDL_ADD_POINT (0.7071, 0.7071);
    SDL_ADD_POINT (1.0000, 0.0000);
    SDL_ADD_POINT (0.7071, -0.7071);
    SDL_ADD_POINT (0.0000, -1.0000);
    SDL_ADD_POINT (-0.7071, -0.7071);
    SDL_ADD_POINT (-1.0000, 0.0000);
    SDL_END_PRIMITIVE;
    SDL_END_SYMBOL;
    
    /* X */
    SDL_START_SYMBOL (CS_POLYMARKER_X);
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (-0.7071, -0.7071);   /* left bottom to right top */
    SDL_ADD_POINT (0.7071, 0.7071);
    SDL_END_PRIMITIVE;
    SDL_START_PRIMITIVE (CS_POLYLINE);
    SDL_ADD_POINT (0.7071, -0.7071);    /* right bottom to left top */
    SDL_ADD_POINT (-0.7071, 0.7071);
    SDL_END_PRIMITIVE;
    SDL_END_SYMBOL;
    
    SDL_END_GROUP;
    status = sdl_Status;
    
    return(status);
    }
/* END:     */
