/**
 * @info: Errors definition
 * @author: Dominik Hlavac Duran
 */

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

// enumeration of error codes
typedef enum error_code{
    OK      				= 0,
    LEX_ERR 				= 1,
    SYN_ERR					= 2,
    SEM_ERR	        		= 3,
    SEM_DOUBLE_DEF_ERR		= 4,
    SEM_SINGLE_LIST_ERR 	= 5,
    SEM_INVALID_PATH_ERR	= 6,
    NO_FILE_ERR				= 7,
    INTERN_ERR				= 8,
    TOO_MUCH_ARG_ERR		= 9

} ERROR_CODE;

#endif