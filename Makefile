all: to_grayscale.o
	gcc -Wall -o to_grayscale to_grayscale.o main.c

to_grayscale.o:
	nasm -f elf64 to_grayscale.asm