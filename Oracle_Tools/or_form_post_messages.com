$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MESSAGES.COM
$! *1    13-NOV-1990 11:23:48 VINCE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MESSAGES.COM
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MESSAGES.COM
$! *1     6-MAR-1990 08:50:19 GILLESPIE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MESSAGES.COM
$ !************************************************************************
$ !
$ ! FILE = OFORM_POST_MESSAGES.COM
$ !
$ !	This command file is run after the oform command file utility
$ !	is used to activate the messages form.
$ !
$ !	The utility to regenerate the err.h and msg.h files is run
$ !	and the .h files are replaced in the installed include files
$ !	directory.
$ !
$ !************************************************************************
$ !
$	SET NOON
$	SAVE_DIR = F$ENVIRONMENT("DEFAULT")
$	LIBRARIAN_ACCOUNT = F$TRNLNM("ESI$LIBRARIAN_ACCOUNT")
$ !
$ !				Set default to scratch dir, and clean files.
$ !
$	SET DEF ESI$ROOT:[TMP]
$	IF F$SEARCH("*ERR.H") .NES. "" THEN DELETE *ERR.H;*/NOLOG
$	IF F$SEARCH("*MSG.H") .NES. "" THEN DELETE *MSG.H;*/NOLOG
$	IF F$SEARCH("*ERR.I") .NES. "" THEN DELETE *ERR.I;*/NOLOG
$	IF F$SEARCH("*MSG.I") .NES. "" THEN DELETE *MSG.I;*/NOLOG
$ !
$ !				Generate include files for changed modules.
$ !
$	MG_CONFIG = "$ESI$BIN:MG_CONFIG
$	MG_CONFIG 'LIBRARIAN_ACCOUNT' -A=ESI
$ !
$ !				If no include files generated, quit.
$ !
$	IF F$SEARCH("*ERR.H") .NES. "" THEN GOTO NEW_FILES_GENERATED
$	IF F$SEARCH("*MSG.H") .NES. "" THEN GOTO NEW_FILES_GENERATED
$	IF F$SEARCH("*ERR.I") .NES. "" THEN GOTO NEW_FILES_GENERATED
$	IF F$SEARCH("*MSG.I") .NES. "" THEN GOTO NEW_FILES_GENERATED
$	GOTO DONE
$ NEW_FILES_GENERATED:
$ !
$ !				Construct list of generated file names.
$ !
$	DIR/OUT=OFORM_POST_MESSAGES.000/NOHEAD/NOTRAIL/COL=1 -
    		*ERR.H,*MSG.H,*ERR.I,*MSG.I
$	SET PROT=(S:WERD,O:WERD,G:WERD,W:WERD) OFORM_POST_MESSAGES.000
$ !
$ !				Access list of generated file names.
$ !
$	OPEN/READ DIR_FILE OFORM_POST_MESSAGES.000
$ !
$ DIR_LOOP:
$ !
$ !				Get next generated file name.
$ !
$	READ/END=EOF_DIR_FILE DIR_FILE FULL_FILE_NAME
$ !
$ !				Allow world access to file.
$ !
$	SET PROT=(S:WERD,O:WERD,G:WERD,W:WERD) 'FULL_FILE_NAME'
$ !
$ !				Strip everything except file name and type.
$ !
$	FILE_NAME = F$PARSE(FULL_FILE_NAME,,,"NAME","SYNTAX_ONLY")
$	FILE_TYPE = F$PARSE(FULL_FILE_NAME,,,"TYPE","SYNTAX_ONLY")
$	FILE = FILE_NAME + FILE_TYPE
$ !
$ !				Delete old configured version of this file
$ !				if it exists.
$ !
$	INCLUDE_FILE = "ESI$GEN:" + FILE
$	IF F$SEARCH(INCLUDE_FILE) .NES. "" THEN DELETE 'INCLUDE_FILE';
$ !
$ !				Configure generated version of file.
$ !
$	COPY 'FILE' ESI$GEN
$	DELETE 'FILE';
$ !
$ !				Loop for all generated files.
$ !
$ 	GOTO DIR_LOOP
$ !
$ EOF_DIR_FILE:
$ !
$ !				Deaccess file list, and delete it.
$ !
$	CLOSE DIR_FILE
$!	DELETE OFORM_POST_MESSAGES.000;*
$ !
$ DONE:
$ !
$ !				Restore defaults, and exit.
$ !
$	SET DEFAULT 'SAVE_DIR'
$	EXIT
