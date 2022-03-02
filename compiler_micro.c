#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXIDLEN 33

typedef enum {FALSE = 0, TRUE} boolean;
typedef enum token_types {
    BEGIN, END, READ, WRITE, ID, INTLITERAL, 
    LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP,
    PLUSOP, MINUSOP, SCANEOF
} token;
typedef char string [MAXIDLEN];

typedef struct operator{
    enum op {PLUS, MINUS} operator;
} op_rec;

enum expr {IDEXPR, LITERALEXPR, TEMPEXPR};

typedef struct expresion{
    enum expr kind;
    union{
        string name;
        int value;
    };
} expr_rec;

extern token scanner(void);

char token_buffer[64];
int token_buffer_index = 0;
token current_token;
string symbolTable[64]; 
int symTabIndex = 0;
FILE *micro_code;
FILE *x86_code;

int compareToken(char a[], int size){
    if(size == token_buffer_index){
        for(int i = 0; i<size; i++){
		    if(a[i]!=token_buffer[i])
		    	return 0;
	    }
	    return 1;
    } else {
        return 0;
    }
}

void clear_buffer(void){
    memset(token_buffer, 0, sizeof token_buffer);
    token_buffer_index = 0;
}

void buffer_char(char c){
    token_buffer[token_buffer_index] = c;
    token_buffer_index++;
}

token check_reserved(){
    char a[] = {'b','e','g','i','n'};
    char b[] = {'e','n','d'};
    char c[] = {'r','e','a','a'};
    char d[] = {'w','r','i','t','e'};
    if (compareToken(a, sizeof a))
    {
        printf("Madera\n");
        return BEGIN;
    } else if (compareToken(b, sizeof b))
    {
        return END;
    } else if (compareToken(c, sizeof c))
    {
        return READ;
    } else if (compareToken(d, sizeof d))
    {
        return WRITE;
    } else
    {
        return ID;
    }
}

void lexical_error(char in_char){
    if (in_char == ':')
    {
        printf("The character %c is only valid on assignments. \n", in_char);
    } else
    {
        printf("The character %c is not allowed. \n", in_char);
    }
    
}

token scanner(void){
    int in_char, c;

    clear_buffer();
    if(feof(micro_code))
        return SCANEOF;

    while ((in_char = getc(micro_code)) != EOF){
        if(isspace(in_char))
            continue; 
        else if (isalpha(in_char)){
            buffer_char(in_char);
            for (c = getc(micro_code); isalnum(c) || c == '_'; c = getc(micro_code)) 
                buffer_char(c);
            printf("%s\n",token_buffer);
            ungetc(c, micro_code);
            return check_reserved();
        }
        else if (isdigit(in_char)){
            buffer_char(in_char);
            for (c = getc(micro_code); isdigit(c); c = getc(micro_code)) 
                buffer_char(c);
            ungetc(c, micro_code);
            return INTLITERAL;
        }
        else if (in_char == '(')
            return LPAREN;
        else if (in_char == ')')
            return RPAREN;
        else if (in_char == ';')
            return SEMICOLON;
        else if (in_char == ',')
            return COMMA;
        else if (in_char == '+')
            return PLUSOP;
        else if (in_char == ':'){
            c = getc(micro_code);
            if (c == '=')
                return ASSIGNOP;
            else {
                ungetc(c, micro_code);
                lexical_error(in_char);
            }
        } else if (in_char == '-') {
            c = getc(micro_code);
            if (c == '-'){
                do
                    in_char = getc(micro_code);
                while (in_char != '\n');
            } else {
                ungetc(c, micro_code);
                return MINUSOP;
            }
        } else
            lexical_error(in_char);
    }
}

void match(token t){
    token s = scanner();
    if (t == s)
    {
        current_token = t;
    } else
    {
        printf("The token is not allowed.\n");
    }
}

int lookup(string s){
    for(int i = 0; i<symTabIndex; i++){
        if(!strcmp(s, symbolTable[i])){
            return 1;
        }
        return 0;
    }
}

void enter(string s){
    strcpy(symbolTable[symTabIndex], s);
    symTabIndex++;
}

void generate(string opcode, string operand1, string operand2, string result){
    printf(opcode,operand1,operand2,result);
}

void check_id(string s)
{
    if(!lookup(s)){
        enter(s);
        generate("Declare", s, "Integer", "");
    }
}

char *get_temp(void)
{
    static int max_temp = 0;
    static char tempname[MAXIDLEN];

    max_temp++;
    sprintf(tempname, "Temp&%d", max_temp);
    check_id(tempname);
    return tempname;
}

void start(void){
    /*Semantic initializations. */
    micro_code = fopen("code", "r");
    x86_code = fopen("x86code", "w");
    if (micro_code == NULL || x86_code == NULL){
        printf("Error! Could not open file\n");
        exit(-1);
    }
}

void finish(void){
    generate("Halt", "", "", "");
}

char* extract(expr_rec source){
    char* str;
    if (source.kind == LITERALEXPR)
    {
        int value = source.value;
        sprintf(str, "%d", value);
        return str;
    } else
    {
        strcpy(str, source.name);
        return str;
    }
}

char* extract_op(op_rec op){
    if (op.operator==MINUS)
    {
        return "SUB";
    } else
    {
        return "ADD";
    }
}

void assign(expr_rec target, expr_rec source){
    generate("Store", extract(source), target.name, "");
}

op_rec process_op(void){
    op_rec o;
    if (current_token == PLUSOP)
        o.operator = PLUS;
    else 
        o.operator = MINUS;
    return o;
}

expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2)
{
    expr_rec e_rec;
    e_rec.kind = TEMPEXPR;

    strcpy(e_rec.name, get_temp());
    generate(extract_op(op), extract(e1), extract(e2), e_rec.name);
    return e_rec;
}

void read_id(expr_rec in_var){
    generate("Read", in_var.name, "Integer", "");
}

expr_rec process_id(void){
    expr_rec t;
    check_id(token_buffer);
    t.kind = IDEXPR;
    strcpy(t.name, token_buffer);
    return t;
}

expr_rec process_literal(void){
    expr_rec t;
    t.kind = LITERALEXPR;
    (void) sscanf(token_buffer, "%d", & t.value);
    return t;
}

void write_expr(expr_rec out_expr)
{
    generate("Write", extract(out_expr), "Integer", "");
}

token next_token(){
    
}

void ident(expr_rec *t){
    match(ID);
    *t = process_id();
}

void syntax_error(token t){
    printf("Syntax error \n");
}

void primary(expr_rec *e_rec){
    token tok = next_token();
    switch(tok){
    case LPAREN:
        match(LPAREN); //expression();
        match(RPAREN);
        break;
    case ID:
        ident(&*e_rec);
        break;
    case INTLITERAL:
        match(INTLITERAL);
        *e_rec = process_literal();
        break;
    default:
        syntax_error(tok);
        break;
    }
}

void add_op(op_rec *op){
    token tok = next_token();
    if (tok == PLUSOP || tok == MINUSOP){
        match(tok);
        *op = process_op();
    }
    else
        syntax_error(tok);
}

void expression (expr_rec *result) {
    expr_rec left_operand, right_operand;
    op_rec op;

    primary(& left_operand);
    while(next_token() == PLUSOP || next_token() == MINUSOP ){
        add_op(& op);
        primary(& right_operand);
        left_operand = gen_infix(left_operand, op, right_operand);
    }
    *result = left_operand;
}

void id_list(void){
    match(ID);
    
    while(next_token()==COMMA){
        match(COMMA);
        match(ID);
    }
}


void expr_list(void){
    /*
    expression();
    while (next_token() == COMMA){
        match(COMMA);
        expression();
    }
    */
}


void statement (void)
{
    token tok = next_token();
    expr_rec e_rec, result;
    switch (tok){
    case ID:
        ident(&e_rec);
        match(ASSIGNOP);
        expression(&result);
        assign(e_rec, result);
        match(SEMICOLON);
        break;
    case READ:
        match(READ);
        match(LPAREN);
        id_list();
        match(RPAREN);
        match(SEMICOLON);
        break;
    case WRITE:
        match(WRITE);
        match(LPAREN);
        expr_list();
        match(RPAREN);
        match(SEMICOLON);
        break;
    default:
        syntax_error(tok);
        break;
    }
}

void statement_list(void){
    statement();
    while (TRUE){
        switch (next_token()){
        case ID:
        case READ:
        case WRITE:
            statement();
            break;
        default:
            return;
        }
    }
}

void program(void){
    start();
    match(BEGIN);
    statement_list();
    match(END);
}

void system_global(void){
    program();
    match(SCANEOF);
    finish();
}

int main()
{
    /*
    char c;
    c = fgetc(micro_code);
    while (c != EOF)
    {
        printf("%c",c);
        c=fgetc(micro_code);
    }
    fclose(x86_code);
    fclose(micro_code);*/
    
    //printf(&micro_code);
    system_global();
    return 0;
}