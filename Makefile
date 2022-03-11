#Compile the compiler
compiler: compiler_micro.c
	gcc -o compiler_micro compiler_micro.c

#Compile & Run the x86 code
x86: x86code.s
	nasm -f elf64 -o x86code.o x86code.s
	ld -o exe x86code.o
	./exe