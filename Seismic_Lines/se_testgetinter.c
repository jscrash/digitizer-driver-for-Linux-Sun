/* DEC/CMS REPLACEMENT HISTORY, Element SE_TESTGETINTER.C */
/* *1    13-NOV-1990 11:26:20 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_TESTGETINTER.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_TESTGETINTER.C */
/* *1     6-MAR-1990 08:28:45 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_TESTGETINTER.C */

/*********************************************************/
/*       main used to  test intersection routines 

*/

/*   we pass the login string and the name of the survey  on
      the command line activating this programm    
*/

#include "esi_gl_defs.h"
#include "esi_ho.h"
#include "esi_ll.h"
#include "intrpdef.h"
#include "esi_qs.h"

extern INT se_getinters () ;


	main ( argc ,argv )
INT argc ;
CHAR **argv ;

{

INT status , nb_lines ;
BOOL all , no_dup  ;
CHAR **pchar ;
DOUBLE cp1 ,cp2 ;
CHAR time_str [ 100] ;
INT nbc ;
CHAR line_nam [30] , project [30]  ;
INT first_cdp , last_cdp  ;
LISTHEAD *list_int ;


/* argc must be equals to 3 
  the first argument is the login string
  the second argument is the project name 
*/

       if ( argc < 3 )   /* not enough  arguments so we abort */
       {
       printf ( " impossible to run the program without login string and survey name ") ;
       exit (1) ;
       }
       else
       {
       /* login into oracle   */

       status = or_login ( 2 , argv ) ;
     
       if ( status ) 
	{
       printf ( " impossible to login  - verify string user/password "); 
	exit (1) ;
        }
       else
	{

	pchar = argv + 2 ;
         strcpy ( project , *pchar ) ;

       status = qs_set_c ( QS_PROJECT_NAME , project , NULL ) ;


/*    interactively ask for a line name  + bounds in cdp and retrieve the intersc
     ections      
*/

        while ( 1)
       {
        printf ( " enter line_name  or stop to exit the loop : \n") ;
  
        scanf ( "%s",line_nam ) ;

         if ( ! strcmp ( line_nam , "stop")) break ;
         printf ( "enter firstcdp and lastcdp for range selection \n",
                  "  0 means no selection ");

         scanf ("%d %d" , &first_cdp , &last_cdp ) ;

         if ( first_cdp == 0 && last_cdp == 0 )
             {
              first_cdp = FLAG_NO_RANGE ;
              last_cdp = FLAG_NO_RANGE ;
                }

        se_getinters ( line_nam , first_cdp , last_cdp , &list_int ) ;

        }


	exit (0) ;
        }
     }
  }

       
