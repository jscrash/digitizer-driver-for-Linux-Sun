/* DEC/CMS REPLACEMENT HISTORY, Element PROG_DB.SQL */
/* *1    13-NOV-1990 11:25:06 VINCE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element PROG_DB.SQL */
/* DEC/CMS REPLACEMENT HISTORY, Element PROG_DB.SQL */
/* *1     6-MAR-1990 08:39:36 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element PROG_DB.SQL */
column atable format a27 heading 'Table Name';
column acolumn format a23 heading 'Column Name';
column amodule format a25 heading 'Program Name';
btitle 'S = Select; I = Insert; D = Delete; W = Where; U = Update';
ttitle 'Finder Database / Program Interface|Sorted by Program';
set echo off;
set pagesize 60;
set linesize 80;
set newpage 0;
break on amodule skip 1 on atable;
spool prog_db

select amodule, atable, acolumn, aaction
from program_db_xref
order by amodule, atable, acolumn, aaction;

ttitle 'Finder Database / Program Interface|Sorted by Table';
break on atable skip 1 on acolumn;

select atable, acolumn, amodule, aaction
from program_db_xref
order by atable, acolumn, amodule, aaction;

spool off;
