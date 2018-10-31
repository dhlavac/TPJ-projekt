/**
 * File:     scaner.c
 * Lexical analysys
 */

#include "scaner.h"

#define NUM_OF_KEYWORDS 22	// key words number

tToken token; //global variable represent token
int row = 0;  //global variable represent actual row 
char *TmpPoint; // temporary variable for storing chaacters during lexical analysys
FILE *file;

/* List of key words */
const char *keywords[NUM_OF_KEYWORDS] = {
 	"partitioning\0", "mount\0", "type\0", "drive\0", "device\0",
 	"initialize\0", "config\0", "partition\0", "partitions\0", "create\0",
 	"filesysem\0", "format\0", "partition_id\0", "partition_nr\0", "size\0",
 	"stripes\0","stripesize\0", "lvm_group\0", "partition_id\0", "partition>_nr\0",
 	"pesize\0", "use\0",
 
};

 /* List of key words - boolen types */
const char *boolean_types[2] = {
 	"true\0", "false\0",
};

 /* List of key words -  types */
const char *types[4] = {
 	"list\0", "symbol\0" ,"integer\0", "boolean\0",
};

/*
* Memory realocation for readen characters
* Using garbage collector to prevent memory loose
*/
static void expand_token(int c, int *i){
 	if((*i) > 0){
 		if((token.attribute = (char *) memrealloc(token.attribute, (*i) + 2))){
 			token.attribute[(*i) + 1] = '\0';
 			token.attribute[(*i)] = c;
 			(*i)++;
 		} else{
 			//memfree(token.attribute);
 			scaner_error = INTERN_ERR;
 		}
 	} else{
 			token.attribute[(*i) + 1] = '\0';
 			token.attribute[(*i)] = c;
 			(*i)++;
 	}
 } 

/*
* Return current character or if is character blank return to stream
*/
static void undo_c(int c){
	//if(!(isspace(c))){
 		ungetc(c, file);
 	//}
}

/*
* Check for keywords in string
*/
static tState check_keyword(char *s){
	int j;

 	for(j = 0; j < NUM_OF_KEYWORDS; j++){
 		if(!(strcmp(s,keywords[j]))){
 			return sKeyWord;
 		}
 	}

 	for(j = 0; j < 2; j++){
 		if(!(strcmp(s,boolean_types[j]))){
 			return sBooleanType;
 		}
 	}

 	for(j = 0; j < 2; j++){
 		if(!(strcmp(s,types[j]))){
 			return sTypes;
 		}
 	}

 	return sIdent;		//No match to key word return identificator
}

/*
* Add id to token depends of type of token
*/
static void fill_token(tState state){
 	token.id = state;
}

/*
* Inicialization string for token
*/
static void init_string(int *i){
 	//memfree(TmpPoint);
 	token.attribute = (char *) memmalloc((*i)+2);
 	if(token.attribute == NULL){
 		//memfree(token.attribute);
 		scaner_error = INTERN_ERR;
 	} else{
 		token.attribute[(*i)] = '\0';
 	}
 }

/* Token procees to determine lexem type */
tToken get_Token(){
 	tState state = sStart;
 	int i = 0;		// counter of read
 	int c;			// current character
 	int escap;		// variable for escape sequencies
 	bool cont = true;
/* token inicialization */
 	token.id = sStart;
 	init_string(&i);

/* DFA  determinitic finite automata*/
 	while((c = getc(file)) && (cont)){
 		switch(state){
 			case sStart:{
 				if(isalpha(c))								state = sIdent;
 				else if((c > '0') && (c <= '9'))			state = sInteger;
 				else if(c == '=')							state = sAssign;
 				else if(c == ':')							state = sColon;
 				else if(c == '>')							state = sRPar;
 				else if(c == '<')							state = sLStartPar;
 				else if(c == EOF)							state = sEndofFile;
 				
 				else if(c == '"'){						
 					state = sString;
 					break;
 				} 	

 				//white character
 				else if(isspace(c)){
 					state = sStart;
 					break;
 				}
 				// caracter is not lexem of language
 				else{
 					state = sError;
 					break;
 				}

 				//extend token with current character
 				expand_token(c, &i);
 				break;
 			}
 			// character with check for key words
 			case sIdent:{
 				if((isdigit(c)) || (isalpha(c)) || c == '_'){
 					state = sIdent;
 					expand_token(c, &i);
 				} else{
 					token.id = check_keyword(token.attribute);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}
 			// integer
 			case sInteger:{
 				if(isdigit(c)){
 					state = sInteger;
 					expand_token(c, &i);
 				} else{
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}
 			// string in quotes
 			case sString:{
 				if(c == '"'){
 					fill_token(state);
 					state = sEnd;
 				} else if((c == '\n') || (c == EOF)){
 					state = sError;
 				} else if(c == 92){
 					state = sEscSeq;
 				} else{
 					expand_token(c, &i);
 					state = sString;
 				}
 				break;
 			}
 			// escape sequestion in string
 			case sEscSeq:{
 				if(c == '"'){
 					state = sString;
 					expand_token(c, &i);
 				} else if(c == 'n'){
 					escap = '\n';
 					state = sString;
 					expand_token(escap, &i);
 				} else if(c == 't'){
 					escap = '\t';
 					state = sString;
 					expand_token(escap, &i);
 				} else if(c == 92){
 					state = sString;
 					expand_token(c, &i);
 				} else{
 					state = sError;
 					undo_c(c);
 				}
 				break;
 			}
 			// character '<'
 			case sLStartPar:{
 				if(c == '/'){
 					state = sLEndPar;
 					expand_token(c, &i);
 				} else{
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}
 			//character '/'
 			case sSlash:{
 				if(c == '/'){
 					state = sLComment;
 				}else{
 					fill_token(state);
 					state = sEnd;
 					undo_c(c);
 				}
 				break;
 			}
 			//line comentary
 			case sLComment:{
 				if(c == '\n'){
 					state = sStart;
 					i = 0;
 				} else if (c == EOF){
 					state = sEndofFile;
 				} else{
 					state = sLComment;
 				}
 				break;
 			}
 			case sColon:		// :
 			case sRPar:			// >
 			case sLEndPar:		// />
 			case sAssign:		// =
 			case sEndofFile:	// EOF
 			case sKeyWord:{		// Keyword
 				fill_token(state);
 				state = sEnd;
 				undo_c(c);
 				break;
 			}
 			//Lexical error
 			case sError:{
 				scaner_error = LEX_ERR;
 				fill_token(state);
 				cont = false;
 				break;
 			}
 			//End state DFA
 			case sEnd:{
				undo_c(c);
				undo_c(c);
 				cont = false;
 				break;
 			}
 		}

 		//Lexical error
 		if(scaner_error){
 			break;
 		}

 		//Increment new line counter
 		if(c == '\n'){
 			row++;
 		}
 	}

 	TmpPoint = token.attribute;
 /* Return token to parser */
 	return token;
}