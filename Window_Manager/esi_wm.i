C  DEC/CMS REPLACEMENT HISTORY, Element ESI_WM.I
C  *1    30-JUN-1989 15:17:35 GILLESPIE ""
C  DEC/CMS REPLACEMENT HISTORY, Element ESI_WM.I
C     
C     $Header: /j/finder/inc/RCS/esi_wm.i,v 1.1 2008/02/09 09:16:37 julian Exp $ esi_wm.i 
C     
      INTEGER WM_CLASS_WINDOW
      INTEGER WM_CLASS_TNR
      INTEGER WM_CLASS_GROUP
      INTEGER WM_CLASS_SEGMENT
      INTEGER WM_CLASS_PICK
      CHARACTER*255 WM_CLSTR_WINDOW
      CHARACTER*255 WM_CLSTR_TNR
      CHARACTER*255 WM_CLSTR_GROUP
      CHARACTER*255 WM_CLSTR_SEGMENT
      CHARACTER*255 WM_CLSTR_PICK
      INTEGER WM_CLASS_BACKGROUND
      INTEGER WM_CLASS_MASK
      INTEGER WM_CLASS_HILIGHT
      CHARACTER*255 WM_CLSTR_BACKGROUND
      CHARACTER*255 WM_CLSTR_MASK
      CHARACTER*255 WM_CLSTR_HILIGHT
      INTEGER WM_CLASS_MAP
      INTEGER WM_CLASS_OVERLAY
      CHARACTER*255 WM_CLSTR_MAP
      CHARACTER*255 WM_CLSTR_OVERLAY
      PARAMETER (WM_CLASS_WINDOW      = 1)
      PARAMETER (WM_CLASS_TNR         = 2)
      PARAMETER (WM_CLASS_GROUP       = 3)
      PARAMETER (WM_CLASS_SEGMENT     = 4)
      PARAMETER (WM_CLASS_PICK        = 5)

      PARAMETER (WM_CLSTR_WINDOW      ='Window')
      PARAMETER (WM_CLSTR_TNR         ='Transformation')
      PARAMETER (WM_CLSTR_GROUP       ='Group')
      PARAMETER (WM_CLSTR_SEGMENT     ='Segment')
      PARAMETER (WM_CLSTR_PICK        ='Pick')

      PARAMETER (WM_CLASS_BACKGROUND  = 101)
      PARAMETER (WM_CLASS_MASK        = 102)
      PARAMETER (WM_CLASS_HILIGHT     = 103)

      PARAMETER (WM_CLSTR_BACKGROUND  ='Background')
      PARAMETER (WM_CLSTR_MASK        ='Mask')
      PARAMETER (WM_CLSTR_HILIGHT     ='Hilight')

      PARAMETER (WM_CLASS_MAP         = 201)
      PARAMETER (WM_CLASS_OVERLAY     = 202)

      PARAMETER (WM_CLSTR_MAP         ='Map')
      PARAMETER (WM_CLSTR_OVERLAY     ='Overlay')

