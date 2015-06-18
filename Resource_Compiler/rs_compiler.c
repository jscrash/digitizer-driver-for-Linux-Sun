/* DEC/CMS REPLACEMENT HISTORY, Element RS_COMPILER.C */
/* *1    13-NOV-1990 11:26:14 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element RS_COMPILER.C */
/* DEC/CMS REPLACEMENT HISTORY, Element RS_COMPILER.C */
/* *6     9-NOV-1990 11:39:27 MING "header files change" */
/* *5     9-NOV-1990 10:43:37 MING "change ho_find_files to ho_find_pathnames" */
/* *4    20-JUN-1990 12:25:48 MING "" */
/* *3    19-JUN-1990 16:51:55 MING "change ho_find_files2 to ho_find_files" */
/* *2    14-MAR-1990 13:17:26 KEE "New RS compiler for both X and Mac version" */
/* *1    14-MAR-1990 13:13:33 KEE "New RS compiler for both X and Mac version" */
/* DEC/CMS REPLACEMENT HISTORY, Element RS_COMPILER.C */
/******************************************************************************

                Copyright Exploration Systems, Inc.1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


/* DEC/CMS REPLACEMENT HISTORY, Element RS_COMPILER.C*/
/* *7     6-APR-1989 15:23:05 JULIAN "(SPR 4981) Added INT casting to the output defines"*/
/* *6    23-FEB-1989 13:35:42 GORDON "(SPR 4981) handle multiple files correctly - reset file_len"*/
/* *5    17-JAN-1989 12:48:33 JHUANG "(SPR 4981) merge PRIME fix"*/
/* *4     6-JAN-1989 09:58:27 GILLESPIE "(SPR 4981) Take args from command line for portability"*/
/* *3    20-DEC-1988 16:28:00 JHUANG "(SPR 4981) modified codes so 'fgets' can compare with NULL"*/
/* *2    17-DEC-1988 14:17:28 CHAU "(SPR 4981) Unisource"*/
/* *1     7-JUL-1987 11:16:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element RS_COMPILER.C*/
/* %F                             RESOURCE COMPILER

    The Resource Compiler will compile a editable text file into a  random
    access file.  After finishing "compile", the resource will create two
    files, "----.rh" for 'C', and "----.ri".
    in the same directory as the original source file.
    If the resource file is System Resource, the compiler will make a tem-
    perory copy of System Resource.

    AUTHOR : James Chang
    MOD by James Chang 12/31/86 convert from PRIME to VAX and combine data map
                and resource file.

*/

/* %C Copyright   Exploration   Systems,   Inc.          7/11/85        */


/* ----------------------#include files for ESI Finder Systems-----------------
*/
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#include "esi_time.h"
#include "esi_ctype.h"

#ifndef ESI_DTYPES_H
#include "esi_dtypes.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

/* -------------- Type define of B-tree Nodes -------------- */
/* ----- opened files list under each module ------ */
typedef struct f_block
{
    FILE *fp_resource;
    FILENAME filename;
    TREEHEAD *map_ptr;
    struct f_block *ptr;
} F_BLOCK;

/* ----- B-tree node in module B-Tree.-------- */
typedef struct md_block
{
    INT delete;
    INT t_files;
    F_BLOCK *ptr;
} MD_BLOCK;

/* ----- B-Tree node in file opened B-Tree.--- */
typedef struct open_block
{
    INT delete;
    FILE *fp_resource;
    FILENAME sfilename;
    INT t_users;
    TREEHEAD *map_ptr;
} OPEN_BLOCK;

/* ------ B-Tree node in data map B-Tree.----- */
typedef struct map_block
{
    INT flag;
    INT offset;
    INT length;
    CHAR *str_ptr;
    INT loaded ;
} MAP_BLOCK;

/* ------ B-Tree nnode in data type B-Tree.--- */
typedef struct type_block
{
    TREEHEAD *tree;
} TYPE_BLOCK;


/*  RS_MANAGER MACROS  */
#define CHECK_CURMDID if (cur_mdid EQUALS 0) return RS_NO_MODULE_OPEN
#define CHECK_SYS_INIT if (sys_init EQUALS FALSE) rsinit()
#define FILEOPEN(name, fp) if((fp=ho_open(name,"r",NULL)) EQUALS NULL)\
	return RS_CANT_OPEN_FILE
#define FIND_DATA(a,b,c)  if (btfind(a,b,c) EQUALS TRUE)
/* RS.COMPILER STRING MACROS                              */
#define TITLE1 "#define"
#define CAST   "(INT)"
#define TITLE3 "PARAMETER ("
#define LEFT_P ")"
#define TITLE31 "="
#define TITLE32 "INTEGER*4"

/* RS.COMPILER FLAG MACROS                                */
#define RS_PRELOAD       04
#define RS_PURGEABLE     32

/* RS.COMPILER PROCESS X/MAC VERSION FLAG */
#define PROCESS_X 1
#define PROCESS_M 2
#define PROCESS_DONE 3

/* RS.COMPILER READ__ MACROS                              */
#define READ_ITEM sscanf(s, "%c%c %s", &c1,&c2, item)
#define READ_ID_NUM sscanf(s,"%c%d", &c1, &id_num)
#define READ_NAME sscanf(s, "%c%c %s", &c1, &c2, id_name)
#define READ_TYPE sscanf(s, "%c%c %s", &c1, &c2, id_type);\
    id_type[4] = '\0'
        

#define DATA_NOT_FOUND else

#if USE_PROTOTYPES

publicdef INT rsclosef(CHAR *rsrcfile, INT mdid);
publicdef INT rscmapt(FILE *fp_map, TREEHEAD *new);
publicdef INT rsfpath(CHAR *os_res_file, CHAR *res_name);
publicdef INT rsfree(INT rsrcid, CHAR *rsrctype);
publicdef CHAR *rsldata(FILE *fp, MAP_BLOCK data);
publicdef INT rsqcurmd(INT *mdid);
publicdef INT rsz_getfile(FILENAME input_file, FILENAME file_name);
privatedef INT split_x_mac_rs (FILENAME rsfile);
privatedef INT rs_produce_binary (FILENAME resource, FILENAME map,
                                  FILENAME binary);
privatedef INT read_flag (CHAR *s, INT *pflag);

#else

publicdef INT rsclosef();
publicdef INT rscmapt();
publicdef INT rsfpath();
publicdef INT rsfree();
publicdef CHAR *rsldata();
publicdef INT rsqcurmd();
publicdef INT rsz_getfile();
privatedef INT split_x_mac_rs ();
privatedef INT rs_produce_binary ();
privatedef INT read_flag ();

#endif
/* ---------------------------------------------------------------------------- */


publicdef int main (argc, argv)
INT argc;
CHAR **argv;
/* %S RESOURCE COMPILER - To compile the resource file and
     to create three files for 'C', 'INFO/BASIC', and 'F77'.  */
    {
    PROGNAME ("RSCOMP");
    FILENAME resource;      /* resource filename           */
    FILENAME out_f77;       /* output filename for F77     */
    FILENAME out_c;         /* output filename for C.      */
    FILENAME out_map;       /* output filename for data map*/
    FILENAME out_bin;       /* output filename for resource binary. */
    FILENAME filename;      /* temp. storage of filename.  */
    FILE *fp_input, *fp_outf77, *fp_outc, *fp_outmap;
    CHAR s[STMAX];          /* Input string                */
    CHAR sx[STMAX];         /* Input string                */
    CHAR *eof_flag;         /* flag of end_of_file.        */
    CHAR *eof_flagx;        /* flag of end_of_file.        */
    INT ctrl_flag;          /* control flvag of string.    */
    CHAR id_type[5];        /* type of resource input I.D .*/
    CHAR id_name[64];       /* name of I.D.                */
    CHAR item[64];          /* name of item.               */
    INT id_num;             /* I.D.# of input.             */
    CHAR c1, c2;            /* temp. char. storage.        */
    INT itemc;              /* the number of items         */
    INT last_data_len;      /* the previous data length    */
    INT data_len;           /* the length of cuurent data. */
    INT file_len;           /* the length of file.         */
    INT offset;             /* the offset of the current data*/
    INT first_data;         /* to indicate the first input data*/
    UINT str_len;           /* length of string in the memory.*/
    UINT data_length;       /* the exactaly read back from file length.  */
    INT temp;
    time_t bintime;
    INT c_type;             /* Generate C include file */
    INT f_type;             /* Generate FORTRAN include file */
    CHAR ascii_time[35];
    CHAR *sp;               /* pointer into s buffer */
    CHAR language[50];      /* language */
    INT nfile;
    CHAR **file_list;
    CHAR *pdot;
    INT status;
    INT i, j;

    INT exit_flag = EXIT_SUCCESS;
    INT process_rs;
    CHAR *tempfile;
    FILENAME xrhfile;
    FILENAME xrifile;
    FILENAME rhfile;
    FILENAME rifile;
    /************************************************************************/

        

    /* Request the resource filename.                          */
    if (argc == 1)
        {
        printf (
"Usage:  RSCOMP filename ...\n\
    where filename ... is a list of resource file names (.RS files).\n\
    File name(s) can contain wildcard characters.\n\
    NOTE: DO NOT include the .RS extension for the resource file name(s)\n"
                );
        exit (exit_flag);
        }
    /* generate resource name (without extension) and file name (with extension) */
    /* Loop for each file on the command line */
    for (nfile = 1; nfile < argc; nfile++)
        {

          strcpy(s,argv[nfile]);
	  temp = strlen(s);
        /* Search for all files with specified name and .RS extension */

#ifdef vms
        if (!(temp > 3 && !strcmp (&argv[nfile][temp-3], ".rs")))
	  sprintf(s, "%s.rs", argv[nfile]);
#else
        if (!(temp > 3 && !strcmp (&argv[nfile][temp-3], ".rs")))
	  {
	    if (hoexists(s) == FALSE)
	      sprintf(s, "%s.rs", argv[nfile]);
	    else continue;
	  }
#endif

        status = ho_find_pathnames (s, &file_list);
        if (status != SUCCESS)
            {
            printf ("%s -- Resource source file %s not found\n", progname, s);
            ts_tcp_free (file_list);
            continue;
            }
	j = ts_tcp_len(file_list);
        for (i=0; i<j; i++)
            {
            tempfile = strrchr (file_list[i], ']');
            if (tempfile == (CHAR *)NULL)
                tempfile = strrchr (file_list[i], '/');
            if (tempfile != (CHAR *)NULL)
                tempfile++;
            else
                tempfile = file_list[i];
            strcpy (resource, file_list[i]);
            strcpy (filename, tempfile);
            /*     drop the trailing extension -- presumably a */
            /*     .RS extension                               */

            pdot = strrchr (filename, '.');
            *pdot = 0;

            split_x_mac_rs (resource);
            process_rs = PROCESS_X;
            time (&bintime);
            strcpy (ascii_time, ctime (&bintime));

            while (process_rs != PROCESS_DONE)
                {
                /* Temporarily, assume only C include file will be generated. */
                c_type = TRUE;
                f_type = FALSE;
                itemc = 1;  /* the number of items         */
                last_data_len = 0;  /* the previous data length    */
                data_len = 0;   /* the length of cuurent data. */
                file_len = 0;   /* the length of file.         */
                offset = 0; /* the offset of the current data*/
                first_data = 1; /* to indicate the first input data*/
                str_len = 0;    /* length of string in the memory.*/
                (process_rs == PROCESS_X) ? sprintf (resource, "%s.rsx",
                                                     filename) : 
                sprintf (resource, "%s.rsm", filename);
                fp_input = ho_open (resource, "r", NULL);
                if (fp_input != (FILE *)NULL)
                    {
                    (process_rs == PROCESS_X) ?
                        printf ("%s -- Opening file %s - X version\n",
                                progname, file_list[i]) : 
                    printf ("%s -- Opening file %s - Mac version\n", progname,
                            file_list[i]);

                    sprintf (rifile, "%s.ri", filename);
                    sprintf (xrifile, "%s.rix", filename);
                    sprintf (rhfile, "%s.rh", filename);
                    sprintf (xrhfile, "%s.rhx", filename);

                    /*     Create three filename.         */
                    if (process_rs == PROCESS_X)
                        {
                        sprintf (out_f77, "%s.rix", filename);
                        sprintf (out_c, "%s.rhx", filename);
                        sprintf (out_map, "%s.rmx", filename);
                        sprintf (out_bin, "%s.rbx", filename);
                        }
                    else
                        {
                        sprintf (out_f77, "%s.rim", filename);
                        sprintf (out_c, "%s.rhm", filename);
                        sprintf (out_map, "%s.rmm", filename);
                        sprintf (out_bin, "%s.rbm", filename);
                        }
                    /*     Open files for reading and writing.  */

                    fp_outf77 = ho_open (out_f77, "w", NULL);
                    fp_outc = ho_open (out_c, "w", NULL);
                    fp_outmap = ho_open (out_map, "w", NULL);

                    /*     write the header on include files.  */

                    fprintf (fp_outc,
                  "/*****************************************************\n");
                    fprintf (fp_outc, "\n");
                    fprintf (fp_outc, "FILE:         %s\n", rhfile);
                    fprintf (fp_outc, "\n");
                    fprintf (fp_outc, "ORIGIN:       %s\n",
                             ho_user_name (NULL));
                    fprintf (fp_outc, "DATE:         %s\n", ascii_time);
                    fprintf (fp_outc, "\n");
                    fprintf (fp_outc,
                "        This file was generated by the Resource Compiler\n");
                    fprintf (fp_outc,
                "        RSCOMP.  Changes to this file should not be done\n");
                    fprintf (fp_outc,
                   "        via any text editing mechanism.  Rather, you \n");
                    fprintf (fp_outc,
                  "        should make changes to the corresponding .rss \n");
                    fprintf (fp_outc,
                "        file, and then recompile the resource file using\n");
                    fprintf (fp_outc, "        RSCOMP\n");
                    fprintf (fp_outc, "\n");
                    fprintf (fp_outc,
                  "*****************************************************/\n");
                    fprintf (fp_outc, "\n");


                    fprintf (fp_outf77,
                  "C*****************************************************\n");
                    fprintf (fp_outf77, "C\n");
                    fprintf (fp_outf77, "C FILE:         %s\n", rifile);
                    fprintf (fp_outf77, "C\n");
                    fprintf (fp_outf77, "C ORIGIN:       %s\n",
                             ho_user_name (NULL));
                    fprintf (fp_outf77, "C DATE:         %s\n", ascii_time);
                    fprintf (fp_outf77, "C\n");
                    fprintf (fp_outf77,
               "C        This file was generated by the Resource Compiler\n");
                    fprintf (fp_outf77,
               "C        RSCOMP.  Changes to this file should not be done\n");
                    fprintf (fp_outf77,
               "C        via any text editing mechanism.  Rather, you \n");
                    fprintf (fp_outf77,
               "C        should make changes to the corresponding .rss \n");
                    fprintf (fp_outf77,
               "C        file, and then recompile the resource file using\n");
                    fprintf (fp_outf77, "C        RSCOMP\n");
                    fprintf (fp_outf77, "C\n");
                    fprintf (fp_outf77,
                 "C******************************************************\n");
                    fprintf (fp_outf77, "C\n");

                    file_len = 0;   /* reinitialize for each file */

                    /* Read and analyze the data from resource file.           */
                    eof_flag = fgets (s, STMAX, fp_input);
                    while (eof_flag != NULL)
                        {
                        /* skip any comments at the beginning of the file */
                        while ((s[0] == ';') && (eof_flag != NULL))
                        eof_flag = fgets (s, STMAX, fp_input);

                        ho_query_offset_length (s, &str_len, &data_length);
                        if (s[0] == ',')
                            {

                            /* the input data is I.D. number, rscord the status for last I.D.  */

                                
                            last_data_len = data_len;
                            data_len = 0;
                            if (first_data != 1)
                                {
                                fprintf (fp_outmap, "L %d\n", last_data_len);
                                fprintf (fp_outmap, "*\n");
                                }
                            first_data = 0;
                            offset = file_len;
                            data_len += data_length;
                            READ_ID_NUM;
                            /* Keep I.D. status in data map file.                     */
                            fprintf (fp_outmap, ", %d\n", id_num);
                            fprintf (fp_outmap, "T %s\n", id_type);
                            fprintf (fp_outmap, "O %d\n", offset);
                            }
                        else
                            {
                            if (s[0] == '*')
                                {
                                switch (s[1])
                                    {
                                case 'L':
                                    c_type = FALSE;
                                    f_type = FALSE;
                                    sp = s;
                                    while (TRUE)
                                        {
                                        sp = ts_next_word (sp, language);
                                        if (language[0] == 0)
                                            break;
                                        ts_sto_upper (language);
                                        if (strcmp ("C", language) == 0)
                                            c_type = TRUE;
                                        if (strcmp ("FORTRAN", language) == 0)
                                            f_type = TRUE;
                                        if (strcmp ("ALL", language) == 0)
                                            {
                                            f_type = TRUE;
                                            c_type = TRUE;
                                            }
                                        }
                                    break;

                                case 'T':
                                    READ_TYPE;
                                    break;
                                case 'N':
                                    data_len += data_length;
                                    READ_NAME;
                                    itemc = 1;
                                    printf ("%s --   Reading %s\n", progname,
                                            id_name);
                                    fprintf (fp_outc, "%s %s\t%s%d\n", TITLE1,
                                             id_name, CAST, id_num);
                                    fprintf (fp_outf77, "      %s %s\n",
                                             TITLE32, id_name);
                                    fprintf (fp_outf77,
                                             "      %s %s %s %d %s\n", TITLE3,
                                             id_name, TITLE31, id_num,
                                             LEFT_P);
                                    break;
                                case 'S':
                                    read_flag (s, &ctrl_flag);
                                    fprintf (fp_outmap, "F %d\n", ctrl_flag);
                                    ctrl_flag = 0;
                                    data_len += data_length;
                                    break;

                                case 'I':
                                    data_len += data_length;
                                    READ_ITEM;
                                    fprintf (fp_outc, "%s %s\t%s%d\n", TITLE1,
                                             item, CAST, itemc);
                                    fprintf (fp_outf77, "      %s %s\n",
                                             TITLE32, item);
                                    fprintf (fp_outf77,
                                             "      %s %s %s %d %s\n", TITLE3,
                                             item, TITLE31, itemc, LEFT_P);
                                    itemc++;
                                    break;

                                default:
                                    data_len += data_length;
                                    break;
                                    }
                                }
                            else
                                data_len += data_length;
                            }
                        file_len += str_len;    /* strlen(s);*/
                        eof_flag = fgets (s, STMAX, fp_input);
                        }

                    /* record the last data and close all files.               */
                    last_data_len = data_len;
                    fprintf (fp_outmap, "L %d\n", last_data_len);
                    fprintf (fp_outmap, "*\n");
                    fprintf (fp_outmap, "#\n");
                    fclose (fp_input);
                    fclose (fp_outmap);
                    fclose (fp_outf77);
                    fclose (fp_outc);

                    /* combines xx.RS file and xx.RM to be xx.RB file.    */
                    rs_produce_binary (resource, out_map, out_bin);
                    ho_delete_file (resource, &status);
                    if (process_rs == PROCESS_M && hoexists (xrhfile) == TRUE)
                        {
                        fp_outc = ho_open (out_c, "r", NULL);
                        fp_input = ho_open (xrhfile, "r", NULL);
                        eof_flag = fgets (s, STMAX, fp_outc);
                        eof_flagx = fgets (sx, STMAX, fp_input);
                        status = SUCCESS;
                        while (eof_flag != NULL && status == SUCCESS)
                            {
                            if (strcmp (s, sx) != 0)
                                {
                                status = FAIL;
								exit_flag = EXIT_FAILURE;
                                break;
                                }
                            else
                                {
                                eof_flag = fgets (s, STMAX, fp_outc);
                                eof_flagx = fgets (sx, STMAX, fp_input);
                                }
                            }
                        if (eof_flag != eof_flagx || status == FAIL)
                            {
                            printf ("\nPlease note\n");
                            printf (
               "%s --  Mac & X version include files of %s are different\n\n",
                                    progname, file_list[i]);
                            }
                        fclose (fp_outc);
                        fclose (fp_input);
                        }
                    if (!c_type || (process_rs == PROCESS_M &&
                                    hoexists (xrhfile) == TRUE))
                        ho_delete_file (out_c, &status);
                    if (!f_type || (process_rs == PROCESS_M &&
                                    hoexists (xrifile) == TRUE))
                        ho_delete_file (out_f77, &status);
                    }
                process_rs = (process_rs == PROCESS_X) ?
                    PROCESS_M : PROCESS_DONE;
                }           /*WHILE*/
            printf ("%s -- Done.\n", progname);

	    sprintf (xrhfile, "%s.rhx", filename);
	    sprintf (rhfile,  "%s.rh", filename);
	    if (hoexists(xrhfile) == TRUE)
	      {
		ho_rename(xrhfile,rhfile);
	      }
	    else
	      {
		sprintf (xrhfile, "%s.rhm", filename);
		if (hoexists(xrhfile) == TRUE)
		  {
		    ho_rename(xrhfile,rhfile);
		  }
	      }
	    sprintf (xrifile, "%s.rix", filename);
	    sprintf (rifile,  "%s.ri", filename);
	    if (hoexists(xrifile) == TRUE)
	      {
		ho_rename(xrifile,rifile);
	      }
	    else
	      {
		sprintf (xrifile, "%s.rim", filename);
		if (hoexists(xrifile) == TRUE)
		  {
		    ho_rename(xrifile,rifile);
		  }
	      }

            }               /*FOR*/
        ts_tcp_free (file_list);
        }                   /*FOR*/

    exit (exit_flag);
    }

/* ----------------------------------------------------------------------------
*/
privatedef INT read_flag (s, pflag)

/* %S Read and analyze the flag.                             */
CHAR s[];
INT *pflag;
    {
    CHAR ts[10], ts1[10], ts2[10];  /* temp. string storage  */
    INT iv1, iv2;           /* temp. int. storage.   */
    INT cmp1, cmp2, cmp3;   /* compare result flaag. */

    sscanf (s, "%s %s %d %s %d", ts, ts1, &iv1, ts2, &iv2);

    /* is preload flag set?                                     */
    cmp1 = (_tolower (ts1[0]) == 'p');
    cmp2 = (_tolower (ts1[1]) == 'r');
    cmp3 = (_tolower (ts1[1]) == 'u');
    if (cmp1 && cmp2)
        *pflag = (iv1 * RS_PRELOAD);
    if (cmp1 && cmp3)
        *pflag = (iv1 * RS_PURGEABLE);

    /* Is preload flag set?                                    */
    cmp1 = (_tolower (ts2[0]) == 'p');
    cmp2 = (_tolower (ts2[1]) == 'r');
    cmp3 = (_tolower (ts2[1]) == 'u');
    if (cmp1 && cmp2)
        *pflag = (iv2 * RS_PRELOAD) + *pflag;
    if (cmp1 && cmp3)
        *pflag = (iv2 * RS_PURGEABLE) + *pflag;
    return(*pflag);
    }

/* =========================rs_produce_binary ================================*/
privatedef INT rs_produce_binary (resource, map, binary)

/*%S to combine the data map and data resource.   */

/*%P
    Input Parameters :
     RESOURCE =( FILENAME) the resource file.
     MAP      =( FILENAME) the map file.
     BINARY   =( FILENAME) the binary file.

    Onput Parameters :
     None.
*/
FILENAME resource, map, binary;
    {
    INT map_offset = 0, status = SUCCESS;
    UINT data_len = 0, str_len = 0;
    CHAR s[STMAX], *eof;
    FILE *fp_input, *fp_outmap, *fp_outbin;

    /* to count the offset of  the map file.   */
    fp_outmap = ho_open (map, "r", NULL);
    while ((eof = fgets (s, STMAX, fp_outmap)) != NULL)
        {
        ho_query_offset_length (s, &str_len, &data_len);
        map_offset += str_len;
        }
    fclose (fp_outmap);

    fp_input = ho_open (resource, "r", NULL);
    fp_outmap = ho_open (map, "r", NULL);
    fp_outbin = ho_open (binary, "w", NULL);
    sprintf (s, "%d\n", map_offset);
    fprintf (fp_outbin, "%s", s);

    /* to copy the map file.   */

#ifdef primos
    while ((eof = fgets (s, STMAX, fp_outmap)) != NULL)
    fprintf (fp_outbin, "%s", s);

    /* to copy the resource file.  */
    while ((eof = fgets (s, STMAX, fp_input)) != NULL)
        {
        while ((eof != NULL) && (s[0] == ';'))
        eof = fgets (s, STMAX, fp_input);   /* skip comments */
        fprintf (fp_outbin, "%s", s);
        }
#endif

#if defined vms || defined sun || defined mips
    while ((eof = fgets (s, STMAX, fp_outmap)) != NULL)
    fputs (s, fp_outbin);

    /* to copy the resource file.  */
    while ((eof = fgets (s, STMAX, fp_input)) != NULL)
        {
        while ((eof != NULL) && (s[0] == ';'))
        eof = fgets (s, STMAX, fp_input);   /* skip comments */
        fputs (s, fp_outbin);
        }
#endif

    fclose (fp_input);
    fclose (fp_outmap);
    fclose (fp_outbin);
    ho_delete_file (map, &status);
    return SUCCESS;
    }

privatedef INT split_x_mac_rs (rsfile)
FILENAME rsfile;
    {
    FILENAME filename;
    CHAR *tempfile;
    FILENAME xrsfile;
    FILENAME macrsfile;
    FILE *rsinput, *xoutput, *macoutput, *output;
    CHAR *eof_flag, *pdot;
    CHAR s[STMAX];
    INT no_error;

    no_error = TRUE;
    tempfile = strrchr (rsfile, ']');
    if (tempfile == (CHAR *)NULL)
        tempfile = strrchr (rsfile, '/');
    if (tempfile != (CHAR *)NULL)
        tempfile++;
    else
        tempfile = rsfile;
    strcpy (filename, tempfile);
    pdot = strrchr (filename, '.');
    *pdot = 0;
    rsinput = ho_open (rsfile, "r", NULL);
    if (rsinput == (FILE *)NULL)
        {
        printf ("Resource source file %s not found\n", rsfile);
        return(FAIL);
        }
    else
        {
        sprintf (xrsfile, "%s.rsx", filename);
        sprintf (macrsfile, "%s.rsm", filename);
        eof_flag = fgets (s, STMAX, rsinput);
        if (s[0] != '*')
            {
            printf ("Resource file %s invalid format at first line\n",
                    rsfile);
            }
        else if (s[1] != 'U')
            {
            xoutput = ho_open (xrsfile, "w", NULL);
            macoutput = ho_open (macrsfile, "w", NULL);
            while ((eof_flag != NULL) && (no_error == TRUE))
                {
                fprintf (xoutput, "%s", s);
                fprintf (macoutput, "%s", s);
                eof_flag = fgets (s, STMAX, rsinput);
                if (s[0] == '*' && s[1] == 'U')
                    no_error = FALSE;
                }
            fclose (xoutput);
            fclose (macoutput);
            }
        else if (s[2] == 'X' || s[2] == 'M')
            {
            output = (s[2] == 'X') ? ho_open (xrsfile, "w", NULL) : 
            ho_open (macrsfile, "w", NULL);
            eof_flag = fgets (s, STMAX, rsinput);
            while (eof_flag != NULL)
                {
                fprintf (output, "%s", s);
                eof_flag = fgets (s, STMAX, rsinput);
                if (s[0] == '*' && s[1] == 'U' && (s[2] == 'M' ||
                                                   s[2] == 'X'))
                    {
                    fclose (output);
                    output = (s[2] == 'X') ? ho_open (xrsfile, "w", NULL) : 
                    ho_open (macrsfile, "w", NULL);
                    eof_flag = fgets (s, STMAX, rsinput);
                    }
                }
            fclose (output);
            }
        else
            {
            printf ("Resource file %s invalid format at first line\n",
                    rsfile);
            }
        fclose (rsinput);
        return(SUCCESS);
        }
    }

