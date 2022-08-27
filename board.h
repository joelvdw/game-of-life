/*
 * Title    : Game of life / file
 * Desc     : Headers for file and array management for the Game of Life
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

#ifndef _FILE_H_
#define _FILE_H_

#define DEFAULT_SIZE 20

typedef struct board {
    char* data;
    int size;
} board_t;

/*
 * Return the corresponding index in the flattened 2d-array
 */
int idx(int i, int j, int size);
/*
 * Alocate memory for a flat 2d-array of size (size x size)
 */
board_t allocBoard(int size);
/*
 * Free memory of a flat 2d-array
 */
void freeBoard(board_t array);
/*
 * Generate the board with the given file
 * Board size can be given, 0 to doesn't set it
 */
board_t getBoard(char* filename, int* size);
/**
 * Save a board to a file
 */
void saveBoard(board_t board, int size);

/**
 * Randomly generate a part of a board
 * Range to generate : (initI -> initI+size; initJ -> initJ+size)
 */
void randomBoardPart(board_t board, int initI, int initJ, int partSize);

/**
 * Apply a vertical or horizontal symmetry on the middle of the board
 */
void symmetryBoardPart(board_t board, int vertical);

/**
 * Fill the board with random values
 * rdmType : 1->full random, 2->vertical symm, 3->horizontal symm, 4->both symm
 */
void randomBoard(board_t board, int rdmType);

#endif