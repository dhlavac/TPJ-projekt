/**
 * File:     scaner.c
 * Lexical analysys
 */

#include "scaner.h"

tToken token; //global variable represent token
int row = 1;  //global variable represent actual row 
int row_tmp = 1; // temporary variable for counting rows
char *TmpPoint; // temporary variable for storing chaacters during lexical analysys
FILE *file;

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
 	ungetc(c, file);
}

/*
* Check for keywords in string
*/
static tState check_keyword(char *s){
	int j;

 	if((strcmp(s, "partitioning")) == 0)
 		return K_PARTITIONING;
 	if((strcmp(s, "mount"))== 0)
 		return K_MOUNT;
 	if((strcmp(s, "type")) == 0)
 		return K_TYPE;
 	if((strcmp(s, "drive")) == 0)
 		return K_DRIVE;
 	if((strcmp(s, "device")) == 0)
 		return K_DEVICE;
 	if((strcmp(s, "initialize")) == 0)
 		return K_INITIALIZE;
 	if((strcmp(s, "config")) == 0)
 		return K_CONFIG;
 	if((strcmp(s, "partition")) == 0)
 		return K_PARTITION;
 	if((strcmp(s, "partitions")) == 0)
 		return K_PARTITIONS;
 	if((strcmp(s, "create")) == 0)
 		return K_CREATE;
 	if((strcmp(s, "filesystem")) == 0)
 		return K_FILESYSTEM;
 	if((strcmp(s, "format")) == 0)
 		return K_FORMAT;
 	if((strcmp(s, "partition_id")) == 0)
 		return K_PARTITION_ID;
 	if((strcmp(s, "partition_nr")) == 0)
 		return K_PARTITION_NR;
 	if((strcmp(s, "size")) == 0)
 		return K_SIZE;
 	if((strcmp(s, "stripes")) == 0)
 		return K_STRIPES ;
 	if((strcmp(s, "stripsize")) == 0)
 		return K_STRIPESSIZE;
 	if((strcmp(s, "lvm_group")) == 0)
 		return K_LVM_GROUP;
 	if((strcmp(s, "pesize")) == 0)
 		return K_PESIZE;
 	if((strcmp(s, "use")) == 0)
 		return K_USE;
 	if((strcmp(s, "all")) == 0)
 		return K_ALL;
 	if((strcmp(s, "no")) == 0)
 		return K_NO;
 	if((strcmp(s, "symbol")) == 0)
 		return K_SYMBOL;
 	if((strcmp(s, "boolean")) == 0)
 		return K_BOOLEAN;
 	if((strcmp(s, "single")) == 0)
 		return K_SINGLE;
 	if((strcmp(s, "list")) == 0)
 		return K_LIST;
 	if((strcmp(s, "integer")) == 0)
 		return K_INTEGER;

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
	if (row == row_tmp){
		token.code_line = row;
	}
	else {
		row_tmp++;
		row = row_tmp;
		token.code_line = row;
	}
 	
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