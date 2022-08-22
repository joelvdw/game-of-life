/*
 * Title    : Game of life / display
 * Desc     : Display management for the Game of Life
 * Author   : Joël von der Weid
 * Date     : December 2018
 * Version  : 0.2

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
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "display.h"
#include "math.h"

#define TEXT_MARGIN 18
#define LINE_MARGIN 28
#define TEXT_SIZE 200
#define BTN_HEIGHT 40
#define CELL_MAX_SIZE 30
#define CELL_MIN_SIZE 2
#define WINDOW_MIN_SIZE 300
#define BORDER 2
#define MARGIN 1
#define PALETTE_SIZE 256
#define CURSOR_SIZE 0
#define HEADER_SIZE 150
   
SDL_Surface *screen = NULL;
int cellSize = 0;
int screenSize = 0;
SDL_Rect btnRect;

SDL_Color colors[PALETTE_SIZE];
TTF_Font* fontS;
TTF_Font* fontM;
TTF_Font* fontL;

Uint32 cellColor;
Uint32 emptyColor;
Uint32 backColor;
Uint32 btnColor;
Uint32 borderColor;

/**
 * Convert the screen coordinate to the array indexes
 * Return -1 if outOfBound, -2 if button pressed
 */
Point getPointFromScreen(int x, int y, int size) {
    Point p = {0, 0};

    // Button
    if (x >= btnRect.x && x <= (btnRect.x+btnRect.w)
        && y >= btnRect.y && y <= (btnRect.y+btnRect.h)) {
        p.i = -2;
        p.j = -2;
        return p;
    }

    p.i = y / (cellSize + MARGIN);
    p.j = x / (cellSize + MARGIN);

    if (p.i >= size || p.j >= size) {
        p.i = -1;
        p.j = -1;
    }

    return p;
}

/**
 * Initialize the grayscale color palette, the needed colors and the fonts
 */
void initVariables() {
    for (int i = 0; i < PALETTE_SIZE; i++) {
        colors[i].r = i;
        colors[i].g = i;
        colors[i].b = i;
    }

    fontS = TTF_OpenFont("arial.ttf", 23);
    fontM = TTF_OpenFont("arial.ttf", 27);
    fontL = TTF_OpenFont("arial.ttf", 30);

    backColor = SDL_MapRGB(screen->format, 200, 200, 200);
    cellColor = SDL_MapRGB(screen->format, 20, 20, 20);
    emptyColor = SDL_MapRGB(screen->format, 235, 235, 235);
    borderColor = SDL_MapRGB(screen->format, 120, 120, 120);
    btnColor = SDL_MapRGB(screen->format, 205, 205, 205);
}

/**
 * Choose the optimal cell size depending on the board size
 */
void setCellSize(int size) {
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    int width = info->current_w;
    int height = info->current_h;
    int mins = min(height - HEADER_SIZE, width);

    cellSize = min(max(mins / size, CELL_MIN_SIZE), CELL_MAX_SIZE);
}

/**
 * Initialize the display window
 */
void initScreen(int size) {
    // Create screen
    SDL_Init(SDL_INIT_VIDEO);
    setCellSize(size);

    screenSize = size * (cellSize + MARGIN) + MARGIN;
    int windowHeight = max(screenSize, WINDOW_MIN_SIZE);
    screen = SDL_SetVideoMode(screenSize + TEXT_SIZE, windowHeight, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Game of Life", NULL);

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "TTF_Init error : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    initVariables();
    SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, PALETTE_SIZE);

    // Display initial components
    SDL_FillRect(screen, NULL, backColor);

    SDL_Rect rect;
    rect.x = screenSize;
    rect.y = 0;
    rect.w = TEXT_SIZE;
    rect.h = windowHeight;
    SDL_FillRect(screen, &rect, backColor);

    rect.x = screenSize + TEXT_MARGIN;
    rect.y = windowHeight - TEXT_MARGIN - BTN_HEIGHT;
    rect.w = TEXT_SIZE - 2*TEXT_MARGIN;
    rect.h = BTN_HEIGHT;
    btnRect = rect;
    SDL_FillRect(screen, &rect, borderColor);
    rect.x += BORDER;
    rect.y += BORDER;
    rect.w -= 2*BORDER;
    rect.h -= 2*BORDER;
    SDL_FillRect(screen, &rect, btnColor);
    rect.y += (BTN_HEIGHT - BORDER*2) / 2 - LINE_MARGIN / 2;
    rect.x += (TEXT_SIZE - 2*(BORDER+TEXT_MARGIN)) / 8;
    SDL_Surface* text = TTF_RenderText_Solid(fontS, "Save board", colors[10]);
    SDL_BlitSurface(text, NULL, screen, &rect);

    rect.x = screenSize + TEXT_MARGIN;
    rect.y = TEXT_MARGIN;
    SDL_Surface* text1 = TTF_RenderText_Solid(fontS, "Generation", colors[75]);
    SDL_BlitSurface(text1, NULL, screen, &rect);
    rect.y += 3 * LINE_MARGIN;

    SDL_Surface* text2 = TTF_RenderText_Solid(fontS, "Delay", colors[75]);
    SDL_BlitSurface(text2, NULL, screen, &rect);

    SDL_FreeSurface(text);
    SDL_FreeSurface(text1);
    SDL_FreeSurface(text2);
}

/**
 * Update the screen with the current game state
 */
void updateScreen(char** board, int size) {
    SDL_Rect position;
    position.x = MARGIN;
    position.y = MARGIN;
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            SDL_Surface* cell = SDL_CreateRGBSurface(SDL_HWSURFACE, cellSize, cellSize, 8, 0, 0, 0, 0);
            SDL_SetPalette(cell, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, PALETTE_SIZE);
            SDL_FillRect(cell, NULL, (board[i][j] == 0 ? emptyColor: cellColor));
            SDL_BlitSurface(cell, NULL, screen, &position);

            SDL_FreeSurface(cell);
            position.x += cellSize + MARGIN;
        }
        position.y += cellSize + MARGIN;
        position.x = MARGIN;
    }

    SDL_Flip(screen);
}

/**
 * Update the texts in the window
 */
void updateTexts(int running, int wait, int generation) {
    SDL_Rect rect;
    rect.x = screenSize;
    rect.y = TEXT_MARGIN + LINE_MARGIN;
    rect.w = TEXT_SIZE;
    rect.h = LINE_MARGIN;
    SDL_FillRect(screen, &rect, backColor);

    rect.y += 3 * LINE_MARGIN;
    rect.h = 4 * LINE_MARGIN;
    SDL_FillRect(screen, &rect, backColor);

    SDL_Rect position;
    position.x = screenSize + TEXT_MARGIN;
    position.y = TEXT_MARGIN + LINE_MARGIN;

    char str[15];
    sprintf(str, "%d", generation);
    SDL_Surface* text = TTF_RenderText_Solid(fontM, str, colors[0]);
    SDL_BlitSurface(text, NULL, screen, &position);
    position.y += 3 * LINE_MARGIN;

    sprintf(str, "%dms", wait);
    SDL_Surface* text1 = TTF_RenderText_Solid(fontM, str, colors[0]);
    SDL_BlitSurface(text1, NULL, screen, &position);
    position.y += 2 * LINE_MARGIN;

    if (!running) {
        SDL_Surface* text2 = TTF_RenderText_Solid(fontL, "PAUSE", colors[100]);
        SDL_BlitSurface(text2, NULL, screen, &position);
        SDL_FreeSurface(text2);
    }

    SDL_Flip(screen);
    SDL_FreeSurface(text);
    SDL_FreeSurface(text1);
}

/**
 * Close the window and free memory
 */
void closeScreen() {
    TTF_CloseFont(fontL);
    TTF_CloseFont(fontM);
    TTF_CloseFont(fontS);
    TTF_Quit();
    SDL_Quit();
}