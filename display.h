/*
 * Title    : Game of life / display
 * Desc     : Headers for display management for the Game of Life
 * Author   : Joël von der Weid - HEPIA ITI
 * Date     : December 2018
 * Version  : 0.2
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

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
void updateScreen(char** board, int size);
/**
 * Update the texts in the window
 */
void updateTexts(int running, int wait, int generation);
/**
 * Close the window and free memory
 */
void closeScreen();

#endif