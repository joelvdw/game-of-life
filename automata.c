/*
 * Title    : Game of life / automata
 * Desc     : Intrinsec Game of Life functions
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

#include "file.h"

/**
 * Calculate the next state of the life game
 * Rules : 3 -> born, 2-3 -> survive, else -> die
 */
void calculateState(char* state, char* newState, int size) {
    int i;
    for (i = 0; i < size; i++) {
        int c1 = 0, c2 = 0;
        int c3 = (i > 0 ? state[idx(i-1, 0, size)] : 0) + state[idx(i, 0, size)] + (i+1 < size ? state[idx(i+1, 0, size)] : 0);

        for (int j = 0; j < size; j++) {
            c2 = c3 - state[idx(i, j, size)];
            c3 = 0;
            if (j+1 < size) {
                for (int k = (i-1 > 0 ? i-1 : 0); k < (i+2 < size ? i+2 : size); k++) {
                    c3 += state[idx(k, j+1, size)];
                }
            }

            int nbrCell = c1 + c2 + c3;
            if (nbrCell == 3 || (state[idx(i, j, size)] == 1 && nbrCell == 2)) {
                newState[idx(i, j, size)] = 1;
            } else {
                newState[idx(i, j, size)] = 0;
            }

            c1 = c2 + state[idx(i, j, size)];
        }
    }
}
