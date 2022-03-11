#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXIDLEN 65
#define IDQUANTITY 300

typedef enum {FALSE = 0, TRUE} boolean;
typedef enum token_types {
    BEGIN, END, READ, WRITE, ID, INTLITERAL, 
    LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP,
    PLUSOP, MINUSOP, IFOP, SCANEOF
} token;
typedef char string [MAXIDLEN];

typedef struct operator{
    enum op {PLUS, MINUS} operator;
} op_rec;

enum expr {IDEXPR, LITERALEXPR, TEMPEXPR};

typedef struct expression{
    enum expr kind;
    union{
        string name;
        int value;
    };
} expr_rec;

extern token scanner(void);

char token_buffer[MAXIDLEN];
int token_buffer_index = 0;
token current_token;
string symbolTable[IDQUANTITY]; 
int symTabIndex = 0;
FILE *micro_code;
FILE *x86_code;
char* declare_statements[IDQUANTITY][80];
int declare_index = 0;

const char* tokenNames[] = {"BEGIN", "END", "READ", "WRITE", "ID", "INTLITERAL", 
    "LPAREN", "RPAREN", "SEMICOLON", "COMMA", "ASSIGNOP",
    "PLUSOP", "MINUSOP", "IFOP", "SCANEOF"};

//Right
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


//Right
void clear_buffer(void){
    memset(token_buffer, 0, sizeof token_buffer);
    token_buffer_index = 0;
}


//Right
void buffer_char(char c){
    token_buffer[token_buffer_index] = c;
    token_buffer_index++;
    if (token_buffer_index > MAXIDLEN-1)
    {
        printf("Identifier or literal is too long\n");
        FILE *x86_code;
        x86_code = fopen("x86code.s", "w");
        fclose(x86_code);
        exit(-1);
    }
}


//Right
token check_reserved(){
    char a[] = {'b','e','g','i','n'};
    char b[] = {'e','n','d'};
    char c[] = {'r','e','a','d'};
    char d[] = {'w','r','i','t','e'};
    char e[] = {'S','C','A','N','E','O','F'};
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
    } else if (compareToken(e, sizeof e))
    {
        return SCANEOF;
    } else {
        return ID;
    }
}

//Right
void lexical_error(char in_char){
    if (in_char == ':')
    {
        printf("The character %c is only valid on assignments. \n", in_char);
    } else
    {
        printf("The character %c is not allowed. \n", in_char);
    }
    FILE *x86_code;
    x86_code = fopen("x86code.s", "w");
    fclose(x86_code);
    exit(-1);
    
}

//Right
token scanner(void){
    int in_char, c;

    clear_buffer();
    if(feof(micro_code))
        return SCANEOF;

    while ((in_char = getc(micro_code)) != EOF){
        if(isspace(in_char) || in_char == 0)
            continue; 
        else if (isalpha(in_char)){
            buffer_char(in_char);
            for (c = getc(micro_code); isalnum(c) || c == '_'; c = getc(micro_code)) 
                buffer_char(c);
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
        else if (in_char == '|')
            return IFOP;
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
//Right
void match(token t){
    token s = scanner();
    
    if (t == s)
    {
        current_token = t;
    } else
    {
        printf("Unexpected token ");
        printf("%s, ", tokenNames[s]);
        printf("Expected token ");
        printf("%s\n", tokenNames[t]);
        FILE *x86_code;
        x86_code = fopen("x86code.s", "w");
        fclose(x86_code);
        exit(-1);
    }
}

//Right
int lookup(string s){
    for(int i = 0; i<symTabIndex; i++){
        if(!strcmp(s, symbolTable[i])){
            return 1;
        }
    }
    return 0;
}

//Right
void enter(string s){
    strcpy(symbolTable[symTabIndex], s);
    symTabIndex++;
}

//Right
void generate(string opcode, string operand1, string operand2, string result){
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    printf("\t%s ", opcode);
    fprintf(x86_code,"\t%s ", opcode);
    if(operand1!="")
        printf("%s", operand1);
        fprintf(x86_code,"%s ", operand1);
    if(operand2!="")
        printf(",%s", operand2);
        fprintf(x86_code,"%s ", operand2);
    if(result!="")
        printf(",%s", result);
        fprintf(x86_code,"%s ", result);
    printf("\n");
    fprintf(x86_code, "\n");
    fclose(x86_code);
}

void declare_id(string name){
    sprintf(declare_statements[declare_index], "\t%s dq 0\n", name);
    declare_index++;
}

//Right
void check_id(string s)
{
    if(!lookup(s)){
        enter(s);
        declare_id(s);
    }
}

//Right
char *get_temp(void)
{
    static int max_temp = 0;
    static char tempname[MAXIDLEN];

    max_temp++;
    sprintf(tempname, "Temp%d", max_temp);
    check_id(tempname);
    return tempname;
}

//Right
void start(void){
    /*Semantic initializations. */
    micro_code = fopen("code", "r");
    x86_code = fopen("x86code.s", "r");
    if (micro_code == NULL || x86_code == NULL){
        printf("Error! Could not open file\n");
        exit(-1);
    }
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s", "global _start\n");
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "%s", "section .text\n");
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "%s", "_start:\n");
    fclose(x86_code);
}

//Right
void finish(void){
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s", "\tmov rax, 60\n");
    fprintf(x86_code, "%s", "\tsyscall\n");
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "%s", "section .data\n");
    for(int i = 0; i < declare_index; i++)
        fprintf(x86_code, "%s", declare_statements[i]);
    fclose(x86_code);
}

char* extract(expr_rec source){
    static char str[MAXIDLEN];
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
        return "sub";
    } else
    {
        return "add";
    }
}

//Right
void assign(expr_rec target, expr_rec source){
    string source_value;
    strcpy(source_value, extract(source));
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s", "\tmov rax, ");
    fprintf(x86_code, "%s\n", source_value);
    fprintf(x86_code, "%s", "\tmov ");
    fprintf(x86_code, "%s", target.name);
    fprintf(x86_code, "%s", ", rax\n");
    fclose(x86_code);
}

//Right
op_rec process_op(void){
    op_rec o;
    if (current_token == PLUSOP)
        o.operator = PLUS;
    else 
        o.operator = MINUS;
    return o;
}

void arithmetic_operation(string o, string op1, string op2, string result){
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    if (op1[0] < 48 || op1[0] > 57)
    {
        fprintf(x86_code, "%s", "\tmov rax, [");
        fprintf(x86_code, "%s]\n", op1);
    } else
    {    
        fprintf(x86_code, "%s", "\tmov rax, ");
        fprintf(x86_code, "%s\n", op1);
    }
    fprintf(x86_code, "\t%s", o);
    if (op2[0] < 48 || op2[0] > 57)
    {
        fprintf(x86_code, "%s", "\trax, [");
        fprintf(x86_code, "%s]\n", op2);
    } else
    {    
        fprintf(x86_code, "%s", "\trax, ");
        fprintf(x86_code, "%s\n", op2);
    }

    fprintf(x86_code, "%s", "\tmov [");
    fprintf(x86_code, "%s", result);
    
    fprintf(x86_code, "%s", "], rax\n");
    fclose(x86_code);
}

//Right
expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2)
{
    expr_rec e_rec;
    // Constant folding
    if (e1.kind == LITERALEXPR && e2.kind == LITERALEXPR)
    {
        e_rec.kind = LITERALEXPR;
        if (op.operator == PLUS)
        {
            e_rec.value = e1.value + e2.value;
        } else
        {
            e_rec.value = e1.value - e2.value;
        }
    } else
    {
        e_rec.kind = TEMPEXPR;
        strcpy(e_rec.name, get_temp());
        string o, ep1, ep2;
        strcpy(o, extract_op(op));
        strcpy(ep1, extract(e1));
        strcpy(ep2, extract(e2));
        arithmetic_operation(o, ep1, ep2, e_rec.name);
    }
    return e_rec;
}

void read_id(expr_rec in_var){
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s\n", "\tmov rdx, 64");
    fprintf(x86_code, "%s", "\tmov rsi, ");
    fprintf(x86_code, "%s\n", in_var.name);
    fprintf(x86_code, "%s\n", "\tmov rdi, 0");
    fprintf(x86_code, "%s\n", "\tmov rax, 0");
    fprintf(x86_code, "%s\n", "\tsyscall");
    fprintf(x86_code, "%s", "\tmov ");
    fprintf(x86_code, "%s, ", in_var.name);
    fprintf(x86_code, "%s\n", "rax");
    fclose(x86_code);
}

//Right
expr_rec process_id(void){
    expr_rec t;
    check_id(token_buffer);
    t.kind = IDEXPR;
    strcpy(t.name, token_buffer);
    return t;
}

//Right
expr_rec process_literal(void){
    expr_rec t;
    t.kind = LITERALEXPR;
    (void) sscanf(token_buffer, "%d", & t.value);
    return t;
}

void write_expr(expr_rec out_expr)
{
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s\n", "\tmov rdx, 64");
    fprintf(x86_code, "%s", "\tmov rsi, ");
    fprintf(x86_code, "%s\n", extract(out_expr));
    fprintf(x86_code, "%s\n", "\tmov rdi, 1");
    fprintf(x86_code, "%s\n", "\tmov rax, 1");
    fprintf(x86_code, "%s\n", "\tsyscall");
    fclose(x86_code);
}

//Maybe
token next_token(void){
    int in_char,c;
    clear_buffer();
    while ((in_char = getc(micro_code)) != EOF)
    {
        if (isspace(in_char) || in_char == 0)
            continue;
        else if (isalpha(in_char))
        {
            buffer_char(in_char);
            for (c = getc(micro_code); isalnum(c) || c == '_'; c = getc(micro_code)){
                buffer_char(c);
            }
            ungetc(c, micro_code);
            for (int i = token_buffer_index-1; i >= 0; i--){
                ungetc(token_buffer[i], micro_code);
            }
            return check_reserved();
        }else if (in_char == '('){
            ungetc(in_char, micro_code);
            return LPAREN;
        }
        else if (isdigit(in_char))
        {
            buffer_char(in_char);
            for (c = getc(micro_code); isdigit(c); c = getc(micro_code)) 
                buffer_char(c);
            ungetc(c, micro_code);
            for (int i = token_buffer_index-1; i >= 0; i--){
                ungetc(token_buffer[i], micro_code);
            }
            return INTLITERAL;
        }else if (in_char == '+'){
            ungetc(in_char, micro_code);
            return PLUSOP;
        } else if (in_char == '-'){
            ungetc(in_char, micro_code);
            return MINUSOP;
        } else if (in_char == '|'){
            ungetc(in_char, micro_code);
            return IFOP;
        } else if (in_char == ','){
            ungetc(in_char, micro_code);
            return COMMA;
        } else{
            ungetc(in_char, micro_code);
            return;
        }
    }
}

//Right
void ident(expr_rec *t){
    match(ID);
    *t = process_id();
}

//Right
void syntax_error(token t){
    printf("Syntax error with token ");
    printf("%s \n", tokenNames[t]);
    FILE *x86_code;
    x86_code = fopen("x86code.s", "w");
    fclose(x86_code);
    exit(-1);
}

void primary(expr_rec *e_rec){
    token tok = next_token();
    switch(tok){
    case LPAREN:
        match(LPAREN);
        expression(&*e_rec);
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

//Right
void add_op(op_rec *op){
    token tok = next_token();
    if (tok == PLUSOP || tok == MINUSOP){
        match(tok);
        *op = process_op();
    }
    else
        syntax_error(tok);
}

char* get_label(string text){
    static int max_label = 1;
    string num;
    static char buffer[MAXIDLEN];
    strcpy(buffer, text);
    sprintf(num, "%d", max_label);
    strcat(buffer, num);
    if(!strcmp(text, "continue"))
        max_label++;
    return buffer;
}

expr_rec gen_if(expr_rec condition, expr_rec then_case, expr_rec else_case){
    expr_rec e_rec;
    string then_if, else_if, continue_if;
    e_rec.kind = TEMPEXPR;
    strcpy(e_rec.name, get_temp());
    strcpy(then_if, get_label("then"));
    strcpy(else_if, get_label("else"));
    strcpy(continue_if, get_label("continue"));
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s", "\tcmp ");
    fprintf(x86_code, "%s, ", extract(condition));
    fprintf(x86_code, "%s", "0\n");
    fprintf(x86_code, "%s", "\tjnz ");
    fprintf(x86_code, "%s\n",  then_if);
    fprintf(x86_code, "%s", "\tjmp ");
    fprintf(x86_code, "%s\n", else_if);
    fprintf(x86_code, "\t%s:\n", then_if);
    fprintf(x86_code, "\tmov %s, ", e_rec.name);
    if(then_case.kind == LITERALEXPR) fprintf(x86_code, "%s\n", extract(then_case));
    else fprintf(x86_code, "[%s]\n", extract(then_case));
    fprintf(x86_code, "%s", "\tJmp ");
    fprintf(x86_code, "%s\n", continue_if);
    fprintf(x86_code, "\t%s:\n", else_if);
    fprintf(x86_code, "\tmov %s, ", e_rec.name);
    if(then_case.kind == LITERALEXPR) fprintf(x86_code, "%s\n", extract(else_case));
    else fprintf(x86_code, "[%s]\n", extract(else_case));
    fprintf(x86_code, "\t%s:\n", continue_if);
    fclose(x86_code);
    return e_rec;
}

//Right
void expression (expr_rec *result) {
    token t = next_token();
    expr_rec then_case, else_case;
    expr_rec first_operand, right_operand;
    op_rec op;
    switch (t)
    {
    case ID:
    case INTLITERAL:
    case LPAREN:
        primary(& first_operand);
        if (next_token()==IFOP)
        {
            match(IFOP);
            primary(&then_case);
            match(IFOP);
            primary(&else_case);
            first_operand = gen_if(first_operand, then_case, else_case);
        } else if (next_token() == PLUSOP || next_token() == MINUSOP)
        {
            while(next_token() == PLUSOP || next_token() == MINUSOP){
                add_op(& op);
                primary(& right_operand);
                first_operand = gen_infix(first_operand, op, right_operand);
            }
        } 
        *result = first_operand;
        break;
    default:
        syntax_error(t);
        break;
    }
}

void id_list(void){
    expr_rec e_rec;
    ident(&e_rec);
    read_id(e_rec);
    
    while(next_token()==COMMA){
        match(COMMA);
        ident(&e_rec);
        read_id(e_rec);
    }
}


void expr_list(void){
    expr_rec e_rec;
    expression(&e_rec);
    write_expr(e_rec);
    while (next_token() == COMMA){
        match(COMMA);
        expression(&e_rec);
        write_expr(e_rec);
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

void command(char c []){
    char command[MAXIDLEN];
    strcpy(command, c);
    system(command);
}

//Right
int main()
{
    printf("Compiling...\n\n");
    system_global();
    //Linux Commands
    command("nasm -f elf64 -o x86code.o x86code.s");
    command("ld -o exe x86code.o");
    printf("Running program...\n\n");
    command("./exe");
    return 0;
}