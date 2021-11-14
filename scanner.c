#include "scanner.h"

/*
 *****************************************************
 * 
 *              Global var                 
 * 
*/

FILE *sourceFile;
char str[MAX_LENGTH];

/*
 *****************************************************
 * 
 *              functions                  
 * 
*/

bool scannerInit(char* file) {
    sourceFile = fopen(file, "r");
    if (!sourceFile)
    {
        perror("File opening failed");
        return false;
    }
    return true;
}

void errorCode() {
    fprintf(stderr, "%s", "Lexical err\n");
}

void getString (char *str, Token *token) {
    token->attribute = (char *) malloc(sizeof(char) * strlen(str));

    if (!token->attribute)
        fprintf(stderr, "%s", "Internal err\n");
    else
        memcpy(token->attribute, str, sizeof(char) * strlen(str));
}

bool isKeyWord (char *str) {
    char *keyword[] = {"do", "else", "end", "function", "global", "if", "integer", "local", 
                        "nil", "number", "require", "return", "string", "then", "while"};
    int lenKeyWord = 15;

    for (int i = 0; i < lenKeyWord; i++) {
        if (strcmp(keyword[i], str) == 0) 
            return true;
    }

    return false;    
}

Token getToken () {
    char c;
    State state = START;
    Token token;
    str[0] = 0;

    while (true) {

        c = (char) fgetc(sourceFile);

/*
 *****************************************************
 * 
 *              Stav 'EOF' || 'EOL' || SPACE || TAB              
 * 
*/

        if (state == START && c == EOF) {
            token.type = ENDOFFILE;
            getString("EOF", &token);
            return token;
        }

        if (state == START && c == '\n') {
                strncat(str, &c, 1);
                token.type = EOL;
                getString(str, &token);
                return token;
            }

        if (state == START && c == ' ') {
            state = START;
            continue;
        } 

        if (state == START && c == '\t') {
            state = START;
            continue;
        } 

/*
 *****************************************************
 * 
 *              Stav  '-'                  
 * 
*/

        if (state == START && c == '-') {
            strncat(str, &c, 1);
            state = SUB;
            continue;
        }

        if (state == SUB) {
            if (c == '-') {
                strncat(str, &c, 1);
                state = BLOCKORLINE;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = SUB;
                getString(str, &token);
                return token;
            }
        }

        if (state == BLOCKORLINE) {
            if (c == '[') {
                strncat(str, &c, 1);
                state = BLOCKCOMMENT;
                continue;
            } else {
                ungetc(c, sourceFile);
                state = COMMENT;
                continue;
            }
        }

        if (state == COMMENT) {
            if (c != '\n' && c != EOF) {
                strncat(str, &c, 1);
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = COMMENT;
                getString(str, &token);
                return token;
            }
        }

        if (state == BLOCKCOMMENT) {
            if (c != ']') {
                strncat(str, &c, 1);
                continue;
            } else {
                strncat(str, &c, 1);
                token.type = BLOCKCOMMENT;
                getString(str, &token);
                return token;
            }
        }

/*
 ******************************************************
 * 
 *                  STAV OPERATORS                 
 * 
 *
*/

        if (state == START && c == '#') {
            strncat(str, &c, 1);
            token.type = LEN;
            getString(str, &token);
            return token;
        }
        
        if (state == START && c == '*') {
            strncat(str, &c, 1);
            token.type = MUL;
            getString(str, &token);
            return token;
        }
        
        if (state == START && c == '+') {
            strncat(str, &c, 1);
            token.type = ADD;
            getString(str, &token);
            return token;
        }

        if (state == START && c == '/') {
            strncat(str, &c, 1);
            state = DIV;
            continue;
        }

        if (state == DIV) {
            if (c == '/') {
                strncat(str, &c, 1);
                token.type = INT_DIV;
                getString(str, &token);
                return token;
            } else {
                ungetc(c, sourceFile);
                token.type = DIV;
                getString(str, &token);
                return token;
            }
        }

        if (state == START && c == '.') {
            c = getc(sourceFile);
            if (c == '.') {
                token.type = KONC;
                getString("..", &token);
            } else {
                errorCode();
                token.type = ERROR;
            }
            return token;
        }

        if (state == START && c == '<') {
            c = getc(sourceFile);
            if (c == '=') {
                token.type = LTE;
                getString("<=", &token);
                return token;
            } else {
                ungetc(c, sourceFile);
                token.type = LT;
                getString("<", &token);
                return token;
            }
        }

        if (state == START && c == '>') {
            c = getc(sourceFile);
            if (c == '=') {
                token.type = GTE;
                getString(">=", &token);
                return token;
            } else {
                ungetc(c, sourceFile);
                token.type = GT;
                getString(">", &token);
                return token;
            }
        }

        if (state == START && c == '=') {
            c = getc(sourceFile);
            if (c == '=') {
                token.type = EQ;
                getString("==", &token);
                return token;
            } else {
                ungetc(c, sourceFile);
                token.type = ASSIGN;
                getString("=", &token);
                return token;
            }
        }

        if (state == START && c == '~') {
            c = getc(sourceFile);
            if (c == '=') {
                token.type = NOTEQ;
                getString("~=", &token);
            } else {
                errorCode();
                token.type = ERROR;
            }
            return token;
        }

        if (state == START && c == '(') {
            strncat(str, &c, 1);
            token.type = LBR;
            getString(str, &token);
            return token;
        }

        if (state == START && c == ')') {
            strncat(str, &c, 1);
            token.type = RBR;
            getString(str, &token);
            return token;
        }

/*
 *****************************************************
 * 
 *              STAV NUMBER || INTEGER         
 * 
*/
        if (state == START && isdigit(c)) {
            strncat(str, &c, 1);
            state = INT;
            continue;
        }

        if (state == INT) {
            if (isdigit(c)) {
                strncat(str, &c, 1);
                state = INT;
                continue;
            } else if (c == '.') {
                strncat(str, &c, 1);
                state = DOUB_DOT1;
                token.type = DOUBLE;
                continue;
            } else if (c == 'e' || c == 'E') {
                strncat(str, &c, 1);
                state = DOUB_EXP1;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = INT;
                getString(str, &token);
                return token;
            }
        }

        if (state == DOUB_DOT1) {
            if (isdigit(c)) {
                strncat(str, &c, 1);
                state = DOUB_DOT2;
            } else {
                ungetc(c, sourceFile);
                errorCode();
                token.type = ERROR;
            }

            getString(str, &token);
            return token;
        }

        if (state == DOUB_DOT2) {
            if (isdigit(c)) {
                strncat(str, &c, 1);
                state = DOUB_DOT2;
                continue;
            } else if (c == 'e' || c == 'E') {
                strncat(str, &c, 1);
                state = DOUB_EXP1;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = DOUBLE;
                getString(str, &token);
                return token;
            }
        }

        if (state == DOUB_EXP1) {
            if (isdigit(c)){
                strncat(str, &c, 1);
                state = DOUBLE;
                continue;
            } else if (c == '+' || c == '-') {
                strncat(str, &c, 1);
                state = DOUB_EXP2;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = ERROR;
                getString(str, &token);
                return token;
            }
        }

        if (state == DOUB_EXP2) {
            if (isdigit(c)){
                strncat(str, &c, 1);
                state = DOUBLE;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = ERROR;
                getString(str, &token);
                return token;
            }
        }

        if (state == DOUBLE) {
            if (isdigit(c)){
                strncat(str, &c, 1);
                state = DOUBLE;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = DOUBLE;
                getString(str, &token);
                return token;
            }
        }

/*        
 ******************************************************
 * 
 *                  STAV STRING                
 * 
*/
        if (state == START && c == '"') {
            strncat(str, &c, 1);
            state = STR;
            token.type = STR;
            continue;
        }

        if (state == STR) {
            if (c == '"') {
                strncat(str, &c, 1);
                getString(str, &token);
                return token;
            } else if (c == '\\') {
                state = ESCAPE1;
                continue;
            } else if (c > 31) {
                strncat(str, &c, 1);
                state = STR;
                continue;
            } else {
                strncat(str, &c, 1);
                token.type = ERROR;
                continue;
            }
        }

        if (state == ESCAPE1) {
            if (c == 'n') {
                strncat("\n", &c, 1);
                state = STR;
                continue;
            } else if (c == 't') {
                strncat("\t", &c, 1);
                state = STR;
                continue;
            } else if (c == '\\') {
                strncat("\\", &c, 1);
                state = STR;
                continue;
            } else if (c == '"') {
                strncat("\"", &c, 1);
                state = STR;
                continue;
            } else if (isdigit(c)) {
                ungetc(c, sourceFile);
                state = ESCAPE2;
                continue;
            } else {
                token.type = ERROR;
                state = STR;
                continue;
            }

        }

        if (state == ESCAPE2) {
            int esc = 100 * (c - 48);
            strncat(str, &c, 1);

            c = (char) fgetc(sourceFile);
            if (isdigit(c)) {
                esc += 10 * (c - 48);
                strncat(str, &c, 1);
            }

            c = (char) fgetc(sourceFile);
            if (isdigit(c)) {
                esc += (c - 48);
                strncat(str, &c, 1);
            }
            
            if (!(esc > 0 && esc < 256)) {
                token.type = ERROR;
            }

            state = STR;
        }

/*
 *****************************************************
 * 
 *                  STAV INDETIFICATOR || KEYWORD                 
 * 
*/

        if (state == START) {
            if (c == '_') {
                strncat(str, &c, 1);
                state = IDENTIFICATOR;
                continue;
            } else if (isalpha(c)) {
                strncat(str, &c, 1);
                state = IDENORKEY;
                continue;
            }
        }

        if (state == IDENORKEY) {
            if (isalpha(c)) {
                strncat(str, &c, 1);
                state = IDENORKEY;
                continue;
            } else if (isdigit(c) || c == '_'){
                strncat(str, &c, 1);
                state = IDENTIFICATOR;
                continue;
            } else {
                ungetc(c, sourceFile);
                if (isKeyWord(str)) {
                    token.type = KEYWORD;
                } else {
                    token.type = IDENTIFICATOR;
                }
                getString(str, &token);
                return token;
            }
        }

        if (state == IDENTIFICATOR) {
            if (isalpha(c) || isdigit(c) || c == '_') {
                strncat(str, &c, 1);
                state = IDENTIFICATOR;
                continue;
            } else {
                ungetc(c, sourceFile);
                token.type = IDENTIFICATOR;
                getString(str, &token);
                return token;
            }
        }

    }
} 
