/* DEC/CMS REPLACEMENT HISTORY, Element LD_WHCS_MOVE_TABS.SQL */
/* *3     8-MAY-1991 14:49:12 LOEBL "(SPR 0) restructured multiple update statements" */
/* *2    21-DEC-1990 14:41:49 ALLAWAY "(SPR 5528) PERFORMANCE OVERHAUL" */
/* *1    21-DEC-1990 14:40:59 ALLAWAY "post loading data moving script" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_WHCS_MOVE_TABS.SQL */
DOC;
ld_whcs_move_tabs.sql Bill Allaway 3/5/90
This script is intended to move the data loaded into loader tables into
their final resting places.

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

--	SELECT DISTINCT SPUD_DATE_DESC FROM LOADER_WELL_HDR;
--	SELECT DISTINCT COMP_DATE_DESC FROM LOADER_WELL_HDR;
#

DEFINE PERMIAN_FLAG = &1
set echo on
set timing on
spool ld_whcs_move_tabs

REMARK First update base_delta_x to be negative if west of the well...
REMARK then  update base_delta_y to be negative if south of the well...
REMARK then  reset deviation_flag to normal...
remark  

REMARK
REMARK Do the following ALTER TABLE to prevent row chaining.
REMARK

ALTER TABLE WELL_HDR PCTFREE 5;

REMARK Now preserve the original engineering offsets if any were loaded.
REMARK You may prefer to use these instead.

	INSERT INTO WELL_HDR_SECOND (
		UWI,
		SOURCE,
		DEVIATION_FLAG,
		TOP_DELTA_X,
		TOP_DELTA_Y,
		BASE_DELTA_X,
		BASE_DELTA_Y,
		TVD)
	SELECT
		UWI,
		'WHCS-66001',
		decode(deviation_flag, null, null, '-' ),
		TOP_DELTA_X,
		TOP_DELTA_Y,
		BASE_DELTA_X * decode(deviation_flag,'3',-1,'4',-1, 1),
		BASE_DELTA_Y * decode(deviation_flag,'2',-1,'3',-1, 1),
		TVD
	FROM LOADER_WELL_HDR WHERE BASE_DELTA_X IS NOT NULL;

COMMIT;

REMARK 
REMARK Now move the problem data...dates.
REMARK 

create unique index 
loader_well_hdr_unique on 
loader_well_hdr (uwi);

UPDATE WELL_HDR H SET 
        ( 
           BASE_DELTA_X,
           BASE_DELTA_Y,
           DEVIATION_FLAG,
           SPUD_DATE, 
           COMP_DATE , 
           RIGREL, 
           ONPROD, 
           CALC_ONPROD, 
           ONINJECT, 
           CONF_REL_DT, 
           STATUS_DATE, 
           FIN_DRILL 
        ) = 
        ( SELECT
           BASE_DELTA_X * decode(deviation_flag,'3',-1,'4',-1, 1),
           BASE_DELTA_Y * decode(deviation_flag,'2',-1,'3',-1, 1),
           '-' ,
           TO_DATE( replace(SPUD_DATE_DESC,' ','0'),  'MM/DD/YYYY'), 
           TO_DATE( replace(COMP_DATE_DESC,' ','0'),  'MM/DD/YYYY'),
           TO_DATE( replace(RIGREL_DESC,' ','0'),     'MM/DD/YYYY') ,
           TO_DATE( replace(ONPROD_DESC,' ','0'),     'MM/DD/YYYY') ,
           TO_DATE( replace(CALC_ONPROD_DESC,' ','0'),'MM/DD/YYYY') ,
           TO_DATE( replace(ONINJECT_DESC,' ','0'),   'MM/DD/YYYY') ,
           TO_DATE( replace(CONF_REL_DT_DESC,' ','0'),'MM/DD/YYYY') ,
           TO_DATE( replace(STATUS_DATE_DESC,' ','0'),'MM/DD/YYYY') ,
           TO_DATE( replace(FIN_DRILL_DESC,' ','0'),  'MM/DD/YYYY') 
	   FROM LOADER_WELL_HDR L 
           WHERE L.UWI = H.UWI
        )
        WHERE EXISTS ( SELECT 'X' 
                       FROM LOADER_WELL_HDR L 
                       WHERE L.UWI = H.UWI);
        

COMMIT;

drop index loader_well_hdr_unique;

REMARK WELL_FORM_TEST_HDR - This inserts the data from loader_form_test_hdr into 
REMARK well_form_test_hdr, converting the shut in times from hour and minutes to
REMARK minutes:

create unique index i_loader_form_test_hdr
on loader_form_test_hdr
(UWI , TEST_NO, TEST_TYPE , SOURCE);

UPDATE WELL_FORM_TEST_HDR F SET
        (
         ISIT, 
         ISIT_UNIT, 
         FSIT, 
         FSIT_UNIT
        ) =
        (SELECT 
           (ISIT_HOURS * 60) + ISIT_MIN ,
           'MIN',
	   (FSIT_HOURS * 60) + FSIT_MIN ,
           'MIN'
	 FROM LOADER_FORM_TEST_HDR L
	 WHERE L.UWI = F.UWI 
           AND L.TEST_NO = F.TEST_NO 
	   AND L.TEST_TYPE = F.TEST_TYPE 
           AND L.SOURCE = F.SOURCE
        )
        WHERE EXISTS ( SELECT 'X'
                       FROM LOADER_FORM_TEST_HDR L
                       WHERE L.UWI = F.UWI 
                         AND L.TEST_NO = F.TEST_NO 
		         AND L.TEST_TYPE = F.TEST_TYPE 
                         AND L.SOURCE = F.SOURCE);
COMMIT;

drop index i_loader_form_test_hdr;

REMARK LOADER_CORE_HDR - Update CORE_DIA in WELL_CORE_HDR...
REMARK 
REMARK Updating the Core_dia field to move insert the decimal in the correct place...

prompt Inserting the data into well_core_hdr...

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
                anal_date,
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
		0.001 * CORE_DIA,
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
                decode(anal_date_desc,'      ', null, 
                       to_date( replace(anal_date_desc,' ','0'), 'mmddyy')),
		REMARKS
	FROM LOADER_CORE_HDR;

COMMIT;
                                                          
REMARK 
REMARK Update the anal_date field with the date. if this fails then you will have
REMARK to patch up the data by hand and try again...
REMARK 

REMARK LOADER_CORE_ANAL - Need to update all the data fields with implied decimals
REMARK then move everything into WELL_CORE_ANAL...
REMARK 
REMARK Updating LOADER_CORE_ANAL to put the decimal places in the correct places...
REMARK 

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
		0.1 * TOP,   
		ANAL_ID,
		0.1 * DEPTH, 
		LENGTH,
		0.01 * KMAX,  
		0.01 * K90,   
		KVERT, 
		0.1 * POROSITY,       
		0.01 * GRAIN_DENSITY,  
		0.01 * BULK_DENSITY,
		0.1 * BULK_MASS_OIL,  
		0.1 * BULK_MASS_WAT,  
		BULK_MASS_SAND, 
		0.1 * PORE_VOL_OIL,   
		0.1 * PORE_VOL_WAT,   
		0.1 * BULK_VOL_OIL,   
		0.1 * BULK_VOL_WAT,   
		GRAIN_MASS_OIL, 
		GRAIN_MASS_WAT, 
		LITHOLOGY,
		0.1 * EFFECTIVE_POROSITY,
		0.1 * GAS_SAT_VOL
	FROM LOADER_CORE_ANAL;

COMMIT;

remark 
remark WELL_PRESS_BHP - Move data from loader_press_bhp and convert the times
remark from hours and minutes to minutes and units...
remark 
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

remark 
remark Move the fault data from LOADER_FAULT_DATA into WELL_TOPS where it will show up
remark 
remark First make the fault name something appropriate for the form_code field ...
remark 

remark
remark Next insert the data into the WELL_TOPS table...
remark 
	INSERT INTO WELL_TOPS(
		UWI,
		FORM_CODE,
		SOURCE,
		MD,
		TVD)
	SELECT
		UWI,
                'FAULT'||fault_name,
		SOURCE,
		TOP,
		TOP_TVD
	FROM LOADER_FAULT_DATA;
COMMIT;

REMARK 
REMARK LOADER_PERM_CORE_DESC - Move the data loaded into this table into 
REMARK the WELL_CORE_DESC table.  NOTE: only applies to Permian basin data. 

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
FROM LOADER_PERM_CORE_DESC
where upper('&PERMIAN_FLAG') = 'PERMIAN';


remark WELL_TEST_VO_FLOW - This statement moves data into WELL_TEST_VO_FLOW, converting
remark time to surface from hours and minutes to minutes and units.
remark 

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

remark 
remark LOADER_TEST_VO - Move data from this table to WELL_TEST_VO, converting
remark VOT_HOURS and VOT_MINUTES to minutes and units...
remark 

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

REMARK The following updates the azimuth field to the correct degrees in preparation
REMARK for moving the data into WELL_DIR_SRVY_PTS.  Actually, these data are not 
REMARK used by the Finder software and so will not be inserted into WELL_DIR_SRVY_PTS.
REMARK If these are the only deviation data available for a well, then a program
REMARK should be written to calculate DX,DY,and TVD and those values should be put in
REMARK WELL_DIR_SRVY_PTS.
REMARK 
REMARK First update the SE Quadrant...

	UPDATE LOADER_DEV_SRVY_PTS
	SET AZIMUTH = 
           DECODE( AZIMUTH_NS, 'S', 180, 360 ) + 
              (AZIMUTH * DECODE( AZIMUTH_NS||AZIMUTH_EW, 'SW', 1, -1) )
	WHERE AZIMUTH_NS||AZIMUTH_EW != 'NE';

COMMIT;

REMARK WELL_DIR_SRVY_PTS - The following prepares the data in LOADER_DIR_SRVY_PTS
REMARK for insertion into this table, then moves the data.

REMARK First make x offsets negative if west of the well location...
REMARK Next make y offsets negative if south of the well location...

	UPDATE LOADER_DIR_SRVY_PTS
		SET DX = DX * DECODE( DX_DIR, 'W', -1 , 1),
                    DY = DY * DECODE( DX_DIR, 'S', -1 , 1) 
	WHERE DX_DIR IN ('S','W');

COMMIT;

remark 
remark Now insert the data into WELL_DIR_SRVY_PTS...
remark 

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

remark 
remark Updating of loaded data that requires that the nodes data be loaded before
remark executing it will do any good.
remark 
remark LOADER_WELL_LOCATIONS - GET NODE_IDs then insert data into WELL_LOCATIONS...
remark 
	UPDATE LOADER_WELL_LOCATIONS L SET
		NODE_ID = (SELECT NODE_ID FROM WELL_HDR H
				WHERE H.UWI = L.UWI)
        where exists (SELECT 'x' FROM WELL_HDR H
				WHERE H.UWI = L.UWI);

 COMMIT;

	INSERT INTO WELL_LOCATIONS(
         NODE_ID,
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
         DISTINCT NODE_ID,
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
	FROM LOADER_WELL_LOCATIONS
	WHERE NODE_ID IS NOT NULL
	AND SOURCE IS NOT NULL;
COMMIT;

create index i_loader_well_locations 
on loader_well_locations 
(uwi);

	UPDATE WELL_HDR h set
           LOCATION_TABLE = 'WELL_LOCATIONS'
	WHERE exists (SELECT 'x' FROM LOADER_WELL_LOCATIONS l 
                      where h.uwi = l.uwi );
COMMIT;

drop index i_loader_well_locations ;

remark 
remark LOADER_TEXAS_LOC - GET NODE_IDs then insert data into WELL_TEXAS_LOC...
remark 
	UPDATE LOADER_TEXAS_LOC L SET
		NODE_ID = (SELECT NODE_ID FROM WELL_HDR H
				WHERE H.UWI = L.UWI)
        where exists (SELECT 'x' FROM WELL_HDR H
				WHERE H.UWI = L.UWI);
COMMIT;
	INSERT INTO WELL_TEXAS_LOC(
         NODE_ID,
         SOURCE,
         SURVEY,
         BLOCK,
         ABSTRACT_NUMBER,
         LOCATION_DESC)
	SELECT
         NODE_ID,
         SOURCE,
         SURVEY,
         BLOCK,
         ABSTRACT_NUMBER,
         LOCATION_DESC
	FROM LOADER_TEXAS_LOC;
COMMIT;

create index i_well_texas_loc
on loader_texas_loc
(uwi);


	UPDATE WELL_HDR h  SET LOCATION_TABLE = 'WELL_TEXAS_LOC'
		WHERE exists (SELECT 'x' from LOADER_TEXAS_LOC l 
                              where h.uwi = l.uwi );
COMMIT;

drop index i_well_texas_loc;

spool off

EXIT
