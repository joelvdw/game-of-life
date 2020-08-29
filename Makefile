CC=gcc -std=c11
CFLAGS=-Wall -Wextra
EXEC=lifegame

all: main.o display.o file.o
	$(CC) -o $(EXEC) $^ -lSDL -lSDLmain -lSDL_ttf

main.o: main.c display.h file.h
	$(CC) $(CFLAGS) -c $<

display.o: display.c
	$(CC) $(CFLAGS) -c $< -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT 

file.o: file.c file.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean mrproper all

clean:	
	rm -f *.o lifegame

mrproper: clean
	rm -fr $(EXEC)
