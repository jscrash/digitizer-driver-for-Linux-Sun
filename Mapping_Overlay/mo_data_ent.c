/* DEC/CMS REPLACEMENT HISTORY, Element MO_DATA_ENT.C*/
/* *7    15-AUG-1991 17:01:34 JTM "(SPR 0) Changed argument to wm_gslwsc() to DOUBLE."*/
/* *6     6-NOV-1989 11:26:05 JULIAN "(SPR 30) add mg_message"*/
/* *5     3-NOV-1989 12:21:28 GILLESPIE "(SPR 1) Changes to keep up with mo_start resource"*/
/* *4    11-AUG-1989 12:50:01 JULIAN "(SPR -1) function mo_draw_vels_trat has return(e) or return;"*/
/* *3    28-JUL-1989 11:24:17 GILLESPIE "(SPR 1) fix minor casting problems"*/
/* *2    13-JUL-1989 13:58:21 GORDON "(SPR 0) add call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 13:09:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MO_DATA_ENT.C*/


/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
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

#include "esi_mo.h"
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_UI_H
#include "esi_ui.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#include "mo_strat.rh"
#include "esi_mg.h"
#include "esi_mo_msg.h"

#if USE_PROTOTYPES

static INT mo_de_zoom(STRAT_MODEL_STRUCT *pModel, NLIST_HEADER nlist, 
                      MO_DATA_STRUCT *pcurrent);
static INT mo_draw_model(STRAT_MODEL_STRUCT *pModel,NLIST_HEADER nlist,
                         MO_DATA_STRUCT *pcurrent);
static INT mo_draw_vels_pt(MO_DATA_STRUCT *pcurrent,INT color);
static INT mo_draw_vels_trat(STRAT_MODEL_STRUCT *pModel,NLIST_HEADER nlist);
publicdef  INT mo_draw_x(DOUBLE x,DOUBLE y,INT color);
static INT mo_trat_to_world(STRAT_MODEL_STRUCT *pModel,
	NLIST_HEADER from, NLIST_HEADER to);
static INT mo_world_to_trat(STRAT_MODEL_STRUCT *pModel,
	NLIST_HEADER from, NLIST_HEADER to);

#else

static INT mo_de_zoom();
static INT mo_draw_model();
static INT mo_draw_vels_pt();
static INT mo_draw_vels_trat();
publicdef  INT mo_draw_x();
static INT mo_trat_to_world();
static INT mo_world_to_trat();

#endif

#define VPL_WIDTH 4
static VOIDPTR value_list[4];
static UINT DimList[] = {1, 2, 3, 4};
static VOIDPTR ValueList[VPL_WIDTH];
static INT VUnits[] = {0, 0, 0, 0};
static INT VDataTypes[VPL_WIDTH]={FLOAT_TYPE,FLOAT_TYPE,FLOAT_TYPE,FLOAT_TYPE};
static size_t  VWidths[VPL_WIDTH]    = {sizeof(FLOAT),sizeof(FLOAT),sizeof(FLOAT),
                                        sizeof(FLOAT)};
static INT VIncs[VPL_WIDTH]      = {VALUE_LIST,VALUE_LIST,VALUE_LIST,VALUE_LIST};

/* ************************************************************************** */
/*                                                                            */
/*     Routine:   MO_DATA_ENTRY                                               */
/*							                      */
/*     Function:  Enter the velocity and density points for the empirical     */
/*                model.                                                      */
/* ************************************************************************** */

publicdef INT mo_data_entry(pModel)
STRAT_MODEL_STRUCT *pModel;
{
    INT            cdlgid,cditem,cdvalue;
    UINT           ngot;
    NLIST_HEADER   temp;
    CHAR           text[STMAX], *ptext;
    BOOL           not_finished, data_is_bad;
    BOOL           vel_is_bad,den_is_bad,error;
    FLOAT          xworld,yworld;
    INT            button;
    FLOAT          x,y,z;
    FLOAT          vel,den;
    FLOAT          max_vel, min_vel;             /* velocity max,min allowed  */
    INT            npts;
    INT            status;
    UINT           point,stroke;
    INT            pickon;
    MO_DATA_STRUCT current;
    FLOAT          xsave,ysave;
    INT		   nl_flag;
/* ************************************************************************** */

/* initialize temp point list */

    nl_init_nlist(&temp,VPL_WIDTH,VUnits,VWidths,VDataTypes,VIncs,0);
    mog_reset_text();
    
/* convert the pointlist to our own trat special */
/* this makes it much easier to get uniform text and tolerances */

    mo_world_to_trat(pModel,pModel->velocity.nlist,temp);

/* Set the max, min velocity range according to whether vels are in metric or
       english units                                                         */

    if ( strcmp(pModel->model_uom, "METRES") == 0 || 
         strcmp(pModel->model_uom, "METERS") == 0)
      {
       max_vel = 8000.0;
       min_vel = 1000.0;
      }
    else
      {
       max_vel = 30000.0;
       min_vel = 4000.0; 
      }

/* set pick tolerance */
    x=.1;
    y=.1;
    value_list[0] = (VOIDPTR)&x;
    value_list[1] = (VOIDPTR)&y;
    nl_set_nlist_info(temp, NL_NLIST_FIND_TOLERANCE, 2, DimList, value_list);

/* define and open dialog box */

    am_define_resource(AM_DIALOG, "mo_strat",
                       MO_DATA_ENTER, 0, &cdlgid, 0);
    strcpy(text,"");
    wiscdt(cdlgid,MO_DE_VELOCITY,text);
    wiscdt(cdlgid,MO_DE_DENSITY,text);
    wiopcd(cdlgid);

/* get started */

   wmwfront(pModel->graphics.window_id);
   wmwopen(pModel->graphics.window_id);
   if(pModel->corr_group != 0)
       {
       wmgvis(pModel->corr_group,GINVIS);
       wm_grsgwk(MAIN_WKID);
       }
   else
       {
       wmwvis(pModel->graphics.window_id,GVISI);
      }
   wmtselect(pModel->graphics.transform_trat);

/* loop on button picks until user hits done or cancel */

    current.point = 0;
    value_list[0] = (VOIDPTR) &x;
    value_list[1] = (VOIDPTR) &y;
    value_list[2] = (VOIDPTR) &vel;
    value_list[3] = (VOIDPTR) &den;

    mo_draw_model(pModel,temp,&current);
    not_finished = TRUE;
    error=FALSE;

    while(not_finished)
        {
        if(NOT error)
            {
            wiscdt(cdlgid,MO_DE_MESSAGE,mg_message(MO_SELECT_DATA));
            }

        wmtselect(pModel->graphics.transform_trat);
        if (ug_if_gpx())
	    wm_set_mouse_menu( (CHAR**)0, (INT*)0 );  /* use default menu */      
	wmtlocate(&status,&x,&y,&button);            /* get button info       */

/* cancel */

        if(status EQUALS FAIL)
            {
            nl_clear_nlist(temp);
            if(pModel->corr_group != 0)
                {
                wmgvis(pModel->corr_group,GINVIS);
                wm_grsgwk(MAIN_WKID);
                }
            else
                {
                wmwvis(pModel->graphics.window_id,GVISI);
                }
            mo_draw_vels(pModel);
            wmtselect(pModel->graphics.transform_id);
            wm_gsplci(pModel->line.color);
            nl_draw_nlist(pModel->model.nlist, 1, 2);
            break;
            }

        switch (button)
            {

            case 1:          /* add point */

/* is point in range of model */

            error=FALSE;
            xworld = (x/TRATX) *
                 ( pModel->graphics.world.xmax - pModel->graphics.world.xmin)
                 + pModel ->graphics.world.xmin;
            yworld = (y/TRATY) *
                 ( pModel->graphics.world.ymax - pModel->graphics.world.ymin)
                 + pModel ->graphics.world.ymin;

            if (yworld > pModel->log_start - MINFROMTOP)
                {
                wibeep(1);
                continue;
                }
            if ( yworld < pModel->nl_user_struct.model_ymin)
                {
                 wibeep(1);
                 continue;
                }
            if (xworld < pModel->well.xmin)
                {
                wibeep(1);
                continue;
                }
            else if (xworld > pModel->well.xmax)
                {
                wibeep(1);
                continue;
                }
            wiscdt(cdlgid,MO_DE_MESSAGE,mg_message(MO_VELO_DENSITY));

/* unhighlight and remove reference to current point */

            if(current.point)
                {
                mo_draw_vels_pt(&current,pModel->hvels_color);
                current.point = 0;
                }

/* make point on screen */

            mo_draw_x(x,y,pModel->line.color+1);
            data_is_bad = TRUE;
            while (data_is_bad)
                {

/* let them hit a button */

                wirqcdi(&cdlgid,&cditem,&cdvalue,text);

/* if cancel undo mark and get next pick */

                if(cditem EQUALS MO_DE_CANCEL)
                    {
                    mo_draw_x(x,y,pModel->background_color);
                    break;
                    }

/* check validity of numbers, velocity first */

                wiqccdi(cdlgid,MO_DE_VELOCITY,&cdvalue,text);
                ptext = ts_snowhite(text);
                if(ts_isfloat(ptext) <= 0)
                    {
                    if(strlen(ptext))
                        {
                        wialert(1,"MO_DATA_ENTRY",mg_message(MO_BAD_VELOCITY),
                                  mg_message(MO_VAL_NOT_NUMERIC));
                        vel_is_bad = TRUE;
                        }
                    else
                        {
                        vel=0.0;
                        vel_is_bad = FALSE;
                        }
                    }
                else
                    {
                    sscanf(ptext,"%f",&vel);
                    if(ui_validate_float(vel,min_vel, max_vel,mg_message(MO_VELO_LIMITS),
                       " ")  EQUALS SUCCESS)
                        {
                        vel_is_bad=FALSE;
                        }
                    else
                        {
                        vel_is_bad=TRUE;
                        }
                    }

/* now density */

                wiqccdi(cdlgid,MO_DE_DENSITY,&cdvalue,text);
                ptext = ts_snowhite(text);
                if(ts_isfloat(ptext) <= 0)
                    {
                    if(strlen(ptext))
                        {
                        wialert(1,"MO_DATA_ENTRY",mg_message(MO_BAD_DENSITY),
				mg_message(MO_VAL_NOT_NUMERIC));
                        den_is_bad = TRUE;
                        }
                    else
                        {
                        den=0.0;
                        den_is_bad = FALSE;
                        }
                    }
                else
                    {
                    sscanf(ptext,"%f",&den);
                    if(ui_validate_float(den,1.0,5.0,
                     mg_message(MO_RANGE_DENSITY)," ") EQUALS SUCCESS)
                        {
                        den_is_bad=FALSE;
                        }
                    else
                        {
                        den_is_bad=TRUE;
                        }
                    }
                data_is_bad = vel_is_bad OR den_is_bad;
                }

/* hit cancel */

            if(data_is_bad)
                {
                break;
                }

/* hit ok, add point */
            mo_draw_x(x,y,pModel->background_color);
            nl_inq_nlist_int(temp,NL_NLIST_NPOINTS,&npts);
            nl_set_current_point(temp,npts,1);
            nl_add_points(temp,1,VPL_WIDTH,DimList,value_list);
            current.x=x;
            current.y=y;
            current.vel=vel;
            current.den=den;
            current.point = TRUE;
            mo_draw_vels_pt(&current,pModel->hvels_color);
            current.point = FALSE;
            break;

/* pick point */

        case 2:

        nl_inq_nlist_int(temp,NL_NLIST_NPOINTS,&npts);
        pickon = FALSE;
        if(npts != 0)
            {
	    nl_set_current_point(temp,1,1);
	    ValueList[0] = (VOIDPTR)&x;
	    ValueList[1] = (VOIDPTR)&y;
	    nl_flag=nl_find_point( temp, 2, DimList, (FLOAT **)ValueList);
	    pickon = NOT (nl_flag & NL_EOL);
	    nl_inq_current_point( temp, &point, &stroke);
            }
        if(NOT pickon)
            {
            wiscdt(cdlgid,MO_DE_MESSAGE,"No point found.");
            error = TRUE;
            }
        else
            {
            mo_draw_vels_pt(&current,pModel->hvels_color);
            current.point=point;
            nl_get_points(temp,1,VPL_WIDTH,DimList,value_list,&ngot);
            current.vel = vel;
            current.den = den;
            current.x=x;
            current.y=y;
            mo_draw_vels_pt(&current,pModel->hvels_cur_color);
            if(current.vel != 0.0)
                sprintf(text,"%4.0f",current.vel);
            else
                strcpy(text,"");
            wiscdt(cdlgid,MO_DE_VELOCITY,text);
            if(current.den != 0.0)
                sprintf(text,"%4.2f",current.den);
            else
                strcpy(text,"");
            wiscdt(cdlgid,MO_DE_DENSITY,text);
            error = FALSE;
            }
        break;

        case 3:     /*edit point */

        if(NOT current.point)
            {
            error=TRUE;
            wiscdt(cdlgid,MO_DE_MESSAGE,mg_message(MO_NO_POINT));
            break;
            }
        if(current.vel != 0.0)
            sprintf(text,"%4.0f",current.vel);
        else
            strcpy(text,"");
        wiscdt(cdlgid,MO_DE_VELOCITY,text);
        if(current.den != 0.0)
            sprintf(text,"%4.2f",current.den);
        else
            strcpy(text,"");
        wiscdt(cdlgid,MO_DE_DENSITY,text);
        wiscdt(cdlgid,MO_DE_MESSAGE,mg_message(MO_NEW_VELO_DENSITY));
        data_is_bad = TRUE;
            while (data_is_bad)
                {

/* let them hit a button */

                wirqcdi(&cdlgid,&cditem,&cdvalue,text);

/* if cancel undo mark and get next pick */

                if(cditem EQUALS MO_DE_CANCEL)
                    {
                    mo_draw_x(x,y,pModel->background_color);
                    break;
                    }

/* check validity of numbers, velocity first */

                wiqccdi(cdlgid,MO_DE_VELOCITY,&cdvalue,text);
                ptext = ts_snowhite(text);
                if(ts_isfloat(ptext) <= 0)
                    {
                    if(strlen(ptext))
                        {
                        wialert(1,"MO_DATA_ENTRY",mg_message(MO_BAD_VELOCITY),
				    mg_message(MO_VAL_NOT_NUMERIC));
                        vel_is_bad = TRUE;
                        }
                    else
                        {
                        vel=0.0;
                        vel_is_bad = FALSE;
                        }
                    }
                else
                    {
                    sscanf(ptext,"%f",&vel);
                    if(ui_validate_float(vel,min_vel, max_vel,
		       mg_message(MO_VELO_LIMITS),
                       " ")  EQUALS SUCCESS)
                        {
                        vel_is_bad=FALSE;
                        }
                    else
                        {
                        vel_is_bad=TRUE;
                        }
                    }

/* now density */

                wiqccdi(cdlgid,MO_DE_DENSITY,&cdvalue,text);
                ptext = ts_snowhite(text);
                if(ts_isfloat(ptext) <= 0)
                    {
                    if(strlen(ptext))
                        {
                        wialert(1,"MO_DATA_ENTRY",mg_message(MO_BAD_DENSITY),
				mg_message(MO_VAL_NOT_NUMERIC));
                        den_is_bad = TRUE;
                        }
                    else
                        {
                        den=0.0;
                        den_is_bad = FALSE;
                        }
                    }
                else
                    {
                    sscanf(ptext,"%f",&den);
                    if (ui_validate_float(den,1.0,5.0,
                     mg_message(MO_RANGE_DENSITY)," ") EQUALS SUCCESS)
                        {
                        den_is_bad=FALSE;
                        }
                    else
                        {
                        den_is_bad=TRUE;
                        }
                    }
                data_is_bad = vel_is_bad OR den_is_bad;
                }

/* hit cancel */

            if(data_is_bad)
                {
                break;
                }
            mo_draw_vels_pt(&current,pModel->background_color);
            current.vel=vel;
            current.den=den;
            mo_draw_vels_pt(&current,pModel->hvels_cur_color);
            nl_set_current_point(temp,current.point,1);
            nl_get_points(temp,1,4,DimList,value_list,&ngot);
            vel =current.vel;
            den =current.den;
            nl_replace_points(temp,1,4,DimList,value_list);
            break;

        case 4:      /* delete point */
        if(NOT current.point)
            {
            error=TRUE;
            wiscdt(cdlgid,MO_DE_MESSAGE,"No point picked.");
            break;
            }
        mo_draw_vels_pt(&current,pModel->background_color);
        nl_set_current_point(temp,current.point,1);
        nl_delete_points(temp,1);
        current.point=0;
        break;

        case 5:          /* move point */

        if(NOT current.point)
            {
            error=TRUE;
            wiscdt(cdlgid,MO_DE_MESSAGE,mg_message(MO_NO_POINT));
            break;
            }

/* is point in range of model */

            error=FALSE;
            xworld = (x/TRATX) *
                 ( pModel->graphics.world.xmax - pModel->graphics.world.xmin)
                 + pModel ->graphics.world.xmin;
            yworld = (y/TRATY) *
                 ( pModel->graphics.world.ymax - pModel->graphics.world.ymin)
                 + pModel ->graphics.world.ymin;

            if (yworld > pModel->log_start - MINFROMTOP)
                {
                wibeep(1);
                continue;
                }
            if (xworld < pModel->well.xmin)
                {
                wibeep(1);
                continue;
                }
            else if (xworld > pModel->well.xmax)
                {
                wibeep(1);
                continue;
                }
            mo_draw_vels_pt(&current,pModel->background_color);
            nl_set_current_point(temp,current.point,1);
            vel=current.vel;
            den=current.den;
            nl_replace_points(temp,1,4,DimList,value_list);
            current.x=x;
            current.y=y;
            mo_draw_vels_pt(&current,pModel->hvels_cur_color);
            break;

        case 7:      /* draw model */
        mo_draw_model(pModel,temp,&current);
        break;

        case 8:      /* zoom in */
        mo_de_zoom(pModel,temp,&current);
        break;

        case WM_BUTTON_DONE:
        not_finished = FALSE;
        break;
        }
    }
    if(status EQUALS SUCCESS)
        {
        mo_trat_to_world(pModel,temp,pModel->velocity.nlist);
        wienmni(pModel->main_menu_id,MO_DRAW_TIEPLOT,OFF);
        pModel->mo_tieplot_onoff=OFF;
        wienmni(pModel->main_menu_id,GENERATE_TRACES,OFF);
        pModel->generate_traces_onoff=OFF;
        wienmni(pModel->main_menu_id,MO_TRACES,OFF);
        pModel->mo_traces_onoff=OFF;
        status = SUCCESS;
        }
    else
        {
        status = FAIL;
        }
    nl_clear_nlist(temp);

    am_release_resource(cdlgid);                  /* close dialog             */

    wienmni(pModel->main_menu_id,GENERATE_MODEL,pModel->generate_model_onoff);
    wienmni(pModel->main_menu_id,MO_DIGITIZE,pModel->mo_digitize_onoff);

    wiopcd(pModel->main_dialog_id);               /* open main dialog         */
    wmwclose(pModel->graphics.window_id);

    return status;
}




/* ************************************************************************** */
/* data entry model redraw */

static INT mo_draw_model(pModel,nlist,pcurrent)

  STRAT_MODEL_STRUCT *pModel;
  NLIST_HEADER       nlist;
  MO_DATA_STRUCT     *pcurrent;
{
    wmtselect(pModel->graphics.transform_id);
    wm_gsplci(pModel->line.color);
    nl_draw_nlist(pModel->model.nlist,1,2);
    mo_mark_layer_ends(pModel, pModel->model.nlist);


    if(pModel->model_type EQUALS PURE_EMPIRICAL)
        {
        mo_draw_vels_trat(pModel, nlist);
        mo_draw_vels_pt(pcurrent,pModel->hvels_cur_color);
        }

    return SUCCESS;
}




/* ************************************************************* */
/* data entry model zoom */

static INT mo_de_zoom(pModel,nlist,pcurrent)
  
  NLIST_HEADER       nlist;
  STRAT_MODEL_STRUCT *pModel;
  MO_DATA_STRUCT     *pcurrent;
{
    am_message(AM_DIALOG,mg_message(MO_DIGIT_TO_ZOOM));
    wmwclose(pModel->graphics.window_id);
    wmwzoom(pModel->graphics.window_id,TRUE);
    wm_grsgwk(MAIN_WKID);
    wmwopen(pModel->graphics.window_id);
    
    mo_draw_model(pModel,nlist,pcurrent);
    am_message(AM_DIALOG,"");

    return SUCCESS;
}


/* **************************************************************************** */
/* change world pointlist to trat pointlist */

static INT mo_world_to_trat(pModel,from,to)
  STRAT_MODEL_STRUCT *pModel;
  NLIST_HEADER       from,to;
{
      UINT    npts;
      INT     i;
      UINT    stroke;
      FLOAT   x,y;
      FLOAT   den,vel;

      nl_inq_nlist_int(to,NL_NLIST_NPOINTS,(INT *)&npts);
      nl_inq_nlist_int(to,NL_NLIST_NSTROKES,(INT *)&stroke);
      if(npts !=0 OR stroke != 0)
         {
         nl_clear_nlist(to);
         }
      nl_start_stroke(to,&stroke);
      nl_inq_nlist_int(from,NL_NLIST_NPOINTS,(INT *)&npts);
      value_list[0]=(VOIDPTR)&x;
      value_list[1]=(VOIDPTR)&y;
      value_list[2]=(VOIDPTR)&vel;
      value_list[3]=(VOIDPTR)&den;
      nl_first_point(from,4,DimList,value_list);

      for(i=0 ; i<npts ; i++)
          {
          x =(x-pModel->graphics.world.xmin) /
          (pModel->graphics.world.xmax-pModel->graphics.world.xmin)* TRATX;
          y =(y-pModel->graphics.world.ymin) /
          (pModel->graphics.world.ymax-pModel->graphics.world.ymin)* TRATY;
          nl_add_points(to, 1, 4, DimList, value_list);
          nl_next_point(from, 4, DimList, value_list);
          }
       return SUCCESS;

}



/* **************************************************************************** */
/* change trat pointlist to world pointlist */

static INT mo_trat_to_world(pModel,from,to)

  STRAT_MODEL_STRUCT *pModel;
  NLIST_HEADER from,to;
{
      INT     i;
      UINT    npts,stroke;
      FLOAT   x, y;
      FLOAT   den,vel;

      nl_inq_nlist_int(to, NL_NLIST_NPOINTS, (INT *)&npts);
      nl_inq_nlist_int(to,NL_NLIST_NSTROKES, (INT *)&stroke);
      if(npts !=0 OR stroke != 0)
          {
          nl_clear_nlist(to);
          }
      nl_start_stroke(to,&stroke);
      nl_inq_nlist_int(from, NL_NLIST_NPOINTS, (INT *)&npts);
      value_list[0]=(VOIDPTR) &x;
      value_list[1]=(VOIDPTR)&y;
      value_list[2]=(VOIDPTR)&vel;
      value_list[3]=(VOIDPTR)&den;
      nl_first_point(from,4,DimList,value_list);
      for(i=0; i<npts ; i++)
          {
          x = (x / TRATX) *
              (pModel->graphics.world.xmax-pModel->graphics.world.xmin)
              + pModel->graphics.world.xmin;
          y = (y / TRATY) *
              (pModel->graphics.world.ymax-pModel->graphics.world.ymin)
              + pModel->graphics.world.ymin;
          nl_add_points(to, 1, 4, DimList, value_list);
          nl_next_point(from, 4, DimList,value_list);
          }

      return SUCCESS;
}


/* **************************************************************************** */

publicdef INT mo_draw_vels(pModel)

  STRAT_MODEL_STRUCT *pModel;
{
    NLIST_HEADER   temp;
    INT            stroke;

/* initialize temp point list */

    nl_init_nlist(&temp,VPL_WIDTH,VUnits,VWidths,VDataTypes,VIncs,0);
    mo_world_to_trat(pModel,pModel->velocity.nlist,temp);
    mo_draw_vels_trat(pModel,temp);
    wm_update();
    nl_free_nlist(temp);

    return SUCCESS;
}

/* **************************************************************************** */
/* draw the vel point list */

static INT mo_draw_vels_trat(pModel,nlist)

  STRAT_MODEL_STRUCT *pModel;
  NLIST_HEADER       nlist;
{
    INT            npts;
    INT            i;
    MO_DATA_STRUCT current;
    VOIDPTR        vl[VPL_WIDTH];

    nl_inq_nlist_int(nlist,NL_NLIST_NPOINTS,&npts);
    if(npts EQUALS 0)return SUCCESS;

    vl[0]=(VOIDPTR)&current.x;
    vl[1]=(VOIDPTR)&current.y;
    vl[2]=(VOIDPTR)&current.vel;
    vl[3]=(VOIDPTR)&current.den;

    wmtselect(pModel->graphics.transform_trat);
    nl_first_point(nlist, VPL_WIDTH, DimList, vl);
    current.point = 1;
    mo_draw_vels_pt(&current, pModel->hvels_color);

    npts--;
    for(i=0; i<npts;i++)
        {
        nl_next_point(nlist, VPL_WIDTH, DimList, vl);
        mo_draw_vels_pt(&current, pModel->hvels_color);
        }

    return SUCCESS;

}


/* **************************************************************************** */
/* DRAW EACH POINT */

static INT mo_draw_vels_pt(pcurrent,color)

  MO_DATA_STRUCT *pcurrent;
  INT            color;
{
    CHAR     vels[32],dens[32],text[65];
    FLOAT    x,y;
    
    if(NOT pcurrent->point) return FAIL;
    mo_draw_x(pcurrent->x,pcurrent->y,color);
    if(pcurrent->vel EQUALS 0.0)
        {
        strcpy(vels,"NA/");
        }
    else
        {
        sprintf(vels,"%4.0f/",pcurrent->vel);
        }

    if(pcurrent->den EQUALS 0.0)
        {
        strcpy(dens,"NA");
        }
    else
        {
        sprintf(dens,"%4.2f",pcurrent->den);
        }
    strcpy(text,vels);
    strcat(text,dens);
    wm_gstxci(color);
    x = 0.07;
    wm_gschh(x);
    x = pcurrent->x + .05;
    y = pcurrent->y - .05;
    wm_gtx(x,y,text);

    return SUCCESS;

}


/* **************************************************************************** */
/* DRAW X */

publicdef INT mo_draw_x(x,y,color)

DOUBLE  x,y;
INT     color;
{
      DOUBLE one=1.0;
      FLOAT xbuf[2],ybuf[2];

      wm_gslwsc(one);
      xbuf[0]=x-.03;
      xbuf[1]=x+.03;
      ybuf[0]=y-.03;
      ybuf[1]=y+.03;
      wm_gsplci(color);
      wm_gpl(2,xbuf,ybuf);
      ybuf[0]=y+.03;
      ybuf[1]=y-.03;
      wm_gpl(2,xbuf,ybuf);
}
/* END-CODE */
