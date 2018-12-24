

#include "error.h"
#include "scaner.h"
#include "garbage.h"
#include "parser.h"

ERROR_CODE error;
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
	error = partitioning();

	if (error == 0) {
		printf("SICKO v PORADKU\n");
	}
	fclose(file);
	memallfree(); /* volanie garbage aby upratal */

	return error;
}
