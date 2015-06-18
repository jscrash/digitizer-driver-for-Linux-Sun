/* DEC/CMS REPLACEMENT HISTORY, Element CONVERT.SQL */
/* *1    13-NOV-1990 11:25:02 VINCE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element CONVERT.SQL */
/* DEC/CMS REPLACEMENT HISTORY, Element CONVERT.SQL */
/* *1     6-MAR-1990 08:39:31 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element CONVERT.SQL */
/* Conversion of Finder schema tables for Gulf Canada Modifications */
/* J Gillespie August, 1989 */

/* Move WELL_LOG_TRACE_HDR data to WELL_LOG_CURVE */

insert into well_log_curve
(
uwi,
source,
trace_type,
log_run,
log_id,
version,
shared_trace,
top,
base,
depth_inc,
depth_units,
depth_type,
trace_min,
trace_max,
trace_units,
trace_comment,
updated_by,
last_update
)
select
uwi,
nvl(source,'UNKNOWN'),
trace,
0,
nvl(service,'UNKNOWN'),
version,
shared,
top,
base,
dinc,
depth_unit,
depth_type,
trace_min,
trace_max,
trace_unit,
comments,
updated_by,
last_update
from well_log_trace_hdr;

/* Update columns in WELL_HDR (aliased WELL_MAIN) */

update well_hdr
set source = info_owner,
fldpoolwild = fpw,
crstatus = substr(status,1,20);

/* conversion for WELL_CHECKSHOT_HDR data - was in WELL_HDR */

insert into well_checkshot_hdr (
uwi,
source,
checkshot_datum,
checkshot_vsd,
checkshot_12way)
select
uwi,
nvl(info_owner,'?'),
checkshot_datum,
checkshot_vsd,
checkshot_12way
from well_hdr
where
checkshot_datum is not null or
checkshot_vsd is not null or
checkshot_12way is not null;

/* Make small change for WELL_TOPS */

update well_tops
set qual_code = substr(mdq,1,1);

/* Conversion of WELL_CASING_DATA table */

insert into well_casing (
uwi,
source,
case_obs_no,
case_code,
casing_size,
depth,
cement,
cement_unit)
select
uwi,
'?',
1,
'CASING',
decode(casing_size,'xxx','yyy',null),
depth,
cement,
cement_unit
from well_casing_data;

/* Conversion of WELL_LINER_DATA table */

insert into well_casing (
uwi,
source,
case_obs_no,
case_code,
casing_size,
depth,
liner_top,
cement,
cement_unit)
select
uwi,
'?',
1,
nvl(liner_type,'LINER'),
decode(liner_size,'xxx','yyy',null),
base,
top,
cement,
cement_unit
from well_liner_data;

/* Drillstem test recoveries */

insert into well_dst_vo_rec (
uwi,
source,
dst_no,
valve_open_no,
recov_type,
recov_amount,
recov_units)
select
uwi,
'?',
test_no,
0,
'?',
amount,
unit
from well_dst_recoveries;

/* Move diectional survey points */

insert into well_survey_pts (
uwi,
source,
survey_no,
point_id,
md,
deviation_angle,
azimuth,
dx,
dy,
tvd)
select
uwi,
nvl(source,'?'),
0,
0,
md,
deviation_angle_degrees + deviation_angle_minutes / 60.,
azimuth,
dx,
dy,
tvd
from well_directional_survey;

/* Minor change for TOWNSHIPS */

update townships set principal_meridian = prime_meridian;
