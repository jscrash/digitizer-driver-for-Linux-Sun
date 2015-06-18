/* DEC/CMS REPLACEMENT HISTORY, Element OR_IAPXTB.C */
/*  4    15-OCT-1991 16:11:54 JANTINA "(SPR 0) Added dd_getseismic user exit" */
/* *3     8-JAN-1991 16:15:46 GILLESPIE "(SPR 6347) Add ADDPATH user exit" */
/* *2    31-OCT-1990 15:07:57 GILLESPIE "(SPR 37) Move from or_iapxtb.mar - better for Unix compatability" */
/* *1    31-OCT-1990 15:02:07 GILLESPIE "Oracle user exit file (C version)" */
/* DEC/CMS REPLACEMENT HISTORY, Element OR_IAPXTB.C */
/* sqlforms,20310,v5$src:[sqlforms.utilh.20310] */
/* copyright (c) 1984 by the Oracle Corporation */
 
/*
NAME
  Definitions for IAP User Exits.
FUNCTION
  Should be included in GENXTB.C and IAPPRS.C.
NOTES
  See IAPPRS.C {Parse/Call IAP User Exit};
  and GENXTB.C {Generate iapxtb[] table}.
OWNER
  Friend
DATE
  11/15/84
MODIFIED
    Gillespie  10/31/90 - Merged into Finder
    Rolle      01/14/88 - more XIT types; remove obsolete stuff
    Fine       12/30/87 - Fix preprocessor text
    Morse      09/09/87 - Add Ada exit call
  Dana	     06/14/85 - Fix nested comment
  Dana	     07/01/85 - Linting
  Dana	     07/12/85 - Change VAX11C to vax11c
*/
 
#ifdef vms
/* Defines for extd, extf, externdef */
# define  extd	      globalref 	     /* use for DECLARING global data */
# define  externdef   globaldef 	     /* use for DEFINING  global data */
# define  extf	      extern		       /* use for declaring FUNCTIONS */
#pragma nostandard
#else
# define  extd	      extern		     /* use for DECLARING global data */
# define  externdef			     /* use for DEFINING  global data */
# define  extf	      extern		       /* use for declaring FUNCTIONS */
#endif
 
typedef struct	exitr				/* Struct definition for exits */
{
	 char	  *exnam;		/* Name of the user exit */
	 int	   (*exfp)();		/* Pointer to the exit routine */
	 int	   extyp;		/* Type code for exit routine  */
#define  XITCC1    1			/* C (for existing exits)      */
#define  XITCC	   2			/* C (call-by-reference)       */
#define  XITCOB    3			/* Cobol 		      */
#define  XITFOR    4			/* Fortran		      */
#define  XITPLI    5                    /* PL/I                             */
#define  XITPAS    6                    /* Pascal                           */
#define  XITAda    7                    /* Ada                              */
} exitr;
 
/* Define the user exits for IAP. */

extf	int	dd_digexit();
extf	int	dd_getuwi();
extf	int	orf_init();
extf	int	dd_get_map();
extf	int	dd_get_xs();
extf	int	sehorld();
extf    int     or_hoaddpath();

externdef	exitr	iapxtb[] = {
	"DD_DIGEXIT",	dd_digexit,	XITCC,
	"DD_GETUWI",	dd_getuwi,	XITCC,
	"ORF_INIT",	orf_init,	XITCC,
	"DD_GET_MAP",	dd_get_map,	XITCC,
	"DD_GET_XS",	dd_get_xs,	XITCC,
	"SEHORLD",	sehorld,	XITCC,
	"ADDPATH",	or_hoaddpath,	XITCC,
	(char *)0,	0,	0
	};

#ifdef vms
#pragma standard
#endif
