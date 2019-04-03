#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"

#define FIXED_RESOLUTION 8

#define FIXED_TO_INT(F) ((F) >> FIXED_RESOLUTION)
#define INT_TO_FIXED(I) ((I) << FIXED_RESOLUTION)
#define FIXED_MULT(A, B) (((A) * (B)) >> FIXED_RESOLUTION)
#define FIXED_DIV(A, B) (((fixed_t) (A) * (1 << FIXED_RESOLUTION)) / (B))

#define FIXED_ONE INT_TO_FIXED(1)

typedef int fixed_t;

#define ENTITY_ALIVE 1
#define ENTITY_DEAD 0

#define MAX_CUE_STRENGTH INT_TO_FIXED(20)
#define MAX_CUE_DISTANCE INT_TO_FIXED(40)

typedef struct {
    u16 color;
    fixed_t radius;
    fixed_t x, y;
    fixed_t vx, vy;
    int alive;
} ball_t;

typedef struct {
    u16 color;
    fixed_t angle;
    fixed_t dist_from_ball;
    int alive;
} cue_t;

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    cue_t *cue;

    ball_t *cue_ball;
    ball_t *balls[15];
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
