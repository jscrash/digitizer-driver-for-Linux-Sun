whenever sqlerror continue
drop table lit2;
create table lit2 (lid number(6) not null 
  , lnm char(20) not null 
  , sur char(20) not null);

insert into lit2 (lid, lnm, sur) 
select 
	s.line_id, 
	line_name, 
	survey 
from 
	&&1..seis_line_hdr s, 
	&&1..seis_tie t 
where 
	s.line_id = t.this_line_id;

set newpage 0
set pagesize 55
set linesize 130

ttitle 'Project &&1 Seismic Line Intersection Report'

col sur format a10 heading 'This|Survey'
col survey format a10 heading 'Tie|Survey'
col lnm format a10 heading 'This|Line Name'
col line_name format a10 heading 'Tie|Line Name'
col this_cdp format 9999 heading 'This|CDP #'
col tie_cdp format 9999 heading 'Tie|CDP #'
col this_shot format 9999 heading 'This|Shot #'
col tie_shot format 9999 heading'Tie|Shot #'
col tie_x format 99,999,999 heading 'X-Coord.'
col tie_y format 99,999,999 heading 'Y-Coord.'

select 
	sur, 
	lnm, 
	this_shot, 
	this_cdp, 
  	survey, 
	line_name, 
	tie_shot, 
	tie_cdp, 
	tie_x, 
	tie_y 
from 
	lit2, 
	&&1..seis_line_hdr h, 
	&&1..seis_tie t 
where 
	lid = t.this_line_id 
and 
  	t.tie_line_id = h.line_id
order by
	sur,
	lnm,
	this_shot;
drop table lit2;
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_INTERSECTION.SQL*/
/* *3    20-AUG-1991 16:06:54 ERIC "(SPR 1) No changes"*/
/* *2    20-AUG-1991 10:08:43 ERIC "(SPR 1) Initial checkin - replaces seis_line_intersections.sql."*/
/* *1    20-AUG-1991 10:07:16 ERIC "SQL query listing project seismic line intersection data."*/
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_INTERSECTION.SQL*/
