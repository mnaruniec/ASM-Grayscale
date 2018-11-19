cflags= -Wall -g -no-pie

to_grayscale: to_grayscale.o main.c
	gcc ${cflags} -o $@ $^

to_grayscale.o: to_grayscale.asm
	nasm -f elf64 -F dwarf $^ -o $@

clean:
	-rm *.o