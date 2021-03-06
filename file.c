/*
 * Title    : Game of life / file
 * Desc     : File and array management for the Game of Life
 * Author   : Joël von der Weid - HEPIA ITI
 * Date     : December 2018
 * Version  : 0.1
 
MIT License

Copyright (c) 2018-2019 VON DER WEID Joël

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "file.h"

#define MIN_SIZE 3

/*
 * Alocate memory for a 2d array
 */
char** allocArray(int size) {
    char** array = malloc(sizeof(char*) * size);
    assert(array != NULL);
    for (int i = 0; i < size; i++) {
        array[i] = malloc(sizeof(char) * size);
        assert(array[i] != NULL);

        for (int j = 0; j < size; j++) {
            array[i][j] = 0;
        }
    }

    return array;
}

/*
 * Free memory of a 2d array
 */
void freeArray(char** array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }

    free(array);
}

/*
 * Generate the board with the given file
 * Board size can be given, 0 to doesn't set it
 */
char** getBoard(char* filename, int* size) {
    char** board;

    if (strcmp(filename, "")) {
        FILE* file = fopen(filename, "r");
        assert(file != NULL);

        // Get size from the first line
        char line[10];
        fgets(line, 10, file);
        line[strlen(line)-1] = 0;

        if (*size == 0) {
            *size = atoi(line);
        }
        if (*size < MIN_SIZE) {
            *size = MIN_SIZE;
        }
    
        board = allocArray(*size);

        // Fill the board with the file content
        int i = 0;
        int j = 0;
        char c = 0;
        while((c = fgetc(file)) != EOF) {
            if (c == '\n') {
                i += 1;
                j = 0;
                if (i == *size) {
                    break;
                }
            } else if (j < *size) {
                if (c == '0') {
                    board[i][j] = 0;
                } else {
                    board[i][j] = 1;
                }

                j += 1;
            }
        }
    
        fclose(file);
    } else {
        if (*size == 0) {
            *size = DEFAULT_SIZE;
        }
        board = allocArray(*size);
    }

    return board;
}

/**
 * Save a board to a file
 */
void saveBoard(char** board, int size) {
    // Get formatted time for the filename
    time_t now;
    char buffer[29];
    struct tm* tm_info;

    time(&now);
    tm_info = localtime(&now);
    strftime(buffer, 29, "saves_%Y%m%d%H%M%S.txt", tm_info);

    FILE* file = fopen(buffer, "w");
    assert(file != NULL);

    fprintf(file, "%d\n", size);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            fprintf(file, "%d", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}