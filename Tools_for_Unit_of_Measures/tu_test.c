/* DEC/CMS REPLACEMENT HISTORY, Element TU_TEST.C */
/* *1    13-NOV-1990 11:27:11 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element TU_TEST.C */
/* DEC/CMS REPLACEMENT HISTORY, Element TU_TEST.C */
/* *1     6-MAR-1990 08:29:56 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element TU_TEST.C */
#include "esi_gl_defs.h"
#include "esi_oracle.h"
#include "esi_tu.h"
main()
{
   INT status;
   CHAR uom_string[40];
   FLOAT scale1,scale2;
   FLOAT wcs_to_inches;
   CHAR uom1[21],uom2[21];
   CHAR yesno[2];
   INT option;
   UOM_STRUCTURE uom_data;
   
   or_login(0,0);

   strcpy(yesno,"y");
   while(yesno[0] == 'y' || yesno[0] == 'Y')
   {
      printf("Test types are:\n");
      printf("1 = parse unit\n2 = display to wcs\n3 = parse scale\n");
      printf("4 = unit conversion\n5 = verify uom\n6 = scale conversion\n");
      printf("7 = parse scale 2\n");
      printf("\nGive your choice:");
      scanf("%d",&option);
      getchar();
      switch(option)
      {
         case 1:    /* tu_parse_unit  */
            printf("Give a value [uom] string:");
            gets(uom_string);
            status = tu_parse_unit(uom_string,&scale1,uom1,uom2);
            printf("Status: %d\n",status);
            printf("Result: %f   %s  %s\n",scale1,uom1,uom2);
            break;
         case 2:   /* tu_disp_to_wcs  */
            printf("Give a unit string:");
            gets(uom_string);
            printf("Give an inches to wcs scale factor:");
            scanf("%f",&wcs_to_inches);
            getchar(); 
            status = tu_disp_units_to_wcs(uom_string,&scale1,wcs_to_inches);
            printf("Status: %d\n",status);
            printf("Result: %f\n",scale1);
            break;
         case 3:    /* tu_parse_scale  */
            printf("Give a scale string:");
            gets(uom_string);
            status = tu_parse_scale(uom_string,&scale1,uom1);
            printf("Status: %d\n",status);
            printf("Result: %f  %s\n",scale1,uom1);
            break;
         case 4:    /* tu_unit_conversion */
            printf("Give the input value, units in & units out:");
            scanf("%f %s %s",&scale1,uom1,uom2);
            getchar(); 
            status = tu_unit_conversion(scale1,uom1,&scale2,uom2);
            printf("Status: %d\n",status);
            printf("Result: %f %s\n",scale2,uom2);
            break;
         case 5:   /* tu_verify_uom  */
            printf("Give the uom:");
            gets(uom1);
            status = tu_verify_uom(uom1,&uom_data);
            printf("Status: %d\n",status);
            printf("Conversion factor: %f    Type: %s\n",
                  uom_data.uom_definition,uom_data.uom_type);
            break;
         case 6:    /* tu_scale_conversion  */
            printf("Give the WCS units:");
            gets(uom1);
            printf("Give a scale string:");
            gets(uom_string);
            status = tu_scale_conversion(uom1,uom_string,&scale1);
            printf("Status: %d\n",status);
            printf("Result: %f\n",scale1);
            break;  
         case 7:    /* tu_parse_scale  */
            printf("Give a scale string:");
            gets(uom_string);
            status = tu_parse_scale2(uom_string,&scale1,uom1,uom2);
            printf("Status: %d\n",status);
            printf("Result: %f  %s  %s\n",scale1,uom1,uom2);
            break;
         default:  
            printf("Unknown option\n");
            break;
      }
      printf("Do you want another test(y or n)? ");
      gets(yesno);
   }
}
