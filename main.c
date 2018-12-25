

#include "error.h"
#include "scaner.h"
#include "garbage.h"
#include "parser.h"

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
