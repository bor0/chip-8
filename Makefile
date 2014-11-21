@all:
	@gcc main.c src/*.c -I/usr/include/SDL/ -Iinclude/ -lSDL_image -lSDL_mixer -lSDL -o chip-8 -ansi -Wall -O2 -pedantic

clean:
	@rm -f chip-8 chip-8.exe
