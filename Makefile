@all:
	@gcc main.c src/*.c -Iinclude/ -o chip8 -ansi -Wall

clean:
	@rm -f chip8 chip8.exe