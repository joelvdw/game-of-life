/*
 * Title    : Game of life / file
 * Desc     : Headers for file and array management for the Game of Life
 * Author   : Joël von der Weid - HEPIA ITI
 * Date     : December 2018
 * Version  : 0.1
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