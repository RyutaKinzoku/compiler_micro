	.file	"compiler_micro.c"
	.text
	.comm	token_buffer,64,32
	.globl	token_buffer_index
	.bss
	.align 4
	.type	token_buffer_index, @object
	.size	token_buffer_index, 4
token_buffer_index:
	.zero	4
	.comm	current_token,4,4
	.comm	symbolTable,2112,32
	.globl	symTabIndex
	.align 4
	.type	symTabIndex, @object
	.size	symTabIndex, 4
symTabIndex:
	.zero	4
	.text
	.globl	compareToken
	.type	compareToken, @function
compareToken:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	token_buffer_index(%rip), %eax
	cmpl	%eax, -28(%rbp)
	jne	.L2
	movl	$0, -4(%rbp)
	jmp	.L3
.L6:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %edx
	movl	-4(%rbp), %eax
	cltq
	leaq	token_buffer(%rip), %rcx
	movzbl	(%rax,%rcx), %eax
	cmpb	%al, %dl
	je	.L4
	movl	$0, %eax
	jmp	.L5
.L4:
	addl	$1, -4(%rbp)
.L3:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L6
	movl	$1, %eax
	jmp	.L5
.L2:
	movl	$0, %eax
.L5:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	compareToken, .-compareToken
	.globl	clear_buffer
	.type	clear_buffer, @function
clear_buffer:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$64, %edx
	movl	$0, %esi
	leaq	token_buffer(%rip), %rdi
	call	memset@PLT
	movl	$0, token_buffer_index(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	clear_buffer, .-clear_buffer
	.globl	buffer_char
	.type	buffer_char, @function
buffer_char:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	movl	token_buffer_index(%rip), %eax
	cltq
	leaq	token_buffer(%rip), %rcx
	movzbl	-4(%rbp), %edx
	movb	%dl, (%rax,%rcx)
	movl	token_buffer_index(%rip), %eax
	addl	$1, %eax
	movl	%eax, token_buffer_index(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	buffer_char, .-buffer_char
	.globl	check_reserved
	.type	check_reserved, @function
check_reserved:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1229407554, -18(%rbp)
	movb	$78, -14(%rbp)
	movw	$20037, -25(%rbp)
	movb	$68, -23(%rbp)
	movl	$1145128274, -22(%rbp)
	movl	$1414091351, -13(%rbp)
	movb	$69, -9(%rbp)
	leaq	-18(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	compareToken
	testl	%eax, %eax
	je	.L10
	movl	$0, %eax
	jmp	.L15
.L10:
	leaq	-25(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	compareToken
	testl	%eax, %eax
	je	.L12
	movl	$1, %eax
	jmp	.L15
.L12:
	leaq	-22(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	compareToken
	testl	%eax, %eax
	je	.L13
	movl	$2, %eax
	jmp	.L15
.L13:
	leaq	-13(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	compareToken
	testl	%eax, %eax
	je	.L14
	movl	$3, %eax
	jmp	.L15
.L14:
	movl	$4, %eax
.L15:
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L16
	call	__stack_chk_fail@PLT
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	check_reserved, .-check_reserved
	.section	.rodata
	.align 8
.LC0:
	.string	"The character %c is only valid on assignments."
	.align 8
.LC1:
	.string	"The character %c is not allowed."
	.text
	.globl	lexical_error
	.type	lexical_error, @function
lexical_error:
.LFB4:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$58, -4(%rbp)
	jne	.L18
	movsbl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L20
.L18:
	movsbl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L20:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	lexical_error, .-lexical_error
	.globl	scanner
	.type	scanner, @function
scanner:
.LFB5:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	call	clear_buffer
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	feof@PLT
	testl	%eax, %eax
	je	.L24
	movl	$13, %eax
	jmp	.L21
.L42:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L25
	jmp	.L24
.L25:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$1024, %eax
	testl	%eax, %eax
	je	.L26
	movl	-4(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	buffer_char
	call	getchar@PLT
	movl	%eax, -8(%rbp)
	jmp	.L27
.L28:
	movl	-8(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	buffer_char
	call	getchar@PLT
	movl	%eax, -8(%rbp)
.L27:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8, %eax
	testl	%eax, %eax
	jne	.L28
	cmpl	$95, -8(%rbp)
	je	.L28
	movq	stdin(%rip), %rdx
	movl	-8(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	ungetc@PLT
	movl	$0, %eax
	call	check_reserved
	jmp	.L21
.L26:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$2048, %eax
	testl	%eax, %eax
	je	.L29
	movl	-4(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	buffer_char
	call	getchar@PLT
	movl	%eax, -8(%rbp)
	jmp	.L30
.L31:
	movl	-8(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	buffer_char
	call	getchar@PLT
	movl	%eax, -8(%rbp)
.L30:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$2048, %eax
	testl	%eax, %eax
	jne	.L31
	movq	stdin(%rip), %rdx
	movl	-8(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	ungetc@PLT
	movl	$5, %eax
	jmp	.L21
.L29:
	cmpl	$40, -4(%rbp)
	jne	.L32
	movl	$6, %eax
	jmp	.L21
.L32:
	cmpl	$41, -4(%rbp)
	jne	.L33
	movl	$7, %eax
	jmp	.L21
.L33:
	cmpl	$59, -4(%rbp)
	jne	.L34
	movl	$8, %eax
	jmp	.L21
.L34:
	cmpl	$44, -4(%rbp)
	jne	.L35
	movl	$9, %eax
	jmp	.L21
.L35:
	cmpl	$43, -4(%rbp)
	jne	.L36
	movl	$11, %eax
	jmp	.L21
.L36:
	cmpl	$58, -4(%rbp)
	jne	.L37
	call	getchar@PLT
	movl	%eax, -8(%rbp)
	cmpl	$61, -8(%rbp)
	jne	.L38
	movl	$10, %eax
	jmp	.L21
.L38:
	movq	stdin(%rip), %rdx
	movl	-8(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	ungetc@PLT
	movl	-4(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	lexical_error
	jmp	.L24
.L37:
	cmpl	$45, -4(%rbp)
	jne	.L39
	call	getchar@PLT
	movl	%eax, -8(%rbp)
	cmpl	$45, -8(%rbp)
	jne	.L40
.L41:
	call	getchar@PLT
	movl	%eax, -4(%rbp)
	cmpl	$10, -4(%rbp)
	jne	.L41
	jmp	.L24
.L40:
	movq	stdin(%rip), %rdx
	movl	-8(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	ungetc@PLT
	movl	$12, %eax
	jmp	.L21
.L39:
	movl	-4(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	lexical_error
.L24:
	call	getchar@PLT
	movl	%eax, -4(%rbp)
	cmpl	$-1, -4(%rbp)
	jne	.L42
.L21:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	scanner, .-scanner
	.section	.rodata
.LC2:
	.string	"The token is not allowed."
	.text
	.globl	match
	.type	match, @function
match:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	call	scanner
	movl	%eax, -4(%rbp)
	movl	-20(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jne	.L44
	movl	-20(%rbp), %eax
	movl	%eax, current_token(%rip)
	jmp	.L46
.L44:
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L46:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	match, .-match
	.globl	lookup
	.type	lookup, @function
lookup:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	nop
	movl	symTabIndex(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jge	.L52
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$5, %rax
	addq	%rdx, %rax
	leaq	symbolTable(%rip), %rdx
	addq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L49
	movl	$1, %eax
	jmp	.L47
.L49:
	movl	$0, %eax
	jmp	.L47
.L52:
.L47:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	lookup, .-lookup
	.globl	enter
	.type	enter, @function
enter:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	symTabIndex(%rip), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$5, %rax
	addq	%rdx, %rax
	leaq	symbolTable(%rip), %rdx
	addq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	strcpy@PLT
	movl	symTabIndex(%rip), %eax
	addl	$1, %eax
	movl	%eax, symTabIndex(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	enter, .-enter
	.globl	generate
	.type	generate, @function
generate:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	generate, .-generate
	.section	.rodata
.LC3:
	.string	""
.LC4:
	.string	"Integer"
.LC5:
	.string	"Declare"
	.text
	.globl	check_id
	.type	check_id, @function
check_id:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lookup
	testl	%eax, %eax
	jne	.L57
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	enter
	movq	-8(%rbp), %rax
	leaq	.LC3(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rdi
	call	generate
.L57:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	check_id, .-check_id
	.section	.rodata
.LC6:
	.string	"Temp&%d"
	.text
	.globl	get_temp
	.type	get_temp, @function
get_temp:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	max_temp.2706(%rip), %eax
	addl	$1, %eax
	movl	%eax, max_temp.2706(%rip)
	movl	max_temp.2706(%rip), %eax
	movl	%eax, %edx
	leaq	.LC6(%rip), %rsi
	leaq	tempname.2707(%rip), %rdi
	movl	$0, %eax
	call	sprintf@PLT
	leaq	tempname.2707(%rip), %rdi
	call	check_id
	leaq	tempname.2707(%rip), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	get_temp, .-get_temp
	.globl	start
	.type	start, @function
start:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	start, .-start
	.section	.rodata
.LC7:
	.string	"Halt"
	.text
	.globl	finish
	.type	finish, @function
finish:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC3(%rip), %rcx
	leaq	.LC3(%rip), %rdx
	leaq	.LC3(%rip), %rsi
	leaq	.LC7(%rip), %rdi
	call	generate
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	finish, .-finish
	.section	.rodata
.LC8:
	.string	"%d"
	.text
	.globl	extract
	.type	extract, @function
extract:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	16(%rbp), %eax
	cmpl	$1, %eax
	jne	.L63
	movl	20(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %edx
	movq	-8(%rbp), %rax
	leaq	.LC8(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf@PLT
	movq	-8(%rbp), %rax
	jmp	.L64
.L63:
	leaq	20(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movq	-8(%rbp), %rax
.L64:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	extract, .-extract
	.section	.rodata
.LC9:
	.string	"SUB"
.LC10:
	.string	"ADD"
	.text
	.globl	extract_op
	.type	extract_op, @function
extract_op:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	$1, %eax
	jne	.L66
	leaq	.LC9(%rip), %rax
	jmp	.L67
.L66:
	leaq	.LC10(%rip), %rax
.L67:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	extract_op, .-extract_op
	.section	.rodata
.LC11:
	.string	"Store"
	.text
	.globl	assign
	.type	assign, @function
assign:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	pushq	88(%rbp)
	pushq	80(%rbp)
	pushq	72(%rbp)
	pushq	64(%rbp)
	pushq	56(%rbp)
	call	extract
	addq	$48, %rsp
	movq	%rax, %rsi
	leaq	20(%rbp), %rax
	leaq	.LC3(%rip), %rcx
	movq	%rax, %rdx
	leaq	.LC11(%rip), %rdi
	call	generate
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	assign, .-assign
	.globl	process_op
	.type	process_op, @function
process_op:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	current_token(%rip), %eax
	cmpl	$11, %eax
	jne	.L70
	movl	$0, -4(%rbp)
	jmp	.L71
.L70:
	movl	$1, -4(%rbp)
.L71:
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	process_op, .-process_op
	.globl	gen_infix
	.type	gen_infix, @function
gen_infix:
.LFB18:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$64, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, -72(%rbp)
	movl	%esi, -76(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$2, -64(%rbp)
	call	get_temp
	movq	%rax, %rdx
	leaq	-64(%rbp), %rax
	addq	$4, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	subq	$8, %rsp
	pushq	88(%rbp)
	pushq	80(%rbp)
	pushq	72(%rbp)
	pushq	64(%rbp)
	pushq	56(%rbp)
	call	extract
	addq	$48, %rsp
	movq	%rax, %r12
	subq	$8, %rsp
	pushq	48(%rbp)
	pushq	40(%rbp)
	pushq	32(%rbp)
	pushq	24(%rbp)
	pushq	16(%rbp)
	call	extract
	addq	$48, %rsp
	movq	%rax, %rbx
	movl	-76(%rbp), %eax
	movl	%eax, %edi
	call	extract_op
	movq	%rax, %rdi
	leaq	-64(%rbp), %rax
	addq	$4, %rax
	movq	%rax, %rcx
	movq	%r12, %rdx
	movq	%rbx, %rsi
	call	generate
	movq	-72(%rbp), %rax
	movq	-64(%rbp), %rcx
	movq	-56(%rbp), %rbx
	movq	%rcx, (%rax)
	movq	%rbx, 8(%rax)
	movq	-48(%rbp), %rcx
	movq	-40(%rbp), %rbx
	movq	%rcx, 16(%rax)
	movq	%rbx, 24(%rax)
	movq	-32(%rbp), %rdx
	movq	%rdx, 32(%rax)
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L75
	call	__stack_chk_fail@PLT
.L75:
	movq	-72(%rbp), %rax
	leaq	-16(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	gen_infix, .-gen_infix
	.section	.rodata
.LC12:
	.string	"Read"
	.text
	.globl	read_id
	.type	read_id, @function
read_id:
.LFB19:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	20(%rbp), %rax
	leaq	.LC3(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rsi
	leaq	.LC12(%rip), %rdi
	call	generate
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	read_id, .-read_id
	.globl	process_id
	.type	process_id, @function
process_id:
.LFB20:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	token_buffer(%rip), %rdi
	call	check_id
	movl	$0, -64(%rbp)
	leaq	-64(%rbp), %rax
	addq	$4, %rax
	leaq	token_buffer(%rip), %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	movq	-72(%rbp), %rax
	movq	-64(%rbp), %rcx
	movq	-56(%rbp), %rbx
	movq	%rcx, (%rax)
	movq	%rbx, 8(%rax)
	movq	-48(%rbp), %rcx
	movq	-40(%rbp), %rbx
	movq	%rcx, 16(%rax)
	movq	%rbx, 24(%rax)
	movq	-32(%rbp), %rdx
	movq	%rdx, 32(%rax)
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L79
	call	__stack_chk_fail@PLT
.L79:
	movq	-72(%rbp), %rax
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	process_id, .-process_id
	.globl	process_literal
	.type	process_literal, @function
process_literal:
.LFB21:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$1, -64(%rbp)
	leaq	-64(%rbp), %rax
	addq	$4, %rax
	movq	%rax, %rdx
	leaq	.LC8(%rip), %rsi
	leaq	token_buffer(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_sscanf@PLT
	movq	-72(%rbp), %rax
	movq	-64(%rbp), %rcx
	movq	-56(%rbp), %rbx
	movq	%rcx, (%rax)
	movq	%rbx, 8(%rax)
	movq	-48(%rbp), %rcx
	movq	-40(%rbp), %rbx
	movq	%rcx, 16(%rax)
	movq	%rbx, 24(%rax)
	movq	-32(%rbp), %rdx
	movq	%rdx, 32(%rax)
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L82
	call	__stack_chk_fail@PLT
.L82:
	movq	-72(%rbp), %rax
	addq	$72, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	process_literal, .-process_literal
	.section	.rodata
.LC13:
	.string	"Write"
	.text
	.globl	write_expr
	.type	write_expr, @function
write_expr:
.LFB22:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	pushq	48(%rbp)
	pushq	40(%rbp)
	pushq	32(%rbp)
	pushq	24(%rbp)
	pushq	16(%rbp)
	call	extract
	addq	$48, %rsp
	leaq	.LC3(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rsi
	leaq	.LC13(%rip), %rdi
	call	generate
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	write_expr, .-write_expr
	.globl	next_token
	.type	next_token, @function
next_token:
.LFB23:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	next_token, .-next_token
	.globl	ident
	.type	ident, @function
ident:
.LFB24:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$88, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$4, %edi
	call	match
	movq	-40(%rbp), %rbx
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	process_id
	movq	-96(%rbp), %rax
	movq	-88(%rbp), %rdx
	movq	%rax, (%rbx)
	movq	%rdx, 8(%rbx)
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, 16(%rbx)
	movq	%rdx, 24(%rbx)
	movq	-64(%rbp), %rax
	movq	%rax, 32(%rbx)
	nop
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L86
	call	__stack_chk_fail@PLT
.L86:
	addq	$88, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	ident, .-ident
	.section	.rodata
.LC14:
	.string	"Syntax error"
	.text
	.globl	syntax_error
	.type	syntax_error, @function
syntax_error:
.LFB25:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	leaq	.LC14(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	syntax_error, .-syntax_error
	.globl	primary
	.type	primary, @function
primary:
.LFB26:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$88, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$0, %eax
	call	next_token
	movl	%eax, -28(%rbp)
	cmpl	$6, -28(%rbp)
	je	.L89
	cmpl	$6, -28(%rbp)
	ja	.L90
	cmpl	$4, -28(%rbp)
	je	.L91
	cmpl	$5, -28(%rbp)
	je	.L92
	jmp	.L90
.L89:
	movl	$6, %edi
	call	match
	movl	$7, %edi
	call	match
	jmp	.L93
.L91:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	ident
	jmp	.L93
.L92:
	movl	$5, %edi
	call	match
	movq	-40(%rbp), %rbx
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	process_literal
	movq	-96(%rbp), %rax
	movq	-88(%rbp), %rdx
	movq	%rax, (%rbx)
	movq	%rdx, 8(%rbx)
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, 16(%rbx)
	movq	%rdx, 24(%rbx)
	movq	-64(%rbp), %rax
	movq	%rax, 32(%rbx)
	jmp	.L93
.L90:
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	syntax_error
	nop
.L93:
	nop
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L94
	call	__stack_chk_fail@PLT
.L94:
	addq	$88, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE26:
	.size	primary, .-primary
	.globl	add_op
	.type	add_op, @function
add_op:
.LFB27:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, %eax
	call	next_token
	movl	%eax, -4(%rbp)
	cmpl	$11, -4(%rbp)
	je	.L96
	cmpl	$12, -4(%rbp)
	jne	.L97
.L96:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	match
	call	process_op
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L98
.L97:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	syntax_error
	nop
.L98:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE27:
	.size	add_op, .-add_op
	.globl	expression
	.type	expression, @function
expression:
.LFB28:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$184, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -136(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	primary
	jmp	.L100
.L101:
	leaq	-116(%rbp), %rax
	movq	%rax, %rdi
	call	add_op
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	primary
	leaq	-192(%rbp), %rax
	movl	-116(%rbp), %edx
	pushq	-32(%rbp)
	pushq	-40(%rbp)
	pushq	-48(%rbp)
	pushq	-56(%rbp)
	pushq	-64(%rbp)
	pushq	-80(%rbp)
	pushq	-88(%rbp)
	pushq	-96(%rbp)
	pushq	-104(%rbp)
	pushq	-112(%rbp)
	movl	%edx, %esi
	movq	%rax, %rdi
	call	gen_infix
	addq	$80, %rsp
	movq	-192(%rbp), %rax
	movq	-184(%rbp), %rdx
	movq	%rax, -112(%rbp)
	movq	%rdx, -104(%rbp)
	movq	-176(%rbp), %rax
	movq	-168(%rbp), %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	movq	-160(%rbp), %rax
	movq	%rax, -80(%rbp)
.L100:
	movl	$0, %eax
	call	next_token
	cmpl	$11, %eax
	je	.L101
	movl	$0, %eax
	call	next_token
	cmpl	$12, %eax
	je	.L101
	movq	-136(%rbp), %rax
	movq	-112(%rbp), %rcx
	movq	-104(%rbp), %rbx
	movq	%rcx, (%rax)
	movq	%rbx, 8(%rax)
	movq	-96(%rbp), %rcx
	movq	-88(%rbp), %rbx
	movq	%rcx, 16(%rax)
	movq	%rbx, 24(%rax)
	movq	-80(%rbp), %rdx
	movq	%rdx, 32(%rax)
	nop
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L102
	call	__stack_chk_fail@PLT
.L102:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE28:
	.size	expression, .-expression
	.globl	id_list
	.type	id_list, @function
id_list:
.LFB29:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$4, %edi
	call	match
	jmp	.L104
.L105:
	movl	$9, %edi
	call	match
	movl	$4, %edi
	call	match
.L104:
	movl	$0, %eax
	call	next_token
	cmpl	$9, %eax
	je	.L105
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE29:
	.size	id_list, .-id_list
	.globl	expr_list
	.type	expr_list, @function
expr_list:
.LFB30:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE30:
	.size	expr_list, .-expr_list
	.globl	statement
	.type	statement, @function
statement:
.LFB31:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, %eax
	call	next_token
	movl	%eax, -100(%rbp)
	cmpl	$4, -100(%rbp)
	je	.L108
	cmpl	$4, -100(%rbp)
	ja	.L109
	cmpl	$2, -100(%rbp)
	je	.L110
	cmpl	$3, -100(%rbp)
	je	.L111
	jmp	.L109
.L108:
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	ident
	movl	$10, %edi
	call	match
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	expression
	pushq	-16(%rbp)
	pushq	-24(%rbp)
	pushq	-32(%rbp)
	pushq	-40(%rbp)
	pushq	-48(%rbp)
	pushq	-64(%rbp)
	pushq	-72(%rbp)
	pushq	-80(%rbp)
	pushq	-88(%rbp)
	pushq	-96(%rbp)
	call	assign
	addq	$80, %rsp
	movl	$8, %edi
	call	match
	jmp	.L112
.L110:
	movl	$2, %edi
	call	match
	movl	$6, %edi
	call	match
	call	id_list
	movl	$7, %edi
	call	match
	movl	$8, %edi
	call	match
	jmp	.L112
.L111:
	movl	$3, %edi
	call	match
	movl	$6, %edi
	call	match
	call	expr_list
	movl	$7, %edi
	call	match
	movl	$8, %edi
	call	match
	jmp	.L112
.L109:
	movl	-100(%rbp), %eax
	movl	%eax, %edi
	call	syntax_error
	nop
.L112:
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L113
	call	__stack_chk_fail@PLT
.L113:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE31:
	.size	statement, .-statement
	.globl	statement_list
	.type	statement_list, @function
statement_list:
.LFB32:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	statement
.L118:
	movl	$0, %eax
	call	next_token
	subl	$2, %eax
	cmpl	$2, %eax
	ja	.L119
	call	statement
	nop
	jmp	.L118
.L119:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE32:
	.size	statement_list, .-statement_list
	.globl	program
	.type	program, @function
program:
.LFB33:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	start
	movl	$0, %edi
	call	match
	call	statement_list
	movl	$1, %edi
	call	match
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE33:
	.size	program, .-program
	.globl	system_global
	.type	system_global, @function
system_global:
.LFB34:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	program
	movl	$13, %edi
	call	match
	call	finish
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE34:
	.size	system_global, .-system_global
	.section	.rodata
.LC15:
	.string	"Hello World"
	.text
	.globl	main
	.type	main, @function
main:
.LFB35:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC15(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE35:
	.size	main, .-main
	.local	max_temp.2706
	.comm	max_temp.2706,4,4
	.local	tempname.2707
	.comm	tempname.2707,33,32
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
