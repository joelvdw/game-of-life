/*
 * Title    : Game of life
 * Desc     : C implementation of the Conway's Game of Life cellular automaton
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
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include "display.h"
#include "board.h"
#include "automata.h"

#define MAIN_WAIT 10
#define MIN_GEN_WAIT 16

/**
 * Print an error and exit
 */
void errorExit(char* msg) {
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
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

/**
 * Manage arguments
 */
void manageArguments(int argc, char** argv, int* size, char** file, int* rand) {
    if (argc != 1 && argc != 3 && argc != 5 && argc != 7) {
        // Show help
        printf("Usage : lifegame [-h] [-n <size>] [-f <file>] [-r <type>]\n");
        printf("         -h          Display this help page\n");
        printf("         -f <file>   Load a board from a file\n");
        printf("                     The first line must be the board size\n");
        printf("         -n <size>   Define the size of the board (the board is always a square)\n");
        printf("         -r <type>   Randomly generate the board\n");
        printf("                     Types : 1 - Full random\n");
        printf("                             2 - Random with a vertical symmetry\n");
        printf("                             3 - Random with a horizontal symmetry\n");
        printf("                             4 - Random with both symmetries\n");
        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i < argc; i++) {
        // size
        if (!strcmp(argv[i], "-n")) {
            if (i+1 < argc) {
                *size = atoi(argv[i+1]);
                if (*size <= 0) {
                    errorExit("Invalid arguments");
                }
            } else {
                errorExit("Invalid arguments");
            }
        }
        // file
        else if (!strcmp(argv[i], "-f")) {
            if (i+1 < argc) {
                *file = argv[i+1];
            } else {
                errorExit("Invalid arguments");
            }
        }
        // random
        else if (!strcmp(argv[i], "-r")) {
            if (i+1 < argc) {
                *rand = atoi(argv[i+1]);
            } else {
                errorExit("Invalid arguments");
            }
        }
    }
}

/*
 * Calculate next board state
 */
board_t nextState(board_t board, int size) {
    board_t nBoard = allocArray(size);

    calculateState(board, nBoard);

    freeArray(board);
    return nBoard;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int size = 0;
    int random = 0;
    char* file = "";

    manageArguments(argc, argv, &size, &file, &random);

    // Create board
    board_t board = getBoard(file, &size);
    initScreen(size);
    if (random > 0) {
        randomBoard(board, random);
    }
    
    // Main loop
    int quit = 0;
    int running = 0;
    int elapsed = 0;
    int wait = 500;
    int generation = 0;
    SDL_Event event;
    int mouseDown = 0;
    int keyDown = 0;

    updateScreen(board);
    updateTexts(running, wait, generation);

    while (!quit) {
        Uint32 tmpTicks = SDL_GetTicks();

        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                // Fill or empty a cell
                if (!mouseDown && !running) {
                    Point p = getPointFromScreen(event.button.x, event.button.y, size);
                    if (p.i == -2) {
                        // -2 is button pressed
                        saveBoard(board, size);
                    } else if (p.i > -1) {
                        board.data[idx(p.i, p.j, size)] = !board.data[idx(p.i, p.j, size)];
                    }
                    updateScreen(board);
                    updateTexts(running, wait, generation);

                    mouseDown = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                mouseDown = 0;
                break;
            case SDL_KEYDOWN:
                // space: run, up/down: speed, right: next state
                if (!keyDown) {
                    switch(event.key.keysym.sym) {
                        case SDLK_SPACE:
                            running = !running;
                            updateTexts(running, wait, generation);
                            break;
                        case SDLK_UP:
                            wait /= 2;
                            if (wait < MIN_GEN_WAIT) {
                                wait = MIN_GEN_WAIT;
                            }
                            updateTexts(running, wait, generation);
                            break;
                        case SDLK_DOWN:
                            wait *= 2;
                            updateTexts(running, wait, generation);
                            break;
                        case SDLK_RIGHT:
                            if (!running) {
                                board = nextState(board, size);
                                generation += 1;
                                updateScreen(board);
                                updateTexts(running, wait, generation);
                            }
                            break;
                        default:
                            break;
                    }

                    keyDown = 1;
                }
                break;
            case SDL_KEYUP:
                keyDown = 0;
                break;
        }

        // Play life game
        if (running && elapsed >= wait && !quit) {
            board = nextState(board, size);
            generation += 1;
            updateScreen(board);
            updateTexts(running, wait, generation);

            elapsed = 0;
        }

        // Main wait
        int spend = (SDL_GetTicks() - tmpTicks);
        if (spend < MAIN_WAIT) {
            SDL_Delay(MAIN_WAIT - spend);
        }
        elapsed += MAIN_WAIT;
    }

    // Free all memory
    freeArray(board);
    closeScreen();

    exit(EXIT_SUCCESS);
}