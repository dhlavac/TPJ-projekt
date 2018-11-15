/*
 * File: parser.c 
 * Description: Implementation of recursive syntax analysys
 */

ERROR_CODE prog() {
    ERROR_CODE error = OK_ERR;
	token = get_Token();
    debug(token);
    switch (token.id) {
        // Lexical error
        case sError: 
            error = LEX_ERR;
            return error;
        break;
        // koniec suboru
        case sEndofFile: 
            error = SYN_ERR;
        break;
        // if there
        default :
            error = body();
        break;
    }

    if (error != OK_ERR) {
        return error;
    }
    // osetrenie ci main bol deklarovany a ci su vsetky funkcie definovane
   // error =  finalFunctionCheckout(&symbol_table);

    // if (error != OK_ERR) {
    //     return error;
    // }

    return OK_ERR;
}

void debug(token) {
    printf("%s\n", token.attribute);
    printf("%s\n", debug_token(token.id));    
}