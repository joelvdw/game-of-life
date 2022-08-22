/*
 * Title    : Game of life / display
 * Desc     : Headers for display management for the Game of Life
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

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "board.h"

typedef struct Point {
    int i;
    int j;
} Point;

/**
 * Convert the screen coordinate to the array indexes
 * Return -1 if outOfBound, -2 if button pressed
 */
Point getPointFromScreen(int x, int y, int size);
/**
 * Initialize the display window
 */
void initScreen(int size);
/**
 * Update the screen with the current game state
 */
void updateScreen(board_t board);
/**
 * Update the texts in the window
 */
void updateTexts(int running, int wait, int generation);
/**
 * Close the window and free memory
 */
void closeScreen();

#endif