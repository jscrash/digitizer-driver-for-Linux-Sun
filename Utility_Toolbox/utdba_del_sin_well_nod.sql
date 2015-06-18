rem utdba_del_sin_well_nod.sql          
                                            
SET HEAD OFF;
SET FEED OFF;                         
SET LINESIZE 80;                           
SET PAGESIZE 10000;
SPOOL DEL_&1.NOD.SQL;                    
SELECT 'PROMPT Deleting node data for well &1 from table '||TNAME||'...',
'DELETE '||TNAME||' WHERE NODE_ID = (SELECT NODE_ID FROM WELL_HDR',
'WHERE UWI = ''&1'');'
FROM ESI.FINDER_TABLES WHERE TNAME IN(     
SELECT DISTINCT TNAME FROM ESI.FINDER_COLUMNS 
WHERE TNAME NOT LIKE 'LYNX%' AND DOMAIN = '10');
SPOOL OFF;
REM START DEL_&1.NOD.SQL 
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_DEL_SIN_WELL_NOD.SQL*/
/* *2    17-JUL-1991 14:42:18 ALLAWAY "(SPR 8648) initial checkin"*/
/* *1    17-JUL-1991 14:33:48 ALLAWAY "DBA example utility script"*/
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_DEL_SIN_WELL_NOD.SQL*/
