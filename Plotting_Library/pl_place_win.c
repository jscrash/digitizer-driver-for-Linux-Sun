/* DEC/CMS REPLACEMENT HISTORY, Element PL_PLACE_WIN.C */
/* *1    12-OCT-1990 17:46:05 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_PLACE_WIN.C */
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


#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_place_win (PLOT_LAYOUT *pl, PL_BASE_WINDOW *bw)
#else
publicdef INT pl_place_win (pl, bw)
PLOT_LAYOUT *pl;
PL_BASE_WINDOW *bw;
#endif
{
  INT i,j;
  DOUBLE cur_x, cur_y;
/* ----------------------------------------------------------- */
/* Calculate the actual plot height and width of each zone and */
/* also the shrink factor and white space of the zone          */
/* ----------------------------------------------------------- */

  if (pl->orientation == 1)            /* vertical superior */
    {
/* ---------- */
/* below zone */
/* ---------- */
    if (pl->zone[0].nwins > 0)
      {
      pl->zone[0].plot_width = bw->width;

      if (pl->zone[0].plot_width > pl->zone[0].max_width)
        {
        pl->zone[0].shrink_factor = 1.0;
	if (pl->zone[0].nwins > 1)
	  {
          pl->zone[0].white_space = (pl->zone[0].plot_width - pl->zone[0].max_width) / (pl->zone[0].nwins - 1);
	  }
        pl->zone[0].plot_height = pl->zone[0].max_height;
        }
      else
        {
        pl->zone[0].shrink_factor = pl->zone[0].plot_width / pl->zone[0].max_width;
        pl->zone[0].white_space = 0.0;
        pl->zone[0].plot_height = pl->zone[0].max_height * pl->zone[0].shrink_factor;
        }
      }
/* ---------- */
/* above zone */
/* ---------- */
    if (pl->zone[2].nwins > 0)
      {
      pl->zone[2].plot_width = bw->width;
      if (pl->zone[2].plot_width > pl->zone[2].max_width)
        {
        pl->zone[2].shrink_factor = 1.0;
	if (pl->zone[2].nwins > 1)
	  {
          pl->zone[2].white_space = (pl->zone[2].plot_width - pl->zone[2].max_width) / (pl->zone[2].nwins- 1);
	  }
        pl->zone[2].plot_height = pl->zone[2].max_height;
        }
      else
        {
        pl->zone[2].shrink_factor = pl->zone[2].plot_width / pl->zone[2].max_width;
        pl->zone[2].white_space = 0.0;
        pl->zone[2].plot_height = pl->zone[2].max_height * pl->zone[2].shrink_factor;
        }
      }
/* ---------- */
/* right zone */
/* ---------- */
    if (pl->zone[1].nwins > 0)
      {
      pl->zone[1].plot_height = pl->zone[0].plot_height + pl->zone[2].plot_height + bw->height;
      if (pl->zone[1].plot_height > pl->zone[1].max_height)
        {
        pl->zone[1].shrink_factor = 1.0;
	if (pl->zone[1].nwins > 1)
	  {
          pl->zone[1].white_space = (pl->zone[1].plot_height - pl->zone[1].max_height) / (pl->zone[1].nwins - 1);
	  }
        pl->zone[1].plot_width = pl->zone[1].max_width;
        }
      else
        {
        pl->zone[1].shrink_factor = pl->zone[1].plot_height / pl->zone[1].max_height;
        pl->zone[1].white_space = 0.0;
        pl->zone[1].plot_width = pl->zone[1].max_width * pl->zone[1].shrink_factor;
	}
      }

/* --------- */
/* left zone */
/* --------- */
    if (pl->zone[3].nwins > 0)
      {
      pl->zone[3].plot_height = pl->zone[0].plot_height + pl->zone[2].plot_height + bw->height;
      if (pl->zone[3].plot_height > pl->zone[3].max_height)
        {
        pl->zone[3].shrink_factor = 1.0;
	if (pl->zone[3].nwins > 1)
	  {
          pl->zone[3].white_space = (pl->zone[3].plot_height - pl->zone[3].max_height) / (pl->zone[3].nwins - 1);
	  }
        pl->zone[3].plot_width = pl->zone[3].max_width;
        }
      else
        {
        pl->zone[3].shrink_factor = pl->zone[3].plot_height / pl->zone[3].max_height;
        pl->zone[3].white_space = 0.0;
        pl->zone[3].plot_width = pl->zone[3].max_width * pl->zone[3].shrink_factor;
        }
      }
    }
  else                              /* horizontal superior */
    {
/* ---------- */
/* right zone */
/* ---------- */
    if (pl->zone[1].nwins > 0)
      {
      pl->zone[1].plot_height = bw->height;
      if (pl->zone[1].plot_height > pl->zone[1].max_height)
        {
        pl->zone[1].shrink_factor = 1.0;
	if (pl->zone[1].nwins > 1)
	  {
          pl->zone[1].white_space = (pl->zone[1].plot_height - pl->zone[1].max_height) / (pl->zone[1].nwins - 1);
	  }
        pl->zone[1].plot_width = pl->zone[1].max_width;
        }
      else
        {
        pl->zone[1].shrink_factor = pl->zone[1].plot_height / pl->zone[1].max_height;
        pl->zone[1].white_space = 0.0;
        pl->zone[1].plot_width = pl->zone[1].max_width * pl->zone[1].shrink_factor;
	}
      }

/* --------- */
/* left zone */
/* --------- */
    if (pl->zone[3].nwins > 0)
      {
      pl->zone[3].plot_height = bw->height;
      if (pl->zone[3].plot_height > pl->zone[3].max_height)
        {
        pl->zone[3].shrink_factor = 1.0;
	if (pl->zone[3].nwins > 1)
	  {
          pl->zone[3].white_space = (pl->zone[3].plot_height - pl->zone[3].max_height) / (pl->zone[3].nwins - 1);
	  }
        pl->zone[3].plot_width = pl->zone[3].max_width;
        }
      else
        {
        pl->zone[3].shrink_factor = pl->zone[3].plot_height / pl->zone[3].max_height;
        pl->zone[3].white_space = 0.0;
        pl->zone[3].plot_width = pl->zone[3].max_width * pl->zone[3].shrink_factor;
	}
      }

/* ---------- */
/* below zone */
/* ---------- */
    if (pl->zone[0].nwins > 0)
      {
      pl->zone[0].plot_width = pl->zone[1].plot_width + pl->zone[3].plot_width + bw->width;
      if (pl->zone[0].plot_width > pl->zone[0].max_width)
        {
        pl->zone[0].shrink_factor = 1.0;
	if (pl->zone[0].nwins > 1)
	  {
          pl->zone[0].white_space = (pl->zone[0].plot_width - pl->zone[0].max_width) / (pl->zone[0].nwins - 1);
	  }
        pl->zone[0].plot_height = pl->zone[0].max_height;
        }
      else
        {
        pl->zone[0].shrink_factor = pl->zone[0].plot_width / pl->zone[0].max_width;
        pl->zone[0].white_space = 0.0;
        pl->zone[0].plot_height = pl->zone[0].max_height * pl->zone[0].shrink_factor;
	}
      }

/* ---------- */
/* above zone */
/* ---------- */
    if (pl->zone[2].nwins > 0)
      {
      pl->zone[2].plot_width = pl->zone[1].plot_width + pl->zone[3].plot_width + bw->width;
      if (pl->zone[2].plot_width > pl->zone[2].max_width)
        {
        pl->zone[2].shrink_factor = 1.0;
	if (pl->zone[2].nwins > 1)
	  {
          pl->zone[2].white_space = (pl->zone[2].plot_width - pl->zone[2].max_width) / (pl->zone[2].nwins - 1);
	  }
        pl->zone[2].plot_height = pl->zone[2].max_height;
        }
      else
        {
        pl->zone[2].shrink_factor = pl->zone[2].plot_width / pl->zone[2].max_width;
        pl->zone[2].white_space = 0.0;
        pl->zone[2].plot_height = pl->zone[2].max_height * pl->zone[2].shrink_factor;
	}
      }
    }

/* ------------------------------------------------- */
/* Calculate the width and height of the entire plot */
/* ------------------------------------------------- */
  pl->width = pl->zone[1].plot_width + pl->zone[3].plot_width + bw->width + 2 * pl->margin;
  pl->height = pl->zone[0].plot_height + pl->zone[2].plot_height + bw->height + 2 * pl->margin;

/* ---------------------------------------------------------------- */
/* Calculate the position of the base window in the plot coordinate */
/* ---------------------------------------------------------------- */
  bw->x = pl->zone[3].plot_width + pl->margin;
  bw->y = pl->zone[0].plot_height + pl->margin;

/* -------------------------------------------------- */
/* Calculate the position of each window in each zone */
/* and the actual width and height of the window      */
/* -------------------------------------------------- */
  for (i = 0; i < 4; ++i)
    {
    switch (i)
      {
/* ------------------------------------------- */
/* Calculate the starting postion of each zone */
/* ------------------------------------------- */
      case 0:       /* below zone */
	if (pl->orientation == 0)
	  {
	  cur_x = pl->margin;
	  }
        else
	  {
	  cur_x = pl->margin + pl->zone[3].plot_width;
	  }
	cur_y = pl->margin;
	break;

      case 1:      /* right zone */
	cur_x = pl->width - pl->margin - pl->zone[1].plot_width;
	if (pl->orientation == 1)
	  {
	  cur_y = pl->margin;
	  }
        else
	  {
	  cur_y = pl->zone[0].plot_height + pl->margin;
	  }
        break;

      case 2:      /* above zone */
	if (pl->orientation == 0)
	  {
	  cur_x = pl->margin;
	  }
        else
	  {
	  cur_x = pl->margin + pl->zone[3].plot_width;
	  }
        cur_y = pl->height - pl->margin - pl->zone[2].plot_height;
	break;

      case 3:      /* left zone */
	cur_x = pl->margin;
	if (pl->orientation == 1)
	  {
	  cur_y = pl->margin;
	  }
        else
	  {
	  cur_y = pl->zone[0].plot_height + pl->margin;
	  }
        break;
      }
/* ----------------------------------------------------------------- */
/* Calculate the position and actual width and height of each window */
/* ----------------------------------------------------------------- */
    for (j = 0; j < pl->zone[i].nwins; ++j)
      {
      pl->zone[i].win[j].plot_width = pl->zone[i].win[j].max_width * 
					  pl->zone[i].shrink_factor; 
      pl->zone[i].win[j].plot_height = pl->zone[i].win[j].max_height * 
					   pl->zone[i].shrink_factor; 
      pl->zone[i].win[j].plot_margin = pl->zone[i].win[j].margin * 
					   pl->zone[i].shrink_factor; 
      if (pl->zone[i].win[j].plot_width < pl->zone[i].win[j].mini_width ||
          pl->zone[i].win[j].plot_height < pl->zone[i].win[j].mini_height)
        {
	pl->zone[i].win[j].drop = TRUE;
	}
      switch (i)
	{
	case 0:
	case 2:
	  pl->zone[i].win[j].x = cur_x;
	  switch (pl->zone[i].win[j].alignment)
	    {
	    case PL_ABOTTOM:
	      pl->zone[i].win[j].y = cur_y;
	      break;

            case PL_ACENTER:
	      pl->zone[i].win[j].y = cur_y + (pl->zone[i].plot_height -
				     pl->zone[i].win[j].plot_height -
				     2*pl->zone[i].win[j].plot_margin) / 2.0;
              break;

            case PL_ATOP:
	      pl->zone[i].win[j].y = cur_y + (pl->zone[i].plot_height -
				     pl->zone[i].win[j].plot_height - 
				     2*pl->zone[i].win[j].plot_margin);
              break;
            }

	  cur_x += pl->zone[i].win[j].plot_width 
		   + 2*pl->zone[i].win[j].plot_margin
		   + pl->zone[i].white_space;
          break;

        case 1:
	case 3:
	  switch (pl->zone[i].win[j].alignment)
	    {
	    case PL_ALEFT:
	      pl->zone[i].win[j].x = cur_x;
	      break;

            case PL_ACENTER:
	      pl->zone[i].win[j].x = cur_x + (pl->zone[i].plot_width -
				     pl->zone[i].win[j].plot_width - 
				     2*pl->zone[i].win[j].plot_margin) / 2.0;
              break;

            case PL_ARIGHT:
	      pl->zone[i].win[j].x = cur_x + (pl->zone[i].plot_width -
				     pl->zone[i].win[j].plot_width - 
				     2*pl->zone[i].win[j].plot_margin);
              break;
            }

	  pl->zone[i].win[j].y = cur_y;
	  cur_y += pl->zone[i].win[j].plot_height 
		   + 2*pl->zone[i].win[j].plot_margin
		   + pl->zone[i].white_space;
          break;
        }
      }
    }
  return SUCCESS;
}


