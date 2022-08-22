/*
 * Title    : Game of life / file
 * Desc     : File and array management for the Game of Life
 * Author   : Joël von der Weid - HEPIA ISC
 * Date     : August 2022
 * Version  : 0.5
  
MIT License

Copyright (c) 2018-2022 VON DER WEID Joël

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
 * Return the corresponding index in the flattened 2d-array
 */
int idx(int i, int j, int size) {
    return (i*size) + j;
}

/*
 * Alocate memory for a flat 2d-array of size (size x size)
 */
char* allocArray(int size) {
    char* array = calloc(sizeof(char), size * size);
    assert(array != NULL);
    
    return array;
}

/*
 * Free memory of a flat 2d-array
 */
void freeArray(char* array) {
    free(array);
}

/*
 * Generate the board with the given file
 * Board size can be given, 0 to doesn't set it
 */
char* getBoard(char* filename, int* size) {
    char* board;

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
                    board[idx(i, j, *size)] = 0;
                } else {
                    board[idx(i, j, *size)] = 1;
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
void saveBoard(char* board, int size) {
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
    for(int i = 0; i < (size*size); i++) {
        fprintf(file, "%d", board[i]);
        if (i % size == (size-1)) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}