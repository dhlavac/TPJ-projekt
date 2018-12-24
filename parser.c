/*
 * File: parser.c 
 * Description: Implementation of recursive syntax analysys
 */

#include "parser.h"

/*
 * Check for left <
 * Return: error code or ok
 */
ERROR_CODE lPar() {
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// if there is <
		case  sLStartPar:
			return OK;
		default:
			return SYN_ERR;
		break;
	}

  return OK;
}

/*
 * Check for left </
 * Return: error code or ok
 */
ERROR_CODE lEndPar() {
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// if there is <
		case  sLEndPar:
			return OK;
		default:
			return SYN_ERR;
		break;
	}

  return OK;
}

/*
 * Check for left >
 * Return: error code or ok
 */
ERROR_CODE rPar() {
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// if there is >
		case  sRPar:
			return OK;
		default:
			return SYN_ERR;
		break;
	}

  return OK;
}

/*
 * Check for entered keyword
 * Return: error code or ok
 */

ERROR_CODE check(int code) {
	token = get_Token();
	debug(token);
	if (token.id == sError){
		return LEX_ERR;
	}
	else if (token.id == sEndofFile){
		return SYN_ERR;
	}
	else if (token.id == code){
		return OK;
	}
	else{
		return SYN_ERR;
	}
}

// /*
//  * Check for left >
//  * Return: error code or ok
//  */
// ERROR_CODE KPartitioning() {
// 	token = get_Token();
// 	debug(token);
// 	switch (token.id) {
// 		// Lexical error
// 		case sError: 
// 			return LEX_ERR;
// 		break;
// 		// End of file
// 		case sEndofFile: 
// 			return SYN_ERR;
// 		break;
// 		// if there is "partitoining"
// 		case  K_PARTITIONING:
// 			return OK;
// 		default:
// 			return SYN_ERR;
// 		break;
// 	}	

//   return OK;
// }

/*
 * Check for left rule -> <partitioning> DRIVE_LIST </partitioning> 
 * Return: error code or ok
 */
ERROR_CODE partitioning() {
	printf("\t\t\t\t\t----PARTITIONING-----\n");
	ERROR_CODE error = OK;
	if ((error = lPar()) != OK)
		return error;
	// there is: <partitioning
	if ((error = check(K_PARTITIONING)) != OK){
		return error;
	}
	// there is: <partitioning>
	if ((error = rPar()) != OK){
		return error;
	}

	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: <partitioning> DRIVE_LIST </
		case  sLEndPar:
			error = OK;
		break;
		// next rule
		case sLStartPar:
			error = drive_list();
		// something else error
		break;
		default:
			return SYN_ERR;
		break;
	}

	if (error != OK) {
		return error;
	}
	// osetrenie ci main bol deklarovany a ci su vsetky funkcie definovane
   // error =  finalFunctionCheckout(&symbol_table);

	// if (error != OK_ERR) {
	//     return error;
	// }

	// there is: <partitioning> DRIVE_LIST </partitioning
	if ((error = check(K_PARTITIONING)) != OK){
		return error;
	}
	// there is: <partitioning> DRIVE_LIST </partitioning>
	if ((error = rPar()) != OK){
		return error;
	}

	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
}


/*
 * Check for left rule ->  <drive> DEVICE INITIALIZE PARTITION_CONFIG PESIZE PARTITION_TYPE USE </drive> DRIVE_LIST or .
 * Return: error code or ok
 */
ERROR_CODE drive_list() {
	printf("\t\t\t\t\t----DRIVE_LIST	-----\n");
	ERROR_CODE error = OK;
	// there is: <drive
	if ((error = check(K_DRIVE)) != OK){
		return error;
	}
	// there is: <drive>
	if ((error = rPar()) != OK){
		return error;
	}
	// there is DEVICE
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = device()) != OK){
		return error;
	}

	// there is INITIALIZE
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = initialize()) != OK){
		return error;
	}

	// there is PARTITION_CONFIG
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = partition_config()) != OK){
		return error;
	}

	// there is PSIZE
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = pesize()) != OK){
		return error;
	}

	// there is PARTITION_TYPE
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = partition_type()) != OK){
		return error;
	}

	// there is USE
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = use()) != OK){
		return error;
	}

	// there is: <drive> DRIVE_LIST </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <drive> DRIVE_LIST </drive
	if ((error = check(K_DRIVE)) != OK){
		return error;
	}
	// there is: <drive> DRIVE_LIST </drive>
	if ((error = rPar()) != OK){
		return error;
	}
	
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: </
		case  sLEndPar:
			return OK;
		break;
		// next rule
		case sLStartPar:
			error = drive_list();
		// something else error
		break;
		default:
			return SYN_ERR;
		break;
	}
	if (error != OK) {
		return error;
	}

	return OK;
}

/*
 * Check for left rule ->  DEVICE -> <device> id </device> .
 * Return: error code or ok
 */
ERROR_CODE device() {
	printf("\t\t\t\t\t----DEVICE	-----\n");
	ERROR_CODE error = OK;
	// there is: <device
	if ((error = check(K_DEVICE)) != OK){
		return error;
	}
	// there is: <device>
	if ((error = rPar()) != OK){
		return error;
	}

	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: identificator
		case  sString:
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
	if (error != OK) {
		return error;
	}

	// there is: <device> id </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <device> id </device
	if ((error = check(K_DEVICE)) != OK){
		return error;
	}
	// there is: <device> id </device>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> PS_TYPE -> id . or PS_TYPE -> .
 * Return: error code or ok
 */
ERROR_CODE ps_type() {
	printf("\t\t\t\t\t----PS_TYPE	-----\n");
	ERROR_CODE error = OK;

	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: <pesize></
		case  sString:
			error = OK;
		break;
		case  sLEndPar:
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <pesize> id </
	if ((error = lEndPar()) != OK){
		return error;
	}

	return OK;
}

/*
 * Check for left rule ->  USE -> <use> USE_TYPE </use> .
 * Return: error code or ok
 */
ERROR_CODE use() {
	printf("\t\t\t\t\t----USE	-----\n");
	ERROR_CODE error = OK;
	// there is: <use
	if ((error = check(K_USE)) != OK){
		return error;
	}
	// there is: <use>
	if ((error = rPar()) != OK){
		return error;
	}

	if ((error = use_type()) != OK){
		return error;
	}

	// there is: <use> id </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <use> id </use
	if ((error = check(K_USE)) != OK){
		return error;
	}
	// there is: <use> id </use>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> USE_TYPE -> all or no
 * Return: error code or ok
 */
ERROR_CODE use_type() {
	printf("\t\t\t\t\t----USE_TYPE	-----\n");
	// there is: all or no
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: <pesize></
		case  K_ALL:
			return OK;
		break;
		case  K_NO:
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
}


/*
 * Check for left rule ->  PESIZE -> <pesize> PS_TYPE </pesize> .
 * Return: error code or ok
 */
ERROR_CODE pesize() {
	printf("\t\t\t\t\t----PESIZE	-----\n");
	ERROR_CODE error = OK;
	// there is: <pesize
	if ((error = check(K_PESIZE)) != OK){
		return error;
	}
	// there is: <pesize>
	if ((error = rPar()) != OK){
		return error;
	}

	if ((error = ps_type()) != OK){
		return error;
	}
	// there is: <pesize> id/none </pesize
	if ((error = check(K_PESIZE)) != OK){
		return error;
	}
	// there is: <device> id </device>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  INITIALIZE -> <initialize CONFIG_TYPE boolean > BOOLEAN </initialize> .
 * Return: error code or ok
 */
ERROR_CODE initialize() {
	printf("\t\t\t\t\t----INITIALIZE	-----\n");
	ERROR_CODE error = OK;
	// there is: <initialize
	if ((error = check(K_INITIALIZE)) != OK){
		return error;
	}
	// there is: <initialize CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}

	// there is: <initialize CONFIG_TYPE boolean
	if ((error = check(K_BOOLEAN)) != OK){
		return error;
	}

	// there is: <initialize CONFIG_TYPE boolean >
	if ((error = rPar()) != OK){
		return error;
	}
	/// there is: <initialize CONFIG_TYPE boolean > BOOLEAN
	if ((error = boolean()) != OK){
		return error;
	}

	// there is: <initialize CONFIG_TYPE boolean > BOOLEAN </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <initialize CONFIG_TYPE boolean > BOOLEAN </initialize
	if ((error = check(K_INITIALIZE)) != OK){
		return error;
	}
	// there is: <initialize CONFIG_TYPE boolean > BOOLEAN </initialize>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}




/*
 * Check for left rule ->  CONFIG_TYPE -> config:type= .
 * Return: error code or ok
 */
ERROR_CODE config_type() {
	printf("\t\t\t\t\t----CONFIG_TYPE	-----\n");
	ERROR_CODE error = OK;
	// there is: config
	if ((error = check(K_CONFIG)) != OK){
		return error;
	}
	// there is: config:
	if ((error = check(sColon)) != OK){
		return error;
	}
	// there is: config:type
	if ((error = check(K_TYPE)) != OK){
		return error;
	}
	// there is: config:type=
	if ((error = check(sAssign)) != OK){
		return error;
	}
	return OK;
}


/*
 * Check for left rule ->  BOOLEAN -> true or false
 * Return: error code or ok
 */
ERROR_CODE boolean() {
	printf("\t\t\t\t\t----BOOLEAN	-----\n");
	ERROR_CODE error = OK;
	// there is: true/false
	if ((error = check(sBooleanType)) != OK){
		return error;
	}
	return OK;
}


/*
 * Check for left rule ->  PARTITION_TYPE -> <type CONFIG_TYPE symbol> id </type> .
 * Return: error code or ok
 */
ERROR_CODE partition_type() {
	printf("\t\t\t\t\t----PARTITION_TYPE	-----\n");
	ERROR_CODE error = OK;
	// there is: <type
	if ((error = check(K_TYPE)) != OK){
		return error;
	}
	// there is: <type CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}

	// there is: <type CONFIG_TYPE symbol
	if ((error = check(K_SYMBOL)) != OK){
		return error;
	}

	// there is: <type CONFIG_TYPE symbol>
	if ((error = rPar()) != OK){
		return error;
	}

	// there is: <type CONFIG_TYPE symbol> id
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: string
		case  sString:
			error =  OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	if (error != OK){
		return error;
	}

	// there is: <type CONFIG_TYPE symbol> id </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <type CONFIG_TYPE symbol> id </type
	if ((error = check(K_TYPE)) != OK){
		return error;
	}
	// there is: <type CONFIG_TYPE symbol> id </type>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  PARTITION_CONFIG -> <partitions CONFIG_TYPE PARTITIONS_TYPE >  PARTITION_LIST </partitions> .
 * Return: error code or ok
 */
ERROR_CODE partition_config() {
	printf("\t\t\t\t\t----PARTITION_CONFIG	-----\n");
	ERROR_CODE error = OK;
	// there is: <partitions
	if ((error = check(K_PARTITIONS)) != OK){
		return error;
	}
	// there is: <partitions CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE
	if ((error = partitions_type()) != OK){
		return error;
	}

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE >
	if ((error = rPar()) != OK){
		return error;
	}

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE >  PARTITION_LIST

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE >  PARTITION_LIST </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE >  PARTITION_LIST </partitions
	if ((error = check(K_PARTITIONS)) != OK){
		return error;
	}
	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE >  PARTITION_LIST </partitions>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> PARTITIONS_TYPE -> list / single
 * Return: error code or ok
 */
ERROR_CODE partitions_type() {
	printf("\t\t\t\t\t----PARTITIONS_TYPE	-----\n");
	// there is: list or single
	token = get_Token();
	debug(token);
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: <pesize></
		case  K_SINGLE:
			return OK;
		break;
		case  K_LIST:
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
}




const char * debug_token(int token_id){
	switch(token_id) {
		case sIdent  :
		  return "---Identifikator";
		  break;
		case sInteger  :
		  return "---Integer";
		  break;
		case sSlash  :
		  return "---Slash";
		  break;
		case sString  :
		  return "---String";
		  break;
		case sBooleanType  :
		  return "---Boolean";
		  break;
		case sAssign  :
		  return "---Rovna sa";
		  break;
		case sColon  :
		  return "---Dvojbodka";
		  break;
		case sRPar  :
		  return "---Zatvorka prava";
		  break;
		case sLStartPar  :
		  return "---Zatvorka lava";
		  break;
		case sLEndPar  :
		  return "---Zatvorka lava konecna";
		  break;
		case sKeyWord  :
		  return "---Klucove slovo";
		  break;
		case sTypes  :
		  return "---Typ";
		  break;
		case K_PARTITIONING  :
		  return "---KPartitioning";
		  break;
		case  K_MOUNT :
		  return "---K_MOUNT";
		  break;
		case   K_TYPE:
		  return "---K_TYPE";
		  break;
		case   K_DRIVE:
		  return "---K_DRIVE";
		  break;
		case   K_DEVICE:
		  return "---K_DEVICE";
		  break;
		case   K_INITIALIZE:
		  return "---K_INITIALIZE";
		  break;
		case   K_CONFIG:
		  return "---K_CONFIG";
		  break;
		case   K_PARTITION:
		  return "---K_PARTITION";
		  break;
		case   K_PARTITIONS:
		  return "---K_PARTITIONS";
		  break;
		 case   K_CREATE:
		  return "---K_CREATE";
		  break;
		case   K_FILESYSTEM:
		  return "---K_FILESYSTEM";
		  break;
		case  K_FORMAT :
		  return "---K_FORMAT";
		  break;
		 case   K_PARTITION_ID:
		  return "---K_PARTITION_ID";
		  break;
		case  K_PARTITION_NR :
		  return "---K_PARTITION_NR";
		  break;
		case   K_SIZE:
		  return "---K_SIZE";
		  break;
		 case  K_STRIPES :
		  return "---K_STRIPES";
		  break;
		case   K_STRIPESSIZE:
		  return "---K_STRIPESSIZE";
		  break;
		case  K_LVM_GROUP :
		  return "---K_LVM_GROUP";
		  break;
		case  K_PESIZE :
		  return "---K_PESIZE";
		  break;
		case K_USE  :
		  return "---K_USE";
		  break;
		case K_ALL  :
		  return "---K_ALL";
		  break;
		case K_NO  :
		  return "---K_NO";
		  break;
		case K_SYMBOL  :
		  return "---K_SYMBOL";
		  break;
		case K_BOOLEAN  :
		  return "---K_BOOLEAN";
		  break;
		case K_LIST  :
		  return "---K_LIST";
		  break;
		case K_SINGLE  :
		  return "---K_SINGLE";
		  break;
		case K_INTEGER  :
		  return "---K_INTEGER";
		  break;
	}
	
}

void debug(tToken token) {
	printf("-----------------\n");
	printf("%s\n", token.attribute);
	//printf("%d\n", token.id);
	printf("%s\n", debug_token(token.id));    
	printf("Line of code %d\n", token.code_line);
}

