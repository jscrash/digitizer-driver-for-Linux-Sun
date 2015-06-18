/* DEC/CMS REPLACEMENT HISTORY, Element LD_CREATE_LOADER_TABLES.SQL */
/* *10   21-DEC-1990 14:21:54 ALLAWAY "(SPR 5528) PERFORMANCE OVERHAUL" */
/* *9     7-NOV-1990 11:11:17 ALLAWAY "(SPR 5528) FIX MINOR SYNTAX ERRORS" */
/* *8     7-NOV-1990 11:09:18 ALLAWAY "(SPR 5528) FIX SOME SYNTAX ERRORS" */
/* *7    16-JUL-1990 18:35:08 ALLAWAY "(SPR -1) CHECKIN FOR 7-15-90 SCHEMA UPDATE" */
/* *6    23-APR-1990 14:26:15 PURNA "(SPR 0) Dorothy's enron mods" */
/* *5     6-APR-1990 16:49:22 ALLAWAY "(SPR -1) fixes to last to create table statements" */
/* *4    27-MAR-1990 08:15:16 DOROTHY "(SPR -1) Corrected errors." */
/* *3    25-MAR-1990 11:05:29 VINCE "(SPR -1) New one from Bill A." */
/* *2    23-MAR-1990 21:20:52 DOROTHY "(SPR -1) CHECKIN" */
/* *1    23-MAR-1990 21:14:47 DOROTHY "CREATE LOADER TABLES FOR WHCS LOADER." */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_CREATE_LOADER_TABLES.SQL */
DOC;
This table is only required if you are loading Permian Basin data...
#
CREATE TABLE LOADER_PERM_CORE_DESC (
     UWI CHAR(20) NOT NULL,                                                    
     SOURCE CHAR(12) NOT NULL,
     CORE_ID CHAR(12) NOT NULL,
     SEQ NUMBER(3),
     CORE_TOP NUMBER(7,2),
     CORE_BASE NUMBER(7,2),
     DIST_FROM_TOP NUMBER(7,2),
     LITHOLOGY_CODE CHAR(12),
     POROSITY_TYPE CHAR(12),
     SHOW_TYPE CHAR(12),
     REMARKS CHAR(240));
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_PERM_CORE_DESC TO PUBLIC;
DOC;
This table allows the loading of test material data with time to surface data 
reported in hours and minutes to be converted to single units when moved into
WELL_TEST_VO_FLOW...
#
CREATE TABLE LOADER_TEST_VO_FLOW(
     UWI CHAR(20) NOT NULL,
     SOURCE CHAR(12) NOT NULL,
     TEST_NO CHAR(4) NOT NULL,
     TEST_TYPE CHAR(12) NOT NULL,
     VALVE_OPEN_NO NUMBER(3) NOT NULL,
     FLUID_TYPE CHAR(12) NOT NULL,
     FLUID_RATE NUMBER(12,2) NULL,
     FLUID_RATE_UNIT CHAR(12) NULL,
     TTS_HOURS NUMBER(5) NULL,
     TTS_MIN NUMBER(5) NULL,
     PERCENT_SHAKE_OUT NUMBER(5,2) NULL,
     FLOW_DURATION_HOURS NUMBER(5) NULL,
     FLOW_DURATION_MIN NUMBER(5) NULL,
     MEASURE_TECH CHAR(12) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_TEST_VO_FLOW TO PUBLIC;

DOC;
WELL_TEST_VO - Loader table to load this table in order to convert 
valve open time from hours and minutes to minutes.
#
CREATE TABLE LOADER_TEST_VO(
     UWI CHAR(20) NOT NULL,                                                    
     SOURCE CHAR(12) NOT NULL,                                                 
     TEST_NO CHAR(4) NOT NULL,
     TEST_TYPE CHAR(12) NOT NULL,
     VALVE_OPEN_NO NUMBER(3) NOT NULL,                                         
     VOT_HOURS NUMBER(5) NULL,                                                      
     VOT_MIN NUMBER(5) NULL,                                                   
     IFP NUMBER(5) NULL,                                                       
     FFP NUMBER(5) NULL,                                                       
     MFP NUMBER(5) NULL,                                                       
     SIP NUMBER(5) NULL,                                                       
     SIT_HOURS NUMBER(5) NULL,                                                      
     SIT_MIN NUMBER(5) NULL,                                                   
     CHOKE_SIZE NUMBER(7,2) NULL,                                               
     CHOKE_DESC CHAR(12) NULL,                                               
     CASING_PRESS NUMBER(5) NULL,                                              
     TUBING_PRESS NUMBER(5) NULL,                                              
     BH_PRESS NUMBER(5) NULL,                                                  
     BH_TEMPERATURE NUMBER(5,2) NULL,                                           
     REMARKS CHAR(240) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_TEST_VO TO PUBLIC;

DOC
The following tables are needed to load the data related to the deviation of
a well. LOADER_DEVIATION_SRVY loads the data presented in degrees of deviation
and azimuth format.  This type of information can not be used directly by 
Finder, but may be useful for coverting to the correct directional survey 
format. Finder currently does not have any conversion programs available.
#
CREATE TABLE LOADER_DEV_SRVY_PTS (
     UWI CHAR(20) NOT NULL,                                                    
     SOURCE CHAR(12) NOT NULL,                                                 
     SURVEY_NO NUMBER(3) NOT NULL,                                             
     MD NUMBER(7,2) NOT NULL,                                                   
     DEVIATION_ANGLE NUMBER(10,6) NULL,                                         
     AZIMUTH NUMBER(10,6) NULL,
     AZIMUTH_NS CHAR(1) NULL,
     AZIMUTH_EW CHAR(1) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_DEV_SRVY_PTS TO PUBLIC;

CREATE TABLE LOADER_DIR_SRVY_PTS (
     UWI CHAR(20) NOT NULL,                                                    
     SOURCE CHAR(12) NOT NULL,                                                 
     SURVEY_NO NUMBER(3) NOT NULL,                                             
     MD NUMBER(7,2) NOT NULL,                                                   
     DX NUMBER(15,2) NOT NULL,                                                  
     DX_DIR CHAR(1) NULL,
     DY NUMBER(15,2) NOT NULL,                                                  
     DY_DIR CHAR(1) NULL,
     TVD NUMBER(7,2) NOT NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_DIR_SRVY_PTS TO PUBLIC;
                                            

CREATE TABLE LOADER_PRESS_BHP(
		UWI CHAR(20) NOT NULL, 
		SOURCE CHAR(12) NOT NULL,       
		TEST_NO CHAR(4) NULL,  
		TEST_TYPE CHAR(12) NULL,
		ES_EXTENSION NUMBER(2) NULL,    
		PRESS_OBS_NO NUMBER(2) NOT NULL,
		BHP_DATE DATE NULL,  
		WH_PRESS NUMBER(5) NULL,
		RUN_DEPTH NUMBER(7,2) NULL,      
		BHFP NUMBER(5) NULL,   
		PRESS_GRADIENT NUMBER(5,2) NULL, 
		TEMPERATURE NUMBER(5,2) NULL,    
		SHUTIN_HOURS NUMBER(5) NULL,  
		SHUTIN_MIN NUMBER(5) NULL,  
		DATUM_PRESS NUMBER(5) NULL,     
		REMARKS_IND CHAR(1) NULL,       
		TUBING_SIZE_DESC CHAR(12) NULL, 
		PACKER_DEPTH NUMBER(7,2) NULL,   
		DURATION_HOURS NUMBER(5) NULL,       
		DURATION_MIN NUMBER(5) NULL,    
		SEA_LEVEL_DATUM CHAR(5) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_PRESS_BHP TO PUBLIC;

DOC;
LOADER_FAULT_DATA - This data is loaded seperately so than it can 
selectively be moved into the well_tops table.
# 
	CREATE TABLE LOADER_FAULT_DATA (
		UWI		CHAR(20) NOT NULL,
		SOURCE		CHAR(12) NOT NULL,
		FAULT_NAME	CHAR(12) ,
		FAULT_TYPE	CHAR(8),
		TOP		NUMBER(7),
		BASE		NUMBER(7),
		TOP_TVD		NUMBER(7),
		BASE_TVD	NUMBER(7),
		TOP_AGE		CHAR(6),
		BASE_AGE	CHAR(6),
		TOP_FORM	CHAR(12),
		BASE_FORM	CHAR(12),
		REMARKS		CHAR(240)); 

GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_FAULT_DATA TO PUBLIC;
DOC;
WELL_HDR - Many of the WHCS records do not contain complete dates. The 
LOADER_WELL_HDR table makes all the date fields (except LAST_UPDATE) a 
character field so that the date information can be put into something 
recognizable by Oracle as a date before being put into its final resting
place in the WELL_HDR table.
#
CREATE TABLE LOADER_WELL_HDR(
     UWI CHAR(20) NOT NULL,
     WELL_NAME CHAR(50) NULL,
     WELL_NUMBER CHAR(20) NULL,
     PRIMARY_SOURCE CHAR(12) NOT NULL,
     NODE_ID NUMBER(10) NULL,
     TOP_DELTA_X NUMBER(15,7) NULL,
     BASE_DELTA_X NUMBER(15,7) NULL,
     BASE_DELTA_X_DIR CHAR(1) NULL,
     TOP_DELTA_Y NUMBER(15,7) NULL,
     BASE_DELTA_Y NUMBER(15,7) NULL,
     BASE_DELTA_Y_DIR CHAR(1) NULL,
     CLASS CHAR(12) NULL,
     OPERATOR CHAR(30) NULL,
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
     SPUD_DATE_DESC CHAR(12)  NULL,
     RIGREL_DESC CHAR(12)  NULL,
     COMP_DATE_DESC CHAR(12)  NULL,
     ONPROD_DESC CHAR(12)  NULL,
     CALC_ONPROD_DESC CHAR(12)  NULL,
     ONINJECT_DESC CHAR(12)  NULL,
     LOG_TD NUMBER(7,2) NULL,
     STATUS_DATE_DESC CHAR(12)  NULL,
     COUNTRY CHAR(3) NULL,
     PROV_ST CHAR(8) NULL,
     COUNTY CHAR(20) NULL,
     PLUGBACK_TD NUMBER(7,2) NULL,
     WATER_DEPTH NUMBER(7,2) NULL,
     CONF_REL_DT_DESC CHAR(12)  NULL,
     DEVIATION_FLAG CHAR(1) NULL,
     WATER_REF CHAR(4) NULL,
     CF_ELEVATION NUMBER(7,2) NULL, 
     PROPRIETARY CHAR(1) NULL,
     ORIG_UNIT CHAR(12) NULL,
     CONF_FLAG CHAR(1) NULL,
     CONF_FORM CHAR(12) NULL,
     DISCOVER_WELL CHAR(1) NULL,
     CONF_DEPTH NUMBER(7,2) NULL,
     FIELD CHAR(12) NULL,
     FIN_DRILL_DESC CHAR(12)  NULL,
     FLDPOOLWILD CHAR(18) NULL,
     FAULT CHAR(1) NULL,
     SHORT_NAME CHAR(20) NULL,
     LAST_UPDATE DATE  NULL,
     LAST_UPTIME_DESC CHAR(12)  NULL,
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
     CONTRACTOR CHAR(20) NULL,
     RIG_NO CHAR(6) NULL,
     LOCATION_TABLE CHAR(30) NULL,
     STORAGE_UNIT CHAR(12) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_WELL_HDR TO PUBLIC;


CREATE TABLE LOADER_FORM_TEST_HDR(
     UWI CHAR(20) NOT NULL,
     SOURCE CHAR(12) NOT NULL,
     TEST_NO CHAR(4) NOT NULL, 
     TEST_TYPE CHAR(12) NOT NULL, 
     ASSIGNED_TEST_NO CHAR(4) NULL,
     TOP NUMBER(7,2) NULL,  
     BASE NUMBER(7,2) NULL, 
     TOP_FORM CHAR(12) NULL,
     TOP_AGE CHAR(6) NULL,
     BASE_FORM CHAR(12) NULL,
     BASE_AGE CHAR(6) NULL,   
     TEST_COMP CHAR(12) NULL,
     TEST_DATE DATE  NULL,
     TEST_RESULT CHAR(1) NULL,
     RECORDER_DEPTH NUMBER(7,2) NULL,
     PIPE_ID NUMBER(7,2) NULL,
     CUSHION_TYPE CHAR(12) NULL,
     CUSHION_LEN NUMBER(7,2) NULL,
     CUSHION_AMT NUMBER(7,2) NULL,
     IHP NUMBER(5) NULL,
     FHP NUMBER(5) NULL,
     MAX_HP NUMBER(5) NULL,
     PREFLOW_TIME NUMBER(7,2) NULL,
     PREFLOW_UNIT CHAR(12) NULL,
     ISIT_HOURS NUMBER(5) NULL,
     ISIT_MIN NUMBER(5) NULL,
     ISIP NUMBER(5) NULL,
     MAX_ISIP NUMBER(5) NULL,
     FSIT_HOURS NUMBER(5) NULL,
     FSIT_MIN NUMBER(5) NULL,
     FSIP NUMBER(5) NULL,
     MAX_FSIP NUMBER(5) NULL,
     MAX_GAS_RATE NUMBER(12,2) NULL,
     FIN_GAS_RATE NUMBER(12,2) NULL,
     MAX_OIL_RATE NUMBER(12,2) NULL,
     MAX_WTR_RATE NUMBER(12,2) NULL,
     TEST_REMARKS CHAR(240) NULL,
     MAX_GAS_UNIT CHAR(12) NULL,
     FIN_GAS_UNIT CHAR(12) NULL,
     MAX_OIL_UNIT CHAR(12) NULL,
     MAX_WTR_UNIT CHAR(12) NULL,
     TOP_CHOKE_DESC CHAR(6) NULL, 
     BOTTOM_CHOKE_DESC CHAR(6) NULL,
     TOOL_DIAMETER CHAR(6) NULL,
     SURF_FLOW_PRESS NUMBER(5) NULL,
     OPR_SEQ NUMBER(3,0) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_FORM_TEST_HDR TO PUBLIC;

DOC;
LOADER_CORE_HDR - This table was made necessary by the need to convert 
CORE_DIA to insert the implied decimal and the ANAL_DATE_DESC field should
be checked for the correct date data (in MMDDYY format)
#
	CREATE TABLE LOADER_CORE_HDR(
		UWI CHAR(20) NOT NULL, 
		SOURCE CHAR(12) NOT NULL,       
		CORE_ID CHAR(12) NOT NULL,      
		OPR_CORE_NO CHAR(6) NULL,
		CORE_DATE DATE NULL, 
		TOP NUMBER(7,2) NULL,   
		BASE NUMBER(7,2) NULL,  
		PRIMARY_FORM CHAR(12) NULL,
		PRIMARY_AGE CHAR(6) NULL,  
		RECOVERED NUMBER(7,2) NULL,      
		REC_UNIT CHAR(12) NULL,
		CORE_TYPE CHAR(12) NULL,
		CORE_DIA NUMBER(8,3) NULL,
		CORE_FLUID CHAR(12) NULL,       
		CORE_HAND CHAR(12) NULL,
		ANL_REPT CHAR(4) NULL, 
		CORE_DIG_AVAIL CHAR(1) NULL,    
		SHOW_TYPE CHAR(12) NULL,
		CORE_OR_SPL CHAR(4) NULL,       
		OPR_SEQ CHAR(3) NULL,  
		LAB_CODE CHAR(12) NULL,
		LAB_FILE_NO CHAR(15) NULL,      
		ANAL_DATE_DESC CHAR(12) NULL, 
		ANALYST CHAR(12) NULL, 
		GAMMA_CORR CHAR(1) NULL,
		SAMPLE_SHAPE CHAR(12) NULL,     
		SAMPLE_DIA NUMBER(3) NULL,      
		SAMPLE_LEN NUMBER(3) NULL,      
		SOLVENT CHAR(2) NULL,  
		EXT_TIME NUMBER(3) NULL,
		DRY_TIME NUMBER(3) NULL,
		DRY_TEMPERATURE NUMBER(5,2) NULL,
		ORIG_UNIT CHAR(12) NULL,
		REMARKS CHAR(240) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_CORE_HDR TO PUBLIC;

DOC;
LOADER_CORE_ANAL - The core analysis data are all reported with implied
decimals.  These have to be converted before the data can be inserted into
WELL_CORE_ANAL...
#
CREATE TABLE LOADER_CORE_ANAL(
		UWI CHAR(20) NOT NULL, 
		SOURCE CHAR(12) NOT NULL,       
		CORE_ID CHAR(12) NOT NULL,      
		SAMPLE_NO NUMBER(3) NOT NULL,      
		TOP NUMBER(9,2) NULL,   
		ANAL_ID CHAR(2) NULL,
		DEPTH NUMBER(9,2) NULL, 
		LENGTH NUMBER(7,2) NULL,
		KMAX NUMBER(12,4) NULL,  
		K90 NUMBER(12,4) NULL,   
		KVERT NUMBER(12,4) NULL, 
		POROSITY NUMBER(9,4) NULL,       
		GRAIN_DENSITY NUMBER(9,4) NULL,  
		BULK_DENSITY NUMBER(9,4) NULL,
		BULK_MASS_OIL NUMBER(9,4) NULL,  
		BULK_MASS_WAT NUMBER(9,4) NULL,  
		BULK_MASS_SAND NUMBER(9,4) NULL, 
		PORE_VOL_OIL NUMBER(9,4) NULL,   
		PORE_VOL_WAT NUMBER(9,4) NULL,   
		BULK_VOL_OIL NUMBER(9,4) NULL,   
		BULK_VOL_WAT NUMBER(9,4) NULL,   
		GRAIN_MASS_OIL NUMBER(9,4) NULL, 
		GRAIN_MASS_WAT NUMBER(9,4) NULL, 
		LITHOLOGY CHAR(32) NULL,
		EFFECTIVE_POROSITY NUMBER(9,4) NULL,
		GAS_SAT_VOL NUMBER(9,4) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_CORE_ANAL TO PUBLIC;
DOC;
LOADER_WELL_LOCATIONS - Needed to load location data based on UWI intead of 
node_id so that the node_ids assigned by the node loader can be used to 
update this table before the data are moved into WELL_LOCATIONS...
#
	CREATE TABLE LOADER_WELL_LOCATIONS(
		UWI		CHAR(20) NOT NULL,
		NODE_ID		NUMBER(10),
		SOURCE		CHAR(12),
		TOWNSHIP	NUMBER(3),
		NORTH_SOUTH	CHAR(1),
		RANGE		NUMBER(3),
		EAST_WEST	CHAR(1),
		SECTION		NUMBER(3),
		SECTION_TYPE	CHAR(4),
		SPOT_CODE	CHAR(10),
		PRINCIPAL_MERIDIAN	NUMBER(3),
		NS_FOOTAGE	NUMBER(7,2),
		NS		CHAR(3),
		EW_FOOTAGE	NUMBER(7,2),
		EW		CHAR(3),
		DESCRIPTION	CHAR(240));
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_WELL_LOCATIONS TO PUBLIC;
DOC;
LOADER_WELL_TOPS - Too many tops where being lost when records containing
duplicate tops were thrown out, so all are loaded here and then moved over...
#
		CREATE TABLE LOADER_WELL_TOPS(
                       UWI CHAR(20) NOT NULL,
                       FORM_CODE CHAR(12) NOT NULL,
                       SOURCE CHAR(30) NULL,
                       MD NUMBER(7,2) NULL,
                       AGE_CODE CHAR(6) NULL,
                       TVD NUMBER(7,2) NULL,
                       PICK_REF CHAR(2) NULL,
                       TVD_METHOD CHAR(6) NULL,
                       LAST_UPDATE DATE NULL,
                       QUAL_CODE CHAR(1) NULL,
                       DESC_CODE CHAR(4) NULL,
                       FAULTED CHAR(1) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_WELL_TOPS TO PUBLIC;

DOC;
LOADER_TEXAS_LOC - This table is needed because these data have to be 
moved into their final resting place after the nodes loader has been run
and a node_id has been assigned to each well...
#
		CREATE TABLE LOADER_TEXAS_LOC(
                       NODE_ID NUMBER(10) NULL,
                       SOURCE CHAR(12) NOT NULL,
                       SURVEY CHAR(18) NULL,
                       BLOCK CHAR(3) NULL,
                       ABSTRACT_NUMBER CHAR(12) NULL,
                       LOCATION_DESC CHAR(240) NULL,
                       UWI CHAR(20) NULL);
GRANT SELECT,INSERT,UPDATE,DELETE ON LOADER_TEXAS_LOC TO PUBLIC;


EXIT;

