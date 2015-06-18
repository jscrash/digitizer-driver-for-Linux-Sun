/* DEC/CMS REPLACEMENT HISTORY, Element LD_MOVE_LOADER_TABS.SQL */
/* *6    16-JUL-1990 18:35:59 ALLAWAY "(SPR -1) CHECKIN FOR 7-15-90 SCHEMA UPDATE" */
/* *5    23-APR-1990 14:26:27 PURNA "(SPR 0) Dorothy's enron mods" */
/* *4     6-APR-1990 16:50:42 ALLAWAY "(SPR -1) bug fixes and top handling enhancements" */
/* *3    27-MAR-1990 08:14:39 DOROTHY "(SPR -1) Corrected errors." */
/* *2    23-MAR-1990 21:21:31 DOROTHY "(SPR -1) CHECKIN" */
/* *1    23-MAR-1990 21:15:53 DOROTHY "MOVE WHCS LOADER TABLES." */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_MOVE_LOADER_TABS.SQL */
DOC;
ld_whcs_move_loader_tabs.sql Bill Allaway 3/5/90
This script is intended to move the data loaded into loader tables into
their final resting places.


LOADER_PERM_CORE_DESC - Move the data loaded into this table into 
the WELL_CORE_DESC table.  NOTE: This statement is currently commented out,
since it only applies to Permian basin data.  If you are loading Permian Basin
data, uncomment this statement:

	INSERT INTO WELL_CORE_DESC(
		UWI,
		SOURCE,
		CORE_ID,
		SEQ,
		DEPTH,
		REMARKS)
	SELECT 
		UWI,
		SOURCE,
		CORE_ID,
		SEQ,
		CORE_TOP + DIST_FROM_TOP,
		LITHOLOGY_CODE||' '||POROSITY_TYPE||' '||SHOW_TYPE||' '||REMARKS
	FROM LOADER_PERM_CORE_DESC;

WELL_TEST_VO_FLOW - This statement moves data into WELL_TEST_VO_FLOW, converting
time to surface from hours and minutes to minutes and units.
#
INSERT INTO WELL_TEST_VO_FLOW(
     UWI,
     SOURCE,
     TEST_NO,
     TEST_TYPE,
     VALVE_OPEN_NO,
     FLUID_TYPE,
     FLUID_RATE,
     FLUID_RATE_UNIT,
     TTS,
     TTS_UNIT,
     PERCENT_SHAKE_OUT,
     FLOW_DURATION,
     FLOW_DURATION_UNIT,
     MEASURE_TECH)
SELECT
     UWI,
     SOURCE,
     TEST_NO,
     TEST_TYPE,
     VALVE_OPEN_NO,
     FLUID_TYPE,
     FLUID_RATE,
     FLUID_RATE_UNIT,
     (TTS_HOURS * 60) + TTS_MIN,
     'MIN',
     PERCENT_SHAKE_OUT,
     (FLOW_DURATION_HOURS * 60) + FLOW_DURATION_MIN,
     'MIN',
     MEASURE_TECH
FROM LOADER_TEST_VO_FLOW;
COMMIT;

DOC;
LOADER_TEST_VO - Move data from this table to WELL_TEST_VO, converting
VOT_HOURS and VOT_MINUTES to minutes and units...
#
INSERT INTO WELL_TEST_VO(
     UWI,                                                    
     SOURCE,                                                 
     TEST_NO,
     VALVE_OPEN_NO,                                         
     VOT,                                                      
     VOT_UNIT,                                                   
     IFP,                                                       
     FFP,                                                       
     MFP,                                                       
     SIP,                                                       
     SIT,                                                      
     SIT_UNIT,                                                   
     CHOKE_SIZE,                                               
     CASING_PRESS,                                              
     TUBING_PRESS,                                              
     BH_PRESS,                                                  
     BH_TEMPERATURE,                                           
     REMARKS)
SELECT
     UWI,                                                    
     SOURCE,                                                 
     TEST_NO,
     VALVE_OPEN_NO,                                         
     (VOT_HOURS * 60) + VOT_MIN, 
     'MIN',                                                   
     IFP,                                                       
     FFP,                                                       
     MFP,                                                       
     SIP,                                                       
     (SIT_HOURS * 60) + SIT_MIN, 
     'MIN',                                                   
     CHOKE_SIZE,                                               
     CASING_PRESS,                                              
     TUBING_PRESS,                                              
     BH_PRESS,                                                  
     BH_TEMPERATURE,                                           
     REMARKS
FROM LOADER_TEST_VO;
COMMIT;
#
DOC;
The following updates the azimuth field to the correct degrees in preparation
for moving the data into WELL_DIR_SRVY_PTS.  Actually, these data are not 
used by the Finder software and so will not be inserted into WELL_DIR_SRVY_PTS.
If these are the only deviation data available for a well, then a program
should be written to calculate DX,DY,TVD and those values should be put in
WELL_DIR_SRVY_PTS.

First update the SE Quadrant...
#
	UPDATE LOADER_DEV_SRVY_PTS
		SET AZIMUTH = 180 - AZIMUTH
	WHERE AZIMUTH_NS = 'S' AND AZIMUTH_EW = 'E';
DOC;
Next update the SW quadrant...
#
	UPDATE LOADER_DEV_SRVY_PTS
		SET AZIMUTH = 180 + AZIMUTH
	WHERE AZIMUTH_NS = 'S' AND AZIMUTH_EW = 'W';
DOC;
Next update the NW quadrant...
#
	UPDATE LOADER_DEV_SRVY_PTS
		SET AZIMUTH = 360 - AZIMUTH
	WHERE AZIMUTH_NS = 'N' AND AZIMUTH_EW = 'W';
DOC;
The NW quadrant already matches the equivilent azimuth. now commit before
continuing...
#
COMMIT;

DOC;
WELL_DIR_SRVY_PTS - The following prepares the data in LOADER_DIR_SRVY_PTS
for insertion into this table, then moves the data.

First make x offsets negative if west of the well location...
#
	UPDATE LOADER_DIR_SRVY_PTS
		SET DX = -1 * DX
	WHERE DX_DIR = 'W';
DOC;
Next make y offsets negative if south of the well location...
#
	UPDATE LOADER_DIR_SRVY_PTS
		SET DY = -1 * DY
	WHERE DX_DIR = 'S';
DOC;
Commit changes before going on...
# 
COMMIT;
DOC;
Now insert the data into WELL_DIR_SRVY_PTS...
#
INSERT INTO WELL_DIR_SRVY_PTS(
     UWI,
     SOURCE,
     SURVEY_NO,
     MD,
     DX,
     DY,
     TVD)
SELECT
     UWI,
     SOURCE,
     SURVEY_NO,
     MD,
     DX,
     DY,
     TVD
FROM LOADER_DIR_SRVY_PTS;
COMMIT;

DOC;
Move the fault data from LOADER_FAULT_DATA into WELL_TOPS where it will show up

First make the fault name something appropriate for the form_code field ...
#
	UPDATE LOADER_FAULT_DATA
		SET FAULT_NAME = 'FAULT'||FAULT_NAME;
COMMIT;
DOC;
Next insert the data into the WELL_TOPS table...
#
	INSERT INTO WELL_TOPS(
		UWI,
		FORM_CODE,
		SOURCE,
		MD,
		TVD)
	SELECT
		UWI,
		FAULT_NAME,
		SOURCE,
		TOP,
		TOP_TVD
	FROM LOADER_FAULT_DATA;
COMMIT;

DOC;
WELL_HDR - Many of the WHCS records do not contain complete dates. The 
LOADER_WELL_HDR table makes all the date fields (except LAST_UPDATE) a 
character field so that the date information can be put into something 
recognizable by Oracle as a date before being put into its final resting
place in the WELL_HDR table. It is strongly recommended that the following
select statements be run to determine what the appropriate date mask is
to apply to the TO_DATE function included in the insert statement below
or to help you design an update statement that will convert the dates to a
consistant format.  Currently, the only dates being loaded are SPUD_DATE
and COMP_DATE so you may want to limit your investigations to these two
fields:

	SELECT DISTINCT SPUD_DATE_DESC FROM LOADER_WELL_HDR;
	SELECT DISTINCT COMP_DATE_DESC FROM LOADER_WELL_HDR;

First update base_delta_x to be negative if west of the well...
#
	UPDATE LOADER_WELL_HDR SET
		BASE_DELTA_X = -1 * BASE_DELTA_X
	WHERE BASE_DELTA_X_DIR = 'W';
DOC;
Next update base_delta_y to be negative if south of the well...
#
	UPDATE LOADER_WELL_HDR SET 
		BASE_DELTA_Y = -1 * BASE_DELTA_Y 
	WHERE BASE_DELTA_Y_DIR = 'Y';
COMMIT;
DOC;
Next we need to be sure certain fields are wide enough for the data to be 
inserted...
#
	ALTER TABLE WELL_HDR MODIFY(
		OPERATOR	CHAR(30),
		PROV_ST		CHAR(8),
		FLDPOOLWILD	CHAR(18));
DOC;
Now move the non-controversial data into well_hdr...
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
         LOG_TD,
         COUNTRY,
         PROV_ST,
         COUNTY,
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
         FLDPOOLWILD,
         FAULT,
         SHORT_NAME,
         LAST_UPDATE,
         LAST_UPTIME,
         LICENSEE,
         AGENT,
         CONFID_INDEX,
         CONFID_INDEX_RATIO,
         CONFID_INDEX_LEVEL,
         STRAT_COLUMN,
         LEASE_NO,
         LEASE_NAME,
         DISTRICT,
         GOVT_ASSIGNED_NO,
         LOCATION_TABLE,
         STORAGE_UNIT)
SELECT
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
         LOG_TD,
         COUNTRY,
         PROV_ST,
         COUNTY,
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
         FLDPOOLWILD,
         FAULT,
         SHORT_NAME,
         LAST_UPDATE,
         LAST_UPTIME,
         LICENSEE,
         AGENT,
         CONFID_INDEX,
         CONFID_INDEX_RATIO,
         CONFID_INDEX_LEVEL,
         STRAT_COLUMN,
         LEASE_NO,
         LEASE_NAME,
         DISTRICT,
         GOVT_ASSIGNED_NO,
         LOCATION_TABLE,
         STORAGE_UNIT
FROM LOADER_WELL_HDR;
COMMIT;
DOC;
Now move the problem data...

First try moving Spud_date...
#
	UPDATE WELL_HDR H SET
         SPUD_DATE = (SELECT TO_DATE(SPUD_DATE_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI
			AND SPUD_DATE_DESC LIKE '%/%/%');
COMMIT;
DOC;
Next move rigrel...

	UPDATE WELL_HDR H SET
         RIGREL = (SELECT TO_DATE(RIGREL_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);
COMMIT;
#
DOC;
Next move comp_date...
#
	UPDATE WELL_HDR H SET
         COMP_DATE = (SELECT TO_DATE(COMP_DATE_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI
			AND COMP_DATE_DESC LIKE '%/%/%');
COMMIT;
DOC;
Next move ONPROD..

	UPDATE WELL_HDR H SET
         ONPROD = (SELECT TO_DATE(ONPROD_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);
COMMIT;

Next move calc_onprod...

	UPDATE WELL_HDR H SET
         CALC_ONPROD = (SELECT TO_DATE(CALC_ONPROD_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);
COMMIT;

next move oninject...

	UPDATE WELL_HDR H SET
         ONINJECT = (SELECT TO_DATE(ONINJECT_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);
COMMIT;

Next move CONF_REL_DT_DESC...

	UPDATE WELL_HDR H SET
         CONF_REL_DT = (SELECT TO_DATE(CONF_REL_DT_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);
COMMIT;

Next move status_date...

	UPDATE WELL_HDR H SET
         STATUS_DATE = (SELECT TO_DATE(STATUS_DATE_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);
COMMIT;

Next move FIN_DRILL...

	UPDATE WELL_HDR H SET
         FIN_DRILL = (SELECT TO_DATE(FIN_DRILL_DESC,'MM/DD/YYYY') 
			FROM LOADER_WELL_HDR L WHERE L.UWI = H.UWI);

COMMIT;
#
DOC;
WELL_FORM_TEST_HDR - This inserts the data from loader_form_test_hdr into 
well_form_test_hdr, converting the shut in times from hour and minutes to
minutes:
#
INSERT INTO WELL_FORM_TEST_HDR(
     UWI,
     SOURCE,
     TEST_NO, 
     TEST_TYPE, 
     ASSIGNED_TEST_NO,
     TOP,  
     BASE, 
     TOP_FORM,
     TOP_AGE,
     BASE_FORM,
     BASE_AGE,   
     TEST_COMP,
     TEST_DATE,
     TEST_RESULT,
     RECORDER_DEPTH,
     PIPE_ID,
     CUSHION_TYPE,
     CUSHION_LEN,
     CUSHION_AMT,
     IHP,
     FHP,
     MAX_HP,
     PREFLOW_TIME,
     PREFLOW_UNIT,
     ISIT,
     ISIT_UNIT,
     ISIP,
     MAX_ISIP,
     FSIT,
     FSIT_UNIT,
     FSIP,
     MAX_FSIP,
     MAX_GAS_RATE,
     FIN_GAS_RATE,
     MAX_OIL_RATE,
     MAX_WTR_RATE,
     TEST_REMARKS,
     MAX_GAS_UNIT,
     FIN_GAS_UNIT,
     MAX_OIL_UNIT,
     MAX_WTR_UNIT,
     TOP_CHOKE_DESC, 
     BOTTOM_CHOKE_DESC,
     TOOL_DIAMETER,
     SURF_FLOW_PRESS,
     OPR_SEQ)
SELECT
     UWI,
     SOURCE,
     TEST_NO, 
     TEST_TYPE, 
     ASSIGNED_TEST_NO,
     TOP,  
     BASE, 
     TOP_FORM,
     TOP_AGE,
     BASE_FORM,
     BASE_AGE,   
     TEST_COMP,
     TEST_DATE,
     TEST_RESULT,
     RECORDER_DEPTH,
     PIPE_ID,
     CUSHION_TYPE,
     CUSHION_LEN,
     CUSHION_AMT,
     IHP,
     FHP,
     MAX_HP,
     PREFLOW_TIME,
     PREFLOW_UNIT,
     (ISIT_HOURS * 60) + ISIT_MIN,
     'MIN',
     ISIP,
     MAX_ISIP,
     (FSIT_HOURS * 60) + FSIT_MIN,
     'MIN',
     FSIP,
     MAX_FSIP,
     MAX_GAS_RATE,
     FIN_GAS_RATE,
     MAX_OIL_RATE,
     MAX_WTR_RATE,
     TEST_REMARKS,
     MAX_GAS_UNIT,
     FIN_GAS_UNIT,
     MAX_OIL_UNIT,
     MAX_WTR_UNIT,
     TOP_CHOKE_DESC, 
     BOTTOM_CHOKE_DESC,
     TOOL_DIAMETER,
     SURF_FLOW_PRESS,
     OPR_SEQ
FROM LOADER_FORM_TEST_HDR;
COMMIT;

DOC;
LOADER_CORE_HDR - Update CORE_DIA then insert records into WELL_CORE_HDR...

Updating the Core_dia field to move insert the decimal in the correct place...
#
	UPDATE LOADER_CORE_HDR SET
		CORE_DIA = CORE_DIA * .001;
COMMIT;
DOC;
Inserting the data into well_core_hdr...
#
	INSERT INTO WELL_CORE_HDR(
		UWI, 
		SOURCE,       
		CORE_ID,      
		OPR_CORE_NO,
		CORE_DATE, 
		TOP,   
		BASE,  
		PRIMARY_FORM,
		PRIMARY_AGE,  
		RECOVERED,      
		REC_UNIT,
		CORE_TYPE,
		CORE_DIA,
		CORE_FLUID,       
		CORE_HAND,
		ANL_REPT, 
		CORE_DIG_AVAIL,    
		SHOW_TYPE,
		CORE_OR_SPL,       
		OPR_SEQ,  
		LAB_CODE,
		LAB_FILE_NO,      
		ANALYST, 
		GAMMA_CORR,
		SAMPLE_SHAPE,     
		SAMPLE_DIA,      
		SAMPLE_LEN,      
		SOLVENT,  
		EXT_TIME,
		DRY_TIME,
		DRY_TEMPERATURE,
		ORIG_UNIT,
		REMARKS)
	SELECT
		UWI, 
		SOURCE,       
		CORE_ID,      
		OPR_CORE_NO,
		CORE_DATE, 
		TOP,   
		BASE,  
		PRIMARY_FORM,
		PRIMARY_AGE,  
		RECOVERED,      
		REC_UNIT,
		CORE_TYPE,
		CORE_DIA,
		CORE_FLUID,       
		CORE_HAND,
		ANL_REPT, 
		CORE_DIG_AVAIL,    
		SHOW_TYPE,
		CORE_OR_SPL,       
		OPR_SEQ,  
		LAB_CODE,
		LAB_FILE_NO,      
		ANALYST, 
		GAMMA_CORR,
		SAMPLE_SHAPE,     
		SAMPLE_DIA,      
		SAMPLE_LEN,      
		SOLVENT,  
		EXT_TIME,
		DRY_TIME,
		DRY_TEMPERATURE,
		ORIG_UNIT,
		REMARKS
	FROM LOADER_CORE_HDR;
COMMIT;
DOC;
Update the anal_date field with the date. if this fails then you will have
to patch up the data by hand and try again...
#
	UPDATE WELL_CORE_HDR H SET
         ANAL_DATE = (SELECT TO_DATE(ANAL_DATE_DESC,'MMDDYY') 
			FROM LOADER_CORE_HDR L WHERE L.UWI = H.UWI
			AND L.CORE_ID = H.CORE_ID
			AND ANAL_DATE_DESC LIKE '%/%/%');


COMMIT;

DOC;
LOADER_CORE_ANAL - Need to update all the data fields with implied decimals
then move everything into WELL_CORE_ANAL...

Updating LOADER_CORE_ANAL to put the decimal places in the correct places...
#
	UPDATE LOADER_CORE_ANAL SET
		TOP = TOP * .1,
		DEPTH = DEPTH * .1,
		KMAX = KMAX * .01,
		K90 = K90 * .01,
		POROSITY = POROSITY * .1,
		EFFECTIVE_POROSITY = EFFECTIVE_POROSITY * .1,
		GAS_SAT_VOL = GAS_SAT_VOL * .1,
		GRAIN_DENSITY = GRAIN_DENSITY * .01,  
		BULK_DENSITY = BULK_DENSITY * .01,
		BULK_MASS_OIL = BULK_MASS_OIL * .1,  
		BULK_MASS_WAT = BULK_MASS_WAT * .1,  
		PORE_VOL_OIL = PORE_VOL_OIL * .1,   
		PORE_VOL_WAT = PORE_VOL_WAT * .1,   
		BULK_VOL_OIL = BULK_VOL_OIL * .1,   
		BULK_VOL_WAT = BULK_VOL_WAT * .1;
COMMIT;
DOC;
Now insert the data into WELL_CORE_ANAL...
#

	INSERT INTO WELL_CORE_ANAL(
		UWI, 
		SOURCE,       
		CORE_ID,      
		SAMPLE_NO,      
		TOP,   
		ANAL_ID,
		DEPTH, 
		LENGTH,
		KMAX,  
		K90,   
		KVERT, 
		POROSITY,       
		GRAIN_DENSITY,  
		BULK_DENSITY,
		BULK_MASS_OIL,  
		BULK_MASS_WAT,  
		BULK_MASS_SAND, 
		PORE_VOL_OIL,   
		PORE_VOL_WAT,   
		BULK_VOL_OIL,   
		BULK_VOL_WAT,   
		GRAIN_MASS_OIL, 
		GRAIN_MASS_WAT, 
		LITHOLOGY,
		EFFECTIVE_POROSITY,
		GAS_SAT_VOL)
	SELECT
		UWI, 
		SOURCE,       
		CORE_ID,      
		SAMPLE_NO,      
		TOP,   
		ANAL_ID,
		DEPTH, 
		LENGTH,
		KMAX,  
		K90,   
		KVERT, 
		POROSITY,       
		GRAIN_DENSITY,  
		BULK_DENSITY,
		BULK_MASS_OIL,  
		BULK_MASS_WAT,  
		BULK_MASS_SAND, 
		PORE_VOL_OIL,   
		PORE_VOL_WAT,   
		BULK_VOL_OIL,   
		BULK_VOL_WAT,   
		GRAIN_MASS_OIL, 
		GRAIN_MASS_WAT, 
		LITHOLOGY,
		EFFECTIVE_POROSITY,
		GAS_SAT_VOL
	FROM LOADER_CORE_ANAL;
COMMIT;

DOC;
LOADER_WELL_TOPS - The whcs data set has several tops per card and if any
one of those duplicates a form_code, the whole card gets thrown out, losing
the other tops on the card.  This table allows all tops to be loaded. They
can then be moved over to WELL_TOPS using SQL and only the individual 
duplicates will be thrown out.

	INSERT INTO WELL_TOPS(
         UWI,
         FORM_CODE,
         SOURCE,
         MD,
         AGE_CODE,
         TVD,
         PICK_REF,
         TVD_METHOD,
         LAST_UPDATE,
         QUAL_CODE,
         DESC_CODE,
         FAULTED)
	SELECT
         UWI,
         FORM_CODE,
         SOURCE,
         MD,
         AGE_CODE,
         TVD,
         PICK_REF,
         TVD_METHOD,
         LAST_UPDATE,
         QUAL_CODE,
         DESC_CODE,
         FAULTED
	FROM LOADER_WELL_TOPS; 
COMMIT;
#
DOC;
WELL_PRESS_BHP - Move data from loader_press_bhp and convert the times
from hours and minutes to minutes and units...
#
	INSERT INTO WELL_PRESS_BHP(
		       UWI,
                       SOURCE,
                       TEST_NO,
                       TEST_TYPE,
                       ES_EXTENSION,
                       PRESS_OBS_NO,
                       BHP_DATE,
                       WH_PRESS,
                       RUN_DEPTH,
                       BHFP,
                       PRESS_GRADIENT,
                       TEMPERATURE,
                       SHUTIN_PERIOD,
                       SHUTIN_UNIT,
                       DATUM_PRESS,
                       REMARKS_IND,
                       TUBING_SIZE_DESC,
                       PACKER_DEPTH,
                       DURATION,
                       DURATION_UNIT ,
                       SEA_LEVEL_DATUM)
		SELECT
		       UWI,
                       SOURCE,
                       TEST_NO,
                       TEST_TYPE,
                       ES_EXTENSION,
                       PRESS_OBS_NO,
                       BHP_DATE,
                       WH_PRESS,
                       RUN_DEPTH,
                       BHFP,
                       PRESS_GRADIENT,
                       TEMPERATURE,
                       (SHUTIN_HOURS * 60) + SHUTIN_MIN,
                       'MIN',
                       DATUM_PRESS,
                       REMARKS_IND,
                       TUBING_SIZE_DESC,
                       PACKER_DEPTH,
                       (DURATION_HOURS * 60) + DURATION_MIN,
                       'MIN',
                       SEA_LEVEL_DATUM 
		FROM LOADER_PRESS_BHP;
	COMMIT;
EXIT;
