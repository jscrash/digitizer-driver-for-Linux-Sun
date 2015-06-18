/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd
			Corte Madera, CA  USA 94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifdef PRIOR_GKS
#include "sun_ansicgks.h"
#endif 


/* Function Description -----------------------------------------------------
Description: Open a metafile workstation

Prototype:
	publicdef INT pf_open_plotfile (ws,metafile,meta_type,mode)
Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
		INT ws;  	(I)
		CHAR *metafile; (I)
		CHAR *meta_type;(I)
		CHAR *mode;     (I)

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/


#if USE_PROTOTYPES
publicdef INT pf_open_plotfile (INT ws,CHAR *metafile,CHAR *meta_type,CHAR *mode)
#else
publicdef INT pf_open_plotfile (ws,metafile,meta_type,mode)
INT ws;
CHAR *metafile;
CHAR *meta_type;
CHAR *mode;
#endif
    {
    CHAR msg[80];
    INT metatype;

#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */

    /* -------------------------------- */
    /* For PRIOR GKS                    */
    /* if mode = write                  */
    /* available plotfile type: CGMT    */
    /*                          CGMC    */
    /*                          CGMB    */
    /*                          GKSM    */
    /*                          GKSMT   */
    /*                          GKSMB   */
    /*                          PS      */
    /*                          PSM     */
    /*                          PSC     */
    /* -------------------------------- */
    if (mode[0] EQUALS 'w' OR mode[0] EQUALS 'W')
	{
        if (ARE_SAME (meta_type, "CGMT"))
            {
            gopenws (ws, metafile, "cgmoct");
            }
        else if (ARE_SAME (meta_type, "CGMC"))
            {
            gopenws (ws, metafile, "cgmoce");
            }
        else if (ARE_SAME (meta_type, "CGMB"))
            {
            gopenws (ws, metafile, "cgmobe");
            }
        else if (ARE_SAME (meta_type, "GKSM"))
            {
            gopenws (ws, metafile, "moasc");
            }
        else if (ARE_SAME (meta_type, "GKSMT"))
            {
            gopenws (ws, metafile, "moasc");
            }
        else if (ARE_SAME (meta_type, "GKSMB"))
            {
            gopenws (ws, metafile, "mobin");
            }
        else if (ARE_SAME (meta_type, "PS"))
            {
            gopenws (ws, metafile, "postscript");
            }
        else if (ARE_SAME (meta_type, "PSC"))
            {
            gopenws (ws, metafile, "postscriptc");
            }
        else if (ARE_SAME (meta_type, "PSM"))
            {
            gopenws (ws, metafile, "postscriptm");
            }
        else
            {
            sprintf (msg, "Metatype %s not supported", meta_type);
            am_message (AM_STATUS, msg);
            return FAIL;
            }
        }
    /* -------------------------------- */
    /* For PRIOR GKS                    */
    /* if mode = read                   */
    /* available plotfile type: CGMT    */
    /*                          CGMC    */
    /*                          CGMB    */
    /*                          GKSM    */
    /*                          GKSMT   */
    /*                          GKSMB   */
    /* -------------------------------- */
    else if (mode[0] EQUALS 'r' OR mode[0] EQUALS 'R')
	{
        if (ARE_SAME (meta_type,"CGMT"))
            {
            gopenws (ws, metafile, "mi");
            }
        else if (ARE_SAME (meta_type,"CGMC"))
            {
            gopenws (ws, metafile, "mi");
            }
        else if (ARE_SAME (meta_type,"CGMB"))
            {
            gopenws (ws, metafile, "mi");
            }
        else if (ARE_SAME (meta_type,"GKSM"))
            {
            gopenws (ws, metafile, "mi");
            }
        else 
            {
            sprintf (msg,"%s metafile type not support.",meta_type);
            am_message (AM_STATUS,msg);
            return FAIL;
            }
	}
    else
	{
	am_message (AM_STATUS,"pf_open_plotfile: Invalid mode");
	return FAIL;
	}
	
#else  /* ========== BEGIN DECW GKS ========== */
    /* -------------------------------- */
    /* For DEC GKS                      */
    /* if mode = write                  */
    /* available plotfile type: CGMT    */
    /*                          CGMC    */
    /*                          CKSM    */
    /* -------------------------------- */
	
    /* --------------------------------------------- */
	/* to get around for DEC GKS putting .LIS as the */
	/* file extension of the metafile                */
	/* --------------------------------------------- */
    strcat (metafile,".");
	

    if (mode[0] EQUALS 'w' OR mode[0] EQUALS 'W')
	{
        if (ARE_SAME (meta_type, "CGMT"))
            {
            metatype = GKS$K_CGM_OUTPUT | GKS$M_CLEAR_TEXT_ENCODING;
            }
        else if (ARE_SAME (meta_type, "CGMC"))
            {
            metatype = GKS$K_CGM_OUTPUT | GKS$M_CHARACTER_ENCODING;
            }
        else if (ARE_SAME (meta_type, "GKSM"))
            {
            metatype = GKS$K_GKSM_OUTPUT;
            }
        else if (ARE_SAME (meta_type, "PS"))
            {
            metatype = GKS$K_POSTSCRIPT;
            }
        else
            {
            sprintf (msg, "Metatype %s not supported", meta_type);
            am_message (AM_STATUS, msg);
            return FAIL;
            }
        }
    /* -------------------------------- */
    /* For DEC GKS                      */
    /* if mode = read                   */
    /* available plotfile type: GKSM    */
    /* -------------------------------- */
    else if (mode[0] EQUALS 'r' OR mode[0] EQUALS 'R')
	{
        if (ARE_SAME (meta_type, "GKSM"))
            {
            metatype = GKS$K_GKSM_INPUT;
	    }
	}
    else
	{
	am_message (AM_STATUS,"pf_open_plotfile: Invalid mode");
	return FAIL;
	}
	
    gks$open_ws (&ws, hoz_str_descpt (metafile), &metatype);
#endif  /* ========== END DECW GKS ========== */
    return SUCCESS;
    }


/* Function Description -----------------------------------------------------
Description: 
    Display plotfile routine

Prototype:
    publicdef INT pf_display_pf (metafile, meta_type)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) CHAR *metafile;
    (I) CHAR *meta_type;

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#if USE_PROTOTYPES
publicdef INT pf_display_pf (CHAR *metafile, CHAR *meta_type)
#else
publicdef INT pf_display_pf (metafile, meta_type)
CHAR *metafile;
CHAR *meta_type;
#endif
    {
    BOOL is_gksm;
    INT ws = GKS_METAIN_WKID;
    INT status = SUCCESS;

#ifdef PRIOR_GKS
    Ggksmit item;
    Gchar *data;
#else
    INT type;
    INT length;
    INT max_length;
    CHAR datarec[1000];
#endif

    /* ------------------------- */
    /* open plotfile for reading */
    /* ------------------------- */
    status = pf_open_plotfile (ws,metafile,meta_type,"r");

    if (status != SUCCESS)
	{
	return FAIL;
	}

    /* ---------------- */
    /* set is_gksm flag */
    /* ---------------- */
    if (ARE_SAME(meta_type,"GKSM"))
	{
	is_gksm = TRUE;
	}
    else
	{
	is_gksm = FALSE;
	}

  /* -------------------------------------- */
  /* reading and interpreting call sequence */
  /* -------------------------------------- */
#ifdef PRIOR_GKS
  do {
     item = ggetgksm (ws);
     data = greadgksm (ws,item.length);
     if (item.type > 0)
       {
       /* ----------------------------------------------------- */
       /* get rid of segment item, color set item, refresh item */
       /* ----------------------------------------------------- */
       if (is_gksm && item.type < 70 && item.type != 56)
	 {
         ginterpret (&item,data);
	 }
       else if (!is_gksm && item.type < 70 && item.type != 1 && item.type != 3
		&& item.type != 4 && item.type != 56)
	 {
         ginterpret (&item,data);
	 }
       }
     free (data);
   } while (item.type > 0);

#else
  max_length = 1000;
  do {
     gks$get_item (&ws,&type,&length);
     gks$read_item (&ws,&max_length,datarec);
     
     if (type > 0)
       {
       if (is_gksm && type < 70 && type != 56)
	 {
         gks$interpret_item (&type,&length,datarec);
	 }
       else if (!is_gksm && type < 70 && type != 1 && type != 3
		&& type != 4 && type != 56)
	 {
         gks$interpret_item (&type,&length,datarec);
	 }
       }
   } while (type > 0);
#endif

   /* ----------------- */
   /* close workstation */
   /* ----------------- */
   wm_gclwk (ws);

   return SUCCESS;
}

