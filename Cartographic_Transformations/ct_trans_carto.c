/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_CARTO.C*/
/* *5    25-JAN-1991 11:40:54 GILLESPIE "(SPR 1)  Rework of user interface; add file i/o"*/
/* *4     6-NOV-1990 14:29:26 PURNA "(SPR 6086) change scanf %f to scanf %lf"*/
/* *3    13-OCT-1989 17:20:19 PURNA "(SPR 5020) Mods for CT parameter change"*/
/* *2    13-OCT-1989 17:11:04 PURNA "(SPR 5020) Mods for CT parameter change"*/
/* *1     1-JUL-1989 12:23:01 GILLESPIE ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_TRANS_CARTO.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc
            c/o Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/******************************************************************************/
/*   Utility to perform cartographic conversions                              */
/******************************************************************************/

#include "esi_c_lib.h"
#include "esi_ho_files.h"
#include "esi_ct.h"
#include "esi_fi.h"
#include "esi_tc.h"
#include "esi_ts.h"
#include "esi_qs.h"
#include "esi_or.h"
INT main (argc, argv)
INT argc;
CHAR **argv;
    {
    COORD xy_in[2], xy_out[2];
    PROJECTION_STRUCTURE *in_projection, *out_projection;
    PROJECTION_NAME projection_id;
    PROJECT_NAME project;
    INT status;
    TCP projection_list;
    INT nitems;
    UINT i;
    UINT id;
    FILE *in = stdin;
    FILENAME in_filename;
    FILENAME out_filename;
    FILE *out = NULL_FILE_PTR;
    CHAR string[133];
    BOOL verbose = FALSE;
    CHAR in_format[132];
    CHAR out_format[132];
    
    strcpy (project, "");
    strcpy (in_format, "%lf %lf");
    strcpy (out_format, "%f\t%f\t%f\t%f\n");
    for (i = 1; i < argc; i++)
        {
        if (ARE_SAME (argv[i], "-help") OR ARE_SAME (argv[i],
                                                     "-HELP") OR 
            ARE_SAME (argv[i], "-?") OR ARE_SAME (argv[i], "?"))
            {
            goto format;
            }
        else if (ARE_SAME (argv[i], "-project") OR ARE_SAME (argv[i], "-PROJECT"))
            {
            if (++i EQUALS argc)
                {
                goto format;
                }
            strcpy (project, argv[i]);
            }
        else if (ARE_SAME (argv[i], "-infile") OR ARE_SAME (argv[i], "-INFILE"))
            {
            if (++i EQUALS argc)
                {
                goto format;
                }
            strcpy (in_filename, argv[i]);
            in = ho_open (in_filename, "r", NULL);
            if (in EQUALS NULL_FILE_PTR)
                {
                fprintf (stderr, "Problem opening input file '%s'\n", in_filename);
                goto format;
                }
            }
        else if (ARE_SAME (argv[i], "-outfile") OR ARE_SAME (argv[i], "-OUTFILE"))
            {
            if (++i EQUALS argc)
                {
                goto format;
                }
            strcpy (out_filename, argv[i]);
            out = ho_open (out_filename, "w", NULL);
            if (out EQUALS NULL_FILE_PTR)
                {
                fprintf (stderr, "Problem opening file '%s' for output\n",
                         out_filename);
                goto format;
                }
            }
        else if (ARE_SAME (argv[i], "-in_format") OR ARE_SAME (argv[i], "-IN_FORMAT"))
            {
            if (++i EQUALS argc)
                {
                goto format;
                }
            strcpy (in_format, argv[i]);
            }
        else if (ARE_SAME (argv[i], "-out_format") OR ARE_SAME (argv[i],
                                                                "-OUT_FORMAT"))
            {
            if (++i EQUALS argc)
                {
                goto format;
                }
            strcpy (out_format, argv[i]);
            }
        else if (ARE_SAME (argv[i], "-v") OR ARE_SAME (argv[i],
                                                       "-V") OR 
                 ARE_SAME (argv[i], "-verbose") OR ARE_SAME (argv[i], "-VERBOSE"))
            {
            verbose = TRUE;
            }
        else if (i > 1)
            {
            fprintf (stderr, "Unrecognized input parameter #%d: %s\n", i, argv[i]);
            }
        }
    /*  Login to the database            */
    
    status = or_login (argc, argv);
    if (status != SUCCESS)
        {
        exit (EXIT_FAILURE);
        }
    if (IS_EMPTY_STRING (project))
        {
        printf ("\nEnter the project name: ");
        gets (project);
        }
    qs_set_c (QS_PROJECT_NAME, project, 0);
    status = finder_config_project (FALSE);
    if (status != SUCCESS)
        {
        fprintf (stderr, "There is a problem attaching to this project:\n%70.70s",
                 status);
        exit (EXIT_FAILURE);
        }
again:
    printf ("\nA blank input line terminates entry . . .\n");
    status = ct_projection_list (&projection_list, &nitems, FALSE);
    if (status != SUCCESS)
        {
        fprintf (stderr,
                 "There is a problem fetching the list of projections:\n%70.70s",
                 status);
        exit (EXIT_FAILURE);
        }
    ts_tcp_sort (projection_list);
    
    ct_get_default_project (projection_id);
    printf ("\nThe following projections are available (default = *):\n");
    for (i = 0; i < nitems; i++)
        {
        if (ARE_SAME (projection_id, projection_list[i]))
            {
            printf ("*");
            }
        else
            {
            printf (" ");
            }
        printf ("%2.2d: %s\n", i + 1, projection_list[i]);
        }
    /*  Get the present projection type  */
    
    printf ("\nEnter the number of the projection to be converted FROM: ");
    FOREVER
        {
        id = 0;
        gets (string);
        if (IS_EMPTY_STRING (string))
            {
            goto way_out;
            }
        sscanf (string, "%d", &id);
        if (id < 1 OR id > nitems)
            {
            printf ("Enter a number between 1 and %d\n", nitems);
            continue;
            }
        break;
        }
    if (ct_get_projection (projection_list[id - 1], &in_projection) != SUCCESS)
        {
        exit (EXIT_FAILURE);
        }
    /*  Get the output projection  */
    
    printf ("Enter the number of the projection to be converted TO: ");
    FOREVER
        {
        id = 0;
        gets (string);
        if (IS_EMPTY_STRING (string))
            {
            goto way_out;
            }
        sscanf (string, "%d", &id);
        if (id < 1 OR id > nitems)
            {
            printf ("Enter a number between 1 and %d\n", nitems);
            continue;
            }
        break;
        }
    if (ct_get_projection (projection_list[id - 1], &out_projection) != SUCCESS)
        {
        exit (EXIT_FAILURE);
        }
    if (verbose)
        {
        printf ("\
          Project: %s\n\
 Input projection: %s\n\
Output Projection: %s\n"
                , project, in_projection->projection_id,
                out_projection->projection_id);
        if (in != stdin)
            {
            printf ("\
       Input file: %s\n\
     Input format: %s\n", in_filename, in_format);
            }
        if (out != NULL_FILE_PTR)
            {
            printf ("\
      Output file: %s\n\
    Output format: %s\n", out_filename, out_format);
            }
        }
    /*  Convert the points as they are input  */
    
    xy_in[0] = xy_in[1] = 0.;
    
    FOREVER
        {
        if (in_projection->projection_type == 0)
            {
            if (in EQUALS stdin)
                printf ("Input coordinates (latitude longitude):");
            if (fgets (string, sizeof string, in) EQUALS NULL OR strlen (string) < 2)
                {
                break;
                }
            sscanf (string, in_format, &(xy_in[1]), &(xy_in[0]));
            }
        else
            {
            if (in EQUALS stdin)
                printf ("Input coordinates (x y):");
            if (fgets (string, sizeof string, in) EQUALS NULL OR strlen (string) < 2)
                {
                break;
                }
            sscanf (string, in_format, &(xy_in[0]), &(xy_in[1]));
            }
        if (verbose)
            {
            printf ("Input: %f\t%f\n", xy_in[0], xy_in[1]);
            }
        status = ct_transform_point (xy_in[0], xy_in[1], in_projection, &xy_out[0],
                                     &xy_out[1], out_projection);
        if (status != 0)
            {
            fprintf (stderr, "USGS cartographic transform failure. Return code:%d",
                     status);
            continue;
            }
        if (in EQUALS stdin OR verbose)
            {
            if (out_projection->projection_type == 0)
                {
                printf ("Latitude: %.8f    Longitude: %.8f\n", xy_out[1], xy_out[0]);
                }
            else
                {
                printf ("X: %.2f    Y: %.2f\n", xy_out[0], xy_out[1]);
                }
            }
        if (out != NULL_FILE_PTR)
            {
            if (in_projection->projection_type == 0)
                {
                COORD temp = xy_in[0];
                xy_in[0] = xy_in[1];
                xy_in[1] = temp;
                }
            if (out_projection->projection_type == 0)
                {
                COORD temp = xy_out[0];
                xy_out[0] = xy_out[1];
                xy_out[1] = temp;
                }
            fprintf (out, out_format, xy_in[0], xy_in[1], xy_out[0], xy_out[1]);
            }
        }
    if (out != NULL_FILE_PTR)
        {
        if (verbose)
            {
            printf ("Closing output file.\n");
            }
        fclose (out);
        }
    if (in EQUALS stdin)
        {
        goto again;
        }
way_out:
    or_logout (FALSE);
    exit (EXIT_SUCCESS);
format:
    printf (
"Program %s\n\
Parameters:\n\
name/password [-project project_name] [-infile file_name] [-outfile file_name]\n\
[-in_format format] [-out_format format] [-verbose | -v]\n\
where\n\
    name/password is a valid Finder account name and password\n\
    project_name is the name of the project containing the projections\n\
    -infile is the optional name of a file containing the X/Y pairs to be converted\n\
    -outfile is the optional name of a file containing the results of the calculations.\n\
    -in_format is the C style format to use for data entry (default = %%lf %%lf)\n\
    -out_format is the C style format to use for writing to output file (default = %%f\\t%%f\\t%%f\\t%%f)\n\
    -verbose is a flag indicating more feedback is desired."
            , argv[0]);
    exit (EXIT_SUCCESS);
    }
