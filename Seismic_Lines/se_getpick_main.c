/* DEC/CMS REPLACEMENT HISTORY, Element SE_GETPICK_MAIN.C */
/* *1    13-NOV-1990 11:26:18 JULIAN "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_GETPICK_MAIN.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_GETPICK_MAIN.C */
/* *1     6-MAR-1990 08:28:42 GILLESPIE "initial release" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_GETPICK_MAIN.C */

/*********************************************************/
/*       main used to run test pick retrieval 

*/

/*   we pass the login string and the name of the survey  on
      the command line activating this programm    
*/

#include "esi_gl_defs.h"
#include "intrpdef.h"
#include "esi_qs.h"



	main ( argc ,argv )
INT argc ;
CHAR **argv ;

{

INT status , nb_lines ;
BOOL all , no_dup  ;
CHAR **pchar ;
DOUBLE cp1 ,cp2 ;
CHAR time_str [ 100] ;
INT nbc ,nb_word , nb_picks , type_of_section ,cdp_inc , icoef ;
CHAR line_nam [30],horname[30],source[30] , project [30]  ;
INT ivect [3000] ;
INT first_cdp , last_cdp  ;

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

        printf ( " enter horizon name or stop to exit the loop : \n") ;
  
        scanf ( "%s",horname ) ;

         if ( ! strcmp ( line_nam , "stop")) break ;



          nb_word = 3000 ;
          strcpy ( source , "PFL") ;

 se_get_hor_pick ( line_nam , horname ,	source , ivect , nb_word , &nb_picks , type_of_section 
                    , &first_cdp , &last_cdp , &cdp_inc , &icoef )  ;


        }


	exit (0) ;
        }
     }
  }

       
