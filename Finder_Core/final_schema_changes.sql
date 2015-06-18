/* DEC/CMS REPLACEMENT HISTORY, Element FINAL_SCHEMA_CHANGES.SQL */
/* *3    17-JUL-1990 16:24:12 ALLAWAY "(SPR -1) update of project conversion script to include 7/15/90 schema updates." */
/* *2    20-JUN-1990 14:45:41 ALLAWAY "(SPR -1) updates projects to new wells schema and integer node_ids" */
/* *1    20-JUN-1990 14:44:09 ALLAWAY "updates old projects to new well schema" */
/* DEC/CMS REPLACEMENT HISTORY, Element FINAL_SCHEMA_CHANGES.SQL */
DOC;
               CHANGES REQUIRED TO ACCOMODATE THE WHCS DATA 
			Bill Allaway 2/21/89

The following script serves the dual purpose of documenting the changes made
to schema resulting from the 1/19/90 meeting between Finder Graphics, Enron,
Applied Terravision and Petroleum Information and providing a tool for 
updating existing Finder projects to the new schema.  Note that while the
DB_VER_TABS utility of Finder will update the table definitions in cases 
where the changes can be handled by simply adding columns, it does not 
rename tables or update columns where column names have changed.  This script
should be run on all Finder Projects or databases to ensure data are moved to
their appropriate resting place BEFORE DB_VER_TABS IS RUN ON THE PROJECT.  

Each table changed will have the changes 
made in the original 1/19/90 update included in the comment portion of the
table change description. Further changes to the schema and any movement of 
data will appear in the uncommented (ie executable) portion of the script.

This script was modified on 6/14/90 to update projects created before 1/19/90,
Including prism schema customers, since all known to contain the interim step
between the old prism schema and todays schema have been updated.
The history of changes will remain in the commented portion of the script.

Final agreement on the next release of the prism schema was made the first week
of July 1990 between ATS and Finder Graphics and this script was modified to
include those changes.  Finder customers who update their tables now will be
compatible with the next release of Applied Terravisions products expected
sometime in August 1990.  No further changes to the well schema are expected to
be made in the near future.

                                SCHEMA CHANGES

WELL_CASING - 1/19 This table had most of its columns renamed. It is therefore 
easier to recreate the table from scratch and move the data across...


	RENAME WELL_CASING TO OLD_WELL_CASING;

	CREATE TABLE WELL_CASING (
		UWI		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		CASE_OBS_NO	NUMBER(2) NOT NULL,
		CASE_DATE	DATE,
		CASE_CODE	CHAR(12),
		CASING_SIZE	NUMBER(5,1),
		CASING_SIZE_DESC	CHAR(12),
		CASING_BORE	NUMBER(5,1),
		CASING_HOLE	NUMBER(5,1),
		DEPTH		NUMBER(7,2),
		SHOE		NUMBER(7,2),
		CASING_FORM	CHAR(12),
		CASING_AGE	CHAR(6),
		LINER_TOP	NUMBER(7,2),
		DENSITY		NUMBER(5,3),
		STEEL_SPEC	CHAR(12),
		STRENGTH	NUMBER(3),
		COLLAR		CHAR(12),
		MIXED_STRING	CHAR(1),
		REMARKS		CHAR(240));

	INSERT INTO WELL_CASING (
		UWI,
		SOURCE,
		CASE_OBS_NO,
		CASE_DATE,
		CASE_CODE,
		CASING_SIZE,
		CASING_SIZE_DESC,
		CASING_BORE,
		CASING_HOLE,
		DEPTH,
		SHOE,
		CASING_FORM,
		CASING_AGE,
		LINER_TOP,
		DENSITY,
		STEEL_SPEC,
		STRENGTH,
		COLLAR,
		MIXED_STRING)
	SELECT
		UWI,
		SOURCE,
		CASE_OBS_NO,
		CASE_DATE,
		CASE_CODE,
		CASING_SIZE,
		CASING_SIZE_DESC,
		CASING_BORE,
		CASING_HOLE,
		DEPTH,
		CASE_SHOE,
		CASE_FORM,
		CASE_AGE,
		LINER_TOP,
		CASE_DENSITY,
		CASE_STEEL,
		CASE_STRENGTH,
		CASE_COLLAR,
		CASE_MXD_STRING
	FROM OLD_WELL_CASING;
DOC;
Further changes since 1/19 include combining WELL_CASING and WELL_TUBULARS:
Scripts below modified to go from pre-1/19 to 6/14/90 schema...

NOTE: FIRST RENAME MIGHT FAIL, DON'T WORRY ABOUT IT...
#
	RENAME WELL_TUBULARS TO OLD_WELL_TUBULARS;

	RENAME WELL_CASING TO WELL_CASING_OLD;

	CREATE TABLE WELL_TUBULARS(
		UWI		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		TUBING_OBS_NO	NUMBER(3) NOT NULL,
		TUBING_DATE	DATE,
		TUBING_TYPE	CHAR(12),
		TUBING_SIZE	NUMBER(5,1),
		TUBING_SIZE_DESC	CHAR(12),
		TUBING_BORE	NUMBER(5,1),
		TUBING_HOLE	NUMBER(5,1),
		TOP		NUMBER(7,2),
		BASE		NUMBER(7,2),
		SHOE		NUMBER(7,2),
		TUBING_FORM	CHAR(12),
		TUBING_AGE	CHAR(6),
		DENSITY		NUMBER(9,4),
		STEEL_SPEC	CHAR(12),
		STRENGTH	NUMBER(3),
		COLLAR		CHAR(12),
		MIXED_STRING	CHAR(1),
		REMARKS		CHAR(240));

	INSERT INTO WELL_TUBULARS(
		UWI,
		SOURCE,
		TUBING_OBS_NO,
		TUBING_DATE,
		TUBING_TYPE,
		TUBING_SIZE,
		TUBING_SIZE_DESC,
		TUBING_BORE,
		TUBING_HOLE,
		TOP,
		BASE,
		SHOE,
		TUBING_FORM,
		TUBING_AGE,
		DENSITY,
		STEEL_SPEC,
		STRENGTH,
		COLLAR,
		MIXED_STRING)
	SELECT
		UWI,
		SOURCE,
		CASE_OBS_NO,
		CASE_DATE,
		CASE_CODE,
		CASING_SIZE,
		CASING_SIZE_DESC,
		CASING_BORE,
		CASING_HOLE,
		LINER_TOP,
		DEPTH,
		CASE_SHOE,
		CASE_FORM,
		CASE_AGE,
		CASE_DENSITY,
		CASE_STEEL,
		CASE_STRENGTH,
		CASE_COLLAR,
		CASE_MXD_STRNG
	FROM WELL_CASING_OLD;

DOC;
THIS INSERT MAY FAIL, DONT WORRY ABOUT IT...
#
	INSERT INTO WELL_TUBULARS(
		UWI,
		SOURCE,
		TUBING_OBS_NO,
		TUBING_TYPE,
		TUBING_SIZE,
		TUBING_SIZE_DESC,
		TOP,
		BASE)
	SELECT
		UWI,
		SOURCE,
		TUBING_OBS_NO,
		TUBING_TYPE,
		TUBING_SIZE,
		TUBING_SIZE_DESC,
		TOP,
		BASE
		FROM OLD_WELL_TUBULARS;
DOC;
WELL_CEMENT - Add one column from PI data...
#
	ALTER TABLE WELL_CEMENT ADD (
		TUBING_TYPE	CHAR(12));
DOC;
No further changes to WELL_CEMENT beyond the 1/19/90 changes...

WELL_CMPT_ANAL_HDR -  The table as described by the 1/19 meeting ended up as:
#	
	CREATE TABLE WELL_CMPT_ANAL_HDR (
		UWI CHAR(20) NOT NULL,	
		SOURCE CHAR(12) NOT NULL,	
		TEST_NO CHAR(4) NOT NULL,	
		ANALYSIS_TYPE CHAR(12) NOT NULL, 
		COMPANY CHAR(12) NULL,	
		EST_DAMAGE_RATIO NUMBER(5,2) NULL,
		PERMEABILITY NUMBER(6,1) NULL,	 
		MAX_RES_PRESS NUMBER(5) NULL,	 
		POT_METRIC_SURF NUMBER(5) NULL,  
		RADIUS_INVEST NUMBER(5) NULL,	 
		REMARKS CHAR(240) NULL);
DOC;
In addition, the following field needs to be added to bring it in line 
with the tests data model. 
#
 	ALTER TABLE WELL_CMPT_ANAL_HDR ADD (
 		TEST_TYPE	CHAR(12));

DOC
WELL_CMPT_GAUGE - The table as described by the 1/19 meeting ended up as:
#
	CREATE TABLE WELL_CMPT_GAUGE(
		UWI CHAR(20) NOT NULL,	  
		SOURCE CHAR(12) NOT NULL,        
		TEST_NO CHAR(4) NOT NULL,        
		ANALYSIS_TYPE CHAR(12) NULL,     
		REPORT_NO NUMBER(3) NULL,        
		EST_DAMAGE_RATIO NUMBER(5,2) NULL,
		PERMEABILITY NUMBER(9,4) NULL,    
		PROD_INDEX NUMBER(4,0) NULL,      
		FINAL_RES_PRESS NUMBER(5) NULL,  
		CONFID_LIMIT NUMBER(6,1) NULL,    
		GAUGE_DEPTH NUMBER(7,2) NULL);     
DOC;
In addition, the following field needs to be added to bring it in line 
with the tests data model. 

#
 	ALTER TABLE WELL_CMPT_GAUGE ADD (
 		TEST_TYPE	CHAR(12));
DOC;
WELL_CMPT_SHUTOFF and WELL_CMPT_DESC are not related to the WELL_CMPT_ANAL_HDR
data and should not be named as though they are.  In fact, the data in 
WELL_CMPT_SHUTOFF can be stored in WELL_TEST_SHUTOFF and the data in 
WELL_CMPT_DESC can be stored in WELL_TEST_NAR. It is recommended that the 
WELL_TEST... tables be used and that these two WELL_CMPT... tables be dropped.

WELL_COMPLETIONS - Change name to WELL_COMPLET_HDR and add Producing_thru
field
	
	RENAME WELL_COMPLETIONS TO WELL_COMPLET_HDR;

	ALTER TABLE WELL_COMPLET_HDR ADD (
		PRODUCING_THRU CHAR(12));

The data stored in WELL_COMPLET_HDR is tied to the IP and Production tests. 
The current schema ignores this and therefore loses some of the context in
which these data should be stored.  Since this is a test related table, 
the name should reflect this relationship. The following changes are 
suggested.

	RENAME WELL_COMPLET_HDR TO WELL_COMPLETIONS;

6/14/90 -The above remain commented out since they are not necessary for 
projects predating the 1/19 meeting...
#
	
	ALTER TABLE WELL_COMPLETIONS ADD (
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_COMPLETIONS ADD (
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_COMPLETIONS ADD (
		TYPE_PERF	CHAR(5));

	ALTER TABLE WELL_COMPLETIONS ADD (
		PRODUCING_THRU CHAR(12));

	ALTER TABLE WELL_COMPLETIONS ADD (
		PERFS_PER	CHAR(2));

	ALTER TABLE WELL_COMPLETIONS ADD (
		PERF_STATUS	CHAR(1));
DOC 
The data destined for this table traditionally have been stored in 
WELL_TEST_FORMS_INT and WELL_PROD_PERFS in Finder, so we need to move this 
data over and drop the obsolete tables...
#
	INSERT INTO WELL_COMPLETIONS (
		UWI,
		TEST_NO,
		TEST_TYPE,
		TOP_AGE,
		TOP_FORM,
		TOP,
		BASE,
		TYPE_PERF,
		PERF_SHOTS,
		PERFS_PER,
		PRODUCING_THRU,
		COMP_TYPE)
		SELECT
			UWI,
			TEST_NO,
			TEST_TYPE,
			AGE_CODE,
			FORM_CODE,
			TOP,
			BASE,
			TYPE_PERF,
			NPERFS,
			PERFS_PER,
			PROD_THRU,
			INTERVAL_TYPE
		FROM WELL_TEST_FORMS_INT;

	INSERT INTO WELL_COMPLETIONS (
		UWI,
		TEST_NO,
		TEST_TYPE,
		TOP,
		BASE,
		PERF_SHOTS)
		SELECT
			UWI,
			TEST_NO,
			'IP/PROD',
			TOP,
			BASE,
			PERF_NO
		FROM WELL_PROD_PERFS;

DOC
WELL_COMPLET_DETL - The data in this table are not related to the data in what
was the WELL_COMPLET_HDR table other than that data are listed by test. This
table was used to store treatment data.  Perforated intervals do not 
necessarily correspond to treated intervals, so I think this table should
not be named to imply that they do.  The PI data puts all completion-
related data into the test cards, so both WELL_COMPLETIONS and this table 
should be able to relate the data stored to test data.  I propose that this
table be renamed to WELL_TREATMENT, and that WELL_PROD_TREATMENT be dropped 
from the Finder Schema.  
The following fields should be removed from WELL_TREATMENT:
	COMP_OBS_NO
The following fields should be added to WELL_TREATMENT to support the manner 
in which PI reports the data:
#
	RENAME WELL_COMPLET_DETL TO WELL_TREATMENT;

	ALTER TABLE WELL_TREATMENT ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_TREATMENT ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_TREATMENT ADD(
		TOP		NUMBER(7,2));

	ALTER TABLE WELL_TREATMENT ADD(
		BASE		NUMBER(7,2));

	ALTER TABLE WELL_TREATMENT ADD(
		REMARKS		CHAR(240));

DOC;
For Gulf Canada and other Applied Terravision customers who want to update,
comment out the above renames and alters and instead execute the following:
#
	CREATE TABLE WELL_TREATMENT (
		UWI CHAR(20) NOT NULL,
		TEST_NO CHAR(4) NOT NULL,
		TEST_TYPE CHAR(12) NULL,
		SOURCE	CHAR(12) NOT NULL,
		INTERVAL_NO NUMBER(2) NULL,
		TOP NUMBER(7,2) NULL,
		BASE NUMBER(7,2) NULL,
		TREATMENT_TYPE CHAR(12) NOT NULL,
		TREATMENT_AMOUNT NUMBER(7,1) NULL,
		TREATMENT_UNIT CHAR(12) NULL,
		AGENT_TYPE CHAR(12) NULL,
		AGENT_AMOUNT NUMBER(7,1) NULL,
		AGENT_UNIT CHAR(12) NULL,
		FORM_BREAK_PRESS NUMBER(5) NULL,
		INJECTION_RATE NUMBER(5,1) NULL,
		ADDITIVE_TYPE CHAR(12) NULL,
		STAGES NUMBER(3) NULL,
		REMARKS CHAR(240) NULL);

Move any data from well_prod_treatment to well_treatment:
#
	INSERT INTO WELL_TREATMENT (
		UWI,
		TEST_NO,
		TEST_TYPE,
		SOURCE,
		INTERVAL_NO,
		TOP,
		BASE,
		TREATMENT_TYPE,
		TREATMENT_AMOUNT,
		TREATMENT_UNIT,
		AGENT_TYPE,
		AGENT_AMOUNT,
		AGENT_UNIT,
		FORM_BREAK_PRESS,
		INJECTION_RATE,
		ADDITIVE_TYPE,
		STAGES,
		REMARKS)
	SELECT
		UWI,
		TEST_NO,
		TEST_TYPE,
		'PROD TREAT',
		TREAT_NO,
		TOP,
		BASE,
		TREAT_TYPE,
		TREAT_WT_VOL,
		TREAT_UNIT,
		PROPANT_TYPE,
		PROPANT_AMT,
		PROPANT_UNIT,
		FM_BREAKDOWN,
		INJ_RATE,
		ADDITIVE_TYPE,
		STAGES,
		REMARKS
	FROM WELL_PROD_TREATMENT;

DOC;
WELL_CORE_HDR - This adds a field for operator assigned core id since it is 
necessary to use the 2-3 columns of the PI cards for core_id in order to join
the various types of core data in the different PI cards.  Also with the
addition of WELL_CORE_OVERLAPS, there is no need for top/base formation and 
age fields. I recommend that these be replaced with PRIMARY_FORM, PRIMARY_AGE
fields, with all formations penetrated listed in well_core_overlaps.  
Therefore remove:
		TOP_FORM
		TOP_AGE
		BASE_FORM
		BASE_AGE 
	FROM WELL_CORE_HDR, then:
#
	ALTER TABLE WELL_CORE_HDR ADD (
		OPR_CORE_NO 	CHAR(6));

	ALTER TABLE WELL_CORE_HDR ADD (
		REMARKS		CHAR(240));

	ALTER TABLE WELL_CORE_HDR ADD (
		PRIMARY_FORM	CHAR(12));

	ALTER TABLE WELL_CORE_HDR ADD (
		PRIMARY_AGE	CHAR(6));

	ALTER TABLE WELL_CORE_HDR MODIFY(
		CORE_DIA	NUMBER(5,3));

	UPDATE WELL_CORE_HDR SET 
		PRIMARY_FORM = TOP_FORM,
		PRIMARY_AGE = TOP_AGE;
DOC;
WELL_CORE_OVERLAPS - New table to store the formations penetrated by a core. 
# 
	CREATE TABLE WELL_CORE_OVERLAPS ( 
		UWI 		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		CORE_ID		CHAR(12) NOT NULL,
		AGE_CODE	CHAR(6),
		FORM_CODE	CHAR(12));
 
	INSERT INTO WELL_CORE_OVERLAPS (
		UWI,
		SOURCE,
		CORE_ID,
		AGE_CODE,
		FORM_CODE)
	SELECT
		UWI,
		SOURCE,
		CORE_ID,
		TOP_AGE,
		TOP_FORM
	FROM WELL_CORE_HDR WHERE TOP_FORM IS NOT NULL;

	INSERT INTO WELL_CORE_OVERLAPS (
		UWI,
		SOURCE,
		CORE_ID,
		AGE_CODE,
		FORM_CODE)
	SELECT
		UWI,
		SOURCE,
		CORE_ID,
		BASE_AGE,
		BASE_FORM
	FROM WELL_CORE_HDR WHERE BASE_FORM IS NOT NULL;
DOC;
WELL_CORE_DESC - The 1/19 meeting resulted in the addition of DEPTH and 
THICK columns to this table.
#
	ALTER TABLE WELL_CORE_DESC ADD (
		DEPTH		NUMBER(7,2));

	ALTER TABLE WELL_CORE_DESC ADD (
		THICK		NUMBER(7,2));
DOC;
No further changes to WELL_CORE_DESC are needed.  The Permian basin 
descriptions will for the moment have their detail data stored in the 
REMARKS field.

WELL_CORE_SAMPLE - The 1/19 meeting resulted in the addition of the following:
#		
	ALTER TABLE WELL_CORE_SAMPLE ADD(
		POROSITY_CODE	CHAR(12));

	ALTER TABLE WELL_CORE_SAMPLE ADD(
		POROSITY_TYPE	CHAR(12));

	ALTER TABLE WELL_CORE_SAMPLE ADD(
		POROSITY_LENGTH	NUMBER(7,2));

	ALTER TABLE WELL_CORE_SAMPLE ADD(
		DIP		NUMBER(3,0));

	ALTER TABLE WELL_CORE_SAMPLE ADD(
		LITHOLOGY	CHAR(32));
DOC;
No further changes to WELL_CORE_SAMPLE are needed.

WELL_CORE_SHOWS - The 1/19 meeting resulted in the addition of the following:
#		
	CREATE TABLE WELL_CORE_SHOWS (
		UWI		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		CORE_ID		CHAR(12),
		POS_NO		NUMBER(3,0),
		SHOW_TYPE	CHAR(12),
		SHOW_QUALITY	CHAR(1),
		SHOW_DESC	CHAR(4));
DOC;
No further changes to WELL_CORE_SHOWS are needed.

WELL_CORE_LINE - The following need to be added to WELL_CORE_LINE to 
accomodate all the core analysis data provided by PI (and most core
analysis companies):
#
	ALTER TABLE WELL_CORE_LINE ADD (
		EFFECTIVE_POR 	NUMBER(9,4));

	ALTER TABLE WELL_CORE_LINE ADD (
		GAS_SAT_VOL	NUMBER(9,4));

DOC;
In addition the following columns are being expanded to facilitate data
loading and unit conversion.
#
	ALTER TABLE WELL_CORE_LINE MODIFY(
		KMAX		NUMBER(12,4),
		K90		NUMBER(12,4),
		KVERT		NUMBER(12,4),
		POROSITY	NUMBER(9,4),
		GRAIN_DENSITY	NUMBER(9,4),
		BULK_DEN	NUMBER(9,4),
		BULK_MASS_OIL	NUMBER(9,4),
		BULK_MASS_WAT	NUMBER(9,4),
		BULK_MASS_SAND	NUMBER(9,4),
		PORE_VOL_OIL	NUMBER(9,4),
		PORE_VOL_WAT	NUMBER(9,4));
		
DOC
WELL_CRD_ANAL_HDR - The 1/19 meeting resulted in the following changes to
this table:
	remove:
		CRUDE_OBS_NO
		DST_NO
		POUR_POINT
	Then make the following changes:
#
	RENAME WELL_CRD_ANAL_HDR TO WELL_OIL_ANAL_HDR;

	ALTER TABLE WELL_OIL_ANAL_HDR ADD(
		TEST_NO		CHAR(4),
		TEST_TYPE	CHAR(12),
		DENSITY_UNIT	CHAR(12),
		GOR		NUMBER(5)); 

	UPDATE WELL_OIL_ANAL_HDR SET
		TEST_NO = DST_NO,
		TEST_TYPE = 'DST';
DOC;
6/90 UPDATE...
#
	ALTER TABLE WELL_OIL_ANAL_HDR MODIFY(DENSITY NUMBER(9,4));
DOC;
No further changes to WELL_OIL_ANAL_HDR are needed.

WELL_CRD_VISCOSITY - The 1/19 meeting resulted in the following changes to
this table:
	remove:
		CRUDE_OBS_NO
		VISCOSITY_NO
	Then make the following changes:
#
	RENAME WELL_CRD_VISCOSITY TO WELL_OIL_VISCOSITY;

	ALTER TABLE WELL_OIL_VISCOSITY ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_OIL_VISCOSITY ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_OIL_VISCOSITY ADD(
		OIL_TYPE	CHAR(12));

	ALTER TABLE WELL_OIL_VISCOSITY ADD(
		POUR_POINT	NUMBER(6,1));

DOC;
No further changes to WELL_OIL_VISCOSITY are needed.

WELL_DRILL_SHOW - Make lithology field longer...
#
	ALTER TABLE WELL_DRILL_SHOW MODIFY (LITHOLOGY CHAR(32));
DOC;
No further changes to WELL_DRILL_SHOW are needed.

WELL DST TABLES - All well DST tables will change names to WELL_FORM_TEST...
The WELL_DST_VO,WELL_DST_VO_MAT,WELL_DST_VO_REC tables will simply become
WELL_TEST... tables and provide storage for the same information from both 
production tests and formation tests.
#
	RENAME WELL_DST_HDR TO WELL_FORM_TEST_HDR;
	RENAME WELL_DST_VO TO WELL_TEST_VO;
	RENAME WELL_DST_VO_MAT TO WELL_TEST_VO_MAT;
	RENAME WELL_DST_VO_REC TO WELL_TEST_VO_REC;
	RENAME WELL_DST_VO_ANAL TO WELL_TEST_VO_ANAL;

DOC
Update old schema tables to the interim schema. Leave this commented out if 
you are currently using the interim schema (based on 1/19/89 meeting).
#
	ALTER TABLE WELL_FORM_TEST_HDR ADD (
		TEST_NO			CHAR(4));

	ALTER TABLE WELL_FORM_TEST_HDR ADD (
		OPR_SEQ			NUMBER(3,0));

	ALTER TABLE WELL_FORM_TEST_HDR ADD (
		TOP_CHOKE_DESC		CHAR(6));

	ALTER TABLE WELL_FORM_TEST_HDR ADD (
		BOTTOM_CHOKE_DESC	CHAR(6));

	ALTER TABLE WELL_FORM_TEST_HDR ADD (
		TOOL_DIAMETER		CHAR(6));

	ALTER TABLE WELL_FORM_TEST_HDR ADD (
		SURF_FLOW_PRESS		NUMBER(5));

	UPDATE WELL_FORM_TEST_HDR
		SET TEST_NO = DST_NO;
COMMIT;

	ALTER TABLE WELL_TEST_VO ADD (
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_TEST_VO ADD (
		BH_TEMPERATURE	NUMBER(5,2));

	ALTER TABLE WELL_TEST_VO ADD (
		REMARKS		CHAR(240));

	UPDATE WELL_TEST_VO
		SET TEST_NO = DST_NO;
COMMIT;

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		TEST_NO			CHAR(4));

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		SALINITY_UNIT		CHAR(12));

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		WATER_RESISTIVITY	NUMBER(8,4));

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		WATER_TEMPERATURE	NUMBER(5,2));

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		GAS_CONTENT		NUMBER(9,2));

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		COND_RATIO		NUMBER(6,2));

	ALTER TABLE WELL_TEST_VO_ANAL ADD(
		COND_RATIO_UNIT		CHAR(12));

	UPDATE WELL_TEST_VO_ANAL
		SET TEST_NO = DST_NO;


COMMIT;

	ALTER TABLE WELL_TEST_VO_MAT ADD (
		TEST_NO			CHAR(4));

	ALTER TABLE WELL_TEST_VO_MAT ADD (
		PERCENT_SHAKE_OUT	NUMBER(5,2));

	ALTER TABLE WELL_TEST_VO_MAT ADD (
		FLOW_DURATION		NUMBER(7,2));

	ALTER TABLE WELL_TEST_VO_MAT ADD (
		FLOW_DURATION_UNIT	CHAR(12));

	ALTER TABLE WELL_TEST_VO_MAT ADD (
		MEASURE_TECH		CHAR(12));

	UPDATE WELL_TEST_VO_MAT
		SET TEST_NO = DST_NO;

	ALTER TABLE WELL_TEST_VO_REC ADD(
		TEST_NO		CHAR(4));

	UPDATE WELL_TEST_VO_REC
		SET TEST_NO = DST_NO;
COMMIT;
DOC;
Additional post 1/19 changes:
WELL_FORM_TEST_HDR - Change field names that suggest specific test type...
	Drop the following fields:
		DST_DATE
		DST_COMP
		DST_REMARKS
	then...
#
		ALTER TABLE WELL_FORM_TEST_HDR ADD(
			TEST_DATE	DATE);

		ALTER TABLE WELL_FORM_TEST_HDR ADD(
			TEST_COMP	CHAR(12));

		ALTER TABLE WELL_FORM_TEST_HDR ADD(
			TEST_REMARKS	CHAR(240));

	UPDATE WELL_FORM_TEST_HDR SET
		TEST_DATE = DST_DATE,
		TEST_COMP = DST_COMP,
		TEST_REMARKS = DST_REMARKS;
COMMIT;
DOC;
WELL_TEST_VO - Add test type to make test schema consistant new tests model.
# 
	ALTER TABLE WELL_TEST_VO ADD (
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_TEST_VO ADD (
		CHOKE_SIZE_DESC	CHAR(8)); 
DOC;
WELL_TEST_VO_ANAL - Add test type to make test schema consistant new 
tests model.
# 
	ALTER TABLE WELL_TEST_VO_ANAL ADD (
		TEST_TYPE	CHAR(12)); 
DOC;
6-90 UPDATE...
#
	ALTER TABLE WELL_TEST_VO_ANAL MODIFY(OIL_DENSITY NUMBER(9,4));


DOC
WELL_TEST_VO_MAT - Add test type to make test schema consistant new 
tests model.
# 
	ALTER TABLE WELL_TEST_VO_MAT ADD (
		TEST_TYPE	CHAR(12)); 
DOC
WELL_TEST_VO_REC - Add test type to make test schema consistant new tests model.
# 
	ALTER TABLE WELL_TEST_VO_REC ADD (
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_TEST_VO_REC ADD (
		REMARKS		CHAR(240)); 
DOC;
Move the data from the production tables into the new generic tables...

First from the interim schema, the next two inserts will fail on older projects:
#
	INSERT INTO WELL_TEST_VO_MAT(
		UWI,SOURCE,TEST_NO,TEST_TYPE,VALVE_OPEN_NO,FLUID_TYPE,
		FLUID_RATE,FLUID_RATE_UNIT,TTS,TTS_UNIT,PERCENT_SHAKE_OUT,
		FLOW_DURATION,FLOW_DURATION_UNIT,MEASURE_TECH)
		SELECT
		UWI,SOURCE,TEST_NO,TEST_TYPE,'1',FLUID_TYPE,
		FLUID_RATE,FLUID_RATE_UNIT,TTS,TTS_UNIT,PERCENT_SHAKE_OUT,
		FLOW_DURATION,FLOW_DURATION_UNIT,MEASURE_TECH
		FROM WELL_PROD_TEST_MAT;

COMMIT;

	INSERT INTO WELL_TEST_VO_REC (
		UWI,SOURCE,TEST_NO,TEST_TYPE,VALVE_OPEN_NO,RECOV_TYPE,
		RECOV_AMOUNT,RECOV_UNIT,RECOV_CONT)
		SELECT
		UWI,SOURCE,TEST_NO,TEST_TYPE,'1',RECOV_TYPE,
		RECOV_AMOUNT,RECOV_UNIT,RECOV_CONT
		FROM WELL_PROD_TEST_REC;
COMMIT;		
DOC;
Next try from the older production tables...
#
	INSERT INTO WELL_TEST_VO_MAT(
		UWI,SOURCE,TEST_NO,TEST_TYPE,VALVE_OPEN_NO,FLUID_TYPE,
		FLUID_RATE,FLUID_RATE_UNIT,TTS,TTS_UNIT)
		SELECT
		UWI,SOURCE,IPRD_NO,'PROD','1',FLUID_TYPE,
		FLUID_RATE,FLUID_RATE_UNIT,TTS,TTS_UNIT
		FROM WELL_INIT_PROD_MAT;

COMMIT;

	INSERT INTO WELL_TEST_VO_REC (
		UWI,SOURCE,TEST_NO,TEST_TYPE,VALVE_OPEN_NO,RECOV_TYPE,
		RECOV_AMOUNT,RECOV_UNIT,RECOV_CONT)
		SELECT
		UWI,SOURCE,IPRD_NO,'PROD','1',RECOV_TYPE,
		RECOV_AMOUNT,RECOV_UNIT,RECOV_CONT
		FROM WELL_INIT_PROD_REC;
COMMIT;		
DOC; 
WELL_DST_MUD - Older schemas (Gulf/ATS) do not have this table. Comment out 
this portion and run the command below which simply creates the table.

Rename to WELL_FORM_TEST_MUD to make naming consistant...

	RENAME WELL_DST_MUD TO WELL_FORM_TEST_MUD;
DOC;
WELL_FORM_TEST_MUD - These are minor changes to load additional information 
from the PI cards into this table.

	ALTER TABLE WELL_FORM_TEST_MUD ADD (
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_FORM_TEST_MUD ADD (
		WEIGHT_UNIT	CHAR(12));

	ALTER TABLE WELL_FORM_TEST_MUD ADD (
		SALINITY_UNIT	CHAR(12));

	UPDATE WELL_FORM_TEST_MUD SET TEST_TYPE = 'DST';
COMMIT;
DOC;
WELL_FORM_TEST_MUD - Create the table for sites that do not have
WELL_DST_MUD....
#
CREATE TABLE WELL_FORM_TEST_MUD(             UWI CHAR(20) NOT NULL,           
                                             SOURCE CHAR(12) NULL,            
                                             TEST_NO CHAR(4) NOT NULL,        
                                             TEST_TYPE CHAR(12) NULL,         
                                             FILTRATE_RESIST NUMBER(8,4) NULL,
                                             FILTRATE_TEMP NUMBER(5,2) NULL,  
                                             MUD_TYPE CHAR(12) NOT NULL,      
                                             TEMPERATURE NUMBER(5,2) NULL,    
                                             VISCOSITY NUMBER(7,2) NULL,      
                                             WEIGHT NUMBER(5,2) NULL,         
                                             RESISTIVITY NUMBER(8,4) NULL,    
                                             SALINITY NUMBER(10,6) NULL,      
                                             SALINITY_UNIT CHAR(12) NULL,     
                                             WEIGHT_UNIT CHAR(12) NULL);       

DOC;
WELL_DRILL_SHOW/WELL_SHOWS - WELL_DRILL_SHOW duplicates information in 
WELL_SHOWS, the main difference being that WELL_SHOWS is tied to test data.  
Since no "show" information is generally given for test data, I propose to 
change the TEST_NO, and TEST_TYPE fields from being NOT NULL to NULL and add 
the sample type and lithology fields to WELL_SHOWS so that any type of shows 
can be tracked with this one table.  The WELL_DRILL_SHOW table will be 
dropped.
# 
	ALTER TABLE WELL_SHOWS ADD (
		SAMPLE_TYPE	CHAR(12));

	ALTER TABLE WELL_SHOWS ADD (
		LITHOLOGY	CHAR(32));

	ALTER TABLE WELL_SHOWS MODIFY(
		TEST_NO 	CHAR(4) NULL,
		TEST_TYPE	CHAR(12) NULL);

	INSERT INTO WELL_SHOWS (
		UWI,
		SOURCE,        
		SHOW_TYPE,     
		TOP,
		BASE,
		TOP_FORM,
		TOP_AGE,
		BASE_FORM,
		BASE_AGE, 
		SAMPLE_TYPE,
		LITHOLOGY,
		REMARKS)
		SELECT
			UWI,
			SOURCE,        
			SHOW_TYPE,     
			TOP,
			BASE,
			TOP_FORM,
			TOP_AGE,
			BASE_FORM,
			BASE_AGE, 
			SAMPLE_TYPE,
			LITHOLOGY,
			REMARKS
		FROM WELL_DRILL_SHOW;
COMMIT;
DOC;
WELL_GAS_ANAL_HDR - The following changes resulted from the 1/19 meeting
	Remove:
		GA_NO
#
	ALTER TABLE WELL_GAS_ANAL_HDR ADD(
		TEST_NO		CHAR(4),
		TEST_TYPE	CHAR(12));
DOC;
No further changes to WELL_GAS_ANAL_HDR are required.

WELL_GAS_ANAL_DETL - The following changes resulted from the 1/19 meeting
	Remove:
		GA_NO
#
	ALTER TABLE WELL_GAS_ANAL_DETL ADD(
		TEST_NO		CHAR(4),
		TEST_TYPE	CHAR(12));
DOC;
No further changes to WELL_GAS_ANAL_DETL are required.


WELL_HDR AND WELL_HDR_SECOND - The following changes resulted from 
the 1/19 meeting.
#
	ALTER TABLE WELL_HDR ADD(
		LEASE_NO	CHAR(12));

	ALTER TABLE WELL_HDR ADD(
		LEASE_NAME	CHAR(40));

	ALTER TABLE WELL_HDR ADD(
		DISTRICT	CHAR(16));

	ALTER TABLE WELL_HDR ADD(
		GOVT_ASSIGNED_NO	CHAR(12));

	ALTER TABLE WELL_HDR ADD(
		STRAT_COLUMN	CHAR(12));

	ALTER TABLE WELL_HDR_SECOND ADD(
		LEASE_NO	CHAR(12));

	ALTER TABLE WELL_HDR_SECOND ADD(
		LEASE_NAME	CHAR(40));

	ALTER TABLE WELL_HDR_SECOND ADD(
		DISTRICT	CHAR(16));

	ALTER TABLE WELL_HDR_SECOND ADD(
		GOVT_ASSIGNED_NO	CHAR(12));

	ALTER TABLE WELL_HDR_SECOND ADD(
		STRAT_COLUMN	CHAR(12));
DOC;
In addition, the following columns need to be widened in order to accomodate
the PI data. (Note that these column widths need not be expanded for 
companies not using the PI data.  Finder Graphics intends to leave them
narrow for those companies that do not use PI data). The storage unit field
is added to accomodate some proprietary data sets with mixed units between
wells.
#
	ALTER TABLE WELL_HDR ADD(
		STORAGE_UNIT	CHAR(12));

	ALTER TABLE WELL_HDR_SECOND ADD(
		STORAGE_UNIT	CHAR(12));

	ALTER TABLE WELL_HDR MODIFY (
		WELL_NUMBER	CHAR(12));

	ALTER TABLE WELL_HDR MODIFY (
		OPERATOR	CHAR(30));

	ALTER TABLE WELL_HDR MODIFY (
		PROV_ST		CHAR(8));

	ALTER TABLE WELL_HDR MODIFY (
		FLDPOOLWILD	CHAR(18));

	ALTER TABLE WELL_HDR_SECOND MODIFY (
		WELL_NUMBER	CHAR(12));

	ALTER TABLE WELL_HDR_SECOND MODIFY (
		OPERATOR	CHAR(30));

	ALTER TABLE WELL_HDR_SECOND MODIFY (
		PROV_ST		CHAR(8));

	ALTER TABLE WELL_HDR_SECOND MODIFY (
		FLDPOOLWILD	CHAR(18));

DOC;
WELL_INIT_POTENT - This table simply gives the details of flow rates 
for a given test.  I suggest the table be named something more descriptive.
To make this table useful it might be good to add choke size and duration
to this table to put the flow rates in context, but this in not needed to
load WHCS data. Also drop IPOT_OBS_NO and replace it with OBS_NO.
#
	RENAME WELL_INIT_POTENT TO WELL_PROD_FLOW_RATE;

	ALTER TABLE WELL_PROD_FLOW_RATE ADD(
		OBS_NO		NUMBER(3));

	UPDATE WELL_PROD_FLOW_RATE SET
		OBS_NO = IPOT_OBS_NO;
COMMIT;
DOC;
WELL_INIT_PROD - The 1/19 meeting resulted in the following changes:

	Remove the following fields:
		OBS_NO, FLUID_TYPE,FLUID_REC,FLUID_FLOW
#
	RENAME WELL_INIT_PROD TO WELL_PROD_TEST_HDR;

	ALTER TABLE WELL_PROD_TEST_HDR MODIFY(TEST_TYPE CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		CHOKE_DESC	CHAR(6));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		METHOD		CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		OIL_GRAVITY	NUMBER(5,1));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		GOR		NUMBER(8,2));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		GOR_UNIT	CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		COND_RATIO	NUMBER(8,2));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		COND_RATIO_UNIT	CHAR(12));
DOC;
While WELL_PROD_FLOW_RATE is supposed to track details of flow rate, I think
that the overall flow rates for a given test should appear in 
WELL_PROD_TEST_HDR.  There is already a field for gas flow rate in the table
the following simply fills in the rest of the data. Also need to make the
test date IPRD_DATE more generic so drop IPRD_DATE replace with PROD_DATE...
#
	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		GAS_FLOW_UNIT	CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		OIL_FLOW	NUMBER(6,1));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		OIL_FLOW_UNIT	CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		WATER_FLOW	NUMBER(6,1));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		WATER_FLOW_UNIT	CHAR(12));

	ALTER TABLE WELL_PROD_TEST_HDR ADD(
		PROD_DATE	DATE);

	ALTER TABLE WELL_PROD_TEST_HDR MODIFY (
		GAS_FLOW	NUMBER(8,3));
		
	UPDATE WELL_PROD_TEST_HDR SET
		PROD_DATE = IPRD_DATE;
COMMIT;
DOC;
WELL_LOG_RUN_HDR - 6/90 UPDATE...
#
	ALTER TABLE WELL_LOG_RUN_HDR MODIFY(FLUID_DENSITY NUMBER(9,4));

	ALTER TABLE WELL_LOG_RUN_HDR MODIFY(LOG_MATRX_DEN NUMBER(9,4));
DOC;
WELL_M_B - Well metes and bounds data. The following fields had been added
in the 1/19 meeting:
#
	ALTER TABLE WELL_M_B ADD(
		NORTH_SOUTH_BOUND	CHAR(1));

	ALTER TABLE WELL_M_B ADD(
		EAST_WEST_BOUND		CHAR(1));

	ALTER TABLE WELL_M_B ADD(
		SPOT_CODE		CHAR(12));
DOC;
No other changes to WELL_M_B are needed. In fact, I think these data are
duplicated in WELL_LOCATIONS at least in part.

WELL_MISC_DATA - This table needs to be resurrected to hold WHCS data not
loaded elsewhere...
#
	CREATE TABLE WELL_MISC_DATA(
		UWI CHAR(20) NOT NULL,
		SOURCE CHAR(12) NOT NULL,
		DATA_TYPE CHAR(5),
		REMARKS CHAR(240),
		LAST_UPDATE DATE);
DOC;
WELL_POROSITY_ZONES - This table will be replaced by WELL_POROUS_INTRVL.
The following changes need to be made:
	DEPTH will be deleted and replaced with TOP
	ELEVATION will be deleted and replaced with TOP_TVD_SS
Therefore the following changes need to be made:
#
	ALTER TABLE WELL_POROUS_INTRVL ADD(
		FORM_CODE	CHAR(12));

	ALTER TABLE WELL_POROUS_INTRVL ADD(
		AGE_CODE	CHAR(6));

	ALTER TABLE WELL_POROUS_INTRVL ADD(
		TOP		NUMBER(7,2));

	ALTER TABLE WELL_POROUS_INTRVL ADD(
		BASE		NUMBER(7,2));

	ALTER TABLE WELL_POROUS_INTRVL ADD(
		TOP_TVD_SS	NUMBER(7,2));

	ALTER TABLE WELL_POROUS_INTRVL ADD(
		POR_QUALITY	CHAR(1));

	ALTER TABLE WELL_POROUS_INTRVL ADD(
		POROSITY_TYPE	CHAR(12));

	UPDATE WELL_POROUS_INTRVL SET
		TOP = DEPTH,
		TOP_TVD_SS = ELEVATION;
COMMIT;

	INSERT INTO WELL_POROUS_INTRVL(
		UWI,
		FORM_CODE,
		TOP,
		BASE,
		POR_QUALITY,
		POROSITY_TYPE)
	   SELECT
		UWI,
		FORM_CODE,
		TOP,
		BASE,
		GPF,
		POROSITY
	   FROM WELL_POROSITY_ZONES;
DOC;
6/90 UPDATE...
WELL_POROUS_INTRVL - Expand the gross_porosity field to allow for room to
massage data using sql updates...
#
	ALTER TABLE WELL_POROUS_INTRVL MODIFY(GROSS_POROSITY NUMBER(9,4));


COMMIT;
DOC;
WELL_PRESS_4PT - New table to store the detail measurements associated with
4 point tests. It was deemed necessary because there are 4
measurements per calculated absolute open flow value and it seemed silly to
widen WELL_PRESS_AOF to store all these measurements.  I decided to leave
WELL_PRESS_AOF to store just the final AOF calculations, wherever they were
from and the 4 point detail data can be stored in this new table: 
# 
	CREATE TABLE WELL_PRESS_4PT (
		UWI		CHAR(20) NOT NULL,
		TEST_NO		CHAR(4)	NOT NULL,
		TEST_TYPE	CHAR(12) NOT NULL,
		AOF_OBS_NO	NUMBER(3) NOT NULL,
		POINT_NO	NUMBER(3),
		CHOKE_SIZE_DESC	CHAR(12),
		FLOW_RATE	NUMBER(7),
		FLOW_PRESS	NUMBER(5),
		BH_FLOW_PRESS	NUMBER(5)); 
DOC;
WELL_PRESS_HDR - The following changes resulted from the 1/19 meeting:
#
	ALTER TABLE WELL_PRESS_HDR ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_PRESS_HDR ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_PRESS_HDR ADD(
		FLOW_TUBE_PRESS	NUMBER(5));

	ALTER TABLE WELL_PRESS_HDR ADD(
		FLOW_CASE_PRESS	NUMBER(5));

	ALTER TABLE WELL_PRESS_HDR ADD(
		SHUT_CASE_PRESS	NUMBER(5));

	ALTER TABLE WELL_PRESS_HDR ADD(
		SHUT_TUBE_PRESS	NUMBER(5));
DOC;
No further changes to WELL_PRESS_HDR are needed.

WELL_PRESS_AOF - The following changes resulted from the 1/19 meeting:
#
	ALTER TABLE WELL_PRESS_AOF ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_PRESS_AOF ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_PRESS_AOF ADD(
		CHOKE_DESC	CHAR(6));

	ALTER TABLE WELL_PRESS_AOF ADD(
		TREAT_PRESS	NUMBER(5));

	ALTER TABLE WELL_PRESS_AOF ADD(
		INST_SHUT_PRESS	NUMBER(5));
DOC;
No further changes to WELL_PRESS_AOF are needed.

WELL_PRESS_BHP - The following changes resulted from the 1/19 meeting:
	Remove column RD_PRESS
#
	ALTER TABLE WELL_PRESS_BHP ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_PRESS_BHP ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_PRESS_BHP ADD(
		TUBING_SIZE_DESC	CHAR(12));

	ALTER TABLE WELL_PRESS_BHP ADD(
		PACKER_DEPTH	NUMBER(7,2));

	ALTER TABLE WELL_PRESS_BHP ADD(
		DURATION	NUMBER(7,2));

	ALTER TABLE WELL_PRESS_BHP ADD(
		DURATION_UNIT	CHAR(12));

	ALTER TABLE WELL_PRESS_BHP ADD(
		SEA_LEVEL_DATUM	CHAR(5));

	ALTER TABLE WELL_PRESS_BHP ADD(
		BHFP		NUMBER(5));
DOC;
No further changes to WELL_PRESS_BHP are needed.

WELL_PROD_FIELD_DATA - This table is specific to PI data and, while the 
data in this table can probably be put into WELL_PROD_ZONE_HDR and 
WELL_PROD_ZONE_PAY, there is no way to correlate the data reported here 
with the other producing zone data reported in other PI cards.  Since 
it is already part of the Finder Schema, I propose to leave it in, but it 
might be more appropriate to make it a loader table and figure out how to 
put it in WELL_PROD_ZONE_HDR later. Add test type to allow it to accomodate 
data from different tests. 
# 
	ALTER TABLE WELL_PROD_FIELD_DATA ADD (
		TEST_TYPE	CHAR(12)); 
DOC;
WELL_TOUR_OCC - 6-90 UPDATE...
#
	ALTER TABLE WELL_TOUR_OCC MODIFY(MUD_DENSITY NUMBER(9,4));

	ALTER TABLE WELL_TOUR_OCC MODIFY(FINAL_MUD_DENSITY NUMBER(9,4));

DOC;
WELL_TEST_NAR - Add source field so more than one persons comments can 
be entered. 
# 
	ALTER TABLE WELL_TEST_NAR ADD ( 
		SOURCE 		CHAR(12)); 

DOC;
WELL_TEST_SHUTOFF - A source field should be added to flag new data. Since
WELL_CMPT_SHUTOFF was never blessed for use in Finder, it will not be
copied into this table.  WELLSCAN USERS WILL HAVE TO MOVE WELL_CMPT_SHUTOFF
BY HAND, IF THE TABLE EXISTS WITH DATA IN IT. 
Well_prod_shutoff will be moved...
#
	ALTER TABLE WELL_TEST_SHUTOFF ADD(
		SOURCE		CHAR(12));

	INSERT INTO WELL_TEST_SHUTOFF (
		UWI,
		SOURCE,
		TEST_TYPE,
		TEST_NO,
		SHUTOFF_TYPE,
		TOP,
		BASE)
	SELECT
		UWI,
		'PROD SHUTOFF',
		TEST_TYPE,
		TEST_NO,
		SHUTOFF_TYPE,
		TOP,
		BASE
	FROM WELL_PROD_SHUTOFF;
COMMIT;
DOC;
WELL_TEXAS_LOC - These fields were simply not wide enough to hold the
data: 
# 
	ALTER TABLE WELL_TEXAS_LOC MODIFY (
		ABSTRACT_NUMBER CHAR(7),
		SURVEY		CHAR(18)); 
DOC;
WELL_TOPS - 1/19 Change to add a faulted field...
#
	ALTER TABLE WELL_TOPS ADD(
		FAULTED		CHAR(1));
DOC;
WELL_WTR_ANAL_HDR - 1/19 Changes include:
	Remove these fields...
		WA_OBS_NO
		DST_NO
	Add the following:
#	
	ALTER TABLE WELL_WTR_ANAL_HDR ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(
		TEST_TYPE	CHAR(12));

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(
		REMARKS		CHAR(240));
DOC;
Update existing tables to the new schema...
#
	UPDATE WELL_WTR_ANAL_HDR SET
		 TEST_NO = WA_OBS_NO;
COMMIT;
DOC;
6/90 UPDATE...
#
	ALTER TABLE WELL_WTR_ANAL_HDR MODIFY(DENSITY NUMBER(9,4));
DOC;
No further changes to WELL_WTR_ANAL_HDR are needed.

WELL_WTR_ANAL_DETL - 1/19 Changes include the following
	Remove these fields...
		WA_OBS_NO
	Add the following:
#	
	ALTER TABLE WELL_WTR_ANAL_DETL ADD(
		TEST_NO		CHAR(4));

	ALTER TABLE WELL_WTR_ANAL_DETL ADD(
		TEST_TYPE	CHAR(12));
DOC;
Update existing tables to the new schema...
#
	UPDATE WELL_WTR_ANAL_DETL SET
		 TEST_NO = WA_OBS_NO;
COMMIT;
DOC;
No further changes to WELL_WTR_ANAL_DETL are needed.

DOC;
         TABLES CHANGED OR CREATED TO SUPPORT PI NPS DATA (PRODUCTION)

FIELD_PROD_HIST - New changes to support nps..
#
	ALTER TABLE FIELD_PROD_HIST ADD (
		FIELD_DISC_DATE	DATE);

	ALTER TABLE FIELD_PROD_HIST ADD (
		FIELD_NAME	CHAR(40));

	ALTER TABLE FIELD_PROD_HIST ADD (SOURCE CHAR(12));
	
DOC;
WELL_PROD_HIST_MISC - New table to track text data associated with monthly
production figures.
#
	CREATE TABLE WELL_PROD_HIST_MISC (
		UWI CHAR(20) NOT NULL, 
		SOURCE CHAR(12) NOT NULL,       
		FLUID CHAR(12) NOT NULL,
		ATTRIBUTE CHAR(12) NOT NULL,
		YEAR NUMBER(4) NOT NULL,
		FIELD_DISC_DATE	DATE,
		FIELD_NAME	CHAR(40) NULL,
		PROD_ZONE CHAR(12) NOT NULL,    
		JAN CHAR(12) NULL,  
		FEB CHAR(12) NULL,  
		MAR CHAR(12) NULL,  
		APR CHAR(12) NULL,  
		MAY CHAR(12) NULL,  
		JUN CHAR(12) NULL,  
		JUL CHAR(12) NULL,  
		AUG CHAR(12) NULL,  
		SEP CHAR(12) NULL,  
		OCT CHAR(12) NULL,  
		NOV CHAR(12) NULL,  
		DEC CHAR(12) NULL);
DOC;
WELL_PROD_ZONE_HDR - The following changes are needed:

	Remove field ONPROD
	then:
#
	ALTER TABLE WELL_PROD_ZONE_HDR ADD(
		OIL_ONPROD	DATE,
		GAS_ONPROD	DATE,
		WATER_ONPROD	DATE,
		WELL_SERIAL	NUMBER(8,0),
		UNIT_ACREAGE	NUMBER(12,2));

	ALTER TABLE WELL_PROD_ZONE_HDR MODIFY(
		COMINGLED	CHAR(4));

DOC;
          UPDATE TABLES WITH NODE IDS TO NEW NUMERIC NODE_ID

NOTE: THIS PROCEDURE ASSUMES EACH WELL HAS A DISTINCT NODE_ID.  ANY SHARED
NODES WILL ONLY REFERENCE THE LAST WELL THAT REFERRED TO THAT NODE WHEN
THIS PROCEDURE IS DONE!

First make sure we can join the two tables after generating node_id 
sequence numbers...

***************Rename original tables  to old_ tables...

NODES first...
#
	RENAME NODES TO OLD_NODES;
DOC;
GULF CANADA - COMMENT OUT ABOVE STATEMENT AND Drop view NODES and create the
table from BASE_NODES...
#
	DROP VIEW NODES;

CREATE TABLE NODES(                          NODE_ID CHAR(20) NOT NULL,
                                             NODE_X NUMBER(15,7) NOT NULL, 
                                             NODE_Y NUMBER(15,7) NOT NULL, 
                                             LOC_QUAL CHAR(1) NOT NULL,    
                                             LATITUDE NUMBER(10,7) NULL,   
                                             LONGITUDE NUMBER(10,6) NULL,  
                                             SOURCE CHAR(12) NULL,         
                                             DATUM CHAR(12) NULL);
                                                                                
INSERT INTO NODES(                        NODE_ID,                              
                                          NODE_X,                               
                                          NODE_Y,                               
                                          LOC_QUAL,                             
                                          LATITUDE,                             
                                          LONGITUDE,                            
                                          SOURCE,                               
                                          DATUM)
                                   SELECT
                                          NODE_ID,                              
                                          NODE_X,                               
                                          NODE_Y,                               
                                          LOC_QUAL,                             
                                          LATITUDE,                             
                                          LONGITUDE,                            
                                          SOURCE,                               
                                          DATUM                                
                                   FROM BASE_NODES;
COMMIT;
DOC;
NODES_SECOND,  This may not exist in older projects...
#
	RENAME NODES_SECOND TO OLD_NODES_SECOND;
DOC;
WELL_HDR...
#
	RENAME WELL_HDR TO OLD_WELL_HDR;
DOC;
WELL_HDR_SECOND ...
#
	RENAME WELL_HDR_SECOND TO OLD_WELL_HDR_SECOND;
DOC;
WELL_LOCATIONS...
#
	RENAME WELL_LOCATIONS TO OLD_WELL_LOCATIONS;
DOC;
WELL_TEXAS_LOC...
#
	RENAME WELL_TEXAS_LOC TO OLD_WELL_TEXAS_LOC;
DOC;
DRILLING_PLATFORM_HDR...
#
	RENAME DRILLING_PLATFORM_HDR TO OLD_DRILLING_PLATFORM_HDR;
DOC;
Next rename text to old_text...
#
	RENAME TEXT TO OLD_TEXT;

DOC;
*****************Add a UWI field to join with well_hdr later...

NODES..., Also add new fields(LOC_QUAL)...
#
	ALTER TABLE OLD_NODES ADD (UWI CHAR(20) NULL);
	ALTER TABLE OLD_NODES ADD (NEW_NODE_ID NUMBER(10) NULL);
	ALTER TABLE OLD_NODES ADD (LOC_QUAL CHAR(1) NULL);
	ALTER TABLE OLD_NODES_SECOND ADD (UWI CHAR(20) NULL);
	ALTER TABLE OLD_NODES_SECOND ADD (NEW_NODE_ID NUMBER(10) NULL);
	ALTER TABLE OLD_NODES_SECOND ADD (LOC_QUAL CHAR(1) NULL);
DOC;
WELL_LOCATIONS...
#
	ALTER TABLE OLD_WELL_LOCATIONS ADD (UWI CHAR(20) NULL);
DOC;
WELL_TEXAS_LOC...
#
	ALTER TABLE OLD_WELL_TEXAS_LOC ADD (UWI CHAR(20) NULL);
DOC;
DRILLING_PLATFORM_HDR...
#
	ALTER TABLE OLD_DRILLING_PLATFORM_HDR ADD(NEW_PLAT_NODE NUMBER(10)NULL);
DOC;
**********************Make node_id a null field...
#
	ALTER TABLE OLD_NODES MODIFY (NODE_ID CHAR(20) NULL);
	ALTER TABLE OLD_NODES_SECOND MODIFY (NODE_ID CHAR(20) NULL);
	ALTER TABLE OLD_WELL_LOCATIONS MODIFY (NODE_ID CHAR(20) NULL);
	ALTER TABLE OLD_WELL_TEXAS_LOC MODIFY (NODE_ID CHAR(20) NULL);
DOC;
***********************Fill uwi from well_hdr...

First NODES...
#
	UPDATE OLD_NODES N SET 
		UWI = (SELECT UWI FROM OLD_WELL_HDR H 
			WHERE H.NODE_ID = N.NODE_ID)
		WHERE N.NODE_ID NOT LIKE 'TXT%';
COMMIT;
DOC;
NODES_SECOND...This may not be present in older projects...
#
	UPDATE OLD_NODES_SECOND N SET 
		UWI = (SELECT DISTINCT UWI FROM OLD_WELL_HDR_SECOND H 
			WHERE H.NODE_ID = N.NODE_ID);
COMMIT;
DOC;
Next WELL_LOCATIONS...
#
	UPDATE OLD_WELL_LOCATIONS L SET 
		UWI = (SELECT UWI FROM OLD_WELL_HDR H 
			WHERE H.NODE_ID = L.NODE_ID);
COMMIT;
DOC;
NEXT WELL_TEXAS_LOC...
#
	UPDATE OLD_WELL_TEXAS_LOC T SET 
		UWI = (SELECT UWI FROM OLD_WELL_HDR H 
			WHERE H.NODE_ID = T.NODE_ID);
COMMIT;
DOC;
***************Next generate node_id's from the sequencer for each well...

WELL_HDR FIRST...
#
	UPDATE OLD_WELL_HDR SET
		NODE_ID = ESI.NODE_ID_SEQ.NEXTVAL;
COMMIT;
DOC;
WELL_HDR_SECOND...
#
	UPDATE OLD_WELL_HDR_SECOND SET
		NODE_ID = ESI.NODE_ID_SEQ.NEXTVAL;
COMMIT;
DOC;
******************Next update the node_id fields with the new node_id...

NODES first...
#
	UPDATE OLD_NODES N SET 
		NEW_NODE_ID = (SELECT NODE_ID FROM OLD_WELL_HDR H 
			WHERE H.UWI = N.UWI)
		WHERE N.NODE_ID NOT LIKE 'TXT%';
DOC;
Generate sequence numbers for the TEXT node_ids...
#
	UPDATE OLD_NODES SET NEW_NODE_ID = ESI.NODE_ID_SEQ.NEXTVAL
		WHERE NODE_ID LIKE 'TXT%';
DOC;
Generate sequence numbers for the rest of the nodes...

NOTE: IF THIS UPDATE ACTUALLY PROCESSES ANY RECORDS, THEY ARE NOT
ASSOCIATED WITH WELLS OR TEXT AND YOU SHOULD DETERMINE WHERE THEY
CAME FROM OR WHAT THEY ARE ASSOCIATED WITH BEFORE DROPPING THESE OLD
TABLES...
#
	UPDATE OLD_NODES N SET
                NEW_NODE_ID = ESI.NODE_ID_SEQ.NEXTVAL
                WHERE N.NEW_NODE_ID IS NULL;
COMMIT;
DOC;
NODES_SECOND...This may not exist in some older projects...
#
	UPDATE OLD_NODES_SECOND N SET 
		NEW_NODE_ID = (SELECT NODE_ID FROM OLD_WELL_HDR_SECOND H 
			WHERE H.UWI = N.UWI AND H.PRIMARY_SOURCE = N.SOURCE);
COMMIT;
DOC;
WELL_LOCATIONS...
#
	UPDATE OLD_WELL_LOCATIONS L SET 
		NODE_ID = (SELECT NODE_ID FROM OLD_WELL_HDR H 
			WHERE H.UWI = L.UWI);
COMMIT;
DOC;
WELL_TEXAS_LOC...
#
	UPDATE OLD_WELL_TEXAS_LOC L SET 
		NODE_ID = (SELECT NODE_ID FROM OLD_WELL_HDR H 
			WHERE H.UWI = L.UWI);
COMMIT;
DOC;
Next update the position field of the TEXT table...

First create a new field to temporarily store the new node_id without losing the
old one...
#
	ALTER TABLE OLD_TEXT ADD(NEW_POSITION NUMBER(10) NULL);

	ALTER TABLE OLD_TEXT MODIFY(POSITION CHAR(20) NULL);
DOC;
Now update the position field....
#
	UPDATE OLD_TEXT T SET
		NEW_POSITION = (SELECT NEW_NODE_ID FROM OLD_NODES
			WHERE NODE_ID LIKE 'TXT%'
			AND SUBSTR(NODE_ID,5) = T.POSITION);
COMMIT;
DOC;
update drilling_platform_hdr with correct node_ids...
#
	UPDATE OLD_DRILLING_PLATFORM_HDR P SET
		NEW_PLAT_NODE = (SELECT NEW_NODE_ID FROM OLD_NODES N
				WHERE N.NODE_ID = P.PLATFORM_NODE);
DOC;

*****************Next create tables with the correct node_id field...
#

	CREATE TABLE NODES (NODE_ID NUMBER(10) NOT NULL,
                       NODE_X NUMBER(15,7) NOT NULL,    
                       NODE_Y NUMBER(15,7) NOT NULL,    
                       LOC_QUAL CHAR(1) NOT NULL,       
                       LATITUDE NUMBER(10,7) NULL,      
                       LONGITUDE NUMBER(10,6) NULL,     
                       SOURCE CHAR(12) NULL,            
                       DATUM CHAR(12) NULL);
DOC;
Next create WELL_HDR with the correct node_id field...
#                                                        
                                                        
CREATE TABLE WELL_HDR(UWI CHAR(20) NOT NULL,           
                       WELL_NAME CHAR(50) NULL,         
                       WELL_NUMBER CHAR(20) NULL,       
                       PRIMARY_SOURCE CHAR(12) NOT NULL,
                       NODE_ID NUMBER(10) NULL,         
                       TOP_DELTA_X NUMBER(15,7) NULL,   
                       BASE_DELTA_X NUMBER(15,7) NULL,  
                       TOP_DELTA_Y NUMBER(15,7) NULL,   
                       BASE_DELTA_Y NUMBER(15,7) NULL,  
                       CLASS CHAR(12) NULL,             
                       OPERATOR CHAR(12) NULL,          
                       PLOT_SYMBOL CHAR(12) NULL,       
                       ELEVATION NUMBER(7,2) NULL,      
                       ELEVATION_REF CHAR(4) NULL,      
                       GROUND_ELEVATION NUMBER(7,2) NULL,
                       KB_ELEVATION NUMBER(7,2) NULL,    
                       PLOT_NAME CHAR(20) NULL,          
                       DRILLERS_TD NUMBER(7,2) NULL,     
                       FORM_AT_TD CHAR(12) NULL,         
                       TD_AGE_CODE CHAR(6) NULL,         
                       TVD NUMBER(7,2) NULL,             
                       CRSTATUS CHAR(12) NULL,           
                       ORSTATUS CHAR(12) NULL,           
                       PRSTATUS CHAR(12) NULL,           
                       SPUD_DATE DATE NULL,              
                       RIGREL DATE NULL,                 
                       COMP_DATE DATE NULL,              
                       ONPROD DATE NULL,                 
                       CALC_ONPROD DATE NULL,            
                       ONINJECT DATE NULL,               
                       LOG_TD NUMBER(7,2) NULL,          
                       STATUS_DATE DATE NULL,            
                       COUNTRY CHAR(3) NULL,             
                       PROV_ST CHAR(2) NULL,             
                       COUNTY CHAR(20) NULL,             
                       PLUGBACK_TD NUMBER(7,2) NULL,     
                       WATER_DEPTH NUMBER(7,2) NULL,     
                       CONF_REL_DT DATE NULL,            
                       DEVIATION_FLAG CHAR(1) NULL,      
                       CF_ELEVATION NUMBER(7,2) NULL,    
                       WATER_REF CHAR(4) NULL,           
                       PROPRIETARY CHAR(1) NULL,         
                       ORIG_UNIT CHAR(12) NULL,          
                       CONF_FLAG CHAR(1) NULL,           
                       CONF_FORM CHAR(12) NULL,          
                       DISCOVER_WELL CHAR(1) NULL,       
                       CONF_DEPTH NUMBER(7,2) NULL,      
                       FIELDCODE CHAR(12) NULL,          
                       FIN_DRILL DATE NULL,              
                       FLDPOOLWILD CHAR(12) NULL,        
                       FAULT CHAR(1) NULL,               
                       SHORT_NAME CHAR(20) NULL,         
                       LAST_UPDATE DATE NULL,            
                       LAST_UPTIME DATE NULL,            
                       LICENSEE CHAR(12) NULL,           
                       AGENT CHAR(12) NULL,              
                       CONFID_INDEX NUMBER(3) NULL,      
                       CONFID_INDEX_RATIO NUMBER(4,2) NULL,
                       CONFID_INDEX_LEVEL NUMBER(2) NULL,
                       STRAT_COLUMN CHAR(12) NULL,       
                       LEASE_NO CHAR(12) NULL,           
                       LEASE_NAME CHAR(40) NULL,         
                       DISTRICT CHAR(16) NULL,           
                       GOVT_ASSIGNED_NO CHAR(12) NULL,   
                       LOCATION_TABLE CHAR(30) NULL,     
                       STORAGE_UNIT CHAR(12) NULL);
DOC;
CREATE NEW WELL_LOCATIONS TABLE...
#
CREATE TABLE WELL_LOCATIONS(     NODE_ID NUMBER(10) NOT NULL,        
                                 SOURCE CHAR(12) NOT NULL,           
                                 TOWNSHIP NUMBER(3) NULL,            
                                 NORTH_SOUTH CHAR(1) NULL,           
                                 RANGE NUMBER(2) NULL,               
                                 EAST_WEST CHAR(1) NULL,             
                                 SECTION NUMBER(3) NULL,             
                                 SECTION_TYPE CHAR(4) NULL,          
                                 SPOT_CODE CHAR(10) NULL,            
                                 PRINCIPAL_MERIDIAN NUMBER(3) NULL,  
                                 NS_FOOTAGE NUMBER(7,2) NULL,        
                                 NS CHAR(3) NULL,                    
                                 EW_FOOTAGE NUMBER(7,2) NULL,        
                                 EW CHAR(3) NULL,           
                                 DESCRIPTION CHAR(240) NULL);
DOC;
Create new well_hdr_second...
#
CREATE TABLE WELL_HDR_SECOND(    UWI CHAR(20) NOT NULL,                                                                             
                                 WELL_NAME CHAR(50) NULL,                                                                           
                                 WELL_NUMBER CHAR(20) NULL,                                                                         
                                 SOURCE CHAR(12) NOT NULL,                                                                          
                                 NODE_ID NUMBER(10) NULL,                                                                           
                                 TOP_DELTA_X NUMBER(15,7) NULL,                                                                     
                                 BASE_DELTA_X NUMBER(15,7) NULL,                                                                    
                                 TOP_DELTA_Y NUMBER(15,7) NULL,                                                                     
                                 BASE_DELTA_Y NUMBER(15,7) NULL,                                                                    
                                 CLASS CHAR(12) NULL,                                                                               
                                 OPERATOR CHAR(12) NULL,                                                                            
                                 GROUND_ELEVATION NUMBER(7,2) NULL,                                                                 
                                 CRSTATUS CHAR(12) NULL,                                                                            
                                 PLOT_SYMBOL CHAR(12) NULL,                                                                         
                                 KB_ELEVATION NUMBER(7,2) NULL,                                                                     
                                 PLOT_NAME CHAR(20) NULL,                                                                           
                                 DRILLERS_TD NUMBER(7,2) NULL,                                                                      
                                 FORM_AT_TD CHAR(12) NULL,                                                                          
                                 ELEVATION NUMBER(7,2) NULL,                                                                        
                                 ELEVATION_REF CHAR(4) NULL,                                                                        
                                 COMP_DATE DATE NULL,                                                                               
                                 LOCATION_TABLE CHAR(30) NULL,                                                                      
                                 LOG_TD NUMBER(7,2) NULL,                                                                           
                                 TD_AGE_CODE CHAR(6) NULL,                                                                          
                                 STATUS_DATE DATE NULL,                                                                             
                                 PRSTATUS CHAR(12) NULL,                                                                            
                                 ORSTATUS CHAR(12) NULL,                                                                            
                                 SPUD_DATE DATE NULL,                                                                               
                                 RIGREL DATE NULL,                                                                                  
                                 ONPROD DATE NULL,                                                                                  
                                 CALC_ONPROD DATE NULL,                                                                             
                                 ONINJECT DATE NULL,                                                                                
                                 COUNTRY CHAR(3) NULL,                                                                              
                                 PROV_ST CHAR(2) NULL,                                                                              
                                 COUNTY CHAR(20) NULL,                                                                              
                                 TVD NUMBER(7,2) NULL,                                                                              
                                 PLUGBACK_TD NUMBER(7,2) NULL,                                                                      
                                 WATER_DEPTH NUMBER(7,2) NULL,                                                                      
                                 DEVIATION_FLAG CHAR(1) NULL,                                                                       
                                 WATER_REF CHAR(4) NULL,                                                                            
                                 CF_ELEVATION NUMBER(7,2) NULL,                                                                     
                                 PROPRIETARY CHAR(1) NULL,                                                                          
                                 ORIG_UNIT CHAR(12) NULL,                                                                           
                                 CONF_FLAG CHAR(1) NULL,                                                                            
                                 CONF_FORM CHAR(12) NULL,                                                                           
                                 DISCOVER_WELL CHAR(1) NULL,                                                                        
                                 CONF_DEPTH NUMBER(7,2) NULL,                                                                       
                                 FIELDCODE CHAR(12) NULL,                                                                           
                                 CONF_REL_DT DATE NULL,                                                                             
                                 FIN_DRILL DATE NULL,                                                                               
                                 FLDPOOLWILD CHAR(12) NULL,                                                                         
                                 FAULT CHAR(1) NULL,                                                                                
                                 SHORT_NAME CHAR(20) NULL,                                                                          
                                 LAST_UPDATE DATE NULL,                                                                             
                                 LAST_UPTIME DATE NULL,                                                                             
                                 LICENSEE CHAR(12) NULL,                                                                            
                                 AGENT CHAR(12) NULL,                                                                               
                                 CONFID_INDEX_LEVEL NUMBER(2) NULL,
                                 CONFID_INDEX NUMBER(3) NULL,                                                                       
                                 CONFID_INDEX_RATIO NUMBER(4,2) NULL,                                                               
                                 LEASE_NO CHAR(12) NULL,                                                                            
                                 LEASE_NAME CHAR(40) NULL,                                                                          
                                 DISTRICT CHAR(16) NULL,                                                                            
                                 GOVT_ASSIGNED_NO CHAR(12) NULL,                                                                    
                                 STRAT_COLUMN CHAR(12) NULL,                                                                        
                                 STORAGE_UNIT CHAR(12) NULL);
                                                                                                                                  
DOC;
Now create text with numeric position field...
#

CREATE TABLE TEXT(
  TEXT_ID NUMBER NOT NULL,                                                                                                          
  POSITION NUMBER(10) NOT NULL,                                                                                                      
  TEXT_STRING CHAR(240) NOT NULL,                                                                                                    
  X_OFFSET NUMBER(15,7)  NULL,                                                                                                      
  Y_OFFSET NUMBER(15,7)  NULL,                                                                                                      
  ANNOTATION_CLASS CHAR(24) NULL,                                                                                                    
  ANNOTATION_SUPERCLASS CHAR(24) NULL,                                                                                               
  TEXT_PATH CHAR(12) NULL,                                                                                                           
  BOX_FLAG CHAR(3) NULL,                                                                                                             
  BOX_HATCH_CODE CHAR(12) NULL,                                                                                                      
  BOX_FILL_COLOR CHAR(15) NULL,                                                                                                      
  BOX_LINE_STYLE CHAR(15) NULL,                                                                                                      
  BOX_LINE_THICKNESS CHAR(12) NULL,                                                                                                  
  ARROW_FLAG CHAR(3) NULL,                                                                                                           
  TEXT_FONT CHAR(20) NULL,                                                                                                           
  TEXT_COLOR CHAR(15) NULL,                                                                                                          
  TEXT_PRECISION CHAR(12) NULL,                                                                                                      
  TEXT_SIZE NUMBER NULL,                                                                                                            
  TEXT_EXPANSION_FACTOR NUMBER NULL,                                                                                                
  LINE_SPACING NUMBER NULL,                                                                                                         
  HORIZONTAL_ALIGNMENT CHAR(12) NULL,                                                                                                
  VERTICAL_ALIGNMENT CHAR(12) NULL,                                                                                                  
  UP_VECTOR_X NUMBER NULL,                                                                                                          
  UP_VECTOR_Y NUMBER NULL);                                                                                                          

DOC;
Create new nodes_second table...
# 
CREATE TABLE NODES_SECOND(       NODE_ID NUMBER(10) NOT NULL,                                                                       
                                 NODE_X NUMBER(15,7) NOT NULL,                                                                      
                                 NODE_Y NUMBER(15,7) NOT NULL,                                                                      
                                 LOC_QUAL CHAR(1) NOT NULL,                                                                         
                                 LATITUDE NUMBER(10,7) NULL,                                                                        
                                 LONGITUDE NUMBER(10,6) NULL,                                                                       
                                 SOURCE CHAR(12) NOT NULL,                                                                          
                                 DATUM CHAR(12) NULL);                                                                               
DOC;
Create new well_texas_loc table...
#
CREATE TABLE WELL_TEXAS_LOC(     NODE_ID NUMBER(10) NOT NULL,                                                                       
                                 SOURCE CHAR(12) NOT NULL,                                                                          
                                 SURVEY CHAR(18) NULL,                                                                              
                                 BLOCK CHAR(3) NULL,                                                                                
                                 ABSTRACT_NUMBER CHAR(12) NULL,                                                                     
                                 LOCATION_DESCRIPTION CHAR(240) NULL);
                                                                                                                                    
DOC;
CREATE NEW DRILLING_PLATFORM_HDR...
#
CREATE TABLE DRILLING_PLATFORM_HDR(NAME CHAR(30) NOT NULL,                                                                            
                                 PLATFORM_TYPE CHAR(30) NULL,                                                                       
                                 WATER_DEPTH NUMBER(7,2) NULL,                                                                      
                                 DATE_INSTALLED DATE NULL,                                                                          
                                 PLATFORM_NODE NUMBER(10) NULL,                                                                     
                                 LOCATION_REF CHAR(15) NULL,                                                                        
                                 DATE_REMOVED DATE NULL,                                                                            
                                 OCS_BLOCK CHAR(10) NULL,                                                                           
                                 UTM_QUAD CHAR(8) NULL,                                                                             
                                 UTM_GRID_BLOCK_NS NUMBER NULL,                                                                     
                                 UTM_GRID_BLOCK_EW NUMBER NULL,                                                                     
                                 JURISDICTION CHAR(1) NULL);                                                                         
                                                                                                                                    
                                                                                                                                    
                                                                                                                                    

DOC;
******* BEGIN MOVING DATA TO NEW TABLES...

Insert data into  WELL_HDR...
#
	INSERT INTO WELL_HDR(
			UWI,
                       WELL_NAME,
                       WELL_NUMBER,
                       PRIMARY_SOURCE,
                       NODE_ID,
                       TOP_DELTA_X,
                       BASE_DELTA_X,
                       TOP_DELTA_Y,
                       BASE_DELTA_Y,
                       CLASS,
                       OPERATOR,
                       PLOT_SYMBOL,
                       ELEVATION,
                       ELEVATION_REF,
                       GROUND_ELEVATION,
                       KB_ELEVATION,
                       PLOT_NAME,
                       DRILLERS_TD,
                       FORM_AT_TD,
                       TD_AGE_CODE,
                       TVD,
                       CRSTATUS,
                       ORSTATUS,
                       PRSTATUS,
                       SPUD_DATE,
                       RIGREL,
                       COMP_DATE,
                       ONPROD,
                       CALC_ONPROD,
                       ONINJECT,
                       LOG_TD,
                       STATUS_DATE,
                       COUNTRY,
                       PROV_ST,
                       COUNTY,
                       PLUGBACK_TD,
                       WATER_DEPTH,
                       CONF_REL_DT,
                       DEVIATION_FLAG,
                       CF_ELEVATION,
                       WATER_REF,
                       PROPRIETARY,
                       ORIG_UNIT,
                       CONF_FLAG,
                       CONF_FORM,
                       DISCOVER_WELL,
                       CONF_DEPTH,
                       FIELDCODE,
                       FIN_DRILL,
                       FLDPOOLWILD,
                       FAULT,
                       SHORT_NAME,
                       LAST_UPDATE,
                       LICENSEE,
                       AGENT,
                       CONFID_INDEX,
                       CONFID_INDEX_RATIO,
                       CONFID_INDEX_LEVEL,
                       LOCATION_TABLE)
		SELECT
			UWI,
                       WELL_NAME,
                       WELL_NUMBER,
                       PRIMARY_SOURCE,
                       TO_NUMBER(NODE_ID),
                       TOP_DELTA_X,
                       BASE_DELTA_X,
                       TOP_DELTA_Y,
                       BASE_DELTA_Y,
                       CLASS,
                       OPERATOR,
                       PLOT_SYMBOL,
                       ELEVATION,
                       ELEVATION_REF,
                       GROUND_ELEVATION,
                       KB_ELEVATION,
                       PLOT_NAME,
                       DRILLERS_TD,
                       FORM_AT_TD,
                       TD_AGE_CODE,
                       TRUE_VERTICAL_DEPTH,
                       CRSTATUS,
                       ORSTATUS,
                       PRSTATUS,
                       SPUD_DATE,
                       RIGREL,
                       COMP_DATE,
                       ONPROD,
                       CALC_ONPROD,
                       ONINJECT,
                       LOG_TD,
                       STATUS_DATE,
                       COUNTRY,
                       PROV_ST,
                       COUNTY,
                       PLUGBACK_TD,
                       WATER_DEPTH,
                       CONF_REL_DT,
                       DEVIATION_FLAG,
                       CF_ELEVATION,
                       WATER_REF,
                       PROPRIETARY,
                       ORIG_UNIT,
                       CONF_FLAG,
                       CONF_FORM,
                       DISCOVER_WELL,
                       CONF_DEPTH,
                       FIELDCODE,
                       FIN_DRILL,
                       FLDPOOLWILD,
                       FAULT,
                       SHORT_NAME,
                       LAST_UPDATE,
                       LICENSEE,
                       AGENT,
                       CONFID_INDEX,
                       CONFID_INDEX_RATIO,
                       CONFID_INDEX_LEVEL,
                       LOCATION_TABLE
		FROM OLD_WELL_HDR;
COMMIT;
DOC;
Insert data into well_locations...
#
INSERT INTO WELL_LOCATIONS(      NODE_ID,             
                                 SOURCE,              
                                 TOWNSHIP,            
                                 NORTH_SOUTH,         
                                 RANGE,               
                                 EAST_WEST,           
                                 SECTION,             
                                 SECTION_TYPE,        
                                 SPOT_CODE,           
                                 PRINCIPAL_MERIDIAN,  
                                 NS_FOOTAGE,          
                                 NS,                  
                                 EW_FOOTAGE,          
                                 EW,                  
                                 DESCRIPTION)         
			SELECT
                                 TO_NUMBER(NODE_ID),         
                                 SOURCE,              
                                 TOWNSHIP,            
                                 NORTH_SOUTH,         
                                 RANGE,               
                                 EAST_WEST,           
                                 SECTION,             
                                 SECTION_TYPE,        
                                 SPOT_CODE,           
                                 PRINCIPAL_MERIDIAN,  
                                 NS_FOOTAGE,          
                                 NS,                  
                                 EW_FOOTAGE,          
                                 EW,                  
                                 DESCRIPTION
			FROM OLD_WELL_LOCATIONS;
DOC;
insert into well_hdr_second...
#
INSERT INTO WELL_HDR_SECOND(     UWI,                                                                                               
                                 WELL_NAME,                                                                                         
                                 WELL_NUMBER,                                                                                       
                                 SOURCE,                                                                                            
                                 NODE_ID,                                                                                           
                                 TOP_DELTA_X,                                                                                       
                                 BASE_DELTA_X,                                                                                      
                                 TOP_DELTA_Y,                                                                                       
                                 BASE_DELTA_Y,                                                                                      
                                 CLASS,                                                                                             
                                 OPERATOR,                                                                                          
                                 GROUND_ELEVATION,                                                                                  
                                 CRSTATUS,                                                                                          
                                 PLOT_SYMBOL,                                                                                       
                                 KB_ELEVATION,                                                                                      
                                 PLOT_NAME,                                                                                         
                                 DRILLERS_TD,                                                                                       
                                 FORM_AT_TD,                                                                                        
                                 ELEVATION,                                                                                         
                                 ELEVATION_REF,                                                                                     
                                 COMP_DATE,                                                                                         
                                 LOCATION_TABLE,                                                                                    
                                 LOG_TD,                                                                                            
                                 TD_AGE_CODE,                                                                                       
                                 STATUS_DATE,                                                                                       
                                 PRSTATUS,                                                                                          
                                 ORSTATUS,                                                                                          
                                 SPUD_DATE,                                                                                         
                                 RIGREL,                                                                                            
                                 ONPROD,                                                                                            
                                 CALC_ONPROD,                                                                                       
                                 ONINJECT,                                                                                          
                                 COUNTRY,                                                                                           
                                 PROV_ST,                                                                                           
                                 COUNTY,                                                                                            
                                 TVD,                                                                                               
                                 PLUGBACK_TD,                                                                                       
                                 WATER_DEPTH,                                                                                       
                                 DEVIATION_FLAG,                                                                                    
                                 WATER_REF,                                                                                         
                                 CF_ELEVATION,                                                                                      
                                 PROPRIETARY,                                                                                       
                                 ORIG_UNIT,                                                                                         
                                 CONF_FLAG,                                                                                         
                                 CONF_FORM,                                                                                         
                                 DISCOVER_WELL,                                                                                     
                                 CONF_DEPTH,                                                                                        
                                 FIELDCODE,                                                                                         
                                 CONF_REL_DT,                                                                                       
                                 FIN_DRILL,                                                                                         
                                 FLDPOOLWILD,                                                                                       
                                 FAULT,                                                                                             
                                 SHORT_NAME,                                                                                        
                                 LAST_UPDATE,                                                                                       
                                 LICENSEE,                                                                                          
                                 AGENT,                                                                                             
                                 CONFID_INDEX,                                                                                      
                                 CONFID_INDEX_RATIO)
			SELECT
                                 UWI,                                                                                               
                                 WELL_NAME,                                                                                         
                                 WELL_NUMBER,                                                                                       
                                 SOURCE,                                                                                            
                                 TO_NUMBER(NODE_ID),                                                                                           
                                 TOP_DELTA_X,                                                                                       
                                 BASE_DELTA_X,                                                                                      
                                 TOP_DELTA_Y,                                                                                       
                                 BASE_DELTA_Y,                                                                                      
                                 CLASS,                                                                                             
                                 OPERATOR,                                                                                          
                                 GROUND_ELEVATION,                                                                                  
                                 CRSTATUS,                                                                                          
                                 PLOT_SYMBOL,                                                                                       
                                 KB_ELEVATION,                                                                                      
                                 PLOT_NAME,                                                                                         
                                 DRILLERS_TD,                                                                                       
                                 FORM_AT_TD,                                                                                        
                                 ELEVATION,                                                                                         
                                 ELEVATION_REF,                                                                                     
                                 COMP_DATE,                                                                                         
                                 LOCATION_TABLE,                                                                                    
                                 LOG_TD,                                                                                            
                                 TD_AGE_CODE,                                                                                       
                                 STATUS_DATE,                                                                                       
                                 PRSTATUS,                                                                                          
                                 ORSTATUS,                                                                                          
                                 SPUD_DATE,                                                                                         
                                 RIGREL,                                                                                            
                                 ONPROD,                                                                                            
                                 CALC_ONPROD,                                                                                       
                                 ONINJECT,                                                                                          
                                 COUNTRY,                                                                                           
                                 PROV_ST,                                                                                           
                                 COUNTY,                                                                                            
                                 TVD,                                                                                               
                                 PLUGBACK_TD,                                                                                       
                                 WATER_DEPTH,                                                                                       
                                 DEVIATION_FLAG,                                                                                    
                                 WATER_REF,                                                                                         
                                 CF_ELEVATION,                                                                                      
                                 PROPRIETARY,                                                                                       
                                 ORIG_UNIT,                                                                                         
                                 CONF_FLAG,                                                                                         
                                 CONF_FORM,                                                                                         
                                 DISCOVER_WELL,                                                                                     
                                 CONF_DEPTH,                                                                                        
                                 FIELDCODE,                                                                                         
                                 CONF_REL_DT,                                                                                       
                                 FIN_DRILL,                                                                                         
                                 FLDPOOLWILD,                                                                                       
                                 FAULT,                                                                                             
                                 SHORT_NAME,                                                                                        
                                 LAST_UPDATE,                                                                                       
                                 LICENSEE,                                                                                          
                                 AGENT,                                                                                             
                                 CONFID_INDEX,                                                                                      
                                 CONFID_INDEX_RATIO                                                                                
			FROM OLD_WELL_HDR_SECOND;
DOC;
INSERTING NODES_SECOND...
#
                                                                                                                                    
INSERT INTO NODES_SECOND(        NODE_ID,                                                                                           
                                 NODE_X,                                                                                            
                                 NODE_Y,                                                                                            
                                 LOC_QUAL,                                                                                          
                                 LATITUDE,                                                                                          
                                 LONGITUDE,                                                                                         
                                 SOURCE,                                                                                            
                                 DATUM)                                                                                             
			SELECT
                                 NEW_NODE_ID,                                                                                           
                                 NODE_X,                                                                                            
                                 NODE_Y,                                                                                            
                                 'A',                                                                                          
                                 LATITUDE,                                                                                          
                                 LONGITUDE,                                                                                         
                                 SOURCE,                                                                                            
                                 DATUM
			FROM OLD_NODES_SECOND;                                                                                             
                                                                                                                                    

DOC;
Next move the data from old_text into new table...
#
INSERT INTO TEXT(
    TEXT_ID,                                                                                                                        
    POSITION,                                                                                                                       
    TEXT_STRING,                                                                                                                    
    X_OFFSET,                                                                                                                       
    Y_OFFSET,                                                                                                                       
    ANNOTATION_CLASS,                                                                                                               
    ANNOTATION_SUPERCLASS,                                                                                                          
    TEXT_PATH,                                                                                                                      
    BOX_FLAG,                                                                                                                       
    BOX_HATCH_CODE,                                                                                                                 
    BOX_FILL_COLOR,                                                                                                                 
    BOX_LINE_STYLE,                                                                                                                 
    BOX_LINE_THICKNESS,                                                                                                             
    ARROW_FLAG,                                                                                                                     
    TEXT_FONT,                                                                                                                      
    TEXT_COLOR,                                                                                                                     
    TEXT_PRECISION,                                                                                                                 
    TEXT_SIZE,                                                                                                                      
    TEXT_EXPANSION_FACTOR,                                                                                                          
    LINE_SPACING,                                                                                                                   
    HORIZONTAL_ALIGNMENT,                                                                                                           
    VERTICAL_ALIGNMENT,                                                                                                             
    UP_VECTOR_X,                                                                                                                    
    UP_VECTOR_Y)                                                                                                                    
SELECT
    TEXT_ID,                                                                                                                        
    NEW_POSITION,                                                                                                                       
    TEXT_STRING,                                                                                                                    
    X_OFFSET,                                                                                                                       
    Y_OFFSET,                                                                                                                       
    ANNOTATION_CLASS,                                                                                                               
    ANNOTATION_SUPERCLASS,                                                                                                          
    TEXT_PATH,                                                                                                                      
    BOX_FLAG,                                                                                                                       
    BOX_HATCH_CODE,                                                                                                                 
    BOX_FILL_COLOR,                                                                                                                 
    BOX_LINE_STYLE,                                                                                                                 
    BOX_LINE_THICKNESS,                                                                                                             
    ARROW_FLAG,                                                                                                                     
    TEXT_FONT,                                                                                                                      
    TEXT_COLOR,                                                                                                                     
    TEXT_PRECISION,                                                                                                                 
    TEXT_SIZE,                                                                                                                      
    TEXT_EXPANSION_FACTOR,                                                                                                          
    LINE_SPACING,                                                                                                                   
    HORIZONTAL_ALIGNMENT,                                                                                                           
    VERTICAL_ALIGNMENT,                                                                                                             
    UP_VECTOR_X,                                                                                                                    
    UP_VECTOR_Y                                                                                                                    
FROM OLD_TEXT;

DOC;
Move the well_texas_loc table...
#
INSERT INTO WELL_TEXAS_LOC(      NODE_ID,                                                                                           
                                 SOURCE,                                                                                            
                                 SURVEY,                                                                                            
                                 BLOCK,                                                                                             
                                 ABSTRACT_NUMBER,                                                                                   
                                 LOCATION_DESCRIPTION)                                                                              
			SELECT
				 TO_NUMBER(NODE_ID),                                                                                           
                                 SOURCE,                                                                                            
                                 SURVEY,                                                                                            
                                 BLOCK,                                                                                             
                                 ABSTRACT_NUMBER,                                                                                   
                                 LOCATION_DESCRIPTION
			FROM OLD_WELL_TEXAS_LOC;                                                                              
DOC;
INSERT DRILLING_PLATFORM_HDR...
#
INSERT INTO DRILLING_PLATFORM_HDR( NAME,                                                                                              
                                 PLATFORM_TYPE,                                                                                     
                                 WATER_DEPTH,                                                                                       
                                 DATE_INSTALLED,                                                                                    
                                 PLATFORM_NODE,                                                                                     
                                 LOCATION_REF,                                                                                      
                                 DATE_REMOVED,                                                                                      
                                 OCS_BLOCK,                                                                                         
                                 UTM_QUAD,                                                                                          
                                 UTM_GRID_BLOCK_NS,                                                                                 
                                 UTM_GRID_BLOCK_EW,                                                                                 
                                 JURISDICTION)                                                                                      
                   SELECT
                                  NAME,                                                                                              
                                 PLATFORM_TYPE,                                                                                     
                                 WATER_DEPTH,                                                                                       
                                 DATE_INSTALLED,                                                                                    
                                 NEW_PLAT_NODE,                                                                                     
                                 LOCATION_REF,                                                                                      
                                 DATE_REMOVED,                                                                                      
                                 OCS_BLOCK,                                                                                         
                                 UTM_QUAD,                                                                                          
                                 UTM_GRID_BLOCK_NS,                                                                                 
                                 UTM_GRID_BLOCK_EW,                                                                                 
                                 JURISDICTION
			FROM OLD_DRILLING_PLATFORM_HDR;                                                                                      
                                                                                                                                    
                                                                                                                                    

DOC;
Lastly move the data into nodes...
#
	INSERT INTO NODES(
		       NODE_ID,
                       NODE_X,
                       NODE_Y,
                       LOC_QUAL,
                       LATITUDE,
                       LONGITUDE,
                       SOURCE,
                       DATUM)
		SELECT
		       NEW_NODE_ID,
                       NODE_X,
                       NODE_Y,
                       'A',
                       LATITUDE,
                       LONGITUDE,
                       SOURCE,
                       DATUM
		FROM OLD_NODES;
COMMIT;
DOC;
                              TABLES TO BE DELETED

The following tables should be deleted (dropped) once the above script runs
successfully. It is commented out at the moment so that you have a chance to
verify that everything worked satisfactorily before deleting the old data 
tables. These lines can be copied to another file and executed once you are 
happy with the above changes.

	DROP TABLE WELL_CASING;
	DROP TABLE OLD_WELL_TUBULARS;
	DROP TABLE WELL_CASING_OLD;
	DROP TABLE OLD_WELL_CASING;
	DROP TABLE WELL_TEST_FORMS_INT;
	DROP TABLE WELL_TESTS;
	DROP TABLE WELL_PROD_PERFS;
	DROP TABLE WELL_COMPLET_DETL;
	DROP TABLE WELL_DRILL_SHOW;
	DROP TABLE WELL_INIT_PROD_MAT;
	DROP TABLE WELL_INIT_PROD_REC;
	DROP TABLE WELL_POROSITY_ZONES;
	DROP TABLE WELL_PRODUCING_ZONES;
	DROP TABLE WELL_PROD_SHUTOFF;
	DROP TABLE WELL_PROD_TEST_MAT;
	DROP TABLE WELL_PROD_TEST_REC;
	DROP TABLE WELL_PROD_TREATMENT;
	DROP TABLE OLD_WELL_HDR;
	DROP TABLE OLD_NODES;
	DROP TABLE OLD_WELL_HDR_SECOND;
	DROP TABLE OLD_NODES_SECOND;
	DROP TABLE OLD_WELL_LOCATIONS;
	DROP TABLE OLD_TEXT;
	DROP TABLE OLD_WELL_TEXAS_LOC;
	DROP TABLE OLD_DRILLING_PLATFORM_HDR;

		FINAL SCHEMA CHANGES FROM JULY 1990

After a week of discussion on changes to the schema and issues related to
integrating the ATS products into Finder, a final set of incremental changes to
the above were agreed upon.  The remainder of the script that follows can be run
seperately and is included as  the file 7-15_schema_update.sql.  That file, not
this full script should be run on projects that have had a Finder update more
recently than the first of Feburary 1990.

DOC;
			7-15_SCHEMA_UPDATE.SQL

This file updates Finder projects created after February 1 1990 to the latest
schema.  It is hoped that with this release the schema will not need further
changes for quite some time and will be the basis for the next release of
Applied Terravision's products Wellscan and Prodscan as well.

The current changes consolidate some tables where similar data are being stored
and adds several new tables to better model how data are collected during
drilling and testing operations.

		CHANGES ALREADY MADE AND SHIPPED WITH 6/15 RELEASE

The changes below were shipped without having an update script because they were
benign changes updateable by DB_VER_TABS. They are documented here for
completeness. All of the changes below can be made simply by running
db_ver_tabs, but you might want to leave them uncommented so you get some
advance warning if there are any problems with fields updated by these
statements...

Change all density fields to be NUMBER (9,4) to allow room for unit conversions
using sql updates...
#
	ALTER TABLE WELL_TEST_VO_ANAL MODIFY(OIL_DENSITY NUMBER(9,4));

	ALTER TABLE WELL_WTR_ANAL_HDR MODIFY(DENSITY NUMBER(9,4));

	ALTER TABLE WELL_OIL_ANAL_HDR MODIFY(DENSITY NUMBER(9,4));

	ALTER TABLE WELL_TOUR_OCC MODIFY(MUD_DENSITY NUMBER(9,4));

	ALTER TABLE WELL_TOUR_OCC MODIFY(FINAL_MUD_DENSITY NUMBER(9,4));

	ALTER TABLE WELL_TUBULARS MODIFY(DENSITY NUMBER(9,4));

	ALTER TABLE WELL_LOG_RUN_HDR MODIFY(FLUID_DENSITY NUMBER(9,4));

	ALTER TABLE WELL_LOG_RUN_HDR MODIFY(LOG_MATRX_DEN NUMBER(9,4));
DOC;
Other changes that are benign as far as current software are concerned...

WELL_POROUS_INTRVL - Expand the gross_porosity field to allow for room to
massage data using sql updates...
#
	ALTER TABLE WELL_POROUS_INTRVL MODIFY(GROSS_POROSITY NUMBER(9,4));
DOC;
FIELD_PROD_HIST - Add a few columns to support the NPS data...
#
	ALTER TABLE FIELD_PROD_HIST ADD (SOURCE CHAR(12));
	
	ALTER TABLE FIELD_PROD_HIST ADD (FIELD_DISC_DATE DATE);
	
	ALTER TABLE FIELD_PROD_HIST ADD (FIELD_NAME CHAR(40));
DOC;

		ADDITIONAL CHANGES REQUIRED IN THIS UPDATE

These remaining changes were agreed upon for the 7/15 update. Most involve
renaming tables or columns and therefor have implications for forms,
loaders, reports, etc...

			TABLE CHANGES

WELL_TEST_VO_MAT - This table is actually modeling flow rates for the different
produced fluids, therefor a more descriptive name was decided upon for the
table...
#
	RENAME WELL_TEST_VO_MAT TO WELL_TEST_VO_FLOW;
DOC;
LOADER_TEST_VO_MAT - This table is actually modeling flow rates for the different
produced fluids, therefor a more descriptive name was decided upon for the
table...
#
	RENAME LOADER_TEST_VO_MAT TO LOADER_TEST_VO_FLOW;
DOC;
WELL_PROD_FLOW_RATE - These data can just as easily be put in WELL_TEST_VO_FLOW
and therefore this table is redundant and will be dropped.  For now, move the
data into WELL_TEST_VO_FLOW(Note: This table was flawed in its design in that
it did not have the TEST_NO AND TEST_TYPE fields required of test tables. The
statements below are provided in case someone actually put anything in this
table, but for most sites this can be left commented out)...

	INSERT INTO WELL_TEST_VO_FLOW(UWI,SOURCE,TEST_NO,VALVE_OPEN_NO,
			FLUID_TYPE,FLUID_RATE,FLUID_RATE_UNIT)
		SELECT UWI,SOURCE,OBS_NO,101,'OIL',OIL_RATE,OIL_RATE_UNIT
		FROM WELL_PROD_FLOW_RATE;

	INSERT INTO WELL_TEST_VO_FLOW(UWI,SOURCE,TEST_NO,VALVE_OPEN_NO,
			FLUID_TYPE,FLUID_RATE,FLUID_RATE_UNIT)
		SELECT UWI,SOURCE,OBS_NO,102,'GAS',GAS_RATE,GAS_RATE_UNIT
		FROM WELL_PROD_FLOW_RATE;

	INSERT INTO WELL_TEST_VO_FLOW(UWI,SOURCE,TEST_NO,VALVE_OPEN_NO,
			FLUID_TYPE,FLUID_RATE,FLUID_RATE_UNIT)
		SELECT UWI,SOURCE,OBS_NO,103,'WATER',WATER_RATE,WATER_RATE_UNIT
		FROM WELL_PROD_FLOW_RATE;
	COMMIT;
WELL_PROD_FIELD_DATA - Rename for DB2 compatibility...
#
	RENAME WELL_PROD_FIELD_DATA TO WELL_PROD_FLD_DAT;
DOC;
WELL_PROD_HIST_MISC - Rename for DB2 compatibility...
#
	RENAME WELL_PROD_HIST_MISC TO WELL_PROD_HIST_MSC;
DOC;
WELL_CORE_LINE - Rename to make table name more descriptive...
#
	RENAME WELL_CORE_LINE TO WELL_CORE_ANAL;
DOC;
LOADER_CORE_LINE - Rename to make table name more descriptive...
#
	RENAME LOADER_CORE_LINE TO LOADER_CORE_ANAL;
DOC;
WELL_FORM_TEST_MUD - This type of data could be associated with any kind of
test, so it is renamed to make it generic...
#
	RENAME WELL_FORM_TEST_MUD TO WELL_TEST_MUD;
DOC;
WELL_TEST_VO_PRESS - New table to track multiple pressure readings during a
given valve_open_period...
#
	CREATE TABLE WELL_TEST_VO_PRESS (
		UWI		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		TEST_NO		CHAR(4) NOT NULL,
		TEST_TYPE	CHAR(12) NULL,
		PRESS_TYPE	CHAR(12) NULL,
		PRESS_OBS_NO	NUMBER(2) NOT NULL,
		VALVE_OPEN_NO	NUMBER(3) NOT NULL,
		PRESSURE	NUMBER(5) NULL);
DOC;
WELL_SIDEWALL_HDR - New table to store sidewall core gun run information...
#
	CREATE TABLE WELL_SIDEWALL_HDR (
			UWI		CHAR(20) NOT NULL,
			SOURCE		CHAR(12) NOT NULL,
			SWC_RUN_NO	CHAR(12) NOT NULL,
			SWC_SHOTS	NUMBER(4) NULL,
			SWC_SHOTS_REC	NUMBER(4) NULL,
			REMARKS		CHAR(240) NULL);
DOC;
WELL_TEST_VO_CONT - New table to track contaminants in produced fluids during a
given valve open period...
#
	CREATE TABLE WELL_TEST_VO_CONT (
		UWI		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		TEST_TYPE	CHAR(12) NULL,
		TEST_NO		CHAR(4) NOT NULL,
		CONTAMINANT	CHAR(12) NOT NULL,
		REMARKS		CHAR(240) NULL);
DOC;
				COLUMN CHANGES

WELL_TEST_VO - Rename CHOKE_SIZE_DESC for DB2 compatibility...
#
	ALTER TABLE WELL_TEST_VO ADD (CHOKE_DESC CHAR(8));

	UPDATE WELL_TEST_VO SET CHOKE_DESC = CHOKE_SIZE_DESC;
COMMIT;
DOC;
LOADER_TEST_VO - Rename CHOKE_SIZE_DESC for DB2 compatibility...
#
        ALTER TABLE LOADER_TEST_VO ADD (CHOKE_DESC CHAR(8));

        UPDATE LOADER_TEST_VO SET CHOKE_DESC = CHOKE_SIZE_DESC;
COMMIT;
DOC;
WELL_PRESS_4PT - Rename CHOKE_SIZE_DESC for DB2 compatibility...
#
        ALTER TABLE WELL_PRESS_4PT ADD (SOURCE CHAR(12));

        ALTER TABLE WELL_PRESS_4PT ADD (CHOKE_DESC CHAR(8));

        UPDATE WELL_PRESS_4PT SET CHOKE_DESC = CHOKE_SIZE_DESC;
COMMIT;
DOC;
WELL_PROD_ZONE_HDR - Rename CRD_OIL_DENSITY_CODE for DB2 compatibility...
#
	ALTER  TABLE WELL_PROD_ZONE_HDR ADD (OIL_DENSITY_CODE CHAR(2));

	UPDATE WELL_PROD_ZONE_HDR SET OIL_DENSITY_CODE = CRD_OIL_DENSITY_CODE;
COMMIT;
DOC;
WELL_WTR_ANAL_HDR - Rename RESERVOIR_TEMPERATURE for DB2 compatibility...
#
        ALTER  TABLE WELL_WTR_ANAL_HDR ADD (RESERVOIR_TEMP NUMBER(5,2));

        UPDATE WELL_WTR_ANAL_HDR SET RESERVOIR_TEMP = RESERVOIR_TEMPERATURE;
COMMIT;
DOC;
WELL_TEXAS_LOC - Rename LOCATION_DESCRIPTION for DB2 compatibility...
#
        ALTER  TABLE WELL_TEXAS_LOC ADD (LOCATION_DESC CHAR(240));

        UPDATE WELL_TEXAS_LOC SET LOCATION_DESC = LOCATION_DESCRIPTION;
COMMIT;
DOC;
LOADER_TEXAS_LOC - Rename LOCATION_DESCRIPTION for DB2 compatibility...
#
        ALTER  TABLE LOADER_TEXAS_LOC ADD (LOCATION_DESC CHAR(240));

        UPDATE LOADER_TEXAS_LOC SET LOCATION_DESC = LOCATION_DESCRIPTION;
COMMIT;
DOC;
WELL_CORE_ANAL - Rename BULK_DEN AND EFFECTIVE_POR for clarity...
#
        ALTER  TABLE WELL_CORE_ANAL ADD (BULK_DENSITY NUMBER(9,4));

        UPDATE WELL_CORE_ANAL SET BULK_DENSITY = BULK_DEN;

        ALTER  TABLE WELL_CORE_ANAL ADD (EFFECTIVE_POROSITY NUMBER(9,4));

        UPDATE WELL_CORE_ANAL SET EFFECTIVE_POROSITY = EFFECTIVE_POR;

	ALTER TABLE WELL_CORE_ANAL ADD (SAMPLE_NO NUMBER(3) NULL);

        UPDATE WELL_CORE_ANAL SET SAMPLE_NO = POS_NO;

	ALTER TABLE WELL_CORE_ANAL ADD (ANAL_ID CHAR(2) NULL);

        UPDATE WELL_CORE_ANAL SET ANAL_ID = LINE_CODE;
COMMIT;
DOC;
LOADER_CORE_ANAL - Rename BULK_DEN AND EFFECTIVE_POR for clarity...
#
        ALTER  TABLE LOADER_CORE_ANAL ADD (BULK_DENSITY NUMBER(9,4));

        UPDATE LOADER_CORE_ANAL SET BULK_DENSITY = BULK_DEN;

        ALTER  TABLE LOADER_CORE_ANAL ADD (EFFECTIVE_POROSITY NUMBER(9,4));

        UPDATE LOADER_CORE_ANAL SET EFFECTIVE_POROSITY = EFFECTIVE_POR;

	ALTER TABLE LOADER_CORE_ANAL ADD (SAMPLE_NO NUMBER(3) NULL);

        UPDATE LOADER_CORE_ANAL SET SAMPLE_NO = POS_NO;

	ALTER TABLE LOADER_CORE_ANAL ADD (ANAL_ID CHAR(2) NULL);

        UPDATE LOADER_CORE_ANAL SET ANAL_ID = LINE_CODE;
COMMIT;
DOC;
WELL_CORE_SAMPLE - Change pos_no to sample_no for clarity
#
	ALTER TABLE WELL_CORE_SAMPLE ADD (SAMPLE_NO NUMBER(3) NULL);

        UPDATE WELL_CORE_SAMPLE SET SAMPLE_NO = POS_NO;
COMMIT;
DOC;
WELL_CORE_SHOWS - Change pos_no to sample_no for clarity
#
	ALTER TABLE WELL_CORE_SHOWS ADD (SAMPLE_NO NUMBER(3) NULL);

        UPDATE WELL_CORE_SHOWS SET SAMPLE_NO = POS_NO;
COMMIT;
DOC;
WELL_CORE_METHOD - Rename POR_ANAL and DEN_ANAL for clarity...

        ALTER  TABLE WELL_CORE_METHOD ADD (POROSITY_ANAL CHAR(50));

        UPDATE WELL_CORE_METHOD SET POROSITY_ANAL = POR_ANAL;
 
        ALTER  TABLE WELL_CORE_METHOD ADD (DENSITY_ANAL CHAR(50));

        UPDATE WELL_CORE_METHOD SET DENSITY_ANAL = DEN_ANAL;
COMMIT;
DOC;
WELL_POROUS_INTRVL - Rename POR_QUALITY for consistancy in naming convention...
#
	ALTER TABLE WELL_POROUS_INTRVL ADD (POROSITY_QUALITY CHAR(1) NULL);

	UPDATE WELL_POROUS_INTRVL SET POROSITY_QUALITY = POR_QUALITY;
COMMIT;
DOC;
ZONE_VALUES - Add a Remarks field and widen the value field...
#
	ALTER TABLE ZONE_VALUES ADD (REMARKS CHAR(240) NULL);

	ALTER TABLE ZONE_VALUES MODIFY (VALUE NUMBER(15,6));
DOC;
WELL_CORE_HDR - Add SWC_RUN_NO to tie back to the above new table...
#
	ALTER TABLE WELL_CORE_HDR ADD (SWC_RUN_NO CHAR(12) NULL);
DOC;
LOADER_CORE_HDR - Add SWC_RUN_NO to tie back to the above new table...
#
	ALTER TABLE LOADER_CORE_HDR ADD (SWC_RUN_NO CHAR(12) NULL);
DOC;
WELL_DLS_NTS - Add township and range modifier flags...
#
	ALTER TABLE WELL_DLS_NTS ADD (TOWNSHIP_MOD CHAR(1) NULL);

	ALTER TABLE WELL_DLS_NTS ADD (RANGE_MOD CHAR(1) NULL);
DOC;
WELL_FORM_TEST_HDR - This table is the most changed of any in this update.
a few columns are renamed for DB2 compatibility or clarity and a number
of fields have been added to accomodate new public data sources...
#
	ALTER TABLE WELL_FORM_TEST_HDR ADD(CUSHION_LEN_UNIT CHAR(12) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(CUSHION_AMT_UNIT CHAR(12) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(PREFLOW_PRESS NUMBER(5) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(HOLE_SIZE NUMBER(5,1) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(CUSHION_GAS_PRESS NUMBER(5) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(CUSHION_INHIB NUMBER(7,1) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(RECORDER_TYPE CHAR(12) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(RECORDER_PERFORM CHAR(12) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(RECORDER_USED CHAR(12) NULL);

	ALTER TABLE WELL_FORM_TEST_HDR ADD(TEST_RESULT CHAR(12) NULL);

	UPDATE WELL_FORM_TEST_HDR SET TEST_RESULT = RESULT;

	ALTER TABLE WELL_FORM_TEST_HDR ADD(CUSHION_TYPE CHAR(12) NULL);

	UPDATE WELL_FORM_TEST_HDR SET CUSHION_TYPE = TYPE_CUSH;

	ALTER TABLE WELL_FORM_TEST_HDR ADD(CUSHION_AMT NUMBER(7,2) NULL);

	UPDATE WELL_FORM_TEST_HDR SET CUSHION_AMT = AMT_CUSH;
COMMIT;
DOC;
WELL_GAS_ANAL_DETL - Expand the analysis field to allow any kind of number...
#
	ALTER TABLE WELL_GAS_ANAL_DETL MODIFY(ANALYSIS NUMBER(15,6));
DOC;
WELL_GAS_ANAL_HDR - Expand the location field to allow for longer description of
position of sample in the core...
#
	ALTER TABLE WELL_GAS_ANAL_HDR MODIFY(LOCATION CHAR(30));
DOC;
WELL_HDR - Add a base_node_id field to accomodate those who wish to store a
bottom hole lat-long and xy...
#
	ALTER TABLE WELL_HDR ADD (BASE_NODE_ID NUMBER(10) NULL);
DOC;
LOADER_WELL_HDR - Add a base_node_id field to accomodate those who wish to
store a bottom hole lat-long and xy...
#
	ALTER TABLE LOADER_WELL_HDR ADD (BASE_NODE_ID NUMBER(10) NULL);
DOC;
WELL_HDR_SECOND - Add a base_node_id field to accomodate those who wish to
store a bottom hole lat-long and xy...
#
	ALTER TABLE WELL_HDR_SECOND ADD (BASE_NODE_ID NUMBER(10) NULL);
DOC;
WELL_LOG_RUN_HDR - Remove the TORTUOSITY AND CEMENTATION fields since they
don't really belong here anyway.  Note that there is no easy way to update
old projects to eliminate these fields and in most cases it doesn't hurt to
leave them in so this script will simply leave them where they are...

WELL_M_B - In any new versions of this table created the fields SPOT_CODE AND
LOC_TYPE will be removed because they were originally put in to accomidate US
location data and this table is not appropriate for those data. In addition,
change surf_location to reference_location for accuracy...
#
	ALTER TABLE WELL_M_B ADD(RATIONALIZATION CHAR(12) NULL);

	ALTER TABLE WELL_M_B ADD(REFERENCE_LOCATION CHAR(20) NULL);

	UPDATE WELL_M_B SET REFERENCE_LOCATION = SURF_LOCATION;

	ALTER TABLE WELL_M_B ADD(NODE_ID NUMBER(10) NULL);

	UPDATE WELL_M_B M SET
		NODE_ID = (SELECT NODE_ID FROM WELL_HDR H
				WHERE H.UWI = M.UWI);
COMMIT;
DOC;
WELL_OIL_ANAL_HDR - Add a bunch of fields to store formation info for the
interval sampled. Also add date sampled...
#
	ALTER TABLE WELL_OIL_ANAL_HDR ADD(SAMPLE_DATE DATE);

	ALTER TABLE WELL_OIL_ANAL_HDR ADD(LOCATION CHAR(30) NULL);

	ALTER TABLE WELL_OIL_ANAL_HDR ADD(TOP_FORM CHAR(12) NULL);

	ALTER TABLE WELL_OIL_ANAL_HDR ADD(TOP_AGE CHAR(6) NULL);

	ALTER TABLE WELL_OIL_ANAL_HDR ADD(BASE_FORM CHAR(12) NULL);

	ALTER TABLE WELL_OIL_ANAL_HDR ADD(BASE_AGE CHAR(6) NULL);
DOC;
WELL_WTR_ANAL_HDR - Add a bunch of fields to store formation info for the
interval sampled. Also add date sampled...
#
	ALTER TABLE WELL_WTR_ANAL_HDR ADD(SAMPLE_DATE DATE);

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(LOCATION CHAR(30) NULL);

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(TOP_FORM CHAR(12) NULL);

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(TOP_AGE CHAR(6) NULL);

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(BASE_FORM CHAR(12) NULL);

	ALTER TABLE WELL_WTR_ANAL_HDR ADD(BASE_AGE CHAR(6) NULL);
DOC;
WELL_PROD_ZONE_HDR - Add interval fields...
#
	ALTER TABLE WELL_PROD_ZONE_HDR ADD (TOP NUMBER(7,2)NULL);

	ALTER TABLE WELL_PROD_ZONE_HDR ADD (BASE NUMBER(7,2)NULL);
DOC;
WELL_TEST_VO - Add test_recovery_meth field...
#
	ALTER TABLE WELL_TEST_VO ADD (TEST_RECOVERY_METH CHAR(12) NULL);
DOC;
WELL_TEST_VO_ANAL - Expand a couple of columns to allow data massaging...
#
	ALTER TABLE WELL_TEST_VO_ANAL MODIFY (GAS_CONTENT NUMBER(15,6));

	ALTER TABLE WELL_TEST_VO_ANAL MODIFY (COND_RATIO NUMBER(15,6));
DOC;
WELL_TEST_VO_REC - Since there is the new contaminants table, the RECOV_CONT
field is no longer needed or appropriate in this table and will be dropped in
any new versions of the table created. Also add a flag for whether or not the
produced fluids had to be reverse-circulated out of the hole...
#
	ALTER TABLE WELL_TEST_VO_REC ADD (REVERSE_CIRC CHAR(3) NULL);

	INSERT INTO WELL_TEST_VO_CONT (UWI,SOURCE,TEST_TYPE,TEST_NO,
			VALVE_OPEN_NO,CONTAMINANT,REMARKS)
		SELECT UWI,SOURCE,TEST_TYPE,TEST_NO,VALVE_OPEN_NO,
			RECOV_CONT,'INSERTED FROM OLD WELL_TEST_VO_REC TABLE'
			FROM WELL_TEST_VO_REC;
COMMIT;
DOC;
WELL_CHECKSHT_SRVY - Rename measured_depth column to be consistant with usage
elsewhere...
#
	ALTER TABLE WELL_CHECKSHT_SRVY ADD (MD NUMBER(7,2) NULL);

	UPDATE WELL_CHECKSHT_SRVY SET MD = MEASURED_DEPTH;
COMMIT;
DOC;
WELL_COMPLETIONS - Rename type_perf for consistancy...
#
	ALTER TABLE WELL_COMPLETIONS ADD (PERF_TYPE CHAR(12) NULL);

	UPDATE WELL_COMPLETIONS SET PERF_TYPE = TYPE_PERF;
COMMIT;
DOC;
WELL_MISC_DATA - Widen data_type field to be more generic..
#
	ALTER TABLE WELL_MISC_DATA MODIFY (DATA_TYPE CHAR(12));
DOC;
WELL_PROD_FLD_DAT - Add a source field...
#
	ALTER TABLE WELL_PROD_FLD_DAT ADD (SOURCE CHAR(12));
DOC;
WELL_PROD_HIST - This fixes a typo in recently released versions of the
schema.(oooops!)...
#
	ALTER TABLE WELL_PROD_HIST ADD (AUG NUMBER(12,2) NULL);

	UPDATE WELL_PROD_HIST SET AUG = AUR;
COMMIT;
DOC;
WELL_PROD_TEST_HDR - Rename prod_date to test_date for clarity...
#
	ALTER TABLE WELL_PROD_TEST_HDR ADD (TEST_DATE DATE);

	UPDATE WELL_PROD_TEST_HDR SET TEST_DATE = PROD_DATE;
COMMIT;

DOC;
			TABLES TO BE DROPPED

DON'T DO THIS UNTIL YOU ARE SURE ALL OF THE ABOVE WORKED OK AND YOU ARE SURE YOU
WANT TO DROP THEM.  LEAVE THIS COMMENTED OUT UNTIL YOU NEED THEM!!!!!!!!!!!!!!

	DROP TABLE WELL_PROD_FLOW_RATE;

ZONE_EQUIV - This table is being dropped because it is not used for anything and
is not useful for anything...

	DROP TABLE ZONE_EQUIV;
#
COMMIT;
EXIT;

