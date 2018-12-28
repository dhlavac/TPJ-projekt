/*
 * File: semantic.c
 * Description: Implementation of functions used for semantic analysys
*/

#include "semantic.h"

/*
 * Function set all partition atributes counters to zero
 * Input: structure of partition atribute counters
 * Return: structure with serro counters
 */
tPartAtrNumber set_zero_counter(tPartAtrNumber structure){
	structure.partition_format_counter = 0;
	structure.partition_mount_counter = 0;
	structure.lv_name_counter = 0;
	structure.lvm_group_counter = 0;
	structure.partition_id_counter = 0;
	structure.stripes_counter = 0;
	structure.partition_fs_counter = 0;
	structure.partition_nr_counter = 0;

	return structure;
}

/*
 * Function check for multiple occurance of partitions attributes
 * Input: structure of partition atribute counters
 * Return: error code or ok
 */
ERROR_CODE check_multiple_occurance(tPartAtrNumber structure){
	if (structure.partition_format_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.partition_mount_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.lv_name_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.lvm_group_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.partition_id_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.stripes_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.partition_fs_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}
	if (structure.partition_nr_counter > 1) {
		return SEM_DOUBLE_DEF_ERR;
	}

	return OK;
}

/*
 * Function check if string if is a valid path (contains slash at the begginning of string)
 * Input: pointer to string
 * Return: error code or ok
 */
ERROR_CODE check_path(char *string){
	if(string[0] == SLASH){
		return OK;
	}
	return SEM_INVALID_PATH_ERR;
}
