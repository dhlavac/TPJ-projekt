#ifndef SCANER_INCLUDED
#define SCANER_INCLUDED

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "garbage.h"

/* States of the FSM */
typedef enum{
	sStart = 0,			// 00 - Start
	sIdent,				// 01 - Identificator			
	sInteger,			// 02 - Numeral value
	sSlash	,			// 03 - / 
	sString,			// 04 - String literal
	sBooleanType,		// 05 - true/false
	sAssign,			// 06 - = 
	sLComment,			// 07 - // 
	sColon,				// 08 - : 
	sRPar,				// 09 - > 
	sLStartPar,			// 10 - < 
	sLEndPar,			// 11 - </
	sEndofFile,			// 12 - End of File
	sKeyWord,			// 13 - Key words
	sError,				// 14 - Lexical error
	sEnd, 				// 15 - End state
	sEscSeq,			// 16 - Escape sequestion
	sTypes				// 17 - Type - list symbol boolean 
} tState;

/* Struct for token */
typedef struct{
	int id;
	char *attribute;
} tToken;

/* Global variable represnt lexical token */
extern tToken token;

/* Lexical error */
ERROR_CODE scaner_error;

/* Declaration get_Token function*/
tToken get_Token();

#endif