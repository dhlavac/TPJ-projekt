/*
 * File: semantic.h 
 * Description: Header file for semantic analyse
 */

#ifndef SEMANTIC_INCLUDED
#define SEMANTIC_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "scaner.h"
#include "parser.h"

/* Struct for token */
typedef struct{
	int partition_format_counter;
	int partition_mount_counter;
	int lv_name_counter;
	int lvm_group_counter;
	int partition_id_counter;
	int stripes_counter;
	int partition_fs_counter;
	int partition_nr_counter;
} tPartAtrNumber;

#define LIST 69
#define SINGLE 70
#define SLASH 47

tPartAtrNumber set_zero_counter();
ERROR_CODE check_multiple_occurance();
ERROR_CODE check_path();

#endif
