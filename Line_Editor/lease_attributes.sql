ttitle 'Project &1 Lease Attribute Reports'

break on lease_table skip 2 nodup
set newpage 0
set linesize 80
set pagesize 30
set space 5

col attribute_name format a19 heading 'Attribute Name'
col lease_table format a19 heading 'Lease Table Name'
col lease_data_item format a30 heading 'Attribute Definition' word_wrap

select
	lease_table,
	attribute_name,
	lease_data_item
from
	&1..lease_attributes
order by
	lease_table,
	attribute_name
/
/* DEC/CMS REPLACEMENT HISTORY, Element LEASE_ATTRIBUTES.SQL*/
/* *4    20-AUG-1991 15:59:48 ERIC "(SPR 1) No changes"*/
/* *3    19-AUG-1991 15:11:54 ERIC "(SPR 1) Made query project-specific."*/
/* *2    19-AUG-1991 14:53:35 ERIC "(SPR 1) Initial checkin"*/
/* *1    19-AUG-1991 14:52:17 ERIC "SQL query to list lease attributes required by LEASE mapping overlyy."*/
/* DEC/CMS REPLACEMENT HISTORY, Element LEASE_ATTRIBUTES.SQL*/
