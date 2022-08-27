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
#include "board.h"

#define MIN_SIZE 3

/*
 * Return the corresponding index in the flattened 2d-array
 */
int idx(int i, int j, int size) {
    return (i*size) + j;
}

/*
 * Alocate memory for a board of size (size x size)
 */
board_t allocBoard(int size) {
    board_t board;
    board.size = size;
    board.data = calloc(sizeof(char), size * size);
    assert(board.data != NULL);
    
    return board;
}

/*
 * Free memory of a board
 */
void freeBoard(board_t board) {
    free(board.data);
}

/*
 * Generate the board with the given file
 * Board size can be given, 0 to doesn't set it
 */
board_t getBoard(char* filename, int* size) {
    board_t board;

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
    
        board = allocBoard(*size);

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
                    board.data[idx(i, j, *size)] = 0;
                } else {
                    board.data[idx(i, j, *size)] = 1;
                }

                j += 1;
            }
        }
    
        fclose(file);
    } else {
        if (*size == 0) {
            *size = DEFAULT_SIZE;
        }
        board = allocBoard(*size);
    }

    return board;
}

/**
 * Save a board to a file
 */
void saveBoard(board_t board, int size) {
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
        fprintf(file, "%d", board.data[i]);
        if (i % size == (size-1)) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
}


/**
 * Randomly generate a part of a board
 * Range to generate : (initI -> initI+size; initJ -> initJ+size)
 */
void randomBoardPart(board_t board, int initI, int initJ, int partSize) {
    for (int i = initI; i < initI+partSize; i++) {
        for (int j = initJ; j < initJ+partSize; j++) {
            board.data[idx(i, j, board.size)] = rand()%2;
        }
    }
}

/**
 * Apply a vertical or horizontal symmetry on the middle of the board
 */
void symmetryBoardPart(board_t board, int vertical) {
    for (int i = 0; i < (vertical ? board.size/2 : board.size); i++) {
        int nI =  (vertical ? board.size - i - 1 : i);
        for (int j = 0; j < (vertical ? board.size : board.size/2); j++) {
            int nJ =  (vertical ? j : board.size - j - 1);

            board.data[idx(nI, nJ, board.size)] = board.data[idx(i, j, board.size)];
        }
    }
}

/**
 * Fill the board with random values
 * rdmType : 1->full random, 2->vertical symm, 3->horizontal symm, 4->both symm
 */
void randomBoard(board_t board, int rdmType) {
    int size = board.size;
    if (rdmType == 1) {
        randomBoardPart(board, 0, 0, size);
    } else {
        randomBoardPart(board, 0, 0, size/2 + size%2);
        if (rdmType == 2) {
            randomBoardPart(board, 0, size/2+size%2, size/2);
            symmetryBoardPart(board, 1);
        } else if (rdmType == 3) {
            randomBoardPart(board, size/2+size%2, 0, size/2);
            symmetryBoardPart(board, 0);
        } else {
            symmetryBoardPart(board, 1);
            symmetryBoardPart(board, 0);
        }
    }
}
