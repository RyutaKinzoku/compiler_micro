#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXIDLEN 65
#define IDQUANTITY 1024

typedef enum {FALSE = 0, TRUE} boolean;
typedef enum token_types {
    BEGIN, END, READ, WRITE, ID, INTLITERAL, 
    LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGNOP,
    PLUSOP, MINUSOP, IFOP, SCANEOF,COMMENT
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
    "PLUSOP", "MINUSOP", "IFOP", "SCANEOF", "COMMENT"};


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



#define INT_MAX 0x7fffffff
#define INT_MIN -2147483648


void buffer_char(char c){
    token_buffer[token_buffer_index] = c;
    token_buffer_index++;
    if(isdigit(token_buffer[0])){
        if (atol(token_buffer)>INT_MAX)
        {
            printf("Literal is too long\n");
            FILE *x86_code;
            x86_code = fopen("x86code.s", "w");
            fclose(x86_code);
            exit(-1);
        }
    }
    if (token_buffer_index > MAXIDLEN-1)
    {
        printf("Identifier or literal is too long\n");
        FILE *x86_code;
        x86_code = fopen("x86code.s", "w");
        fclose(x86_code);
        exit(-1);
    }
}



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
                return COMMENT;
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


int lookup(string s){
    for(int i = 0; i<symTabIndex; i++){
        if(!strcmp(s, symbolTable[i])){
            return 1;
        }
    }
    return 0;
}


void enter(string s){
    strcpy(symbolTable[symTabIndex], s);
    symTabIndex++;
    if (symTabIndex > IDQUANTITY)
    {
        printf("Too many symbols\n");
        x86_code = fopen("x86code.s", "w");
        fclose(x86_code);
        exit(-1);
    }
    
}


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


void check_id(string s)
{
    if(!lookup(s)){
        enter(s);
        declare_id(s);
    }
}


char *get_temp(void)
{
    static int max_temp = 0;
    static char tempname[MAXIDLEN];

    max_temp++;
    
    sprintf(tempname, "Temp%d", max_temp);
    check_id(tempname);
    return tempname;
}


void start(void){
    /*Semantic initializations. */
    micro_code = fopen("code", "r");
    x86_code = fopen("x86code.s", "w");
    if (micro_code == NULL || x86_code == NULL){
        printf("Error! Could not open file\n");
        exit(-1);
    }
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s", "global _start\n");
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "%s", "section .text\n");
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "%s", "_start:\n");
    fclose(x86_code);
}


void finish(void){
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s", "\tmov rax, 60\n");
    fprintf(x86_code, "%s", "\tsyscall\n");
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "%s", "section .data\n");
    fprintf(x86_code, "%s", "\tnewline db 10\n");
    fprintf(x86_code, "%s", "\tminus db 45\n");
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


void assign(expr_rec target, expr_rec source){
    string source_value;
    strcpy(source_value, extract(source));
    FILE *x86_code;
    x86_code = fopen("x86code.s", "a+");

    
    if (source_value[0] == 45){
        fprintf(x86_code, "%s", "\tmov rax, ");
        fprintf(x86_code, "%s\n", source_value);
    }
    else if (source_value[0] < 48 || source_value[0] > 57)
    {
        fprintf(x86_code, "%s", "\tmov rax, [");
        fprintf(x86_code, "%s]\n", source_value);
    } 
    else
    {
        fprintf(x86_code, "%s", "\tmov rax, ");
        fprintf(x86_code, "%s\n", source_value);
    }

    fprintf(x86_code, "%s", "\tmov [");
    fprintf(x86_code, "%s", target.name);
    fprintf(x86_code, "%s", "], rax\n");
    fclose(x86_code);
}


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

int overflow(int val1, int val2, op_rec op){
    long result, lval1, lval2;
    lval1 = (long) val1;
    lval2 = (long) val2;
    if (op.operator == PLUS)
    {
        result = lval1 + lval2;
    } else
    {
        result = lval1 - lval2;
    }
    if (result > INT_MAX || result < INT_MIN)
    {
        printf("Integer overflow\n");
        FILE *x86_code;
        x86_code = fopen("x86code.s", "w");
        fclose(x86_code);
        exit(-1);
    }
    
    return (int) result;
    
}


expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2)
{
    expr_rec e_rec;
    // Constant folding
    if (e1.kind == LITERALEXPR && e2.kind == LITERALEXPR)
    {
        e_rec.kind = LITERALEXPR;
        e_rec.value = overflow(e1.value, e2.value, op);
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

char* get_read_label(string text){
    static int max_read_label = 1;
    string num;
    static char read_buffer[MAXIDLEN];
    strcpy(read_buffer, text);
    sprintf(num, "%d", max_read_label);
    strcat(read_buffer, num);
    if(!strcmp(text, "fi"))
        max_read_label++;
    return read_buffer;
}

void read_id(expr_rec in_var){
    string be_loop, re_loop, fi_loop;
    FILE *x86_code;
    strcpy(be_loop, get_read_label("be"));
    strcpy(re_loop, get_read_label("re"));
    strcpy(fi_loop, get_read_label("fi"));
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s\n", "\tmov rdx, 64");
    fprintf(x86_code, "%s", "\tmov rsi, ");
    fprintf(x86_code, "%s\n", in_var.name);
    fprintf(x86_code, "%s\n", "\tmov rdi, 0");
    fprintf(x86_code, "%s\n", "\tmov rax, 0");
    fprintf(x86_code, "%s\n\n", "\tsyscall");

    fprintf(x86_code, "%s\n", "\txor rax, rax");
    fprintf(x86_code, "%s\n", "\txor rbx, rbx");
    fprintf(x86_code, "%s\n", "\txor rcx, rcx");
    fprintf(x86_code, "%s\n\n", "\txor r8, r8");

    fprintf(x86_code, "\t%s:\n", be_loop);
    fprintf(x86_code, "%s", "\tmov rcx, [");
    fprintf(x86_code, "%s+r8]\n\n", in_var.name);

    fprintf(x86_code, "\t%s:\n", re_loop);
    fprintf(x86_code, "%s", "\tcmp rcx, 0\n");
    fprintf(x86_code, "\tjz %s\n", fi_loop);
    fprintf(x86_code, "%s", "\tmov bl, cl\n");
    fprintf(x86_code, "%s", "\tshr rcx, 8\n");
    fprintf(x86_code, "%s", "\tcmp rbx, '0'\n");
    fprintf(x86_code, "\tjb %s\n", re_loop);
    fprintf(x86_code, "%s", "\tcmp rbx, '9'\n");
    fprintf(x86_code, "\tja %s\n", re_loop);
    fprintf(x86_code, "%s", "\tsub rbx, 48\n");
    fprintf(x86_code, "%s", "\timul rax, 10\n");
    fprintf(x86_code, "%s", "\tadd rax, rbx\n");
    fprintf(x86_code, "\tjmp %s\n\n", re_loop);

    fprintf(x86_code, "\t%s:\n", fi_loop);
    fprintf(x86_code, "%s", "\tadd r8, 8\n");
    fprintf(x86_code, "%s", "\tcmp r8, 9\n");
    fprintf(x86_code, "\tjb %s\n\n", be_loop);

    fprintf(x86_code, "\tmov qword [%s], 0\n\n", in_var.name);
    fprintf(x86_code, "\tmov [%s], rax\n\n", in_var.name);
    fclose(x86_code);
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

char* get_loop_labels(string text){
    static int max_loop_label = 1;
    string num;
    static char loop_buffer[MAXIDLEN];
    strcpy(loop_buffer, text);
    sprintf(num, "%d", max_loop_label);
    strcat(loop_buffer, num);
    if(!strcmp(text, "e"))
        max_loop_label++;
    return loop_buffer;
}

void write_expr(expr_rec out_expr)
{
    string l_loop, n_loop, e_loop;
    FILE *x86_code;
    strcpy(l_loop, get_loop_labels("l"));
    strcpy(n_loop, get_loop_labels("n"));
    strcpy(e_loop, get_loop_labels("e"));
    
    x86_code = fopen("x86code.s", "a+");
    fprintf(x86_code, "%s\n", "\txor r8, r8");
    fprintf(x86_code, "%s", "\tmov rax, ");
    if(out_expr.kind == LITERALEXPR) fprintf(x86_code, "%s\n", extract(out_expr));
    else fprintf(x86_code, "[%s]\n", extract(out_expr));
    fprintf(x86_code, "%s", "\n");
    
    fprintf(x86_code, "%s\n", "\ttest rax, rax");
    fprintf(x86_code, "%s", "\tjns ");
    fprintf(x86_code, "%s\n", l_loop);
    fprintf(x86_code, "%s\n", "\tmov rcx, minus");
    fprintf(x86_code, "%s\n", "\tmov rdx, 1");
    fprintf(x86_code, "%s\n", "\tmov rsi, rcx");
    fprintf(x86_code, "%s\n", "\tmov rdi, 1");
    fprintf(x86_code, "%s\n", "\tmov rax, 1");
    fprintf(x86_code, "%s\n", "\tsyscall");
    fprintf(x86_code, "%s\n", "\txor rax, rax");
    fprintf(x86_code, "%s", "\tmov rax, ");
    if(out_expr.kind == LITERALEXPR) fprintf(x86_code, "%s\n", extract(out_expr));
    else fprintf(x86_code, "[%s]\n", extract(out_expr));
    fprintf(x86_code, "%s\n", "\tneg rax");

    fprintf(x86_code, "\t%s:\n", l_loop);
    fprintf(x86_code, "%s\n", "\tmov rdx, 0");
    fprintf(x86_code, "%s\n", "\tmov rbx, 10");
    fprintf(x86_code, "%s\n", "\tdiv rbx");
    fprintf(x86_code, "%s\n", "\tadd rdx, '0'");
    fprintf(x86_code, "%s\n", "\tpush rdx");
    fprintf(x86_code, "%s\n", "\tinc r8");
    fprintf(x86_code, "%s\n", "\tcmp rax, 0");
    fprintf(x86_code, "%s", "\tjz ");
    fprintf(x86_code, "%s\n", n_loop);
    fprintf(x86_code, "%s", "\tjmp ");
    fprintf(x86_code, "%s\n", l_loop);
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "\t%s:\n", n_loop);
    fprintf(x86_code, "%s\n", "\tcmp r8, 0");
    fprintf(x86_code, "%s", "\tjz ");
    fprintf(x86_code, "%s\n", e_loop);
    fprintf(x86_code, "%s\n", "\tdec r8");
    fprintf(x86_code, "%s\n", "\tmov rcx, rsp");
    fprintf(x86_code, "%s\n", "\tmov rdx, 1");
    fprintf(x86_code, "%s\n", "\tmov rsi, rcx");
    fprintf(x86_code, "%s\n", "\tmov rdi, 1");
    fprintf(x86_code, "%s\n", "\tmov rax, 1");
    fprintf(x86_code, "%s\n", "\tsyscall");
    fprintf(x86_code, "%s\n", "\tadd rsp, 8");
    fprintf(x86_code, "%s", "\tjmp ");
    fprintf(x86_code, "%s\n", n_loop);
    fprintf(x86_code, "%s", "\n");
    fprintf(x86_code, "\t%s:\n", e_loop);
    fprintf(x86_code, "%s\n", "\tmov rcx, newline");
    fprintf(x86_code, "%s\n", "\tmov rdx, 1");
    fprintf(x86_code, "%s\n", "\tmov rsi, rcx");
    fprintf(x86_code, "%s\n", "\tmov rdi, 1");
    fprintf(x86_code, "%s\n", "\tmov rax, 1");
    fprintf(x86_code, "%s\n", "\tsyscall");
    fprintf(x86_code, "%s", "\n");
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
            c = getc(micro_code);
            if (c == '-'){
                ungetc(c, micro_code);
                ungetc(in_char, micro_code);
                return COMMENT;
            } else {
                ungetc(c, micro_code);
                ungetc(in_char, micro_code);
                return MINUSOP;
            }
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


void ident(expr_rec *t){
    match(ID);
    *t = process_id();
}


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


void add_op(op_rec *op){
    token tok = next_token();
    if (tok == PLUSOP || tok == MINUSOP){
        match(tok);
        *op = process_op();
    }
    else{
        syntax_error(tok);
    }
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
    if (condition.kind == LITERALEXPR)
    {
        fprintf(x86_code, "\tmov rax, %s\n", extract(condition));
        fprintf(x86_code, "%s", "\tcmp ");
        fprintf(x86_code, "%s, ", "rax");
    } else
    {
        fprintf(x86_code, "%s", "\tcmp ");
        fprintf(x86_code, "qword[%s], ", extract(condition));
    }
    fprintf(x86_code, "%s", "0\n");

    fprintf(x86_code, "%s", "\tjnz ");
    fprintf(x86_code, "%s\n",  then_if);
    fprintf(x86_code, "%s", "\tjmp ");
    fprintf(x86_code, "%s\n", else_if);
    fprintf(x86_code, "\t%s:\n", then_if);

    fprintf(x86_code, "\tmov %s, ", "rax");
    if(then_case.kind == LITERALEXPR) fprintf(x86_code, "%s\n", extract(then_case));
    else fprintf(x86_code, "[%s]\n", extract(then_case));
    fprintf(x86_code, "\tmov [%s], rax\n", e_rec.name);

    fprintf(x86_code, "%s", "\tJmp ");
    fprintf(x86_code, "%s\n", continue_if);
    fprintf(x86_code, "\t%s:\n", else_if);
    
    fprintf(x86_code, "\tmov %s, ", "rax");
    if(else_case.kind == LITERALEXPR) fprintf(x86_code, "%s\n", extract(else_case));
    else fprintf(x86_code, "[%s]\n", extract(else_case));
    fprintf(x86_code, "\tmov [%s], rax\n", e_rec.name);

    fprintf(x86_code, "\t%s:\n", continue_if);
    fclose(x86_code);
    return e_rec;
}


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
    case COMMENT:
        match(COMMENT);
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
        case COMMENT:
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