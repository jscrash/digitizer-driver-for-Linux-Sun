/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_ENV.C */
/* *3     4-JAN-1991 13:14:04 GILLESPIE "(SPR 1) Add esi_ts.h" */
/* *2     4-JAN-1991 10:48:21 GILLESPIE "(SPR 1) Move environment determination from ho_set_log.pc" */
/* *1     3-JAN-1991 16:49:19 GILLESPIE "Routine to return character string of OS environment" */
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_ENV.C */
#include "esi_c_lib.h"
#include "esi_ho.h"
#include "esi_ts.h"

#if USE_PROTOTYPES
publicdef INT ho_get_environment(char *environment)
#else
publicdef INT ho_get_environment(environment)
char *environment;
#endif
	{

	*environment = NUL;
#ifdef SUN4
    strcpy (environment, "SUN4");
#endif

#ifdef SUN3
    strcpy (environment, "SUN3");
#endif

#ifdef mips
    strcpy (environment, "ULTRIXR");
#endif

#ifdef vms
    strcpy (environment, "VMS");
#endif

#ifdef primos
    strcpy (environment, "PRIMOS");
#endif

#ifdef applec
    strcpy (environment, "MPW");
#endif

#ifdef THINK_C
    strcpy (environment, "THINK_C");
#endif

	if (IS_EMPTY_STRING(environment))
		{
		strcpy(environment,"UNKNOWN");
		return FAIL;
		}
	else
		{
		return SUCCESS;
		}
	}
