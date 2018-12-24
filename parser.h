
#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "scaner.h"

ERROR_CODE partitioning();
void debug();
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
#endif