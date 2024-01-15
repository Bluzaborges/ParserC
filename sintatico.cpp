#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

//Tokens
#define TKId 1
#define TKConstant 2
#define TKInt 3
#define TKFloat 4
#define TKChar 5
#define TKDouble 6
#define TKLong 7
#define TKUnsigned 8
#define TKStruct 9
#define TKWhile 10
#define TKFor 11
#define TKDo 12
#define TKIf 13
#define TKElse 14
#define TKSwitch 15
#define TKCase 16
#define TKDefault 17
#define TKSizeof 18
#define TKContinue 19
#define TKBreak 20
#define TKReturn 21
#define TKAssignment 22
#define TKEqual 23
#define TKSum 24
#define TKSumAssignment 25
#define TKIncrement 26
#define TKSubtraction 27
#define TKSubtractionAssignment 28
#define TKDecrement 29
#define TKStar 30
#define TKMultiplicationAssignment 31
#define TKDivision 32
#define TKDivisionAssignment 33
#define TKModulus 34
#define TKModulusAssignment 35
#define TKAmpersand 36
#define TKBitwiseAndAssignment 37
#define TKBitwiseOr 38
#define TKBitwiseOrAssignment 39
#define TKBitwiseXor 40
#define TKBitwiseXorAssignment 41
#define TKBitwiseNot 42
#define TKRightShift 43
#define TKLeftShift 44
#define TKGreaterThan 45
#define TKLessThan 46
#define TKGreaterThanOrEqual 47
#define TKLessThanOrEqual 48
#define TKNotEqual 49
#define TKLogicalAnd 50
#define TKLogicalOr 51
#define TKLogicalNot 52
#define TKPeriod 53
#define TKArrow 54
#define TKLeftParenthesis 55
#define TKRightParenthesis 56
#define TKLeftBracket 57
#define TKRightBracket 58
#define TKLeftCurlyBrace 59
#define TKRightCurlyBrace 60
#define TKComma 61
#define TKSemiColon 62
#define TKColon 63

//Erros
#define ERR_TYPE_NAME 1
#define ERR_LONG 2
#define ERR_LONG_LONG 3
#define ERR_SEMICOLON_DEC 4
#define ERR_DIR_DEC_ID 5
#define ERR_BLK_LFT_CUR 6
#define ERR_BLK_RIG_CUR 7
#define ERR_STM 8
#define ERR_SEMICOLON_EXP 9
#define ERR_LFT_PAR_IF 10
#define ERR_EXP_IF 11
#define ERR_RIG_PAR_IF 12
#define ERR_LFT_PAR_WHILE 13
#define ERR_EXP_WHILE 14
#define ERR_RIG_PAR_WHILE 15
#define ERR_WHILE_DO 16
#define ERR_SEMICOLON_DO 17
#define ERR_EXP_STM_FOR 18
#define ERR_LFT_PAR_FOR 19
#define ERR_RIG_PAR_FOR 20
#define ERR_EXP_FOR 21
#define ERR_SEMICOLON_JMP 22
#define ERR_EXP_STM_RTR 23
#define ERR_EXP_ASG 24
#define ERR_PRI_EXP 25
#define ERR_ARG_PFIX 26
#define ERR_ARG_RIG_PAR 27

//Tamanho da palavra
#define N 50
//Tamanho do buffer que armazena ultimos tokens lidos
#define L 6

typedef struct token {
    int code;
    char lexeme[N];
    char name[N];
} TOKEN;

typedef struct keyword {
    char word[N];
    int token;
} KEYWORD;

typedef struct last_tokens_list {
    char lexeme[N];
} LAST_TOKENS;

int current_line = -1;
int current_column = 0;
int current_token = 0;
char current_line_string[N];

int error_list[N];
int error_list_position = 0;

LAST_TOKENS last_tokens[N];

char file_name[N];
FILE *file;

KEYWORD reserved_words[] = {
    {"int", TKInt},
    {"float", TKFloat},
    {"char", TKChar},
    {"double", TKDouble},
    {"long", TKLong},
    {"unsigned", TKUnsigned},
    {"struct", TKStruct},
    {"while", TKWhile},
    {"for", TKFor},
    {"do", TKDo},
    {"if", TKIf},
    {"else", TKElse},
    {"switch", TKSwitch},
    {"case", TKCase},
    {"default", TKDefault},
    {"continue", TKContinue},
    {"break", TKBreak},
    {"return", TKReturn},
    {"sizeof", TKSizeof}
};

void update_last_tokens(char *lexeme){

    for (int i = 0; i < L; i++)
        strcpy(last_tokens[i].lexeme, last_tokens[i + 1].lexeme);

    strcpy(last_tokens[L - 1].lexeme, lexeme);
}

int is_constant(char string[N]){

    for (int i = 0; string[i] != '\0'; i++)
        if (isalpha(string[i]) || string[i] == '_')
            return 0;

    return 1;
}

TOKEN get_reserved_word(char string[N]){

    TOKEN token;

    token.code = 0;
    strcpy(token.lexeme, string);
    strcpy(token.name, "TK");

    for (int i = 0; i < sizeof(reserved_words) / sizeof(reserved_words[0]); i++)
        if (strcmp(string, reserved_words[i].word) == 0)
            token.code = reserved_words[i].token;

    if (token.code){
        string[0] = toupper(string[0]);
        strcat(token.name, string);
    } else {
        if (is_constant(string)){
            token.code = TKConstant;
            strcpy(token.name, "TKConstant");
        } else {
            token.code = TKId;
            strcpy(token.name, "TKId");
        }
    }

    return token;
}

TOKEN get_token(char string[N], int position){

    TOKEN token;
    int lexeme_position = 0;

    token.code = 0;
    token.lexeme[lexeme_position] = string[position];

    switch(string[position]){
        case '=':
            if (string[position + 1] == '='){
                token.code = TKEqual;
                strcpy(token.name, "TKEqual");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKAssignment;
                strcpy(token.name, "TKAssignment");
            }
            break;
        case '&': 
            if (string[position + 1] == '=') {
                token.code = TKBitwiseAndAssignment;
                strcpy(token.name, "TKBitwiseAndAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '&') {
                token.code = TKLogicalAnd; 
                strcpy(token.name, "TKLogicalAnd");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKAmpersand; 
                strcpy(token.name, "TKAmpersand");
            }
            break;
        case '|': 
            if (string[position + 1] == '=') {
                token.code = TKBitwiseOrAssignment;
                strcpy(token.name, "TKBitwiseOrAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '|') {
                token.code = TKLogicalOr; 
                strcpy(token.name, "TKLogicalOr");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKBitwiseOr; 
                strcpy(token.name, "TKBitwiseOr");
            }
            break;
        case '>': 
            if (string[position + 1] == '=') {
                token.code = TKGreaterThanOrEqual;
                strcpy(token.name, "TKGreaterThanOrEqual");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '>'){
                token.code = TKRightShift;
                strcpy(token.name, "TKRightShift");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKGreaterThan;
                strcpy(token.name, "TKGreaterThan");
            }
            break;
        case '<': 
            if (string[position + 1] == '=') {
                token.code = TKLessThanOrEqual; 
                strcpy(token.name, "TKLessThanOrEqual");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '<') {
                token.code = TKLeftShift; 
                strcpy(token.name, "TKLeftShift");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKLessThan; 
                strcpy(token.name, "TKLessThan");
            }
            break;
        case '!': 
            if (string[position + 1] == '=') {
                token.code = TKNotEqual; 
                strcpy(token.name, "TKNotEqual");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKLogicalNot;
                strcpy(token.name, "TKLogicalNot");
            } 
            break;
        case '-': 
            if (string[position + 1] == '=') {
                token.code = TKSubtractionAssignment;
                strcpy(token.name, "TKSubtractionAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '-') {
                token.code = TKDecrement;
                strcpy(token.name, "TKDecrement");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '>') {
                token.code = TKArrow; 
                strcpy(token.name, "TKArrow");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKSubtraction; 
                strcpy(token.name, "TKSubtraction");
            }
            break;
        case '+': 
            if (string[position + 1] == '=') {
                token.code = TKSumAssignment;
                strcpy(token.name, "TKSumAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else if (string[position + 1] == '+') {
                token.code = TKIncrement;
                strcpy(token.name, "TKIncrement");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKSum; 
                strcpy(token.name, "TKSum"); 
            }
            break;
        case '*': 
            if (string[position + 1] == '=') {
                token.code = TKMultiplicationAssignment;
                strcpy(token.name, "TKMultiplicationAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKStar; 
                strcpy(token.name, "TKStar"); 
            }
            break;
        case '/': 
            if (string[position + 1] == '=') {
                token.code = TKDivisionAssignment;
                strcpy(token.name, "TKDivisionAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKDivision;
                strcpy(token.name, "TKDivision");
            }
            break;
        case '%': 
            if (string[position + 1] == '=') {
                token.code = TKModulusAssignment;
                strcpy(token.name, "TKModulusAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKModulus; 
                strcpy(token.name, "TKModulus"); 
            }
            break;
        case '^': 
            if (string[position + 1] == '=') {
                token.code = TKBitwiseXorAssignment;
                strcpy(token.name, "TKBitwiseXorAssignment");
                token.lexeme[++lexeme_position] = string[position + 1];
            } else {
                token.code = TKBitwiseXor; 
                strcpy(token.name, "TKBitwiseXor"); 
            }
            break;
        case '~': 
            token.code = TKBitwiseNot; 
            strcpy(token.name, "TKBitwiseNot"); 
            break;
        case '.': 
            token.code = TKPeriod; 
            strcpy(token.name, "TKPeriod"); 
            break;
        case '(': 
            token.code = TKLeftParenthesis; 
            strcpy(token.name, "TKLeftParenthesis"); 
            break;
        case ')': 
            token.code = TKRightParenthesis; 
            strcpy(token.name, "TKRightParenthesis"); 
            break;
        case '[': 
            token.code = TKLeftBracket; 
            strcpy(token.name, "TKLeftBracket"); 
            break;
        case ']': 
            token.code = TKRightBracket; 
            strcpy(token.name, "TKRightBracket"); 
            break;
        case '{': 
            token.code = TKLeftCurlyBrace; 
            strcpy(token.name, "TKLeftCurlyBrace"); 
            break;
        case '}': 
            token.code = TKRightCurlyBrace; 
            strcpy(token.name, "TKRightCurlyBrace"); 
            break;
        case ',': 
            token.code = TKComma; 
            strcpy(token.name, "TKComma"); 
            break;
        case ';': 
            token.code = TKSemiColon; 
            strcpy(token.name, "TKSemiColon"); 
            break;
        case ':': 
            token.code = TKColon; 
            strcpy(token.name, "TKColon"); 
            break;
    }

    token.lexeme[lexeme_position + 1] = '\0';

    return token;
}

void read_next_line(){
    memset(current_line_string, 0, sizeof(current_line_string));
    fgets(current_line_string, sizeof(current_line_string), file);
    current_column = 0;
    current_line++;
}

void analyse_file(FILE *file_source){

    if (current_line == -1)
        read_next_line();

    TOKEN token;
    char word[N];
    int word_position = 0;
    token.code = 0;

    for (int i = current_column; current_line_string[i] != '\0'; i++){

        if (current_line_string[i] == '\n'){
            i = -1;
            read_next_line();
            continue;
        }

        if (current_line_string[i] == '\0' || current_line_string[i] == ' ' || current_line_string[i] == '\t' || current_line_string[i] == '\r'){
            current_column++;
            continue;
        }

        if (!isdigit(current_line_string[i]) && !isalpha(current_line_string[i]) && current_line_string[i] != '_'){
            token = get_token(current_line_string, i);

            if (token.code){
                update_last_tokens(token.lexeme);
                i += strlen(token.lexeme) - 1;
                current_column = i + 1;
                break;
            } else{
                token.code = -1;
                break;
            }
        } else {

            if (isdigit(current_line_string[i + 1]) || isalpha(current_line_string[i + 1]) || current_line_string[i + 1] == '_'){
                word[word_position] = current_line_string[i];
                word_position++;
            }

            if (current_line_string[i + 1] == ' ' || get_token(current_line_string, i + 1).code != 0 || current_line_string[i + 1] == '\0' || current_line_string[i + 1] == '\n' || current_line_string[i + 1] == '\t' || current_line_string[i + 1] == '\r'){
                word[word_position] = current_line_string[i];
                word[word_position + 1] = '\0';

                token = get_reserved_word(word);

                update_last_tokens(token.lexeme);

                memset(word, 0, sizeof(word));
                word_position = 0;

                current_column = i - strlen(word) + 1;
                break;
            }
        }
    }

    current_token = token.code;

    memset(error_list, 0, sizeof(error_list));
    error_list_position = 0;
}

int START();
int START_LIST();
int EXTERNAL_DECLARATION();
int EXTERNAL_DECLARATION_();
int TYPE();
int TYPE_NAME();
int LONG_DECLARATION();
int DECLARATION();
int INIT_DECLARATOR_LIST();
int INIT_DECLARATOR();
int DECLARATOR();
int DIRECT_DECLARATOR();
int DIRECT_DECLARATOR_();
int DIRECT_DECLARATOR_PARENTHESIS();
int DIRECT_DECLARATOR_BRACKET();
int PARAMETER_LIST();
int PARAMETER_LIST_();
int PARAMETER();
int PARAMETER_IDENTIFIER();
int IDENTIFIER_LIST();
int COMPOUND_BLOCK();
int COMPOUND_BLOCK_CORE();
int STATEMENT_LIST();
int STATEMENT();
int ELSE_STATEMENT();
int FOR_ENCLOSED();
int EXPRESSION_STATEMENT();
int EXPRESSION();
int EXPRESSION_();
int ASSIGNMENT_EXPRESSION();
int ASSIGNMENT_EXPRESSION_();
int LOGICAL_OR_EXPRESSION();
int LOGICAL_OR_EXPRESSION_();
int LOGICAL_AND_EXPRESSION();
int LOGICAL_AND_EXPRESSION_();
int BITWISE_OR_EXPRESSION();
int BITWISE_OR_EXPRESSION_();
int BITWISE_XOR_EXPRESSION();
int BITWISE_XOR_EXPRESSION_();
int BITWISE_AND_EXPRESSION();
int BITWISE_AND_EXPRESSION_();
int EQUALITY_EXPRESSION();
int EQUALITY_EXPRESSION_();
int RELATIONAL_EXPRESSION();
int RELATIONAL_EXPRESSION_();
int SHIFT_EXPRESSION();
int SHIFT_EXPRESSION_();
int ADDITIVE_EXPRESSION();
int ADDITIVE_EXPRESSION_();
int MULTIPLICATIVE_EXPRESSION();
int MULTIPLICATIVE_EXPRESSION_();
int UNARY_EXPRESSION();
int UNARY_OPERATOR();
int POSTFIX_EXPRESSION();
int POSTFIX_EXPRESSION_();
int POSTFIX_EXPRESSION_PARENTHESIS();
int ARGUMENT_LIST();
int ARGUMENT_LIST_();
int PRIMARY_EXPRESSION();

void print_error_message(int err_code){
    switch(err_code){
        case ERR_TYPE_NAME: printf("Esperada definicao de tipo. "); break;
        case ERR_LONG: printf("Esperado 'int' ou 'long int' apos 'long'. "); break;
        case ERR_LONG_LONG: printf("Esperado 'int' apos 'long long'. "); break;
        case ERR_SEMICOLON_DEC: printf("Esperado ';' apos declaracao. "); break;
        case ERR_DIR_DEC_ID: printf("Esperado identificador. "); break;
        case ERR_BLK_LFT_CUR: printf("Esperado '{' para abertura de comandos compostos. "); break;
        case ERR_BLK_RIG_CUR: printf("Esperado '}' para encerramento de comandos compostos. "); break;
        case ERR_STM: printf("Esperado comando composto. "); break;
        case ERR_SEMICOLON_EXP: printf("Esperado ';' apos expressao. "); break;
        case ERR_LFT_PAR_IF: printf("Esperado '(' apos comando 'if'. "); break;
        case ERR_EXP_IF: printf("Esperada expressao para comando 'if'. "); break;
        case ERR_RIG_PAR_IF: printf("Esperado ')' apos expressao para comando 'if'. "); break;
        case ERR_LFT_PAR_WHILE: printf("Esperado '(' apos comando 'while'. "); break;
        case ERR_EXP_WHILE: printf("Esperada expressao para comando 'while'. "); break;
        case ERR_RIG_PAR_WHILE: printf("Esperado ')' apos expressao para comando 'while'. "); break;
        case ERR_WHILE_DO: printf("Esperado comando 'while' apos bloco de comandos para comando 'do'. "); break;
        case ERR_SEMICOLON_DO: printf("Esperado ';' para concluir comando 'do'. "); break;
        case ERR_EXP_STM_FOR: printf("Esperado comando expressao para comando 'for'. "); break;
        case ERR_LFT_PAR_FOR: printf("Esperado '(' apos comando 'for'. "); break;
        case ERR_RIG_PAR_FOR: printf("Esperado ')' apos expressao para comando 'for'. "); break;
        case ERR_EXP_FOR: printf("Esperada expressao para comando 'for'. "); break;
        case ERR_SEMICOLON_JMP: printf("Esperado ';' apos comando jump. "); break;
        case ERR_EXP_STM_RTR: printf("Esperado comando expressao apos comando 'return'. "); break;
        case ERR_EXP_ASG: printf("Esperada expressao de atribuicao apos comando de atribuicao. "); break;
        case ERR_PRI_EXP: printf("Esperada expressao primaria. "); break;
        case ERR_ARG_PFIX: printf("Esperado argumentos ou ')'. "); break;
        case ERR_ARG_RIG_PAR: printf("Esperado ')' apos lista de parametros. "); break;
        default: printf("Comando incorreto. "); break;
    }
}

void print_syntax_error(){

    printf("\nLinha   Coluna   Arquivo   Mensagem\n");

    for (int i = 0; i < error_list[i] != 0; i++){
        printf("%.2d %7.2d %13s   ", current_line, current_column, file_name);
        printf("[Erro sintatico]: ");
        print_error_message(error_list[i]);
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < L; i++){
        printf("%s ", last_tokens[i].lexeme);
    }

    printf("\n");

    for (int i = 0; i < L - 1; i++){
        int spaces = strlen(last_tokens[i].lexeme);
        for (int j = 0; j < spaces; j++){
            printf(" ");
        }
        printf(" ");
    }
    printf("^ %d:%d\n", current_line, current_column);
}

void handle_syntax_error(int err_code){

    int listed_error = 0;

    for (int i = 0; i < N; i++)
        if (error_list[i] == err_code)
            listed_error = 1;

    if (!listed_error){
        error_list[error_list_position] = err_code;
        error_list_position++; 
    }
}

void stop_analysis(){
    print_syntax_error();
    exit(0);
}

int START(){
    if (EXTERNAL_DECLARATION()){
        if (feof(file)) {
            return 1;
        } else if (START_LIST()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int START_LIST(){
    if (EXTERNAL_DECLARATION()){
        if (START_LIST()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int EXTERNAL_DECLARATION(){
    if (TYPE()){
        if (DECLARATOR()){
            if (EXTERNAL_DECLARATION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 0;
}

int EXTERNAL_DECLARATION_(){
    if (COMPOUND_BLOCK()){
        return 1;
    } else if (DECLARATION()){
        return 1;
    } else
        return 0;
}

int TYPE(){
    if (current_token == TKUnsigned){
        analyse_file(file);
        if (TYPE_NAME()){
            return 1;
        } else
            return 0;
    } else if (TYPE_NAME()){
        return 1;
    } else
       return 0;
}

int TYPE_NAME(){
    if (current_token == TKInt){
        analyse_file(file);
        return 1;
    } else if (current_token == TKLong){
        analyse_file(file);
        if (LONG_DECLARATION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKFloat){
        analyse_file(file);
        return 1;
    } else if (current_token == TKChar){
        analyse_file(file);
        return 1;
    } else if (current_token == TKDouble){
        analyse_file(file);
        return 1;
    } else {
       handle_syntax_error(ERR_TYPE_NAME);
       return 0;
    }
}

int LONG_DECLARATION(){
    if (current_token == TKInt){
        analyse_file(file);
        return 1;
    } else if (current_token == TKLong){
        analyse_file(file);
        if (current_token == TKInt){
            analyse_file(file);
            return 1;
        } else {
            handle_syntax_error(ERR_LONG_LONG);
            stop_analysis();
            return 0;
        }
    } else {
        handle_syntax_error(ERR_LONG);
        stop_analysis();
        return 0;
    }
}

int DECLARATION(){
    if (INIT_DECLARATOR()){
        if (INIT_DECLARATOR_LIST()){
            if (current_token == TKSemiColon){
                analyse_file(file);
                return 1;
            } else {
                handle_syntax_error(ERR_SEMICOLON_DEC);
                stop_analysis();
                return 0;
            }
        } else
            return 0;
    } else
        return 0;
}

int INIT_DECLARATOR_LIST(){
    if (current_token == TKComma){
        analyse_file(file);
        if (DECLARATOR()){
            if (INIT_DECLARATOR()){
                if (INIT_DECLARATOR_LIST()){
                    return 1;
                } else
                    return 0;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int INIT_DECLARATOR(){
    if (current_token == TKAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int DECLARATOR(){
    if (current_token == TKStar){
        analyse_file(file);
        if (DIRECT_DECLARATOR()){
            return 1;
        } else
            return 0;
    } else if (DIRECT_DECLARATOR()){
        return 1;
    } else
        return 0;
}

int DIRECT_DECLARATOR(){
    if (current_token == TKId){
        analyse_file(file);
        if (DIRECT_DECLARATOR_()){
            return 1;
        } else  
            return 0;
    } else {
        handle_syntax_error(ERR_DIR_DEC_ID);
        stop_analysis();
        return 0;
    }
}

int DIRECT_DECLARATOR_(){
    if (current_token == TKLeftParenthesis){
        analyse_file(file);
        if (DIRECT_DECLARATOR_PARENTHESIS()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKLeftBracket){
        analyse_file(file);
        if (DIRECT_DECLARATOR_BRACKET()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int DIRECT_DECLARATOR_PARENTHESIS(){
    if (PARAMETER_LIST()){
        if (current_token == TKRightParenthesis){
            analyse_file(file);
            if (DIRECT_DECLARATOR_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (IDENTIFIER_LIST()){
        if (current_token == TKRightParenthesis){
            analyse_file(file);
            if (DIRECT_DECLARATOR_()){
                return 1;
            } else
                return 0;  
        } else
            return 0;  
    } else if (current_token == TKRightParenthesis){
        analyse_file(file);
        if (DIRECT_DECLARATOR_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int DIRECT_DECLARATOR_BRACKET(){
    if (current_token == TKConstant){
        analyse_file(file);
        if (current_token == TKRightBracket){
            analyse_file(file);
            if (DIRECT_DECLARATOR_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKRightBracket){
        analyse_file(file);
        if (DIRECT_DECLARATOR_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int PARAMETER_LIST(){
    if (PARAMETER()){
        if (PARAMETER_LIST_()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int PARAMETER_LIST_(){
    if (current_token == TKComma){
        analyse_file(file);
        if (PARAMETER()){
            if (PARAMETER_LIST_()){
                return 1;
            } else
                return 0;
        } else {
            stop_analysis();
            return 0;
        }
    } else
        return 1;
}

int PARAMETER(){
    if (TYPE()){
        if (PARAMETER_IDENTIFIER()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int PARAMETER_IDENTIFIER(){
    if (current_token == TKStar){
        analyse_file(file);
        if (current_token == TKId){
            analyse_file(file);
            return 1;
        } else {
            handle_syntax_error(ERR_DIR_DEC_ID);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKId){
        analyse_file(file);
        return 1;
    } else {
        handle_syntax_error(ERR_DIR_DEC_ID);
        stop_analysis();
        return 0;
    }
}

int IDENTIFIER_LIST(){
    if (current_token == TKId){
        analyse_file(file);
        if (IDENTIFIER_LIST()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int COMPOUND_BLOCK(){
    if (current_token == TKLeftCurlyBrace){
        analyse_file(file);
        if (COMPOUND_BLOCK_CORE()){
            return 1;
        } else
            return 0;
    } else {
        handle_syntax_error(ERR_BLK_LFT_CUR);
        stop_analysis();
        return 0;
    }
}

int COMPOUND_BLOCK_CORE(){
    if (current_token == TKRightCurlyBrace){
        analyse_file(file);
        return 1;
    } else if (STATEMENT()){
        if (STATEMENT_LIST()){
            if (current_token == TKRightCurlyBrace){
                analyse_file(file);
                return 1;
            } else {
                handle_syntax_error(ERR_BLK_RIG_CUR);
                stop_analysis();
                return 0;
            }
        } else
            return 0;
    } else {
        handle_syntax_error(ERR_BLK_RIG_CUR);
        stop_analysis();
        return 0;
    }
}

int STATEMENT_LIST(){
    if (STATEMENT()){
        if (STATEMENT_LIST()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int STATEMENT(){
    if (TYPE()){
        if (DECLARATOR()){
            if (DECLARATION()){
                return 1;
            } else {
                stop_analysis();
                return 0;
            }
        } else {
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKSemiColon){
        analyse_file(file);
        return 1;
    } else if (EXPRESSION()){
        if (current_token == TKSemiColon){
            analyse_file(file);
            return 1;
        } else {
            handle_syntax_error(ERR_SEMICOLON_EXP);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKIf){
        analyse_file(file);
        if (current_token == TKLeftParenthesis){
            analyse_file(file);
            if (EXPRESSION()){
                if (current_token == TKRightParenthesis){
                    analyse_file(file);
                    if (COMPOUND_BLOCK()){
                        if (ELSE_STATEMENT()){
                            return 1;
                        } else
                            return 0;
                    } else
                        return 0;
                } else {
                    handle_syntax_error(ERR_RIG_PAR_IF);
                    stop_analysis();
                    return 0;
                }
            } else {
                handle_syntax_error(ERR_EXP_IF);
                stop_analysis();
                return 0;
            }
        } else {
            handle_syntax_error(ERR_LFT_PAR_IF);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKWhile){
        analyse_file(file);
        if (current_token == TKLeftParenthesis){
            analyse_file(file);
            if (EXPRESSION()){
                if (current_token == TKRightParenthesis){
                    analyse_file(file);
                    if (COMPOUND_BLOCK()){
                        return 1;
                    } else
                        return 0;
                } else {
                    handle_syntax_error(ERR_RIG_PAR_WHILE);
                    stop_analysis();
                    return 0;
                }
            } else {
                handle_syntax_error(ERR_EXP_WHILE);
                stop_analysis();
                return 0;
            }
        } else {
            handle_syntax_error(ERR_LFT_PAR_WHILE);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKDo){
        analyse_file(file);
        if (COMPOUND_BLOCK()){
            if (current_token == TKWhile){
                analyse_file(file);
                if (current_token == TKLeftParenthesis){
                    analyse_file(file);
                    if (EXPRESSION()){
                        if (current_token == TKRightParenthesis){
                            analyse_file(file);
                            if (current_token == TKSemiColon){
                                analyse_file(file);
                                return 1;
                            } else {
                                handle_syntax_error(ERR_SEMICOLON_DO);
                                stop_analysis();
                                return 0;
                            }
                        } else {
                            handle_syntax_error(ERR_RIG_PAR_WHILE);
                            stop_analysis();
                            return 0;
                        }
                    } else {
                        handle_syntax_error(ERR_EXP_WHILE);
                        stop_analysis();
                        return 0;
                    }
                } else {
                    handle_syntax_error(ERR_LFT_PAR_WHILE);
                    stop_analysis();
                    return 0;
                }
            } else {
                handle_syntax_error(ERR_WHILE_DO);
                stop_analysis();
                return 0;
            }
        } else
            return 0;
    } else if (current_token == TKFor){
        analyse_file(file);
        if (current_token == TKLeftParenthesis){
            analyse_file(file);
            if (EXPRESSION_STATEMENT()){
                if (EXPRESSION_STATEMENT()){
                    if (FOR_ENCLOSED()){
                        return 1;
                    } else
                        return 0;
                } else {
                    handle_syntax_error(ERR_EXP_STM_FOR);
                    stop_analysis();
                    return 0;
                }
            } else {
                handle_syntax_error(ERR_EXP_STM_FOR);
                stop_analysis();
                return 0;
            }
        } else {
            handle_syntax_error(ERR_LFT_PAR_FOR);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKContinue){
        analyse_file(file);
        if (current_token == TKSemiColon){
            analyse_file(file);
            return 1;
        } else {
            handle_syntax_error(ERR_SEMICOLON_JMP);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKBreak){
        analyse_file(file);
        if (current_token == TKSemiColon){
            analyse_file(file);
            return 1;
        } else {
            handle_syntax_error(ERR_SEMICOLON_JMP);
            stop_analysis();
            return 0;
        }
    } else if (current_token == TKReturn){
        analyse_file(file);
        if (EXPRESSION_STATEMENT()){
            return 1;
        } else {
            handle_syntax_error(ERR_EXP_STM_RTR);
            stop_analysis();
            return 0;
        }
    } else {
        handle_syntax_error(ERR_STM);
        return 0;
    }
}

int ELSE_STATEMENT(){
    if (current_token == TKElse){
        analyse_file(file);
        if (COMPOUND_BLOCK()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int FOR_ENCLOSED(){
    if (current_token == TKRightParenthesis){
        analyse_file(file);
        if (COMPOUND_BLOCK()){
            return 1;
        } else
            return 0;
    } else if (EXPRESSION()){
        if (current_token == TKRightParenthesis){
            analyse_file(file);
            if (COMPOUND_BLOCK()){
                return 1;
            } else
                return 0;
        } else {
            handle_syntax_error(ERR_RIG_PAR_FOR);
            stop_analysis();
            return 0;
        }
    } else {
        handle_syntax_error(ERR_EXP_FOR);
        stop_analysis();
        return 0;
    }
}

int EXPRESSION_STATEMENT(){
    if (current_token == TKSemiColon){
        analyse_file(file);
        return 1;
    } else if (EXPRESSION()){
        if (current_token == TKSemiColon){
            analyse_file(file);
            return 1;
        } else {
            handle_syntax_error(ERR_SEMICOLON_EXP);
            stop_analysis();
            return 0;
        }
    } else
        return 0;
}

int EXPRESSION(){
    if (ASSIGNMENT_EXPRESSION()){
        if (EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int EXPRESSION_(){
    if (current_token == TKComma){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            if (EXPRESSION_()){
                return 1;
            } else 
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int ASSIGNMENT_EXPRESSION(){
    if (LOGICAL_OR_EXPRESSION()){
        if (ASSIGNMENT_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int ASSIGNMENT_EXPRESSION_(){
    if (current_token == TKAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKSumAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKSubtractionAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKMultiplicationAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKDivisionAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKModulusAssignment){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int LOGICAL_OR_EXPRESSION(){
    if (LOGICAL_AND_EXPRESSION()){
        if (LOGICAL_OR_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int LOGICAL_OR_EXPRESSION_(){
    if (current_token == TKLogicalOr){
        analyse_file(file);
        if (LOGICAL_AND_EXPRESSION()){
            if (LOGICAL_OR_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int LOGICAL_AND_EXPRESSION(){
    if (BITWISE_OR_EXPRESSION()){
        if (LOGICAL_AND_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int LOGICAL_AND_EXPRESSION_(){
    if (current_token == TKLogicalAnd){
        analyse_file(file);
        if (BITWISE_OR_EXPRESSION()){
            if (LOGICAL_AND_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int BITWISE_OR_EXPRESSION(){
    if (BITWISE_XOR_EXPRESSION()){
        if (BITWISE_OR_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int BITWISE_OR_EXPRESSION_(){
    if (current_token == TKBitwiseOr){
        analyse_file(file);
        if (BITWISE_XOR_EXPRESSION()){
            if (BITWISE_OR_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int BITWISE_XOR_EXPRESSION(){
    if (BITWISE_AND_EXPRESSION()){
        if (BITWISE_XOR_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int BITWISE_XOR_EXPRESSION_(){
    if (current_token == TKBitwiseXor){
        analyse_file(file);
        if (BITWISE_AND_EXPRESSION()){
            if (BITWISE_XOR_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int BITWISE_AND_EXPRESSION(){
    if (EQUALITY_EXPRESSION()){
        if (BITWISE_AND_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int BITWISE_AND_EXPRESSION_(){
    if (current_token == TKAmpersand){
        analyse_file(file);
        if (EQUALITY_EXPRESSION()){
            if (BITWISE_AND_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int EQUALITY_EXPRESSION(){
    if (RELATIONAL_EXPRESSION()){
        if (EQUALITY_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int EQUALITY_EXPRESSION_(){
    if (current_token == TKEqual){
        analyse_file(file);
        if (RELATIONAL_EXPRESSION()){
            if (EQUALITY_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKNotEqual){
        analyse_file(file);
        if (RELATIONAL_EXPRESSION()){
            if (EQUALITY_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int RELATIONAL_EXPRESSION(){
    if (SHIFT_EXPRESSION()){
        if (RELATIONAL_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int RELATIONAL_EXPRESSION_(){
    if (current_token == TKGreaterThan){
        analyse_file(file);
        if (SHIFT_EXPRESSION()){
            if (RELATIONAL_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKGreaterThanOrEqual){
        analyse_file(file);
        if (SHIFT_EXPRESSION()){
            if (RELATIONAL_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKLessThan){
        analyse_file(file);
        if (SHIFT_EXPRESSION()){
            if (RELATIONAL_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKLessThanOrEqual){
        analyse_file(file);
        if (SHIFT_EXPRESSION()){
            if (RELATIONAL_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int SHIFT_EXPRESSION(){
    if (ADDITIVE_EXPRESSION()){
        if (SHIFT_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int SHIFT_EXPRESSION_(){
    if (current_token == TKRightShift){
        analyse_file(file);
        if (ADDITIVE_EXPRESSION()){
            if (SHIFT_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKLeftShift){
        analyse_file(file);
        if (ADDITIVE_EXPRESSION()){
            if (SHIFT_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int ADDITIVE_EXPRESSION(){
    if (MULTIPLICATIVE_EXPRESSION()){
        if (ADDITIVE_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int ADDITIVE_EXPRESSION_(){
    if (current_token == TKSum){
        analyse_file(file);
        if (MULTIPLICATIVE_EXPRESSION()){
            if (ADDITIVE_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKSubtraction){
        analyse_file(file);
        if (MULTIPLICATIVE_EXPRESSION()){
            if (ADDITIVE_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int MULTIPLICATIVE_EXPRESSION(){
    if (UNARY_EXPRESSION()){
        if (MULTIPLICATIVE_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int MULTIPLICATIVE_EXPRESSION_(){
    if (current_token == TKStar){
        analyse_file(file);
        if (UNARY_EXPRESSION()){
            if (MULTIPLICATIVE_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKDivision){
        analyse_file(file);
        if (UNARY_EXPRESSION()){
            if (MULTIPLICATIVE_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKModulus){
        analyse_file(file);
        if (UNARY_EXPRESSION()){
            if (MULTIPLICATIVE_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int UNARY_EXPRESSION(){
    if (current_token == TKIncrement){
        analyse_file(file);
        if (UNARY_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKDecrement){
        analyse_file(file);
        if (UNARY_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (UNARY_OPERATOR()){
        if (UNARY_EXPRESSION()){
            return 1;
        } else
            return 0;
    } else if (POSTFIX_EXPRESSION()){
        return 1;
    } else
        return 0;
}

int UNARY_OPERATOR(){
    if (current_token == TKAmpersand){
        analyse_file(file);
        return 1;
    } else if (current_token == TKStar){
        analyse_file(file);
        return 1;
    } else if (current_token == TKSum){
        analyse_file(file);
        return 1;
    } else if (current_token == TKSubtraction){
        analyse_file(file);
        return 1;
    } else if (current_token == TKLogicalNot){
        analyse_file(file);
        return 1;
    } else
        return 0;
}

int POSTFIX_EXPRESSION(){
    if (PRIMARY_EXPRESSION()){
        if (POSTFIX_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int POSTFIX_EXPRESSION_(){
    if (current_token == TKLeftBracket){
        analyse_file(file);
        if (EXPRESSION()){
            if (current_token == TKRightBracket){
                analyse_file(file);
                if (POSTFIX_EXPRESSION_()){
                    return 1;
                } else
                    return 0;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKLeftParenthesis){
        analyse_file(file);
        if (POSTFIX_EXPRESSION_PARENTHESIS()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKPeriod){
        analyse_file(file);
        if (current_token == TKId){
            analyse_file(file);
            if (POSTFIX_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKArrow){
        analyse_file(file);
        if (current_token == TKId){
            analyse_file(file);
            if (POSTFIX_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else if (current_token == TKIncrement){
        analyse_file(file);
        if (POSTFIX_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else if (current_token == TKDecrement){
        analyse_file(file);
        if (POSTFIX_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else
        return 1;
}

int POSTFIX_EXPRESSION_PARENTHESIS(){
    if (current_token == TKRightParenthesis){
        analyse_file(file);
        if (POSTFIX_EXPRESSION_()){
            return 1;
        } else
            return 0;
    } else if (ARGUMENT_LIST()){
        if (current_token == TKRightParenthesis){
            analyse_file(file);
            if (POSTFIX_EXPRESSION_()){
                return 1;
            } else
                return 0;
        } else {
            handle_syntax_error(ERR_ARG_RIG_PAR);
            stop_analysis();
            return 0;
        }
    } else {
        handle_syntax_error(ERR_ARG_PFIX);
        stop_analysis();
        return 0;
    }
}

int ARGUMENT_LIST(){
    if (ASSIGNMENT_EXPRESSION()){
        if (ARGUMENT_LIST_()){
            return 1;
        } else
            return 0;
    } else
        return 0;
}

int ARGUMENT_LIST_(){
    if (current_token == TKComma){
        analyse_file(file);
        if (ASSIGNMENT_EXPRESSION()){
            if (ARGUMENT_LIST_()){
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else
        return 1;
}

int PRIMARY_EXPRESSION(){
    if (current_token == TKId){
        analyse_file(file);
        return 1;
    } else if (current_token == TKConstant){
        analyse_file(file);
        return 1;
    } if (current_token == TKLeftParenthesis){
        analyse_file(file);
        if (EXPRESSION()){
            if (current_token == TKRightParenthesis){
                analyse_file(file);
                return 1;
            } else
                return 0;
        } else
            return 0;
    } else {
        handle_syntax_error(ERR_PRI_EXP);
        return 0;
    }
}

int main(){

    strcpy(file_name, "fonte.c");

    file = fopen(file_name, "r");

    if (file == NULL){
        printf("Erro ao abrir arquivo fonte.\n");
        return 1;
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    analyse_file(file);
    if (START() == 1){
        clock_gettime(CLOCK_MONOTONIC, &end);
        printf("\nO programa %s foi reconhecido com sucesso.\n", file_name);
        printf("Tempo de compilacao: %f\n", (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9);
    } else {
        stop_analysis();
    }
}
