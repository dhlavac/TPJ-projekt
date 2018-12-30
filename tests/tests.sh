#!/usr/bin/env bash
# Author: Dominik Hlavac Duran
# Automatic testing script for TPJ project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Name of executable file
TASK="./projekt"

# cesty ke vstupním a výstupním souborům
LOCAL_IN_PATH="./inputs/" # (simple relative path)
LOCAL_IN_PATH2="./inputs/" #Alternative 1 (primitive relative path)
LOCAL_IN_PATH3=`pwd`"/inputs/" #Alternative 2 (absolute path)
LOCAL_OUT_PATH="./outputs/" # (simple relative path)
LOCAL_OUT_PATH2="./outputs/" #Alternative 1 (primitive relative path)
LOCAL_OUT_PATH3=`pwd`"/outputs/" #Alternative 2 (absolute path)
# cesta pro ukládání chybového výstupu studentského skriptu
LOG_PATH="./outputs/"
ERR_COUNT=0
COUNT=0


# test01: Argument error; Expected output: test01.out; Expected return code: 7
$TASK 2> ${LOG_PATH}test01.err 1> ${LOG_PATH}test01.out
echo -n $? > ${LOG_PATH}test01.!!!

# test02: Input error - nonexisting file; Expected output: test02.out; Expected return code: 8
$TASK nonexisting_file 2> ${LOG_PATH}test02.err 2> ${LOG_PATH}test02.err 1> ${LOG_PATH}test02.out
echo -n $? > ${LOG_PATH}test02.!!!

# test03: Argument error - too much arguments; Expected output: test03.out; Expected return code: 9
$TASK first second third 2> ${LOG_PATH}test03.err 2> ${LOG_PATH}test03.err 1> ${LOG_PATH}test03.out
echo -n $? > ${LOG_PATH}test03.!!!

# test04: Valid file -  Expected output: test04.out; Expected return code: 0 
$TASK inputs/disk_field_config 2> ${LOG_PATH}test04.err 2> ${LOG_PATH}test04.err 1> ${LOG_PATH}test04.out
echo -n $? > ${LOG_PATH}test04.!!!

# test05: Valid file - Expected output: test05.out; Expected return code: 0
$TASK inputs/big_lvm_setup 2> ${LOG_PATH}test05.err 2> ${LOG_PATH}test05.err 1> ${LOG_PATH}test05.out
echo -n $? > ${LOG_PATH}test05.!!!

# test06: Semantic error - Expected output: test06.out; Expected return code: 6
$TASK inputs/invalid_path_err_in 2> ${LOG_PATH}test06.err 2> ${LOG_PATH}test06.err 1> ${LOG_PATH}test06.out
echo -n $? > ${LOG_PATH}test06.!!!

# test07: Lexical error - Expected output: test07.out; Expected return code: 1
$TASK inputs/lexical_err_line2 2> ${LOG_PATH}test07.err 2> ${LOG_PATH}test07.err 1> ${LOG_PATH}test07.out
echo -n $? > ${LOG_PATH}test07.!!!

# test08: Syntax error - Expected output: test08.out; Expected return code: 2
$TASK inputs/missing_config_in_partition 2> ${LOG_PATH}test08.err 2> ${LOG_PATH}test08.err 1> ${LOG_PATH}test08.out
echo -n $? > ${LOG_PATH}test08.!!!

# test09: Syntax error - Expected output: test09.out; Expected return code: 2
$TASK inputs/missing_partition_in_drive 2> ${LOG_PATH}test09.err 2> ${LOG_PATH}test09.err 1> ${LOG_PATH}test09.out
echo -n $? > ${LOG_PATH}test09.!!!

# test10: Syntax error - Expected output: test10.out; Expected return code: 2
$TASK inputs/missing_size_in_partition 2> ${LOG_PATH}test10.err 2> ${LOG_PATH}test10.err 1> ${LOG_PATH}test10.out
echo -n $? > ${LOG_PATH}test10.!!!

# test11: Valid file - Expected output: test11.out; Expected return code: 0
$TASK inputs/multiple_drives_multiple_partitions 2> ${LOG_PATH}test11.err 2> ${LOG_PATH}test11.err 1> ${LOG_PATH}test11.out
echo -n $? > ${LOG_PATH}test11.!!!

# test12: Valid file - Expected output: test12.out; Expected return code: 0
$TASK inputs/only_partitioning 2> ${LOG_PATH}test12.err 2> ${LOG_PATH}test12.err 1> ${LOG_PATH}test12.out
echo -n $? > ${LOG_PATH}test12.!!!

# test13: Semantic error -double definition - Expected output: test13.out; Expected return code: 4
$TASK inputs/sem_err_double_def_in_partition 2> ${LOG_PATH}test13.err 2> ${LOG_PATH}test13.err 1> ${LOG_PATH}test13.out
echo -n $? > ${LOG_PATH}test13.!!!

# test14: Semantic error- list/single wrong definied - Expected output: test14.out; Expected return code: 5
$TASK inputs/sem_err_list_definition 2> ${LOG_PATH}test14.err 2> ${LOG_PATH}test14.err 1> ${LOG_PATH}test14.out
echo -n $? > ${LOG_PATH}test14.!!!

# test15: Valid file - Expected output: test15.out; Expected return code: 0
$TASK inputs/single_drive_multiple_partitions 2> ${LOG_PATH}test15.err 2> ${LOG_PATH}test15.err 1> ${LOG_PATH}test15.out
echo -n $? > ${LOG_PATH}test15.!!!

# test16: Valid file - Expected output: test16.out; Expected return code: 0
$TASK inputs/single_drive_single_partitions 2> ${LOG_PATH}test16.err 2> ${LOG_PATH}test16.err 1> ${LOG_PATH}test16.out
echo -n $? > ${LOG_PATH}test16.!!!

# test17: Syntax error - Expected output: test17.out; Expected return code: 2
$TASK inputs/syntax_err_line2 2> ${LOG_PATH}test17.err 2> ${LOG_PATH}test17.err 1> ${LOG_PATH}test17.out
echo -n $? > ${LOG_PATH}test17.!!!

# test18: Syntax error - Expected output: test18.out; Expected return code: 2
$TASK inputs/syntax_err_line5 2> ${LOG_PATH}test18.err 2> ${LOG_PATH}test18.err 1> ${LOG_PATH}test18.out
echo -n $? > ${LOG_PATH}test18.!!!

# test19: Semantic error - Expected output: test19.out; Expected return code: 3
$TASK inputs/wrong_type_in_config 2> ${LOG_PATH}test19.err 2> ${LOG_PATH}test19.err 1> ${LOG_PATH}test19.out
echo -n $? > ${LOG_PATH}test19.!!!

# test20: Semantic error  - Expected output: test20.out; Expected return code: 3
$TASK inputs/wrong_type_definied_and_used 2> ${LOG_PATH}test20.err 2> ${LOG_PATH}test20.err 1> ${LOG_PATH}test20.out
echo -n $? > ${LOG_PATH}test20.!!!

for i in `ls ./outputs/ `
	do
		((COUNT++))
		echo "Test: $i"
		printf "\n"
		diff -a ./outputs/"$i" ./ref-out/"$i"
		if [ $? -ne 0 ]; then
			tput setaf 1;
			printf "TEST $i FAILED"
			tput setaf 7;
			((ERR_COUNT++))
		fi	
		if [ $? -eq 0 ]; then
			tput setaf 2;
			printf "TEST $i OK"
			tput setaf 7;
		fi	
		printf "\n**********************************************************************\n"
	done
echo "Failed : $ERR_COUNT tests from $COUNT ";
