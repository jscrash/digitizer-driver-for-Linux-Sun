set echo on
delete from LYNX_ARCS;                                                          
commit;
delete from LYNX_ARC_GC;                                                        
commit;
delete from LYNX_AREA_BUNDLE;                                                   
commit;
delete from LYNX_CULTURE;                                                       
commit;
delete from LYNX_CULTURE_ATTRIBUTES;                                            
commit;
delete from LYNX_DATA_TYPES;                                                    
commit;
delete from LYNX_DTYPE_ARC_GC;                                                  
commit;
delete from LYNX_DTYPE_NODE_GC;                                                 
commit;
delete from LYNX_DTYPE_REGION_GC;                                               
commit;
delete from LYNX_DTYPE_TEXT_GC;                                                 
commit;
delete from LYNX_FEATURE;                                                       
commit;
delete from LYNX_IFV;                                                           
commit;
delete from LYNX_LINE_BUNDLE;                                                   
commit;
delete from LYNX_LRFR;                                                          
commit;
delete from LYNX_NODE_GC;                                                       
commit;
delete from LYNX_OBJECTS;                                                       
commit;
delete from LYNX_REGION_GC;                                                     
commit;
delete from LYNX_SYMBOL_BUNDLE;                                                 
commit;
delete from LYNX_TEXT;                                                          
commit;
delete from LYNX_TEXT_BUNDLE;                                                   
commit;
delete from LYNX_TEXT_GC;                                                       
commit;
delete from sections;
commit;
delete from nodes;
commit;
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_LYNX_CLEAR.SQL*/
/* *2    16-AUG-1991 14:30:22 ALLAWAY "(SPR 8648) initial checkin"*/
/* *1    16-AUG-1991 14:28:35 ALLAWAY "delete the contents of the lynx database tables."*/
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_LYNX_CLEAR.SQL*/
