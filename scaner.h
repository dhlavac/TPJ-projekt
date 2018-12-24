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

/*Keywords*/
#define K_PARTITIONING 18		// partitioning
#define K_MOUNT 19				// mount
#define K_TYPE 20				// type
#define K_DRIVE 21				// drive
#define K_DEVICE 22				// Device
#define K_INITIALIZE 23			// initialize
#define K_CONFIG 24				// config
#define K_PARTITION 25			// partition
#define K_PARTITIONS 26			// partitions
#define K_CREATE 27				// create
#define K_FILESYSTEM 28			// filesystem
#define K_FORMAT 29				// format
#define K_PARTITION_ID 30		// partition_id
#define K_PARTITION_NR 31		// partition_nr
#define K_SIZE 32				// size
#define K_STRIPES 33			// stripes
#define K_STRIPESSIZE 34		// stripesize
#define K_LVM_GROUP 35 			// lvm_group
#define K_PESIZE 36				// pesize
#define K_USE 37				// use
#define K_ALL 38				// all
#define K_NO 39					// no
#define K_SYMBOL 40				// symbol
#define K_BOOLEAN 41			// boolean
#define K_LIST 42				// list
#define K_SINGLE 43				// single
#define K_INTEGER 44			// integer
/* Struct for token */
typedef struct{
	int id;
	char *attribute;
	int code_line;
} tToken;

/* Global variable represnt lexical token */
extern tToken token;

/* Lexical error */
ERROR_CODE scaner_error;

/* Declaration get_Token function*/
tToken get_Token();

#endif