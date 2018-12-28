/*
 * File: main.c 
 * Description: Main file of program for porcessing configuration files for Foreman disc configuration tool
 */

#include "error.h"
#include "scaner.h"
#include "garbage.h"
#include "parser.h"
#include "semantic.h"

tError error;
FILE *file;

int main(int argc, char **argv){

	meminitialization();/* init garbage collector*/

	if (argc == 1) {
		memallfree(); /* call garbage collector to clean up */
		fprintf(stderr, "No file entered for anlysys.\n");
	}
	if(!(file = fopen(argv[1], "r"))){
		fprintf(stderr, "Cannot open file for reading.\n");
		memallfree(); // free memorry using garbage collector
		return INTERN_ERR;
	}
	// start file analysys
	error = syntax_analysys();

	if (error.error_code == 0) {
		printf("Everything ok, no errors occures.\n");
	}else{
		switch (error.error_code) {
			// Syntax error
			case SYN_ERR: 
				fprintf(stderr, "Syntax error occures on line %d\n",error.current_line);
			break;
			// Lexical error
			case LEX_ERR: 
				fprintf(stderr, "Lexical error - unknown character occures on line %d\n",error.current_line);
			break;
			// Semantic error
			case  SEM_ERR:
				fprintf(stderr, "Semantic error occures on line %d\n",error.current_line);
			break;
			// Semantic error with multiple definition
			case  SEM_DOUBLE_DEF_ERR:
				fprintf(stderr, "Semantic error occures on line %d  - multiple times atribute definition\n",error.current_line);
			break;
			// Semantic error with multiple definition
			case  SEM_INVALID_PATH_ERR:
				fprintf(stderr, "Semantic error occures on line %d  - string must be path e.g. /example/other\n",error.current_line);
			break;
			// Semantic error - wrong definition of list/single partitions
			case  SEM_SINGLE_LIST_ERR:
				fprintf(stderr, "Semantic error occures in partitions block ended on line %d - declared \
					 single in partitions config but used list - too much partitions in block.\n", error.current_line);
			break;
			// Internal error
			case  INTERN_ERR:
				fprintf(stderr, "Internal error\n");
			break;
		}
	}
	fclose(file);
	memallfree(); /* volanie garbage aby upratal */

	return error.error_code;
}
