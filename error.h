/**
 * @info: Errors definition
 */

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
 
typedef enum error_code{
    OK      				= 0,
    LEX_ERR 				= 1,
    SYN_ERR					= 2,
    SEM_ERR	        		= 3,
    SEM_DOUBLE_DEF_ERR		= 4,
    SEM_SINGLE_LIST_ERR 	= 5,
    SEM_INVALID_PATH_ERR	= 6,
    INTERN_ERR				= 7

} ERROR_CODE;

#endif