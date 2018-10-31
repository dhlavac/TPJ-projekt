

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 #include "error.h"
 #include "scaner.h"
#include "garbage.h"

 ERROR_CODE error;
 FILE *file;

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
	}
	
}


int main(int argc, char **argv){
	meminitialization();/* init garbage */
	if (argc == 1) {
		memallfree(); /* volanie garbage aby upratal */
		fprintf(stderr, "Subor sa netvorit.\n");
	}
	if(!(file = fopen(argv[1], "r"))){
		fprintf(stderr, "Subor sa nepodarilo otvorit.\n");
		memallfree(); /* volanie garbage aby upratal */
		return INTERN_ERR;
	}
	while (token.id != sEndofFile){
		token = get_Token();
		printf("%s\n", token.attribute);
		printf("%s\n", debug_token(token.id));
		if (token.id == sError){
			break;
		}
	}
	fclose(file);
	memallfree(); /* volanie garbage aby upratal */

}

