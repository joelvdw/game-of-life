/*
 * Title    : Game of life / file
 * Desc     : Headers for file and array management for the Game of Life
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

#ifndef _FILE_H_
#define _FILE_H_

#define DEFAULT_SIZE 20

/**
 * Alocate memory for a 2d array
 */
char** allocArray(int size);
/**
 * Free memory of a 2d array
 */
void freeArray(char** array, int size);
/*
 * Generate the board from the given file
 * Board max size can be given, 0 to doesn't set it
 */
char** getBoard(char* filename, int* size);
/**
 * Save a board to a file
 */
void saveBoard(char** board, int size);

#endif