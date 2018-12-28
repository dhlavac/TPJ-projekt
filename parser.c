/*
 * File: parser.c 
 * Description: Implementation of recursive syntax analysys
 */

#include "parser.h"

// structure for detecting semantic errors
tPartAtrNumber occurance;
// number of partitions in config - used for semantic analysys
int partitions_number;
int partitions_config_type;
/*
 * Check for left <
 * Return: error code or ok
 */
ERROR_CODE lPar() {
	token = get_Token();
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

/*
 * Function start syntax analysys
 * Return: structure with error code and number of line if error occures
 */
tError syntax_analysys(){
	tError error;
	error.error_code = partitioning();
	error.current_line = token.code_line;
	return error;
}

/*
 * Check for left rule -> <partitioning> DRIVE_LIST </partitioning> 
 * Return: error code or ok
 */
ERROR_CODE partitioning() {
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
	ERROR_CODE error = OK;
	// there is: <device
	if ((error = check(K_DEVICE)) != OK){
		return error;
	}
	// there is: <device>
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <device> id 
	token = get_Token();
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
			if ((error = check_path(token.attribute)) != OK){
				return error;
			}
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
	ERROR_CODE error = OK;

	token = get_Token();
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
			error = OK;
		break;
		case  sLEndPar:
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <ps_type> id </
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
	// there is: all or no
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: all
		case  K_ALL:
			return OK;
		break;
		// there is no
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
	token = get_Token();
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
		case  K_SYMBOL:
			error =  OK;
		break;
		case K_INTEGER:
		case K_BOOLEAN:
			return SEM_ERR;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <type CONFIG_TYPE symbol>
	if ((error = rPar()) != OK){
		return error;
	}

	// there is: <type CONFIG_TYPE symbol> id
	token = get_Token();
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
	// set number of partittions to zero
	partitions_number = 0;
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

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE > PARTITION
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = partition()) != OK){
		return error;
	}

	// there is: <partitions CONFIG_TYPE PARTITIONS_TYPE > PARTITION PARTITION_LIST or <partitions CONFIG_TYPE PARTITIONS_TYPE > PARTITION </
	if ((error = partition_list()) != OK){
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
	// semantic analysys for number of declared partitions
	if ((partitions_config_type == SINGLE) && (partitions_number > 1)){
		return SEM_SINGLE_LIST_ERR;
	}

	return OK;
}

/*
 * Check for left rule -> PARTITIONS_TYPE -> list / single
 * Return: error code or ok
 */
ERROR_CODE partitions_type() {
	// there is: list or single
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: single
		case  K_SINGLE:
			partitions_config_type = SINGLE;
			return OK;
		break;
		// there is: list
		case  K_LIST:
			partitions_config_type = LIST;
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
}

/*
 * Check for left rule -> PARTITION -> <partition> PARTITION_CREATE_CONFIG PARTITON_SIZE PARTITION_ATRIBUTES </partition> .
 * Return: error code or ok
 */
ERROR_CODE partition() {
	// set partitions atribute occurance to zero - semantic analysys
	occurance = set_zero_counter(occurance);
	// increment number of partitions or semantic analysys
	partitions_number++;
	ERROR_CODE error = OK;
	// there is:  <partition
	if ((error = check(K_PARTITION)) != OK){
		return error;
	}
	// there is:  <partition>
	if ((error = rPar()) != OK){
		return error;
	}

	// there is:<partition> PARTITION_CREATE_CONFIG
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = partition_create_config()) != OK){
		return error;
	}

	// there is:<partition> PARTITION_CREATE_CONFIG PARTITON_SIZE
	if ((error = lPar()) != OK){
		return error;
	}
	if ((error = partition_size()) != OK){
		return error;
	}

	// there is:<partition> PARTITION_CREATE_CONFIG PARTITON_SIZE PARTITION_ATRIBUTES 
	if ((error = partition_atributes()) != OK){
		return error;
	}

	// there is: <partition> PARTITION_CREATE_CONFIG PARTITON_SIZE PARTITION_ATRIBUTES </partition
	if ((error = check(K_PARTITION)) != OK){
		return error;
	}
	// there is: <partition> PARTITION_CREATE_CONFIG PARTITON_SIZE PARTITION_ATRIBUTES </partition>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> PARTITION_LIST -> PARTITION PARTITION_LIST  or PARTITION_LIST -> .
 * Return: error code or ok
 */
ERROR_CODE partition_list() {
	ERROR_CODE error = OK;
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: <
		case  sLStartPar:
			if ((error = partition()) != OK){
				return error;
			}
		break;
		// there is: </
		case  sLEndPar:
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
	if ((error = partition_list()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  PARTITION_CREATE_CONFIG -> <create CONFIG_TYPE boolean > BOOLEAN </create> .
 * Return: error code or ok
 */
ERROR_CODE partition_create_config() {
	ERROR_CODE error = OK;
	// there is: <create
	if ((error = check(K_CREATE)) != OK){
		return error;
	}
	// there is:<create CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}

	// there is: <create CONFIG_TYPE boolean
	token = get_Token();
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
		case  K_BOOLEAN:
			error =  OK;
		break;
		case K_INTEGER:
		case K_SYMBOL:
			return SEM_ERR;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <create CONFIG_TYPE boolean >
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <create CONFIG_TYPE boolean > BOOLEAN
	if ((error = boolean()) != OK){
		return error;
	}

	// there is: <create CONFIG_TYPE boolean > BOOLEAN </
	if ((error = lEndPar()) != OK){
		return error;
	}


	// there is: <create CONFIG_TYPE boolean > BOOLEAN </create
	if ((error = check(K_CREATE)) != OK){
		return error;
	}
	// there is: <create CONFIG_TYPE boolean > BOOLEAN </create>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> PARTITON_SIZE -> <size> id </size> .
 * Return: error code or ok
 */
ERROR_CODE partition_size() {
	ERROR_CODE error = OK;
	// there is: <size 
	if ((error = check(K_SIZE)) != OK){
		return error;
	}
	// there is: <size>
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <size> id 
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: symbol
		case  sString:
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <size> id </
	if ((error = lEndPar()) != OK){
		return error;
	}
	// there is: <size> id </size
	if ((error = check(K_SIZE)) != OK){
		return error;
	}
	// there is:<size> id </size> 
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> PARTITION_MOUNT -> <mount> id </mount> .
 * Return: error code or ok
 */
ERROR_CODE mount() {
	ERROR_CODE error = OK;
	// there is: <mount>
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <mount> id 
	token = get_Token();
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
			if ((error = check_path(token.attribute)) != OK){
				return error;
			}
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <mount> id </
	if ((error = lEndPar()) != OK){
		return error;
	}
	// there is: <mount> id </mount
	if ((error = check(K_MOUNT)) != OK){
		return error;
	}
	// there is: <mount> id </mount>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> LV_NAME -> <lv_name> id </lv_name> .
 * Return: error code or ok
 */
ERROR_CODE lv_name() {
	ERROR_CODE error = OK;
	// there is: <lv_name>
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <lv_name> id
	token = get_Token();
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
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <lv_name> id </
	if ((error = lEndPar()) != OK){
		return error;
	}
	// there is: <lv_name> id </lv_name
	if ((error = check(K_LV_NAME)) != OK){
		return error;
	}
	// there is: <lv_name> id </lv_name>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> LVM_GROUP -> <lvm_group> id </lvm_group> .
 * Return: error code or ok
 */
ERROR_CODE lvm_group() {
	ERROR_CODE error = OK;
	// there is: <lvm_group>
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <lvm_group> id
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		case sInteger:
		case sBooleanType:
			return SEM_ERR;
		break;
		// there is: string
		case  sString:
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <lvm_group> id </
	if ((error = lEndPar()) != OK){
		return error;
	}
	// there is: <lvm_group> id </lvm_group
	if ((error = check(K_LVM_GROUP)) != OK){
		return error;
	}
	// there is: <lvm_group> id </lvm_group>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}


/*
 * Check for left rule ->  PARTITON_ID -> <partition_id  CONFIG_TYPE ATRIBUTE_TYPE > id </partition_id> .
 * Return: error code or ok
 */
ERROR_CODE partition_id() {
	ERROR_CODE error = OK;
	int tmp_attr_type;
	// there is: <partition_id  CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}
	// there is: <partition_id  CONFIG_TYPE ATRIBUTE_TYPE
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: symbol
		case  K_SYMBOL:
			tmp_attr_type = K_SYMBOL;
			error = OK;
		break;
		// there is: integer
		case  K_INTEGER:
			tmp_attr_type = K_INTEGER;
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <partition_id  CONFIG_TYPE ATRIBUTE_TYPE >
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <partition_id  CONFIG_TYPE ATRIBUTE_TYPE> id
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: symbol
		case  sString:
			if (tmp_attr_type == K_INTEGER){
				return SEM_ERR;
			}
			error = OK;
		break;
		// there is: integer
		case  sInteger:
			if (tmp_attr_type == K_SYMBOL){
				return SEM_ERR;
			}
			error = OK;
		break;
		// wrong types
		case sIdent:
		case sBooleanType:
			return SEM_ERR;
		break;	
		default:
			return SYN_ERR;
		break;
	}

	// there is: <partition_id  CONFIG_TYPE ATRIBUTE_TYPE > id </
	if ((error = lEndPar()) != OK){
		return error;
	}


	// there is: <partition_id  CONFIG_TYPE ATRIBUTE_TYPE > id </partition_id
	if ((error = check(K_PARTITION_ID)) != OK){
		return error;
	}
	// there is: <partition_id  CONFIG_TYPE ATRIBUTE_TYPE > id </partition_id> 
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  PARTITON_NR -> <partition_nr  CONFIG_TYPE integer > id </partition_nr> .
 * Return: error code or ok
 */
ERROR_CODE partition_nr() {
	ERROR_CODE error = OK;
	// there is: <partition_nr  CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}
	// there is: <partition_nr CONFIG_TYPE integer
	token = get_Token();
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
		case  K_INTEGER:
			error =  OK;
		break;
		case K_SYMBOL:
		case K_BOOLEAN:
			return SEM_ERR;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <partition_nr  CONFIG_TYPE integer >
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <partition_nr  CONFIG_TYPE integer> id
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		case sString:
		case sBooleanType:
		case sIdent:
			return SEM_ERR;
		break;	
		// there is: integer
		case  sInteger:
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <partition_nr  CONFIG_TYPE integer > id </
	if ((error = lEndPar()) != OK){
		return error;
	}


	// there is: <partition_nr  CONFIG_TYPE integer > id </partition_nr
	if ((error = check(K_PARTITION_NR)) != OK){
		return error;
	}
	// there is: <partition_nr  CONFIG_TYPE integer > id </partition_nr> 
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule -> PARTITION_ATRIBUTES -> PARTITION_FORMAT or PARTITION_MOUNT or LV_NAME or PARTITION_NR or LVM_GROUP or PARTITION_ID or STRIPES or PARTITION_FS or .
 * Return: error code or ok
 */
ERROR_CODE partition_atributes() {
	ERROR_CODE error = OK;
	// there is: < or </
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: <
		case  sLStartPar:
			error = OK;
		break;
		// there is: </
		case  sLEndPar:
			return OK;
		break;
		default:
			return SYN_ERR;
		break;
	}
	// there is: filesystem or mount or lv_name or lvm_group or stripes or format or partition_id or partition_nr
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: filsystem
		case  K_FILESYSTEM:
			occurance.partition_fs_counter++;
			if ((error = filesystem()) != OK){
				return error;
			}
		break;
		// there is: mount
		case  K_MOUNT:
			occurance.partition_mount_counter++;
			if ((error = mount()) != OK){
				return error;
			}
		break;
		// there is: lv_name
		case  K_LV_NAME:
			occurance.lv_name_counter++;
			if ((error = lv_name()) != OK){
				return error;
			}
		break;
		// there is: lvm_group
		case  K_LVM_GROUP:
			occurance.lvm_group_counter++;
			if ((error = lvm_group()) != OK){
				return error;
			}
		break;
		// there is: stripes
		case  K_STRIPES:
			occurance.stripes_counter++;
			if ((error = stripes()) != OK){
				return error;
			}
		break;
		// there is: format
		case  K_FORMAT:
			occurance.partition_format_counter++;
			if ((error = format()) != OK){
				return error;
			}
		break;
		// there is: partition_id
		case  K_PARTITION_ID:
			occurance.partition_id_counter++;
			if ((error = partition_id()) != OK){
				return error;
			}
		break;
		// there is: partition_nr
		case  K_PARTITION_NR:
			occurance.partition_nr_counter++;
			if ((error = partition_nr()) != OK){
				return error;
			}
		break;
		default:
			return SYN_ERR;
		break;
	}
	if ((error = check_multiple_occurance(occurance)) != OK){
		return error;
	}

	if ((error = partition_atributes()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  PARTITION_FORMAT -> <format CONFIG_TYPE boolean > BOOLEAN </format> .
 * Return: error code or ok
 */
ERROR_CODE format() {
	ERROR_CODE error = OK;
	// there is: <format CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}
	// there is: <format CONFIG_TYPE boolean
	token = get_Token();
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
		case  K_BOOLEAN:
			error =  OK;
		break;
		case K_INTEGER:
		case K_SYMBOL:
			return SEM_ERR;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <format CONFIG_TYPE boolean >
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <format CONFIG_TYPE boolean > BOOLEAN
	if ((error = boolean()) != OK){
		return error;
	}

	// there is: <format CONFIG_TYPE boolean > BOOLEAN </
	if ((error = lEndPar()) != OK){
		return error;
	}


	// there is: <format CONFIG_TYPE boolean > BOOLEAN </format
	if ((error = check(K_FORMAT)) != OK){
		return error;
	}
	// there is: <format CONFIG_TYPE boolean > BOOLEAN </format>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  STRIPES ->  <stripes CONFIG_TYPE ATRIBUTE_TYPE > id </stripes> .
 * Return: error code or ok
 */
ERROR_CODE stripes() {
	ERROR_CODE error = OK;
	int tmp_attr_type;
	// there is: <stripes CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}
	// there is: <stripes CONFIG_TYPE ATRIBUTE_TYPE >
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: symbol
		case  K_SYMBOL:
			tmp_attr_type = K_SYMBOL;
			error = OK;
		break;
		// there is: integer
		case  K_INTEGER:
			tmp_attr_type = K_INTEGER;
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <stripes CONFIG_TYPE ATRIBUTE_TYPE >
	if ((error = rPar()) != OK){
		return error;
	}
	// there is: <stripes CONFIG_TYPE ATRIBUTE_TYPE > id
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		// there is: symbol
		case  sString:
			// semantic control
			if (tmp_attr_type == K_INTEGER){
				return SEM_ERR;
			}
			error = OK;
		break;
		// there is: integer
		case  sInteger:
			if (tmp_attr_type == K_SYMBOL){
				return SEM_ERR;
			}
			error = OK;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <stripes CONFIG_TYPE ATRIBUTE_TYPE > id </
	if ((error = lEndPar()) != OK){
		return error;
	}


	// there is: <stripes CONFIG_TYPE ATRIBUTE_TYPE > id </stripes
	if ((error = check(K_STRIPES)) != OK){
		return error;
	}
	// there is: <stripes CONFIG_TYPE ATRIBUTE_TYPE > id </stripes>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}

/*
 * Check for left rule ->  PARTITION_FS -> <filesystem CONFIG_TYPE symbol> id </filesystem> .
 * Return: error code or ok
 */
ERROR_CODE filesystem() {
	ERROR_CODE error = OK;
	// there is: <filesystem CONFIG_TYPE
	if ((error = config_type()) != OK){
		return error;
	}

	// there is: <filesystem CONFIG_TYPE symbol
	token = get_Token();
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
		case  K_SYMBOL:
			error =  OK;
		break;
		case K_INTEGER:
		case K_BOOLEAN:
			return SEM_ERR;
		break;
		default:
			return SYN_ERR;
		break;
	}

	// there is: <filesystem CONFIG_TYPE symbol>
	if ((error = rPar()) != OK){
		return error;
	}

	// there is: <filesystem CONFIG_TYPE symbol> id
	token = get_Token();
	switch (token.id) {
		// Lexical error
		case sError: 
			return LEX_ERR;
		break;
		// End of file
		case sEndofFile: 
			return SYN_ERR;
		break;
		case sInteger:
		case sIdent:
		case sBooleanType:
			return SEM_ERR;
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

	// there is:<filesystem CONFIG_TYPE symbol> id </
	if ((error = lEndPar()) != OK){
		return error;
	}

	// there is: <filesystem CONFIG_TYPE symbol> id </filesystem
	if ((error = check(K_FILESYSTEM)) != OK){
		return error;
	}
	// there is: <filesystem CONFIG_TYPE symbol> id </filesystem>
	if ((error = rPar()) != OK){
		return error;
	}
	return OK;
}
