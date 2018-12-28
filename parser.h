
#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"
#include "error.h"
#include "scaner.h"

/* Struct for token */
typedef struct{
	int current_line;
	ERROR_CODE error_code;
} tError;

tError syntax_analysys();
ERROR_CODE partitioning();
ERROR_CODE check();
ERROR_CODE rPar();
ERROR_CODE lPar();
ERROR_CODE KPartitioning();
ERROR_CODE drive_list();
ERROR_CODE device();
ERROR_CODE pesize();
ERROR_CODE ps_type();
ERROR_CODE use();
ERROR_CODE use_type();
ERROR_CODE initialize();
ERROR_CODE config_type();
ERROR_CODE boolean();
ERROR_CODE partition_type();
ERROR_CODE partitions_type();
ERROR_CODE partition_config();
ERROR_CODE partition();
ERROR_CODE partition_list();
ERROR_CODE partition_create_config();
ERROR_CODE partition_id();
ERROR_CODE partition_nr();
ERROR_CODE partition_size();
ERROR_CODE partition_atributes();
ERROR_CODE mount();
ERROR_CODE lvm_group();
ERROR_CODE lv_name();
ERROR_CODE filesystem();
ERROR_CODE stripes();
ERROR_CODE format();

#endif
