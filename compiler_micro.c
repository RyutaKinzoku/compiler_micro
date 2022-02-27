#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

extern char token_buffer[64];
int token_buffer_index = 0;

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
    char a[5] = {'B','E','G','I','N'};
    char b[3] = {'E','N','D'};
    char c[4] = {'R','E','A','D'};
    char d[5] = {'W','R','I','T','E'};
    if (compareToken(a, sizeof a))
    {
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
        printf("The character %c is only valid on assignments.", in_char);
    } else
    {
        printf("The character %c is not allowed.", in_char);
    }
    
}

token scanner(void){
    int in_char, c;

    clear_buffer();
    if(feof(stdin))
        return SCANEOF;

    while ((in_char = getchar()) != EOF){
        if(isspace(in_char))
            continue; 
        else if (isalpha(in_char)){
            buffer_char(in_char);
            for (c = getchar(); isalnum(c) || c == '_'; c = getchar()) 
                buffer_char(c);
            ungetc(c, stdin);
            return check_reserved();
        }
        else if (isdigit(in_char)){
            buffer_char(in_char);
            for (c = getchar(); isdigit(c); c = getchar()) 
                buffer_char(c);
            ungetc(c, stdin);
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
            c = getchar();
            if (c == '=')
                return ASSIGNOP;
            else {
                ungetc(c, stdin);
                lexical_error(in_char);
            }
        } else if (in_char == '-') {
            c = getchar();
            if (c == '-'){
                do
                    in_char = getchar();
                while (in_char != '\n');
            } else {
                ungetc(c, stdin);
                return MINUSOP;
            }
        } else
            lexical_error(in_char);
    }
}

void match(token t){
    
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

void ident(expr_rec *t){
    match(ID);
    *t = process_id();
}

void id_list(void){
    match(ID);
    
    while(next_token()==COMMA){
        match(COMMA);
        match(ID);
    }
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

/*
void expr_list(void){
    expression();
    while (next_token() == COMMA){
        match(COMMA);
        expression();
    }
}
*/

void add_op(op_rec *op){
    token tok = next_token();
    if (tok == PLUSOP || tok == MINUSOP){
        match(tok);
        *op = process_op();
    }
    else
        syntax_error(tok);
}

void primary(expr_rec *e_rec){
    token tok = next_token();
    switch(tok){
    case LPAREN:
        match(LPAREN); //expression();
        match(RPAREN);
        break;
    case ID:
        ident(&e_rec);
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
    /*Semantic initializationMAs, none needed. */
}

void finish(void){
    generate("Halt", "", "", "");
}

void assign(expr_rec target, expr_rec source){
    generate("Store", extract(source), target.name, "");
}

op_rec process_op(void){
    op_rec o;
    if (token_buffer == PLUSOP)
        o.operator = PLUS;
    else 
        o.operator = MINUS;
    return o;
}

expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2)
{
    expr_rec e_rec;
    e_rec.kind = TEMPEXPR;

    strcopy(e_rec.name, get_temp());
    generate(extract(op), extract(e1), extract(e2), e_rec.name);
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
