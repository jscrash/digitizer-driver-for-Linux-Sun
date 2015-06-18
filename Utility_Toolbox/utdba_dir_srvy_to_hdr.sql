rem utdba_dir_srvy_to_hdr.sql

UPDATE WELL_HDR H
SET DRILLERS_TD = (SELECT MAX(MD) FROM WELL_DIR_SRVY_PTS P 
                   WHERE P.UWI = H.UWI), 
TVD = (SELECT TVD FROM WELL_DIR_SRVY_PTS D 
       WHERE D.UWI = H.UWI AND MD = (SELECT MAX(MD) 
                           FROM WELL_DIR_SRVY_PTS A 
                           WHERE A.UWI = H.UWI)), 
BASE_DELTA_X = (SELECT DX FROM WELL_DIR_SRVY_PTS S 
                WHERE S.UWI = H.UWI AND 
                      MD = (SELECT MAX(MD) 
                            FROM WELL_DIR_SRVY_PTS B 
                            WHERE B.UWI = H.UWI)),
BASE_DELTA_Y = (SELECT DY FROM WELL_DIR_SRVY_PTS C 
                WHERE C.UWI = H.UWI AND 
                      MD = (SELECT MAX(MD) 
                            FROM WELL_DIR_SRVY_PTS E 
                            WHERE E.UWI = H.UWI))
WHERE UWI IN (SELECT DISTINCT G.UWI
              FROM WELL_HDR G,WELL_DIR_SRVY_PTS F
              WHERE G.UWI=F.UWI
              AND DRILLERS_TD IS NULL OR 
              DRILLERS_TD < (SELECT MAX(MD) FROM WELL_DIR_SRVY_PTS
                             WHERE UWI = G.UWI));

/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_DIR_SRVY_TO_HDR.SQL*/
/* *2    17-JUL-1991 14:42:22 ALLAWAY "(SPR 8648) initial checkin"*/
/* *1    17-JUL-1991 14:38:16 ALLAWAY "DBA example utility script"*/
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_DIR_SRVY_TO_HDR.SQL*/
