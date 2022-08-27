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

#define MAIN_WAIT 5
#define MIN_GEN_WAIT 16

typedef struct game_state {
    board_t currBoard;
    board_t nextBoard;
    int generation;
} game_state_t;

/**
 * Print an error and exit
 */
void errorExit(char* msg) {
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
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
void updateState(game_state_t* state) {
    calculateState(state->currBoard, state->nextBoard);
    
    board_t tmp = state->currBoard;
    state->currBoard = state->nextBoard;
    state->nextBoard = tmp;
    state->generation += 1;
}

void guiLoop(game_state_t state) {
    int quit = 0;
    int running = 0;
    int elapsed = 0;
    int wait = 500;
    SDL_Event event;
    int mouseDown = 0;
    int keyDown = 0;

    updateScreen(state.currBoard);
    updateTexts(running, wait, state.generation);

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
                    Point p = getPointFromScreen(event.button.x, event.button.y, state.currBoard.size);
                    if (p.i == -2) {
                        // -2 is button pressed
                        saveBoard(state.currBoard, state.currBoard.size);
                    } else if (p.i > -1) {
                        
                        state.currBoard.data[idx(p.i, p.j, state.currBoard.size)] = !state.currBoard.data[idx(p.i, p.j, state.currBoard.size)];
                    }
                    updateScreen(state.currBoard);
                    updateTexts(running, wait, state.generation);

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
                            updateTexts(running, wait, state.generation);
                            break;
                        case SDLK_UP:
                            wait /= 2;
                            if (wait < MIN_GEN_WAIT) {
                                wait = MIN_GEN_WAIT;
                            }
                            updateTexts(running, wait, state.generation);
                            break;
                        case SDLK_DOWN:
                            wait *= 2;
                            updateTexts(running, wait, state.generation);
                            break;
                        case SDLK_RIGHT:
                            if (!running) {
                                updateState(&state);
                                updateScreen(state.currBoard);
                                updateTexts(running, wait, state.generation);
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
            updateState(&state);
            updateScreen(state.currBoard);
            updateTexts(running, wait, state.generation);

            elapsed = 0;
        }

        // Main wait
        int spend = (SDL_GetTicks() - tmpTicks);
        if (spend < MAIN_WAIT) {
            SDL_Delay(MAIN_WAIT - spend);
        }
        elapsed += MAIN_WAIT;
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int size = 0;
    int random = 0;
    char* file = "";

    manageArguments(argc, argv, &size, &file, &random);

    // Create board
    board_t board1 = getBoard(file, &size);
    board_t board2 = allocBoard(size);
    initScreen(size);
    if (random > 0) {
        randomBoard(board1, random);
    }
    game_state_t state = { board1, board2, 0 };
    
    // Main loop
    guiLoop(state);

    // Free all memory
    freeBoard(state.currBoard);
    freeBoard(state.nextBoard);
    closeScreen();

    exit(EXIT_SUCCESS);
}
