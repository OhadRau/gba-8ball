#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"
#include "myLib.h"

// Friction coefficient (V' = (1 - FRICTION) * V)
#define FRICTION 8

#define ENTITY_ALIVE 1
#define ENTITY_DEAD 0

typedef struct {
    fixed_t radius;
    fixed_t x, y;
    fixed_t vx, vy;
    int alive;
} ball_t;

#define MAX_CUE_STRENGTH INT_TO_FIXED(15)
#define MAX_CUE_DISTANCE INT_TO_FIXED(40)

typedef struct {
    fixed_t angle;
    fixed_t dist_from_ball;
    int alive;
} cue_t;

// Different statuses for gameOver so the main loops knows what screen to show the user
#define GAME_OVER_WIN 1
#define GAME_OVER_LOSS 2

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int score;
    int turns;

    cue_t *cue;

    ball_t *cue_ball;
    ball_t *balls[15];
} AppState;

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// Free all dynamically allocated memory from the app state
void cleanupAppState(AppState *state);

// This function will be used to process app frames.
void processAppState(AppState *state, u32 keysPressedBefore, u32 keysPressedNow);

#endif
