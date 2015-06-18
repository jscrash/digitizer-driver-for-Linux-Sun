remark This command script is used by the ld_tobin_assemble procedure
remark to enhance performance during loads by dropping non-essential
remark and/or unique indexes.
remark
remark The first and only argument is the project name
remark
drop index &1..LYNX_CULTURE_DATA_TYPE ;
drop index &1..LYNX_CULTURE_MAXX      ;
drop index &1..LYNX_CULTURE_MAXY      ;
drop index &1..LYNX_CULTURE_MINX      ;
drop index &1..LYNX_CULTURE_MINY      ;
drop index &1..LYNX_CULTURE_NAME      ;
drop index &1..LYNX_TEXT_NODE_ID      ;
drop index &1..NODES_NODE_Y           ;
drop index &1..SECTIONS_MERIDIAN      ;
drop index &1..SECTIONS_RANGE         ;
drop index &1..SECTIONS_TOWNSHIP      ;
exit
