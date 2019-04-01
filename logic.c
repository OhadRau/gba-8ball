#include "logic.h"
#include <stdlib.h>

#define FRICTION 1
/* TODO: Optimize this. We can get it to
   constant time by a rough approximation
   (like FISR) or by using a lookup table
   if we can constrain these values to a
   given range and round as needed */
fixed_t fixed_sqrt(fixed_t f) {
    if (f < INT_TO_FIXED(2)) {
        return f;
    }
    fixed_t next = 2 * fixed_sqrt(f >> 2);
    if (FIXED_MULT((next + FIXED_ONE), (next + FIXED_ONE)) > f) {
        return next;
    } else {
        return next + FIXED_ONE;
    }
}

/*
// IDK Why, but this works only for 4 bits of fixed point :^(

static fixed_t fixed_sqrt(fixed_t f) {
    if (f < 0) {
        f = -f;
    }

    // Initial guess: f/2
    fixed_t guess = f >> 1;

    for (int iter = 0; iter < 10; iter++) {
        fixed_t err = FIXED_DIV(f - FIXED_MULT(guess, guess), guess << 1);
        guess += err;
    };

    return guess;
}*/

fixed_t fixed_sin(fixed_t f) {
    fixed_t less180 = INT_TO_FIXED(180) - f;
    fixed_t num = FIXED_MULT(4 * f, less180);
    fixed_t denom = INT_TO_FIXED(40500) - FIXED_MULT(f, less180);
    return FIXED_DIV(num, denom);
}

fixed_t fixed_cos(fixed_t f) {
    fixed_t f_squared = FIXED_MULT(f, f);
    fixed_t pi_squared = INT_TO_FIXED(32400);
    fixed_t num = pi_squared - (4 * f_squared);
    fixed_t denom = pi_squared + f_squared;
    return FIXED_DIV(num, denom);
}

static int check_collision(ball_t *a, ball_t *b) {
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;

    fixed_t max_dist = a->radius + b->radius;

    // Calculating the sqrt is slow, so try to rule out false collisions early
    if (dx > max_dist || dy > max_dist) {
        return 0;
    }

    fixed_t d = fixed_sqrt(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));
    
    return d <= max_dist;
}

static void update_ball(ball_t *ball) {
    ball->x += ball->vx;
    ball->y += ball->vy;

    // Remember, velocities are fixed point
    if (ball->vx < 2 && ball->vx > -2) {
        ball->vx = 0;
    }
    if (ball->vy < 2 && ball->vy > -2) {
        ball->vy = 0;
    }

    // vx, vy decay from friction
    ball->vx = FIXED_MULT(ball->vx, FIXED_ONE - FRICTION);
    ball->vy = FIXED_MULT(ball->vy, FIXED_ONE - FRICTION);
}

static void collide(ball_t *a, ball_t *b) {
    // Delta (distance) between balls
    fixed_t dx = a->x - b->x;
    fixed_t dy = a->y - b->y;
    fixed_t d = fixed_sqrt(FIXED_MULT(dx, dx) + FIXED_MULT(dy, dy));

    // Normal to collision
    // Division is slow, can we get rid of this?
    fixed_t nx = FIXED_DIV(-dx, d);
    fixed_t ny = FIXED_DIV(-dy, d);

    // Tangent
    fixed_t tx = -ny;
    fixed_t ty = nx;

    fixed_t dpTanA = FIXED_MULT(a->vx, tx) + FIXED_MULT(a->vy, ty);
    fixed_t dpTanB = FIXED_MULT(b->vx, tx) + FIXED_MULT(b->vy, ty);

    fixed_t dpNormA = FIXED_MULT(a->vx, nx) + FIXED_MULT(a->vy, ny);
    fixed_t dpNormB = FIXED_MULT(b->vx, nx) + FIXED_MULT(b->vy, ny);

    // We don't need to calculate momentum since mass = 1 for both balls. mA, mB = dpNormB, dpNormA
    a->vx = FIXED_MULT(tx, dpTanA) + FIXED_MULT(nx, dpNormB);
    a->vy = FIXED_MULT(ty, dpTanA) + FIXED_MULT(ny, dpNormB);
    
    b->vx = FIXED_MULT(tx, dpTanB) + FIXED_MULT(nx, dpNormA);
    b->vy = FIXED_MULT(ty, dpTanB) + FIXED_MULT(ny, dpNormA);
}

void initializeAppState(AppState *appState) {
    appState->gameOver = 0;

    cue_t *cue = malloc(sizeof(cue_t));
    cue->color = YELLOW;
    cue->length = INT_TO_FIXED(50);
    cue->angle = INT_TO_FIXED(0);
    cue->x = INT_TO_FIXED(50);
    cue->y = INT_TO_FIXED(0);
    appState->cue = cue;

    ball_t *cue_ball = malloc(sizeof(ball_t));
    cue_ball->color = WHITE;
    cue_ball->radius = INT_TO_FIXED(5);
    cue_ball->x = INT_TO_FIXED(50);
    cue_ball->y = INT_TO_FIXED(50);
    cue_ball->vx = INT_TO_FIXED(0);
    cue_ball->vy = INT_TO_FIXED(0);
    appState->cue_ball = cue_ball;

    ball_t *other = malloc(sizeof(ball_t));
    other->color = BLUE;
    other->radius = INT_TO_FIXED(5);
    other->x = INT_TO_FIXED(120);
    other->y = INT_TO_FIXED(55);
    other->vx = INT_TO_FIXED(0);
    other->vy = INT_TO_FIXED(0);
    appState->other = other;
}

void cleanupAppState(AppState *appState) {
    free(appState->cue_ball);
    free(appState->other);
    free(appState->cue);
    // Uh... apparently the appState is on the stack oof
    //free(appState);
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */
    AppState nextAppState = *currentAppState;

    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);

    // Copy the balls into the new state (without reusing the same pointer)
    nextAppState.cue_ball = malloc(sizeof(ball_t));
    *(nextAppState.cue_ball) = *(currentAppState->cue_ball);
    nextAppState.other = malloc(sizeof(ball_t));
    *(nextAppState.other) = *(currentAppState->other);
    nextAppState.cue = malloc(sizeof(cue_t));
    *(nextAppState.cue) = *(currentAppState->cue);

    // If nothing is moving
    if (nextAppState.cue_ball->vx == 0 && nextAppState.cue_ball->vy == 0) {
        // Cue mode!
        nextAppState.cue->x = nextAppState.cue->length;//FIXED_MULT(nextAppState.cue->length, fixed_cos(nextAppState.cue->angle));
        nextAppState.cue->y = nextAppState.cue->length;//FIXED_MULT(nextAppState.cue->length, fixed_sin(nextAppState.cue->angle));

        if (KEY_DOWN(ANY_KEY, keysPressedNow)) {
            nextAppState.cue->angle += FIXED_ONE;
        }
    } else {
        // Just calculate motion
        update_ball(nextAppState.cue_ball);
        update_ball(nextAppState.other);

        if (check_collision(nextAppState.cue_ball, nextAppState.other)) {
            collide(nextAppState.cue_ball, nextAppState.other);
        }
    }

    return nextAppState;
}
