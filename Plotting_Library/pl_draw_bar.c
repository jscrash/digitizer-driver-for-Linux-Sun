/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_BAR.C */
/* *4    29-JUL-1991 13:39:00 JESSIE "(SPR 8744) status checking for TU_UNIT_CONVERSION case#3453" */
/* *3    16-JAN-1991 17:02:51 MING "(SPR -1) change dimension for label_uom1, label_uom2" */
/* *2    23-OCT-1990 15:18:33 MING "(SPR 6075) fix bug on changing 0.0 to 0.00 when doing checking invalid scale" */
/* *1    12-OCT-1990 17:37:24 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_BAR.C */

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



/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT func (arg);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg	- description

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/


#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
 
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif


#ifndef ESI_ORACLE_H
#include "esi_oracle.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif
publicdef INT round(); 

#if USE_PROTOTYPES
publicdef INT pl_draw_bar(CHAR *name,CHAR *scale_uom,
			  GKS_REAL tot_length,GKS_REAL height)
#else 

publicdef  INT pl_draw_bar (name,scale_uom,tot_length,height)
CHAR     *name;
CHAR     *scale_uom;
GKS_REAL tot_length;
GKS_REAL height;
#endif
{
   
   GKS_INT color;
   GKS_INT font,precision;
   INT     i,ii,j,width;
   INT     interval_len_cmin[2];
   BOOL    flag;
   DOUBLE  scale;
   DOUBLE  interval_len;
   DOUBLE  con_m_k,con_i_c;
   CHAR    a[8];
   CHAR    metric_c='Y';
   CHAR    nmetric_c='Y';
   CHAR    meter_mile_unit[20],cm_in_unit[20];
   DOUBLE  base_unit;
   GKS_REAL first_pos[2],len_per_interval[2];
   GKS_REAL ary_x[5],ary_y[5];
   GKS_REAL uu,tot_len[2];
   GKS_REAL ary_y_center[2],y_text;
   GKS_REAL line1,line2,line3,b_line,t_line;
   CHAR     test[20];
   CHAR     label_uom1[20],label_uom2[20];
   INT      no_of_segment[2];
   INT      status = SUCCESS;

/* ------------------ */
/* check on bad scale */
/* ------------------ */
   if (strcmp (scale_uom,"0.00 ") == 0)
     {
     am_message (AM_STATUS,"Invalid scale");
     return FAIL;
     }

    /*******************************/
    /* retrive data from data base */
    /*******************************/

      if (tot_length < 2.0) 
      {
	printf("length too small!");
	return FAIL;
      }

      /**************************************************/
      /*     set initial value for GKS                  */
      /**************************************************/
      

      lu_text_to_index( "COLOR", &color, "BLACK" );
      wm_gsln(1);		/* solid line */

      lu_text_to_index("FONT",&font,"NORMAL");
      lu_text_to_index("TEXT_PRECISION",&precision,"STROKE");
      wm_gstxfp(font,precision);
      wm_gschup(0.0, 1.0);
      wm_gstxp(GRIGHT);
      wm_gstxal(2,3);

      wm_gsplci(color);         /* set line color to black */
      wm_gstxci(color);         /* set test color to black */
      wm_gslwsc(1.0);           /* set line width 1 */
   
      /*******************/
      /* UNIT conversion */
      /*******************/

      status = tu_parse_unit(scale_uom,&scale,meter_mile_unit,cm_in_unit);
      if (status != SUCCESS)
	{
	am_message (AM_STATUS,"Invalid scale");
	return FAIL;
	}
     
      /*********************************************************/

      if (cm_in_unit[0] == 'K')
	{
 	        scale = scale / 100000.0;  /*kilometer -> cm*/
		strcpy(cm_in_unit,"CM");
	}
      
      if (cm_in_unit[0] == 'M')
	{
 	        scale = scale / 100.0;  /*meter -> cm*/
		strcpy(cm_in_unit,"CM");
	}

      
      if (cm_in_unit[0] == 'C')
      { /* CM -> INCH */
	interval_len = scale*2;
        if (meter_mile_unit[0] == 'C')
	{
	     interval_len_cmin[0] = round(interval_len);
	     if(interval_len_cmin[0] >= 100.0)
   	     {
 	        scale = scale / 100.0;  /*cm->m*/
		interval_len = interval_len /100.0;
		interval_len_cmin[0] = interval_len_cmin[0] / 100.0;
        	strcpy(label_uom1,"Meters");
	        strcpy(label_uom2,"Feet");
		interval_len = interval_len * 3;
		interval_len_cmin[1] = round(interval_len);
	     }
	     else
	     {
        	strcpy(label_uom1,"Centimeters");
	        strcpy(label_uom2,"Inches");
		interval_len = interval_len /2.0;
		interval_len_cmin[1] = round(interval_len);
	     }
	}
        else if (meter_mile_unit[0] == 'M')
	{
             interval_len_cmin[0] = round(interval_len);
	     if (interval_len_cmin[0] >= 1000.0)
	     {
 	        scale = scale / 1000.0;  /*meter -> km*/
		interval_len = interval_len / 1000.0;
		interval_len_cmin[0] = interval_len_cmin[0] / 1000.0;
        	strcpy(label_uom1,"Kilometers");
	        strcpy(label_uom2,"Miles");
		interval_len_cmin[1] = interval_len_cmin[0];
             }
	     else
	     {
        	strcpy(label_uom1,"Meters");
	        strcpy(label_uom2,"Feet");
		interval_len = interval_len * 3;
		interval_len_cmin[1] = round(interval_len);
	     }
	}
        else if (meter_mile_unit[0] == 'K')
	{
	        strcpy(label_uom1,"Kilometers");
	        strcpy(label_uom2,"Miles");
		interval_len_cmin[0] = round(interval_len);
		interval_len_cmin[1] = interval_len_cmin[0];
	}
	else
	{
		printf("UNIT too small!");
		return FAIL;
	}
        base_unit =(DOUBLE)interval_len_cmin[1] / (DOUBLE) interval_len_cmin[0];
	status = tu_unit_conversion((DOUBLE)1.0,label_uom2,&con_m_k,label_uom1);
	if (status != SUCCESS)
	  {
	  printf("UNIT conversion fail!");
	  return FAIL;
	  }
	a[0] =  metric_c;
	a[1] =  nmetric_c;

	tu_unit_conversion((DOUBLE)1.0,"IN",&con_i_c,"CM");
	tot_length = tot_length * con_i_c; 
      }
      /*******************************************************************/
      if (cm_in_unit[0] == 'M')
	{
 	        scale = scale / (12.0 * 5280);  /*mile -> in*/
		strcpy(cm_in_unit,"IN");
	}
      
      if (cm_in_unit[0] == 'F')
	{
 	        scale = scale / 12.0;  /*foot -> in*/
		strcpy(cm_in_unit,"IN");
	}

      if (cm_in_unit[0] == 'I')
      { /* INCH -> CM */
        interval_len = scale;
        if (meter_mile_unit[0] == 'I')
	{
	     interval_len_cmin[0] = round(interval_len);
	     if(interval_len_cmin[0] >= 12.0)
   	     {
 	        scale = scale / 12.0;  /*in->foot*/
		interval_len = interval_len_cmin[0] / 12.0;
		interval_len_cmin[0] = round(interval_len);
        	strcpy(label_uom2,"Meters");
	        strcpy(label_uom1,"Feet");
		interval_len /= 3.0;
		interval_len_cmin[1] = round(interval_len);
             }
	     else
	     {
        	strcpy(label_uom2,"Centimeters");
	        strcpy(label_uom1,"Inches");
		interval_len *= 2.0;
		interval_len_cmin[1] = round(interval_len );
	     }
	}
        else if (meter_mile_unit[0] == 'F')
	{
	     interval_len_cmin[0] = round(interval_len);
	     if(interval_len_cmin[0] >= 5280.0)
	     {
 	        scale = scale / 5280.0;  /*feet -> mile*/
		interval_len = interval_len_cmin[0] / 5280.0;
		interval_len_cmin[0] = round(interval_len);
        	strcpy(label_uom2,"Kilometers");
	        strcpy(label_uom1,"Miles");
		interval_len_cmin[1] = interval_len_cmin[0];
             }
	     else
	     {
        	strcpy(label_uom2,"Meters");
	        strcpy(label_uom1,"Feet");
		interval_len /= 3.0;
		interval_len_cmin[1] = round(interval_len );
	     }
	}
        else if (meter_mile_unit[0] == 'M')
	{
	        strcpy(label_uom2,"Kilometers");
	        strcpy(label_uom1,"Miles");
		interval_len_cmin[0] = round(interval_len);
		interval_len_cmin[1] = interval_len_cmin[0];
	}
	else
	{
		printf("UNIT too small!");
		return FAIL;
	}
        base_unit = (DOUBLE)interval_len_cmin[1] / (DOUBLE) interval_len_cmin[0];
	status = tu_unit_conversion((DOUBLE)1.0,label_uom2,&con_m_k,label_uom1);
	if (status != SUCCESS)
	  {
	  printf("UNIT conversion fail!");
	  return FAIL;
	  }
	a[1] = metric_c;
	a[0] = nmetric_c;
      }
      /*******************************************************************/

      /******************************************************/
      /* first scale unit is according to the map base unit */
      /******************************************************/
  
	if(cm_in_unit[0] == 'C')
	{
	     tu_unit_conversion((DOUBLE)1.0,"CM",&con_i_c,"IN");
	     tot_length *= con_i_c; 
	     tot_length -= 1.0 / (tot_length * 2);
             len_per_interval[0] = interval_len_cmin[0] / scale;
	     len_per_interval[0] *= con_i_c;
	     no_of_segment[0] = (INT)((tot_length - 1) / len_per_interval[0]);
       	     tot_len[0] = no_of_segment[0] * len_per_interval[0];
	}
	else
	{
	     tot_length -= 1.0 / (tot_length * 2);
             len_per_interval[0] = interval_len_cmin[0] / scale;
	     no_of_segment[0] = (INT)((tot_length - 1) / len_per_interval[0]);
       	     tot_len[0] = no_of_segment[0] * len_per_interval[0];
	}

      /*****************************************************************/
      /* second scale unit is convert from tu_unit_conversion function */
      /*****************************************************************/

	len_per_interval[1] = len_per_interval[0] * con_m_k * base_unit;
        no_of_segment[1] = (INT)((tot_length - 1) / len_per_interval[1]);
	tot_len[1] = no_of_segment[1] * len_per_interval[1];

      /******************************************************/
      /* caculate the first drawing position of scale bars  */
      /******************************************************/

	first_pos[0] = ((tot_length - 1)/ 2.0) - (tot_len[0] / 2.0) + 1;
	first_pos[1] = ((tot_length - 1)/ 2.0) - (tot_len[1] / 2.0) + 1;

      /******************************************************/
      /* reset the '0' position                             */
      /******************************************************/
      /*	
	if (len_per_interval[0] > len_per_interval[1])
	{
		first_pos[1] = first_pos[0] + len_per_interval[0] 
					    - len_per_interval[1];
		no_of_segment[1] = (tot_len[0] - len_per_interval[0])
				   / len_per_interval[1];
		no_of_segment[1] += 1;
	}
	else
	{
		first_pos[0] = first_pos[1] + len_per_interval[1] 
					    - len_per_interval[0];
		no_of_segment[0] = (tot_len[1] - len_per_interval[1])
				   / len_per_interval[0];
		no_of_segment[0] += 1;
	}
      */
      /******************************************************/
      /* caculate height of scale bar                       */
      /******************************************************/
       
       line1 = height / 4;
       line2 = line1 * 2 ;
       line3 = line1 * 3 ;

      /******************************************************/
      /*   Caculate the positions for drawing               */
      /******************************************************/

       for(i=0 ; i<2; i++)
       {
          if (a[i] == 'Y')
          {
	   if(i == 0)
	   {
		b_line = line2; 
		t_line = line3;
		y_text = line3 + (line1 / 2);
	   }
	   else
	   {
		b_line = line1;
		t_line = line2;
		y_text = line1 / 2;
	   }

	   ary_y[4] = ary_y[1] = ary_y[0] = b_line;
	   ary_y[3] = ary_y[2] = t_line;
	   ary_y_center[0] = ary_y_center[1] = (line1 / 2) + b_line;

      /****************************/
      /* draw the scale bar boxes */
      /****************************/

           flag = TRUE;
	   uu = len_per_interval[i] / 4;
	   ary_x[2] = first_pos[i];
	   for (j = 0; j < 4; j++)    /* draw first box */
	   {
		ary_x[4] = ary_x[3] = ary_x[0] = ary_x[2];
		ary_x[1] = ary_x[2] = ary_x[2] + uu;
		if (flag == TRUE)
		{
			wm_gpl(5,ary_x,ary_y);
			wm_gslwsc(2.0);     /* set line width 2 */
			wm_gpl(2,ary_x,ary_y_center);
			wm_gslwsc(1.0);     /* set line width 1 */
			flag = FALSE;
                }
		else
		{
			wm_gpl(5,ary_x,ary_y);
			flag = TRUE;
                }
	   }
	   sprintf(test,"%d",interval_len_cmin[i]);
           wm_gschh(line1 * 0.4);
	   wm_gtx(first_pos[i],y_text,test); /* put interval on top of bar */
	   flag = TRUE;
	   for (ii = 0; ii < no_of_segment[i] - 1 ; ii++)  
					   /* draw the rest of boxes */
	   {
		sprintf(test,"%d",ii*interval_len_cmin[i]);
		wm_gtx(ary_x[2],y_text,test);  /* put the scale no */
		ary_x[4] = ary_x[3] = ary_x[0] = ary_x[2];
		ary_x[1] = ary_x[2] = ary_x[2] + len_per_interval[i];
		if (flag == TRUE)
		{
			wm_gpl(5,ary_x,ary_y);
			wm_gslwsc(2.0);     /* set line width 2 */
			wm_gpl(2,ary_x,ary_y_center);
			wm_gslwsc(1.0);     /* set line width 1 */
			flag = FALSE;
                }
		else
		{
			wm_gpl(5,ary_x,ary_y);
			flag = TRUE;
                }
            }
	    sprintf(test,"%d",ii*interval_len_cmin[i]);
	    wm_gtx(ary_x[2],y_text,test);  /* put the scale no for last one */
            wm_gschh(line1 * 0.6);
	    if (i == 0)
               wm_gtx(first_pos[0]/2, ary_y_center[0],label_uom1);
					   /* put the scale label in front  */
	    else
               wm_gtx(first_pos[1]/2, ary_y_center[0],label_uom2);
					   /* put the scale label in front  */
         }  /* if command */
       } /* for loop */
     return SUCCESS;
}

publicdef INT round(interval_l) 
DOUBLE interval_l;
{
      /*********************************************************/
      /* Round scale into 1,2 or 5 base unit of interval len */
      /*********************************************************/
INT i = 0;
INT j ;
INT round_val;
      while(interval_l >= 10.0)
      {
	interval_l /= 10;
	i++;
      }
      if (interval_l <= 1.5) round_val = 1;
      else if (interval_l <= 3.5) round_val = 2;
      else if (interval_l <= 7.5) round_val = 5;
      else round_val = 10;
      for (j = 0; j < i; j++)
	round_val *= 10;
      return (round_val);
}
