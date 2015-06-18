/* DEC/CMS REPLACEMENT HISTORY, Element PF_OVERLAY.C */
/* *2     6-JUN-1991 15:44:21 MING "(SPR 0) new checkin" */
/* *1     6-JUN-1991 15:43:02 MING "plot file overlay" */
/* DEC/CMS REPLACEMENT HISTORY, Element PF_OVERLAY.C */
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
#include "esi_md_defs.h"
#include "esi_ov.h"                    /* overlay stuff */
#include "esi_ov_defs.h"               /* for overlay information */
#include "esi_pf.h"                    /* plotfile overlay defs */
#include "esi_mp.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Create and post plotfile..

Prototype:
    publicdef INT pf_overlay(INT display_line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT pf_overlay (INT display_line)
#else
publicdef INT pf_overlay (display_line)
INT display_line;
#endif
    {
    INT status = SUCCESS;
    PF_INFO pf;       /* structure of pf info */
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_PARM_STRUCT *parmstruc;
    INT row;
    INT i;
    
    /* * INITIALIZE PLOTFILE  STRUCTURE * */
    /* ---------------------------------- */
    
    tc_zeroes (&pf, sizeof(PF_INFO));
    
    /* * FILL  PLOTFILE STRUCTURE * */
    /* ---------------------------- */
    
    mp_get_current_map (&mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[row];

    /* ------------------------ */
    /* fill up the pf structure */
    /* ------------------------ */
    strcpy (pf.app,"MAPPING");
    
    for (i = 0; i < verbstruc->nrows; i++)
        {
        if (verbstruc->key_used[i] != 0)
            {
            parmstruc = verbstruc->parm_ptr[i];
            switch (verbstruc->key_code[i])
                {
            case 1:                 /* FILENAME */
                fm_parse_long_name (parmstruc->parmval.cval,pf.analyst,pf.filename);
                break;
                }
            }
        }
    /* now that the structure has been filled, post plotfile overlay */
    
    ov_open_overlay (display_line,(CHAR *) 0);
    /* ------- */
    /* post it */
    /* ------- */
    status = pf_post_overlay (&pf);
    ov_close_overlay (display_line);
    
    return(status);
    }
/* END:     */
