/* DEC/CMS REPLACEMENT HISTORY, Element FIH_INIT_MNU.C */
/* *10    5-AUG-1991 17:01:04 JTM "(SPR 0) Added do_beta_menus() to remove beta menu items that do no have the right logical */
/*defined." */
/* *9    12-JUL-1991 15:43:57 JTM "(SPR 0) Added tests to dynamic memory allocation to avoid possible future errors." */
/* *8    14-AUG-1990 22:22:27 GILLESPIE "(SPR 2001) Changes for new include files" */
/* *7    14-AUG-1990 13:12:07 VINCE "(SPR 5644) Header Standardization" */
/* *6    13-DEC-1989 11:37:52 GORDON "(SPR 0) One more time..." */
/* *5    13-DEC-1989 07:42:49 GORDON "(SPR 0) Fix the null-termination of dynamic_menus array." */
/* *4    12-DEC-1989 11:18:00 GORDON "(SPR 0) Replace 1 more 0 with NULL_MENU_ID" */
/* *3    27-NOV-1989 13:07:44 GORDON "(SPR 0) Use NULL_MENU_ID, not 0 for terminating menu element" */
/* *2    20-SEP-1989 15:30:48 GORDON "(SPR 200) Renamed version" */
/* *1    20-SEP-1989 15:30:01 GORDON "renamed version" */
/* DEC/CMS REPLACEMENT HISTORY, Element FIH_INIT_MNU.C */
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

#ifndef ESI_FI_INT_H
#include "esi_fi_int.h"
#endif

#include "esi_c_lib.h"

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#include "esi_am.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_mg.h"
#include "esi_er.h"
#include "esi_fi_err.h"
#include "esi_ho.h"
#include "esi_ho_files.h"
#include "esi_fi_msg.h"

/* delimiter character for MENU macros which define the menu items */

#define DELIMITER ','
#define BEGIN_SIZE 18
#define MENU_SIZE   4
#define END_SIZE   16
extern FINDER_MENU_STRUCT finder_menus[];   /* the linked in array */

/* space for run-time menus */
static FINDER_MENU_STRUCT *dynamic_menus = (FINDER_MENU_STRUCT *)
0;
/* where the final array will be */
static FINDER_MENU_STRUCT *combined_menus = (FINDER_MENU_STRUCT *)
0;

static TCP curFile;

/* strings used to locate Software_Bus macro references (by name) */

static CHAR begin_macro[BEGIN_SIZE + 1] = "BEGIN_FINDER_MENUS";
static CHAR menu_macro[MENU_SIZE + 1] = "MENU";
static CHAR end_macro[END_SIZE + 1] = "END_FINDER_MENUS";

static CHAR er_string[128];     /* message buffer */

/* Tables of strings; wherein the "menu text", "application names",    */
/* and "function names" are kept.  For a given element in FINDER_MENUS */
/* structure array, the above listed items are at the same index in    */
/* the following arrays.                                               */

static INT highest_menu = 0;    /* just what it says */

typedef struct 
    {
    INT old_num;
    INT new_num;
    } MENU_MAP;

static MENU_MAP **menu_maps = (MENU_MAP **)
0;

#if USE_PROTOTYPES
    static INT process_menu_macro(FINDER_MENU_STRUCT *mPtr, CHAR *line, 
                                    MENU_MAP *mMapPtr );
    static VOID shuffle_menu( FINDER_MENU_STRUCT *mPtr );
    static INT menu_map_add( MENU_MAP *mMapPtr, FINDER_MENU_STRUCT *mPtr );
    static INT menu_map_lookup( MENU_MAP *mMapPtr, INT *idPtr );
    static INT menu_name_lookup( FINDER_MENU_STRUCT *mPtr, CHAR *name,
                                    INT *idPtr );
    static INT read_line(FILE *fptr, CHAR *sptr);
    static VOID remove_angle_brackets(CHAR *str);
    static INT skip_chars(CHAR *str, INT ndx);
    static INT skip_space(CHAR *str, INT ndx);
    static VOID free_all(void);
    static INT  copy_token (CHAR *dst, CHAR *src, INT  ndx);
    static INT  do_beta_menus(FINDER_MENU_STRUCT *mptr);

#else

    static INT process_menu_macro();
    static VOID shuffle_menu();
    static INT menu_map_add();
    static INT menu_map_lookup();
    static INT menu_name_lookup();
    static INT read_line();    
    static VOID remove_angle_brackets();    
    static INT skip_chars();    
    static INT skip_space();    
    static VOID free_all();
    static INT  copy_token ();
    static INT  do_beta_menus();

#endif

/* File Description ---------------------------------------------------------
Overview:
    This function is the support routine for the implementation of the   
    Software (BOZO) Bus.  At this point, the FINDER_MENU_STRUCT array
    finder_menus has been statically linked in by a combination of
    MACRO and LINK.  In order to allow the user to link at run-time to
    his/her own programs or command procedures, we will now parse another
    menu definition file, of the same format as FINDER_MENUS.MAR, and
    combine the 2 arrays into one.  Note that the user cannot put in 
    function entry points at this point because we are beyond the linking stage.
                                    
    Usage: ret = FIH_INIT_MENUS ();             
                                    
    Output: Return Value - 0=success, FAIL=fatal error
                                    
    Notes:  The definition of the MENU macros (found in MENU_NAME file) 
            is contained in a VAX assembly language file called     
            FINDER_MENUS.MAR, but this macro can be summarized as   
            a loader for an array of structures described below:        
                                
            Entry   Name        Length  Type    Description     
            1       MenuID      4       INT     ID of menu      
            2       MenuText    32      CHAR    Menu text       
            3       Type        4       INT     Selection type      
            4       AppEntry    4       PTR     Function entry point    
            5       AppText     255     CHAR    Name of Program     
                                
            FIH_INIT_MENUS will load this structure array at run-time.  
                                
            FIH_INIT_MENUS will obtain the menu definitions from a file 
            which the user names on the command line using the -menu option.

    Modification History:                       
    10/13/88 - PRIME implementation of the Software Bus     
    10/19/88 - set TYPE field to ZERO on last item (not just ID)    
    10/20/88 - absorb header file (FIH_INIT_MENUS.H) into here  
    07/25/89 - Redesign for implementation of combining static & dynamic menus -GS
    08/03/89 - Handle multiple menu files -GS

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT fih_init_menus ();  

    publicdef FINDER_MENU_STRUCT *fiz_finder_menus();
    
Private_Functions:
    static INT process_menu_macro(FINDER_MENU_STRUCT *mPtr, CHAR *line, 
                                    MENU_MAP *mMapPtr );
                                    
    static VOID shuffle_menu( FINDER_MENU_STRUCT *mPtr );
    
    static INT menu_map_add( MENU_MAP *mMapPtr, FINDER_MENU_STRUCT *mPtr );
    
    static INT menu_map_lookup( MENU_MAP *mMapPtr, INT *idPtr );
    
    static INT menu_name_lookup( FINDER_MENU_STRUCT *mPtr, CHAR *name,
                                    INT *idPtr );
    static INT read_line(FILE *fptr, CHAR *sptr);
    
    static VOID remove_angle_brackets(CHAR *str);
    
    static INT skip_chars(CHAR *str, INT ndx);
    
    static INT skip_space(CHAR *str, INT ndx);
    
    static VOID free_all(void);

    static INT  copy_token (CHAR *dst, CHAR *src, INT  ndx);

    static INT  do_beta_menus(FINDER_MENU_STRUCT *mptr);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Fill-up the FINDER "menu structure".  Read the VAX "macros" which
    define the menus from a file, whose name was on the command line.

Prototype:
    publicdef INT fih_init_menus ();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fih_init_menus () 
#else
publicdef INT fih_init_menus ()
#endif
    {
    INT ret;                        /* return # from various functions  */
    INT index;                      /* index into line while processing */
    INT status, ii;
    INT current;                    /* current item in structure array  */
    INT curMenu;
    INT num_static;                 /* number of entries in static array */
    INT num_dyn;                    /* number entries read in at run-time */
    CHAR line[128];                 /* buffer for line from MENU file   */
    CHAR dummy[128];                /* a sub-string for line pieces     */
    FILE *menuFilePtr;              /* pointer to MENU file's "element" */
    FINDER_MENU_STRUCT tempMenu;    /* a temporary menu item holding area */
    FINDER_MENU_STRUCT *local_ptr;  /* used to init array       */
    CONFIGURATION_WORKSPACE *c;     /* to get menu file name */
    
    status = do_beta_menus(finder_menus);   /* stomp all the beta menu items */

    combined_menus = finder_menus;  /* assume no user menus */
    
    /* get the menu file name */
    status = am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *) & c);
    if (status != SUCCESS)
        return status;
    
    if (c->user_menu_tcp == (CHAR **)0) /* no user file was on command line */
        return SUCCESS;
    
    /* initialize local variables */
    line[0] = 0;
    ret = index = current = 0;
    
    /* get the size of the static array */
    for (local_ptr = finder_menus, num_static = 0; local_ptr->id != NULL_MENU_ID;
         ++local_ptr, ++num_static)
        {
        if (local_ptr->id > highest_menu)   /* remember the highest */
            highest_menu = local_ptr->id;   /* menu number for later */
        }
    dynamic_menus = (FINDER_MENU_STRUCT *)tc_zalloc (MAX_MENU *
                                                     sizeof(FINDER_MENU_STRUCT));

    /* (jtm) moved test in front of assignment to dm[0] */
    if (dynamic_menus == (FINDER_MENU_STRUCT *)0) 
       {
        return FAIL;
       }

    dynamic_menus[0].id = NULL_MENU_ID; /* initialize the list */
    
    /* get space for menu maps - 1 for each menu */
    menu_maps = (MENU_MAP **)tc_zalloc ((ts_tcp_len (c->user_menu_tcp) + 1) *
                                        sizeof(MENU_MAP **));
    
    if (menu_maps == (MENU_MAP **)0)
        {
        free_all ();
        return FAIL;
        }
    /* open the user's file */
    for (ii = 0, curFile = c->user_menu_tcp; *curFile != NULL; ++curFile, ++ii)
        {
        menuFilePtr = ho_open (*curFile, "r", NULL);
        
        if (menuFilePtr == (FILE *)0)
            {
            free_all ();
            sprintf (er_string, mg_message (FI_MENUS_NOT_FOUND), *curFile);
            am_message (AM_STATUS, er_string);
            return FAIL;
            }
        menu_maps[ii] = (MENU_MAP *)tc_zalloc (MAX_MENU * sizeof(MENU_MAP));

        /*(jtm) check memory allocation */ 
        if ( (MENU_MAP *)0 == menu_maps[ii] ) 
           {
            free_all ();
            return FAIL;
           }

        sprintf (er_string, mg_message (FI_USING_MENU_FILE), *curFile);
        am_message (AM_STATUS, er_string);
        
        /* MENU file is open.  Read through it, and locate the  
           BEGIN_FINDER_MENUS macro invocation.  That marks the 
           start of the MENU macros we are looking for.   */
        
        do
            {
            /* get a line to examine */
            ret = read_line (menuFilePtr, line);    /* get next line */
            
            /* skip any leading white space */
            index = skip_space (line, 0);   /* work from start of line */
            
            /* look for a "begin menus" macro in the current line */
            index = copy_token (dummy, line, index);
            ts_sto_upper (dummy);
            if (strncmp (dummy, begin_macro, BEGIN_SIZE) == 0)
                break;              /* exit this loop, if BEGIN_MACRO found */
            } while (ret != EOF);   /* go to EOF looking for "begin" macro */
        
        if (ret == EOF)             /* was a "begin menus" found ??? */
            {                       /* nope, that's a fatal error !! */
            fclose (menuFilePtr);   /* to be safe */
            sprintf (er_string, mg_message (FI_NO_BEGIN_MENUS), *curFile);
            am_message (AM_STATUS, er_string);
            free_all ();
            return FAIL;            /* fatal error */
            }
        /* Read lines, processing the MENU macros contained therein; */
        /* continue until an invocation of END_FINDER_MENUS is seen. */
        do
            {
            /* get a line to examine */
            ret = read_line (menuFilePtr, line);    /* get next line */
            
            /* skip any leading white space */
            index = skip_space (line, 0);   /* work from start of line */
            
            /* look for a "menu" or an "end" macro in the current line */
            index = copy_token (dummy, line, index);
            ts_sto_upper (dummy);
            if (strncmp (dummy, menu_macro, MENU_SIZE) == 0)
                {
                /* "menu" macro found, process it */
                tc_zeroes ((VOIDPTR) & tempMenu, sizeof(tempMenu));
                status = process_menu_macro (&tempMenu, line, menu_maps[ii]);
                if (status == FAIL)
                    {
                    free_all ();
                    return status;  /* failure */
                    }
                if (status == 1)    /* menu defined statically, so "eat it" */
                    continue;       /* i.e., current isn't incremented      */
                
                /* It's a good item, so deserves to be added to our array */
                
                hoblockmove ((VOIDPTR) & tempMenu, (VOIDPTR) & dynamic_menus[current],
                             sizeof(FINDER_MENU_STRUCT));
                
                /* done with this line, so bump current structure item */
                if (++current >= MAX_MENU)  /* array full? */
                    {
                    fclose (menuFilePtr);   /* to be safe */
                    sprintf (er_string, mg_message (FI_TOO_MANY_MENUS), *curFile);
                    am_message (AM_STATUS, er_string);
                    free_all ();
                    return FAIL;    /* fatal error */
                    }
                dynamic_menus[current].id = NULL_MENU_ID;   /* Mark next one as end */
                
                }
            /* end of "menu" macro processing */
            else
                {
                if (strncmp (dummy, end_macro, END_SIZE) == 0)
                    {
                    dynamic_menus[current].id = NULL_MENU_ID;
                    break;
                    }
                }
            } while (ret != EOF);
        
        fclose (menuFilePtr);       /* finished reading, so close the MENU file */
        
        /* if no "end" macro was found, complain, then exit with error */
        if (ret == EOF)
            {
            sprintf (er_string, mg_message (FI_NO_END_MENUS), *curFile);
            am_message (AM_STATUS, er_string);
            free_all ();
            return FAIL;
            }
        }
    /* end main file-processing loop */
    
    
    /* count the number of dynamically read menu items */
    
    for (local_ptr = dynamic_menus, num_dyn = 0; local_ptr->id != NULL_MENU_ID;
         ++local_ptr, ++num_dyn)
        ;
    
    /* We've made it this far, so at long last, we can assemble the menus 
    into one giant new array */
    
    combined_menus = (FINDER_MENU_STRUCT *)tc_zalloc ((num_static + num_dyn + 1) *
                                                      sizeof(FINDER_MENU_STRUCT));
    if (combined_menus == (FINDER_MENU_STRUCT *)0)
        {
        free_all ();
        combined_menus = finder_menus;  /* point it back to static array */
        return FAIL;
        }
    hoblockmove ((VOIDPTR)finder_menus, (VOIDPTR)combined_menus,
                 num_static * sizeof(FINDER_MENU_STRUCT));
    
    hoblockmove ((VOIDPTR)dynamic_menus, (VOIDPTR)(combined_menus + num_static),
                 (num_dyn + 1) * sizeof(FINDER_MENU_STRUCT));
    
    tc_free (dynamic_menus);        /* Release all that memory, we're done with it */
    tc_free (menu_maps);
    
    /* walk thru the dynamic part of the final array, moving
       any "orphan" items back to the menu where they belong and
       resolving any "NextMenu" MenuType's Activates.menu number */
    for (local_ptr = combined_menus + num_static; local_ptr->id != NULL_MENU_ID;
         ++local_ptr)
        {
        if (local_ptr->Type == NextMenu)    /* resolve go-to menu number */
            /* use AppName string to find  real menu to branch to */
            menu_name_lookup (combined_menus, local_ptr->AppName,
                              &local_ptr->Activates.menu);
        
        if (local_ptr->Type == MenuName)
            {
            curMenu = local_ptr->id;    /* menu name & all its children */
            continue;               /* are OK (remember the id) */
            }
        if (local_ptr->id != curMenu)   /* must be an orphan item, so */
            shuffle_menu (local_ptr);   /* move it to its parent's home */
        }
    return SUCCESS;
    }                               /* end of FIH_INIT_MENUS */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Enter a MENU macro's contents into the MENU data structure. 
    Returns SUCCESS, FAIL or 1 if successful, but it's a menu type 
    MenuName that's already defined statically.
    
Prototype:
    static INT process_menu_macro (FINDER_MENU_STRUCT *mPtr, CHAR *line, 
                                    MENU_MAP *mMapPtr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) mPtr        -(FINDER_MENU_STRUCT *) Pointer to structure to fill.
    (I) line        -(CHAR *) Line containing the VAX "menu" macro.
    (I) mMapPtr     -(MENU_MAP *) This menu file's menu map.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PRIVATE to <fih_init_menus>.

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT process_menu_macro (FINDER_MENU_STRUCT *mPtr, CHAR *line,
                                   MENU_MAP *mMapPtr) 
#else
static INT process_menu_macro (mPtr, line, mMapPtr)
FINDER_MENU_STRUCT *mPtr;
CHAR *line;
MENU_MAP *mMapPtr;
#endif
    {
    static INT current_id;      /* last menu ID seen */
    INT status;
    INT i, index;                   /* indices into STR  */
    CHAR dummy[128];                /* temporary string  */
    
    /* skip MENU macro name and its trailing white-space */
    index = skip_space (line, 0);
    index = skip_chars (line, index);
    index = skip_space (line, index);
    
    /* 1st argument to MENU macro is the MENU ID */
    index = copy_token (dummy, line, index);    /* get MENU ID */
    sscanf (dummy, "%d", &i);       /* convert the number */
    mPtr->id = i;                   /* store value in ID field */
    
    /* 2nd argument to MENU macro is the MENU item TEXT */
    index = copy_token (dummy, line, index);    /* get MENU TEXT */
    remove_angle_brackets (dummy);  /* kill VAX-specific brackets */
    mPtr->Name = tc_alloc (strlen (dummy) + 1);
    strcpy (mPtr->Name, dummy);
    
    /* 3rd argument to MENU macro is the MENU item TYPE */
    index = copy_token (dummy, line, index);    /* get MENU TYPE */
    i = (INT)Undefined;             /* initialize receiving integer */
    sscanf (dummy, "%d", &i);       /* convert the number */
    mPtr->Type = (MenuType)i;       /* put it in TYPE field */
    
    /* 4th argument to MENU macro depends on the TYPE. */
    if (mPtr->Type == MenuName)     /* Ids of all menu names are mapped */
        {
        status = menu_map_add (mMapPtr, mPtr);
        if (status == 1)
            {
            current_id = mPtr->id;
            return status;          /* menu is defined in static menus, don't go on */
            }
        if (status != SUCCESS)      /* bad - menu already defined in dynamic menus */
            {
            sprintf (er_string, mg_message (FI_BAD_MENU_ID), *curFile);
            am_message (AM_STATUS, er_string);
            return FAIL;
            }
        }
    /* it wasn't an old MenuName, so map the id to the final menu id */
    else if (menu_map_lookup (mMapPtr, &mPtr->id) != SUCCESS)
        {
        sprintf (er_string, mg_message (FI_ORPHAN_MENU_ITEM), *curFile);
        am_message (AM_STATUS, er_string);
        return FAIL;
        }
    switch (mPtr->Type)
        {
    case Undefined:                 /* TYPE = 0 */
        break;                      /* item is probably a "dimmed" menu pick. */
    case MenuName:                  /* TYPE = 1 */
        /* save ID for consistency checking, then skip this argument. */
        current_id = mPtr->id;      /* remember menu's ID */
        index = copy_token (dummy, line, index);    /* skip unused token */
        break;
    case NextMenu:                  /* TYPE = 2 */
        /* isolate the next menu's ID, and put it in structure */
        index = copy_token (dummy, line, index);    /* get NEXT MENU */
        mPtr->Activates.menu = 0;   /* Not allowed to specify menu
                                    number; must use AppName field */
        break;
    case EventsFunction:            /* TYPE = 3 */
    case NoEventsFunction:          /* TYPE = 4 */
        /* Not allowed -- do nothing (will be dimmed later) */
        break;
    case ProgramName:               /* TYPE = 5 */
    case CPName:                    /* TYPE = 6 */
    case CommandName:               /* TYPE = 7 */
    case ProgramNowait:             /* TYPE = 8 */
    case CPNowait:                  /* TYPE = 9 */
        /* ignore this argument */
        index = copy_token (dummy, line, index);    /* skip unused token */
        break;
    default:
        sprintf (er_string, mg_message (FI_UNKNOWN_MENU), *curFile);
        am_message (AM_STATUS, er_string);
        return FAIL;                /* failure */
        }
    /* end of switch */
    
    /* 5th argument to MENU macro also depends on the TYPE.  It is a 
       text string with various uses */
    
    switch (mPtr->Type)
        {
    case Undefined:                 /* TYPE = 0 */
        break;
    case MenuName:                  /* TYPE = 1 */
    case NextMenu:                  /* TYPE = 2 */
    case EventsFunction:            /* TYPE = 3 */
    case NoEventsFunction:          /* TYPE = 4 */
    case ProgramName:               /* TYPE = 5 */
    case CPName:                    /* TYPE = 6 */
    case CommandName:               /* TYPE = 7 */
    case ProgramNowait:             /* TYPE = 8 */
    case CPNowait:                  /* TYPE = 9 */
        /* isolate this argument string, and put it in the structure. */
        index = copy_token (dummy, line, index);    /* APPL name, etc.   */
        remove_angle_brackets (dummy);  /* kill VAX-specific braces */
        mPtr->AppName = tc_alloc (strlen (dummy) + 1);
        strcpy (mPtr->AppName, dummy);
        if (mPtr->Type == NextMenu && strlen (dummy) == 0)
            {
            sprintf (er_string, mg_message (FI_GOTO_MENU_NAME), *curFile);
            am_message (AM_STATUS, er_string);
            }
        break;
    default:
        sprintf (er_string, mg_message (FI_UNKNOWN_MENU), *curFile);
        am_message (AM_STATUS, er_string);
        return FAIL;                /* failure */
        }
    /* end of switch */
    
    /* Automatically "dim-out" any menu item whose type is Undefined. */
    /* Items in this category include ones without action codes, and  */
    /* ones whose function name can NOT be linked to dynamically.     */
    if (mPtr->Type == Undefined)
        {
        strcpy (&dummy[1], mPtr->Name);
        dummy[0] = '(';             /* this'll cause "dimming" on MAC */
        strcpy (mPtr->Name, dummy);
        }
    /* Perform a "structure check"; that is, make sure that this MENU */
    /* macro's ID field matches the ID field of the last TYPE #1 MENU */
    /* macro that was encountered.                                    */
    if (mPtr->id != current_id)
        {
        sprintf (er_string, mg_message (FI_BAD_MENU_ID), *curFile);
        am_message (AM_STATUS, er_string);
        return FAIL;                /* failure */
        }
    return(SUCCESS);                /* success */
    }                               /* end of PROCESS_MENU_MACRO */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Read in a line of text from the input file, and store it (without 
    the end-of-line marker) into the caller's string buffer.          
    
Prototype:
    static INT read_line (FILE *fptr, CHAR *sptr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) fptr        -(FILE *) File to read from.
    (M) sptr        -(CHAR *) Buffer for line.
    
Return Value/Status:
    Return the status code from the FSCANF operation                  

Scope:
    PRIVATE to <fih_init_menus>.

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT read_line (FILE *fptr, CHAR *sptr) 
#else
static INT read_line (fptr, sptr)
FILE *fptr;
CHAR *sptr;
#endif
    {                               /* start of READ_LINE */
    INT ret;
    CHAR dummy[16];
    
    ret = fscanf (fptr, "%[^\n]", sptr);    /* get next line */
    ret = fscanf (fptr, "%[\n]", dummy);    /* consume EOL   */
    
    return(ret);
    }                               /* end of READ_LINE */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Remove the "angle brackets" (<>) that surround the incoming string.
    These are required by VAX MACRO syntactics, but cause trouble here.
    This function also removes all LEADING & TRAILING spaces from its argument.

Prototype:
    static VOID remove_angle_brackets (CHAR *str);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) str         -(CHAR *) A string to process.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID remove_angle_brackets (CHAR *str) 
#else
static VOID remove_angle_brackets (str)
CHAR *str;
#endif
    {
    INT i;
    
    ts_trim (str);
    
    i = strlen (str) - 1;           /* get its new length */
    if (*(str + i) == '>')          /* a closing brace?   */
        *(str + i) = '\0';          /* yep - so kill it!! */
    
    if (*(str) == '<')              /* opening one too ?? */
        strcpy (str, str + 1);      /* yep !! */
    
    return;
    }                               /* end of REMOVE_ANGLE_BRACKETS  */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Skip over spaces; that is, return the index of the next 
    non-space character past a certain index (NDX) in STR.  

Prototype:
    static INT  skip_space (CHAR *str, INT  ndx);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) str         -(CHAR *) A string to process.
    (I) ndx         -(INT) Index to start from.

Return Value/Status:
    Function returns the spaces's index.
    
Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT skip_space (CHAR *str, INT ndx) 
#else
static INT skip_space (str, ndx)
CHAR *str;
INT ndx;
#endif
    {                               /* start of SKIP_SPACE */
    INT i;                          /* array index */
    
    for (i = ndx; (*(str + i) == ' ') && (*(str + i) != 0); i++)    /* skip spaces */
            ;
    
    return(i);                      /* give caller the spaces's index */
    }                               /* end of SKIP_SPACE */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Skip over characters; that is, return the index of the next
    space character past a certain index (NDX) in the string (STR).

Prototype:
    static INT  skip_chars (CHAR *str, INT  ndx);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) str         -(CHAR *) A string to process.
    (I) ndx         -(INT) Index to start from.
    
Return Value/Status:
    Function returns the character's index.
    
Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT skip_chars (CHAR *str, INT ndx) 
#else
static INT skip_chars (str, ndx)
CHAR *str;
INT ndx;
#endif
    {                               /* start of SKIP_CHARS */
    INT i;                          /* array index */
    
    for (i = ndx; (*(str + i) != ' ') && (*(str + i) != 0); i++)    /* skip chars */
            ;
    
    return(i);                      /* give caller the character's index */
    }                               /* end of SKIP_CHARS */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Copy a token, starting from a given index (NDX), from one string
    to another; that is, copy all characters from NDX to the next
    DELIMITER.  Don't include the comma, and return the index of the 
    next character past it (or the end of the string).  

Prototype:
    static INT  copy_token (CHAR *dst, CHAR *src, INT  ndx);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) dst         -(CHAR *) Destination string.
    (I) src         -(CHAR *) Source string.
    (I) ndx         -(INT) Index to start from.
    
Return Value/Status:
    Function returns the index of the token.
    
Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT copy_token (CHAR *dst, CHAR *src, INT ndx) 
#else
static INT copy_token (dst, src, ndx)
CHAR *dst;
CHAR *src;
INT ndx;
#endif
    {                               /* start of COPY_TOKEN */
    INT i, j;                       /* array indices */
    
    /* skip over white space */
    i = skip_space (src, ndx);
    
    /* copy characters up to next comma or end-of-string */
    for (j = 0; (*(src + i) != DELIMITER) && (*(src + i) != 0); i++, j++)
        *(dst + j) = *(src + i);
    
    /* terminate the output string */
    *(dst + j) = 0;
    
    /* return a new index to the caller */
    if (*(src + i) != 0)            /* end-of-string? or, was comma seen. */
        i++;                        /* not end-of-string, so advance index to next token */
    
    return(i);                      /* give caller the index */
    }                               /* end of COPY_TOKEN */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Move the given menu item back in the array to the spot at the end of
    its owner's menu space.  

Prototype:
    static VOID shuffle_menu(FINDER_MENU_STRUCT *mPtr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mPtr        -(FINDER_MENU_STRUCT *) Menu data structure.
    
Return Value/Status:
    None.
    
Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID shuffle_menu (FINDER_MENU_STRUCT *mPtr) 
#else
static VOID shuffle_menu (mPtr)
FINDER_MENU_STRUCT *mPtr;
#endif
    {
    FINDER_MENU_STRUCT temp_menu;
    FINDER_MENU_STRUCT *temp_ptr;
    
    /* find the place where menu item belongs */
    for (temp_ptr = combined_menus; temp_ptr->id <= mPtr->id; ++temp_ptr)
        {                           /* Make sure that "Quit Finder" remains
                                       the last item in any menu */
        if (mPtr->id == temp_ptr->id && !strcmp (temp_ptr->Name, "Quit Finder"))
            break;
        }
    if (temp_ptr >= mPtr)
        return;                     /* should never happen, but in case */
    
    /* copy the menu-to-be-moved into a holding area */
    hoblockmove ((VOIDPTR)mPtr, (VOIDPTR) & temp_menu, sizeof(temp_menu));
    
    /* do the shift */
    hoblockmove ((VOIDPTR)temp_ptr, (VOIDPTR)(temp_ptr + 1),
                 (size_t)((mPtr - temp_ptr) * sizeof(FINDER_MENU_STRUCT)));
    
    /* now slide the menu-to-be-moved into its new home */
    hoblockmove ((VOIDPTR) & temp_menu, (VOIDPTR)temp_ptr, sizeof(temp_menu));
    
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Add an entry into the table which will map a logical menu id to an 
    absolute menu id.
    
Prototype:
    static INT menu_map_add(MENU_MAP *mMapPtr, FINDER_MENU_STRUCT *mPtr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mMapPtr     -(MENU_MAP *) The menu map to use.
    (M) mPtr        -(FINDER_MENU_STRUCT *) Menu data structure.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT menu_map_add (MENU_MAP *mMapPtr, FINDER_MENU_STRUCT *mPtr) 
#else
static INT menu_map_add (mMapPtr, mPtr)
MENU_MAP *mMapPtr;
FINDER_MENU_STRUCT *mPtr;
#endif
    {
    FINDER_MENU_STRUCT *local_menu_ptr;
    
    /* first, find next empty slot in menu number map */
    for (; mMapPtr->new_num != 0; ++mMapPtr)
        {
        if (mMapPtr->old_num == mPtr->id)   /* make sure the menu isn't */
            return FAIL;            /* already in the list */
        }
    mMapPtr->old_num = mPtr->id;    /* set 1st struct member */
    /* now look for a matching menu name & use it if found */
    if (menu_name_lookup (finder_menus, mPtr->Name, &mPtr->id) == SUCCESS ||
        menu_name_lookup (dynamic_menus, mPtr->Name, &mPtr->id) == SUCCESS)
        {
        mMapPtr->new_num = mPtr->id;
        return 1;
        }
    /* didn't find it in static menus, so make it the next one in the grand list */
    mMapPtr->new_num = mPtr->id = ++highest_menu;   /* also bump the menu counter */
    return SUCCESS;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Translate the logical menu id into the absolute menu id.

Prototype:
    static INT menu_map_lookup(MENU_MAP *mMapPtr, INT *idPtr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) mMapPtr     -(MENU_MAP *) The menu map to use.
    (M) idPtr       -(INT *) Menu id pointer.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT menu_map_lookup (MENU_MAP *mMapPtr, INT *idPtr) 
#else
static INT menu_map_lookup (mMapPtr, idPtr)
MENU_MAP *mMapPtr;
INT *idPtr;
#endif
    {
    for (; mMapPtr->new_num != 0; ++mMapPtr)
        {
        if (mMapPtr->old_num == *idPtr) /* found it, so set to new */
            {
            *idPtr = mMapPtr->new_num;
            return SUCCESS;
            }
        }
    return FAIL;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Get the menu id associated with the given menu name in the given menu
    list (null-terminated).

Prototype:
    static INT menu_name_lookup(FINDER_MENU_STRUCT *mPtr, CHAR *name, INT *idPtr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) mPtr        -(FINDER_MENU_STRUCT *) Menu data structure.
    (I) name        -(CHAR *) Menu name.
    (O) idPtr       -(INT *) Menu id associated with name.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT menu_name_lookup (FINDER_MENU_STRUCT *mPtr, CHAR *name, INT *idPtr) 
#else
static INT menu_name_lookup (mPtr, name, idPtr)
FINDER_MENU_STRUCT *mPtr;
CHAR *name;
INT *idPtr;
#endif
    {
    /* walk thru the menus, looking for a matching menu name */
    for (; mPtr->id != NULL_MENU_ID; ++mPtr)
        {
        if (mPtr->Type != MenuName) /* ignore all menu items */
            continue;
        if (strcmp (name, mPtr->Name) == 0)
            {
            *idPtr = mPtr->id;      /* found it! */
            return SUCCESS;
            }
        }
    return FAIL;                    /* didn't find it */
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Free the menu maps.
    
Prototype:
    static VOID free_all();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PRIVATE to <fih_init_menus>.
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID free_all () 
#else
static VOID free_all ()
#endif
    {
    INT ii;
    
    if (dynamic_menus != (FINDER_MENU_STRUCT *)0)
        tc_free (dynamic_menus);
    if (combined_menus != (FINDER_MENU_STRUCT *)0)
        tc_free (combined_menus);
    if (menu_maps != (MENU_MAP **)0)
        {
        for (ii = 0; menu_maps[ii] != (MENU_MAP *)0; ++ii)
            tc_free (menu_maps[ii]);
        tc_free (menu_maps);
        }
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This is where all of finder gets its menu pointer.

Prototype:
    publicdef FINDER_MENU_STRUCT *fiz_finder_menus();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns combined_menus.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef FINDER_MENU_STRUCT *fiz_finder_menus () 
#else
publicdef FINDER_MENU_STRUCT *fiz_finder_menus ()
#endif
    {
    return combined_menus;
    }
/* END:     */


/* Function Description -----------------------------------------------------
Description:
     Go through the static menu structure and remove any beta test items that
     should not be displayed.

Prototype:
     static INT do_beta_menus(FINDER_MENU_STRUCT *mptr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
     (M)   FINDER_MENU_STRUCT *mptr;    Menu structure to read and possible modify.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT do_beta_menus (FINDER_MENU_STRUCT *mptr)
#else
static INT do_beta_menus (mptr)
FINDER_MENU_STRUCT *mptr;
#endif
{
 INT total;        /* total number of items in menu */
 INT sofar;        /* number of items done so far */
 INT status = SUCCESS;  /* assume successful */ 
 FINDER_MENU_STRUCT *local_ptr;  /* used to move through array        */
 INT  i;             /* loop counter */

 /* 
   count up the total number of items in the menu 
 */
 for ( local_ptr = mptr, total = 1; local_ptr->id != NULL_MENU_ID; local_ptr++ ) { total++; }

 /*
   next loop through, looking for beta items that shouldn't be displayed, and stomp them.
 */
 for (local_ptr = mptr, sofar = 1; local_ptr->id != NULL_MENU_ID; local_ptr++, sofar++)
 {
  if ( local_ptr->BetaLogical != (CHAR *)0 )
  {
   TCP tcp = (TCP)0;
   /*
     is there a logical name for this item?
   */

   if( ( ho_translate_symbol(local_ptr->BetaLogical, &tcp) == SUCCESS ) &&
       ( strcmp(tcp[0], "ON") == 0 ) )
   {
    continue;  /* go ahead and show it */
   }

   if( tcp != (TCP)0 ) ts_tcp_free(tcp);

   /*
     don't show this beta item.
     now move everything up one, stomping the unwanted menu item.
     don't use hoblockmove, because the addresses are screwy.
   */

   for (i = 0; i < total - sofar; i++)
   {
    *(local_ptr + i) = *(local_ptr + i + 1);
   }
   /*
     back the local_ptr up so we will check the one just moved 
   */
   local_ptr--;
  }
  else{} /* it is not a BETA item. go ahead and show it */

 }


 return( status );

}
/* END: do_beta_menus() */
