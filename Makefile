@all:
	@gcc main.c src/*.c -I/usr/include/SDL/ -Iinclude/ -lSDL_image -lSDL_mixer -lSDL -o chip8 -ansi -Wall

clean:
	@rm -f chip8 chip8.exe