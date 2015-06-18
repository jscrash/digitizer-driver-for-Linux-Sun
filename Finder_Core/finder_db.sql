/* DEC/CMS REPLACEMENT HISTORY, Element FINDER_DB.SQL */
/* *5     4-DEC-1990 12:44:21 VINCE "(SPR 1) added set feedback off to suppress the n records selected message" */
/* *4     4-DEC-1990 11:55:35 VINCE "(SPR 1) added env. to finder_logical_names report and modified t/btitle formating" */
/* *3    12-AUG-1989 13:58:52 GILLESPIE "(SPR 3) Fix  up the formatting" */
/* *2    31-JUL-1989 17:05:02 GILLESPIE "(SPR 33) Changes for V6 Oracle" */
/* *1    19-JUN-1989 14:53:31 SYSTEM "" */
/* DEC/CMS REPLACEMENT HISTORY, Element FINDER_DB.SQL */

/*	This script prints out the current Finder Database 'Schema'      */

set feedback  off
set termout   off
set linesize  132
set pagesize   60
set newpage     0
set pause     off
set echo      off
set sqlprompt  ''
set docu      off
set trimout    on
set verify    off

COL TNAME        FORMAT A30    HEADING 'Table Name';
COL CNAME        FORMAT A26    HEADING 'Column name';
COL DOMAIN       FORMAT 999    HEADING 'DM';
COL DOMAIN_CODE  FORMAT 999999 HEADING 'Domain|Code';
COL COLNUM       FORMAT 99     HEADING "#";
COL DEFAULT_NAME FORMAT A20    HEADING 'Default Name';
COL SCOPE        FORMAT A7     HEADING 'Scope';
COL PRODUCT      FORMAT A9     HEADING 'Product';
COL CTYPE        FORMAT A12    HEADING 'Column Type';
COL ENVIRONMENT  FORMAT A8     HEADING 'Env.';

COL DEFAULT_DIRECTORY HEADING 'Default Directory';
COL NAME              HEADING 'Name';
COL INAME             HEADING 'Index Name';
COL SCALE             HEADING 'Scale';
COL WIDTH             HEADING 'Width';

btitle skip 1 left   'FINDER Graphics Systems, Inc.' -
              center 'Company Confidential' -
              right  'page ' format 999 sql.pno

ttitle center 'Finder Database Tables' skip 1 -
       center 'Sorted by Table' skip 2

col description format a82 wrap word heading 'Description'

SELECT tname, scope, product, description 
FROM esi.finder_tables
ORDER BY tname; 

ttitle center 'Finder Database Tables' skip 1 -
       center 'Sorted by Scope and Table' skip 2

break on scope skip 1
SELECT scope, tname, product, description
FROM esi.finder_tables
ORDER BY scope, tname;

ttitle center 'Finder Database Tables' skip 1 -
       center 'Sorted by Product and Table' skip 2

break on product skip 1
SELECT product, tname, scope, description
FROM esi.finder_tables
ORDER BY product, tname;

ttitle center 'Finder Database Tables and Columns' skip 1 -
       center 'Sorted by Scope and Table' skip 2

col description format a47 wrap word
col tname format a23
col u format a1 heading ' '
col primary_key format a1 heading ' '

btitle 	skip 1 -
left   "* = Unique Idx Not Null; + = Non-Unique Idx Not Null; - = Not Null;"-
right "Finder Graphics Systems, Inc." skip 1 -
left   "x = Unique Idx;          l = Non-Unique Idx;          @ = Primary Key"-
right "Company Confidential  page" format 999 sql.pno

break on scope page on tname skip 1
SELECT scope, C.tname, colnum, C.cname, 
     decode(U.index_type,'UNIQUE',
		decode(NULLS,'NOT NULL','*','x'), 
		         'NONUNIQUE',
		decode(NULLS,'NOT NULL','+','|'), 
		decode(NULLS,'NOT NULL','-',NULL)) U,
     decode(primary_key, NULL,' ','@') primary_key,
     ctype||
     decode(ctype,'CHAR','('||width||')',
                  'NUMBER',decode(width,NULL,NULL,'('||width||
                               decode(scale,NULL,NULL,','||scale)
                            ||')')
           ) ctype,
        domain, C.description
FROM esi.finder_columns C, esi.finder_tables T, esi.finder_column_indexes U
WHERE C.tname=T.tname
and C.tname=U.tname(+) and C.cname=U.cname(+)
order by scope, C.tname, colnum;

ttitle center 'Finder Database Storage Defaults' skip 1 -
       center 'Sorted by Object Name' skip 2

btitle skip 1 left   'FINDER Graphics Systems, Inc.' -
              center 'Company Confidential' -
              right  'page ' format 999 sql.pno

col oname format a30 heading 'Object Name';
column initial_alloc format 999,999 heading 'Initial|Alloc';
column next_alloc format 999,999 heading 'Next|Alloc';
column minextents_alloc format 999,999 heading 'Min Ext|Alloc';
column maxextents_alloc format 999,999 heading 'Max Ext|Alloc';
column pctincrease_alloc format 99 heading 'Pct Inc|Alloc';
column pct_free format 99 heading 'Pct|Free';
column ini_trans format 9999 heading 'Init|Trans';
column max_trans format 9999 heading 'Max|Trans';
clear break;
break on scope skip 2 on tname skip 1

select oname,initial_alloc,next_alloc,minextents_alloc,maxextents_alloc,
	pctincrease_alloc,pct_free, ini_trans, max_trans
from esi.finder_table_space
order by oname;

ttitle center 'Finder Database Indexes' skip 1 -
       center 'Sorted by Scope' skip 2

btitle skip 1 left   'FINDER Graphics Systems, Inc.' -
              center 'Company Confidential' -
              right  'page ' format 999 sql.pno

COL TNAME FORMAT A27
clear break;
break on scope skip 2 on tname skip 1 on iname
col index_type heading 'Index|Type'
col seq format 999
select scope, a.tname, iname, cname, index_type, seq
from esi.finder_column_indexes a, esi.finder_tables b
where a.tname=b.tname
order by scope, a.tname, iname, seq;

ttitle center 'Finder Database Views' skip 1 -
       center 'Sorted by Scope' skip 2

clear break;
break on scope skip 2;
col view_text format a90 wrap word;
set long 1000;
select scope, view_name, view_text
from esi.finder_views
order by scope, view_name;

ttitle center 'Finder Database Synonyms' skip 1 -
       center 'Sorted by Scope' skip 2

clear break;
break on scope skip 2;
col synonym_name format a30;
select scope, synonym_name, table_owner, table_name
from esi.finder_synonyms
order by scope, synonym_name;

ttitle center 'Finder Database Sequences' skip 1 -
       center 'Sorted by Scope' skip 2

clear break;
break on scope skip 2;
col sequence_name format a30;
select scope, sequence_name, start_with, increment_by, min_value, max_value,
       cycle_flag, order_flag, cache_size
from esi.finder_sequences
order by scope, sequence_name;

ttitle center 'Finder Database System Defaults' skip 1 -
       center 'Sorted by Application' skip 2

clear break;
column default_name format a25;
column application_name format a25;
column default_value format A85 wrap word;
break on application_name skip 2;
select default_name, application_name, default_value
from esi.system_defaults
order by application_name, default_name;

ttitle center 'Finder Database Public Privileges' skip 1 -
       center 'Sorted by Scope' skip 2

clear breaks
break on scope page;
column tname format a30 heading 'Table Name';
column "Select" format a6
column "Insert" format a6
column "Update" format a6
column "Delete" format a6
column "Reference" format a10
select scope, tname,
decode(public_select,    'Y',' Yes', ' ..') "Select",
decode(public_insert,    'Y',' Yes', ' ..') "Insert",
decode(public_update,    'Y',' Yes', ' ..') "Update",
decode(public_delete,    'Y',' Yes', ' ..') "Delete",
decode(public_references,'Y',' Yes', ' ..') "Reference"
from esi.finder_table_privileges
order by scope, tname;

ttitle center 'Finder Database Logical Names' skip 1 -
       center 'Sorted by Scope' skip 2

break on scope page on name skip 1

col default_directory format a35
col name              format a30
col description       format A47 wrap word

select scope, name, environment, default_directory, description
from codes.finder_logical_names
order by scope, name, environment;

ttitle center 'Finder Database Logical Names' skip 1 -
       center 'Sorted by Logical Name' skip 2

break on name skip 1

select name, environment, default_directory, scope, description
from codes.finder_logical_names
order by name, environment, scope;

ttitle center 'Finder Database Tables and Columns' skip 1 -
       center 'Sorted by Data Domain' skip 2

column description format a58 wrap word;
column tname format a27 heading 'Table Name';
break on domain skip 1
select domain, tname, cname,
ctype||
     decode(ctype,'CHAR','('||width||')',
                  'NUMBER',decode(width,NULL,NULL,'('||width||
                               decode(scale,NULL,NULL,','||scale)
                            ||')')
           ) ||
        decode(NULLS,'NOT NULL',' *',NULL) ctype,
        description
from esi.finder_columns
where domain is not null
order by domain, tname, cname;

ttitle center 'Finder Database Domains' skip 1 -
       center 'Sorted by Default Name' skip 2

clear breaks
col description format a66 wrap word;
col width format 99999;
col scale format 99999;

select default_name, domain_code, ctype, width, scale, description
from esi.finder_domains
order by default_name;

ttitle center 'Finder Database Domains' skip 1 -
       center 'Sorted by Domain Code' skip 2

clear breaks
select domain_code, default_name, ctype, width, scale, description
from esi.finder_domains
order by domain_code;

ttitle center 'Finder Database Table Constraints' skip 1 -
       center 'Sorted by Scope' skip 2

clear break;
COL CHECK_CONSTRAINT FORMAT A95 WRAP WORD HEADING 'Check Constraints' JUSTIFY CENTER
column tname format a27 heading 'Table Name';
break on scope skip 2
select scope, tname, check_constraint
from esi.finder_tables
where check_constraint is not null
order by scope, tname;

ttitle center 'Finder Database Column Constraints and Assertions' skip 1 -
       center 'Sorted by Scope' skip 2

clear break;
col default_value format a7 heading 'Default|Value'
col check_constraint format a50 wrap word;
column tname format a27 heading 'Table Name';
break on scope skip 2 on tname skip 1;
select scope, b.tname, cname, default_value, b.check_constraint
from esi.finder_tables a, esi.finder_columns b
where (B.check_constraint is not null OR
default_value is not null)
and a.tname=b.tname
order by scope, b.tname;

ttitle center 'Finder Database Column Foreign Key References' skip 1 -
       center 'Sorted by Scope' skip 2

column tname format a27 heading 'Table Name';
column foreign_tname format A30 heading 'Foreign Key|Reference Table';
column foreign_key_cname heading 'Foreign Key|Reference Column';
select scope, b.tname, cname,
    decode(foreign_key_towner,null,null,foreign_key_towner||'.')
    ||foreign_key_tname foreign_tname,
    foreign_key_cname
from esi.finder_tables a, esi.finder_columns b
where a.tname = b.tname
and foreign_key_cname is not null
and foreign_key_tname is not null
order by scope, b.tname, foreign_key_towner, foreign_key_tname,cname;
