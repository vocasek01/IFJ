#include "scanner.h"
#include "expr.h"
/*
 *****************************************************
 * 
 *              Global var                 
 * 
*/

FILE *sourceFile;
Token token;
char name[MAX_LENGTH];

/*
 *****************************************************
 * 
 *              functions                  
 * 
*/

bool scannerInit(char* file)
{
    sourceFile = fopen(file, "r");
    if (!sourceFile)
    {
        perror("File opening failed");
        return false;
    }
    return true;
}
void errorCode()
{
    fprintf(stderr, "%s", "Lexical err\n");
}

Token getToken()
{
    char c;
    State state = STATE_START;
    name[0] = 0;
    while (true)
    {
        c = (char)fgetc(sourceFile);
        /*
 *****************************************************
 * 
 *              Stav 'EOF'                  
 * 
*/
        if (c == EOF)
        {
            token.tokenName = "EOF";
            token.type = ENDOFFILE;
            return token;
        }
        /*
 *****************************************************
 * 
 *              Stav  '/'                  
 * 
*/

        if (state == STATE_START && c == '/')
        {
            strncat(name, &c, 1);
            state = STATE_DIV;
            continue;
        }
        if (state == STATE_DIV)
        {
            if (c == '/')
            {
                strncat(name, &c, 1);
                state = STATE_COMMENT;
                continue;
            }
            else
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = DIV;
                return token;
            }
        }
        if (state == STATE_COMMENT)
        {
            if (c != '\n')
            {
                strncat(name, &c, 1);
                continue;
            }
            else
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = COMMENT;
                fseek(sourceFile, -1, SEEK_CUR);
                return token;
            }
        }

        /*
 *****************************************************
 * 
 *              Stav  '='                  
 * 
 */

        if (state == STATE_START && c == '=')
        {
            strncat(name, &c, 1);
            state = STATE_EQL;
            continue;
        }
        if (state == STATE_EQL)
        {
            if (c == '=')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = EQL;
                return token;
            }
            else
            {
                token.tokenName = name;
                token.type = ASSIGN;
                fseek(sourceFile, -1, SEEK_CUR);
                return token;
            }
        }
        /*
 *****************************************************
 * 
 *              Základní stavy                  
 * 
*/
        if (state == STATE_START)
        {
            if (isspace(c) && c != '\n')
            {
                continue;
            }
            if (c == '\"')
            {
                strncat(name, &c, 1);
                state = STATE_QUOTATION_MARK;
                continue;
            }
            if (isalpha(c) || c == '_')
            {
                strncat(name, &c, 1);
                state = STATE_ID;
                continue;
            }
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                state = STATE_NUMBER;
                continue;
            }
            if (c == ',')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = COMA;
                return token;
            }
            if (c == ';')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = SEMICOLON;
                return token;
            }
            if (c == '.')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = DOT;
                return token;
            }
            if (c == '\n')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = EOL;
                return token;
            }
            if (c == '{')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = LCBR;
                return token;
            }
            if (c == '}')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = RCBR;
                return token;
            }
            if (c == '(')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = LBR;
                return token;
            }
            if (c == ')')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = RBR;
                return token;
            }
        }

        /*
 *****************************************************
 * 
 *                  STAV ID                 
 * 
*/

        if (state == STATE_ID)
        {
            if (c == '_' || isalpha(c) || isdigit(c))
            {
                strncat(name, &c, 1);
                if (strcmp(name, "if") == 0)
                {
                    token.tokenName = name;
                    token.type = IF;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "else") == 0)
                {
                    token.tokenName = name;
                    token.type = ELSE;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "for") == 0)
                {
                    token.tokenName = name;
                    token.type = FOR;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "return") == 0)
                {
                    token.tokenName = name;
                    token.type = RETURN;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "package") == 0)
                {
                    token.tokenName = name;
                    token.type = PACKAGE;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "func") == 0)
                {
                    token.tokenName = name;
                    token.type = FUNC;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "string") == 0)
                {
                    token.tokenName = name;
                    token.type = STRING_TYPE;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "int") == 0)
                {
                    token.tokenName = name;
                    token.type = INT_TYPE;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "bool") == 0)
                {
                    token.tokenName = name;
                    token.type = BOOLEAN_TYPE;
                    token.attribute.string = name;
                    return token;
                }
                if (strcmp(name, "float64") == 0)
                {
                    token.tokenName = name;
                    token.type = FLOAT64_TYPE;
                    token.attribute.string = name;
                    return token;
                }
                continue;
            }
            else
            {
                token.tokenName = name;
                token.type = ID;
                token.attribute.string = name;
                fseek(sourceFile, -1, SEEK_CUR);
                return token;
            }
        }

        /*        
 ******************************************************
 * 
 *                  STAV " "                 
 * 
*/
        if (state == STATE_QUOTATION_MARK)
        {
            if (c == '\"')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = STRING;
                return token;
            }
            if (c == '\\')
            {
                strncat(name, &c, 1);
                state = STATE_STR2;
                continue;
            }
            else
            {
                strncat(name, &c, 1);
                continue;
            }
        }
        if (state == STATE_STR2)
        {
            if (c == 'x')
            {
                strncat(name, &c, 1);
                state = STATE_STR3;
                continue;
            }
            if (c == '\"' || c == 'n' || c == 't' || c == 's' || c == '\\')
            {
                strncat(name, &c, 1);
                state = STATE_QUOTATION_MARK;
                continue;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_STR3)
        {
            if (isdigit(c) || isalpha(c))
            {
                strncat(name, &c, 1);
                state = STATE_QUOTATION_MARK;
                continue;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }

        /*
 ******************************************************
 * 
 *                  STAV NUMBER                 
 * 
*/

        if (state == STATE_NUMBER)
        {
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                continue;
            }
            if (c == 'E' || c == 'e')
            {
                strncat(name, &c, 1);
                state = STATE_EXP;
                continue;
            }
            if (c == '.')
            {
                strncat(name, &c, 1);
                state = STATE_DOUBLE;
                continue;
            }
            else
            {
                token.tokenName = name;
                token.type = INT;
                token.attribute.integer_num = atoi(name);
                return token;
            }
        }
        if (state == STATE_EXP)
        {
            if (c == '+' || c == '-')
            {
                strncat(name, &c, 1);
                state = STATE_EXP2;
                continue;
            }
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                state = STATE_EXP3;
                continue;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_EXP2)
        {
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                state = STATE_EXP3;
                continue;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_EXP3)
        {
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                continue;
            }
            else
            {
                token.tokenName = name;
                token.type = DOUBLE;
                token.attribute.float_num = atof(name);
                return token;
            }
        }
        if (state == STATE_DOUBLE)
        {
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                state = STATE_DOUBLE2;
                continue;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_DOUBLE2)
        {
            if (isdigit(c))
            {
                strncat(name, &c, 1);
                continue;
            }
            if (c == 'E' || c == 'e')
            {
                strncat(name, &c, 1);
                state = STATE_EXP;
                continue;
            }
            else
            {
                token.tokenName = name;
                token.type = DOUBLE;
                token.attribute.float_num = atof(name);
                fseek(sourceFile, -1, SEEK_CUR);
                return token;
            }
        }
        /*
 ******************************************************
 * 
 *                  STAV EXPRESSION                 
 * 
 * '!' '>' '<' '&'  '|'   '+'  '-'  '*'  '/' ':'
*/

        if (state == STATE_START && c == '!')
        {
            strncat(name, &c, 1);
            state = STATE_NOTEQ;
            continue;
        }
        if (state == STATE_START && c == '>')
        {
            strncat(name, &c, 1);
            state = STATE_GTE;
            continue;

            /* strncat(name, &c, 1);
            token.tokenName = name;
            token.type = GT;
            return token;*/
        }
        if (state == STATE_START && c == '<')
        {
            strncat(name, &c, 1);
            state = STATE_LTE;
            continue;
            /*
            strncat(name, &c, 1);
            token.tokenName = name;
            token.type = LT;
            return token;
            */
        }

        if (state == STATE_START && c == ':')
        {
            strncat(name, &c, 1);
            state = STATE_DOUBLEDOT;
            continue;
        }
        if (state == STATE_START && c == '&')
        {
            strncat(name, &c, 1);
            state = STATE_AND;
            continue;
        }
        if (state == STATE_START && c == '|')
        {
            strncat(name, &c, 1);
            state = STATE_OR;
            continue;
        }
        if (state == STATE_START && c == '+')
        {
            strncat(name, &c, 1);
            token.tokenName = name;
            token.type = ADD;
            return token;
        }
        if (state == STATE_START && c == '-')
        {
            strncat(name, &c, 1);
            token.tokenName = name;
            token.type = SUB;
            return token;
        }
        if (state == STATE_START && c == '*')
        {

            strncat(name, &c, 1);
            token.tokenName = name;
            token.type = MUL;
            return token;
        }
        if (state == STATE_NOTEQ)
        {
            if (c == '=')
            {
                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = NOTEQ;
                return token;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_DOUBLEDOT)
        {
            if (c == '=')
            {

                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = DEF;
                return token;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_GTE)
        {
            if (c == '=')
            {

                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = GTE;
                return token;
            }
            else
            {
                token.tokenName = name;
                token.type = GT;
                fseek(sourceFile, -1, SEEK_CUR);
                return token;
            }
        }
        if (state == STATE_LTE)
        {
            if (c == '=')
            {

                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = LTE;
                return token;
            }
            else
            {
                token.tokenName = name;
                token.type = LT;
                fseek(sourceFile, -1, SEEK_CUR);
                return token;
            }
        }
        if (state == STATE_AND)
        {
            if (c == '&')
            {

                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = EXPR;
                return token;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        if (state == STATE_OR)
        {
            if (c == '|')
            {

                strncat(name, &c, 1);
                token.tokenName = name;
                token.type = EXPR;
                return token;
            }
            else
            {
                state = STATE_START;
                errorCode();
                token.type = ERROR;
                continue;
            }
        }
        else
        {
            errorCode();
            token.type = ERROR;
            return token;
        }
    }
}

int main(int argc, char *argv[])
{
    scannerInit(argv[1]);
    Token a = getToken();
    parseExpr(a);

    return 0;
}