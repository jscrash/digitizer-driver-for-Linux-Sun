/* DEC/CMS REPLACEMENT HISTORY, Element WI_COLOR.C */
/* *6    27-JUN-1991 16:01:14 SANTIAGO "(SPR 8438) This file is no longer needed for Finder" */
/* *5    23-OCT-1990 13:54:08 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *4    16-MAR-1990 15:40:51 KEE "(SPR -1) change static -> publicdef of setColorWithName" */
/* *3     2-MAR-1990 11:16:53 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X" */
/* *2     6-FEB-1990 00:13:12 JULIAN "(SPR -1) X Version of WI Module" */
/* *1     5-FEB-1990 23:50:48 JULIAN "X COLOR INITIALIZATION" */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_COLOR.C */
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

/* RJS - 06.27.91

   this file had contained two public functions wiz_color_init and setColorWithName

   the only reference to wiz_color_init that I found in FINDER_SOURCE was in
   xf_initalize.c and those have been removed as of CMS version 14 of that file.

   the only reference to setColorWithName was in wiz_color_init in this same file
   and that was removed. Incidentally it was serious incorrect!

   Therefore this file is no longer needed.
*/
