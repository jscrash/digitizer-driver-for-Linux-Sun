/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DLG.C*/
/* *5    13-JUN-1991 14:48:05 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method"*/
/* *4    23-OCT-1990 13:56:30 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *3     2-MAR-1990 11:24:26 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:42:45 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DLG.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DLG.C                                 */
/* *3     2-MAR-1990 11:24:26 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:42:45 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:38:32 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DLG.C                                 */

/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_READ_DIALOG                                                           */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Read a custom dialog resource into the custom dialog structure.               */
/*                                                                                  */
/*    Input Parameters;                                                             */
/*                                                                                  */
/*         cdrsid = (INT) custom dialog resource ID.                                */
/*         thecdlg = (CUSTOM_DIALOG *) custom dialog structure pointer.             */
/*                                                                                  */
/*************************************************************************          */

#include "esi_c_lib.h"

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_RS_H
#include "esi_rs.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_CTYPE_H
#include "esi_ctype.h"
#endif

#define READ_RSRC { if(rsrcptr != NULL) \
                    { tempptr = rsgets(rsrcptr, linein); \
                      rsrcptr = tempptr; \
                    } else { \
                      linein[0] = NUL; }}
        

/************************************************************************************/
publicdef INT wi_read_dialog (cdrsid, thecdlg)  /*************************************************************************/

INT cdrsid;
CUSTOM_DIALOG *thecdlg;
    {
    PROGNAME ("wi_read_dialog");
#define IDENT progname
    INT id;
    INT i;
    INT j;
    INT numitms;
    FILE *fpr;
    INT ifoundit;
    INT ls;
    INT thetype;
    INT current_cd_group;
    INT current_cd_font;
    INT current_cd_textsize;
    BOOL current_cd_hscroll;
    INT verify_type;
    INT verify_length;
    INT verlen;
    CHAR linein[STMAX];
    CHAR *flag;
    CHAR cper;
    CHAR item_string[STMAX];        /*%Q too short                                  */
    CHAR *rsrcptr;
    CHAR *tempptr;
    CHAR *wasptr;
    CHAR *lenptr;
    
    /* These items are defined by the Macintosh Resource Protocol.                  */
#define NUM_MAC_TYPES 17
    static  struct cd_types
        {
        CHAR type_text[16];
        INT type_id;
        } cd_mac_types[NUM_MAC_TYPES] = 
        {
        "btnitem", 0, "chkitem", 1, "radioitem", 2, "stattext", 3, "rectitem", 4,
        "edittext", 5, "selector", 6, "vscroll", 16, "valuator", 17, "locktext", 18,
        "optionmenu", 19, "setgroup", -1,   /* special commands                     */
        "setfont", -2, "settextsize", -3, "helpkey", -4, "setmodal", -5, 
	 "sethscroll",  -6
        };

#define NUM_VER_TYPES 4
    static  struct ver_types
        {
        CHAR ver_text[20];
        INT ver_id;
        } edit_ver_types[NUM_VER_TYPES] = 
        {
        "verifynone", 0, "verifyalphanumeric", 1, "verifyfloat", 2, "verifyinteger", 3
        };
    
    /********************************************************************************/
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    /* Load Custom Dialog Resource into memory.                                     */
    ifoundit = rsfetch (cdrsid, "CDLG", &rsrcptr);
    if (ifoundit != TRUE)
        {
        RETURN (WI_RESOURCE_NOT_FOUND);
        }
    /* Read into the custom dialog data structure.                                  */
    READ_RSRC;                      /* Read rectangle.                              */
    if (tempptr EQUALS NULL)
        {
        RETURN (WI_EOF_ON_RESOURCE);
        }
    sscanf (linein, "%d %d %d %d", &thecdlg->cd_rect.iymin, &thecdlg->cd_rect.ixmin,
            &thecdlg->cd_rect.iymax, &thecdlg->cd_rect.ixmax);
    
    READ_RSRC;                      /* Read Title.                                  */
    wiz_remove_cr (linein);
    if (tempptr EQUALS NULL)
        {
        RETURN (WI_EOF_ON_RESOURCE);
        }
    strcpy (thecdlg->cd_title, linein);
    
    /* Read count of items from resource.                                           */
    READ_RSRC;
    if (tempptr EQUALS NULL)
        {
        RETURN (WI_EOF_ON_RESOURCE);
        }
    sscanf (linein, "%d", &thecdlg->num_cd_items);
    numitms = thecdlg->num_cd_items;
    if (numitms > MAX_CD_ITEMS)
        {
        RETURN (WI_TOO_MANY_CDI);
        }
    /* Set default group ID for radio buttons.                                      */
    current_cd_group = 0;
    
    /* Set default font for text.                                                   */
    current_cd_font = 0;
    
    /* Set default size for text.                                                   */
    current_cd_textsize = 12;
    
    /* default = no hscroll                                                         */
    current_cd_hscroll = FALSE;

    /* default = no helpkey defined from resource...                                */
    wiv.current.rsrc_helpkey[0] = NUL;
    
    /* default = non-modal                                                          */
    wiv.current.modal_state = 0;
    
    /* Read Items from Resource file. ============================                  */
    for (i = 0; i < thecdlg->num_cd_items; )
        {
        
        /* Clear the verification fields...                                         */
        thecdlg->cd_item[i].cd_item_verify = 0;
        thecdlg->cd_item[i].cd_item_verify_len = 0;
        
        /* Check for termination in Text Reading.                                   */
        if (tempptr EQUALS NULL)
            {
            RETURN (WI_EOF_ON_RESOURCE);
            }
        /* Read until a nonblank line.                                              */
        do
            {
            READ_RSRC;
            if (tempptr EQUALS NULL)
                {
                RETURN (WI_EOF_ON_RESOURCE);
                }
            } while (isspace (linein[0]));
        
        /* Read item type.                                                          */
        sscanf (linein, "%s", item_string);
        ts_sto_lower (item_string); /* Convert to lower case.                       */
        
        /* Scan item types for a match.                                             */
        thecdlg->cd_item[i].cd_item_type = 0;   /* default to button                */
        
        ifoundit = FALSE;           /* Not found flag.                              */
        for (j = 0; j < NUM_MAC_TYPES; j++)
            {
            if (strcmp (cd_mac_types[j].type_text, item_string) == 0)
                {
                thecdlg->cd_item[i].cd_item_type = cd_mac_types[j].type_id;
                thetype = cd_mac_types[j].type_id;
                ifoundit = TRUE;
                break;
                }
            }
        /* Check for valid type.                                                    */
        if (ifoundit == FALSE)
            {
            RETURN (WI_BAD_CD_TYPE);
            }
        /* Process command or item.                                                 */
        switch (thetype)
            {
        case -1:                    /* Set GROUP Command. ----------------------    */
	                            /* default = no hscroll for a new group         */
	    current_cd_hscroll = FALSE;
            sscanf (linein, "%s %d", item_string, &current_cd_group);
            break;
            
        case -2:                    /* Set FONT Command. ----------------------     */
            sscanf (linein, "%s %d", item_string, &current_cd_font);
            break;
            
        case -3:                    /* SetTEXTSIZE Command. ----------------------  */
            sscanf (linein, "%s %d", item_string, &current_cd_textsize);
            break;
            
        case -4:                    /* HELPKEY command                              */
            sscanf (linein, "%s %s", item_string, wiv.current.rsrc_helpkey);
            break;
            
        case -5:                    /* SETMODAL command                             */
            wiv.current.modal_state = 1;
            break;
            
        case -6:                   /* SET HSCROLL command                          */
	      current_cd_hscroll = TRUE;
              break;

        default:                    /* Define item.                                 */
            
            /* This part of the code checks if the control is an editext field...   */
            /* *                                                                    */
            /* * If so, it checks if the next line matches any of the Verify keywords and   */
            /* * if so, it then checks for a value following it...                  */
            /* *                                                                    */
            /*                                                                      */
            
            if (thecdlg->cd_item[i].cd_item_type == 5)
                {
                wasptr = rsrcptr;   /* we may have to restore this...               */
                ifoundit = FALSE;   /* Not found flag.                              */
                
                READ_RSRC;          /* get a line of data                           */
                sscanf (linein, "%s", item_string);
                ts_sto_lower (item_string); /* Convert to lower case.               */
                
                for (j = 0; j < NUM_VER_TYPES; j++) /* check eack keyword           */
                    {
                    if (strcmp (edit_ver_types[j].ver_text, item_string) == 0)
                        {
                        
                        /* found a keyword, load its value into the dialog structure*/
                        thecdlg->cd_item[i].cd_item_verify = edit_ver_types[j].ver_id;
                        
                        /* register that we found it                                */
                        ifoundit = TRUE;
                        
                        /* Is there a length specified?                             */
                        /* calculate the pointer just pass the name                 */
                        lenptr = linein + strlen (edit_ver_types[j].ver_text);
                        
                        /* calc any value...                                        */
                        verlen = 0;
                        sscanf (lenptr, "%d", &verlen);
                        if (verlen != 0 && verlen != EOF)
                            {
                            thecdlg->cd_item[i].cd_item_verify_len = verlen;
                            }
                        }
                    }
                /* Restore the rsrcptr, if there was no edittext verification.      */
                if (ifoundit == FALSE)
                    {
                    rsrcptr = wasptr;
                    }
                }
            /* This part of the code checks if the control is a BUTTON...           */
            /* *                                                                    */
            /* * If so, it checks if the next line matches 'defaultbutton'; if so it sets   */
            /* * the 'cd_item_verify' field to -1...                                */
            /* *                                                                    */
            /*                                                                      */
            
            if (thecdlg->cd_item[i].cd_item_type == 0)
                {
                wasptr = rsrcptr;   /* we may have to restore this...               */
                ifoundit = FALSE;   /* Not found flag.                              */
                
                READ_RSRC;          /* get a line of data                           */
                sscanf (linein, "%s", item_string);
                ts_sto_lower (item_string); /* Convert to lower case.               */
                
                if (strcmp (item_string, "defaultbutton") == 0)
                    {
                    
                    /* found a keyword, load its value into the dialog structure    */
                    thecdlg->cd_item[i].cd_item_verify = -1;
                    
                    /* register that we found it                                    */
                    ifoundit = TRUE;
                    
                    }
                /* Restore the rsrcptr, if there was no default specification.      */
                if (ifoundit == FALSE)
                    {
                    rsrcptr = wasptr;
                    }
                }
            /* Read Rectangle for item.                                             */
            READ_RSRC;
            if (tempptr EQUALS NULL)
                {
                RETURN (WI_EOF_ON_RESOURCE);
                }
            sscanf (linein, "%d %d %d %d", &thecdlg->cd_item[i].cd_item_rect.iymin,
                    &thecdlg->cd_item[i].cd_item_rect.ixmin,
                    &thecdlg->cd_item[i].cd_item_rect.iymax,
                    &thecdlg->cd_item[i].cd_item_rect.ixmax);
            
            /* Read the text for the control...                                     */
            
            thecdlg->cd_item[i].cd_item_text[0] = NUL;
            do
                {
                READ_RSRC;
                if (isgraph (linein[0]))
                    {
                    strcat (thecdlg->cd_item[i].cd_item_text, linein);
                    }
                } while (isgraph (linein[0]));
            wiz_remove_cr (thecdlg->cd_item[i].cd_item_text);
            
            /* Set Item GROUP.                                                      */
            thecdlg->cd_item[i].cd_item_group = current_cd_group;
            /* Set Item FONT.                                                       */
            thecdlg->cd_item[i].cd_item_font = current_cd_font;
            /* Set Item TEXTSIZE.                                                   */
            thecdlg->cd_item[i].cd_item_textsize = current_cd_textsize;
             /* Set Item HSCROLL flag                                               */
             thecdlg->cd_item[i].cd_hscroll = current_cd_hscroll;
            
            /* Increment item counter.                                              */
            i++;
            }
        }
    /* End Item Loop =====================================                          */
    
    /* Scan for extraneous material.                                                */
    while (rsrcptr != NULL)
        {
        tempptr = rsgets (rsrcptr, linein);
        rsrcptr = tempptr;
        }
    RETURN (SUCCESS);
    }
