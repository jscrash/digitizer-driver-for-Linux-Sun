/* DEC/CMS REPLACEMENT HISTORY, Element LD_UK_LINES.C*/
/* *5    14-AUG-1990 13:34:24 VINCE "(SPR 5644) Header Standardization"*/
/* *4    11-AUG-1989 11:08:09 JULIAN "(SPR -1) redeclaration of clear_edittext"*/
/* *3     4-AUG-1989 11:07:21 ADEY "(SPR -1) CHANGE NAMES"*/
/* *2    25-JUL-1989 16:51:48 ADEY "(SPR -1) Improve User Interface & decode lat/long and survey/line for seismic"*/
/* *1    19-JUN-1989 13:03:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element LD_UK_LINES.C*/
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
#include "esi_am.h"
#include "esi_am_err.h"
#include "esi_ll.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_wi.h"
#include "uk_work.h"

#ifndef ESI_LD_MSG_H

#include "esi_ld_msg.h"

#endif

#ifndef ESI_LD_ERR_H

#include "esi_ld_err.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#include "ld_uk_main.rh"
#include "ld_uk_lines.rh"
#include "ld_uk_header.rh"
#include "ld_uk_fields.rh"
#include "ld_uk_lines_con.rh"
publicdef INT ukooa_lines_analyze_flat_file ();
publicdef INT ukooa_lines_reset_dialog ();

static INT ukooa_lines_strip_blanks ();
static INT ukooa_lines_set_selectors ();
static INT ukooa_lines_get_edittext ();
static INT ukooa_lines_insert_link ();
static INT ukooa_lines_delete_link ();
static INT ukooa_lines_replace_link ();
static INT printlist ();
static INT clear_edittext ();   /* clear edit text field */
static INT get_selection ();
static INT sort_list ();

publicdef INT confirm_srvr ();

static INT *select_array;       /* global variable */

/* File Description ---------------------------------------------------------
Overview:
    Ukooa lines server.
    START-HISTORY :
    12/12/86 KIMMAN   first time checkin
    12/12/86 GEORGE DOBEK/KIMMAN  Template constructed
    END-HISTORY 
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ukooa_lines_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);
    
    publicdef INT ukooa_lines_analyze_flat_file(UKOOA_WORKSPACE *pukooa);
    
    publicdef INT ukooa_lines_reset_dialog(UKOOA_WORKSPACE *pukooa);
    
    publicdef INT confirm_srvr(INT id, INT item, UKOOA_WORKSPACE *pukooa);

Private_Functions:
    static INT ukooa_lines_strip_blanks(CHAR *string);
    
    static INT ukooa_lines_set_selectors(UKOOA_WORKSPACE *pukooa);
    
    static INT get_selection(UKOOA_WORKSPACE *pukooa);
    
    static INT clear_edittext (INT id);
    
    static INT sort_list ();
    
    static INT ukooa_lines_get_edittext(UKOOA_WORKSPACE *pukooa, LINE_LINK **linelink);
    
    static INT ukooa_lines_replace_link(UKOOA_WORKSPACE *pukooa, LINE_LINK *linelink,
                                        INT pos);
    
    static INT ukooa_lines_insert_link(UKOOA_WORKSPACE *pukooa, LINE_LINK *linelink, 
                                        INT pos);
    
    static INT ukooa_lines_delete_link(UKOOA_WORKSPACE *pukooa, INT pos);
    
    static INT printlist(UKOOA_WORKSPACE *pukooa);
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Data lines analysis custom dialog server.

Prototype:
    publicdef INT ukooa_lines_server(INT id, INT item, UKOOA_WORKSPACE *pukooa);    

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id of the UKOOA header decoding custom dialog,
                        assigned by the am_define_resource service.
    (I) item        -(INT) Resource item to process.
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_lines_server (INT id, INT item, UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_lines_server (id, item, pukooa)
INT id;
INT item;
UKOOA_WORKSPACE *pukooa;
#endif
    {
    LINE_LINK dummylink, *linelink = &dummylink, *boguslink;
    CHAR message[STMAX], text[STMAX];
    INT i, count, status, cid;
    
    get_selection (pukooa);
    switch (item)
        {
    case UKOOA_LINES_All:
        count = llcount (&pukooa->line_ll);
        for (i = 0; i < count; i++)
            wi_set_default_sel (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox,
                                i + 1);
        break;
        
    case UKOOA_LINES_Radio_Honor:
        pukooa->lines.Replacement = FALSE;
        break;
        
    case UKOOA_LINES_Radio_Replace:
        pukooa->lines.Replacement = TRUE;
        break;
        
    case UKOOA_LINES_Btn_Add_Before:
        if (llcount (&pukooa->line_ll) <= 0)
            {
            ukooa_lines_get_edittext (pukooa, &linelink);
            llappnd (&pukooa->line_ll, &linelink);
            ukooa_lines_set_selectors (pukooa);
            }
        else
            {
            wi_set_default_sel (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox,
                                pukooa->lines.Line_Selected);
            ukooa_lines_get_edittext (pukooa, &linelink);
            llgetn (&pukooa->line_ll, &boguslink, pukooa->lines.Line_Selected);
            llinsb (&pukooa->line_ll, &linelink);
            ukooa_lines_insert_link (pukooa, linelink,
                                     pukooa->lines.Line_Selected - 1);
            }
        break;
        
    case UKOOA_LINES_Btn_Add_After:
        wi_set_default_sel (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox,
                            pukooa->lines.Line_Selected);
        
        if (llcount (&pukooa->line_ll) <= 0)
            {
            ukooa_lines_get_edittext (pukooa, &linelink);
            llappnd (&pukooa->line_ll, &linelink);
            ukooa_lines_set_selectors (pukooa);
            }
        else
            {
            ukooa_lines_get_edittext (pukooa, &linelink);
#ifdef DEBUG
            printlist (pukooa);
            sprintf (message, "selected: [%d]", pukooa->lines.Line_Selected);
            wimsg (message);
#endif
            llgetn (&pukooa->line_ll, &boguslink, pukooa->lines.Line_Selected);
            llinsa (&pukooa->line_ll, &linelink);
            ukooa_lines_insert_link (pukooa, linelink, pukooa->lines.Line_Selected);
            }
        break;
        
    case UKOOA_LINES_Btn_Replace:
        wi_set_default_sel (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox,
                            pukooa->lines.Line_Selected);
        
        if (llcount (&pukooa->line_ll) <= 0)
            {
            ukooa_lines_get_edittext (pukooa, &linelink);
            llappnd (&pukooa->line_ll, &linelink);
            ukooa_lines_set_selectors (pukooa);
            }
        else
            {
            ukooa_lines_get_edittext (pukooa, &linelink);
#ifdef DEBUG
            printlist (pukooa);
            sprintf (message, "selected: [%d]", pukooa->lines.Line_Selected);
            wimsg (message);
#endif
            llgetn (&pukooa->line_ll, &boguslink, pukooa->lines.Line_Selected);
            llreplc (&pukooa->line_ll, &linelink);
            ukooa_lines_replace_link (pukooa, linelink, pukooa->lines.Line_Selected);
            }
        break;
        
    case UKOOA_LINES_Btn_Delete:
#ifdef DEBUG
        printlist (pukooa);
#endif
        if (llcount (&pukooa->line_ll) <= 0)
            break;
        else
            {
            status = am_define_resource (AM_DIALOG, "LD_UK_LINES_CON", CONFIRM_CDLG,
                                         confirm_srvr, &cid, pukooa);
            if (status == SUCCESS)
                {
                count = llcount (&pukooa->line_ll);
                for (i = 0; select_array[i]; i++)
                    ;
                sprintf (text, mg_message (LD_UK_LINES_DELETE), i, count);
                status = wi_set_dialog_text (cid, CDLG_200_MESSAGE, text);
                status = wi_open_dialog (cid);
                }
            }
        break;
        
    case UKOOA_LINES_Btn_Cancel:
    case UKOOA_LINES_Btn_Done:
        wiclcd (pukooa->lines.dialog_id);
        wiopcd (pukooa->main_dialog_id);
        break;
        
    default:
        break;
        }
    clear_edittext (id);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Raw UKOOA flat-file Anaysis. Information within the flat-file is
    acquired and stored for use during processing. Information includes:

        1) Byte offset within flat-file to start of a new seimic line,
        2) Line Identification,
        3) First shotpoint of seismic line,
        4) Last shotpoint of seismic line,
        5) Number of shotpoints to skip (initially set to 0).

Prototype:
    publicdef INT ukooa_lines_analyze_flat_file(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_lines_analyze_flat_file (UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_lines_analyze_flat_file (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT i, status, nassigned;
    CHAR LINE[STMAX], PreviousLINE[STMAX];
    CHAR SP[STMAX], FirstSP[STMAX], LastSP[STMAX], Skip[STMAX];
    CHAR Buffer[STMAX], message[STMAX];
    INT LINEwidth, SPwidth;
    LINE_LINK dummylink, *linelink = &dummylink;
    INT len, blen;
    CHAR *Bufferptr1, *Bufferptr2;
    
    LINEwidth = pukooa->fields.Columns[0][1] - pukooa->fields.Columns[0][0] + 1;
    SPwidth = pukooa->fields.Columns[1][1] - pukooa->fields.Columns[1][0] + 1;
    
    if (LINEwidth <= 0 || SPwidth <= 0)
        {
        sprintf (message, mg_message (LD_UK_ALERT), pukooa->fields.Columns[0][0],
                 pukooa->fields.Columns[0][1], pukooa->fields.Columns[1][0],
                 pukooa->fields.Columns[1][1]);
        
        am_message (AM_ALERT, message);
        return(FAIL);
        }
    /* Open file */
    pukooa->lines.fp = ho_open (pukooa->lines.file_name, "r", NULL);
    if (NOT pukooa->lines.fp)
        {
        sprintf (message, mg_message (LD_CANT_OPEN_FILE), pukooa->lines.file_name);
        am_message (AM_ALERT, message);
        return(FAIL);
        }
    /* Skip header records */
    for (i = 1; i <= pukooa->header.NumHeaderRec; i++)
        {
        if (fgets (Buffer, STMAX, pukooa->lines.fp) == (CHAR *)NULL)
            {
            sprintf (message, mg_message (LD_END_OF_FILE),
                     pukooa->header.NumHeaderRec);
            am_message (AM_ALERT, message);
            fclose (pukooa->lines.fp);
            return(FAIL);
            }
        }
    strcpy (Buffer, "");
    strcpy (LINE, "");
    strcpy (PreviousLINE, "");
    strcpy (SP, "");
    strcpy (FirstSP, "");
    strcpy (LastSP, "");
    strcpy (Skip, "0");
    
    llfree (&pukooa->line_ll);
    /*     reinitialise the link list    */
    
    llinit (&pukooa->line_ll, 0, sizeof(LINE_LINK *), 0);
    
    while (feof (pukooa->lines.fp) == 0)
        {
        fgets (Buffer, STMAX, pukooa->lines.fp);
        
        blen = strlen (Buffer) - 1;
        if (Buffer[blen] == '\n')
            Buffer[blen] = '\0';
        
        if (blen >= pukooa->fields.Columns[0][0])
            {
            Bufferptr1 = Buffer + pukooa->fields.Columns[0][0] - 1;
            
            if ((len = strlen (Bufferptr1)) <= LINEwidth)
                {
                strncpy (LINE, Bufferptr1, len);
                LINE[len] = '\0';
                }
            else
                {
                strncpy (LINE, Bufferptr1, LINEwidth);
                LINE[LINEwidth] = '\0';
                }
            if (strlen (Buffer) >= pukooa->fields.Columns[1][0])
                {
                Bufferptr2 = Buffer + pukooa->fields.Columns[1][0] - 1;
                
                if ((len = strlen (Bufferptr2)) <= SPwidth)
                    {
                    strncpy (SP, Bufferptr2, len);
                    SP[len] = '\0';
                    }
                else
                    {
                    strncpy (SP, Bufferptr2, SPwidth);
                    SP[SPwidth] = '\0';
                    }
                }
            else
                strcpy (SP, "");
            }
        else
            {
            strcpy (LINE, "");
            strcpy (SP, "");
            }
        if (strlen (LINE) > 0)
            {
            if (strcmp (LINE, PreviousLINE) != 0)
                {
                if (strlen (PreviousLINE) > 0)
                    {
                    linelink = ALLOCATE (LINE_LINK);
                    linelink->line = STRING_ALLOCATE (PreviousLINE);
                    strcpy (linelink->line, PreviousLINE);
                    ukooa_lines_strip_blanks (linelink->line);
                    linelink->firstsp = STRING_ALLOCATE (FirstSP);
                    strcpy (linelink->firstsp, FirstSP);
                    ukooa_lines_strip_blanks (linelink->firstsp);
                    linelink->lastsp = STRING_ALLOCATE (LastSP);
                    strcpy (linelink->lastsp, LastSP);
                    ukooa_lines_strip_blanks (linelink->lastsp);
                    linelink->skip = STRING_ALLOCATE (Skip);
                    strcpy (linelink->skip, Skip);
                    llappnd (&pukooa->line_ll, &linelink);
                    }
                strcpy (PreviousLINE, LINE);
                strcpy (FirstSP, SP);
                }
            strcpy (LastSP, SP);
            }
        }
    if (strlen (PreviousLINE) > 0)
        {
        linelink = ALLOCATE (LINE_LINK);
        linelink->line = STRING_ALLOCATE (PreviousLINE);
        strcpy (linelink->line, PreviousLINE);
        ukooa_lines_strip_blanks (linelink->line);
        linelink->firstsp = STRING_ALLOCATE (FirstSP);
        strcpy (linelink->firstsp, FirstSP);
        ukooa_lines_strip_blanks (linelink->firstsp);
        linelink->lastsp = STRING_ALLOCATE (LastSP);
        strcpy (linelink->lastsp, LastSP);
        ukooa_lines_strip_blanks (linelink->lastsp);
        linelink->skip = STRING_ALLOCATE (Skip);
        strcpy (linelink->skip, Skip);
        llappnd (&pukooa->line_ll, &linelink);
        }
#ifdef DEBUG
    printlist (pukooa);
#endif
    status = fclose (pukooa->lines.fp);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT ukooa_lines_strip_blanks(CHAR *string);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *)
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_lines_strip_blanks (CHAR *string) 
#else
static INT ukooa_lines_strip_blanks (string)
CHAR *string;
#endif
    {
    CHAR *cptr = string;
    CHAR *eptr;
    INT len, i;
    
    if (strlen (string) == 0)
        return SUCCESS;
    
    /* strip leading blanks */
    while (*cptr != '\0' && *cptr == ' ')
        cptr++;
    
    if ((len = strlen (cptr)) == 0)
        {
        strcpy (string, "");
        return SUCCESS;
        }
    for (i = len, eptr = cptr + len - 1; i > 0; eptr--, i--)
        {
        if (*eptr == ' ')
            *eptr = '\0';
        else
            break;
        }
    strcpy (string, cptr);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT ukooa_lines_reset_dialog(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ukooa_lines_reset_dialog (UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT ukooa_lines_reset_dialog (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT count;
    
    count = llcount (&pukooa->line_ll);
    wiscdv (pukooa->lines.dialog_id, UKOOA_LINES_Radio_Honor, ON);
    wiscdv (pukooa->lines.dialog_id, UKOOA_LINES_Radio_Replace, OFF);
    pukooa->lines.Replacement = FALSE;
    tc_free (select_array);
    select_array = (INT *)tc_zalloc ((count + 1) * sizeof(INT));
    ukooa_lines_set_selectors (pukooa);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT ukooa_lines_set_selectors(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_lines_set_selectors (UKOOA_WORKSPACE *pukooa) 
#else
static INT ukooa_lines_set_selectors (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT count;
    INT i;
    LINE_LINK dummylink, *linelink = &dummylink;
    
    count = llcount (&pukooa->line_ll);
    if (count <= 0)
        return FAIL;
    
    pukooa->lines.line_tcp = TCP_ALLOCATE (count);
    pukooa->lines.firstsp_tcp = TCP_ALLOCATE (count);
    pukooa->lines.lastsp_tcp = TCP_ALLOCATE (count);
    pukooa->lines.skip_tcp = TCP_ALLOCATE (count);
    
    for (i = 0; i < count; i++)
        {
        if (i == 0)
            llfirst (&pukooa->line_ll, &linelink);
        else
            llnext (&pukooa->line_ll, &linelink);
        
        if (strlen (linelink->line) == 0)
            {
            pukooa->lines.line_tcp[i] = STRING_ALLOCATE ("?");
            strcpy (pukooa->lines.line_tcp[i], "?");
            }
        else
            {
            pukooa->lines.line_tcp[i] = STRING_ALLOCATE (linelink->line);
            strcpy (pukooa->lines.line_tcp[i], linelink->line);
            }
        if (strlen (linelink->firstsp) == 0)
            {
            pukooa->lines.firstsp_tcp[i] = STRING_ALLOCATE ("?");
            strcpy (pukooa->lines.firstsp_tcp[i], "?");
            }
        else
            {
            pukooa->lines.firstsp_tcp[i] = STRING_ALLOCATE (linelink->firstsp);
            strcpy (pukooa->lines.firstsp_tcp[i], linelink->firstsp);
            }
        if (strlen (linelink->lastsp) == 0)
            {
            pukooa->lines.lastsp_tcp[i] = STRING_ALLOCATE ("?");
            strcpy (pukooa->lines.lastsp_tcp[i], "?");
            }
        else
            {
            pukooa->lines.lastsp_tcp[i] = STRING_ALLOCATE (linelink->lastsp);
            strcpy (pukooa->lines.lastsp_tcp[i], linelink->lastsp);
            }
        if (strlen (linelink->skip) == 0)
            {
            pukooa->lines.skip_tcp[i] = STRING_ALLOCATE ("?");
            strcpy (pukooa->lines.skip_tcp[i], "?");
            }
        else
            {
            pukooa->lines.skip_tcp[i] = STRING_ALLOCATE (linelink->skip);
            strcpy (pukooa->lines.skip_tcp[i], linelink->skip);
            }
        }
    wisselt (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox,
             UKOOA_LINES_ScrollBar, count, count, pukooa->lines.line_tcp);
    wisselt (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox,
             UKOOA_LINES_ScrollBar, count, 0, pukooa->lines.firstsp_tcp);
    wisselt (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, UKOOA_LINES_ScrollBar,
             count, 0, pukooa->lines.lastsp_tcp);
    wisselt (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, UKOOA_LINES_ScrollBar,
             count, 0, pukooa->lines.skip_tcp);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT get_selection(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT get_selection (UKOOA_WORKSPACE *pukooa) 
#else
static INT get_selection (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT NumItems, count, i;
    CHAR message[STMAX];
    
    count = llcount (&pukooa->line_ll);
    for (i = 0; i < count; i++)
        select_array[i] = 0;
    wiqsel (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, &NumItems,
            select_array);
    if (NumItems == 0)
        {
        pukooa->lines.Line_Selected = 1;
        strcpy (message, "Please select a LINE name");
        wiscdt (pukooa->lines.dialog_id, UKOOA_LINES_Status_Stat, message);
        }
    else
        {
        for (i = 0; select_array[i]; i++)
            ;
        pukooa->lines.Line_Selected = select_array[i - 1];
        }
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT clear_edittext (INT id);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id of the UKOOA header decoding custom dialog,
                        assigned by the am_define_resource service.

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT clear_edittext (INT id) 
#else
static INT clear_edittext (id)
INT id;
#endif
    {
    wiscdt (id, UKOOA_LINES_LineName_Edit, " ");
    wiscdt (id, UKOOA_LINES_FirstSP_Edit, " ");
    wiscdt (id, UKOOA_LINES_LastSP_Edit, " ");
    wiscdt (id, UKOOA_LINES_Skip_Edit, " ");
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT sort_list ();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT sort_list () 
#else
static INT sort_list ()
#endif
    {
    INT i, j, temp;
    
    for (i = 0; select_array[i]; i++)
        for (j = i + 1; select_array[j]; j++)
            if (select_array[i] < select_array[j])
                {
                temp = select_array[i];
                select_array[i] = select_array[j];
                select_array[j] = temp;
                }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT confirm_srvr(INT id, INT item, UKOOA_WORKSPACE *pukooa);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id of the UKOOA header decoding custom dialog,
                        assigned by the am_define_resource service.
    (I) item        -(INT) Resource item to process.
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT confirm_srvr (INT id, INT item, UKOOA_WORKSPACE *pukooa) 
#else
publicdef INT confirm_srvr (id, item, pukooa)
INT id;
INT item;
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT i;
    LINE_LINK *boguslink;
    switch (item)
        {
    case CDLG_200_DELETE:
        sort_list ();
        for (i = 0; select_array[i]; i++)
            {
            llgetn (&pukooa->line_ll, &boguslink, select_array[i]);
            lldela (&pukooa->line_ll);
            ukooa_lines_delete_link (pukooa, select_array[i]);
            }
    case CDLG_200_NOT_DELETE:
    default:
        break;
        }
    am_release_resource (id);
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT ukooa_lines_get_edittext(UKOOA_WORKSPACE *pukooa, LINE_LINK **linelink);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.
    (M) linelink    -(LINE_LINK **)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_lines_get_edittext (UKOOA_WORKSPACE *pukooa,
                                         LINE_LINK **linelink) 
#else
static INT ukooa_lines_get_edittext (pukooa, linelink)
UKOOA_WORKSPACE *pukooa;
LINE_LINK **linelink;
#endif
    {
    LINE_LINK dummylink, *link = &dummylink;
    CHAR text[STMAX];
    INT Value;
    
    link = *linelink = ALLOCATE (LINE_LINK);
    
    wiqccdi (pukooa->lines.dialog_id, UKOOA_LINES_LineName_Edit, &Value, text);
    ukooa_lines_strip_blanks (text);
    link->line = STRING_ALLOCATE (text);
    strcpy (link->line, text);
    
    wiqccdi (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_Edit, &Value, text);
    ukooa_lines_strip_blanks (text);
    link->firstsp = STRING_ALLOCATE (text);
    strcpy (link->firstsp, text);
    
    wiqccdi (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_Edit, &Value, text);
    ukooa_lines_strip_blanks (text);
    link->lastsp = STRING_ALLOCATE (text);
    strcpy (link->lastsp, text);
    
    wiqccdi (pukooa->lines.dialog_id, UKOOA_LINES_Skip_Edit, &Value, text);
    ukooa_lines_strip_blanks (text);
    link->skip = STRING_ALLOCATE (text);
    strcpy (link->skip, text);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT ukooa_lines_replace_link(UKOOA_WORKSPACE *pukooa, LINE_LINK *linelink,
                                        INT pos);
                                        
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.
    (M) linelink    -(LINE_LINK **)
    (I) pos         -(INT)
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_lines_replace_link (UKOOA_WORKSPACE *pukooa, LINE_LINK *linelink,
                                         INT pos) 
#else
static INT ukooa_lines_replace_link (pukooa, linelink, pos)
UKOOA_WORKSPACE *pukooa;
LINE_LINK *linelink;
INT pos;
#endif
    {
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, pos);
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox, pos);
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, pos);
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, pos);
    
    if (strlen (linelink->line) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, pos,
                           "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, pos,
                           linelink->line);
    
    if (strlen (linelink->firstsp) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox, pos,
                           "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox, pos,
                           linelink->firstsp);
    
    if (strlen (linelink->lastsp) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, pos,
                           "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, pos,
                           linelink->lastsp);
    
    if (strlen (linelink->skip) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, pos, "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, pos,
                           linelink->skip);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT ukooa_lines_insert_link(UKOOA_WORKSPACE *pukooa, LINE_LINK *linelink, 
                                        INT pos);
                                        
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.
    (M) linelink    -(LINE_LINK **)
    (I) pos         -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_lines_insert_link (UKOOA_WORKSPACE *pukooa, LINE_LINK *linelink,
                                        INT pos) 
#else
static INT ukooa_lines_insert_link (pukooa, linelink, pos)
UKOOA_WORKSPACE *pukooa;
LINE_LINK *linelink;
INT pos;
#endif
    {
    pos++;
    
    if (strlen (linelink->line) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, pos,
                           "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, pos,
                           linelink->line);
    
    if (strlen (linelink->firstsp) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox, pos,
                           "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox, pos,
                           linelink->firstsp);
    
    if (strlen (linelink->lastsp) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, pos,
                           "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, pos,
                           linelink->lastsp);
    
    if (strlen (linelink->skip) == 0)
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, pos, "?");
    else
        wi_insert_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, pos,
                           linelink->skip);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT ukooa_lines_delete_link(UKOOA_WORKSPACE *pukooa, INT pos);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.
    (I) pos         -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT ukooa_lines_delete_link (UKOOA_WORKSPACE *pukooa, INT pos) 
#else
static INT ukooa_lines_delete_link (pukooa, pos)
UKOOA_WORKSPACE *pukooa;
INT pos;
#endif
    {
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LineName_SelBox, pos);
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_FirstSP_SelBox, pos);
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_LastSP_SelBox, pos);
    wi_delete_cd_line (pukooa->lines.dialog_id, UKOOA_LINES_Skip_SelBox, pos);
    
    return(SUCCESS);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT printlist(UKOOA_WORKSPACE *pukooa);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pukooa      -(UKOOA_WORKSPACE *) Pointer to the application workspace,
                        which was defined in the calling routine.

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <ld_uk_lines.c>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT printlist (UKOOA_WORKSPACE *pukooa) 
#else
static INT printlist (pukooa)
UKOOA_WORKSPACE *pukooa;
#endif
    {
    INT i;
    INT count;
    LINE_LINK *linelink;
    CHAR message[STMAX];
    
    count = llcount (&pukooa->line_ll);
    if (count > 0)
        {
        for (i = 1; i <= count; i++)
            {
            llgetn (&pukooa->line_ll, &linelink, i);
            sprintf (message, "[%s][%s][%s][%s]", linelink->line, linelink->firstsp,
                     linelink->lastsp, linelink->skip);
            wimsg (message);
            }
        }
    }
/* END:     */
