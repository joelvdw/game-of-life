# MIT License
# 
# Copyright (c) 2018-2022 VON DER WEID Joël
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

CC=gcc -std=c11
CFLAGS=-Wall -Wextra
EXEC=lifegame

all: main.o display.o file.o automata.o
	$(CC) -o $(EXEC) $^ -lSDL -lSDLmain -lSDL_ttf

main.o: main.c display.h file.h automata.h
	$(CC) $(CFLAGS) -c $<

display.o: display.c display.h math.h file.h
	$(CC) $(CFLAGS) -c $< -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT 

file.o: file.c file.h
	$(CC) $(CFLAGS) -c $<

automata.o: automata.c automata.h file.h
	$(CC) $(CFLAGS) -c $<


.PHONY: clean mrproper all

clean:	
	rm -f *.o $(EXEC)

mrproper: clean
	rm -fr $(EXEC)
