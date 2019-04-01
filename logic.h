#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"

#define FIXED_RESOLUTION 4

#define FIXED_TO_INT(F) ((F) >> FIXED_RESOLUTION)
#define INT_TO_FIXED(I) ((I) << FIXED_RESOLUTION)
#define FIXED_MULT(A, B) (((A) * (B)) >> FIXED_RESOLUTION)
#define FIXED_DIV(A, B) (((fixed_t) A * (1 << FIXED_RESOLUTION)) / B)

#define FIXED_ONE INT_TO_FIXED(1)

typedef int fixed_t;

typedef struct {
    u16 color;
    fixed_t radius;
    fixed_t x, y;
    fixed_t vx, vy;
} ball_t;

typedef struct {
    u16 color;

    fixed_t length;
    fixed_t angle;

    fixed_t x, y;
} cue_t;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    cue_t *cue;

    ball_t *cue_ball;
    ball_t *other;
} AppState;

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
// This function frees memory used by the AppState
void cleanupAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
